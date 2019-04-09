#include "TinderGUI.h"
#include <qdebug.h>
#include <qrect.h>
#include <qdesktopwidget.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#include "calibrationWindow.h"

QString username;		//Utilisateur actuel VARIABLE GLOBALE

//Git test
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	createMenu();
	createGroupBoxConnexion();
	createFormGroupInscrire();
	
	m_mainWidget = new QWidget(this); 

	m_btnQuitter = new QPushButton(tr("&Quitter"));

	connect(m_btnQuitter, SIGNAL(clicked()), this, SLOT(popUp()));

	m_bottomLayout = new QHBoxLayout;
	m_bottomLayout->addStretch();
	m_bottomLayout->addWidget(m_btnQuitter);

	m_mainLayout = new QVBoxLayout(m_mainWidget);
	m_mainLayout->addWidget(m_GroupBoxConnexion);
	m_mainLayout->addWidget(m_GroupBoxInsription);
	m_mainLayout->addLayout(m_bottomLayout);

	m_mainLayout->setSpacing(35);		//Espace entre les groupbox du mainLayout
	setCentralWidget(m_mainWidget);
	//setFixedSize(900,800);				//MIEUX DE PAS METTRE DE FIXED SIZE CÔTÉ ERGONOMIE OU PAS GRAVE ????????????????
	setWindowTitle("Tinder.net\Connexion");
}

MainWindow::~MainWindow()
{
	//Destructeur de la première window
}

//void MainWindow::setPartner(QWidget * partner)
//{
//	m_deuxiemeFenetre = partner;
//}

void MainWindow::createMenu()
{
	m_menuBar = new QMenuBar;

	m_fileMenu = menuBar()->addMenu(tr("&File"));
	m_exitAction = m_fileMenu->addAction(tr("Quitter"));

	connect(m_exitAction, SIGNAL(triggered()), this, SLOT(popUp()));
}

void MainWindow::createGroupBoxConnexion()
{
	m_GroupBoxConnexion = new QGroupBox(tr("Deja un compte? Connecte toi!"));
	m_formLayoutConnexion = new QFormLayout;
	
	m_labelUsername = new QLabel("Username: ", this);
	m_lineEditUsername = new QLineEdit(this);
	m_labelMDP = new QLabel("Mot de passe: ", this);
	m_lineEditMDP = new QLineEdit(this);
	m_lineEditMDP->setEchoMode(QLineEdit::Password);
	m_btnConnexion = new QPushButton("Connexion", this);

	m_formLayoutConnexion->addRow(m_labelUsername, m_lineEditUsername);
	m_formLayoutConnexion->addRow(m_labelMDP, m_lineEditMDP);
	m_formLayoutConnexion->addRow(NULL, m_btnConnexion);

	connect(m_btnConnexion, SIGNAL(clicked(bool)), this, SLOT(confirmConnexion()));	//Bouton connexion qui ouvre la deuxieme window ???????????

	m_GroupBoxConnexion->setLayout(m_formLayoutConnexion);
}

void MainWindow::createFormGroupInscrire()
{
	m_GroupBoxInsription = new QGroupBox(tr("Pas de compte? Inscrit toi!"));
	m_formLayoutInscription = new QFormLayout;

	m_labelUsername = new QLabel("Username :", this);
	m_lineEditUsernameInscrire = new QLineEdit(this);
	m_labelMDP = new QLabel("Mot de passe: ", this);
	m_lineEditMDPInscrire = new QLineEdit(this);
	m_lineEditMDPInscrire->setEchoMode(QLineEdit::Password);

	m_labelConfirmMDP = new QLabel("Confirmation mot de passe: ", this);
	m_lineEditConfirmMDP = new QLineEdit(this);
	m_lineEditConfirmMDP->setEchoMode(QLineEdit::Password);

	m_btnInscrire = new QPushButton("S'inscrire", this);

	m_formLayoutInscription->addRow(m_labelUsername, m_lineEditUsernameInscrire);
	m_formLayoutInscription->addRow(m_labelMDP, m_lineEditMDPInscrire);
	m_formLayoutInscription->addRow(m_labelConfirmMDP, m_lineEditConfirmMDP);
	m_formLayoutInscription->addRow(NULL, m_btnInscrire);

	connect(m_btnInscrire, SIGNAL(clicked(bool)), this, SLOT(confirmInscription()));

	m_GroupBoxInsription->setLayout(m_formLayoutInscription);
}
void MainWindow::popUp()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "ATTENTION!", "Voulez-vous vraiment quitter?", QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		close();
	}
}

