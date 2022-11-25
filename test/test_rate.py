import time
import numpy as np
import ray
import random

# from pyaircombat import SRAAM
from pyaircombat.plane import Plane
from pyaircombat.plane import JsbsimCatalog as c

from pympler import tracker
# ray.init()

def get_init_condition():
    max_range = 0.1
    red_y = 2 * max_range * np.random.random() - max_range
    blue_y = 2 * max_range * np.random.random() - max_range
    equal = [max_range, -max_range, -90, 90], [-max_range, max_range, 90, -90]
    # 可以设置权重
    initial_pos = random.choice(equal)
    red_x, blue_x, red_psi, blue_psi = \
        np.clip(np.random.random(), 0.5, 1) * initial_pos[0], \
        np.clip(np.random.random(), 0.5, 1) * initial_pos[1], \
        initial_pos[2], \
        initial_pos[3]


    init_conditions0 = {
         "aircraft_name": "f16",
            "missiles": [1, 0],
            "jsbsim": {
            c.ic_h_sl_ft: 28000,
            c.ic_terrain_elevation_ft: 0,
            c.ic_long_gc_deg: red_x,
            c.ic_lat_geod_deg: red_y,
            c.ic_u_fps: 590.73,
            c.ic_v_fps: 0,
            c.ic_w_fps: 0,
            c.ic_p_rad_sec: 0,
            c.ic_q_rad_sec: 0,
            c.ic_r_rad_sec: 0,
            c.ic_roc_fpm: 0,
            c.ic_psi_true_deg: red_psi,
            # c.fcs_throttle_cmd_norm: 0.8,
            # c.fcs_mixture_cmd_norm: 1,
            c.gear_gear_pos_norm: 0,
            c.gear_gear_cmd_norm: 0,
            }
    }

    init_conditions1 = {
         "aircraft_name": "f16",
            "missiles": [1, 0],
            "jsbsim": {
            c.ic_h_sl_ft: 28000,
            c.ic_terrain_elevation_ft: 0,
            c.ic_long_gc_deg: blue_x,
            c.ic_lat_geod_deg: blue_y,
            c.ic_u_fps: 590.73,
            c.ic_v_fps: 0,
            c.ic_w_fps: 0,
            c.ic_p_rad_sec: 0,
            c.ic_q_rad_sec: 0,
            c.ic_r_rad_sec: 0,
            c.ic_roc_fpm: 0,
            c.ic_psi_true_deg: blue_psi,
            # c.fcs_throttle_cmd_norm: 0.8,
            # c.fcs_mixture_cmd_norm: 1,
            c.gear_gear_pos_norm: 0,
            c.gear_gear_cmd_norm: 0,
            }
    }
    return init_conditions0,init_conditions1

# init_conditions0 = {
#         c.ic_h_sl_ft: 10000,
#         c.ic_terrain_elevation_ft: 0,
#         c.ic_long_gc_deg: 0.0,
#         c.ic_lat_geod_deg: 0.0,
#         c.ic_u_fps: 800,
#         c.ic_v_fps: 0,
#         c.ic_w_fps: 0,
#         c.ic_p_rad_sec: 0,
#         c.ic_q_rad_sec: 0,
#         c.ic_r_rad_sec: 0,
#         c.ic_roc_fpm: 0,
#         c.ic_psi_true_deg: 90,
#         c.fcs_throttle_cmd_norm: 0.8,
#         c.fcs_mixture_cmd_norm: 1,
#         c.gear_gear_pos_norm: 0,
#         c.gear_gear_cmd_norm: 0,
# }
#
# init_conditions1 = {
#         c.ic_h_sl_ft: 10000,
#         c.ic_terrain_elevation_ft: 0,
#         c.ic_long_gc_deg: 0.15,
#         c.ic_lat_geod_deg: 0.01,
#         c.ic_u_fps: 800,
#         c.ic_v_fps: 0,
#         c.ic_w_fps: 0,
#         c.ic_p_rad_sec: 0,
#         c.ic_q_rad_sec: 0,
#         c.ic_r_rad_sec: 0,
#         c.ic_roc_fpm: 0,
#         c.ic_psi_true_deg: 270,
#         c.fcs_throttle_cmd_norm: 0.8,
#         c.fcs_mixture_cmd_norm: 1,
#         c.gear_gear_pos_norm: 0,
#         c.gear_gear_cmd_norm: 0,
# }

