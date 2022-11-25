import re


missile_data = """missile[1].init("tgt_num","int",0,"Target tail # attacked by 'this' missile","combus","data","");
missile[2].init("STEL",0,0,0,"Position of target, downloaded from 'combus' - m","combus","out","");
missile[3].init("VTEL",0,0,0,"Velocity of target, downloaded from 'combus' - m","combus","out","");
missile[4].init("TTL",1,0,0,0,1,0,0,0,1,"TM of target, downloaded from 'combus' - m","combus","","");
missile[5].init("tgt_com_slot","int",0,"This target slot in combus - ND","combus","out","");
missile[6].init("sht_num","int",0,"Shooter tail # launching 'this' missile","combus","data","");
missile[7].init("SSEL",0,0,0,"Position of shooter, downloaded from 'combus' - m","combus","","");
missile[8].init("dts",0,"Target-shooter distance - m","combus","out","scrn,plot");
missile[9].init("STSL",0,0,0,"Velocity of shooter, downloaded from 'combus' - m","combus","out","");
missile[50].init("mprop","int",0,"=0: Motor off, =1:Motor on","propulsion","data","");
missile[51].init("aexit",0,"Nozzle exit area - m^2","propulsion","data","");
missile[59].init("vmass",92,"Vehicle mass - kg","propulsion","out","plot");
missile[63].init("thrust",0,"Rocket thrust parallel to vehicle centerline - N","propulsion","out","plot");
missile[64].init("xcgref",1.536,"Launch CG aft of vehicle nose - m","propulsion","init","");
missile[70].init("xcg",1.536,"Vehicle CG aft of vehicle nose - m","propulsion","diag","");
missile[71].init("ai11",0.308,"Roll moment of inertia - kg*m^2","propulsion","out","plot");
missile[72].init("ai33",59.80,"Pitch/Yaw moment of inertia - kg*m^2","propulsion","out","plot");
missile[73].init("mfreeze_prop","int",0,"Saving mfreeze variable","propulsion","save","");
missile[74].init("thrustf",0,"Saved thrust when mfreez=1 - N","propulsion","save","");
missile[75].init("vmassf",0,"Saved mass when mfreez=1 - kg","propulsion","save","");
missile[76].init("xcgf",0,"Saved cg when mfreez=1 - m","propulsion","save","");
missile[77].init("ai11f",0,"Saved ai11 when mfreez=1 - kgm^2","propulsion","save","");
missile[78].init("ai33f",0,"Saved ai33 when mfreez=1 - kgm^2","propulsion","save","");
missile[103].init("refl",0,"Reference length for moment derivatives - m","aerodynamics","init","");
missile[104].init("refa",0,"Reference area for aero coefficients - m^2","aerodynamics","init","");
missile[120].init("ca",0,"Axial force coefficient","aerodynamics","out","");
missile[121].init("cy",0,"Side force coefficient","aerodynamics","out","");
missile[122].init("cn",0,"Normal force coefficient","aerodynamics","out","");
missile[123].init("cll",0,"Rolling moment coefficient","aerodynamics","out","");
missile[124].init("clm",0,"Pitching moment coefficient","aerodynamics","out","");
missile[125].init("cln",0,"Yawing moment coefficient","aerodynamics","out","");
missile[130].init("cn0",0,"Normal force coeff as function of Mach and alpha","aerodynamics","diag","");
missile[131].init("cnp",0,"Normal force coeff of roll attitude","aerodynamics","diag","");
missile[132].init("clm0",0,"Pitching moment coeff as a function of Mach and alpha","aerodynamics","diag","");
missile[133].init("clmp",0,"Pitching moment coeff of roll attitude","aerodynamics","diag","");
missile[134].init("cyp",0,"Side force coeff of roll attitude","aerodynamics","diag","");
missile[135].init("clnp",0,"Yawing moment coeff of roll attitude","aerodynamics","diag","");
missile[136].init("ca0",0,"Axial force coeff at zero incidence","aerodynamics","diag","");
missile[137].init("caa",0,"Axial force coeff alpha derivative - 1/deg","aerodynamics","diag","");
missile[138].init("cad",0,"Axial force coeff of control surface drag - 1/deg^2","aerodynamics","diag","");
missile[139].init("cndq",0,"Normal force coeff of pitch control deflection - 1/deg","aerodynamics","diag","");
missile[140].init("clmdq",0,"Pitching moment coeff of pitch control deflec - 1/deg","aerodynamics","diag","");
missile[141].init("clmq",0,"Pitching moment coeff pitch damping - 1/deg","aerodynamics","diag","");
missile[142].init("cllap",0,"Rolling moment coeff of incidence angle - 1/deg^2","aerodynamics","diag","");
missile[143].init("clldp",0,"Rolling moment coeff of roll control deflec - 1/deg","aerodynamics","diag","");
missile[144].init("cllp",0,"Rolling moment coeff of roll damping - 1/deg","aerodynamics","diag","");
missile[145].init("dna",0,"Normal force slope derivative - m/s^2","aerodynamics","out","");
missile[146].init("dnd",0,"Pitch control force derivative - m/s^2","aerodynamics","out","");
missile[147].init("dma",0,"Pitch moment derivative - 1/s^2","aerodynamics","out","");
missile[148].init("dmq",0,"Pitch damping derivative - 1/s","aerodynamics","out","");
missile[149].init("dmd",0,"Pitch control derivative - 1/s^2","aerodynamics","out","");
missile[150].init("dlp",0,"Roll damping derivative - 1/s","aerodynamics","out","");
missile[151].init("dld",0,"Roll control derivative - 1/s^2","aerodynamics","out","");
missile[152].init("cna",0,"Normal force coeff in aeroballistic axes","aerodynamics","diag","");
missile[153].init("cya",0,"Side force coeff in aeroballistic axes","aerodynamics","diag","");
missile[154].init("clma",0,"Pitching moment coeff in aeroballistic axes","aerodynamics","diag","");
missile[155].init("clna",0,"Yawing moment coeff in aeroballistic axes","aerodynamics","diag","");
missile[159].init("stmarg",0,"Static margin (+stable, -unstable) - diameter","aerodynamics","diag","plot");
missile[160].init("realq1",0,"First real root of airframe pitch dynamics - rad/s","aerodynamics","diag","plot");
missile[161].init("realq2",0,"Second real root of airframe pitch dynamics - rad/s","aerodynamics","diag","plot");
missile[162].init("wnq",0,"Natural frequency of airframe pitch dynamics - rad/s","aerodynamics","diag","plot");
missile[163].init("zetq",0,"Damping of airframe pitch dynamics - NA","aerodynamics","diag","plot");
missile[164].init("realp",0,"Real root of airframe roll dynamics - rad/s","aerodynamics","diag","");
missile[165].init("alplimx",0,"Maximum total alpha permissible - deg","aerodynamics","data","");
missile[166].init("gavail",0,"Maneuver headroom - g's","aerodynamics","diag","plot");
missile[167].init("gmax",0,"Max maneuverability limited by ALPLIMX - g's","aerodynamics","diag","plot");
missile[168].init("pqreal",0,"Real part or mean value (real roots) of airf dyn - rad","aerodynamics","diag","");
missile[180].init("trcode",0,"Termination code number","aerodynamics","init","");
missile[181].init("tmcode",0,"Dummy variable initialized to zero","aerodynamics","data","");
missile[182].init("trcvel",0,"Minimum closing speed - m/s","aerodynamics","data","");
missile[183].init("trmach",0,"Minimum Mach number","aerodynamics","data","");
missile[184].init("trdynm",0,"Minimum dynamic pressure - Pa","aerodynamics","data","");
missile[185].init("trload",0,"Minimum load capacity - g's","aerodynamics","data","");
missile[186].init("tralp",0,"Maximum total angle of attack - rad","aerodynamics","data","");
missile[187].init("trtht",0,"Maximum pitch gimbal angle - rad","aerodynamics","data","");
missile[189].init("trthtd",0,"Maximum pitch gimbal rate - rad/s","aerodynamics","data","");
missile[190].init("trphid",0,"Maximum roll gimbal rate - rad/s","aerodynamics","data","");
missile[191].init("trate",0,"Maximum tracking rate - rad/s","aerodynamics","data","");
missile[200].init("mseek","int",0," =2:Enable, =3:Acquisition, =4:Lock","seeker","data/diag","com");
missile[201].init("ms1dyn","int",0,"=0: Kinemtic, =1:Dynamic","seeker","data","");
missile[202].init("isets1","int",0,"Seeker flag","seeker","init","");
missile[203].init("epchac",0,"Epoch of start of seeker acquisition - s","seeker","save","");
missile[204].init("ibreak","int",0,"Flag for seeker break-lock ND","seeker","init","");
missile[206].init("dblind",0,"Blind range - m","seeker","data","");
missile[207].init("racq",0,"Acquisition range - m","seeker","data","");
missile[208].init("dtimac",0,"Time duration to acquire target - s","seeker","data","");
missile[209].init("dbt",0,"true distance to target - m","seeker","diag","scrn,plot");
missile[211].init("gk",0,"K.F. gain - 1/s","seeker","data","");
missile[212].init("zetak",0,"K.F. damping","seeker","data","");
missile[213].init("wnk",0,"K.F. natural frequency - rad/s","seeker","data","");
missile[215].init("biast",0,"Pitch gimbal bias errors - rad","seeker","data","");
missile[216].init("randt",0,"Pitch gimbal random errors - rad","seeker","data","");
missile[217].init("biasp",0,"Roll gimbal bias error - rad","seeker","data","");
missile[218].init("randp",0,"Roll gimbal bias error - rad","seeker","data","");
missile[219].init("wlq1d",0,"Pitch sight line spin derivative - rad/s^2","seeker","state","");
missile[220].init("wlq1",0,"Pitch sight line spin - rad/s","seeker","state","");
missile[221].init("wlqd",0,"Pitch pointing rate derivative - rad/s^2","seeker","state","");
missile[222].init("wlq",0,"Pitch pointing rate - rad/s","seeker","state","");
missile[223].init("wlr1d",0,"Yaw sight line spin derivative - rad/s^2","seeker","state","");
missile[224].init("wlr1",0,"Yaw sight line spin - rad/s","seeker","state","");
missile[225].init("wlrd",0,"Yaw pointing rate derivative - rad/s^2","seeker","state","");
missile[226].init("wlr",0,"Yaw pointing rate - rad/s","seeker","state","");
missile[227].init("wlq2d",0,"Second state variable deriv in K.F. - rad/s^3","seeker","state","");
missile[228].init("wlq2",0,"Second state variable in K.F. - rad/s^2","seeker","state","");
missile[229].init("wlr2d",0,"Second state variable der in K.F. - rad/s^3","seeker","state","");
missile[230].init("wlr2",0,"Second state variable in K.F. - rad/s^2","seeker","state","");
missile[231].init("fovyaw",0,"Half yaw field-of-view at acquisition - rad","seeker","data","");
missile[232].init("fovpitch",0,"Half positive pitch field-of-view at acquis. - rad","seeker","data","");
missile[234].init("dba",0,"Distance between active seeker and its aimpoint - m","seeker","diag","");
missile[235].init("daim",0,"Dist from targ to initiate aimpoint mode - m","seeker","data","");
missile[236].init("BIASAI",1,0.5,0.2,"Bias error of aimpoint mode in target coor - m","seeker","data","");
missile[239].init("BIASSC",0,0,0,"Bias error of hot spot mode in target coor - m","seeker","data","");
missile[242].init("RANDSC",0,0,0,"Random error of hot spot mode in targ coor - m","seeker","data","");
missile[245].init("epy",0,"Error of pointing in pitch - rad","seeker","diag","");
missile[246].init("epz",0,"Error of pointing in yaw - rad","seeker","diag","");
missile[247].init("thtpb",0,"Pitch pointing angle - rad","seeker","out",""); //////////  俯仰指向角
missile[248].init("psipb",0,"Yaw pointing angle - rad","seeker","out","");   //////////  偏航指向角
missile[249].init("ththb",0,"Head pitch angle - rad","seeker","diag","");
missile[250].init("phihb",0,"Head roll angle - rad","seeker","diag","");
missile[251].init("ththbx",0,"Head pitch angle - deg","seeker","diag","scrn,plot");
missile[252].init("phihbx",0,"Head roll angle - deg","seeker","diag","scrn,plot");
missile[253].init("TPB",0,0,0,0,0,0,0,0,0,"I/G TM of pointing axes wrt body axes","seeker","init","");
missile[262].init("THB",0,0,0,0,0,0,0,0,0,"I/G TM of head axes wrt body axes","seeker","init","");
missile[271].init("timeac",0,"Time duration of seeker acquisition phase - s","seeker","diag","");
missile[272].init("psiot1",0,"Azimuth of sensor LOS in target axes - rad","seeker","diag","");
missile[273].init("thtot1",0,"Elevation of sensor LOS in target axes - rad","seeker","diag","");
missile[274].init("dvbtc",0,"Closing velocity, computed - m/s","seeker","diag","");
missile[276].init("EAHH",0,0,0,"Aimpoint displacement wrt center of F.P. - rad","seeker","diag","plot");
missile[279].init("EPHH",0,0,0,"Computer pointing error of sensor wrt center of F.P.","seeker","diag","");
missile[282].init("EAPH",0,0,0,"Aimpoint to computer pointing displacement - rad","seeker","diag","");
missile[285].init("thtpbx",0,"Pitch pointing angle - deg","seeker","diag","scrn,plot"); //俯仰指向角
missile[286].init("psipbx",0,"Yaw pointing angle - deg","seeker","diag","scrn,plot");   //偏航指向角
missile[287].init("sigdpy",0,"Pitch sight line spin - rad/s","seeker","out","plot");  /////  俯仰视线旋转
missile[288].init("sigdpz",0,"Yaw sight line spin - rad/s","seeker","out","plot");    ////   偏航视线旋转
missile[289].init("biaseh",0,"Image blur and pixel bias errors - rad","seeker","data","");
missile[290].init("randeh",0,"Image blur and pixel random errors - rad","seeker","data","");
missile[291].init("psihlx",0,"Yaw angle of CL seeker wrt local axes - deg","seeker","diag","");
missile[292].init("ththlx",0,"Pitch angle of CL seeker wrt local axes - deg","seeker","diag","");
missile[293].init("phihlx",0,"Roll angle of CL seeker wrt local axes - deg","seeker","diag","");
missile[294].init("SBTL",0,0,0,"Position of missile wrt target - m","seeker","diag","");
missile[400].init("mguid","int",0,"=0:None, =3:Pro-Nav, =6:Comp Pro-Nav","guidance","data","");
missile[401].init("gnav",0,"Navigation gain - ND","guidance","data","");
missile[402].init("ancomx",0,"Normal acceleration command - g's","guidance","out","scrn,plot");
missile[403].init("alcomx",0,"Lateral acceleration command - g's","guidance","out","scrn,plot");
missile[404].init("gn",0,"Guidance Gain - m/s","guidance","diag","");
missile[405].init("mnav","int",0,"=0: Reset, =3:Update","guidance","data","");
missile[406].init("apny",0,"Pronav acceleration along P2 axis - m/s^2","guidance","diag","plot");
missile[407].init("apnz",0,"Pronav acceleration along P3 axis - m/s^2","guidance","diag","");
missile[408].init("adely",0,"Vehicle longit accel correction term along P2 - m/s^2","guidance","diag","plot");
missile[409].init("adelz",0,"Vehicle longit accel correction term along P3 - m/s^2","guidance","diag","");
missile[410].init("all",0,"Vehicle lateral accel before limiting - m/s^2","guidance","diag","plot");
missile[411].init("ann",0,"Vehicle normal accel before limiting - m/s^2","guidance","diag","");
missile[412].init("epchta",0,"Epoch of target data receipt - sec","guidance","save","");    ///////////////
missile[440].init("WOELC",0,0,0,"O LOS rate computed from kinematic data - rad/s","guidance","out","");
missile[443].init("tgoc",0,"Time-to-go, computed - s","guidance","diag","plot");
missile[444].init("dtbc",0,"Distance-to-target, computed - m","guidance","diag","plot");/////////////////
missile[445].init("dvtbc",0,"Closing speed, computed - m/s","guidance","diag","plot");
missile[446].init("psiobcx",0,"Yaw LOS angle wrt missile - deg","guidance","diag","plot");
missile[447].init("thtobcx",0,"Pitch LOS angle wrt missile - deg","guidance","diag","plot");
missile[448].init("UTBLC",0,0,0,"LOS unit vector from extrapolated data - NA","guidance","out","");
missile[455].init("STELC",0,0,0,"Target location, extrapol onboard missile - m","guidance","diag","");////////////
missile[458].init("STBLC",0,0,0,"Target wrt Missile position, extrapolated - m","guidance","diag","");///////////
missile[459].init("STELM",0,0,0,"Stored target position - m","guidance","save","");              //////////
missile[460].init("VTELC",0,0,0,"Stored target velocity - m/s","guidance","save","");           //////////
missile[500].init("maut","int",0," =2:Rate; =3:Accel controller","control","data","");
missile[501].init("mfreeze","int",0,"=0:Unfreeze; =1:Freeze; increment for more","control","data","");
missile[504].init("wacl",0,"Nat freq of accel close loop complex pole - rad/s","control","data","");
missile[505].init("zacl",0,"Damping of accel close loop complex pole - ND","control","data","");
missile[506].init("pacl",0,"Close loop real pole - ND","control","data","");
missile[507].init("alimit",0,"Total structural acceleration limiter - g's","control","data","");
missile[508].init("dqlimx",0,"Pitch flap control limiter - deg","control","data","");
missile[509].init("drlimx",0,"Yaw flap control limiter - deg","control","data","");
missile[510].init("dplimx",0,"Roll command limiter - deg","control","data","");
missile[511].init("phicomx",0,"Commanded roll angle - deg","control","data","plot");
missile[512].init("wrcl",0,"Freq of roll closed loop complex pole - rad/s","control","data","");
missile[513].init("zrcl",0,"Damping of roll closed loop pole - ND","control","data","");
missile[514].init("yyd",0,"Yaw feed-forward derivative variable- m/s","control","state","");
missile[515].init("yy",0,"Yaw feed-forward integration variable- m/s","control","state","");
missile[516].init("zzd",0,"Pitch feed-forward derivative variable- m/s","control","state","");
missile[517].init("zz",0,"Pitch feed-forward integration variable- m/s","control","state","");
missile[519].init("dpcx",0,"Roll flap command deflection - deg","control","out","plot");
missile[520].init("dqcx",0,"Pitch flap command deflection - deg","control","out","plot");
missile[521].init("drcx",0,"Yaw flap command deflection - deg","control","out","plot");
missile[524].init("GAINFB",0,0,0,"Feedback gain of rate, accel and control","control","diag","");
missile[525].init("gainp",0,"Feed-forward gain - s^2/m","control","data","");
missile[531].init("gkp",0,"Gain of roll rate feedback - s","control","diag","");
missile[532].init("gkphi",0,"Gain of roll angle feedback -","control","diag","");
missile[533].init("fspb2m",0,"Max pitch accel transient - m/s^2","control","diag","");
missile[534].init("fspb2mt",0,"Stagetime at max pitch accel transient - s","control","diag","");
missile[535].init("fspb3m",0,"Max yaw accel transient - m/s^2","control","diag","");
missile[536].init("fspb3mt",0,"Stagetime at max yaw accel transient - s","control","diag","");
missile[537].init("qqxm",0,"Max pitch rate transient - deg/s","control","diag","");
missile[538].init("qqxmt",0,"Stagetime at max pitch rate transient - s","control","diag","");
missile[539].init("rrxm",0,"Max yaw rate transient - deg/s","control","diag","");
missile[540].init("rrxmt",0,"Stagetime at max yaw rate transient - s","control","diag","");
missile[541].init("dqcxm",0,"Max pitch flap transient - deg","control","diag","");
missile[542].init("dqcxmt",0,"Stagetime at max pitch flap transient - s","control","diag","");
missile[543].init("drcxm",0,"Max yaw flap transient - deg","control","diag","");
missile[544].init("drcxmt",0,"Stagetime at max yaw flap transient - s","control","diag","");
missile[545].init("isetc2",0,"Flag to print freeze variables","control","init","");
missile[546].init("factwacl",0,"Fact assoc with closed loop natural frequency","control","data","");
missile[547].init("factzacl",0,"Fact assoc with closed loop damping","control","data","");
missile[550].init("zetlagr",0,"Desired damping of closed rate loop ND","control","data","");
missile[553].init("ratelimx",0,"Rate command limiter - deg/s","control","data","");
missile[560].init("zrate",0,"Zero pole of rate TR.FCT. - 1/rad","control","diag","");
missile[561].init("grate",0,"Feedback gain of rate loop - ND","control","diag","");
missile[562].init("wnlagr",0,"Nat freq of closed rate loop - rad/s","control","diag","");
missile[600].init("mact","int",0,"=0:no dynamics, =2:second order","actuator","data","");
missile[602].init("dlimx",0,"Control fin limiter - deg","actuator","data","");
missile[604].init("ddlimx",0,"Control fin rate limiter - deg/s","actuator","data","");
missile[605].init("wnact",0,"Natural frequency of actuator - rad/s","actuator","data","");
missile[606].init("zetact",0,"Damping of actuator - ND","actuator","data","");
missile[619].init("dpx",0,"Roll control deflection - deg","actuator","out","plot");
missile[620].init("dqx",0,"Pitch control deflection - deg","actuator","out","plot");
missile[621].init("drx",0,"Yaw control deflection - deg","actuator","out","plot");
missile[622].init("delx1",0,"Fin #1 output position - deg","actuator","diag","plot");
missile[623].init("delx2",0,"Fin #2 output position - deg","actuator","diag","");
missile[624].init("delx3",0,"Fin #3 output position - deg","actuator","diag","");
missile[625].init("delx4",0,"Fin #4 output position - deg","actuator","diag","");
missile[630].init("dxd1",0,"Fin #1 position derivative - deg/s","actuator","state","");
missile[631].init("dxd2",0,"Fin #2 position derivative - deg/s","actuator","state","");
missile[632].init("dxd3",0,"Fin #3 position derivative - deg/s","actuator","state","");
missile[633].init("dxd4",0,"Fin #4 position derivative - deg/s","actuator","state","");
missile[634].init("dx1",0,"Fin #1 position - deg","actuator","state","");
missile[635].init("dx2",0,"Fin #2 position - deg","actuator","state","");
missile[636].init("dx3",0,"Fin #3 position - deg","actuator","state","");
missile[637].init("dx4",0,"Fin #4 position - deg","actuator","state","");
missile[638].init("ddxd1",0,"Fin #1 rate derivative - deg/s^2","actuator","state","");
missile[639].init("ddxd2",0,"Fin #2 rate derivative - deg/s^2","actuator","state","");
missile[640].init("ddxd3",0,"Fin #3 rate derivative - deg/s^2","actuator","state","");
missile[641].init("ddxd4",0,"Fin #4 rate derivative - deg/s^2","actuator","state","");
missile[642].init("ddx1",0,"Fin #1 rate - deg/s","actuator","state","plot");
missile[643].init("ddx2",0,"Fin #2 rate - deg/s","actuator","state","");
missile[644].init("ddx3",0,"Fin #3 rate - deg/s","actuator","state","");
missile[645].init("ddx4",0,"Fin #4 rate - deg/s","actuator","state","");
missile[646].init("delcx1",0,"Fin #1 position command - deg","actuator","diag","");
missile[647].init("delcx2",0,"Fin #2 position command - deg","actuator","diag","");
missile[648].init("delcx3",0,"Fin #3 position command - deg","actuator","diag","");
missile[649].init("delcx4",0,"Fin #4 position command - deg","actuator","diag","");
missile[652].init("miss",0,"Miss distance - m","intercept","diag","plot");
missile[653].init("hit_time",0,"Intercept time - s","intercept","diag","");
missile[654].init("MISS_L",0,0,0,"Miss vector in local level coord. - m","intercept","diag","");
missile[655].init("time_m",0,"Previous time - s","intercept","save","");
missile[656].init("SBTLM",0,0,0,"Previous displacment vector. - m","intercept","save","");
missile[657].init("STMEL",0,0,0,"Previous aircraft taget displacment vector. - m","intercept","save","");
missile[658].init("SBMEL",0,0,0,"Previous missile displacment vector. - m","intercept","save","");
missile[659].init("mode","int",0,"Mode flags |mseek|mguid|maut|mprop|  - ND","intercept","diag","scrn");
missile[700].init("mtvc","int",0,"=0:no TVC;=1:no dyn;=2:scnd order;=3:2+gain","tvc","data","");
missile[702].init("tvclimx",0,"Nozzle deflection limiter - deg","tvc","data","");
missile[704].init("dtvclimx",0,"Nozzle deflection rate limiter - deg/s","tvc","data","");
missile[705].init("wntvc",0,"Natural frequency of TVC - rad/s","tvc","data","");
missile[706].init("zettvc",0,"Damping of TVC - ND","tvc","data","");
missile[707].init("factgtvc",0,"Factor for TVC gain - ND","tvc","data","");
missile[708].init("gtvc",0,"TVC nozzle deflection gain - ND","tvc","data,diag","");
missile[709].init("parm",0,"Propulsion moment arm from vehicle nose - m","tvc","data","");
missile[710].init("FPB",0,0,0,"Thrust force in body axes - N","tvc","out","");
missile[711].init("FMPB",0,0,0,"Thrust moment in body axes - Nm","tvc","out","");
missile[712].init("etax",0,"Nozzle pitch deflection - deg","tvc","diag","plot");
missile[713].init("zetx",0,"Nozzle yaw deflection - deg","tvc","diag","plot");
missile[714].init("etacx",0,"Commanded nozzle pitch deflection - deg","tvc","diag","plot");
missile[715].init("zetcx",0,"Commanded nozzle yaw deflection - deg","tvc","diag","plot");
missile[730].init("etasd",0,"Pitch nozzle derivative - rad/s","tvc","state","");
missile[731].init("zetad",0,"Yaw nozzle derivative - rad/s","tvc","state","");
missile[734].init("etas",0,"Pitch nozzle deflection - rad","tvc","state","");
missile[735].init("zeta",0,"Yaw nozzle deflection - rad","tvc","state","");
missile[738].init("detasd",0,"Pitch nozzle rate derivative - rad/s^2","tvc","state","");
missile[739].init("dzetad",0,"Yaw nozzle rate derivative - rad/s^2","tvc","state","");
missile[742].init("detas",0,"Pitch nozzle rate - rad/s","tvc","state","plot");
missile[743].init("dzeta",0,"Yaw nozzle rate - rad/s","tvc","state","plot");"""

