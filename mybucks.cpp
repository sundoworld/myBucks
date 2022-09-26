#include <QtGui> 
#include "mybucks.h"

// if we include <QtGui> there is no need to include every class used: <QString>, <QFileDialog>,...

myBucks::myBucks(QMainWindow *parent)
        :QMainWindow(parent)
{
	setupUi( this ); // this sets up GUI
	
	//To remove Maximize option on right top of the window
	Qt::WindowFlags flags;
	flags = Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
	setWindowFlags( flags );
	setWindowIcon(QIcon("purse.png"));

	//To start the app in the centre of the screen
	QDesktopWidget *desktop = QApplication::desktop();

	int screenWidth, width; 
	int screenHeight, height;
	int x, y;
	QSize windowSize;
	 
	screenWidth = desktop->width(); // get width of screen
	screenHeight = desktop->height(); // get height of screen
	 
	windowSize = size(); // size of our application window
	width = windowSize.width(); 
	height = windowSize.height();
	x = (screenWidth - width) / 2;   // little computations to find centre
	y = (screenHeight - height) / 2;
	y -= 50;
	move ( x, y );  // move window to centre
	
	//Add Menu Bar Items here
        QAction *about = new QAction("&About", this);
	QAction *info = new QAction("&Information", this);
        QMenu *file;
	file = menuBar()->addMenu("&Help");
	file->addAction(info);
	file->addAction(about);
	
	/*/To group Radio buttons
	QFile fileee(".csv");
     	if (!fileee.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::about(this, "in.txt",
                                 "File not found");
	}*/
	//Update the current_balances to app
        get();
	QString db = QString::fromUtf8(value[0].c_str());
        QString sbi = QString::fromUtf8(value[1].c_str());
	QString wallet = QString::fromUtf8(value[2].c_str());
        lineEdit->setDisabled(1);
        lineEdit_2->setDisabled(1);
        lineEdit_3->setDisabled(1);
	lineEdit->setText( db );
	lineEdit_2->setText( sbi );
	lineEdit_3->setText( wallet );

	// signals/slots mechanism in action
	connect( pushButton, SIGNAL( clicked() ), this, SLOT( myBucksTrack() ) );
	connect( about, SIGNAL( triggered() ), this , SLOT( about() ) ); 
	connect( info, SIGNAL( triggered() ), this , SLOT( info() ) );
	connect( comboBox_2, SIGNAL( currentIndexChanged( int ) ), this, SLOT( handleSelectionChanged( int ) ));
}


