#include "TinderGUI.h"
#include <qdebug.h>
#include <qrect.h>
#include <qdesktopwidget.h>
#include <qapplication.h>
#include <qdebug.h>
#include <string>
#include <regex>


//QString description = "Ceci est la description par default. Pour la modifier, clickez sur option, parametre compte.";
fluxy core;
QString path;
bool paramOpen = false;

MainWindow::MainWindow(CommunicationFPGA &port, QWidget *parent)
    : QMainWindow(parent)
{
	path = "";
	createMenu();
	createGroupBoxConnexion();
	createFormGroupInscrire();

	m_mainWidget = new QWidget;

	m_btnQuitter = new QPushButton(tr("&Quitter"));
	m_btnQuitter->setStyleSheet(QString("background-color: lightgreen"));

	connect(m_btnQuitter, SIGNAL(clicked()), this, SLOT(popUpQuitter()));		//Confirmation que l'utilisateur veut réellement quitter

	m_bottomLayout = new QHBoxLayout;
	m_bottomLayout->addStretch();				//"Stretch tout à droite"
	m_bottomLayout->addWidget(m_btnQuitter);

	QPixmap bkgnd("./PhotoProfil/tinder.png");
	bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);

	m_mainLayout = new QVBoxLayout(m_mainWidget);
	m_mainLayout->addWidget(m_GroupBoxConnexion);
	m_mainLayout->addWidget(m_GroupBoxInsription);
	m_mainLayout->addLayout(m_bottomLayout);

	//m_mainLayout->setSpacing(35);		//Espace entre les groupbox du mainLayout
	setCentralWidget(m_mainWidget);
	//setFixedSize(900,800);				//MIEUX DE PAS METTRE DE FIXED SIZE CÔTÉ ERGONOMIE OU PAS GRAVE ????????????????
	setWindowTitle("Tinder.net - Connexion");

	setWindowIcon(QIcon("./PhotoProfil/tinderLogo.png"));
	ptr_port = &port;
}

MainWindow::~MainWindow()
{
	//core.save();
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
	m_lineEditUsername->setStyleSheet(QString("background-color: pink"));

	m_labelMDP = new QLabel("Mot de passe: ", this);
	m_lineEditMDP = new QLineEdit(this);
	m_lineEditMDP->setStyleSheet(QString("background-color: pink"));

	m_lineEditMDP->setEchoMode(QLineEdit::Password);
	m_btnConnexion = new QPushButton("Connexion", this);
	m_btnConnexion->setStyleSheet(QString("background-color: lightgreen"));

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
	m_lineEditUsernameInscrire->setStyleSheet(QString("background-color: pink"));

	m_labelMDP = new QLabel("Mot de passe: ", this);
	m_lineEditMDPInscrire = new QLineEdit(this);
	m_lineEditMDPInscrire->setEchoMode(QLineEdit::Password);
	m_lineEditMDPInscrire->setStyleSheet(QString("background-color: pink"));

	m_labelConfirmMDP = new QLabel("Confirmation mot de passe: ", this);
	m_lineEditConfirmMDP = new QLineEdit(this);
	m_lineEditConfirmMDP->setEchoMode(QLineEdit::Password);
	m_lineEditConfirmMDP->setStyleSheet(QString("background-color: pink"));


	m_labelDescriptionInscription = new QLabel("Description: ", this);
	m_textEditDescriptionInscription = new QTextEdit(this);
	m_textEditDescriptionInscription->setStyleSheet(QString("background-color: pink"));


	m_labelImage = new QLabel("Choisir une image de profil: ", this);
	m_btnChoisirImage = new QPushButton("Importer une photo", this);
	m_btnChoisirImage->setStyleSheet(QString("background-color: lightgreen"));

	m_btnInscrire = new QPushButton("S'inscrire", this);
	m_btnInscrire->setStyleSheet(QString("background-color: lightgreen"));

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
		path = "./PhotoProfil/profilePicture.png";		//path par défault si l'utilisateur cancel le "browsing"
		qDebug() << path << endl;
	}
	else {
		for (int i = 0; i < fileNames.length(); i++) {

			path += fileNames[i];			//path est le path de l'image
		}
	}
}

