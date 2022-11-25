import time
from pyaircombat.missile import Missile,MissileType
import matplotlib.pyplot as plt
from pympler import tracker
import gc

# # print(dir(SRAAM.SRAAM))
# m = SRAAM.SRAAM()
# m.missileInit(0, 0, -10000, 0, 0, 0, 0, 0, 300, 3, 1)
# # SRAAM.helloworld([m])
# x = []
# y = []
# d0 = []
# d1 = []
# dist = []
# t_x=10000
# t_v=200
# t0 = time.time()
# for i in range(1000):
#     m.setTarget(t_x,0,-10000,0,0,0,1)
#     # dist.append(m.getValue('dbt'))
#     ans = m.run(0.2,8)
#     if ans<2:
#         print(ans)
#         break
#     # print(m.getValue('dbt'),m.getValue("vmach"),m.getSimTime())
#     # print(m.getValue('sbel1'),m.getValue('sbel2'),m.getValue('sbel3'))
#     # x.append(m.getValue('sbel1'))
#     # y.append(m.getValue('sbel2'))
#     # d0.append(m.getValue('thtpbx'))
#     # d1.append(m.getValue('psipbx'))
#     # t_x+=t_v*0.2
#     # print(m.getValue('STEL'))
# print(time.time()-t0)
# # print(x[-1],y[-1])
# # plt.plot(x,y)
# # plt.plot(d0)
# # plt.plot(dist)
# # plt.show()


# class A:
#     def __init__(self):
#         self.m = SRAAM.SRAAM()
#         self.m.missileInit(0, 0, -10000, 0, 0, 0, 0, 0, 300, 3, 1)
#     def run(self):
#         self.m.setTarget(10000,0,-10000,0,0,0,1)
#         ans = self.m.run(0.2, 8)
#
# list_A = [A() for _ in range(2)]
# t0 = time.time()
# for _ in range(1000):
#     SRAAM.helloworld(list_A)
# # for _ in range(1000):
# #     for a in list_A:
# #         a.run()
# print(time.time()-t0)
# # print([a.m.getSimTime() for a in list_A])

# list_M = [Missile() for _ in range(2)]
# for m in list_M:
#     m.active(0, 0, -10000, 0, 0, 0, 0, 0, 300, 3)
#     m.set_target(10000, 0, -10000, 0, 0, 0, 1)
# t0 = time.time()
# missiles = [m.missile for m in list_M]
# # for _ in range(1000):
# #     d = SRAAM.parallel_run(missiles, 1/12, 8)
# # # for _ in range(1000):
# # #     for m in list_M:
# # #         m.run(0.002,0)
# for m in list_M:
#     for _ in range(1000):
#         m.run_with_target(10000,0,-10000,0,0,0,1)
#         if m.running_state != 0:
#             break
# print(time.time()-t0)
#
# print([m.getSimTime() for m in list_M])
tr = tracker.SummaryTracker()
for i in range(1000000):
    m = Missile(MissileType.sraam, 0.2, tag=f"1")
    m.active(0, 0, -10000, 0, 0, 0, 0, 0, 300, 3)
    for _ in range(100):
        m.run_with_target(10000,0,-10000,0,0,0,1)
    # if i % 1000 == 0:
    #     tr.print_diff()