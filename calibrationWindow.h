
#ifndef PCH_H
#define PCH_H

#include <QWidget>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qtextedit.h>

#include "traitementPhonemes.h"

const int NUM_REP = 2;

class calibWindow: public QWidget
{

public:
	calibWindow();
	~calibWindow();

	void writeToOutput(std::string msg, bool writeToTitle = false);

private slots:
	void mainButtonClicked();
	void cancelButtonClicked();

private:
	void createObjects();
	void createLayout();
	void createMenu();

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
	CommunicationFPGA *port;
	bool isConnection = false;
	bool canceledLastPhoneme = false;
};

#endif //PCH_H