@ray.remote
class test_actor:
    def __init__(self,worker_id):
        self.worker_id = worker_id
        init_conditions0, init_conditions1 = get_init_condition()
        self.plane0 = Plane("A320",agent_interaction_steps=12)
        self.plane1 = Plane("A320",agent_interaction_steps=12)

    def run(self):
        total_sim_time = 0
        for _ in range(1):
            init_conditions0, init_conditions1 = get_init_condition()
            self.plane0.reset(init_conditions0)
            self.plane1.reset(init_conditions1)
            missile_active_time = 0
            for i in range(5000):
                self.plane0.add_plane_can_locked(self.plane1)
                self.plane1.add_plane_can_locked(self.plane0)
                state0 = self.plane0.step([0, 0, 0, 0.1,0,0])
                state1 = self.plane1.step([0, 0, 0, 0.1,0,0])
                if state0 or state1:
                    break
            total_sim_time += self.plane0.get_sim_time()
        return total_sim_time,self.worker_id
@ray.remote
def single_agent():
    init_conditions0, init_conditions1 = get_init_condition()
    plane0 = Plane("f16",agent_interaction_steps=12)
    plane1 = Plane("f16",agent_interaction_steps=12)
    total_sim_time = 0
    for _ in range(1):
        init_conditions0, init_conditions1 = get_init_condition()
        plane0.reset(init_conditions0)
        plane1.reset(init_conditions1)
        missile_active_time = 0
        for i in range(5000):
            plane0.add_plane_can_locked(plane1)
            plane1.add_plane_can_locked(plane0)
            state0 = plane0.step([0,0,0,0.1, 0, 0])
            state1 = plane1.step([0,0,0,0.1, 0, 0])
            if state0 or state1:
                break
        total_sim_time += plane0.get_sim_time()

    return total_sim_time

def single_agent_local():
    init_conditions0, init_conditions1 = get_init_condition()
    plane0 = Plane("f16",agent_interaction_steps=12)
    plane1 = Plane("f16",agent_interaction_steps=12)
    total_sim_time = 0
    for _ in range(1):
        init_conditions0, init_conditions1 = get_init_condition()
        plane0.reset(init_conditions0)
        plane1.reset(init_conditions1)
        missile_active_time = 0
        for i in range(5000):
            plane0.add_plane_can_locked(plane1)
            plane1.add_plane_can_locked(plane0)
            state0 = plane0.step([0,0,0,0.1, 0, 0])
            state1 = plane1.step([0,0,0,0.1, 0, 0])
            if state0 or state1:
                break
        total_sim_time += plane0.get_sim_time()

    return total_sim_time

def test_rate_0():
    total_collect_num = 10000
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

def test_rate_1():
    t0 = time.time()
    # t = random_agent()
    n = 16
    t = ray.get([single_agent.remote() for _ in range(n)])
    # t = ray.get(random_agent.remote())
    t1 = time.time() - t0
    print(t)
    # print(t/(time.time()-t0))
    print(sum(t) / t1)
    print(sum(t) / t1 / n)
    ray.shutdown()

def test_rate_2():
    total_collect_num = 100000
    num_workers = 16
    truelly_collect_num = 0
    total_sim_time = 0
    actors = [test_actor.remote(i) for i in range(num_workers)]
    begin = time.time()
    while truelly_collect_num < total_collect_num:
        task_list = []
        for actor in actors:
            task_list.append(actor.run.remote())
        while truelly_collect_num < total_collect_num:
            done_id, task_list = ray.wait(task_list)
            sim_time,worker_id = ray.get(done_id)[0]
            total_sim_time += sim_time
            task_list.append(actors[worker_id].run.remote())
            truelly_collect_num += 1
            print(total_sim_time/(time.time()-begin))
    ray.shutdown()

if __name__ == "__main__":
    # test_rate_1()
    # test_rate_0()
    tr = tracker.SummaryTracker()
    for _ in range(1000):
        for j in range(10):
            single_agent_local()
        tr.print_diff()

