#include "TinderGUI.h"
#include <qdebug.h>
#include <qrect.h>
#include <qdesktopwidget.h>
#include <qapplication.h>

QString username;		//Utilisateur + description actuel - VARIABLE GLOBALE
QString description = "ceci est la description de l'utilisateur actuellement connecte. (modifiable)";

MainWindow::MainWindow(CommunicationFPGA &port)
{
	createMenu();
	createGroupBoxConnexion();
	createFormGroupInscrire();

	m_mainWidget = new QWidget(this);

	m_btnQuitter = new QPushButton(tr("&Quitter"));

	connect(m_btnQuitter, SIGNAL(clicked()), this, SLOT(popUpQuitter()));		//Confirmation que l'utilisateur veut réellement quitter

	m_bottomLayout = new QHBoxLayout;
	m_bottomLayout->addStretch();				//"Stretch tout à droite"
	m_bottomLayout->addWidget(m_btnQuitter);

	m_mainLayout = new QVBoxLayout(m_mainWidget);
	m_mainLayout->addWidget(m_GroupBoxConnexion);
	m_mainLayout->addWidget(m_GroupBoxInsription);
	m_mainLayout->addLayout(m_bottomLayout);

	//m_mainLayout->setSpacing(35);		//Espace entre les groupbox du mainLayout
	setCentralWidget(m_mainWidget);
	//setFixedSize(900,800);				//MIEUX DE PAS METTRE DE FIXED SIZE CÔTÉ ERGONOMIE OU PAS GRAVE ????????????????
	setWindowTitle("Tinder.net - Connexion");

	*ptr_port = port;
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

	m_fileMenu = menuBar()->addMenu(tr("&Aide"));
	m_connexionAction = m_fileMenu->addAction(tr("Connexion"));
	m_inscriptionAction = m_fileMenu->addAction(tr("Inscription"));

	connect(m_connexionAction, SIGNAL(triggered()), this, SLOT(popUpConnexion()));
	connect(m_inscriptionAction, SIGNAL(triggered()), this, SLOT(popUpInscription()));
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

	connect(m_btnConnexion, SIGNAL(clicked(bool)), this, SLOT(confirmConnexion()));	//Bouton connexion qui ouvre la deuxieme window

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

	m_labelDescriptionInscription = new QLabel("Description: ", this);
	m_textEditDescriptionInscription = new QTextEdit(this);

	m_labelImage = new QLabel("Choisir une image de profil: ", this);
	m_btnChoisirImage = new QPushButton("Importer une photo", this);

	m_btnInscrire = new QPushButton("S'inscrire", this);

	m_formLayoutInscription->addRow(m_labelUsername, m_lineEditUsernameInscrire);
	m_formLayoutInscription->addRow(m_labelMDP, m_lineEditMDPInscrire);
	m_formLayoutInscription->addRow(m_labelConfirmMDP, m_lineEditConfirmMDP);
	m_formLayoutInscription->addRow(m_labelDescriptionInscription, m_textEditDescriptionInscription);
	m_formLayoutInscription->addRow(m_labelImage, m_btnChoisirImage);
	m_formLayoutInscription->addRow(NULL, m_btnInscrire);

	connect(m_btnInscrire, SIGNAL(clicked(bool)), this, SLOT(confirmInscription()));		//Verification des entrées et inscription de l'utilisateur
	connect(m_btnChoisirImage, SIGNAL(clicked(bool)), this, SLOT(browseImage()));		//Browse une image de profil

	m_GroupBoxInsription->setLayout(m_formLayoutInscription);
}

void MainWindow::popUpConnexion()
{
	QMessageBox::information(this, "Info sur la connexion", "Si vous posseder un compte, il vous suffit d'entrez votre nom d'utilisateur et votre mot de passe, sinon, entrez vos informations dans l'onglet du bas, inscription", QMessageBox::Ok);
}

void MainWindow::popUpInscription()
{
	QMessageBox::information(this, "Info sur l'inscription", "Si vous ne posseder pas de compte, remplissez TOUTES les cases de l'onglet inscription du bas, assurez vous d'entrez une confirmation valide du mot de passe!", QMessageBox::Ok);
}

