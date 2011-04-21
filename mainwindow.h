#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "ui_mainwindow.h"
#include "Rating.h"
#include "Sizing.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

        private slots:
                void on_pushButton_8_clicked();
                void on_pushButton_6_clicked();
                void on_pushButton_results_clicked();
                void on_pushButton_clicked();
                void on_pushButton_3_clicked();

	protected:
		void changeEvent(QEvent *e);

	private:
		Ui::MainWindow *ui;
                Rating *rating;
                Sizing *sizing;
};

#endif // MAINWINDOW_H
