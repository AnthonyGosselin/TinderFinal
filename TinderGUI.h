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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	//void setPartner(QWidget *partner);

private slots:
	void popUp();
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
	QAction *m_exitAction;

	QLabel* m_labelUsername;
	QLabel* m_labelMDP;
	QLabel* m_labelConfirmMDP;
	QLabel* m_labelInscrire;

	QLineEdit* m_lineEditUsername;
	QLineEdit* m_lineEditUsernameInscrire;	//username inscription
	QLineEdit* m_lineEditMDP;
	QLineEdit* m_lineEditMDPInscrire;		//mdp inscription
	QLineEdit* m_lineEditConfirmMDP;		//confirmation mdp inscription
	QPushButton* m_btnConnexion;
	QPushButton* m_btnQuitter;
	QPushButton* m_btnInscrire;
};

class SecondWindow : public QMainWindow
{
	Q_OBJECT

public:
	SecondWindow(QWidget *parent = 0);
	~SecondWindow();

	//void setPartner(QWidget *partner);
	
private slots:
	void popUpAboutApp();
	void popUpAboutMe();
	void deconnexionPopUp();
	void openThirdWindow();
	void calibrate();

private:

	QWidget *m_premiereFenetre;	//La premiere fenetre

	void createMenu2();
	void createGroupBoxInfo();
	void createGroupBoxImage();
	void createGroupBoxAppreciation();

	QWidget *m_secondWidget;

	QVBoxLayout *m_secondMainLayout;
	QMenuBar *m_menuBar2;
	QMenu *m_fileMenu;
	QAction *m_aboutAppAction;
	QAction *m_aboutMeAction;
	QMenu *m_optionMenu;
	QAction *m_calibrateAction;

	QGroupBox *m_groupBoxInfo;
	QGridLayout *m_gridLayoutInfo;
	QLabel *m_labelTextInfo;
	QLineEdit *m_lineEditUsernameInfo;
	QPushButton * m_btnParametre;
	
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