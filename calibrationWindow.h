
#ifndef PCH_H
#define PCH_H

#include <QWidget>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qtextedit.h>
#include <qscrollbar.h>

#include "traitementPhonemes.h"

const int NUM_REP = 2;
const double CALIB_READ_TIME = 2000; //ms

class calibWindow: public QWidget
{

public:
	calibWindow(CommunicationFPGA &port);
	~calibWindow();

	void writeToOutput(std::string msg, bool writeToTitle = false);

private slots:
	void mainButtonClicked();
	void cancelButtonClicked();

private:
	void createObjects();
	void createLayout();
	void createMenu();
	void connectFPGA(bool fpga_connected);

	QLabel *mainLabel;
	QTextEdit *output;
	QPushButton *mainButton;
	QPushButton *cancelButton;
	QGridLayout *layout;

	bool calibrationDone = false;
	int currPhoneme = 0;
	int currRep = 0;
	PhonemeRef phonemeRefTab[NUM_PHONEMES];
	PhonemeRef *recordingPhoneme;
	PhonemeRef *lastRecordedPhoneme;
	bool isConnection = false;
	bool canceledLastPhoneme = false;

	CommunicationFPGA *ptr_port;
};

#endif //PCH_H