flat_data="""flat6[0].init("time",0,"Simulation time - sec","newton","exec","scrn,plot,com");
flat6[1].init("halt","int",0,"=1: vehicle object is halted - ND","newton","exec","");
flat6[52].init("press",0,"Atmospheric pressure - Pa","environment","out","");
flat6[53].init("rho",0,"Atmospheric density - kg/m^3","environment","out","");
flat6[54].init("vsound",0,"Sonic speed - m/sec","environment","diag","");
flat6[55].init("grav",0,"Gravity acceleration - m/s^2","environment","out","");
flat6[56].init("vmach",0,"Mach number of missile","environment","out","scrn,plot,com");
flat6[57].init("pdynmc",0,"Dynamic pressure - Pa","environment","out","plot");
flat6[58].init("tempk",0,"Atmospheric temperature - K","environment","out","");
flat6[59].init("mfreeze_environ","int",0,"Saving 'mfreeze' value","environment","save","");
flat6[60].init("pdynmcf",0,"Saved dyn.press. when mfreeze=1 - Pa","environment","save","");
flat6[61].init("vmachf",0,"Saved Mach# when mfreeze=1 - Pa","environment","save","");
flat6[101].init("ck",50,"Quaternion orthogonalizing factor ","kinematics","data","");
flat6[110].init("q0d",0,"Quaternion derivative, 0-th component","kinematics","state","");
flat6[111].init("q0",0,"Quaternion, 0-th component","kinematics","state","");
flat6[112].init("q1d",0,"Quaternion derivative, 1-th component","kinematics","state","");
flat6[113].init("q1",0,"Quaternion, 1-th component","kinematics","state","");
flat6[114].init("q2d",0,"Quaternion derivative, 2-th component","kinematics","state","");
flat6[115].init("q2",0,"Quaternion, 2-th component","kinematics","state","");
flat6[116].init("q3d",0,"Quaternion derivative, 3-th component","kinematics","state","");
flat6[117].init("q3",0,"Quaternion, 3-th component","kinematics","state","");
flat6[120].init("TBL",0,0,0,0,0,0,0,0,0,"T.M. of body wrt local level coord","kinematics","out","");
flat6[134].init("psibl",0,"Yawing angle of vehicle - rad","kinematics","diag","");
flat6[135].init("thtbl",0,"Pitching angle of vehicle - rad","kinematics","diag","");
flat6[136].init("phibl",0,"Rolling angle of vehicle - rad","kinematics","diag","");
flat6[137].init("psiblx",0,"G Yawing angle of vehicle - deg","kinematics","in/di","scrn,plot");
flat6[138].init("thtblx",0,"G Pitching angle of vehicle - deg","kinematics","in/di","scrn,plot");
flat6[139].init("phiblx",0,"G Rolling angle of vehicle - deg","kinematics","in/di","scrn,plot");
flat6[140].init("alppx",0,"Total angle of attack - deg","kinematics","out","scrn,plot");
flat6[141].init("phipx",0,"Aerodynamic roll angle - deg","kinematics","out","scrn,plot");
flat6[142].init("alpp",0,"Total angle of attack - rad","kinematics","out","");
flat6[143].init("phip",0,"Aerodynamic roll angle - rad","kinematics","out","");
flat6[144].init("alphax",0,"Angle of attack - deg","kinematics","diag","plot");
flat6[145].init("betax",0,"Side slip angle - deg","kinematics","diag","plot");
flat6[146].init("erq",0,"Error of quaternion nonorthogonality","kinematics","diag","plot");
flat6[147].init("etbl",0,"Error of D.C.M. nonorthogonality","kinematics","diag","plot");
flat6[148].init("TLB",0,0,0,0,0,0,0,0,0,"T.M. of local level wrt body coord","kinematics","diag","");
flat6[154].init("ppd",0,"Body roll angular velocity derivative - rad/s^2","euler","state","");
flat6[155].init("pp",0,"Body roll angular velocity - rad/s","euler","state","");
flat6[156].init("qqd",0,"Body pitch angular velocity derivative - rad/s^2","euler","state","");
flat6[157].init("qq",0,"Body pitch angular velocity - rad/s","euler","state","");
flat6[158].init("rrd",0,"Body yaw angular velocity derivative - rad/s^2","euler","state","");
flat6[159].init("rr",0,"Body yaw angular velocity - rad/s","euler","state","");
flat6[160].init("ppx",0,"Body roll angular velocity in body axes - deg/s","euler","out","plot");
flat6[161].init("qqx",0,"Body pitch angular velocity in body axes - deg/s","euler","out","plot");
flat6[162].init("rrx",0,"Body yaw angular velocity in body axes - deg/s","euler","out","plot");
flat6[163].init("WBEB",0,0,0,"Ang vel of veh wrt earth, body axes - rad/s","euler","diag","");
flat6[200].init("FAPB",0,0,0,"Aerodynamic and propulsive forces in body axes - N","forces","out","");
flat6[201].init("FMB",0,0,0,"Aerodynamic and propulsive moments in body axes - N*m","forces","out","");
flat6[210].init("VBEBD",0,0,0," Derivative of missile velocity - m/s^2","newton","state","");
flat6[213].init("VBEB",0,0,0,"Missile velocity in body axes - m/s","newton","state","");
flat6[216].init("SBELD",0,0,0,"Derivative of missile position - m/s","newton","state","");
flat6[219].init("SBEL",0,0,0,"Missile pos. wrt point E in local level axes - m","newton","state","plot,com");
flat6[220].init("sbel1",0,"Initial north comp of SBEL - m","newton","data","");
flat6[221].init("sbel2",0,"Initial east comp of SBEL - m","newton","data","");
flat6[222].init("sbel3",0,"Initial down comp of SBEL - m","newton","data","");
flat6[223].init("trackd",0,"Ground track derivative - m","newton","state","");
flat6[224].init("track",0,"Ground track - m","newton","state","plot");
flat6[230].init("FSPB",0,0,0,"Specific force in body axes - m/s^2","newton","out","");
flat6[233].init("VBEL",0,0,0,"Missile velocity in local level axes - m/s","newton","out","com,scrn,plot");
flat6[236].init("dvbe",0,"Missile speed - m/s","newton","in/out","plot");
flat6[237].init("alpha0x",0,"Initial angle-of-attack - deg","newton","data","");
flat6[238].init("beta0x",0,"Initial side slip angle - deg","newton","data","");
flat6[239].init("hbe",0,"Height above ground - m","newton","out","scrn,plot");
flat6[240].init("psivlx",0,"Heading angle - deg","newton","diag","scrn,plot");
flat6[241].init("thtvlx",0,"Vertical flight path angle - deg","newton","diag","scrn,plot");
flat6[244].init("anx",0,"Normal specific force component - g's","newton","diag","scrn,plot,com");
flat6[245].init("ayx",0,"Side specific force component - g's","newton","diag","scrn,plot,com");
flat6[246].init("ATB",0,0,0,"Tangential accel, solving Newton's Law - m/s^2","newton","diag","");
flat6[247].init("mfreeze_newt","int",0,"Saving mfreze value - ND","newton","save","");
flat6[248].init("dvbef",0,"Saved speed under mfreeze=1 - m/s","newton","save","");"""