void MainWindow::confirmConnexion()
{
	QString str = m_lineEditUsername->text(), str1 = m_lineEditMDP->text();

	if (str != NULL && str1 != NULL) {
		username = m_lineEditUsername->text();
		openSecondWindow();
	}
	else QMessageBox::warning(this, "ATTENTION!", "Veuillez entrez votre nom d'utilisateur ET votre mot de passe...", QMessageBox::Ok);
	m_lineEditUsername->setFocus();
}

void MainWindow::confirmInscription()
{
	if (m_lineEditUsernameInscrire->text()==NULL) {
		QMessageBox::information(this, "ATTENTION!", "Vous devez choisir un nom d'utilisateur!", QMessageBox::Ok);
		m_lineEditUsernameInscrire->setFocus();
	}

	else if (m_lineEditMDPInscrire->text() == NULL) {
		QMessageBox::information(this, "ATTENTION!", "Vous devez entrez un mot de passe!", QMessageBox::Ok);
		m_lineEditMDPInscrire->setFocus();
	}
	else if (m_lineEditConfirmMDP->text() == NULL) {
		QMessageBox::information(this, "ATTENTION!", "Vous n'avez pas confirmer votre mot de passe!", QMessageBox::Ok);
		m_lineEditConfirmMDP->setFocus();
	}

	else if (m_lineEditMDPInscrire->text() != m_lineEditConfirmMDP->text()) {
		QMessageBox::information(this, "ATTENTION!", "Vos deux mots de passe ne sont pas identiques, veuillez verifier votre entree.", QMessageBox::Ok);
		m_lineEditMDPInscrire->clear();
		m_lineEditConfirmMDP->clear();
		m_lineEditMDPInscrire->setFocus();
	}

	else {
		username = m_lineEditUsernameInscrire->text();
		openSecondWindow();
	}
}

void MainWindow::openSecondWindow()
{	
	//QWidget *w2 = new QWidget;
	//w2 = new SecondWindow();

	QWidget *w2 = new QWidget;
	w2 = new SecondWindow();	
	w2->show();

	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	int x = (screenGeometry.width() - w2->width())/2;
	int y = (screenGeometry.height() - w2->height()) / 2;
	w2->move(x,y);

	//MainWindow::~MainWindow();				//QUAND ON CLOSE UNE WINDOW EST-ELLE DÉTRUITE ????????????
	close();
}

//DEUXIEME FENETRE----------------------------------------------------------------------------------------------------------------------------

SecondWindow::SecondWindow(QWidget *parent)
	: QMainWindow(parent)
{	
	m_btnQuit = new QPushButton(tr("&Deconnexion"));
	connect(m_btnQuit, SIGNAL(clicked()), this, SLOT(deconnexionPopUp()));

	m_bottomLayout = new QHBoxLayout;
	m_bottomLayout->addStretch();
	m_bottomLayout->addWidget(m_btnQuit);

	m_secondWidget = new QWidget(this);
	
	m_secondMainLayout = new QVBoxLayout(m_secondWidget);		//faut que mainLayout hérite de ma fenetre princ ??? 
	createMenu2();
	createGroupBoxInfo();
	createGroupBoxImage();
	createGroupBoxAppreciation();

	m_secondMainLayout->addWidget(m_groupBoxInfo);
	m_secondMainLayout->addWidget(m_groupBoxImage);
	m_secondMainLayout->addWidget(m_groupBoxAppreciation);
	m_secondMainLayout->addLayout(m_bottomLayout);

	m_secondMainLayout->setSpacing(50);		//Espace entre les groupbox du secondMainLayout
	setCentralWidget(m_secondWidget);	
}

SecondWindow::~SecondWindow()
{
	//Destructeur de la deuxiemeWindow (w2)
}

//void SecondWindow::setPartner(QWidget * partner)
//{
//	m_premiereFenetre = partner;
//}

