///////////////////////////////////////////////////////////////////////////////
//FILE: 'missile_functions.cpp'
//
// Contains utilitiy functions for the 'Missile' class:
//		array sizing
//		writing banners to output
//		writing data to output
//
//001222 Created by Peter H Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

#include "class_hierarchy.hpp"

///////////////////////////////////////////////////////////////////////////////
//Determining dimensions of arrays: 'missile6', 'scrn_missile6', 'plot_missile6'
//and 'com_missile6'
// 
//Out to Missile:: nmissile6, nscrn_missile6, nplot_missile6, ncom_missile6,
//	flat6_scrn_count, missile_scrn_count, flat6_plot_count, missile_plot_count 		 ,
//
//001212 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////
void Missile::sizing_arrays()
{
	const char *key1="empty";
	const char *key2="scrn";
	const char *key3="plot";
	const char *key4="com";
	int flat6_full_count=0;
	int missile_full_count=0;
	int i(0);

	//initialize 'Flat6' and 'Missile' member variables
	flat6_scrn_count=0;
	missile_scrn_count=0;
	flat6_plot_count=0;
	missile_plot_count=0;
	flat6_com_count=0;
	missile_com_count=0;

	//counting in 'flat6' array
 	for(i=0;i<NFLAT6;i++)
	{
		if(strcmp(flat6[i].get_name(),key1))
			flat6_full_count++;
		if(strstr(flat6[i].get_out(),key2))
			flat6_scrn_count++;
		if(strstr(flat6[i].get_out(),key3))
			flat6_plot_count++;
		if(strstr(flat6[i].get_out(),key4))
			flat6_com_count++;
	}
	//counting in 'missile' array
 	for(i=0;i<NMISSILE;i++)
	{
		if(strcmp(missile[i].get_name(),key1))
			missile_full_count++;
		if(strstr(missile[i].get_out(),key2))
			missile_scrn_count++;
		if(strstr(missile[i].get_out(),key3))
			missile_plot_count++;
		if(strstr(missile[i].get_out(),key4))
			missile_com_count++;
	}
	//output to Missile::protected
	nmissile6=flat6_full_count+missile_full_count;
	nscrn_missile6=flat6_scrn_count+missile_scrn_count;
	nplot_missile6=flat6_plot_count+missile_plot_count;
	ncom_missile6=flat6_com_count+missile_com_count;
}