void MainWindow::popUpQuitter()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "ATTENTION!", "Voulez-vous vraiment quitter?", QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		core.save();
		close();
	}
}

void MainWindow::confirmConnexion()
{
	QString str = m_lineEditUsername->text(), str1 = m_lineEditMDP->text();

	int usernamePasswordCheck = core.login(str.toStdString(), str1.toStdString());

	if (str != NULL && str1 != NULL && usernamePasswordCheck == 0) {
		//username = m_lineEditUsername->text();
		openSecondWindow();
	}
	else if (str == NULL || str1 == NULL) {
		QMessageBox::warning(this, "ATTENTION!", "Veuillez entrez votre nom d'utilisateur ET votre mot de passe...", QMessageBox::Ok);
		m_lineEditUsername->setFocus();
	}

	else if (usernamePasswordCheck == -1) {
		QMessageBox::warning(this, "ATTENTION!", "Cette combinaison d'utilisateur et de mot de passe n'est pas valide", QMessageBox::Ok);
		m_lineEditUsername->setFocus();
	}
	else if (usernamePasswordCheck == 1) {
		QMessageBox::warning(this, "ATTENTION!", "Il n'y a pas d'autre utilisateur dans la liste, revenez plus tard", QMessageBox::Ok);
		m_lineEditUsername->setFocus();
	}
}

void MainWindow::confirmInscription()
{
	QString strUsername = m_lineEditUsernameInscrire->text();
	QString strMDP = m_lineEditMDPInscrire->text();
	QString textDescription = m_textEditDescriptionInscription->toPlainText();
	int usernamePasswordCheck = core.addAccount(strUsername.toStdString(), strMDP.toStdString(), textDescription.toStdString());

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

	else if (usernamePasswordCheck == -1) {
		QMessageBox::information(this, "ATTENTION!", "Ce nom d'utilisateur est deja utilise... Veuillez en choisir un autre", QMessageBox::Ok);
		m_lineEditUsernameInscrire->setFocus();
	}
	else if (usernamePasswordCheck == 1) {
		QMessageBox::warning(this, "ATTENTION!", "Il n'y a pas d'autre utilisateur dans la liste, revenez plus tard", QMessageBox::Ok);
		m_lineEditUsername->setFocus();
	}

	else {
		if (path.isEmpty()) {
			path = "./PhotoProfil/profilePicture.png";		//SET LE PATH PAR DEFAULT 
		}
		//username = m_lineEditUsernameInscrire->text();
		core.setPath(path.toStdString());
		core.save();
		openSecondWindow();
	}
}

void MainWindow::openSecondWindow()
{
	QWidget *w2 = new QWidget;

	w2 = new SecondWindow(*ptr_port, this);
	w2->setWindowTitle("Bienvenue " + QString::fromStdString(core.getName_U()));
	//w2->show();

	QRect screenGeometry = QApplication::desktop()->screenGeometry();		//Permet de centrer la deuxieme fenetre (w2) au centre de l'écran 
	int x = (screenGeometry.width() - w2->width()) / 2;
	int y = (screenGeometry.height() - w2->height()) / 2;
	w2->move(x, y);

	close();
}

//DEUXIEME FENETRE----------------------------------------------------------------------------------------------------------------------------

SecondWindow::SecondWindow(CommunicationFPGA &port, QWidget *parent)
	: QMainWindow(parent)
{
	qDebug() << path << endl;

	m_btnQuit = new QPushButton(tr("&Deconnexion"));
	m_btnQuit->setStyleSheet(QString("background-color: lightgreen"));

	connect(m_btnQuit, SIGNAL(clicked()), this, SLOT(deconnexionPopUp()));			//Confirmation que l'utilisateur veut vraiment quitter

	m_bottomLayout = new QHBoxLayout;
	m_bottomLayout->addStretch();
	m_bottomLayout->addWidget(m_btnQuit);

	m_secondWidget = new QWidget(this);

	m_secondMainLayout = new QVBoxLayout(m_secondWidget);
	createMenu2();
	createGroupBoxImage();
	createGroupBoxAppreciation();

	QPixmap bkgnd("./PhotoProfil/tinder.png");
	bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);

	m_secondMainLayout->addWidget(m_groupBoxImage);
	m_secondMainLayout->addWidget(m_groupBoxAppreciation);
	m_secondMainLayout->addLayout(m_bottomLayout);

	setCentralWidget(m_secondWidget);	

	ptr_port = &port;

	this->show();

	// On brise s'il y avait une boucle avant
	if (isReading) {
		breakReading = true;
		while (breakReading)
			activeWait(10);
	}
	loadSignature();
	loopReadPhoneme(port);

}

