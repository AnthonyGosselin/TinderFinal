
#ifndef PCH_H
#define PCH_H

#include <QWidget>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qtextedit.h>

#include "traitementPhonemes.h"

const int NUM_REP = 1;

class calibWindow : public QWidget
{
public:
	calibWindow();
	~calibWindow();

	void writeToOutput(std::string msg, bool writeToTitle = false);

private slots:
	void mainButtonClicked();

private:
	void createObjects();
	void createLayout();

	QLabel *mainLabel;
	QTextEdit *output;
	QPushButton *mainButton;
	QGridLayout *layout;

	bool calibrationDone = false;
	int currPhoneme = 0;
	int currRep = 0;
	PhonemeRef phonemeRefTab[NUM_PHONEMES];
	PhonemeRef *recordingPhoneme;
	CommunicationFPGA *port;
	bool isConnection = false;
};

#endif //PCH_H
