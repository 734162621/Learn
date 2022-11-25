#include "class_hierarchy.hpp"
#include "missilesim.h"
#include <time.h>
#include <iostream>

	void combus_status(Packet* combus, int* status, int num_vehicles);

	void missilesim::missile_init(double sbel1, double sbel2, double sbel3, double psiblx, double thtblx, double phiblx, double alpha0x, double beta0x, double dvbe,double maut)
	{
		
		missile_init_data[0] = sbel1;
		missile_init_data[1] = sbel2;
		missile_init_data[2] = sbel3;
		missile_init_data[3] = psiblx;
		missile_init_data[4] = thtblx;
		missile_init_data[5] = phiblx;
		missile_init_data[6] = alpha0x;
		missile_init_data[7] = beta0x;
		missile_init_data[8] = dvbe;
		missile_init_data[9] = maut;
	}

	void missilesim::target_input(double sael1, double sael2, double sael3, double psialx, double thtalx, double dvae, double iftarget)
	{
		target[0] = sael1;
		target[1] = sael2;
		target[2] = sael3;
		target[3] = psialx;
		target[4] = thtalx;
		target[5] = dvae;
		target[6] = iftarget;
	}

	double missilesim::GetIntStep(){
	    return this->int_step;
	}


	void acquire_title_options(fstream& input, char* title, char* options);


	double acquire_endtime(fstream& input);


	void number_modules(fstream& input, int& num);


	void order_modules(fstream& input, int& num, Module* module_list);


	void number_objects(fstream& input, int& num_vehicles, int& num_missile, int& num_target);


	Cadac* set_obj_type(fstream& input, Module* module_list, int num_modules,
		int num_target);


	void acquire_timing(fstream& input, double& plot_step, double& scn_step, double& com_step,
		double& traj_step, double& int_step);

	void comscrn_data(Packet* combus, int num_vehicles);

	//Vehicle vehicle_list(2);


	missilesim::missilesim()
	{
	}

	missilesim::~missilesim()
	{
		delete vehicle_type;
	}

	double  missilesim::GetMissileValue(char* name) {
		return vehicle_type->vehicle_data_OUT(name);
	}

	void missilesim::InputMissileValue(char* name, double data) {
		vehicle_type->vehicle_data_Input(name, data);
	}


	void missilesim::SimInit() {
		if (missile_type == 0){
			guidance_start_time = 0.25; //sraam
			max_mprop_time = 2.69;
		}
		else{
			guidance_start_time = 5; //amraam
			max_mprop_time = 60;
		}
		sim_time = 0;
		int_step = 0;

		num_modules=13;
		module_list = new Module[num_modules];
		if (module_list == 0) { cerr << "*** Error: module_list[] allocation failed *** \n"; exit(1); }
		module_list[0].name="environment";
		module_list[0].definition="def";
		module_list[0].execution="exec";
		module_list[0].initialization="0";
		module_list[0].termination="0";
		module_list[1].name="kinematics";
		module_list[1].definition="def";
		module_list[1].initialization="init";
		module_list[1].execution="exec";
		module_list[1].termination="0";
		module_list[2].name="aerodynamics";
		module_list[2].definition="def";
		module_list[2].initialization="init";
		module_list[2].execution="exec";
		module_list[2].termination="0";
		module_list[3].name="propulsion";
		module_list[3].definition="def";
		module_list[3].execution="exec";
		module_list[3].initialization="0";
		module_list[3].termination="0";
		module_list[4].name="seeker";
		module_list[4].definition="def";
		module_list[4].execution="exec";
		module_list[4].initialization="0";
		module_list[4].termination="0";
		module_list[5].name="guidance";
		module_list[5].definition="def";
		module_list[5].execution="exec";
		module_list[5].initialization="0";
		module_list[5].termination="0";
		module_list[6].name="control";
		module_list[6].definition="def";
		module_list[6].execution="exec";
		module_list[6].initialization="0";
		module_list[6].termination="0";
		module_list[7].name="actuator";
		module_list[7].definition="def";
		module_list[7].execution="exec";
		module_list[7].initialization="0";
		module_list[7].termination="0";
		module_list[8].name="tvc";
		module_list[8].definition="def";
		module_list[8].execution="exec";
		module_list[8].initialization="0";
		module_list[8].termination="0";
		module_list[9].name="forces";
		module_list[9].definition="def";
		module_list[9].execution="exec";
		module_list[9].initialization="0";
		module_list[9].termination="0";
		module_list[10].name="euler";
		module_list[10].definition="def";
		module_list[10].execution="exec";
		module_list[10].initialization="0";
		module_list[10].termination="0";
		module_list[11].name="newton";
		module_list[11].definition="def";
		module_list[11].initialization="init";
		module_list[11].execution="exec";
		module_list[11].termination="0";
		module_list[12].name="intercept";
		module_list[12].definition="def";
		module_list[12].execution="exec";
		module_list[12].initialization="0";
		module_list[12].termination="0";
		scrn_step=1;
		com_step=1;
		plot_step=.05;
		traj_step=.2;
		int_step=0.002;
		num_vehicles=2;
		num_missile=1;
		num_target=1;
		if (init)
		{
				vehicle_type=new Missile(module_list,num_modules,num_target);
				// vehicle_list.add_vehicle(*vehicle_type);
		}
		
		vehicle_type->def_actuator();
		vehicle_type->def_aerodynamics();
		vehicle_type->def_control();
		vehicle_type->def_environment();
		vehicle_type->def_euler();
		vehicle_type->def_forces();
		vehicle_type->def_guidance();
		vehicle_type->def_intercept();
		vehicle_type->def_kinematics();
		vehicle_type->def_propulsion();
		vehicle_type->def_seeker();
		vehicle_type->def_tvc();
		vehicle_type->def_newton();

		// vehicle_type->vehicle_data(input, missile_rootpath, init);
		vehicle_type->set_vehicle_data(missile_type, init);
		vehicle_type->vehicle_data_init(missile_init_data, 10);

		vehicle_type->init_kinematics();
		vehicle_type->init_aerodynamics();
		vehicle_type->init_newton();

		init = false;
		delete[] module_list;
	}


	double missilesim::SimTime()
	{
		return sim_time;
	}

	int missilesim::Run()
	{
//		cout<<"=====================================environment()"<<endl;
		vehicle_type->environment();
//		cout<<"=====================================kinematics(int_step)"<<endl;
		vehicle_type->kinematics(int_step);   
		// 0.01
//		cout<<"=====================================aerodynamics()"<<endl;
		vehicle_type->aerodynamics();
//		cout<<"=====================================propulsion(max_mprop_time)"<<endl;
		vehicle_type->propulsion(max_mprop_time);  
		
		// 0.008
//		cout<<"=====================================seeker(num_vehicles, int_step, target, 7, guidance_start_time)"<<endl;
		vehicle_type->seeker(num_vehicles, int_step, target, 7, guidance_start_time);
//		cout<<"=====================================guidance(num_vehicles)"<<endl;
		vehicle_type->guidance(num_vehicles);
//		cout<<"=====================================control(int_step)"<<endl;
		vehicle_type->control(int_step);
//		cout<<"=====================================actuator(int_step)"<<endl;
		vehicle_type->actuator(int_step);  
		
		// 0.005
//		cout<<"=====================================tvc(int_step)"<<endl;
		vehicle_type->tvc(int_step);
//		cout<<"=====================================forces()"<<endl;
		vehicle_type->forces();
//		cout<<"=====================================euler(int_step)"<<endl;
		vehicle_type->euler(int_step);
//		cout<<"=====================================newton(sim_time, int_step)"<<endl;
		vehicle_type->newton(sim_time, int_step);
//		cout<<"=====================================intercept(NULL, 0, int_step, NULL)"<<endl;
		vehicle_type->intercept(NULL, 0, int_step, NULL);

//		cout<<"=====================================flat6[1].integer()"<<endl;
		sim_time += int_step;
		int i_return;
		i_return = vehicle_type->flat6[1].integer();
//		cout<<"=====================================ready to return"<<endl;
		return i_return;
	}