void MainWindow::browseImage()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), "/path/to/file/", tr("Png files (*.png)"));
	if (fileNames.isEmpty()) {
		QString path = "path par default!";		//path par défault si l'utilisateur cancel le "browsing"
	}
	else {
		QString path;
		for (int i = 0; i < fileNames.length(); i++) {

			path += fileNames[i];			//path de l'image choisie
		}
	}
}

void MainWindow::popUpQuitter()
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
	if (m_lineEditUsernameInscrire->text() == NULL) {
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
		description = m_textEditDescriptionInscription->toPlainText();
		openSecondWindow();
	}
}

void MainWindow::openSecondWindow()
{
	QWidget *w2 = new QWidget;
	w2 = new SecondWindow(*ptr_port);

	w2->setWindowTitle("Bienvenue " + username);
	w2->show();

	QRect screenGeometry = QApplication::desktop()->screenGeometry();		//Permet de centrer la deuxieme fenetre (w2) au centre de l'écran 
	int x = (screenGeometry.width() - w2->width()) / 2;
	int y = (screenGeometry.height() - w2->height()) / 2;
	w2->move(x, y);

	//Insérer boucle de lecture ici

	close();
}

//DEUXIEME FENETRE----------------------------------------------------------------------------------------------------------------------------

SecondWindow::SecondWindow(CommunicationFPGA &port, QWidget *parent)
	: QMainWindow(parent)
{
	m_btnQuit = new QPushButton(tr("&Deconnexion"));
	connect(m_btnQuit, SIGNAL(clicked()), this, SLOT(deconnexionPopUp()));			//Confirmation que l'utilisateur veut vraiment quitter

	m_bottomLayout = new QHBoxLayout;
	m_bottomLayout->addStretch();
	m_bottomLayout->addWidget(m_btnQuit);

	m_secondWidget = new QWidget(this);

	m_secondMainLayout = new QVBoxLayout(m_secondWidget);
	createMenu2();
	createGroupBoxImage();
	createGroupBoxAppreciation();

	m_secondMainLayout->addWidget(m_groupBoxImage);
	m_secondMainLayout->addWidget(m_groupBoxAppreciation);
	m_secondMainLayout->addLayout(m_bottomLayout);

	setCentralWidget(m_secondWidget);

	*ptr_port = port;
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

	m_optionMenu = menuBar()->addMenu(tr("&Option"));
	m_parametreCompte = m_optionMenu->addAction(tr("Parametre du compte"));

	m_helpMenu = menuBar()->addMenu(tr("&Aide"));
	m_aboutAppAction = m_helpMenu->addAction(tr("A propos de Tinder"));
	m_aboutMeAction = m_helpMenu->addAction(tr("A propos de moi"));
	m_calibrateAction = m_optionMenu->addAction("Calibrer");

	connect(m_parametreCompte, SIGNAL(triggered()), this, SLOT(openThirdWindow()));			//Bouton dans la barre de menu qui ouvre la troisieme fenetre
	connect(m_aboutAppAction, SIGNAL(triggered()), this, SLOT(popUpAboutApp()));			//Les deux boutons du menu d'aide qui ouvre des MessageBox
	connect(m_aboutMeAction, SIGNAL(triggered()), this, SLOT(popUpAboutMe()));
	connect(m_calibrateAction, SIGNAL(triggered()), this, SLOT(calibrate()));
}

