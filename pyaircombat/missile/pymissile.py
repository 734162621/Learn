import SRAAM
import enum


class MissileState(enum.Enum):
    Unactive = -1
    Running = 0
    Hit = 1
    Miss = 2


class MissileType(enum.Enum):
    none = -1
    sraam = 0
    amraam = 1


class Missile:
    def __init__(self, missile_type=MissileType.sraam, sim_step=1 / 12, tag=''):
        self.tag = tag
        self.missile_type = missile_type
        self.sim_step = sim_step
        self.min_dist = 8 if missile_type == MissileType.sraam else 20
        self.max_run_time = 20 if missile_type == MissileType.sraam else 60
        self.missile = SRAAM.SRAAM()
        self.missile.InitParamter(missile_type.value, self.min_dist, self.max_run_time)
        self.dist = None
        self.running_state = self.missile.state

    def get_missile_type(self):
        return self.missile_type

    def run(self):
        self.missile.run(self.sim_step)

    def active(self, sbel1, sbel2, sbel3, psiblx, thtblx, phiblx, alpha0x, beta0x, dvbe, maut):
        self.missile.missileInit(sbel1, sbel2, -sbel3, psiblx, thtblx, phiblx, alpha0x, beta0x, dvbe, maut)

    def set_target(self, sael1, sael2, sael3, psialx, thtalx, dvae, iftarget):
        self.missile.setTarget(sael1, sael2, -sael3, psialx, thtalx, dvae, iftarget)

    def run_with_target(self, sael1, sael2, sael3, psialx, thtalx, dvae, iftarget):
        self.missile.setTarget(sael1, sael2, -sael3, psialx, thtalx, dvae, iftarget)
        self.missile.run(self.sim_step)
        self.running_state = MissileState(self.missile.state)
        self.dist = self.missile.dist

    def getSimTime(self):
        return self.missile.sim_time

    def get_pos(self):
        return self.missile.pos

    def get_rot(self):
        return self.missile.rot

    def dist2target(self):
        if not self.dist:
            self.dist = self.missile.dist
        return self.dist
