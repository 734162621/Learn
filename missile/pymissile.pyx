# cython: language_level = 3
# distutils: language = c++
from pymissile cimport missilesim
import cmath
from cython.parallel cimport prange
from libcpp.vector cimport vector
from libcpp.string cimport string


cdef class Missile:
    cdef missilesim*thisptr
    cdef int missile_type
    cdef double sim_step
    cdef double min_dist
    cdef double max_run_time
    cdef double dist

    def __init__(self, missile_type=0, sim_step=1/12):
        self.missile_type = missile_type
        self.sim_step = sim_step
        self.min_dist = 8 if missile_type == 0 else 20
        self.max_run_time = 20 if missile_type == 0 else 60
        self.dist = cmath.inf

    def __cinit__(self):
        self.thisptr = new missilesim()
        if self.thisptr is NULL:
            raise MemoryError()

    def __dealloc__(self) -> None:
        del self.thisptr

    def set_missile_type(self,missile_type: int) -> None:
        self.missile_type = missile_type

    def get_missile_type(self) -> int:
        return self.missile_type

    def active(self,sbel1 :float, sbel2 :float, sbel3 :float,
                    psiblx :float, thtblx :float, phiblx :float,
                    alpha0x :float, beta0x :float, dvbe :float, maut :float) -> None:
        self.thisptr.Unity_missile_init(sbel1, sbel2, sbel3, psiblx, thtblx, phiblx, alpha0x, beta0x, dvbe, maut)
        self.thisptr.SimInit(self.missile_type)
        self.thisptr.SetTargetMinDist(self.min_dist)

    def run_with_target(self, sael1 :float, sael2 :float, sael3 :float,
                        psialx :float, thtalx :float, dvae :float, iftarget :float) -> float|None:
        self.thisptr.Unity_target_input(sael1, sael2, sael3, psialx, thtalx, dvae, iftarget)
        if self.thisptr.SimTime() > self.max_run_time:
            return None
        self.dist = self.run(self.sim_step)
        return self.dist

    def run(self,t :float) -> float:
        dbt = 1e10
        dbt_bytes = "dbt".encode()
        for i in range(int(t/self.thisptr.GetIntStep())):
            self.thisptr.Run()
            dbt = self.thisptr.GetMissileValue(dbt_bytes)
            if dbt < self.min_dist:
                return -1
        return dbt

    def getValue(self,name :str) -> float:
        return self.thisptr.GetMissileValue(name.encode())

    def get_pos(self) ->list[float]:
        return [self.getValue('sbel1'), self.getValue('sbel2'), -self.getValue('sbel3')]

    def get_rot(self) ->list[float]:
        return [self.getValue('psiblx'), self.getValue('thtblx'), self.getValue('phiblx')]

    def get_simtime(self) -> float:
        return self.thisptr.SimTime()

    def dist2target(self) -> float:
        return self.dist

# def parallel_run(missiles :list[Missile],t):
#     cdef int i
#     cdef int size = len(missiles)
#     cdef double dbt
#     cdef missilesim* missile
#     cdef vector[missilesim*] list_missile = missiles
#     cdef string dbt_char= "dbt".encode()
#     # cdef double[] dbts = new double[size]
#     for i in prange(size, nogil=True, num_threads=4):
#         dbt = 1e10
#         missile = list_missile[i]
#         for i in range(int(t / missile.GetIntStep())):
#             missile.Run()
#             dbt = missile.GetMissileValue(dbt_char)
#             if dbt < 8:
#                 dbt = -1
#         # dbts[i] = dbt
#     # return dbts

