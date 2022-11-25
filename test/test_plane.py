import numpy as np

from pyaircombat.plane import Plane
from pyaircombat.plane.jsbsim_catalog import JsbsimCatalog as c

init_conditions = {
        c.ic_h_sl_ft: 10000,
        c.ic_terrain_elevation_ft: 0,
        # c.ic_long_gc_deg: 1.442031,
        c.ic_long_gc_deg: 0.0,
        # c.ic_lat_geod_deg: 43.607181,
        c.ic_lat_geod_deg: 0.0,
        c.ic_u_fps: 800,
        c.ic_v_fps: 0,
        c.ic_w_fps: 0,
        c.ic_p_rad_sec: 0,
        c.ic_q_rad_sec: 0,
        c.ic_r_rad_sec: 0,
        c.ic_roc_fpm: 0,
        c.ic_psi_true_deg: 100,
        c.fcs_throttle_cmd_norm: 0.8,
        c.fcs_mixture_cmd_norm: 1,
        c.gear_gear_pos_norm: 0,
        c.gear_gear_cmd_norm: 0,
}

p = Plane(init_conditions=init_conditions)

for _ in range(100):
    p.step(np.zeros(4),None)