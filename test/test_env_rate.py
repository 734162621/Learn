import time

import numpy as np
import random

from pyaircombat.envs import AirCombatEnv
from pyaircombat.plane import JsbsimCatalog as c
from pyaircombat.callbacks import GlobalCallback
import ray
import os
# os.environ["RAY_record_ref_creation_sites"] = "1"
# ray.init()


class ExampleCallback(GlobalCallback):
    def before_reset(self):
        self.planes_reward = {plane_tag: 0 for plane_tag in self.env.all_planes_tags}

    def init_condition(self, camp, plane_tag):
        max_range = 0.2
        equal = [max_range, -max_range, -90, 90], [-max_range, max_range, 90, -90]
        initial_pos = random.choice(equal)
        if camp == 'r':
            x = np.clip(np.random.random(), 0.5, 1) * initial_pos[0]
            y = 2 * max_range * np.random.random() - max_range
            psi = initial_pos[2]
        else:
            x = np.clip(np.random.random(), 0.5, 1) * initial_pos[1]
            y = 2 * max_range * np.random.random() - max_range
            psi = initial_pos[3]

        init_conditions = {
            "aircraft_name": "f16",
            "missiles": [1, 1],
            "jsbsim": {
                c.ic_h_sl_ft: 28000,
                c.ic_terrain_elevation_ft: 0,
                c.ic_long_gc_deg: x,
                c.ic_lat_geod_deg: y,
                c.ic_u_fps: 590.73,
                c.ic_v_fps: 0,
                c.ic_w_fps: 0,
                c.ic_p_rad_sec: 0,
                c.ic_q_rad_sec: 0,
                c.ic_r_rad_sec: 0,
                c.ic_roc_fpm: 0,
                c.ic_psi_true_deg: psi,
                c.fcs_throttle_cmd_norm: 0.8,
                c.fcs_mixture_cmd_norm: 1,
                c.gear_gear_pos_norm: 0,
                c.gear_gear_cmd_norm: 0,
            }
        }

        return init_conditions

    def reward_func(self, alive_planes):
        done = False
        info = {plane_tag: [] for plane_tag in self.env.all_planes_tags}
        reward = self.planes_reward
        if len(alive_planes[0]) == 0:
            done = True
        if len(alive_planes[1]) == 0:
            done = True
        return reward, done, info

    def on_missile_hit(self, missile_tag, plane_tag):
        missile_from_tag, missile_type, missile_idx = missile_tag.split(":")
        missile_type = 'sraam' if missile_type == '0' else 'amraam'
        # print(f"{missile_from_tag}'s {missile_type} missile:{missile_idx} hit {plane_tag}")
        self.planes_reward[missile_from_tag] += 1
        self.planes_reward[plane_tag] -= 1

    def on_plane_crash(self, plane_tag):
        # print(f"{plane_tag} crash")
        self.planes_reward[plane_tag] -= 1

@ray.remote
def single_agent():
    env = AirCombatEnv(3, 3, callbacks_class=ExampleCallback)  # 3v3
    total_sim_time = 0
    for _ in range(1):
        env.reset()
        for i in range(5000):
            missile_type = -1 if i < 10 else 1  # missile type ( 0:no missile, 1:sraam, 2:armsaam ) launch arsaam when start
            if i > 50:  # launch sraam when 50 step
                missile_type = 0
            obs, reward, done, info = env.step({plane_tag: [0, 0, 0, 0.1, missile_type, 0] for plane_tag in
                                                env.get_alive_plane_tags()})  # camp 0 plane 0 ( aileron,elevator,rudder,throttle,missile_type,target_idx)
            if done:
                # print(reward)
                break
        total_sim_time += env.get_sim_time()
    env.close()
    return total_sim_time

def test_rate_0():
    total_collect_num = 100000
    num_workers = 16
    truelly_collect_num = 0
    total_sim_time = 0
    begin = time.time()
    while truelly_collect_num < total_collect_num:
        task_list = []
        for actor in range(num_workers):
            task_list.append(single_agent.remote())
        while truelly_collect_num < total_collect_num:
            done_id, task_list = ray.wait(task_list)
            sim_time = ray.get(done_id)
            total_sim_time += sum(sim_time)
            task_list.append(single_agent.remote())
            truelly_collect_num += 1
            print(total_sim_time/(time.time()-begin))
    ray.shutdown()

if __name__ == '__main__':
    test_rate_0()
