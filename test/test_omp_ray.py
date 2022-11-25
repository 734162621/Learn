
import ray
import time
from multiprocessing import Pool




@ray.remote
def test_ray():
    p = Pool(4)

    p.map(f,[1,2,3,4])
#
# # ray.get(test_ray.remote())
#
if __name__=='__main__':
    def f(i):
        print(i)
    ray.get(test_ray.remote())
#     t0 = time.time()
#     list_M = [SRAAM.SRAAM() for _ in range(10)]
#     for m in list_M:
#         m.missileInit(0, 0, -10000, 0, 0, 0, 0, 0, 300, 3, 1)
#         m.setTarget(10000, 0, -10000, 0, 0, 0, 1)
#         m.setTargetMinDist(8)
#     t0 = time.time()
#     # for _ in range(1000):
#         # d = SRAAM.parallel_run(list_M, 1 / 12, 8)
#     for m in list_M:
#         for _ in range(1000):
#             m.run(1/12)
#     print([m.getSimTime() for m in list_M])
#     t1 = time.time()
#     print(t1-t0)
#     print(Test())
#     print(time.time()-t1)