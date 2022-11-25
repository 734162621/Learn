import math
import numpy as np


def limitangle(a):
    if a < 0:
        a += 360
    if a >= 360:
        a -= 360
    return a


def getDistanceTo(lon1, lat1, lon0, lat0):
    delta_lat_rad = math.radians(lat1)
    delta_lon_rad = math.radians(lon1)
    distance_a = math.sin(0.5 * delta_lat_rad) ** 2 + math.cos(lat1) * math.cos(lat0) * math.sin(
        0.5 * delta_lon_rad) ** 2
    return 2.0 * 6378245.0 * math.atan2(math.sqrt(distance_a), math.sqrt(1.0 - distance_a))


def wsgtoxyz(lat, lon, alti):
    x_sign = -1 if lat < 0 else 1
    y_sign = -1 if lon < 0 else 1
    return [getDistanceTo(0, lat, 0, 0) * x_sign, getDistanceTo(lon, 0, 0, 0) * y_sign, alti]


def rotationMatrix(pitch, roll, psi):
    R = np.array([[math.cos(pitch) * math.cos(psi), math.sin(pitch), -math.cos(pitch) * math.sin(psi)],
                  [-math.sin(pitch) * math.cos(psi) * math.cos(roll) + math.sin(psi) * math.sin(roll),
                   math.cos(pitch) * math.cos(roll),
                   math.sin(pitch) * math.sin(psi) * math.cos(roll) + math.cos(psi) * math.sin(roll)],
                  [math.sin(pitch) * math.cos(psi) * math.sin(roll) + math.sin(psi) * math.cos(roll),
                   -math.cos(pitch) * math.sin(roll),
                   -math.sin(pitch) * math.sin(psi) * math.sin(roll) + math.cos(psi) * math.cos(roll)]])
    return R


def get_dist_matrix(camp0_pos, camp1_pos):
    # camp0_pos = np.stack([p.get_xyz_pos() for p in camp0], 1)
    # camp1_pos = np.stack([p.get_xyz_pos() for p in camp1], 1)
    delta_pos = camp1_pos.reshape((3, 1, -1)) - camp0_pos.reshape((3, -1, 1))
    return np.sqrt(np.square(delta_pos).sum(0))


def get_angle_between(pos0, rot0, pos1, rot1):
    delta_pos = pos1 - pos0
    delta_pos_length = np.sqrt(np.square(delta_pos).sum(1))
    theta0, psi0 = rot0[:, 0], rot0[:, 1]
    cos_theta0 = np.cos(theta0)
    head_vector0 = np.stack([cos_theta0 * np.cos(psi0), cos_theta0 * np.sin(psi0), np.sin(theta0)])
    angle0 = np.sum(delta_pos.T * head_vector0, 0) / delta_pos_length
    theta1, psi1 = rot1[:, 0], rot1[:, 1]
    cos_theta1 = np.cos(theta1)
    head_vector1 = np.stack([cos_theta1 * np.cos(psi1), cos_theta1 * np.sin(psi1), np.sin(theta1)])
    angle1 = np.sum(-delta_pos.T * head_vector1, 0) / delta_pos_length
    return np.arccos(angle0), np.arccos(angle1)


def global_radar_info(planes_pos, planes_attitudes, planes_velocity):
    cos_attitudes, sin_attitudes = np.cos(planes_attitudes), np.sin(planes_attitudes)
    cos_theta, cos_psi, cos_phi = np.split(cos_attitudes, 3, 1)
    cos_theta, cos_psi, cos_phi = cos_theta.squeeze(), cos_psi.squeeze(), cos_phi.squeeze()
    sin_theta, sin_psi, sin_phi = np.split(sin_attitudes, 3, 1)
    sin_theta, sin_psi, sin_phi = sin_theta.squeeze(), sin_psi.squeeze(), sin_phi.squeeze()
    M = np.array([[cos_theta * cos_psi, cos_theta * sin_psi, -sin_theta],
                  [sin_phi * sin_theta * cos_psi - cos_phi * sin_psi, sin_phi * sin_theta * sin_psi + cos_phi * cos_psi,
                   sin_phi * cos_theta],
                  [cos_phi * sin_theta * cos_psi + sin_phi * sin_psi, cos_phi * sin_theta * sin_psi - sin_phi * cos_psi,
                   cos_phi * cos_theta]]).swapaxes(0, 2)
    n = planes_pos.shape[0]
    delta_pos = planes_pos.reshape((1, -1, 3)) - planes_pos.reshape((-1, 1, 3))
    pos_ = delta_pos @ M
    dist = np.linalg.norm(delta_pos, axis=2) + np.eye(n)*1e10
    cos_alpha_ = pos_[:, :, 0] / dist
    sin_theta_ = pos_[:, :, 2] / dist
    tan_psi_ = pos_[:, :, 1] / (pos_[:, :, 0] + np.eye(n))
    head_vector = np.stack([cos_theta * cos_psi, cos_theta * sin_psi, sin_theta], 1)
    radius_velocity = head_vector @ planes_velocity.T
    attitudes_ = np.stack([np.arccos(cos_alpha_), np.arcsin(sin_theta_), np.arctan(tan_psi_)],2)
    return dist, attitudes_, radius_velocity


def locked_decided(camp0_pos, camp0_rot, camp1_pos, camp1_rot, lock_dist, lock_angle):
    delta_pos = camp1_pos.T.reshape((3, 1, -1)) - camp0_pos.T.reshape((3, -1, 1))
    dist_square = np.square(delta_pos).sum(0)
    ind_lq_dist = np.argwhere(dist_square <= lock_dist ** 2)
    if not ind_lq_dist.size:
        return [], []
    else:
        ind_camp0, ind_camp1 = ind_lq_dist[:, 0], ind_lq_dist[:, 1]
        delta_pos = camp1_pos[ind_camp1] - camp0_pos[ind_camp0]
        delta_pos_length = np.linalg.norm(delta_pos, axis=1)
        theta0, psi0 = camp0_rot[ind_camp0, 0], camp0_rot[ind_camp0, 1]
        cos_theta0 = np.cos(theta0)
        head_vector0 = np.stack([cos_theta0 * np.cos(psi0), cos_theta0 * np.sin(psi0), np.sin(theta0)], 1)
        angle0 = np.arccos(np.sum(delta_pos * head_vector0, 1) / delta_pos_length)
        theta1, psi1 = camp1_rot[ind_camp1, 0], camp1_rot[ind_camp1, 1]
        cos_theta1 = np.cos(theta1)
        head_vector1 = np.stack([cos_theta1 * np.cos(psi1), cos_theta1 * np.sin(psi1), np.sin(theta1)], 1)
        angle1 = np.arccos(np.sum(-delta_pos * head_vector1, 1) / delta_pos_length)
        ind_lq_angle0 = ind_lq_dist[np.argwhere(angle0 <= lock_angle)]
        ind_lq_angle0 = [] if ind_lq_angle0.size == 0 else ind_lq_angle0.squeeze(1)
        ind_lq_angle1 = ind_lq_dist[np.argwhere(angle1 <= lock_angle)]
        ind_lq_angle1 = [] if ind_lq_angle1.size == 0 else ind_lq_angle1.squeeze(1)[:, [1, 0]]
        return ind_lq_angle0, ind_lq_angle1
