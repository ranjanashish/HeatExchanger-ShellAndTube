#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
        rating(new Rating()),
        sizing(new Sizing(rating))
{
	ui->setupUi(this);
        connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
	connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(on_pushButton_3_clicked()));
        connect(ui->pushButton_results, SIGNAL(clicked()), this, SLOT(on_pushButton_results_clicked()));
        connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(on_pushButton_6_clicked()));
        connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(on_pushButton_8_clicked()));
}

MainWindow::~MainWindow()
{
	delete ui;
	delete rating;
	delete sizing;
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void MainWindow::on_pushButton_clicked()
{
	double geometrical_data[11];
	/**********  Geometrical Data **********/
	geometrical_data[0] = ui->inside_shell_dia->value();
	geometrical_data[1] = ui->outside_tube_dia->value();
	geometrical_data[2] = ui->inside_tube_dia->value();
	geometrical_data[3] = ui->bundle_dia->value();

	geometrical_data[4] = ui->overall_nom_tube_len->value();
	geometrical_data[5] = ui->tube_layout_pitch->value();
	geometrical_data[6] = ui->tube_layout_characteristic_anlge->currentText().toInt();
	geometrical_data[7] = ui->segmental_baffle_cut->value();

	geometrical_data[8] = ui->num_of_tubes->value();
	geometrical_data[9] = ui->num_of_tube_passes->currentText().toInt();
	geometrical_data[10] = ui->num_of_sealing_strips->value();
	/**********  Geometrical Data **********/

	rating->input_geometrical_data(&geometrical_data[0]);
        QMessageBox::information(this, "Geometrical Data", "Geometrical Data SAVED!");
}

void MainWindow::on_pushButton_3_clicked()
{
        double thermophysical_data[16];
        /**********  Temperatures **********/
        thermophysical_data[0] = ui->temp_shell_in->value();
        thermophysical_data[1] = ui->temp_shell_out->value();
        thermophysical_data[2] = ui->temp_tube_in->value();
        thermophysical_data[3] = ui->temp_tube_out->value();
	/**********  Temperatures **********/

	/**********  Shell Fluid Thermophysical Properties **********/
        thermophysical_data[4] = ui->specific_heat_shell_fluid->value();
        thermophysical_data[5] = ui->density_shell_fluid->value();
        thermophysical_data[6] = ui->thermal_conductivity_shell_fluid->value();
        thermophysical_data[7] = ui->dynamic_viscosity_shell_fluid->value();
        thermophysical_data[8] = ui->fouling_resistance_shell_fluid->value();
	/**********  Shell Fluid Thermophysical Properties **********/

	/**********  Tube Fluid Thermophysical Properties **********/
        thermophysical_data[9] = ui->specific_heat_tube_fluid->value();
        thermophysical_data[10] = ui->density_tube_fluid->value();
        thermophysical_data[11] = ui->thermal_conductivity_tube_fluid->value();
        thermophysical_data[12] = ui->dynamic_viscosity_tube_fluid->value();
        thermophysical_data[13] = ui->fouling_resistance_tube_fluid->value();
	/**********  Tube Fluid Thermophysical Properties **********/

	/**********  Process Data **********/
        thermophysical_data[14] = ui->shell_fluid_mass_flow_rate->value();
        thermophysical_data[15] = ui->tube_fluid_mass_flow_rate->value();
	/**********  Process Data **********/

        rating->input_thermophysical_data(&thermophysical_data[0]);
        sizing->input_thermophysical_data(&thermophysical_data[0]);
        QMessageBox::information(this, "Thermophysical Data", "Thermophysical Data SAVED!");
}

void MainWindow::on_pushButton_results_clicked()
{
        if(ui->radioButton_rating_program->isChecked()){
            rating->initialize();
            double shell_side_htc = rating->shell_side_htc();
            double shell_side_pd  = rating->shell_side_pd();
            QString res1 = "The shell side heat transfer coefficient is <b>" + QString::number(shell_side_htc) + " W/m<sup>2</sup>K</b> <br />";
            QString res2 = "The shell side pressure drop is <b>" + QString::number(shell_side_pd) + " kPa</b> <br />";
            QString res = res1 + res2;
            ui->results->setText(res);
        }
        else{
            QString absolute_path_to_sizing_result_file = QFileDialog::getSaveFileName(this, tr("Open Sizing Result File"), "/", tr("Text files (*.txt)"));
            sizing->initialize();
            sizing->calculate(absolute_path_to_sizing_result_file);
            QString res = "The result has been written in the file <b>" + absolute_path_to_sizing_result_file + "</b>";
            ui->results->setText(res);
        }
}

void MainWindow::on_pushButton_6_clicked()
{
        if(ui->radioButton_rating_program->isChecked()){

        }
        else{
        }
}

void MainWindow::on_pushButton_8_clicked()
{
        double customer_constraints[3];
        /**********  Estimations **********/
        customer_constraints[0] = ui->estimated_htc->value();
        /**********  Estimations **********/

        /**********  Constraints **********/
        customer_constraints[1] = ui->max_allowable_pd_shell_side->value();
        customer_constraints[2] = ui->max_allowable_pd_tube_side->value();
        /**********  Constraints **********/

        sizing->input_customer_constraints(&customer_constraints[0]);
        QMessageBox::information(this, "Customer Constraints Data", "Customer Constraints Data SAVED!");
}
