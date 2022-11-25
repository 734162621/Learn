import numpy as np

from pyaircombat.plane import Plane
from pyaircombat.plane import JsbsimCatalog as c
from pyaircombat.utils import locked_decided, global_radar_info
from pyaircombat.visualiser import FigureVisualiser
from pyaircombat.callbacks import GlobalCallback


class AirCombatEnv:
    def __init__(self, num_red, num_blue, callbacks_class, jsbsim_freq=60, agent_interaction_steps=12):
        self.num_red = num_red
        self.num_blue = num_blue
        self.num_planes = {'r': num_red, 'b': num_blue}
        self.lock_dist = 40000
        self.lock_angle = np.radians(25)
        self.callbacks: GlobalCallback = callbacks_class(self)
        self.planes = {}
        self.camps = {'r': [], 'b': []}
        for camp in self.camps.keys():
            for i in range(self.num_planes[camp]):
                plane_tag = f"{camp}{i}"
                plane = Plane("F16", max_oppo_num=self.num_planes['b' if camp == 'r' else 'r'],
                              tag=plane_tag, callbacks=self.callbacks,
                              jsbsim_freq=jsbsim_freq, agent_interaction_steps=agent_interaction_steps)
                self.planes[plane_tag] = plane
                self.camps[camp].append(plane)
        self.all_planes_tags = list(self.planes.keys())
        self.sim_step = agent_interaction_steps / jsbsim_freq
        self.action_space = {plane_tag: plane.get_action_space() for plane_tag, plane in self.planes.items()}
        self.visualizer = None

    def radar_simulate(self):
        # compute radar and locked info
        radar_infos, planes_infos, locked_infos = {}, {}, None
        alive_planes = self.get_alive_planes()
        alive_red_num = len(alive_planes[0])
        alive_bule_num = len(alive_planes[1])
        if alive_red_num and alive_bule_num:
            all_planes = alive_planes[0] + alive_planes[1]
            planes_pos = np.stack([p.get_xyz_pos() for p in all_planes])
            planes_attitude = np.stack([[p.get_property_value(c.attitude_pitch_rad),
                                         p.get_property_value(c.attitude_psi_rad),
                                         p.get_property_value(c.attitude_phi_rad)] for p in all_planes])
            planes_velocity = np.stack([p.get_velocity() for p in all_planes])[:, :3]
            dist, attitudes_, radius_velocity = global_radar_info(planes_pos, planes_attitude, planes_velocity)
            radar_infos = {"dist": dist, "radar_attitude": attitudes_, "radius_velocity": radius_velocity}
            planes_infos = {"attitude": planes_attitude, "velocity": planes_velocity}
            locked_infos = np.argwhere(attitudes_[:alive_red_num, alive_red_num:, 0] <= self.lock_angle), \
                           np.argwhere(attitudes_[alive_red_num:, :alive_red_num, 0] <= self.lock_angle)
            for i, locked_info in enumerate(locked_infos):
                for can_locked_pair in locked_info:
                    alive_planes[i][can_locked_pair[0]].add_plane_can_locked(alive_planes[1 - i][can_locked_pair[1]])

        return alive_planes, radar_infos, planes_infos, locked_infos

    def step(self, actions):
        self.callbacks.before_step()
        # plane sim step
        for plane_tag, action in actions.items():
            self.planes[plane_tag].step(action)
        # compute radar and locked info
        alive_planes, radar_infos, planes_infos, locked_infos = self.radar_simulate()
        self.callbacks.after_step()
        return self.callbacks.get_obs(alive_planes, radar_infos, planes_infos,
                                      locked_infos), *self.callbacks.reward_func(alive_planes)

    def get_alive_planes(self):
        return [[p for p in self.camps['r'] if p.is_alive()], [p for p in self.camps['b'] if p.is_alive()]]

    def get_alive_plane_tags(self):
        return [plane_tag for plane_tag, plane in self.planes.items() if plane.is_alive()]

    def get_sim_time(self):
        return max([p.get_sim_time() for p in self.planes.values()])

    def reset(self):
        self.callbacks.before_reset()
        if self.visualizer:
            self.visualizer.reset()
        for camp, camp_planes in self.camps.items():
            for plane_tag, plane in enumerate(camp_planes):
                plane.reset(self.callbacks.init_condition(camp, plane_tag))
        alive_planes, radar_infos, planes_infos, locked_infos = self.radar_simulate()
        self.callbacks.after_reset()
        return self.callbacks.get_obs(alive_planes, radar_infos, planes_infos, locked_infos)

    def render(self):
        if not self.visualizer:
            self.visualizer = FigureVisualiser()
        self.visualizer.plot(self.planes)

    def close(self):
        for plane in self.planes.values():
            plane.close()