void SecondWindow::createGroupBoxImage()
{
	m_groupBoxImage = new QGroupBox(tr("Utilisateur en attente de jugement: "));
	m_vLayoutImage = new QVBoxLayout;
	m_imageLabel = new QLabel;
	m_nomAgeLabel = new QLabel("Nom de l'usager", this);
	m_textEditDescription = new QTextEdit("Ceci est la description de l'utilisateur presentement sous evaluation!", this);
	m_textEditDescription->setStyleSheet(QString("background-color: lightgray"));
	m_textEditDescription->setReadOnly(true);
	QFont f("Arial", 14, QFont::Bold);
	m_nomAgeLabel->setFont(f);
	QPixmap pixMap("./PhotoProfil/profilePicture.png");

	QRect screenGeometry = QApplication::desktop()->screenGeometry();			//Permet de trouver la bonne dimension pour l'image selon la hauteur de l'écran
	int h = screenGeometry.height();
	int newImageSize = (int)(h * 0.30);

	QPixmap scaledPix = pixMap.scaled(QSize(newImageSize, newImageSize));		//permet de redimensionner l'image

	//QPixmap scaledPix = pixMap.scaled(QSize(750, 750));
	m_imageLabel->setPixmap(scaledPix);				//Attribution de l'image (pixmap) au label m_imageLabel

	m_vLayoutImage->addWidget(m_imageLabel);
	m_vLayoutImage->addWidget(m_nomAgeLabel);
	m_vLayoutImage->addWidget(m_textEditDescription);
	m_vLayoutImage->setAlignment(Qt::AlignCenter);		//Allignement de l'image au centre (son layout en fait...)

	m_groupBoxImage->setLayout(m_vLayoutImage);
}

void SecondWindow::createGroupBoxAppreciation()
{
	m_groupBoxAppreciation = new QGroupBox(tr("Votre appreciation: "));
	m_gridLayoutAppreciation = new QGridLayout;
	m_btnDislike = new QPushButton("Dislike", this);
	m_btnLike = new QPushButton("Like", this);
	m_btnSuperlike = new QPushButton("Superlike", this);

	m_gridLayoutAppreciation->addWidget(m_btnDislike, 4, 1, 1, 1);
	m_gridLayoutAppreciation->addWidget(m_btnLike, 4, 2, 1, 1);
	m_gridLayoutAppreciation->addWidget(m_btnSuperlike, 4, 3, 1, 1);

	m_groupBoxAppreciation->setLayout(m_gridLayoutAppreciation);
}

void SecondWindow::calibrate()
{
	QWidget *calib = new QWidget;
	calib = new calibWindow(*ptr_port);

	calib->setMinimumSize(400, 200);
	calib->show();
}

void SecondWindow::popUpAboutApp()
{
	QMessageBox::information(this, "A propos de Tinder...", "Tinder est une application de reseautage social permettant batir une relation amoureuse assurement solide. "
		"Cette application defile des profils d'utilisateurs inscrits et permet a l'utilisateur connecte d'apprecier ou non la personne "
		"affichee selon plusieurs parametres (son nom, son age, sa photo de profil, sa description, etc.)");
}

void SecondWindow::popUpAboutMe()
{
	QMessageBox::information(this, "A propos de moi...", "Voici des informations aleatoires sur l'utilisateur presentement connecte (statistiques base de donnees, etc...)");
}

void SecondWindow::deconnexionPopUp()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "ATTENTION!", "Voulez-vous vraiment vous deconnecter?", QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		QWidget *w1 = new QWidget;			//Recréation et réaffichage de la premiere fenetre avec le constructeur de MainWindow
		w1 = new MainWindow(*ptr_port);

		w1->show();
		close();			//Ferme this, étant la deuxieme fenetre
	}
}

void SecondWindow::openThirdWindow()
{
	QWidget *w3 = new QWidget;				//Création et affichage de la troisieme fenetre avec le constructeur de ThirdWindow
	w3 = new ThirdWindow();
	w3->setWindowTitle("Parametre du compte");
	w3->show();
}