SecondWindow::~SecondWindow()
{
	//Destructeur de la deuxiemeWindow (w2)
}

void SecondWindow::calibrate() {
	breakReading = true;

	QWidget *calib = new QWidget;
	calib = new calibWindow(*ptr_port, core);

	calib->setMinimumSize(400, 200);
	calib->show();
}

void SecondWindow::loadSignature() {

	PhonemeRef newPhonemeTab[NUM_PHONEMES];
	core.getFiltre(newPhonemeTab[0].referenceTab, newPhonemeTab[1].referenceTab, newPhonemeTab[2].referenceTab, newPhonemeTab[3].referenceTab);

	//On regroupe le tout dans une signature
	cout << "Loading signature...\n";
	CustomSoundSignature newSignature;
	for (int i = 0; i < NUM_PHONEMES; i++) {
		newSignature.phonemeRefTab[i] = newPhonemeTab[i];
		cout << "Phoneme #" << i << " ( ";
		for (int y = 0; y < NUM_FILTERS; y++) {
			cout << newPhonemeTab[i].referenceTab[y] << (y < NUM_FILTERS - 1 ? ", " : ")\n");
		}
	}

	curr_signature = newSignature;

}

//Boucle de lecture principale du programme
void SecondWindow::loopReadPhoneme(CommunicationFPGA &port) {
	isReading = true;
	int readsInRow = 0;
	int lastRead = -1;

	int readsByPhoneme[NUM_PHONEMES] = { 1, 0, 8, 0 };

	cout << "Signature reference: \n";
	for (int i = 0; i < NUM_PHONEMES; i++) {
		cout << "Phoneme #" << i << " ( ";
		for (int y = 0; y < NUM_FILTERS; y++) {
			cout << curr_signature.phonemeRefTab[i].referenceTab[y] << (y < NUM_FILTERS - 1 ? ", " : ")\n");
		}
	}

	while (!breakReading) {

		//if (!paramOpen) {
			int matchPhoneme = identifyPhoneme(curr_signature, readPhonemeFromPort(port));

			if (matchPhoneme < 0) {
				//cout << "No match\n";
				lastRead = matchPhoneme;
				readsInRow = 0;
			}
			else {
				cout << "Matched: " << matchPhoneme << endl;

				if (matchPhoneme == lastRead) {
					readsInRow++;
					cout << "Reads in a row: " << readsInRow << endl;

					if (readsInRow >= readsByPhoneme[matchPhoneme]) {
						cout << "STREAK MATCH: " << matchPhoneme << endl;

						//Appeler la fonction appropriée
						switch (matchPhoneme) {
						case 0:
							cout << "like user\n";
							likeUser();
							break;
						case 1:
							cout << "Dislike user\n";
							dislikeUser();
							break;
						case 2:
							cout << "Open the third window\n";
							openThirdWindow();
							break;
						case 3:
							cout << "Super like user\n";
							superlikeUser();
							break;
						}
					}
				}

				lastRead = matchPhoneme;

				//Pourrais tenter d'identifier le phoneme encore quelques fois ici pour valider (va falloir réduire le temps de lecture de phonème)
			}

		//}
		//else
			//cout << "Param is open\n";

	activeWait(50);

	}
	breakReading = false;
	//isReading = false;
}

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
	connect(m_calibrateAction, &QAction::triggered, this, &SecondWindow::calibrate);
}

