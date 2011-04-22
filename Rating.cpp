#include	"Rating.h"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: Rating
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Rating::Rating(){
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: input_geometrical_data
 * Description:  inputs geometrical data
 *--------------------------------------------------------------------------------------
 */
void Rating::input_geometrical_data(double *ptr){
	/**********  Geometrical Data **********/
	D_s      = *ptr++;
	D_t      = *ptr++;
	D_ti     = *ptr++;
	D_otl    = *ptr++;

	L_to     = *ptr++;
	L_tp     = *ptr++;
	theta_tp = *ptr++;
	B_c      = *ptr++;

	N_tt     = *ptr++;
	N_tp     = *ptr++;
	N_ss     = *ptr;
	/**********  Geometrical Data **********/
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: input_thermophysical_data
 * Description:  inputs thermophysical data
 *--------------------------------------------------------------------------------------
 */
void Rating::input_thermophysical_data(double *ptr){
	/**********  Temperatures **********/
	T_si  = *ptr++;
	T_so  = *ptr++;
	T_ti  = *ptr++;
	T_to  = *ptr++;
	/**********  Temperatures **********/

	/**********  Shell Fluid Thermophysical Properties **********/
	c_p_s = *ptr++;
	rho_s = *ptr++;
	k_s   = *ptr++;
	mu_s  = *ptr++;
	R_f_o = *ptr++;
	/**********  Shell Fluid Thermophysical Properties **********/

	/**********  Tube Fluid Thermophysical Properties **********/
	c_p_t = *ptr++;
	rho_t = *ptr++;
	k_t   = *ptr++;
	mu_t  = *ptr++;
	R_f_i = *ptr++;
	/**********  Tube Fluid Thermophysical Properties **********/

	/**********  Process Data **********/
	M_s  = *ptr++;
	M_t  = *ptr;
	/**********  Process Data **********/
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: initialize
 * Description:  initializes the basic variables
 *--------------------------------------------------------------------------------------
 */
void Rating::initialize(){
	/********** TEMPERATURES **********/
	if(T_si > T_ti){
		temp_hot_in = T_si;	//in degree celcius
		temp_hot_out = T_so;	//in degree celcius
		temp_cold_in = T_ti;	//in degree celcius
		temp_cold_out = T_to;	//in degree celcius
	}
	else{
		temp_hot_in = T_ti;	//in degree celcius
		temp_hot_out = T_to;	//in degree celcius
		temp_cold_in = T_si;	//in degree celcius
		temp_cold_out = T_so;	//in degree celcius
	}
	/********** TEMPERATURES **********/

	/********** GEOMETRICAL DATA **********/
	L_tw = (D_t - D_ti) / 2;					//in mm: tube wall thickness
	if(0.2 * D_s > 50.8)
                L_b = 0.2 * D_s;					//in mm: baffle spacing
	else
		L_b = 50.8;						//in mm: baffle spacing

	L_ts = 0.1 * D_s;						//in mm: tubesheet thickness
	L_ti = L_to - L_ts;						//in mm: baffled tube length (for U-tube bundles)
	L_ta = L_ti + 0.3 * D_otl;					//in mm: effective tube length for heat transfer area (for U-tube bundles)

	L_tb = 0.8;							//in mm: (tube outside diameter)-to-baffle hole clearance (diametral)
	L_sb = (1.6 + 0.004 * D_s);  			 		//in mm: inside shell-to-baffle clearance (diametral)
	L_bb = (D_s - D_otl);						//in mm: inside shell-to-tube clearance (diametral)

	D_ctl = D_otl - D_t;						//in mm:

	if(theta_tp == 30)		//triangular layout
		L_pp = 0.866 * L_tp;					//in mm: tube vertical pitch
	else if(theta_tp == 90)		//square layout
		L_pp = L_tp;						//in mm: tube vertical pitch
	else if(theta_tp == 45)		//rotated square layout
		L_pp = 0.707 * L_tp;					//in mm: tube vertical pitch
	/********** GEOMETRICAL DATA **********/

	/************************************************** BELL-DELAWARE METHOD **************************************************/
	/******************** (1) EVALUATION OF CRITICAL ITEMS IN INPUT DATA ********************/
	/******************** (1) EVALUATION OF CRITICAL ITEMS IN INPUT DATA ********************/

	/******************** (2) CALCULATION OF BASIC PARAMETERS ********************/
	/*** (1)S_m : cross-flow area (in mm2) ***/
	S_m = L_b * (L_bb + ((D_ctl * (L_tp - D_t)) / L_tp));

        /*** (2)m_s : cross-flow mass velocity (in kg/m2.s) ***/
	m_s = (M_s / S_m) * pow(10, 6);

	/*** (3)Re_s : shell-side reynolds number (dimensionless) ***/
	Re_s = ((m_s * (D_t / 1000)) / mu_s);

        /*** (4)Pr_s : shell-side prandtl number (dimensionless) ***/
	Pr_s = (c_p_s * mu_s / k_s);

	/*** (5)delta_T_M : mean temperature difference (in degree celcius) ***/
	delta_T_M = (lmtd_factor() * lmtd());
	/******************** (2) CALCULATION OF BASIC PARAMETERS ********************/

	/******************** (3) AUXILIARY CALCULATIONS ********************/
	/*** (1)theta_ds : (in degrees) ***/
	theta_ds = 2 * acos(1 - (B_c/50)) * 57.29578049;

	/*** (2)theta_ctl : (in degrees) ***/
	theta_ctl = 2 * acos((D_s / D_ctl)*(1 - (B_c/50)))*57.29578049;

	/*** (3)F_w : fraction of number of tubes in one window (dimensionless) ***/
	F_w = (theta_ctl / 360) - (sin(theta_ctl / 57.29578049) / 6.2831852);

	/*** (4)F_c : fraction of number of tubes in pure cross-flow between baffle tips (dimensionless) ***/
	F_c = 1 - 2 * F_w;

	/*** (5)N_tcc : number of effective tube rows crossed in one cross-flow section, that is, between the baffle tips (dimensionless) ***/
	N_tcc = D_s * (1 - (B_c/50)) / L_pp;

	/*** (6)N_tcw : number of effective tube rows crossed in one baffle window section (dimensionless) ***/
	N_tcw = 0.8 * ((D_s * (B_c/100)) - ((D_s - D_ctl) / 2)) / L_pp;

	/*** (7)N_b : number of baffles (dimensionless) ***/
	N_b = (L_ti / L_b) - 1;

	/*** (8)S_b : bundle-to-shell bypass area within one baffle (in mm2) ***/
	S_b = L_b * (D_s - D_otl);

	/*** (9)S_sb : shell-to-baffle leakage area (in mm2) ***/
	S_sb = 0.00436 * D_s * L_sb * (360 - theta_ds);

	/*** (10)S_tb : tube-to-baffle hole leakage area for one baffle (in mm2) ***/
	S_tb = (0.78539815 * (pow((D_t + L_tb),2) - D_t * D_t) * N_tt * (1 - F_w));
	/******************** (3) AUXILIARY CALCULATIONS ********************/
	/************************************************** BELL-DELAWARE METHOD **************************************************/
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: lmtd
 * Description:  calculates the logarithmic mean temperature difference
 *--------------------------------------------------------------------------------------
 */
inline double Rating::lmtd() const{
	double delta_temp_1 = temp_hot_in - temp_cold_out;
	double delta_temp_2 = temp_hot_out - temp_cold_in;
	return (delta_temp_1 - delta_temp_2)/log(delta_temp_1/delta_temp_2);
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: lmtd_factor
 * Description:  calculates the lmtd correction factor
 *  Conditions:  (a) one shell-side pass
 *               (b) two tube-side pass
 *--------------------------------------------------------------------------------------
 */
inline double Rating::lmtd_factor() const{
	double P = (temp_cold_out - temp_cold_in)/(temp_hot_in - temp_cold_in);
	double R = (temp_hot_in - temp_hot_out)/(temp_cold_out - temp_cold_in);
	return ((sqrt(R*R + 1) * log((1 - P) / (1 - P*R))) / ((R-1)*log((2 - P*(R + 1 - sqrt(R*R + 1)))/(2 - P*(R + 1 + sqrt(R*R + 1))))));
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: heat_transfer
 * Description:  calculates the amount of heat transfer taking place in the HX
 *--------------------------------------------------------------------------------------
 */
inline double Rating::heat_transfer() const{
	return std::abs(M_s * c_p_s * (T_si - T_so));
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: h_s_ideal
 * Description:  h_ideal : calculates the ideal heat transfer coefficient for the shell-side
 *--------------------------------------------------------------------------------------
 */
double Rating::h_s_ideal() const{
	/********** j_i **********/
	double a1 = 0.321;
	double a2 = -0.388;
	double a3 = 1.45;
	double a4 = 0.519;
	double a = a3 / (1 + (0.14 * pow(Re_s, a4))); 				//values of a1,a2,a3,a4 for different values of Re in the table
	double j_i = a1 * pow (Re_s, a2) * pow(((1.33 * D_t) / L_tp), a);
	/********** j_i **********/

	return j_i * c_p_s * m_s * pow(Pr_s, -0.666667);
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: shell_side_htc
 *  Description:  h_s : calculates shell side heat transfer coefficient
 *                (a) h_ideal :	ideal heat transfer coefficient
 *                (b) J_c     : segmental baffle window correction factor
 *                (c) J_l     : baffle leakage correction factor
 *                (d) J_b     : bypass correction factor from tube bundle to shell
 *                (e) J_r     : laminar heat transfer correction factor for adverse temperature gradient (applicable only for Re < 100)
 *                (f) J_s     : non-equal inlet outlet baffle spacing correction factor
 *--------------------------------------------------------------------------------------
 */
double Rating::shell_side_htc() const{
	/********** J_c **********/
	double J_c = (0.55 + 0.72 * F_c);
	/********** J_c **********/

	/********** J_l **********/
	double r_s      = S_sb / (S_sb + S_tb);
	double r_lm     = (S_sb + S_tb) / S_m;
	double J_l = (0.44 * (1 - r_s) + (1 - 0.44 * (1 - r_s)) * exp(-2.2 * r_lm));
	/********** J_l **********/

	/********** J_b **********/
	double F_sbp = S_b / S_m;					//dimesionless: ratio of bypass to cross-flow area
	double r_ss = N_ss / N_tcc;
	double C_bh;
	if(Re_s < 100)
		C_bh = 1.35;
	else
		C_bh = 1.25;
	double J_b = exp(-(C_bh*F_sbp) * (1 - pow(2*r_ss , 0.33333)));
	/********** J_b **********/

	return (h_s_ideal() * J_c * J_l * J_b);
}


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: shell_side_pd
 * Description:  calculates shell side pressure drop
 *--------------------------------------------------------------------------------------
 */
double Rating::shell_side_pd() const{
	/********** f_i **********/
	double b1 = 0.372;
	double b2 = -0.123;
	double b3 = 7;
	double b4 = 0.5;
	double b = b3 / (1 + (0.14 * pow(Re_s, b4)));
	double f_i = b1 * pow(Re_s, b2) * pow(((1.33 * D_t) / L_tp), b);
	/********** f_i **********/

	/********** delta_pw **********/
	double r_s      = S_sb / (S_sb + S_tb);
	double r_lm     = (S_sb + S_tb) / S_m;
	double p = -0.15 * (1 + r_s) + 0.81;
	double R_l = exp(-1.33 * (1 + r_s) * pow(r_lm, p));
	double S_wg = (PI * (D_s) * (D_s) * ((theta_ds/360) - (sin(theta_ds)/(2*PI))))/4;
	double F_w = (theta_ctl/360)-(sin(theta_ctl)/(2*PI));
	double S_wt = N_tt * F_w * (PI/4) * D_t * D_t;
	double S_w = S_wg - S_wt;
	double m_w = (1000000 * M_s) / sqrt(S_m * S_w);
	double delta_pw = N_b * R_l * (((2 + (0.6 * N_tcw)) * m_w * m_w)/(2000 * rho_s));
	/********** delta_pw **********/

	/********** delta_pc **********/
	double F_sbp = S_b / S_m;					//dimesionless: ratio of bypass to cross-flow area
	double r_ss = N_ss / N_tcc;
	double C_bp = 3.7;
	double R_b = exp(-(C_bp * F_sbp) * (1 - pow(2 * r_ss, 0.33333)));
	double delta_pbi = 0.002 * f_i * N_tcc * M_s * M_s / rho_s;
	double delta_pc = delta_pbi * (N_b - 1) * R_b * R_l;
	/********** delta_pc **********/

	/********** delta_pe **********/
	double R_s = 2;
	double delta_pe = delta_pbi * (1+(N_tcw/N_tcc)) * R_b * R_s ;
	/********** delta_pe **********/

	return delta_pc + delta_pw + delta_pe;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: tube_side_htc
 * Description:  calculates tube side heat transfer coefficient
 *--------------------------------------------------------------------------------------
 */
double Rating::tube_side_htc() const{
        double tube_cs_area = 0.78539816 * D_ti * D_ti;                                    //in mm2: tube cross-sectional area
        double total_tube_cs_area_per_pass = (N_tt / N_tp) * tube_cs_area;                 //in mm2: total tube cross-sectional area per pass
        double u_t = (M_t * pow(10, 6) / (rho_t * total_tube_cs_area_per_pass));           //in m/s:
        double Re_t = (rho_t * u_t * D_ti) / (mu_t * 1000);                                //dimensionless: tube-side reynolds number
        double Pr_t = (mu_t * c_p_t) / k_t;                                                //dimensionless: tube-side prandtl number
        return ((0.027 * pow(Re_t, 0.8) * pow(Pr_t, 0.33) * k_t * 1000) / D_ti);           //in W/m2.K: tube side heat transfer coefficient
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Rating
 *      Method:  Rating :: tube_side_pd
 * Description:  calculates tube side pressure drop
 *--------------------------------------------------------------------------------------
 */
double Rating::tube_side_pd() const{
        double tube_cs_area = 0.78539816 * D_ti * D_ti;                                    //in mm2: tube cross-sectional area
        double total_tube_cs_area_per_pass = (N_tt / N_tp) * tube_cs_area;                 //in mm2: total tube cross-sectional area per pass
        double u_t = (M_t * pow(10, 6) / (rho_t * total_tube_cs_area_per_pass));           //in m/s:
        double Re_t = (rho_t * u_t * D_ti) / (mu_t * 1000);                                //dimensionless: tube-side reynolds number
        double f = pow((0.79 * log(Re_t) - 1.64), -2);                                     //dimensionless: darcy friction factor
        return ((N_tp * ((f * (L_to / D_ti)) + 2.5) * rho_t * u_t * u_t) / (2 * 1000));    //in kPa: tube side pressure drop
}
