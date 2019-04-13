#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qmenubar.h>
#include <qmenu.h>
#include <qgroupbox.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <qgridlayout.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qtextedit.h>
#include <qpixmap.h>

#include <qstringlist.h>
#include <qfiledialog.h>

#include "fluxy.h"
#include "CommunicationFPGA.h"
#include "calibrationWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(CommunicationFPGA &port, QWidget *parent = 0);
    ~MainWindow();

	//void setPartner(QWidget *partner);

private slots:
	void popUpQuitter();
	void popUpConnexion();
	void browseImage();
	void popUpInscription();
	void confirmConnexion();
	void confirmInscription();
	void openSecondWindow();

private: 

	QWidget *m_deuxiemeFenetre;	//L'AUTRE FENETRE

	QString m_username;

	void createMenu();
	void createGroupBoxConnexion();
	void createFormGroupInscrire();

	QWidget *m_mainWidget;					//Main widget = main layout
	QVBoxLayout *m_mainLayout;
	QHBoxLayout *m_bottomLayout;

	QGroupBox *m_GroupBoxConnexion;		//GroupBoxes widgets contenant les layouts ci-dessus
	QGroupBox *m_GroupBoxInsription;

	QFormLayout *m_formLayoutConnexion;		//Layouts utilisés
	QFormLayout *m_formLayoutInscription;

	QMenuBar *m_menuBar;
	QMenu *m_fileMenu;
	QAction *m_connexionAction;
	QAction *m_inscriptionAction;

	QLabel* m_labelUsername;
	QLabel* m_labelMDP;
	QLabel* m_labelConfirmMDP;
	QLabel* m_labelInscrire;
	QLabel *m_labelDescriptionInscription;
	QLabel *m_labelImage;

	QLineEdit* m_lineEditUsername;
	QLineEdit* m_lineEditUsernameInscrire;	//username inscription
	QLineEdit* m_lineEditMDP;
	QLineEdit* m_lineEditMDPInscrire;		//mdp inscription
	QLineEdit* m_lineEditConfirmMDP;		//confirmation mdp inscription
	QTextEdit *m_textEditDescriptionInscription;
	QPushButton* m_btnConnexion;
	QPushButton* m_btnQuitter;
	QPushButton* m_btnInscrire;
	QPushButton *m_btnChoisirImage;

	CommunicationFPGA *ptr_port;
};

class SecondWindow : public QMainWindow
{
	Q_OBJECT

public:
	SecondWindow(CommunicationFPGA &port, QWidget *parent = 0);
	~SecondWindow();

	//void setPartner(QWidget *partner);
	
private slots:
	void popUpAboutApp();
	void popUpAboutMe();
	void deconnexionPopUp();
	void openThirdWindow();
	void dislikeUser();
	void likeUser();
	void superlikeUser();

private:
	QWidget *m_premiereFenetre;	//La premiere fenetre

	void createMenu2();
	void createGroupBoxImage();
	void createGroupBoxAppreciation();
	void calibrate();

	void loopReadPhoneme(CommunicationFPGA &port, CustomSoundSignature &newSignature);

	bool isReading = false;
	bool breakReading = false;

	QWidget *m_secondWidget;

	QVBoxLayout *m_secondMainLayout;
	QMenuBar *m_menuBar2;
	QMenu *m_optionMenu;
	QMenu *m_helpMenu;
	QAction *m_parametreCompte;
	QAction *m_aboutAppAction;
	QAction *m_aboutMeAction;
	QAction *m_calibrateAction;
	
	QGroupBox *m_groupBoxImage;
	QVBoxLayout *m_vLayoutImage;
	QLabel *m_imageLabel;
	QLabel *m_nomAgeLabel;

	QLabel *m_labelDescription;
	QLineEdit *m_lineEditDescription;
	QTextEdit * m_textEditDescription;

	QGroupBox *m_groupBoxAppreciation;
	QGridLayout *m_gridLayoutAppreciation;
	QPushButton *m_btnDislike;
	QPushButton *m_btnLike;
	QPushButton *m_btnSuperlike;

	QHBoxLayout *m_bottomLayout;
	QPushButton *m_btnQuit;

	CommunicationFPGA *ptr_port;
};

class ThirdWindow : public QMainWindow
{
	Q_OBJECT

public:
	ThirdWindow(QWidget *parent = 0);
	~ThirdWindow();

private slots:
	void savePopUp();
	void cancelPopUp();
	void modifPhotoProfil();

private:
	void createGroupBoxCompte();

	QWidget *m_thirdWidget;

	QVBoxLayout *m_thirdMainLayout;
	QGroupBox *m_groupBoxCompte;
	QFormLayout *m_formLayoutCompte;
	QLabel *m_labelUsernameCpt, *m_labelMDPCpt, *m_labelConfirmMDPCpt, *m_labelDescriptionCpt, *m_labelImageCpt;
	QLineEdit *m_lineEditUsernameCpt, *m_lineEditMDPCpt, *m_lineEditConfirmMDPCpt;
	QTextEdit *m_textLineDescription;
	QPushButton *m_btnPhotoProfil;

	QHBoxLayout *m_bottomLayout;
	QPushButton *m_btnSave;
	QPushButton *m_btnCancel;
	

};
#endif // MAINWINDOW_H