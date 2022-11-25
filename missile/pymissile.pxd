# cython: language_level=3
# distutils: language=c++

cdef extern from "missilesim.h":
    cdef cppclass missilesim:
        missilesim() except +
        void Unity_missile_init(double sbel1, double sbel2, double sbel3, double psiblx, double thtblx, double phiblx,
                                double alpha0x, double beta0x, double dvbe,double maut)
        void Unity_target_input(double sael1, double sael2, double sael3, double psialx, double thtalx, double dvae,
                                double iftarget)
        void SimInit(int missiletype)
        void SetTargetMinDist(double min_dist);
        double GetTargetMinDist();
        double  GetMissileValue(char* name)
        double  GetIntStep();
        void InputMissileValue(char* name, double data)
        void Run()
        double SimTime()