///////////////////////////////////////////////////////////////////////////////
//通过消除“flat6”和“飞弹”中的空插槽构建“missile6”模块阵列
//and merging the two arrays 
//
//Output: Missile::missile6[] 
//
//001212 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::vehicle_array()
{
	const char *test="empty";
	int i(0);

	//load nonempty slots from flat6 array into missile6 array
	int k=0;
	for(i=0;i<NFLAT6;i++)
	{
		if(strcmp(flat6[i].get_name(),test))
		{
			missile6[k]=flat6[i];
			k++;
		}
	}	
	//load nonempty slots from missile array into missile6 array	
	int m=0;
	for(i=0;i<NMISSILE;i++)
	{
		if(strcmp(missile[i].get_name(),test))
		{
			missile6[k+m]=missile[i];
			m++;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//Building 'scrn_missile6' module-array from 'missile6' array by keying on the word 'scrn'
//
//Output: Missile::scrn_missile6[] 
//
//001214 Created by Peter Zipfel
///////////////////////////////////////////////////////////////////////////////
void Missile::scrn_array()
{
	int k=0;
	char *buff;
	const char *key="scrn";

	for(int i=0;i<nmissile6;i++)
	{
		buff=missile6[i].get_out();
		if(strstr(buff,key))
		{
			scrn_missile6[k]=missile6[i];
			k++;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//Building 'plot_missile6' module-array from 'missile6' array by keying on the word 'plot'
//
//Output: Missile::plot_missile6[] 
//
//001214 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////
void Missile::plot_array()
{
	int k=0;
	char *buff;
	const char *key="plot";

	for(int i=0;i<nmissile6;i++)
	{
		buff=missile6[i].get_out();
		if(strstr(buff,key))
		{
			plot_missile6[k]=missile6[i];
			k++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//First label is time-since-launch-of-vehicle 'time', always at flat6[0]
//eight accross, unlimited down
//data field will be 15 spaces, total width 120 spaces
//labels longer than 14 characters will be truncated
//Accomodates 3x1 vectors
//
//010106 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::scrn_banner()
{
	char *buff1;
	char buff2[15];
	int label_length=14;
	int k=0;

	cout<<"\n Vehicle: MISSILE6 "<<'\n'; 

	for(int i=0;i<nscrn_missile6;i++)
	{
		cout.setf(ios::left);

		buff1=scrn_missile6[i].get_name();

		//truncating if more than 14 characters
		strncpy(buff2,buff1,label_length);
		buff2[14]=0;

		//Vectors are recognized by upper case character 
		if(isupper(buff2[0]))
		{
			for(int i=1;i<4;i++)
			{				
				cout.width(strlen(buff2));
				cout<<buff2;cout.width(15-strlen(buff2));cout<<i;
				k++;
				if(k>7){k=0;cout<<'\n';}
			}
		}
		else
		{
			cout.width(15);
			cout<<buff2;
			k++;
			if(k>7){k=0;cout<<'\n';}
		}
	}
	cout<<"\n\n";
}
///////////////////////////////////////////////////////////////////////////////
//Writing out banner of labels to 'tabout.asc'
//
//First label is time-since-launch-of-vehicle 'time', always at flat6[0]
//eight accross, unlimited down
//data field will be 15 spaces, total width 120 spaces
//labels longer than 14 characters will be truncated
//Accomodates 3x1 vectors
//
//010114 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::tabout_banner(ofstream &ftabout,char *title)
{
	char *buff1;
	char buff2[15];
	int label_length=14;
	int k=0;

	ftabout<<"\n"<<title<<"   "<< __DATE__ <<" "<< __TIME__ <<"\n";
	ftabout<<"\n Vehicle: MISSILE6 "<<'\n';
	

	for(int i=0;i<nscrn_missile6;i++)
	{
		ftabout.setf(ios::left);

		buff1=scrn_missile6[i].get_name();

		//truncating if more than 14 characters
		strncpy(buff2,buff1,label_length);
		buff2[14]=0;

		//Vectors are recognized by upper case character 
		if(isupper(buff2[0]))
		{
			for(int i=1;i<4;i++)
			{				
				ftabout.width(strlen(buff2));
				ftabout<<buff2;ftabout.width(15-strlen(buff2));ftabout<<i;
				k++;
				if(k>7){k=0;ftabout<<'\n';}
			}
		}
		else
		{
			ftabout.width(15);
			ftabout<<buff2;
			k++;
			if(k>7){k=0;ftabout<<'\n';}
		}
	}
	ftabout<<"\n\n";
}
///////////////////////////////////////////////////////////////////////////////
//Reading input data from input file 'input.asc' for each missile vehicle object 
//Assigning initial values to module-variables of 'flat6' and 'missile' arrays  
//
//The first missile object 'input.asc' reads until the first 'END' after 
//'MISSILE6'. The second missile object reads untile the second 'END', etc until   
//
//Output:	missile6_name ('Missile' data member)
//			flat6[] data values (Flat6 data member)
//			missile[] data values ('Missile' data member)
//			event_ptr_list[] ('Event' data members)
//
//Limitation: real and integer variables can be read only (could be expanded to vectors)  实数和整数变量只能读取（可以扩展为向量）		 
//
//001230 Created by Peter Zipfel
//010924 Added reading of random variables, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::set_vehicle_data(int missile_type,bool init)
{
	missile[1].gets((int)1); //tgt_num
	missile[165].gets((double)46); //alplimx
	missile[50].gets((int)1); //mprop
	missile[600].gets((int)2); //mact
	missile[602].gets((double)28); //dlimx
	missile[604].gets((double)600); //ddlimx
	missile[606].gets((double)0.7); //zetact
	missile[500].gets((int)2); //maut
	missile[511].gets((double)0); //phicomx
	missile[512].gets((double)20); //wrcl
	missile[513].gets((double)0.9); //zrcl
	missile[550].gets((double)0.6); //zetlagr
	missile[200].gets((int)2); //mseek
	missile[201].gets((int)1); //ms1dyn
	missile[206].gets((double)3); //dblind
	missile[208].gets((double).25); //dtimac
	missile[211].gets((double)10); //gk
	missile[212].gets((double)0.9); //zetak
	missile[213].gets((double)60); //wnk
	missile[231].gets((double)0.0314); //fovyaw
	missile[232].gets((double)0.0314); //fovpitch
	missile[215].gets((double)0); //biast
	missile[217].gets((double)0); //biasp
	missile[289].gets((double)0); //biaseh
	missile[405].gets((int)3); //mnav
	missile[500].gets((int)2); //maut

	if(missile_type == 0)
	{
		missile[51].gets((double)0.0125); //aexit
		missile[605].gets((double)100); //wnact
		missile[507].gets((double)50); //alimit
		missile[508].gets((double)10); //dqlimx
		missile[509].gets((double)10); //drlimx
		missile[510].gets((double)10); //dplimx
		missile[207].gets((double)19999); //racq
		missile[401].gets((double)3.75); //gnav
	}
	else{
		missile[51].gets((double)0.0314); //aexit
		missile[605].gets((double)600); //wnact
		missile[507].gets((double)60); //alimit
		missile[508].gets((double)2); //dqlimx
		missile[509].gets((double)2); //drlimx
		missile[510].gets((double)2); //dplimx
		missile[207].gets((double)20000); //racq
		missile[401].gets((double)3.1); //gnav
		missile[202].gets((int)1); //isets1
		missile[400].gets((int)3); //mguid
	}

	

	if (init)
	{
	aerotable.set_title("TITLE Sraam6 aero deck (from SR1S)");
			aerotable.set_capacity(16);
			aerotable.alloc_mem();
			table=new Table;
			table->set_dim(1);
			table->set_name("ca0_vs_mach");
			table->set_var1_dim(14);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [14]{0.343, 0.538, 0.685, 0.832, 0.809, 0.742, 0.74, 0.739, 0.728, 0.711, 0.697, 0.685, 0.675, 0.665};
			aerotable.set_counter(0);
	aerotable.add_table("ca0_vs_mach",table);
			table=new Table;
			table->set_dim(1);
			table->set_name("caa_vs_mach");
			table->set_var1_dim(14);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [14]{-0.0168, -0.0226, -0.0245, -0.0288, -0.029, -0.007, -0.0065, -0.0061, -0.0056, -0.0052, -0.0048, -0.0046, -0.0043, -0.0041};
			aerotable.set_counter(1);
	aerotable.add_table("caa_vs_mach",table);
			table=new Table;
			table->set_dim(1);
			table->set_name("cad_vs_mach");
			table->set_var1_dim(14);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [14]{0.0058, 0.0102, 0.011, 0.0144, 0.0134, 0.0085, 0.0064, 0.0051, 0.0043, 0.0038, 0.0034, 0.003, 0.0026, 0.0024};
			aerotable.set_counter(2);
	aerotable.add_table("cad_vs_mach",table);
			table=new Table;
			table->set_dim(1);
			table->set_name("caoff_vs_mach");
			table->set_var1_dim(14);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [14]{0.094, 0.091, 0.107, 0.144, 0.133, 0.119, 0.101, 0.085, 0.072, 0.06, 0.052, 0.045, 0.042, 0.038};
			aerotable.set_counter(3);
	aerotable.add_table("caoff_vs_mach",table);
			table=new Table;
			table->set_dim(1);
			table->set_name("clmq_vs_mach");
			table->set_var1_dim(14);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [14]{-23.1551, -20.4505, -19.0051, -18.3272, -11.1299, -8.6025, -7.083, -6.0708, -5.4161, -4.9576, -4.6164, -4.3528, -4.0976, -3.9001};
			aerotable.set_counter(4);
	aerotable.add_table("clmq_vs_mach",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("cn0_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.0, 0.72, 1.497, 2.631, 3.741, 4.369, 5.296, 6.173, 6.937, 7.7, 15.721, 19.076, 25.75, 32.322, 34.851, 0.0, 0.778, 1.582, 2.69, 3.508, 4.703, 5.853, 10.59, 14.058, 17.418, 20.861, 23.721, 29.175, 34.391, 35.801, 0.0, 0.825, 1.667, 2.76, 3.706, 4.995, 8.687, 12.101, 15.632, 19.153, 22.251, 25.324, 30.829, 35.089, 36.5, 0.0, 0.974, 1.958, 3.133, 4.341, 5.878, 10.897, 15.126, 19.308, 23.045, 26.87, 30.6, 36.12, 41.38, 43.13, 0.0, 0.913, 1.844, 2.847, 4.058, 7.614, 11.604, 15.716, 19.486, 23.412, 26.899, 29.954, 35.796, 41.201, 42.642, 0.0, 0.653, 1.283, 1.955, 4.054, 7.073, 10.184, 13.394, 16.034, 18.818, 21.678, 24.32, 29.201, 34.236, 36.067, 0.0, 0.537, 1.022, 1.579, 4.064, 6.623, 9.231, 11.629, 14.336, 17.0, 19.771, 22.505, 28.04, 33.998, 36.269, 0.0, 0.456, 0.873, 1.928, 3.978, 6.236, 8.362, 10.762, 13.307, 15.981, 18.89, 21.878, 27.567, 33.868, 36.543, 0.0, 0.401, 0.781, 1.977, 3.807, 5.721, 7.862, 10.207, 12.772, 15.652, 18.617, 21.601, 27.328, 33.836, 36.88, 0.0, 0.348, 0.704, 1.956, 3.618, 5.376, 7.426, 9.777, 12.514, 15.391, 18.347, 21.331, 27.06, 33.394, 36.112, 0.0, 0.307, 0.678, 1.924, 3.384, 5.103, 7.13, 9.594, 12.329, 15.2, 18.146, 21.12, 26.833, 33.234, 36.048, 0.0, 0.284, 0.791, 1.861, 3.219, 4.901, 6.962, 9.439, 
	12.154, 15.023, 17.963, 20.928, 26.648, 33.237, 36.312, 0.0, 0.262, 0.797, 1.804, 3.074, 4.724, 6.851, 9.312, 12.017, 14.901, 17.836, 20.786, 26.509, 33.038, 36.048, 0.0, 0.246, 0.801, 1.717, 2.953, 4.64, 6.771, 9.22, 11.924, 14.819, 17.752, 20.688, 26.396, 32.938, 36.018};
			aerotable.set_counter(5);
	aerotable.add_table("cn0_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("cnp_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.0, 0.021, 0.138, 0.151, 0.116, -0.214, -0.529, -0.737, -0.743, -0.67, -0.643, -0.688, -1.202, -1.793, -2.169, 0.0, 0.025, 0.142, 0.161, 0.18, -0.043, -0.274, -0.452, -0.507, -0.558, -0.673, -0.881, -1.691, -2.375, -2.687, 0.0, 0.026, 0.147, 0.17, 0.213, 0.038, -0.152, -0.322, -0.404, -0.507, -0.696, -0.987, -1.954, -2.695, -2.984, 0.0, 0.033, 0.177, 0.213, 0.309, 0.183, 0.032, -0.141, -0.263, -0.444, -0.761, -1.201, -2.538, -3.479, -3.776, 0.0, 0.033, 0.156, 0.203, 0.339, 0.325, 0.263, 0.136, -0.025, -0.274, -0.665, -1.172, -2.537, -3.429, -3.633, 0.0, 0.016, 0.062, 0.115, 0.207, 0.227, 0.178, 0.054, -0.144, -0.377, -0.636, -0.913, -1.489, -1.901, -2.158, 0.0, 0.007, 0.034, 0.128, 0.239, 0.291, 0.315, 0.265, 0.165, 0.016, -0.178, -0.387, -1.096, -1.798, -2.216, 0.0, 0.005, 0.025, 0.107, 0.205, 0.278, 0.299, 0.294, 0.237, 0.159, 0.025, -0.228, -0.843, -1.676, -2.297, 0.0, 0.003, 0.021, 0.091, 0.185, 0.261, 0.322, 0.337, 0.302, 0.228, 0.113, -0.06, -0.728, -1.582, -2.395, 0.0, 0.003, 0.017, 0.082, 0.165, 0.253, 0.321, 0.369, 0.348, 0.288, 0.178, -0.002, -0.705, -1.54, -2.17, 0.0, 0.003, 0.018, 0.074, 0.152, 0.24, 0.317, 0.389, 0.375, 0.321, 0.22, 0.034, -0.628, -1.476, -2.152, 0.0, 0.002, 0.025, 0.069, 0.141, 0.224, 0.316, 0.376, 0.401, 0.348, 0.255, 0.079, -0.526, -1.426, -2.229, 0.0, 0.002, 0.025, 0.066, 0.135, 0.216, 0.319, 0.38, 0.446, 0.374, 0.277, 0.112, -0.519, -1.386, -2.151, 0.0, 0.001, 0.025, 0.065, 0.131, 0.212, 0.321, 0.397, 
	0.474, 0.394, 0.286, 0.122, -0.5, -1.344, -2.143};
			aerotable.set_counter(6);
	aerotable.add_table("cnp_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("clm0_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.0, -1.157, -1.628, -3.148, -3.171, -3.349, -3.286, -4.179, -4.93, -6.3, -7.6, -8.89, -13.16, -17.7, -20.84, 0.0, -1.512, -2.468, -4.434, -4.99, -5.932, -6.9, -8.428, -10.15, -12.48, -15.61, -18.64, -26.02, -32.09, -35.63, 0.0, -1.782, -3.04, -5.358, -6.254, -7.68, -9.054, -11.242, -13.61, -16.63, -20.51, -24.37, -33.22, -39.93, -43.4, 0.0, -2.775, -5.021, -8.563, -10.505, -13.309, -16.071, -19.717, -23.62, -28.26, -33.88, -39.55, -51.9, -60.16, -63.66, 0.0, -2.144, -3.94, -6.502, -7.639, -9.673, -12.23, -15.609, -19.55, -24.27, -30.01, -36.09, -48.88, -56.92, -59.81, 0.0, -0.188, -0.263, 0.576, 2.34, 3.267, 3.704, 3.697, 3.09, 1.86, -0.05, -2.45, -7.6, -13.98, -19.65, 0.0, 0.13, 0.267, 1.698, 3.628, 4.653, 5.285, 5.47, 5.08, 3.91, 2.33, 0.34, -4.95, -11.72, -16.61, 0.0, 0.291, 0.601, 2.083, 4.016, 5.206, 5.77, 6.049, 5.8, 4.93, 3.31, 1.47, -3.08, -10.54, -16.54, 0.0, 0.402, 0.832, 2.323, 4.202, 5.511, 6.446, 6.724, 6.62, 6.04, 4.74, 2.61, -2.08, -11.07, -19.99, 0.0, 0.429, 0.889, 2.312, 4.075, 5.338, 6.344, 6.877, 6.77, 6.31, 5.28, 3.44, -1.67, -10.03, -17.1, 0.0, 0.47, 0.997, 2.364, 4.062, 5.327, 6.357, 7.077, 7.06, 6.64, 5.79, 4.2, -1.1, -9.57, -16.85, 0.0, 0.493, 1.168, 2.368, 4.004, 5.255, 6.28, 7.051, 7.24, 6.78, 5.97, 4.5, -0.73, -9.22, -16.94, 0.0, 0.522, 1.24, 2.395, 3.988, 5.227, 6.274, 7.077, 7.42, 6.86, 6.02, 4.6, -0.66, -8.75, -16.05, 0.0, 0.549, 1.299, 2.425, 3.995, 5.237, 6.303, 7.11, 7.56, 7.01, 6.11, 4.7, -0.61, -8.62, 
	-16.25};
			aerotable.set_counter(7);
	aerotable.add_table("clm0_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("clmp_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.0, -0.1, -0.821, -0.714, -0.922, -0.083, -0.299, 0.246, -0.395, -0.274, 0.379, 0.67, 3.625, 6.206, 6.836, 0.0, -0.133, -0.906, -0.93, -1.467, -1.125, -1.488, -1.172, -1.612, -1.321, -0.163, 1.14, 6.559, 10.373, 11.146, 0.0, -0.15, -0.971, -1.04, -1.743, -1.642, -2.062, -1.885, -2.245, -1.812, -0.403, 1.412, 8.11, 12.575, 13.415, 0.0, -0.216, -1.235, -1.431, -2.516, -2.682, -3.212, -3.045, -3.249, -2.509, -0.432, 2.403, 12.05, 18.309, 19.451, 0.0, -0.222, -1.124, -1.481, -2.866, -3.727, -4.697, -4.999, -5.203, -4.437, -2.329, 0.832, 11.081, 17.475, 18.326, 0.0, -0.108, -0.365, -0.872, -1.903, -2.601, -3.092, -3.372, -3.306, -2.837, -1.664, -0.237, 2.945, 5.392, 6.521, 0.0, -0.062, -0.246, -1.045, -2.048, -2.717, -3.304, -3.904, -4.019, -3.449, -2.321, -1.482, 1.552, 4.845, 5.632, 0.0, -0.039, -0.168, -0.739, -1.561, -2.479, -3.002, -3.553, -3.927, -3.594, -2.52, -1.183, 0.694, 4.166, 5.61, 0.0, -0.027, -0.129, -0.61, -1.197, -1.874, -2.821, -3.321, -3.696, -3.943, -3.482, -2.081, 0.096, 3.631, 6.621, 0.0, -0.02, -0.104, -0.519, -1.026, -1.48, -2.338, -3.21, -3.523, -3.78, -3.692, -2.813, -0.229, 3.676, 5.775, 0.0, -0.017, -0.102, -0.453, -0.9, -1.282, -1.933, -3.03, -3.406, -3.617, -3.682, -3.217, -0.493, 3.454, 5.701, 0.0, -0.015, -0.155, -0.404, -0.804, -1.172, -1.68, -2.651, -3.416, -3.558, -3.652, -3.329, -0.594, 3.179, 5.727, 0.0, -0.013, -0.151, -0.359, -0.728, -1.061, -1.502, -2.379, -3.62, -3.691, -3.754, -3.401, -0.486, 3.122, 
	5.465, 0.0, -0.01, -0.148, -0.326, -0.672, -0.982, -1.399, -2.179, -3.696, -3.88, -3.865, -3.462, -0.374, 2.991, 5.526};
			aerotable.set_counter(8);
	aerotable.add_table("clmp_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("cyp_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.0, -0.017, -0.122, -0.171, -0.383, -0.489, -0.507, -0.506, -0.479, -0.475, -0.477, -0.411, 0.195, 1.784, 2.833, 0.0, -0.019, -0.12, -0.187, -0.435, -0.61, -0.719, -0.836, -0.893, -0.91, -0.823, -0.654, 0.204, 2.42, 3.511, 0.0, -0.02, -0.123, -0.197, -0.467, -0.68, -0.849, -1.0, -1.084, -1.102, -0.988, -0.774, 0.211, 2.766, 3.898, 0.0, -0.025, -0.144, -0.251, -0.606, -0.917, -1.196, -1.447, -1.587, -1.61, -1.465, -1.172, 0.131, 3.553, 4.933, 0.0, -0.025, -0.12, -0.244, -0.59, -0.929, -1.259, -1.544, -1.69, -1.694, -1.526, -1.203, 0.089, 3.467, 4.747, 0.0, -0.012, -0.043, -0.178, -0.43, -0.661, -0.883, -1.035, -1.076, -1.002, -0.85, -0.628, 0.054, 1.96, 2.819, 0.0, -0.006, -0.018, -0.173, -0.412, -0.594, -0.733, -0.848, -0.944, -1.013, -1.027, -0.874, -0.36, 1.487, 2.896, 0.0, -0.004, -0.015, -0.137, -0.33, 
	-0.488, -0.626, -0.749, -0.847, -0.925, -0.977, -0.85, -0.403, 1.259, 3.001, 0.0, -0.003, -0.012, -0.114, -0.276, -0.42, -0.553, -0.692, -0.817, -0.909, -0.953, -0.842, -0.373, 1.084, 3.131, 0.0, -0.002, -0.011, -0.099, -0.239, -0.369, -0.499, -0.637, -0.779, -0.898, -0.949, -0.83, -0.335, 1.054, 2.836, 0.0, -0.002, -0.012, -0.088, -0.212, -0.332, -0.458, -0.596, -0.737, -0.878, -0.939, -0.817, -0.32, 0.965, 2.812, 0.0, -0.002, -0.022, -0.079, -0.191, -0.303, -0.425, -0.557, -0.699, -0.839, -0.922, -0.8, -0.33, 0.863, 2.913, 0.0, -0.001, -0.022, -0.072, -0.174, -0.28, -0.4, -0.531, -0.68, -0.823, -0.925, -0.793, -0.305, 0.826, 2.812, 
	0.0, -0.001, -0.022, -0.067, -0.161, -0.265, -0.38, -0.517, -0.667, -0.816, -0.928, -0.788, -0.28, 0.773, 2.8};
			aerotable.set_counter(9);
	aerotable.add_table("cyp_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("clnp_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.0, 0.084, 0.774, 0.979, 2.526, 3.814, 5.286, 6.842, 8.435, 9.656, 9.319, 9.069, 4.866, -5.554, -8.933, 0.0, 0.107, 0.792, 1.183, 3.05, 4.798, 6.694, 8.264, 9.729, 10.695, 10.684, 10.141, 5.184, -9.582, -14.563, 0.0, 0.121, 0.822, 1.296, 3.347, 5.346, 7.318, 9.209, 10.805, 11.781, 11.777, 11.023, 5.439, -11.696, -17.527, 0.0, 0.167, 1.012, 1.749, 4.48, 7.227, 9.894, 12.438, 14.393, 15.532, 15.376, 14.176, 6.611, -17.117, -25.415, 0.0, 0.168, 0.866, 1.787, 4.557, 7.517, 10.542, 13.362, 15.44, 16.541, 16.507, 15.466, 8.334, -15.38, -23.944, 0.0, 0.088, 0.284, 1.291, 3.353, 5.46, 7.553, 9.314, 10.509, 11.088, 11.172, 10.863, 8.516, -3.449, -8.521, 0.0, 0.05, 0.132, 1.356, 3.313, 4.892, 6.129, 7.413, 8.748, 9.985, 10.887, 10.611, 8.904, -0.966, -7.358, 0.0, 0.032, 0.097, 1.038, 2.522, 3.83, 5.052, 6.342, 7.59, 
	8.736, 9.611, 9.197, 7.499, -0.105, -7.33, 0.0, 0.022, 0.079, 0.852, 2.053, 3.069, 4.132, 5.36, 6.701, 7.966, 8.962, 8.594, 6.897, 0.987, -8.651, 0.0, 0.017, 0.067, 0.722, 1.742, 2.597, 3.482, 4.6, 5.899, 7.247, 8.304, 8.027, 6.447, 1.063, -7.544, 0.0, 0.013, 0.075, 0.627, 1.515, 2.272, 3.03, 4.062, 5.264, 6.674, 7.723, 7.546, 6.061, 1.296, -7.449, 0.0, 0.012, 0.149, 0.555, 1.342, 2.031, 2.707, 3.633, 4.787, 6.111, 7.253, 7.074, 5.645, 1.5, -7.483, 0.0, 0.009, 0.152, 0.485, 1.184, 1.814, 2.429, 3.274, 4.444, 5.76, 7.051, 6.796, 5.266, 1.528, -7.14, 0.0, 0.008, 0.148, 0.432, 1.064, 1.649, 2.232, 3.014, 4.185, 5.533, 6.894, 6.609, 4.991, 1.628, -7.22};
			aerotable.set_counter(10);
	aerotable.add_table("clnp_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("cndq_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.137, 0.134, 0.139, 0.153, 0.163, 0.147, 0.111, 0.102, 0.067, 0.055, 0.057, 0.047, 0.052, 0.022, 0.022, 0.157, 0.153, 0.159, 0.175, 0.187, 0.176, 0.149, 0.142, 0.114, 0.102, 0.101, 0.088, 0.074, 0.021, 0.022, 0.169, 0.165, 0.171, 0.188, 0.201, 0.193, 0.169, 0.164, 0.139, 0.127, 0.124, 0.109, 0.085, 0.022, 0.022, 0.212, 0.208, 0.215, 0.236, 0.252, 0.246, 0.224, 0.22, 0.196, 0.181, 0.175, 
	0.154, 0.114, 0.025, 0.026, 0.187, 0.183, 0.19, 0.206, 0.219, 0.219, 0.21, 0.21, 0.199, 0.188, 0.179, 0.159, 0.111, 0.019, 0.02, 0.112, 0.112, 0.113, 0.117, 0.12, 0.118, 0.115, 0.109, 0.101, 0.092, 0.082, 0.069, 0.042, 0.018, 0.019, 0.082, 0.082, 0.083, 0.086, 0.09, 0.091, 0.089, 0.088, 0.086, 0.084, 0.077, 0.068, 0.054, 0.022, 0.023, 0.064, 0.064, 0.065, 0.067, 0.07, 0.073, 0.077, 0.074, 0.074, 0.073, 0.074, 0.068, 0.06, 0.032, 0.033, 0.053, 0.053, 0.054, 0.055, 0.058, 0.06, 0.064, 0.069, 0.068, 0.067, 0.069, 0.071, 0.056, 0.046, 0.047, 0.045, 0.045, 0.046, 0.047, 0.049, 0.052, 0.056, 0.061, 0.065, 0.063, 0.064, 0.068, 0.058, 0.036, 
	0.036, 0.039, 0.039, 0.04, 0.041, 0.043, 0.046, 0.05, 0.054, 0.062, 0.061, 0.061, 0.064, 0.057, 0.038, 0.038, 0.035, 0.035, 0.035, 0.037, 0.039, 0.042, 0.045, 0.05, 0.056, 0.061, 0.059, 0.062, 0.058, 0.044, 0.044, 
	0.031, 0.031, 0.031, 0.033, 0.034, 0.038, 0.042, 0.047, 0.052, 0.063, 0.058, 0.06, 0.058, 0.041, 0.042, 0.028, 0.027, 0.028, 0.029, 0.031, 0.035, 0.039, 0.044, 0.05, 0.063, 0.058, 0.058, 0.058, 0.043, 0.044};      
			aerotable.set_counter(11);
	aerotable.add_table("cndq_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("clmdq_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{-1.052, -1.027, -1.062, -1.169, -1.25, -1.127, -0.849, -0.78, -0.515, -0.417, -0.434, -0.359, -0.401, -0.166, -0.169, -1.2, -1.171, -1.214, -1.334, -1.427, -1.347, -1.137, -1.086, -0.873, -0.783, -0.77, -0.671, -0.562, -0.163, -0.165, -1.293, -1.263, -1.309, -1.436, -1.536, -1.474, -1.295, -1.251, -1.061, -0.972, -0.945, -0.833, -0.65, -0.164, -0.167, -1.634, -1.597, -1.654, -1.811, -1.935, -1.889, -1.722, -1.691, -1.504, -1.394, -1.347, -1.187, -0.879, -0.193, -0.195, -1.475, -1.442, -1.492, -1.621, -1.724, -1.726, -1.656, -1.657, -1.565, -1.479, -1.41, -1.254, -0.876, -0.153, -0.155, -0.893, -0.891, -0.901, -0.931, -0.955, -0.944, -0.913, -0.866, -0.806, -0.731, -0.655, -0.554, -0.334, -0.147, -0.149, -0.652, -0.651, -0.658, -0.683, -0.716, -0.727, -0.713, -0.703, -0.69, -0.671, -0.61, -0.543, -0.43, -0.177, -0.18, -0.51, -0.509, -0.515, -0.533, -0.556, -0.579, -0.615, -0.591, -0.587, -0.585, -0.59, -0.54, -0.48, -0.258, -0.263, -0.42, -0.419, -0.424, -0.439, -0.458, -0.48, -0.512, -0.55, -0.542, -0.531, -0.548, -0.568, -0.448, -0.369, -0.376, -0.358, -0.356, -0.362, -0.374, -0.391, -0.413, -0.443, -0.485, -0.52, -0.502, -0.51, -0.539, -0.458, -0.282, -0.287, -0.312, -0.31, -0.316, -0.327, -0.342, -0.366, -0.396, -0.432, -0.495, -0.484, -0.48, -0.508, -0.453, -0.298, -0.304, -0.277, -0.275, -0.279, -0.29, -0.305, -0.329, -0.36, -0.397, -0.447, -0.48, -0.465, -0.489, -0.463, -0.346, -0.352, -0.243, -0.241, -0.246, -0.257, -0.272, -0.298, -0.328, -0.369, -0.414, -0.495, -0.456, -0.474, -0.464, -0.326, -0.332, -0.217, -0.215, -0.22, -0.231, -0.247, -0.275, -0.306, -0.349, -0.395, -0.499, -0.457, -0.46, -0.462, -0.341, -0.347};
			aerotable.set_counter(12);
	aerotable.add_table("clmdq_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("cllap_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.0, -0.0012, -0.0017, -0.0007, -0.001, -0.001, -0.001, -0.0009, -0.0007, -0.0006, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0, -0.0014, -0.0016, -0.0008, -0.0011, -0.0011, -0.001, -0.0009, -0.0008, -0.0006, -0.0005, -0.0004, -0.0003, -0.0001, 0.0, 0.0, -0.0016, -0.0017, -0.0009, -0.0011, -0.0012, -0.0011, -0.001, -0.0008, -0.0007, -0.0006, -0.0005, -0.0003, -0.0001, 0.0, 0.0, -0.0021, -0.002, -0.0011, -0.0014, -0.0015, -0.0014, -0.0012, -0.0011, -0.0009, -0.0008, -0.0007, -0.0004, -0.0001, 0.0, 0.0, -0.0021, -0.0018, -0.0012, -0.0014, -0.0014, -0.0013, -0.0012, -0.0011, -0.0009, -0.0008, -0.0007, -0.0005, -0.0001, 0.0, 0.0, -0.0013, -0.0008, -0.0011, -0.0012, -0.0011, -0.001, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0003, -0.0001, 0.0, 0.0, -0.0008, -0.0003, -0.001, -0.0011, -0.001, -0.0008, -0.0007, -0.0006, -0.0006, -0.0005, -0.0005, -0.0003, -0.0001, 0.0, 0.0, -0.0006, -0.0002, -0.0009, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0005, -0.0005, -0.0004, -0.0003, -0.0001, 0.0, 0.0, -0.0003, -0.0002, -0.0007, -0.0008, -0.0006, -0.0005, -0.0005, -0.0005, -0.0005, -0.0004, -0.0004, -0.0003, -0.0001, 0.0, 0.0, -0.0003, -0.0002, -0.0006, -0.0007, -0.0006, -0.0005, -0.0004, -0.0004, -0.0004, -0.0004, -0.0003, -0.0003, 
	-0.0001, 0.0, 0.0, -0.0002, -0.0002, -0.0005, -0.0006, -0.0005, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0, -0.0002, -0.0004, -0.0005, -0.0005, -0.0004, -0.0004, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0002, -0.0001, 0.0, 0.0, -0.0001, -0.0004, -0.0004, -0.0005, -0.0004, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0002, -0.0001, 0.0, 0.0, -0.0001, -0.0004, -0.0004, -0.0004, -0.0004, -0.0003, -0.0003, -0.0002, -0.0003, -0.0003, -0.0003, -0.0002, -0.0001, 0.0};
			aerotable.set_counter(13);
	aerotable.add_table("cllap_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("clldp_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{0.084, 0.085, 0.088, 0.09, 0.084, 0.066, 0.052, 0.038, 0.029, 0.028, 0.024, 0.028, 0.026, 0.01, 0.01, 0.096, 0.097, 0.1, 0.103, 0.098, 0.084, 0.074, 0.063, 0.056, 0.054, 0.049, 
	0.049, 0.034, 0.009, 0.009, 0.103, 0.105, 0.109, 0.111, 0.107, 0.095, 0.086, 0.077, 0.07, 0.067, 0.06, 0.058, 0.037, 0.008, 0.008, 0.129, 0.132, 0.136, 0.139, 0.137, 0.126, 0.118, 0.107, 0.098, 0.093, 0.084, 0.077, 0.049, 0.009, 0.009, 0.119, 0.121, 0.126, 0.13, 0.13, 0.124, 0.12, 0.114, 0.107, 0.102, 0.093, 0.083, 0.05, 0.006, 0.006, 0.075, 0.075, 0.075, 0.074, 0.073, 0.071, 0.068, 0.063, 0.058, 0.052, 0.045, 0.037, 0.021, 
	0.009, 0.009, 0.057, 0.057, 0.058, 0.059, 0.059, 0.059, 0.058, 0.057, 0.055, 0.052, 0.046, 0.042, 0.029, 0.012, 0.012, 0.046, 0.046, 0.047, 0.048, 0.049, 0.052, 0.052, 0.05, 0.05, 0.05, 0.049, 0.043, 0.038, 0.02, 0.019, 0.039, 0.039, 0.04, 0.041, 0.042, 0.045, 0.048, 0.049, 0.048, 0.048, 0.049, 0.049, 0.037, 0.03, 0.029, 0.034, 0.034, 0.035, 0.036, 0.037, 0.04, 0.043, 0.047, 0.048, 0.046, 0.048, 0.05, 0.039, 0.023, 0.023, 0.03, 0.03, 0.031, 0.032, 0.033, 0.036, 0.039, 0.044, 0.047, 0.044, 0.046, 0.049, 0.04, 0.025, 0.025, 0.027, 0.027, 0.028, 0.029, 0.031, 0.033, 0.036, 0.041, 0.045, 0.045, 0.045, 0.048, 0.041, 0.03, 0.03, 0.024, 0.024, 0.025, 0.027, 0.028, 0.031, 0.034, 0.038, 0.044, 0.048, 0.044, 0.047, 0.041, 0.029, 0.028, 0.022, 0.022, 0.023, 0.025, 0.026, 0.029, 0.033, 0.037, 0.044, 0.049, 0.044, 0.046, 0.041, 0.031, 0.03};
			aerotable.set_counter(14);
	aerotable.add_table("clldp_vs_mach_alpha",table);
			table=new Table;
			table->set_dim(2);
			table->set_name("cllp_vs_mach_alpha");
			table->set_var1_dim(14);
			table->set_var2_dim(15);
			table->var1_values=new double [14]{0.6, 0.85, 0.95, 1.05, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.5, 5.0};
			table->var2_values=new double [15]{0.0, 3.0, 6.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 75.0, 90.0};
			table->var3_values=new double [1];
			table->data=new double [210]{-0.156, -0.158, -0.163, -0.167, -0.157, -0.122, -0.097, -0.071, -0.053, -0.052, -0.045, -0.053, -0.048, -0.019, -0.019, -0.178, -0.181, -0.186, -0.19, -0.183, -0.157, -0.137, -0.118, -0.105, -0.101, -0.091, -0.09, -0.063, -0.016, -0.016, -0.191, -0.195, -0.202, -0.206, -0.199, -0.176, -0.16, -0.142, -0.129, -0.124, -0.112, -0.107, -0.069, -0.015, -0.015, -0.239, -0.244, -0.252, -0.258, -0.254, -0.234, -0.218, -0.199, -0.183, -0.173, -0.156, -0.143, -0.09, -0.016, -0.016, -0.221, -0.225, -0.234, -0.242, -0.241, -0.231, -0.223, -0.212, -0.199, -0.189, -0.173, -0.154, -0.094, -0.012, -0.012, -0.138, -0.139, -0.139, -0.138, -0.136, -0.132, -0.125, -0.117, -0.107, -0.096, -0.084, -0.069, -0.039, -0.017, -0.017, -0.105, -0.106, -0.107, -0.109, -0.11, -0.109, -0.107, -0.105, -0.102, -0.096, -0.085, -0.078, -0.053, 
	-0.022, -0.022, -0.086, -0.086, -0.088, -0.089, -0.091, -0.096, -0.097, -0.094, -0.093, -0.093, -0.09, -0.079, -0.07, -0.036, -0.036, -0.072, -0.073, -0.075, -0.076, -0.078, -0.083, -0.089, -0.092, -0.088, -0.089, 
	-0.091, -0.091, -0.069, -0.056, -0.055, -0.063, -0.064, -0.065, -0.067, -0.069, -0.074, -0.08, -0.087, -0.088, -0.085, -0.088, -0.092, -0.072, -0.043, -0.042, -0.056, -0.056, -0.058, -0.06, -0.062, -0.067, -0.073, 
	-0.082, -0.088, -0.082, -0.085, -0.091, -0.074, -0.047, -0.046, -0.05, -0.051, -0.052, -0.054, -0.057, -0.062, -0.068, -0.075, -0.084, -0.084, -0.083, -0.088, -0.076, -0.056, -0.055, -0.045, -0.045, -0.047, -0.049, -0.052, -0.057, -0.063, -0.071, -0.082, -0.089, -0.081, -0.086, -0.075, -0.054, -0.053, -0.04, -0.041, -0.043, -0.046, -0.048, -0.054, -0.061, -0.068, -0.081, -0.091, -0.081, -0.085, -0.075, -0.058, -0.056};      
			aerotable.set_counter(15);
	aerotable.add_table("cllp_vs_mach_alpha",table);
	}

	if (init)
	{
	proptable.set_title("TITLE Sraam6 Propulsion Deck");
			proptable.set_capacity(5);
			proptable.alloc_mem();
			table=new Table;
			table->set_dim(1);
			table->set_name("mass_vs_time");
			table->set_var1_dim(13);
			table->var1_values=new double [13]{0.0, 0.05, 0.269, 0.538, 0.807, 1.076, 1.345, 1.614, 1.883, 2.152, 2.421, 2.69, 6.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [13]{91.9563, 91.0355, 88.949, 85.1842, 81.1246, 77.6092, 73.8308, 70.1749, 66.6596, 63.2849, 59.6698, 56.2951, 56.2951};
			proptable.set_counter(0);
	proptable.add_table("mass_vs_time",table);
			table=new Table;
			table->set_dim(1);
			table->set_name("thrust_vs_time");
			table->set_var1_dim(13);
			table->var1_values=new double [13]{0.0, 0.05, 0.269, 0.538, 0.807, 1.076, 1.345, 1.614, 1.883, 2.152, 2.421, 2.69, 6.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [13]{0.0, 31763.0, 34459.0, 35953.0, 36069.0, 35451.0, 34677.0, 33938.0, 33195.0, 32346.0, 31274.0, 0.0, 0.0};
			proptable.set_counter(1);
	proptable.add_table("thrust_vs_time",table);
			table=new Table;
			table->set_dim(1);
			table->set_name("moipitch_vs_time");
			table->set_var1_dim(13);
			table->var1_values=new double [13]{0.0, 0.05, 0.269, 0.538, 0.807, 1.076, 1.345, 1.614, 1.883, 2.152, 2.421, 2.69, 6.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [13]{59.8043, 59.7501, 57.9604, 56.1844, 54.4083, 52.822, 51.195, 49.7307, 48.28, 46.9107, 45.6498, 44.3618, 44.3618};
			proptable.set_counter(2);
	proptable.add_table("moipitch_vs_time",table);
			table=new Table;
			table->set_dim(1);
			table->set_name("moiroll_vs_time");
			table->set_var1_dim(13);
			table->var1_values=new double [13]{0.0, 0.05, 0.269, 0.538, 0.807, 1.076, 1.345, 1.614, 1.883, 2.152, 2.421, 2.69, 6.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [13]{0.3078, 0.3078, 0.2996, 0.2929, 0.2847, 0.2752, 0.2657, 0.2562, 0.244, 0.2305, 0.2169, 0.202, 0.202};
			proptable.set_counter(3);
	proptable.add_table("moiroll_vs_time",table);
			table=new Table;
			table->set_dim(1);
			table->set_name("cg_vs_time");
			table->set_var1_dim(13);
			table->var1_values=new double [13]{0.0, 0.05, 0.269, 0.538, 0.807, 1.076, 1.345, 1.614, 1.883, 2.152, 2.421, 2.69, 6.0};
			table->var2_values=new double [1];
			table->var3_values=new double [1];
			table->data=new double [13]{1.5357, 1.5339, 1.5067, 1.4778, 1.4488, 1.4191, 1.3891, 1.3569, 1.3238, 1.2896, 1.2565, 1.2205, 1.2205};
			proptable.set_counter(4);
	proptable.add_table("cg_vs_time",table);
	}
}

void Missile::vehicle_data_init(double *arr,int length) {

	//for (int i = 0; i < length; i++)
	//{
	//	cout << arr[i] << " ";
	//}
	//cout << endl;
	
	flat6[220].gets(arr[0]);
	flat6[221].gets(arr[1]);
	flat6[222].gets(arr[2]);  

	flat6[236].gets(arr[8]); 
	flat6[237].gets(arr[6]);  
	flat6[238].gets(arr[7]);  

	flat6[137].gets(arr[3]);  
	flat6[138].gets(arr[4]);  
	flat6[139].gets(arr[5]); 

	int int_data = (int)arr[9];
	missile[500].gets(int_data); 
}


double Missile::vehicle_data_OUT(char* name) {

	char* buff = NULL;
	double data;
	int  i(0);
	char* integer = NULL;

	for (i = 0; i < NFLAT6; i++)
	{
		buff = flat6[i].get_name();
		if (!strcmp(buff, name))
		{
			//加载实际值
			integer = flat6[i].get_type();
			if (!strcmp(integer, "int"))
			{
				//加载整数值
				data = (double)flat6[i].integer();
			}
			else
			{
				//加载实际值
				data = flat6[i].real();
			}
			return data;
			break;
		}
	}

	for (i = 0; i < NMISSILE; i++)
	{
		buff = missile[i].get_name();
		if (!strcmp(buff, name))
		{
			//加载实际值
			integer = missile[i].get_type();
			if (!strcmp(integer, "int"))
			{
				//加载整数值
				data = (double)missile[i].integer();
			}
			else
			{
				//加载实际值
				data = missile[i].real();
			}
			return data;
			break;
		}
	}
	return 0;
}

void Missile::vehicle_data_Input(char* name, double data)
{
	char* buff = NULL;
	int  i(0);
	char* integer = NULL;
	int int_data(0);

	for (i = 0; i < NFLAT6; i++)
	{
		buff = flat6[i].get_name();
		if (!strcmp(buff, name))
		{
		
			integer = flat6[i].get_type();
			if (!strcmp(integer, "int"))
			{

				int_data = (int)data;
				flat6[i].gets(int_data);
			}
			else
			{
		
				flat6[i].gets(data);
			}
			break;
		}
	}

	for (i = 0; i < NMISSILE; i++)
	{
		buff = missile[i].get_name();
		if (!strcmp(buff, name))
		{
	
			integer = missile[i].get_type();
			if (!strcmp(integer, "int"))
			{
		
				int_data = (int)data;
				missile[i].gets(int_data);
			}
			else
			{
		
				missile[i].gets(data);
			}
			break;
		}
	}

}

void Missile::vehicle_Reset() {

	int  i(0);
	Matrix TTL = missile[4].mat();
	Matrix BIASAI = missile[236].vec();
	for (i = 0; i < NFLAT6; i++)
	{
		flat6[i].gets(0);
	}
	for (i = 0; i < NMISSILE; i++)
	{
		missile[i].gets(0);
	}

	missile[4].gets_mat(TTL);
	missile[236].gets_vec(BIASAI);

	missile[59].gets(92);
	missile[64].gets(1.536);
	missile[70].gets(1.536);
	missile[71].gets(0.308);
	missile[72].gets(59.80);
}
	



///////////////////////////////////////////////////////////////////////////////
//Building index array of those 'flat6[]' and missile[] variables 
//that are output to screen  
//
//Output: Missile::flat6_scrn_ind[], missile_crn_ind[] 
//
//001213 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::scrn_index_arrays()
{
	const char *test="scrn";
	int i(0);

	int k=0;
	for(i=0;i<NFLAT6;i++)
	{
		if(strstr(flat6[i].get_out(),test))
		{
			flat6_scrn_ind[k]=i;
			k++;
		}
	}
	int l=0;
	for(i=0;i<NMISSILE;i++)
	{
		if(strstr(missile[i].get_out(),test))
		{
			missile_scrn_ind[l]=i;
			l++;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//Writing data to screen
//
//First label is time-since-launch-of-vehicle 'time', always at flat6[0]
//Accomodates real, integers (printed as real) and 3x1 vectors 
//eight accross, unlimited down
//data field 15 spaces, total width 120 spaces
//
//010112 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::scrn_data()
{

	int index;
	char *integer;
	char *vector;
	Matrix VEC(3,1);
	int k(0),i(0);
	
	cout<<missile6_name<<'\n';
	cout.setf(ios::left);

	//writing to screen the variables from the 'Flat6' class
	for(i=0;i<flat6_scrn_count;i++)
	{
		index=flat6_scrn_ind[i];
		//checking for integers
		integer=flat6[index].get_type();
		vector=flat6[index].get_name();
		if(!strcmp(integer,"int"))
		{
			cout.width(15);
			cout<<flat6[index].integer();			
			k++; if(k>7){k=0;cout<<'\n';}
		}
		//checking vor vectors
		else if(isupper(vector[0]))
		{
			VEC=flat6[index].vec();

			cout.width(15);
			cout<<VEC.get_loc(0,0);
			k++; if(k>7){k=0;cout<<'\n';}
			cout.width(15);
			cout<<VEC.get_loc(1,0);
			k++; if(k>7){k=0;cout<<'\n';}
			cout.width(15);
			cout<<VEC.get_loc(2,0);
			k++; if(k>7){k=0;cout<<'\n';}
		}
		else
		{
			//real variables
			cout.width(15);
			cout<<flat6[index].real();
			k++; if(k>7){k=0;cout<<'\n';}
		}
	}
	//writing to screen the variables from the 'Missile' class
	for(i=0;i<missile_scrn_count;i++)
	{
		index=missile_scrn_ind[i];
		//checking for integers
		integer=missile[index].get_type();
		vector=missile[index].get_name();
		if(!strcmp(integer,"int"))
		{
			cout.width(15);
			cout<<missile[index].integer();			
			k++; if(k>7){k=0;cout<<'\n';}

		}
		//checking vor vectors
		else if(isupper(vector[0]))
		{
			VEC=missile[index].vec();
			cout.width(15);
			cout<<VEC.get_loc(0,0);
			k++; if(k>7){k=0;cout<<'\n';}
			cout.width(15);
			cout<<VEC.get_loc(1,0);
			k++; if(k>7){k=0;cout<<'\n';}
			cout.width(15);
			cout<<VEC.get_loc(2,0);
			k++; if(k>7){k=0;cout<<'\n';}
		}
		else
		{
			//real variables
			cout.width(15);
			cout<<missile[index].real();
			k++; if(k>7){k=0;cout<<'\n';}
		}
	}
	cout<<"\n";
}
///////////////////////////////////////////////////////////////////////////////
//Writing data to 'tabout.asc'
//
//First label is time-since-launch-of-vehicle 'time', must be at flat6[0]
//Accomodates real, integers (printed as real) and 3x1 vectors 
//eight accross, unlimited down
//data field 15 spaces, total width 120 spaces
//
//010114 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::tabout_data(ofstream &ftabout)
{

	int index;
	char *integer;
	char *vector;
	Matrix VEC(3,1);
	int i(0),k(0);
	
	ftabout<<missile6_name<<'\n';
	ftabout.setf(ios::left);

	//writing to 'tabout.asc' the variables from the 'Flat6' class
	for(i=0;i<flat6_scrn_count;i++)
	{
		index=flat6_scrn_ind[i];
		//checking for integers
		integer=flat6[index].get_type();
		vector=flat6[index].get_name();
		if(!strcmp(integer,"int"))
		{
			ftabout.width(15);
			ftabout<<flat6[index].integer();			
			k++; if(k>7){k=0;ftabout<<'\n';}
		}
		//checking vor vectors
		else if(isupper(vector[0]))
		{
			VEC=flat6[index].vec();

			ftabout.width(15);
			ftabout<<VEC.get_loc(0,0);
			k++; if(k>7){k=0;ftabout<<'\n';}
			ftabout.width(15);
			ftabout<<VEC.get_loc(1,0);
			k++; if(k>7){k=0;ftabout<<'\n';}
			ftabout.width(15);
			ftabout<<VEC.get_loc(2,0);
			k++; if(k>7){k=0;ftabout<<'\n';}
		}
		else
		{
			//real variables
			ftabout.width(15);
			ftabout<<flat6[index].real(); 
			k++; if(k>7){k=0;ftabout<<'\n';}
		}
	}
	//writing to 'tabout.asc' the variables from the 'Missile' class
	for(i=0;i<missile_scrn_count;i++)
	{
		index=missile_scrn_ind[i];
		//checking for integers
		integer=missile[index].get_type();
		vector=missile[index].get_name();
		if(!strcmp(integer,"int"))
		{
			ftabout.width(15);
			ftabout<<missile[index].integer();			
			k++; if(k>7){k=0;ftabout<<'\n';}

		}
		//checking vor vectors
		else if(isupper(vector[0]))
		{
			VEC=missile[index].vec();
			ftabout.width(15);
			ftabout<<VEC.get_loc(0,0);
			k++; if(k>7){k=0;ftabout<<'\n';}
			ftabout.width(15);
			ftabout<<VEC.get_loc(1,0);
			k++; if(k>7){k=0;ftabout<<'\n';}
			ftabout.width(15);
			ftabout<<VEC.get_loc(2,0);
			k++; if(k>7){k=0;ftabout<<'\n';}
		}
		else
		{
			//real variables
			ftabout.width(15);
			ftabout<<missile[index].real();
			k++; if(k>7){k=0;ftabout<<'\n';}
		}
	}
	ftabout<<"\n";
}
///////////////////////////////////////////////////////////////////////////////
//Writing out banner of labels to 'ploti.asc', i=1,2,3...
//
//First label is time-since-launch-of-vehicle 'time', always at flat6[0]
//five accross, unlimited down
//data field width 16 spaces, total width 80 spaces
//labels longer than 8 characters will be truncated
//Accomodates 3x1 vectors
//
//010115 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::plot_banner(ofstream &fplot,char *title)
{
	char *buff1;
	char buff2[15];
	int label_length=8;
	int i(0),k(0),m(0);

	fplot<<"1"<<title<<" '"<<missile6_name<<" ' "<< __DATE__ <<" "<< __TIME__ <<"\n";
	
	//determining the number vector variables
	for(i=0;i<nplot_missile6;i++)
	{
		buff1=plot_missile6[i].get_name();
		if(isupper(buff1[0])) m++;
	}
	//increase number of variables by vector components
	int nvariables=nplot_missile6+2*m;
	
	fplot<<"  0  0 " <<nvariables<<"\n"; 

	//writing banner to plot file 'ploti.asc'
	for(i=0;i<nplot_missile6;i++)
	{
		fplot.setf(ios::left);

		buff1=plot_missile6[i].get_name();

		//truncating if more than 8 characters
		strncpy(buff2,buff1,label_length);
		buff2[8]=0;

		//Vectors are recognized by upper case character 
		if(isupper(buff2[0]))
		{
			for(int i=1;i<4;i++)
			{				
				fplot.width(strlen(buff2));
				fplot<<buff2;fplot.width(16-strlen(buff2));fplot<<i;
				k++;
				if(k>4){k=0;fplot<<'\n';}
			}
		}
		else
		{
			fplot.width(16);
			fplot<<buff2;
			k++;
			if(k>4){k=0;fplot<<'\n';}
		}
	}
	if((nvariables%5))fplot<<"\n";
}
///////////////////////////////////////////////////////////////////////////////
//Building index array of those 'flat6[]' and missile[] variables that are  
//output to 'ploti.asc'
//
//Output: Missile::flat6_plot_ind[], missile_plot_ind[] 
//
//001213 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::plot_index_arrays()
{
	const char *test="plot";
	int i(0);

	int k=0;
	for(i=0;i<NFLAT6;i++)
	{
		if(strstr(flat6[i].get_out(),test))
		{
			flat6_plot_ind[k]=i;
			k++;
		}
	}
	int l=0;
	for(i=0;i<NMISSILE;i++)
	{
		if(strstr(missile[i].get_out(),test))
		{
			missile_plot_ind[l]=i;
			l++;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//Writing data to 'ploti.asc', i=1,2,3...
//
//Accomodates real, integers (printed as real) and 3x1 vectors 
//five accross, unlimited down
//data field 16 spaces, total width 80 spaces
//
//010116 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::plot_data(ofstream &fplot,bool merge)
{

	int index;
	char *integer;
	char *vector;
	Matrix VEC(3,1);
	int i(0),k(0);
	
	fplot.setf(ios::left);

	//writing to 'ploti.asc' the variables from the 'Flat6' class
	for(i=0;i<flat6_plot_count;i++)
	{
		index=flat6_plot_ind[i];
		//checking for integers
		integer=flat6[index].get_type();
		vector=flat6[index].get_name();
		if(!strcmp(integer,"int"))
		{
			//casting integer to real variable
			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<(double) flat6[index].integer();			
			k++;
		}
		//checking vor vectors
		else if(isupper(vector[0]))
		{
			VEC=flat6[index].vec();

			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<VEC.get_loc(0,0);
			k++; 
			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<VEC.get_loc(1,0);
			k++;
			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<VEC.get_loc(2,0);
			k++;
		}
		else
		{
			//real variables
			if(merge&&(index==0))
				//for merging files, time at last entry must be '-1'
			{
				fplot.width(16);
				fplot<<"-1.0";
				k++;
			}
			else
			{
				if(k>4){k=0;fplot<<'\n';}
				fplot.width(16);
				fplot<<flat6[index].real(); 
				k++;
			}
		}
	}
	//writing to 'ploti.asc' the variables from the 'Missile' class
	for(i=0;i<missile_plot_count;i++)
	{
		index=missile_plot_ind[i];
		//checking for integers
		integer=missile[index].get_type();
		vector=missile[index].get_name();
		if(!strcmp(integer,"int"))
		{
			//casting integer to real variable
			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<(double) missile[index].integer();			
			k++;

		}
		//checking vor vectors
		else if(isupper(vector[0]))
		{
			VEC=missile[index].vec();
			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<VEC.get_loc(0,0);
			k++;
			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<VEC.get_loc(1,0);
			k++;
			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<VEC.get_loc(2,0);
			k++;
		}
		else
		{
			//real variables
			if(k>4){k=0;fplot<<'\n';}
			fplot.width(16);
			fplot<<missile[index].real();
			k++;
		}
	}
	fplot<<"\n";
}
///////////////////////////////////////////////////////////////////////////////
//Watching for and executing events
// 
//Max number of events set by global constant NEVENT
//Max number of variables in any event set by global constant NVAR
//Each event has to be surrounded in 'input.asc' by 'IF' and 'ENDIF'
//Event criteria and variables can be 'double' or 'int'
//New variable-values are subsituted before calling modules
//
//010125 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::event(char *options)
{
	Variable *watch_variable_ptr;
	double current_value=0;
	int current_value_int=0;
	double crit_value=0;
	int crit_value_int=0;
	char oper;
	char *intest=NULL;
	int i(0);

	//initializing event flag ('Missile' member)
	event_epoch=false;

	//returning if no events occur for this vehicle object
	//or if all events have already occurred
	if(!event_total)return;
	
	//getting watch variable's current value and critical value
	watch_variable_ptr=event_ptr_list[nevent]->get_variable();
	crit_value=event_ptr_list[nevent]->get_value();

	//testing for integer
	intest=watch_variable_ptr->get_type();
	if(!strcmp(intest,"int"))
	{
		current_value_int=watch_variable_ptr->integer();
		crit_value_int=(int)crit_value;
	}
	else
		current_value=watch_variable_ptr->real();
	//getting relational operator
	oper=event_ptr_list[nevent]->get_operator();

	//checking if event occurred
	if(oper=='<')
	{
		if(!strcmp(intest,"int"))
		{
			if(current_value_int<crit_value_int)
				event_epoch=true;
		}
		else
		{
			if(current_value<crit_value)
				event_epoch=true;
		}
	}
	else if(oper=='=')
	{
		if(!strcmp(intest,"int"))
		{
			if(current_value_int==crit_value_int)
				event_epoch=true;
		}
		else
		{
			if(current_value==crit_value)event_epoch=true;
		}
	}
	else if(oper=='>')
	{
		if(!strcmp(intest,"int"))
		{
			if(current_value_int>crit_value_int)
				event_epoch=true;
		}
		else
		{
			if(current_value>crit_value)
				event_epoch=true;
		}
	}

	//loading new variables
	if(event_epoch)
	{
		int *flat6_index_list;
		double *flat6_value_list;
		int flat6_size;
		int *missile_index_list;
		double *missile_value_list;
		int missile_size;
		int index;
		char *integer;
		double value;
		int value_int;

		flat6_index_list=event_ptr_list[nevent]->get_flat6_indices();
		flat6_value_list=event_ptr_list[nevent]->get_flat6_values();
		flat6_size=event_ptr_list[nevent]->get_flat6_size();

		missile_index_list=event_ptr_list[nevent]->get_missile_indices();
		missile_value_list=event_ptr_list[nevent]->get_missile_values();
		missile_size=event_ptr_list[nevent]->get_missile_size();

		for(i=0;i<flat6_size;i++)
		{
			index=flat6_index_list[i];
			value=flat6_value_list[i];

			integer=flat6[index].get_type();
			if(!strcmp(integer,"int"))
			{
				value_int=(int)value;
				flat6[index].gets(value_int);
			}
				flat6[index].gets(value); // new value assigned
		}
		for(i=0;i<missile_size;i++)
		{
			index=missile_index_list[i];
			value=missile_value_list[i];

			integer=missile[index].get_type();
			if(!strcmp(integer,"int"))
			{
				value_int=(int)value;
				missile[index].gets(value_int);
			}
				missile[index].gets(value); // new value assigned
		}
				
		//writing event message to console
		double time=flat6[0].real();
		char *name=watch_variable_ptr->get_name();
		if(strstr(options,"y_events"))
		{
			cout<<" *** Event #"<<nevent+1<<'\t'<<missile6_name<<'\t'<<"time = "<<time
				<<"\tsec;  criteria:  "<<name<<" "<<oper<<" "<<crit_value<<"\t***\n";
		}
		//increment event number
		nevent++;
		//reset 'event_total' to zero after last event has occured
		if(nevent==event_total)event_total=0;	
	}
}
///////////////////////////////////////////////////////////////////////////////
//Composing documention on file 'doc.asc'
//Listing 'missile' and 'flat6' module-variable arrays
//
//010126 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
//020911 Added module-variable error flagging, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::document(ostream &fdoc,char *title,Document *doc_missile6)
{
	fdoc<<"*********************************************************************************************************************\n";
	fdoc<<"********************************************** MISSILE6 **************************************************************\n";
	fdoc<<"*********************************************************************************************************************\n";
	fdoc<<"\n"<<"*** "<<title<<"   "<< __DATE__ <<" "<< __TIME__ <<" ***\n\n";
//	fdoc<<"01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456\n";
	fdoc<<"\n\n                                       Missile Module-Variable Array \n\n";
	fdoc<<"---------------------------------------------------------------------------------------------------------------------\n";
	fdoc<<"|LOC|        NAME       |                    DEFINITION                       |   MODULE   | PURPOSE |    OUTPUT    |\n";
	fdoc<<"---------------------------------------------------------------------------------------------------------------------\n";

	char name_error_code[]="A";
	int i(0),j(0);
	for(i=0;i<NMISSILE;i++)
	{
		for(j=0;j<i;j++){
			if(!strcmp(missile[i].get_name(),missile[j].get_name())&&strcmp(missile[i].get_name(),"empty"))
				missile[i].put_error(name_error_code);
		}				
		if(!strcmp(missile[i].get_error(),"A")) cout<<" *** Error code 'A': duplicate name in missile[] array, see 'doc.asc' ***\n"; 
		if(!strcmp(missile[i].get_error(),"*")) cout<<" *** Error code '*': duplicate location in missile[] array, see 'doc.asc' ***\n"; 

				fdoc<<missile[i].get_error();
		fdoc.setf(ios::left);
		fdoc.width(4);fdoc<<i;
		if(!strcmp(missile[i].get_type(),"int"))
		{
			fdoc.width(15);fdoc<<missile[i].get_name();
			fdoc.width(5);fdoc<<" int ";
		}
		else
		{
			fdoc.width(20);fdoc<<missile[i].get_name();
		}
		fdoc.width(54);fdoc<<missile[i].get_def();
		fdoc.width(13);fdoc<<missile[i].get_mod();
		fdoc.width(10);fdoc<<missile[i].get_role();
		fdoc<<missile[i].get_out();
		fdoc<<"\n";
		if(!((i+1)%10))fdoc<<"----------------------------------------------------------------------------------------------------------------------\n";			
	}

	fdoc<<"\n\n                                       Flat6 Module-Variable Array \n\n";
	fdoc<<"---------------------------------------------------------------------------------------------------------------------\n";
	fdoc<<"|LOC|        NAME       |                   DEFINITION                        |   MODULE   | PURPOSE |    OUTPUT    |\n";
	fdoc<<"---------------------------------------------------------------------------------------------------------------------\n";
	  
	for(i=0;i<NFLAT6;i++)
	{
		for(j=0;j<i;j++){
			if(!strcmp(flat6[i].get_name(),flat6[j].get_name())&&strcmp(flat6[i].get_name(),"empty"))
				flat6[i].put_error(name_error_code);
		}				
		if(!strcmp(flat6[i].get_error(),"A")) cout<<" *** Error code 'A': duplicate name in flat6[] array, see 'doc.asc' ***\n"; 
		if(!strcmp(flat6[i].get_error(),"*")) cout<<" *** Error code '*': duplicate location in flat6[] array, see 'doc.asc' ***\n"; 
		
		fdoc<<flat6[i].get_error();
		fdoc.setf(ios::left);
		fdoc.width(4);fdoc<<i;
		if(!strcmp(flat6[i].get_type(),"int"))
		{
			fdoc.width(15);fdoc<<flat6[i].get_name();
			fdoc.width(5);fdoc<<" int ";
		}
		else
		{
			fdoc.width(20);fdoc<<flat6[i].get_name();
		}
		fdoc.width(54);fdoc<<flat6[i].get_def();
		fdoc.width(13);fdoc<<flat6[i].get_mod();
		fdoc.width(10);fdoc<<flat6[i].get_role();
		fdoc<<flat6[i].get_out();
		fdoc<<"\n";
		if(!((i+1)%10))fdoc<<"----------------------------------------------------------------------------------------------------------------------\n";			
	}

	//building doc_missile6[] for documenting 'input.asc' and eliminating 'empty' slots
	int counter=0;
	for(i=0;i<NMISSILE;i++){
		if(strcmp(missile[i].get_name(),"empty")){
			doc_missile6[counter].put_doc_offset(counter);
			doc_missile6[counter].put_name(missile[i].get_name());
			doc_missile6[counter].put_type(missile[i].get_type());
			doc_missile6[counter].put_def(missile[i].get_def());
			doc_missile6[counter].put_mod(missile[i].get_mod());
			counter++;
		}
	}
	for(i=0;i<NFLAT6;i++){
		if(strcmp(flat6[i].get_name(),"empty")){
			doc_missile6[counter].put_doc_offset(counter);
			doc_missile6[counter].put_name(flat6[i].get_name());
			doc_missile6[counter].put_type(flat6[i].get_type());
			doc_missile6[counter].put_def(flat6[i].get_def());
			doc_missile6[counter].put_mod(flat6[i].get_mod());
			counter++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//Building index array of those 'flat6[]' and missile[] variables 
//that are output to 'combus' 'data'  
//
//Output: Missile::flat6_com_ind[], missile_com_ind[] 
//
//010210 Created by Peter Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

void Missile::com_index_arrays()
{
	const char *test="com";
	int i(0);

	int k=0;
	for(i=0;i<NFLAT6;i++)
	{
		if(strstr(flat6[i].get_out(),test))
		{
			flat6_com_ind[k]=i;
			k++;
		}
	}
	int l=0;
	for(i=0;i<NMISSILE;i++)
	{
		if(strstr(missile[i].get_out(),test))
		{
			missile_com_ind[l]=i;
			l++;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//Initializing loading 'packet' with 'MISSILE6' data
//
//uses C-code 'sprintf' function to convert 'int' to 'char'
//differs from 'loading_packet' only by initializing 'status=1'
//
//Output by 'return packet'
//
//010401 Created by Peter H Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

Packet Missile::loading_packet_init(int num_missile,int num_target)
{
	string id;
	char object[4];
	static int c_count=0;
	int index;
	int i(0),j(0);
	
	c_count++;
	if(c_count==(num_missile+1))c_count=1;
	sprintf(object,"%i",c_count);
	id="m"+string(object);

	//building 'data' array of module-variables
	for(i=0;i<flat6_com_count;i++)
	{
		index=flat6_com_ind[i];
		com_missile6[i]=flat6[index]; 
	}
	for(j=0;j<missile_com_count;j++)
	{
		index=missile_com_ind[j];
		com_missile6[i+j]=missile[index];
	}
	//refreshing the packet
	packet.set_id(id);
	packet.set_status(1);
	packet.set_data(com_missile6);
	packet.set_ndata(ncom_missile6);

	return packet;
}
///////////////////////////////////////////////////////////////////////////////
//Loading 'packet' with 'MISSILE6' data
//
//uses C-code 'sprintf' function to convert 'int' to 'char'
//
//010206 Created by Peter H Zipfel
//011129 Adapted to MISSILE6 simulation, PZi
///////////////////////////////////////////////////////////////////////////////

Packet Missile::loading_packet(int num_missile,int num_target)
{
	int index;
	int i(0),j(0);

/*	string id;
	char object[4];
	static int c_count=0;
	
	c_count++;
	if(c_count==(num_missile+1))c_count=1;
	sprintf(object,"%i",c_count);
	id="m"+string(object);
*/
	//building 'data' array of module-variables
	for(i=0;i<flat6_com_count;i++)
	{
		index=flat6_com_ind[i];
		com_missile6[i]=flat6[index];
	}
	for(j=0;j<missile_com_count;j++)
	{
		index=missile_com_ind[j];
		com_missile6[i+j]=missile[index];
	}
	//refreshing the packet
//	packet.set_id(id);
	packet.set_data(com_missile6);
	packet.set_ndata(ncom_missile6);

	return packet;
}
///////////////////////////////////////////////////////////////////////////////
//Reading tables from table decks
//
//Supports 1, 2, 3 dim tables stored seperately in data decks
// Keying on AERO_DECK and PROP_DECK in 'input.asc' this function  reads the tables
// from the data files and stores them in 'Plane::Datadeck aerotable' and 
// 'Plane::Datadeck proptable'. In the modules, table look up is carried out by 
// double value=aerodeck.look_up(string name, double var1);  
// double value=aerodeck.look_up(string name, double var1,double var2);  
// double value=aerodeck.look_up(string name, double var1,double var2,double var3);  
//
// To add new tables, just include them in the files of  AERO_DECK and PROP_DECK
// For debugging puposes un-comment the print out provision of the tables below
//
//030721 Created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////