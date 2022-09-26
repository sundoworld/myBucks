#ifndef MYBUCKS_H
#define MYBUCKS_H

#include "ui_mybucks.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class myBucks : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	myBucks(QMainWindow *parent = 0);
	std::string id[5];
	std::string value[5];
	void get();
	QString currentDateTime();
	
public slots:
	void myBucksTrack();
	void about();
	void info();
	void handleSelectionChanged( int );
};

#endif