void SecondWindow::createMenu2()
{
	m_menuBar2 = new QMenuBar;

	m_fileMenu = menuBar()->addMenu(tr("&Help"));
	m_aboutAppAction = m_fileMenu->addAction(tr("About Tinder"));
	m_aboutMeAction = m_fileMenu->addAction(tr("About Me"));

	m_optionMenu = menuBar()->addMenu("&Options");
	m_calibrateAction = m_optionMenu->addAction("Calibrer");

	connect(m_aboutAppAction, SIGNAL(triggered()), this, SLOT(popUpAboutApp()));
	connect(m_aboutMeAction, SIGNAL(triggered()), this, SLOT(popUpAboutMe()));
	connect(m_calibrateAction, SIGNAL(triggered()), this, SLOT(calibrate()));
}

void SecondWindow::createGroupBoxInfo()
{
	m_groupBoxInfo = new QGroupBox(tr("Compte"));
	m_gridLayoutInfo = new QGridLayout;
	m_labelTextInfo = new QLabel("Connecte en tant que: ", this);
	m_lineEditUsernameInfo = new QLineEdit(this);
	m_btnParametre = new QPushButton("Parametres", this);
	m_lineEditUsernameInfo->setText(username);
	m_lineEditUsernameInfo->setReadOnly(true);

	m_gridLayoutInfo->addWidget(m_labelTextInfo, 1, 1,1 ,1);
	m_gridLayoutInfo->addWidget(m_lineEditUsernameInfo, 1, 2, 1, 1);
	m_gridLayoutInfo->addWidget(m_btnParametre, 2, 1, 1, 2);
	
	connect(m_btnParametre, SIGNAL(clicked(bool)), this, SLOT(openThirdWindow()));	//Bouton connexion qui ouvre la thridWindow

	m_groupBoxInfo->setLayout(m_gridLayoutInfo);
}

void SecondWindow::createGroupBoxImage()
{
	m_groupBoxImage = new QGroupBox(tr("Utilisateur en attente de jugement: "));
	m_vLayoutImage = new QVBoxLayout;
	m_imageLabel = new QLabel;
	m_nomAgeLabel = new QLabel("Nom de l'usager", this);
	m_textEditDescription = new QTextEdit("Voici une description ecrite par la personne affichee ci-dessus blablaakifmafsfdsjfnjfsfjsdfsmkfsfsdfdsfs", this);
	m_textEditDescription->setReadOnly(true);
	QFont f("Arial", 14, QFont::Bold);
	m_nomAgeLabel->setFont(f);
	QPixmap pixMap("./PhotoProfil/profilePicture.png");

	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	int h = screenGeometry.height();
	int newImageSize = (int)(h * 0.34);

	QPixmap scaledPix = pixMap.scaled(QSize(newImageSize, newImageSize));
	m_imageLabel->setPixmap(scaledPix);
	//m_imageLabel->setScaledContents(true);

	m_vLayoutImage->addWidget(m_imageLabel);
	m_vLayoutImage->addWidget(m_nomAgeLabel);
	m_vLayoutImage->addWidget(m_textEditDescription);
	m_vLayoutImage->setAlignment(Qt::AlignCenter);

	m_groupBoxImage->setLayout(m_vLayoutImage);
}

void SecondWindow::createGroupBoxAppreciation()
{
	m_groupBoxAppreciation = new QGroupBox(tr("Votre appreciation: "));
	m_gridLayoutAppreciation = new QGridLayout;
	m_btnDislike = new QPushButton("Dislike", this);
	m_btnLike = new QPushButton("Like", this);
	m_btnSuperlike = new QPushButton("Superlike", this);

	m_gridLayoutAppreciation->addWidget(m_btnDislike,4,1,1,1);
	m_gridLayoutAppreciation->addWidget(m_btnLike, 4, 2, 1, 1);
	m_gridLayoutAppreciation->addWidget(m_btnSuperlike, 4, 3, 1, 1);

	m_groupBoxAppreciation->setLayout(m_gridLayoutAppreciation);
}

void SecondWindow::popUpAboutApp()
{
	QMessageBox::information(this, "A propos de Tinder...", "Tinder est une application de reseautage social permettant batir une relation amoureuse assurement solide."
							"Cette application defile des profils d'utilisateurs inscrits et permet a l'utilisateur connecte d'apprecier ou non la personne"
							"affichee selon plusieurs parametres (son nom, son age, sa photo de profil, sa description, etc.)");
}

