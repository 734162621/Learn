import math
import enum
import numpy as np
import gym
from gym.spaces import Box, Discrete

from pyaircombat.plane.simulation import Simulation
from pyaircombat.plane.jsbsim_catalog import JsbsimCatalog as c
from pyaircombat.utils import wsgtoxyz, limitangle
from pyaircombat.missile import Missile, MissileState, MissileType
from pyaircombat.callbacks import GlobalCallback


class PlaneState(enum.Enum):
    Crash = -1
    Running = 0
    Shotdown = 1
    Locked = 2


class Plane:
    plane_action_var = [
        c.fcs_aileron_cmd_norm,
        c.fcs_elevator_cmd_norm,
        c.fcs_rudder_cmd_norm,
        c.fcs_throttle_cmd_norm,
    ]

    def __init__(self, aircraft_name="A320", tag='', max_oppo_num=1,
                 jsbsim_freq=60, agent_interaction_steps=5, callbacks: GlobalCallback = None,
                 missile_run_when_miss=False):
        self.callbacks = callbacks or GlobalCallback()
        self.aircraft_name = aircraft_name
        self.tag = tag if tag else aircraft_name
        self.jsbsim_freq = jsbsim_freq
        self.agent_interaction_steps = agent_interaction_steps
        self.sim = None
        self.sim_step = agent_interaction_steps / jsbsim_freq
        self.init_missiles_num = None
        self.missiles = None
        self.missile_run_when_miss = missile_run_when_miss
        self.max_oppo_num = max_oppo_num
        self.targeted_missiles = []
        self.planes_can_locked = []
        self.running_state = PlaneState.Running
        self.simple_trim = 0
        self.xyz_pos = None
        self.wsg_pos = None
        self.attitude = None
        self.velocity = None

    def simple_trim_on(self):
        self.simple_trim = 1
        self.sim.set_property_value(c.simulation_do_simple_trim, 1)

    def simple_trim_off(self):
        self.simple_trim = 0
        self.sim.set_property_value(c.simulation_do_simple_trim, 0)

    def step(self, action):
        if self.is_alive():
            *plane_action, missile_type, target_idx = action
            missile_type = MissileType(missile_type)
            if self.simple_trim == 0:
                self.sim.set_property_values(self.plane_action_var, plane_action)
            if missile_type != MissileType.none:
                self.__fire_target(missile_type, target_idx)
            # sim step
            self.sim.run()
            # missile step
            if bool(self.sim.get_property_value(c.detect_extreme_state)):
                self.running_state = PlaneState.Crash
                self.callbacks.on_plane_crash(self.tag)
                return -1
            else:
                self.__missile_run()
            # clear cache and locked info
            self.__clean_cache()
            self.planes_can_locked = []
            return 0
        else:
            return -1

    def close(self):
        self.sim.close()

    def reset(self, init_conditions: dict):
        if self.sim:
            self.sim.close()
        self.aircraft_name = init_conditions.get("aircraft_name", self.aircraft_name)
        self.init_missiles_num = init_conditions.get("missiles", [0, 0])
        self.sim = Simulation(aircraft_name=self.aircraft_name,
                              init_conditions=init_conditions["jsbsim"],
                              jsbsim_freq=self.jsbsim_freq,
                              agent_interaction_steps=self.agent_interaction_steps)
        self.missiles = {
            MissileType.sraam: [Missile(MissileType.sraam, self.sim_step, tag=f"{self.tag}:0:{i}") for i in
                                range(self.init_missiles_num[0])],
            MissileType.amraam: [Missile(MissileType.amraam, self.sim_step, tag=f"{self.tag}:1:{i}") for i in
                                 range(self.init_missiles_num[1])]}
        self.targeted_missiles = []
        self.planes_can_locked = []
        self.running_state = PlaneState.Running
        self.simple_trim = 0
        self.__clean_cache()

    def is_alive(self):
        return self.running_state == PlaneState.Running or self.running_state == PlaneState.Locked

    def is_locked(self):
        return self.running_state == PlaneState.Locked

    def add_plane_can_locked(self, plane_can_locked):
        self.planes_can_locked.append(plane_can_locked)

    def get_target_locked_idx(self):
        return list(range(len(self.planes_can_locked)))

    def get_sim_time(self):
        return self.sim.get_property_value(c.simulation_sim_time_sec)

    def get_action_space(self):
        space_tuple = ()
        # plane action
        for prop in self.plane_action_var:
            if prop.spaces is Box:
                space_tuple += (Box(low=np.array([prop.min]), high=np.array([prop.max]), dtype=np.float),)
            elif prop.spaces is Discrete:
                space_tuple += (Discrete(prop.max - prop.min + 1),)
        # missile action
        space_tuple += (Discrete(3),  # missile type
                        Discrete(self.max_oppo_num))  # target id
        return gym.spaces.Tuple(space_tuple)

    def get_missiles_num(self):
        return len(self.missiles[MissileType.sraam]), len(self.missiles[MissileType.amraam])

    def get_wsg_pos(self):
        if not self.wsg_pos:
            b, l, h = self.sim.get_property_value(c.position_lat_geod_deg), \
                      self.sim.get_property_value(c.position_long_gc_deg), \
                      self.sim.get_property_value(c.position_h_sl_ft) / 3.2808399
            self.wsg_pos = (b, l, h)
        return self.wsg_pos

    def get_property_value(self, name):
        return self.sim.get_property_value(name)

    def get_xyz_pos(self):
        if not self.xyz_pos:
            b, l, h = self.get_wsg_pos()
            self.xyz_pos = wsgtoxyz(b, l, h)
        return self.xyz_pos

    def get_velocity(self):
        if not self.velocity:
            v_north = self.sim.get_property_value(c.velocities_v_north_fps) / 3.2808399
            v_east = self.sim.get_property_value(c.velocities_v_east_fps) / 3.2808399
            v_down = self.sim.get_property_value(c.velocities_v_down_fps) / 3.2808399
            v = math.sqrt(v_north ** 2 + v_east ** 2 + v_down ** 2)
            self.velocity = (v_north, v_east, v_down, v)
        return self.velocity

    def get_attitude(self):
        if not self.attitude:
            psiblx = limitangle(self.sim.get_property_value(c.attitude_psi_deg))
            thtblx = limitangle(self.sim.get_property_value(c.attitude_pitch_deg))
            phiblx = limitangle(self.sim.get_property_value(c.attitude_roll_deg))
            alpha = self.sim.get_property_value(c.aero_alpha_deg)
            beta = self.sim.get_property_value(c.aero_beta_deg)
            self.attitude = (psiblx, thtblx, phiblx, alpha, beta)
        return self.attitude

    def __missile_run(self):
        if self.targeted_missiles:
            x, y, z = self.get_xyz_pos()
            v_north, v_east, v_down, v = self.get_velocity()
            psialx = limitangle(abs(math.degrees(math.acos(v_north / v))))
            thtalx = limitangle(abs(math.degrees(math.acos(v_down / v))))
            for missile in self.targeted_missiles:
                if missile.running_state == MissileState.Unactive:
                    self.targeted_missiles.remove(missile)
                elif missile.running_state == MissileState.Hit:
                    self.running_state = PlaneState.Shotdown
                    self.targeted_missiles = []
                    self.callbacks.on_missile_hit(missile.tag, self.tag)
                    return -1
                elif not self.missile_run_when_miss and missile.running_state == MissileState.Miss:
                    self.targeted_missiles.remove(missile)
                else:
                    missile.run_with_target(x, y, z, psialx, thtalx, v, 1)
        else:
            self.running_state = PlaneState.Running

    def __being_target(self, missile):
        self.targeted_missiles.append(missile)
        self.running_state = PlaneState.Locked

    def __fire_target(self, missile_type, target_plane_id):
        if target_plane_id < len(self.planes_can_locked) and self.missiles[missile_type]:
            missile = self.missiles[missile_type].pop(0)
            x, y, z = self.get_xyz_pos()
            psiblx, thtblx, phiblx, alpha, beta = self.get_attitude()
            _, _, _, v = self.get_velocity()
            missile.active(x, y, z, psiblx, thtblx, phiblx, alpha, beta, v, 3)
            self.planes_can_locked[target_plane_id].__being_target(missile)

    def __clean_cache(self):
        self.xyz_pos = None
        self.wsg_pos = None
        self.attitude = None
        self.velocity = None