void SecondWindow::createGroupBoxImage()
{
	m_groupBoxImage = new QGroupBox(tr("Utilisateur en attente de jugement: "));
	m_vLayoutImage = new QVBoxLayout;
	m_imageLabel = new QLabel;
	m_nomAgeLabel = new QLabel(QString::fromStdString(core.getName_P()), this);
	m_textEditDescription = new QTextEdit(QString::fromStdString(core.checkDescription_P()), this);
	m_textEditDescription->setStyleSheet(QString("background-color: lightgray"));
	m_textEditDescription->setReadOnly(true);
	QFont f("Arial", 14, QFont::Bold);
	m_nomAgeLabel->setFont(f);
	//QPixmap pixMap("./PhotoProfil/profilePicture.png");

	/*if (path == "") {
		core.setPath("./PhotoProfil/profilePicture.png");
	}*/
	QPixmap pixMap(QString::fromStdString(core.getPath_P()));

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
	m_btnDislike->setStyleSheet(QString("background-color: lightgreen"));

	m_btnLike = new QPushButton("Like", this);
	m_btnLike->setStyleSheet(QString("background-color: lightgreen"));

	m_btnSuperlike = new QPushButton("Superlike", this);
	m_btnSuperlike->setStyleSheet(QString("background-color: lightgreen"));


	m_gridLayoutAppreciation->addWidget(m_btnDislike, 4, 1, 1, 1);
	m_gridLayoutAppreciation->addWidget(m_btnLike, 4, 2, 1, 1);
	m_gridLayoutAppreciation->addWidget(m_btnSuperlike, 4, 3, 1, 1);

	connect(m_btnDislike, SIGNAL(clicked()), this, SLOT(dislikeUser()));			//Bouton dans la barre de menu qui ouvre la troisieme fenetre
	connect(m_btnLike, SIGNAL(clicked()), this, SLOT(likeUser()));
	connect(m_btnSuperlike, SIGNAL(clicked()), this, SLOT(superlikeUser()));

	m_groupBoxAppreciation->setLayout(m_gridLayoutAppreciation);
}

void SecondWindow::dislikeUser()
{
	if (core.dislike() == 1) {		//fin de la liste, il n'y a plus personne a apprecier
		QMessageBox::information(this, "ATTENTION!", "Il n'y a plus personne a apprecier", QMessageBox::Ok);
		noMoreJudgment();
		return;
	}

	m_nomAgeLabel->setText(QString::fromStdString(core.getName_P()));
	m_textEditDescription->setText(QString::fromStdString(core.checkDescription_P()));

	//Modification de l'image affichée
	QPixmap pixMap(QString::fromStdString(core.getPath_P()));

	QRect screenGeometry = QApplication::desktop()->screenGeometry();			//Permet de trouver la bonne dimension pour l'image selon la hauteur de l'écran
	int h = screenGeometry.height();
	int newImageSize = (int)(h * 0.30);

	QPixmap scaledPix = pixMap.scaled(QSize(newImageSize, newImageSize));		//permet de redimensionner l'image

	m_imageLabel->setPixmap(scaledPix);
}

void SecondWindow::likeUser()
{
	int retour = core.like();
	if (retour == 1) {			//fin de la liste, il n'y a plus personne a apprecier
		QMessageBox::information(this, "ATTENTION!", "Il n'y a plus personne a apprecier", QMessageBox::Ok);
		noMoreJudgment();
		return;
	}
	else if (retour == 2) {		//plus de user et que tu as un match
		QMessageBox::information(this, "WOW!", "VOUS AVEZ UN MATCH!!! Cependant, il n'y a plus d'utilisateur dans la liste... Revenez plus tard pour d'autres rencontres sensuelles. ;)", QMessageBox::Ok);
		noMoreJudgment();
		return;
	}

	else if (retour == 0) {
		QMessageBox::information(this, "WOW!", "VOUS AVEZ UN MATCH!!! Écrivez rapidement à l'utilisateur en question pour organiser une rencontre sensuelle ;)", QMessageBox::Ok);
	}

	m_nomAgeLabel->setText(QString::fromStdString(core.getName_P()));
	m_textEditDescription->setText(QString::fromStdString(core.checkDescription_P()));

	//Modification de l'image affichée
	QPixmap pixMap(QString::fromStdString(core.getPath_P()));

	QRect screenGeometry = QApplication::desktop()->screenGeometry();			//Permet de trouver la bonne dimension pour l'image selon la hauteur de l'écran
	int h = screenGeometry.height();
	int newImageSize = (int)(h * 0.30);

	QPixmap scaledPix = pixMap.scaled(QSize(newImageSize, newImageSize));		//permet de redimensionner l'image

	m_imageLabel->setPixmap(scaledPix);
}

