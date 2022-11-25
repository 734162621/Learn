class GlobalCallback:
    def __init__(self, env=None):
        self.env = env

    def before_step(self):
        pass

    def after_step(self):
        pass

    def before_reset(self):
        pass

    def after_reset(self):
        pass

    def init_condition(self, camp, plane_idx):
        raise NotImplementedError("init condition func must be implemented!")

    def on_missile_hit(self, missile_tag, plane_tag):
        pass

    def on_plane_crash(self, plane_tag):
        pass

    def reward_func(self, alive_planes):
        raise NotImplementedError("reward func must be implemented!")

    def get_obs(self, alive_planes, radar_infos, planes_infos, locked_infos):
        pass
