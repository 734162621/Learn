#pragma once
#include "class_hierarchy.hpp"

	enum MissileState{
		Unactive = -1,
		Running = 0,
		Hit = 1,
		Miss = 2
	};
	class missilesim {

	public:

		double missile_init_data[10] = { 0.00 };
		double target[7] = { 0.00 };
		int num_modules; 
		Module* module_list = NULL; 
		double plot_step; 
		double scrn_step; 
		double int_step;
		double com_step; 
		double traj_step;
		int num_vehicles; 
		int num_missile; 
		int num_target; 
		int missile_type; 
		double guidance_start_time; 
		double max_run_time; 
		double max_mprop_time; 
		double min_target_dist = 20; 
		Missile* vehicle_type = NULL; 
		// Vehicle vehicle_list;

		double sim_time = 0; 
		MissileState state = Running;

		char title[CHARL]; 
		char options[CHARL]; 
		bool init = true;

		missilesim();
		~missilesim();
		void   missile_init(double sbel1, double sbel2, double sbel3, double psiblx, double thtblx, double phiblx, double alpha0x, double beta0x, double dvbe,double maut);
		void   target_input(double sael1, double sael2, double sael3, double psialx, double thtalx, double dvae, double iftarget);
		void   SimInit();
		void   InputMissileValue(char* name, double data);
		int   Run();
		double GetMissileValue(char* name);
		double GetIntStep();
		double SimTime();
		
	};