re_p = re.compile(".*\[(.*)\].init\(\"(\w*)\".*")
missile_data_dict = dict()
flat_data_dict = dict()
for m_d in missile_data.split('\n'):
    ret = re.match(re_p,m_d)
    missile_data_dict[ret.group(2)] = [ret.group(1), "\"int\"" in m_d]
for f_d in flat_data.split('\n'):
    ret = re.match(re_p,f_d)
    flat_data_dict[ret.group(2)] = [ret.group(1), "\"int\"" in f_d]

def deck_trans(deck_path,deck_table_name):
    with open(deck_path,'r') as f:
        ans = []
        lines = f.readlines()
        title = lines[0]
        data = iter(lines)
        DIM_line_idx = [i  for i,line in enumerate(lines) if "DIM" in line]
        table_count = len(DIM_line_idx)
        ans.append(f"{deck_table_name}.set_title(\"{title[:-1]}\");")
        ans.append(f"{deck_table_name}.set_capacity({table_count});")
        ans.append(f"{deck_table_name}.alloc_mem();")
        i = 0
        for i in range(DIM_line_idx[0]):
            next(data)
        for j in range(table_count):
            dim_check = next(data)
            table_dim = int(dim_check[0])
            ans.append(f"table=new Table;")
            ans.append(f"table->set_dim({dim_check[0]});")
            ans.append(f"table->set_name(\"{dim_check.split()[1]}\");")
            var_dim = next(data).split('//')[0].split()[1::2]
            var_dim = [int(d) for d in var_dim]
            while len(var_dim)<3:
                var_dim.append(1)
            for k in range(table_dim):
                ans.append(f"table->set_var{k+1}_dim({var_dim[k]});")
            num_rows = max([int(dim) for dim in var_dim])
            table_data = []
            for k in range(num_rows):
                line_data = [float(d) for d in next(data).split()]
                table_data.append(line_data)
            var1_value = [table_data[i][0] for i in range(var_dim[0])]
            var2_value = [table_data[i][min(1,len(table_data[i])-1)] for i in range(var_dim[1])] if var_dim[1]>1 else []
            var3_value = [table_data[i][min(2,len(table_data[i])-1)] for i in range(var_dim[2])] if var_dim[2]>1 else []
            t_data = [table_data[i][table_dim:] for i in range(min([dim for dim in var_dim if dim != 1]))]
            t_data_cat = []
            for t_d in t_data:
                t_data_cat = t_data_cat + t_d
            if j != table_count-1:
                next(data)
            ans.append(f"table->var1_values=new double [{var_dim[0]}]"+"{};".format("{"+str(var1_value)[1:-1]+"}"))
            ans.append(f"table->var2_values=new double [{var_dim[1]}]"+"{};".format("{"+str(var2_value)[1:-1]+"}" if var_dim[1]>1 else ""))
            ans.append(f"table->var3_values=new double [{var_dim[2]}]"+"{};".format("{"+str(var3_value)[1:-1]+"}" if var_dim[2]>1 else ""))
            ans.append(f"table->data=new double [{var_dim[0]*var_dim[1]*var_dim[2]}]"+"{};".format("{"+str(t_data_cat)[1:-1]+"}"))
            ans.append(f"{deck_table_name}.set_counter({j});\n{deck_table_name}.add_table(\"{dim_check.split()[1]}\",table);")

        return ans

