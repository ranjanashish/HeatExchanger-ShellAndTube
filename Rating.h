#ifndef RATING_H
#define RATING_H

#include <cmath>

#define PI 3.1415926

class Rating{
        public:
                Rating();
                void input_geometrical_data(double *);
                void input_thermophysical_data(double *);
                void initialize();
                inline double lmtd() const;
                inline double lmtd_factor() const;
                inline double heat_transfer() const;
                double h_s_ideal() const;
                double shell_side_htc() const;
		double shell_side_pd() const;
                double tube_side_htc() const;
                double tube_side_pd() const;
        private:
                /************************************************** INPUT DATA **************************************************/
                /********** TEMPERATURES **********/
                double T_si;		//in degree celcius: shell-side temperature inlet
                double T_so;		//in degree celcius: shell-side temperature outlet
                double T_ti;		//in degree celcius: tube-side temperature inlet
                double T_to;		//in degree celcius: tube-side temperature outlet
                /********** TEMPERATURES **********/

                /********** GEOMETRICAL DATA **********/
                double D_s;	//in mm: inside shell diameter
                double D_t;	//in mm: outside tube diameter
                double D_ti;	//in mm: inside tube diameter
                double D_otl;	//in mm: bundle diameter

                double L_to;	//in mm: overall nominal tube length
                double L_tp;	//in mm: tube layout pitch
                int theta_tp;   //in degrees: tube layout characteristic angle

                double B_c;	//in percentage: segmental baffle cut

                int N_tt;	//number of tubes or holes in tubesheet (for U tubes)
                int N_tp;	//number of tube passes
                int N_ss;	//number of sealing strips (pairs)
                /********** GEOMETRICAL DATA **********/

                /********** PROCESS DATA **********/
                double M_s;	//in kg/s: shell fluid mass flow rate
                double M_t;	//in kg/s: tube fluid mass flow rate
                /********** PROCESS DATA **********/

                /********** AT SHELL FLUID MEAN TEMPERATURE **********/
                double c_p_s;	//in J/kg.K: specific heat
                double rho_s;	//in kg/m3 : density
                double k_s;	//in W/m.K : thermal conductivity
                double mu_s;	//in kg/m.s: dynamic viscosity
                double R_f_o;	//in m.K/W : shell-side fouling resistance (referred to shell-side surface)
                /********** AT SHELL FLUID MEAN TEMPERATURE **********/

                /********** AT TUBE FLUID MEAN TEMPERATURE **********/
                double c_p_t;	//in J/kg.K: specific heat
                double rho_t;	//in kg/m3 : density
                double k_t;	//in W/m.K : thermal conductivity
                double mu_t;	//in kg/m.s: dynamic viscosity
                double R_f_i;	//in m.K/W : tube-side fouling resistance (referred to inside tube surface)
                /********** AT TUBE FLUID MEAN TEMPERATURE **********/
                /************************************************** INPUT DATA **************************************************/

                /**************************************** CALCULATED DATA FROM INPUT DATA ****************************************/
                /********** TEMPERATURES **********/
                double temp_hot_in;	//in degree celcius
                double temp_hot_out;	//in degree celcius
                double temp_cold_in;	//in degree celcius
                double temp_cold_out;	//in degree celcius
                /********** TEMPERATURES **********/

                /********** GEOMETRICAL DATA **********/
                double L_tw;	//in mm: tube wall thickness
                double L_b;	//in mm: baffle spacing

                double L_ts;	//in mm: tubesheet thickness
                double L_ti;	//in mm: baffled tube length
                double L_ta;	//in mm: effective tube length for heat transfer area

                double L_tb;	//in mm: (tube outside diameter)-to-baffle hole clearance (diametral)
                double L_sb;	//in mm: inside shell-to-baffle clearance (diametral)
                double L_bb;	//in mm: inside shell-to-tube clearance (diametral)

                double D_ctl;	//in mm:
                double L_pp;	//in mm: tube vertical pitch
                /********** GEOMETRICAL DATA **********/

                /************************************************** BELL-DELAWARE METHOD **************************************************/
                /******************** (1) EVALUATION OF CRITICAL ITEMS IN INPUT DATA ********************/
                /******************** (1) EVALUATION OF CRITICAL ITEMS IN INPUT DATA ********************/

                /******************** (2) CALCULATION OF BASIC PARAMETERS ********************/
                double S_m;		//in mm2           : cross-flow area
                double m_s;		//in kg/m2.s       : cross-flow mass velocity
                double Re_s;		//dimensionless    : shell-side reynolds number
                double Pr_s;		//dimensionless    : shell-side prandtl number
                double delta_T_M;	//in degree celcius: mean temperature difference
                /******************** (2) CALCULATION OF BASIC PARAMETERS ********************/

                /******************** (3) AUXILIARY CALCULATIONS ********************/
                double theta_ds;	//in degrees   :
                double theta_ctl;	//in degrees   :
                double F_w;		//dimensionless: fraction of number of tubes in one window
                double F_c;		//dimensionless: fraction of number of tubes in pure cross-flow between baffle tips
                double N_tcc;		//dimensionless: number of effective tube rows crossed in one cross-flow section, that is, between baffle tips
                double N_tcw;		//dimensionless: number of effective tube rows crossed in one baffle window section
                double N_b;		//dimensionless: number of baffles
                double S_b;		//in mm2       : bundle-to-shell bypass area within one baffle
                double S_sb;		//in mm2       : shell-to-baffle leakage area
                double S_tb;		//in mm2       : tube-to-baffle hole leakage area for one baffle
                /******************** (3) AUXILIARY CALCULATIONS ********************/
                /************************************************** BELL-DELAWARE METHOD **************************************************/
                /**************************************** CALCULATED DATA FROM INPUT DATA ****************************************/
};

#endif // RATING_H