void SecondWindow::popUpAboutMe()
{
	QMessageBox::information(this, "A propos de moi...", "Voici des informations aleatoires sur l'utilisateur presentement connecte (statistiques base de donnees, etc...)");
}

void SecondWindow::calibrate()
{
	QWidget *calib = new QWidget;
	calib = new calibWindow;

	calib->setMinimumSize(400, 200);
	calib->show();
}

void SecondWindow::deconnexionPopUp()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "ATTENTION!", "Voulez-vous vraiment vous deconnecter?", QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		QWidget *w1 = new QWidget;
		w1 = new MainWindow();

		w1->show();
		//SecondWindow::~SecondWindow();	//Détruit la deuixieme fenetre
		close();
	}
}

void SecondWindow::openThirdWindow()
{
	QWidget *w3 = new QWidget;
	w3 = new ThirdWindow();
	w3->setWindowTitle("Parametre du compte");
	w3->show();
}

//TROISIEME FENETRE (MODIF_PARAMETRE COMPTE)
ThirdWindow::ThirdWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_btnSave = new QPushButton(tr("&Sauvegarder"));
	m_btnCancel = new QPushButton(tr("&Cancel"));

	connect(m_btnSave, SIGNAL(clicked()), this, SLOT(savePopUp()));
	connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(cancelPopUp()));

	m_bottomLayout = new QHBoxLayout;
	m_bottomLayout->addStretch();
	m_bottomLayout->setSpacing(15);
	m_bottomLayout->addWidget(m_btnSave);
	m_bottomLayout->addWidget(m_btnCancel);
	
	m_thirdWidget = new QWidget(this);

	m_thirdMainLayout = new QVBoxLayout(m_thirdWidget);		//faut que mainLayout hérite de ma fenetre princ ??? 
	//createMenu2();
	createGroupBoxCompte();

	setWindowModality(Qt::ApplicationModal);

	m_thirdMainLayout->addWidget(m_groupBoxCompte);
	m_thirdMainLayout->addLayout(m_bottomLayout);

	m_thirdMainLayout->setSpacing(50);		//Espace entre les groupbox du secondMainLayout
	setCentralWidget(m_thirdWidget);
}

ThirdWindow::~ThirdWindow()
{

}

void ThirdWindow::createGroupBoxCompte()
{
	m_groupBoxCompte = new QGroupBox(tr("Veuillez entrez les modifications de compte desirees: "));
	m_formLayoutCompte = new QFormLayout;

	m_labelUsernameCpt = new QLabel("Nouveau nom d'utilisateur: ", this);
	m_labelMDPCpt = new QLabel("Nouveau mot de passe: ",this);
	m_labelConfirmMDPCpt = new QLabel("Confirmation du nouveau mot de passe: ", this);
	m_labelDescriptionCpt = new QLabel("Nouvelle description: ", this);
	m_labelImageCpt = new QLabel("Nouvelle photo de profil: ", this);
	
	m_lineEditUsernameCpt = new QLineEdit(this);
	m_lineEditMDPCpt = new QLineEdit(this);
	m_lineEditConfirmMDPCpt = new QLineEdit(this);
	m_lineEditMDPCpt->setEchoMode(QLineEdit::Password);
	m_lineEditConfirmMDPCpt->setEchoMode(QLineEdit::Password);

	m_textLineDescription = new QTextEdit("Voici ma description personnelle !",this);
	m_btnPhotoProfil = new QPushButton("Selectionner une image", this);

	m_formLayoutCompte->addRow(m_labelUsernameCpt, m_lineEditUsernameCpt);
	m_formLayoutCompte->addRow(m_labelMDPCpt, m_lineEditMDPCpt);
	m_formLayoutCompte->addRow(m_labelConfirmMDPCpt, m_lineEditConfirmMDPCpt);
	m_formLayoutCompte->addRow(m_labelDescriptionCpt, m_textLineDescription);
	m_formLayoutCompte->addRow(m_labelImageCpt, m_btnPhotoProfil);

	m_groupBoxCompte->setLayout(m_formLayoutCompte);
}

void ThirdWindow::savePopUp()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "ATTENTION!", "Voulez-vous vraiment sauvegarder les modifications apportes ?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		close();
	}
}
void ThirdWindow::cancelPopUp()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "ATTENTION!", "Voulez-vous vraiment abandonner vos modifications ?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		close();
	}
}