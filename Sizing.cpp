#include <Sizing.h>

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Sizing
 *      Method:  Sizing :: Sizing
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Sizing::Sizing(Rating *rat){
        r = rat;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Sizing
 *      Method:  Sizing :: input_customer_constraints
 * Description:  inputs customer constraints data
 *--------------------------------------------------------------------------------------
 */
void Sizing::input_customer_constraints(double *ptr){
	/**********  Estimations **********/
	U = *ptr++;
	/**********  Estimations **********/

	/**********  Constraints **********/
	max_pd_s = *ptr++;
	max_pd_t = *ptr;
	/**********  Constraints **********/
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Sizing
 *      Method:  Sizing :: input_thermophysical_data
 * Description:  inputs thermophysical data
 *--------------------------------------------------------------------------------------
 */
void Sizing::input_thermophysical_data(double *ptr){
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
 *       Class:  Sizing
 *      Method:  Sizing :: initialize
 * Description:  initializes the basic variables
 *--------------------------------------------------------------------------------------
 */
void Sizing::initialize(){
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
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Sizing
 *      Method:  Sizing :: calculate
 * Description:  carries out the sizing of a shell and tube heat exchanger
 *--------------------------------------------------------------------------------------
 */
void Sizing::calculate(QString absolute_path_to_sizing_result_file){
        /********** Resutl File **********/
        if(QFile::exists(absolute_path_to_sizing_result_file))
                if(QFile::remove(absolute_path_to_sizing_result_file)){ }

        QFile sizing_result_file(absolute_path_to_sizing_result_file);
        if(sizing_result_file.open(QIODevice::WriteOnly)){ }
        QTextStream out(&sizing_result_file);
        /********** Resutl File **********/

        double geometrical_data[11];

        /********** Default Values **********/
        theta_tp = 30;   //in degrees: tube layout characteristic angle
        B_c = 25; 	 //in percentage: segmental baffle cut
        N_tp = 2;	 //number of tube passes
        N_ss = 5;	 //number of sealing strips (pairs)
        /********** Default Values **********/

	double A = (heat_transfer() / (U * lmtd_factor() * lmtd()));
        const double tube_layout_density[19][4] = { { 6,  9.0, 0.211, 0.5},
                                                    {10, 13.5, 0.156, 1.5},
                                                    {12, 15.5, 0.142, 1.5},
                                                    {14, 18.0, 0.123, 2.0},
                                                    {14, 19.0, 0.110, 2.0},
                                                    {16, 20.0, 0.117, 2.0},
                                                    {16, 21.0, 0.103, 2.0},
                                                    {18, 23.0, 0.096, 2.0},
                                                    {18, 24.0, 0.089, 2.0},
                                                    {20, 25.0, 0.091, 2.0},
                                                    {20, 26.0, 0.084, 2.0},
                                                    {22, 27.0, 0.086, 2.5},
                                                    {22, 29.0, 0.074, 2.5},
                                                    {25, 30.0, 0.079, 2.5},
                                                    {25, 32.0, 0.069, 2.5},
                                                    {30, 36.0, 0.066, 2.5},
                                                    {30, 38.0, 0.059, 2.5},
                                                    {38, 45.0, 0.053, 2.5},
                                                    {38, 47.0, 0.490, 2.5} };

        double L_ta;
        for(L_ta = 4500; L_ta < 6500; L_ta += 50){
                for(int i = 0; i < 19; i++){
			D_t = tube_layout_density[i][0];
			L_tp = tube_layout_density[i][1];
			D_ti = D_t - 2 * tube_layout_density[i][3];
                        double A_star = tube_layout_density[i][2];
                        double D_ctl = sqrt((1000000*A)/ (A_star * L_ta));

                        if((D_ctl/L_ta) > 0.1 && (D_ctl/L_ta) < 0.2){
				D_otl = D_ctl + D_t;
                                double N_tt_temp = (0.9 * D_ctl * D_ctl) / (L_tp * L_tp);
                                D_s = (13.89 + D_ctl + D_t) / 0.99556;

                                double L_ts = 0.1 * D_s;
                                double L_ti = L_ta - 0.3 * D_otl;
                                L_to = L_ti + L_ts;

                                const double phi[9][2] = { { 200, 0.150},
                                                           { 250, 0.100},
                                                           { 300, 0.075},
                                                           { 400, 0.050},
                                                           { 600, 0.040},
                                                           { 800, 0.035},
                                                           {1000, 0.025},
                                                           {1500, 0.020},
                                                           {2000, 0.010} };
				for(int k = 0; k<9 ; k++){
                                        if((D_ctl > phi[k][0])   && (D_ctl < phi[k+1][0])){
                                                double phi_n = (phi[k][1] + phi[k+1][1])/2;
                                                N_tt = N_tt_temp * (1 - phi_n);
					}
				}
			}
                        else
                                continue;

                        geometrical_data[0] = D_s;
                        geometrical_data[1] = D_t;
                        geometrical_data[2] = D_ti;
                        geometrical_data[3] = D_otl;
                        geometrical_data[4] = L_to;
                        geometrical_data[5] = L_tp;
                        geometrical_data[6] = theta_tp;
                        geometrical_data[7] = B_c;
                        geometrical_data[8] = N_tt;
                        geometrical_data[9] = N_tp;
                        geometrical_data[10] = N_ss;

                        r->input_geometrical_data(&geometrical_data[0]);
                        r->initialize();

                        double pd_shell = r->shell_side_pd();

                        if(pd_shell < max_pd_s){
                            QString res1 = "Inside Shell Diameter                    : " + QString::number(D_s) + " mm";
                            out << res1 << "\n";
                            QString res2 = "Outside Tube Diameter                    : " + QString::number(D_t) + " mm";
                            out << res2 << "\n";
                            QString res3 = "Inside Tube Diameter                     : " + QString::number(D_ti) + " mm";
                            out << res3 << "\n";
                            QString res4 = "Number of Tubes                          : " + QString::number(N_tt) + " ";
                            out << res4 << "\n";
                            QString res5 = "Shell-side Pressure Drop                 : " + QString::number(pd_shell) + " kPa";
                            out << res5 << "\n";
                            QString res6 = "Max Allowable Pressure Drop (shell-side) : " + QString::number(max_pd_s) + " kPa";
                            out << res6 << "\n \n";
                        }
		}
	} 
        sizing_result_file.close();
}	

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Sizing
 *      Method:  Sizing :: lmtd
 * Description:  calculates the logarithmic mean temperature difference
 *--------------------------------------------------------------------------------------
 */
inline double Sizing::lmtd() const{
	double delta_temp_1 = temp_hot_in - temp_cold_out;
	double delta_temp_2 = temp_hot_out - temp_cold_in;
	return (delta_temp_1 - delta_temp_2)/log(delta_temp_1/delta_temp_2);
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Sizing
 *      Method:  Sizing :: lmtd_factor
 * Description:  calculates the lmtd correction factor
 *  Conditions:  (a) one shell-side pass
 *               (b) two tube-side pass
 *--------------------------------------------------------------------------------------
 */
inline double Sizing::lmtd_factor() const{
	double P = (temp_cold_out - temp_cold_in)/(temp_hot_in - temp_cold_in);
	double R = (temp_hot_in - temp_hot_out)/(temp_cold_out - temp_cold_in);
	return ((sqrt(R*R + 1) * log((1 - P) / (1 - P*R))) / ((R-1)*log((2 - P*(R + 1 - sqrt(R*R + 1)))/(2 - P*(R + 1 + sqrt(R*R + 1))))));
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Sizing
 *      Method:  Sizing :: heat_transfer
 * Description:  calculates the amount of heat transfer taking place in the HX
 *--------------------------------------------------------------------------------------
 */
inline double Sizing::heat_transfer() const{
	return std::abs(M_s * c_p_s * (T_si - T_so));
}