def input_trans(input_file_dir):
    with open(input_file_dir+"input.asc","r") as f:
        lines = f.readlines()
        lines_iter = iter(lines)
        ans = []
        deck_tables = []
        t_map = {"def":"definition","init":"initialization","exec":"execution","term":"termination"}
        while next(lines_iter) != "MODULES\n":
            pass
        num_modules = 0
        while True:
            line = next(lines_iter)
            if line == "END\n":
                break
            else:
                name,structure_datas = line.split()
                all_type = list(t_map.keys())
                ans.append(f"module_list[{num_modules}].name=\"{name}\";")
                for s_data in structure_datas.split(','):
                    ans.append(f"module_list[{num_modules}].{t_map[s_data]}=\"{s_data}\";")
                    all_type.remove(s_data)
                for s_data in all_type:
                    ans.append(f"module_list[{num_modules}].{t_map[s_data]}=\"0\";")
            num_modules += 1
        ans = [f"num_modules={num_modules};",
                "module_list = new Module[num_modules];",
                "if (module_list == 0) { cerr << "+'"*** Error: module_list[] allocation failed *** \\n";'+" exit(1); "+"}"] + ans
        while next(lines_iter) != "TIMING\n":
            pass
        while True:
            line = next(lines_iter)
            if line == "END\n":
                break
            else:
                name,structure_datas = line.split()
                ans.append(f"{name}={structure_datas};")
        num_vehicles = next(lines_iter).split()[1]
        num_missile = sum([1 for line in lines if "MISSILE6" in line])
        num_target = sum([1 for line in lines if "TARGET3" in line])
        ans.append(f"num_vehicles={num_vehicles};")
        ans.append(f"num_missile={num_missile};")
        ans.append(f"num_target={num_target};")
        ans.append(f"if (init)\n"+
                    "{\n"+
                    "\tCadac *vehicle_type=new Missile(module_list,num_modules,num_target);\n"+
                    "\tvehicle_list.add_vehicle(*vehicle_type);\n"+
                    "}")

        line = next(lines_iter)
        print("\n".join(ans))
        var_data = []
        while True:
            line = next(lines_iter)
            if line.isspace():
                continue
            if  "END\n" in line:
                break
            else:
                if line.split()[0].startswith('//'):
                    continue
                if "DECK" in line:
                    _,file_name = line.split()
                    deck_table_name = "aerotable" if "aero" in file_name else "proptable"
                    deck_tables.append(deck_trans(input_file_dir+file_name,deck_table_name))
                name,structure_datas,*_ = line.split()
                if name in missile_data_dict:
                    int_data = ('(int)' if missile_data_dict[name][1] else '(double)') + structure_datas
                    var_data.append(f"missile[{missile_data_dict[name][0]}].gets({int_data}); //{name}")
        print("---------------------------deck_table------------------------")
        print("\n".join(var_data))
        for deck_table in deck_tables:
            deck_code = "\n\t".join(deck_table)
            print("if (init)\n"+"{\n"+f"{deck_code}\n"+"}\n")

if __name__ == '__main__':
    input_trans("C:\\Users\\WE\\Desktop\\missile\\test\\MissileData\\SRAAM\\")