void SecondWindow::superlikeUser()
{
	int retour = core.superLike();
	if (retour == 1) {			//fin de la liste, il n'y a plus personne a apprecier
		QMessageBox::information(this, "ATTENTION!", "Il n'y a plus personne a apprecier", QMessageBox::Ok);
		noMoreJudgment();
		return;
	}
	else if (retour == 2) {		//plus de user et que tu as un match
		QMessageBox::information(this, "WOW!", "VOUS AVEZ UN SUPER MATCH!!! Cependant, il n'y a plus d'utilisateur dans la liste... Revenez plus tard pour d'autres rencontres sensuelles. ;)", QMessageBox::Ok);
		noMoreJudgment();
		return;
	}

	else if (retour == 0) {
		QMessageBox::information(this, "WOW!", "VOUS AVEZ UN SUPER MATCH!!! Écrivez rapidement à l'utilisateur en question pour organiser une SUPER rencontre sensuelle ;)", QMessageBox::Ok);
	}

	m_nomAgeLabel->setText(QString::fromStdString(core.getName_P()));
	m_textEditDescription->setText(QString::fromStdString(core.checkDescription_P()));

	//Modification de l'image affichée
	QPixmap pixMap(QString::fromStdString(core.getPath_P()));

	QRect screenGeometry = QApplication::desktop()->screenGeometry();			//Permet de trouver la bonne dimension pour l'image selon la hauteur de l'écran
	int h = screenGeometry.height();
	int newImageSize = (int)(h * 0.30);

	QPixmap scaledPix = pixMap.scaled(QSize(newImageSize, newImageSize));		//permet de redimensionner l'image

	m_imageLabel->setPixmap(scaledPix);
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
		core.logout();
	}
}

void SecondWindow::openThirdWindow()
{
	if (!paramOpen) {
		paramOpen = true;
		QWidget *w3 = new QWidget;				//Création et affichage de la troisieme fenetre avec le constructeur de ThirdWindow
		w3 = new ThirdWindow();
		cout << "after show\n";
	}
}

void SecondWindow::noMoreJudgment() {
		
	m_btnDislike->setEnabled(false);
	m_btnLike->setEnabled(false);
	m_btnSuperlike->setEnabled(false);

	m_nomAgeLabel->setText("La file est vide... \nrevenez plus tard :(");
	m_textEditDescription->setText("");
	//Modification de l'image affichée
	QPixmap pixMap("./PhotoProfil/imageTriste.PNG");

	QRect screenGeometry = QApplication::desktop()->screenGeometry();			//Permet de trouver la bonne dimension pour l'image selon la hauteur de l'écran
	int h = screenGeometry.height();
	int newImageSize = (int)(h * 0.30);

	QPixmap scaledPix = pixMap.scaled(QSize(newImageSize, newImageSize));		//permet de redimensionner l'image

	m_imageLabel->setPixmap(scaledPix);
}

//TROISIEME FENETRE (MODIF_PARAMETRE COMPTE)
ThirdWindow::ThirdWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowModality(Qt::ApplicationModal);

	m_btnSave = new QPushButton(tr("&Sauvegarder"));
	m_btnCancel = new QPushButton(tr("&Annuler"));
	m_btnSave->setStyleSheet(QString("background-color: lightgreen"));
	m_btnCancel->setStyleSheet(QString("background-color: lightgreen"));

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

	QPixmap bkgnd("./PhotoProfil/tinder.png");
	bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);

	m_thirdMainLayout->addWidget(m_groupBoxCompte);
	m_thirdMainLayout->addLayout(m_bottomLayout);

	setCentralWidget(m_thirdWidget);
	setWindowTitle("Parametre du compte");
	show();
}

