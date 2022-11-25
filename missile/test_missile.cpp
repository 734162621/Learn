# include <missilesim.h>

void main()
{
	double scrn_time = 0;
	bool increment_scrn_time = false;
    missilesim *m = new missilesim;
	m->missile_init(0, 0, -10000, 0, 0, 0, 0, 0, 300,3);
	m->SimInit();

	// m->scrn_step = 0.1;
	while (m->sim_time <= 100)
	{
		m->target_input(10000, 0, -10000, 0, 0, 250, 1);

		m->Run();
		//vehicle_type->scrn_data();
		// vehicle_type->scrn_data();

		//if (fabs(scrn_time - sim_time) < (int_step / 2 + EPS))
		//{
		//	vehicle_type->scrn_data();
		//	increment_scrn_time = true;
		//	//comscrn_data(combus, num_vehicles);
		//	cout << GetMissileValue("sbel1") << "       ";
		//	cout << GetMissileValue("sbel2") << "       ";
		//	cout << GetMissileValue("sbel3") << "       ";

		//	cout << GetMissileValue("psiblx") << "       ";
		//	cout << GetMissileValue("thtblx") << "       ";
		//	cout << GetMissileValue("phiblx") << "       ";
		//}
		//if (increment_scrn_time) scrn_time += scrn_step;
		//
		//increment_scrn_time = false;
	}
	// vehicle_type->scrn_data();

}