#ifndef SIZING_H
#define SIZING_H

#include <cmath>
#include <QtCore>
#include <QtGui>
#include "Rating.h"

#define PI 3.1415926

class Sizing{
        public:
                Sizing(Rating *);
                void input_thermophysical_data(double *);
                void input_customer_constraints(double *);
                void initialize();
                void calculate(QString);
                inline double lmtd() const;
                inline double lmtd_factor() const;
                inline double heat_transfer() const;
        private:
                Rating *r;

                /************************************************** INPUT DATA **************************************************/
                /********** TEMPERATURES **********/
                double T_si;		//in degree celcius: shell-side temperature inlet
                double T_so;		//in degree celcius: shell-side temperature outlet
                double T_ti;		//in degree celcius: tube-side temperature inlet
                double T_to;		//in degree celcius: tube-side temperature outlet
                /********** TEMPERATURES **********/

                /********** ESTIMATIONS **********/
                double U;           //in W/m2.K: overall heat transfer coefficient
                /********** ESTIMATIONS **********/

                /********** CONSTRAINTS **********/
                double max_pd_s;    //in kPa: maximum allowable pressure drop (shell-side)
                double max_pd_t;    //in kPa: maximum allowable pressure drop (tube-side)
                /********** CONSTRAINTS **********/

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
                /**************************************** CALCULATED DATA FROM INPUT DATA ****************************************/
};

#endif // SIZING_H