ThirdWindow::~ThirdWindow()
{
	cout << "Close third window\n";
	paramOpen = false;
}

void ThirdWindow::createGroupBoxCompte()
{
	//QString textMDP = QString::fromStdString(		GET LE PASSWORD

	m_groupBoxCompte = new QGroupBox(tr("Veuillez entrez les modifications de compte desirees: "));
	m_formLayoutCompte = new QFormLayout;

	m_labelUsernameCpt = new QLabel("Nom d'utilisateur actuel (non modifiable): ", this);
	m_labelMDPCpt = new QLabel("Nouveau mot de passe: ", this);
	m_labelConfirmMDPCpt = new QLabel("Confirmation du nouveau mot de passe: ", this);
	m_labelDescriptionCpt = new QLabel("Modifie la description actuelle: ", this);
	m_labelImageCpt = new QLabel("Nouvelle photo de profil: ", this);

	m_lineEditUsernameCpt = new QLineEdit(QString::fromStdString(core.getName_U()), this);
	m_lineEditUsernameCpt->setStyleSheet(QString("background-color: lightgray"));
	m_lineEditUsernameCpt->setReadOnly(true);
	m_lineEditMDPCpt = new QLineEdit(QString::fromStdString(core.getPass()), this);	
	m_lineEditMDPCpt->setStyleSheet(QString("background-color: pink"));
	m_lineEditConfirmMDPCpt = new QLineEdit(QString::fromStdString(core.getPass()), this);
	m_lineEditConfirmMDPCpt->setStyleSheet(QString("background-color: pink"));
	m_lineEditMDPCpt->setEchoMode(QLineEdit::Password);
	m_lineEditConfirmMDPCpt->setEchoMode(QLineEdit::Password);

	m_textLineDescription = new QTextEdit(QString::fromStdString(core.checkDescription_U()), this);
	m_textLineDescription->setStyleSheet(QString("background-color: pink"));

	m_btnPhotoProfil = new QPushButton("Selectionner une image", this);
	m_btnPhotoProfil->setStyleSheet(QString("background-color: lightgreen"));

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
			
		path = "" ;	//vide le path
		for (int i = 0; i < fileNames.length(); i++) {
			path += fileNames[i];			//path est le path de l'image
		}
			
		//Tentation pour copier l'image choisi dans le répertoire local s'il ne l'était pas déjà
		/*QRegExp rx("/PhotoProfil/");
		if (!rx.exactMatch(path)) {
			cout << "Copying file in local directory\n";
			QString newName = QString("./PhotoProfil/%1.PNG").arg(randRange(100000, 99999));
			QFile::copy(path, newName);
		}
		else
			cout << "Image already in local directory\n";*/

	}
}

void ThirdWindow::savePopUp()
{
	if (m_lineEditMDPCpt->text() == "") {
		QMessageBox::warning(this, "ATTENTION!", "Vous ne pouvez pas avoir un mot de passe vide", QMessageBox::Ok);
		m_lineEditMDPCpt->clear();
		m_lineEditConfirmMDPCpt->clear();
		m_lineEditMDPCpt->setFocus();
	}

	else if (m_lineEditMDPCpt->text() == m_lineEditConfirmMDPCpt->text()) {

		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "ATTENTION!", "Voulez-vous vraiment sauvegarder les modifications apportees ?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			//description = m_textLineDescription->toPlainText();

			QString textMDP = m_lineEditConfirmMDPCpt->text();		//Si l'utilisateur accepte la sauvegarder, le nouveau mot de passe est enregistré
			QString textMDPConfirm = m_lineEditMDPCpt->text();
			core.changePassword(textMDP.toStdString(), textMDPConfirm.toStdString());

			QString textDescription = m_textLineDescription->toPlainText();		//Si l'utilisateur accepte la sauvegarder, la nouvelle description est enregistrée
			core.changeDescription(textDescription.toStdString());
			core.setPath(path.toStdString());		//modification du path
			core.save();
			close();
		}
	}
	else {
		QMessageBox::warning(this, "ATTENTION!", "Vos deux mots de passe ne sont pas identiques, veuillez recommencer vos entrees.", QMessageBox::Ok);
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