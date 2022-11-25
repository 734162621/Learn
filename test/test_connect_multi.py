import time
import matplotlib.pyplot as plt
import numpy as np

from pyaircombat.plane import Plane
from pyaircombat.plane import JsbsimCatalog as c
import SRAAM

fig = plt.figure()
# 创建3d绘图区域
ax = plt.axes(projection='3d')

init_conditions0 = {
    c.ic_h_sl_ft: 20000,
    # c.ic_terrain_elevation_ft: 0,
    c.ic_long_gc_deg: 0.0,
    c.ic_lat_geod_deg: 0.0,
    c.ic_u_fps: 800,
    c.ic_v_fps: 0,
    c.ic_w_fps: 0,
    # c.ic_p_rad_sec: 0,
    # c.ic_q_rad_sec: 0,
    # c.ic_r_rad_sec: 0,
    # c.ic_roc_fpm: 0,
    c.ic_psi_true_deg: 90,
    # c.fcs_throttle_cmd_norm: 0.8,
    # c.fcs_mixture_cmd_norm: 1,
    # c.gear_gear_pos_norm: 0,
    # c.gear_gear_cmd_norm: 0,
}

init_conditions1 = {
    c.ic_h_sl_ft: 20000,
    # c.ic_terrain_elevation_ft: 0,
    c.ic_long_gc_deg: 0.3,
    c.ic_lat_geod_deg: 0.01,
    c.ic_u_fps: 800,
    c.ic_v_fps: 0,
    c.ic_w_fps: 0,
    # c.ic_p_rad_sec: 0,
    # c.ic_q_rad_sec: 0,
    # c.ic_r_rad_sec: 0,
    # c.ic_roc_fpm: 0,
    c.ic_psi_true_deg: 270,
    # c.fcs_throttle_cmd_norm: 0.8,
    # c.fcs_mixture_cmd_norm: 1,
    # c.gear_gear_pos_norm: 0,
    # c.gear_gear_cmd_norm: 0,
}


def random_agent():
    missile_pos0 = []
    missile_pos1 = []
    p_pos0 = []
    p_pos1 = []
    dist = []
    # env = gym.make("GymJsbsim-HeadingControlTask-v0")
    p0 = Plane("F16", init_conditions=init_conditions0,agent_interaction_steps=12, sraam_num=1, amraam_num=1)
    p1 = Plane("F16", init_conditions=init_conditions1,agent_interaction_steps=12, sraam_num=1, amraam_num=1)
    p0.reset()
    p1.reset()
    missile_active_time = 50
    # p0.simple_trim_on()
    # p1.simple_trim_on()
    time0 = time.time()
    for i in range(5000):
        p0.add_plane_can_locked(p1)
        p1.add_plane_can_locked(p0)
        missile_type = 0 if i == missile_active_time else -1
        state0 = p0.step([0, 0, 0, 0, missile_type, 0])
        state1 = p1.step([0, 0, 0, 0, missile_type, 0])
        if state0 or state1:
            break
        p_pos0.append(p0.get_xyz_pos())
        p_pos1.append(p1.get_xyz_pos())
        if p0.targeted_missiles:
            missile_pos0.append(p0.targeted_missiles[0].get_pos())
            dist.append(p0.targeted_missiles[0].dist2target())
        if p1.targeted_missiles:
            missile_pos1.append(p1.targeted_missiles[0].get_pos())

    print("sim_time: {}".format(p0.get_sim_time()))
    print('rate:', p0.get_sim_time() / (time.time() - time0))
    p_pos0 = np.stack(p_pos0)
    p_pos1 = np.stack(p_pos1)
    missile_pos0 = np.stack(missile_pos0)
    missile_pos1 = np.stack(missile_pos1)
    ax.scatter(*p_pos0[0])
    ax.scatter(*p_pos1[0])
    ax.plot3D(p_pos0[:, 0], p_pos0[:, 1], p_pos0[:, 2], label='r_0')
    ax.plot3D(p_pos1[:, 0], p_pos1[:, 1], p_pos1[:, 2], label='b_0')
    ax.plot3D(missile_pos0[:, 0], missile_pos0[:, 1], missile_pos0[:, 2], '--', label='missile_0')
    ax.plot3D(missile_pos1[:, 0], missile_pos1[:, 1], missile_pos1[:, 2], '--', label='missile_1')
    ax.legend()
    plt.figure()
    plt.plot(dist)
    plt.show()


if __name__ == "__main__":
    random_agent()