void myBucks::myBucksTrack()
{
	QButtonGroup *bg = new QButtonGroup(this);
	bg->addButton(radioButton);
	bg->addButton(radioButton_2);
	//Here we process the Bank Update
	if(radioButton->isChecked())
	{
		bg->setExclusive(false);
		bg->checkedButton()->setChecked(false);
		bg->setExclusive(true);
		/*QMessageBox::about(this, "Option A",
        	                 "Bank Update. Work in Progress...\n");
		QMessageBox::about(this, "Option 1",
        	                 comboBox->currentText());
		QMessageBox::about(this, "Option 2",
        	                 comboBox_2->currentText());
		QMessageBox::about(this, "Option 3",
        	                 comboBox_3->currentText());
		QMessageBox::about(this, "Option 4",
        	                 lineEdit_4->text());
		QMessageBox::about(this, "Option 5",
        	                 lineEdit_5->text());
		QMessageBox::about(this, "Option 6",
        	                 currentDateTime());*/
		QString buf, db_new, sbi_new, wallet_new;
		double bal = 0.0, wall = 0.0;
		buf.append("\n");
		buf.append(currentDateTime());
		buf.append(",");
		buf.append(comboBox->currentText());
		buf.append(",");
		buf.append(lineEdit_4->text());
		buf.append(",");
		buf.append(comboBox_3->currentText());
		if(comboBox_2->currentIndex()==0) { //debit[-]
			buf.append(",");
			buf.append(lineEdit_5->text());
			buf.append(",");
			buf.append(",");
			bal = (double)atof(value[comboBox->currentIndex()].c_str()) - lineEdit_5->text().toDouble();
			if( checkBox->checkState() == Qt::Checked ) {
				wall = (double)atof(value[2].c_str()) + lineEdit_5->text().toDouble();
				wallet_new = QString::number(wall);
			}
			buf.append(QString::number(bal));
		}
		if(comboBox_2->currentIndex()==1) { //credit[+]
			buf.append(",");
			buf.append(",");
			buf.append(lineEdit_5->text());
			buf.append(",");
			bal = (double)atof(value[comboBox->currentIndex()].c_str()) + lineEdit_5->text().toDouble();
			buf.append(QString::number(bal));
		}
		if( checkBox->checkState() == Qt::Unchecked )
			wallet_new = QString::fromUtf8(value[2].c_str());
		if(comboBox->currentIndex() == 0 ) {
			db_new = QString::number(bal);
			sbi_new = QString::fromUtf8(value[1].c_str());}
		else{
		     db_new = QString::fromUtf8(value[0].c_str());
                sbi_new = QString::number(bal);
		}
	/*	QMessageBox::about(this, "Option 6",
        	                 buf);
		QMessageBox::about(this, "DB_NEW",
				db_new);
		QMessageBox::about(this, "SBI_NEW",
				sbi_new);
		QMessageBox::about(this, "WALLET_NEW",
				wallet_new);
*/
		QFile bankout("bank_updates.csv");
		if( !bankout.open(QIODevice::Append) ) {
			QMessageBox::about(this, "File Write Error",
                                "Unable to Write data into File: \n1. Either no space in HDD.\n2. Or no permission for current user to save file.\n Kindly check for availabity of file named [bank_updates.csv]");}
		else {
			QTextStream fout(&bankout);
			fout << buf;
		}
		QFile balout("current_balances.csv");
                if( !balout.open(QIODevice::WriteOnly) ) {
                        QMessageBox::about(this, "File Write Error",
                                "Unable to Write data into File: \n1. Either no space in HDD.\n2. Or no permission for current user to save file.\n Kindly check for availabity of file named [current_balances.csv]");}
                else {
                        QTextStream fwout(&balout);
                        fwout << "DB," << db_new << endl << "SBI," << sbi_new << endl << "WALLET," << wallet_new << endl << "EOF,999999" ; 
                }

		//Update the current_balances to app
		get();
		QString db = QString::fromUtf8(value[0].c_str());
		QString sbi = QString::fromUtf8(value[1].c_str());
		QString wallet = QString::fromUtf8(value[2].c_str());
		lineEdit->setText( db );
		lineEdit_2->setText( sbi );
		lineEdit_3->setText( wallet );
	}

	//Here we process the Expense Update 
	else if(radioButton_2->isChecked())
	{
		bg->setExclusive(false);
		bg->checkedButton()->setChecked(false);
		bg->setExclusive(true);
		//QMessageBox::about(this, "Option B",
        	//                 "Expense Update. Work in Progress...\n");
		QString buf, db_new, sbi_new, wallet_new;
                double bal = 0.0;
                buf.append("\n");
                buf.append(currentDateTime());
                buf.append(",");
                buf.append(comboBox_4->currentText());
                buf.append(",");
                buf.append(lineEdit_7->text());
                buf.append(",");
                buf.append(lineEdit_6->text());
		QFile expout("expense_updates.csv");
                if( !expout.open(QIODevice::Append) ) {
                        QMessageBox::about(this, "File Write Error",
                                "Unable to Write data into File: \n1. Either no space in HDD.\n2. Or no permission for current user to save file.\n Kindly check for availabity of file named [bank_updates.csv]");}
                else {
                        QTextStream fxout(&expout);
                        fxout << buf;
                }
                bal = (double)atof(value[comboBox_4->currentIndex()].c_str()) - lineEdit_6->text().toDouble();
                //QMessageBox::about(this, "Balance",
                //                   QString::number(bal));
                switch(comboBox_4->currentIndex()) { 
		case 0:				//Deutsche Bank
                        db_new = QString::number(bal);
			sbi_new = QString::fromUtf8(value[1].c_str());
			wallet_new = QString::fromUtf8(value[2].c_str());
			break;
                case 1:				//State Bank of India
			db_new = QString::fromUtf8(value[0].c_str());
                	sbi_new = QString::number(bal);
			wallet_new = QString::fromUtf8(value[2].c_str());
			break;
                case 2:				//Cash from Wallet
			db_new = QString::fromUtf8(value[0].c_str());
			sbi_new = QString::fromUtf8(value[1].c_str());
                        wallet_new = QString::number(bal);
			break;
		}
		QFile baleout("current_balances.csv");
                if( !baleout.open(QIODevice::WriteOnly) ) {
                        QMessageBox::about(this, "File Write Error",
                                "Unable to Write data into File: \n1. Either no space in HDD.\n2. Or no permission for current user to save file.\n Kindly check for availabity of file named [current_balances.csv]");}
                else {
                        QTextStream fwxout(&baleout);
                        fwxout << "DB," << db_new << endl << "SBI," << sbi_new << endl << "WALLET," << wallet_new << endl << "EOF,999999" ;
                }
                //QMessageBox::about(this, "Option 6",
                //                       buf);
                //QMessageBox::about(this, "DB_NEW",
                //                       db_new);
		//Update the current_balances to app
		get();
		QString db = QString::fromUtf8(value[0].c_str());
		QString sbi = QString::fromUtf8(value[1].c_str());
		QString wallet = QString::fromUtf8(value[2].c_str());
		lineEdit->setText( db );
		lineEdit_2->setText( sbi );
		lineEdit_3->setText( wallet );
	}

	//Here Empty Selection is processed...
	else
	{
		QMessageBox::about(this, "Important!!!",
                                 "Select your operation as Bank or Expense Update please...\n");
	}
	lineEdit_4->clear();
	lineEdit_5->clear();
	lineEdit_6->clear();
	lineEdit_7->clear();
}

void myBucks::about()
{
	QMessageBox::about(this, "About myBuck$",
        	                 "Version 1.0a\n"
				 "Author: Sundaresan V\nEmail: sundaresan.24121988@gmail.com\nWebLink @ google+, Linkedin and facebook.\n"
                           	 "Bye.\n");
}

void myBucks::info()
{
	QMessageBox::about(this, "myBuck$ Information",
        	                 "Please update the csv files created now with your details for the first time. \n"
				 "Dont edit the files manually. \nBe cautious for every update of yours. \n"
                           	 "]-[@v3 a n!c3 d@y.\n");
}

void myBucks::get()
{
    std::ifstream  data("current_balances.csv");
    bool id_flag = true;
    std::string line;
    int i=0;
    while(std::getline(data,line))
    {
        std::stringstream  lineStream(line);
        std::string        cell;
        while(std::getline(lineStream,cell,','))
        {
            // You have a cell!!!!
            if (cell == "EOF")
		break;
            if(id_flag)
            { 
               id[i] = cell;
               id_flag = false;
            }
            else
            {
               value[i] = cell;
               id_flag = true;
               i++;  
            }
            std::cout << cell << std::endl;
        }
    }
}

QString myBucks::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%T", &tstruct);
    
    QString dime = QString::fromUtf8(buf);
    return dime;
}

void myBucks::handleSelectionChanged( int index ) {
	checkBox->setDisabled(index);
}