//TROISIEME FENETRE (MODIF_PARAMETRE COMPTE)
ThirdWindow::ThirdWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowModality(Qt::ApplicationModal);

	m_btnSave = new QPushButton(tr("&Sauvegarder"));
	m_btnCancel = new QPushButton(tr("&Annuler"));

	connect(m_btnSave, SIGNAL(clicked()), this, SLOT(savePopUp()));			//Confirmation de la sauvegarde lorsque l'utilisateur appuie sur le bouton save de la fenetre 3 de modifications des parametres
	connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(cancelPopUp()));		//Confirmation de l'annulation lorsque l'utilisateur appuie sur le bouton cancel de la fenetre 3 de modifications des parametres

	m_bottomLayout = new QHBoxLayout;
	m_bottomLayout->addStretch();

	m_bottomLayout->addWidget(m_btnSave);
	m_bottomLayout->addWidget(m_btnCancel);

	m_thirdWidget = new QWidget(this);

	m_thirdMainLayout = new QVBoxLayout(m_thirdWidget);		//faut que mainLayout hérite de ma fenetre princ ??? 
	//createMenu2();
	createGroupBoxCompte();


	m_thirdMainLayout->addWidget(m_groupBoxCompte);
	m_thirdMainLayout->addLayout(m_bottomLayout);

	setCentralWidget(m_thirdWidget);
}

ThirdWindow::~ThirdWindow()
{

}

void ThirdWindow::createGroupBoxCompte()
{
	m_groupBoxCompte = new QGroupBox(tr("Veuillez entrez les modifications de compte desirees: "));
	m_formLayoutCompte = new QFormLayout;

	m_labelUsernameCpt = new QLabel("Nom d'utilisateur actuel (non modifiable): ", this);
	m_labelMDPCpt = new QLabel("Nouveau mot de passe: ", this);
	m_labelConfirmMDPCpt = new QLabel("Confirmation du nouveau mot de passe: ", this);
	m_labelDescriptionCpt = new QLabel("Modifie la description actuelle: ", this);
	m_labelImageCpt = new QLabel("Nouvelle photo de profil: ", this);

	m_lineEditUsernameCpt = new QLineEdit(username, this);
	m_lineEditUsernameCpt->setStyleSheet(QString("background-color: lightgray"));
	m_lineEditUsernameCpt->setReadOnly(true);
	m_lineEditMDPCpt = new QLineEdit(this);
	m_lineEditConfirmMDPCpt = new QLineEdit(this);
	m_lineEditMDPCpt->setEchoMode(QLineEdit::Password);
	m_lineEditConfirmMDPCpt->setEchoMode(QLineEdit::Password);

	m_textLineDescription = new QTextEdit(description, this);
	m_btnPhotoProfil = new QPushButton("Selectionner une image", this);

	m_formLayoutCompte->addRow(m_labelUsernameCpt, m_lineEditUsernameCpt);
	m_formLayoutCompte->addRow(m_labelMDPCpt, m_lineEditMDPCpt);
	m_formLayoutCompte->addRow(m_labelConfirmMDPCpt, m_lineEditConfirmMDPCpt);
	m_formLayoutCompte->addRow(m_labelDescriptionCpt, m_textLineDescription);
	m_formLayoutCompte->addRow(m_labelImageCpt, m_btnPhotoProfil);

	connect(m_btnPhotoProfil, SIGNAL(clicked()), this, SLOT(modifPhotoProfil()));		//modification du path de la photo de profil

	m_groupBoxCompte->setLayout(m_formLayoutCompte);
}

void ThirdWindow::modifPhotoProfil() {

	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), "/path/to/file/", tr("Png files (*.png)"));
	if (fileNames.isEmpty()) {
		//rien ne se passe, garde le même path qu'avant
	}
	else {
		QString path = "";	//vide le path
		for (int i = 0; i < fileNames.length(); i++) {

			path += fileNames[i];			//path est le path de l'image
		}
	}
}

void ThirdWindow::savePopUp()
{
	if (m_lineEditMDPCpt->text() == m_lineEditConfirmMDPCpt->text()) {

		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "ATTENTION!", "Voulez-vous vraiment sauvegarder les modifications apportees ?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			description = m_textLineDescription->toPlainText();
			close();
		}
	}
	else {
		QMessageBox::warning(this, "ATTENTION!", "Vos deux mots de passe ne sont pas identiques, veuillez verifier votre entree.", QMessageBox::Ok);
		m_lineEditMDPCpt->clear();
		m_lineEditConfirmMDPCpt->clear();
		m_lineEditMDPCpt->setFocus();
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