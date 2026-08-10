#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <libsgp4/Tle.h>
#include <libsgp4/SGP4.h>
#include <libsgp4/Eci.h>
#include <libsgp4/CoordGeodetic.h>
#include <libsgp4/DateTime.h>
#include <libsgp4/Util.h>
#include <QUrl>
#include <QNetworkRequest>
#include <QDebug>
#include <libsgp4/Observer.h>
#include <libsgp4/CoordTopocentric.h>
#include <QStyle>

//exemple position near my location
const double OBSERVER_LATITUDE = 43.3615;
const double OBSERVER_LONGITUDE = 3.2079;
const double OBSERVER_ALTITUDE_KM = 0.068;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mapWidget = new MapWidget(this);
    mapWidget->setGeometry(20, 80, 500, 260);
    resize(560, 380);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onTleReplyFinished);
    fetchTle();
    radarWidget = new RadarWidget(this);
    radarWidget->setGeometry(540, 10, 260, 260);
    passesListWidget = new QListWidget(this);
    passesListWidget->setGeometry(20, 350, 780, 150);
    resize(820, 520);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->show();
    refreshPassesList();
    resize(820, 380);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePosition);
    timer->start(2000); // toutes les 2 secondes
    updatePosition(); // premier affichage immédiat, sans attendre 2s
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePosition()
{
    libsgp4::Tle tle(
        currentTleName.toStdString(),
        currentTleLine1.toStdString(),
        currentTleLine2.toStdString()
        );
    libsgp4::SGP4 sgp4(tle);
    libsgp4::Eci position = sgp4.FindPosition(libsgp4::DateTime::Now());
    libsgp4::CoordGeodetic geo = position.ToGeodetic();
    libsgp4::Observer observer(OBSERVER_LATITUDE, OBSERVER_LONGITUDE, OBSERVER_ALTITUDE_KM);
    libsgp4::CoordTopocentric lookAngle = observer.GetLookAngle(position);
    double azimuthDeg = libsgp4::Util::RadiansToDegrees(lookAngle.azimuth);
    double elevationDeg = libsgp4::Util::RadiansToDegrees(lookAngle.elevation);

    radarWidget->setLookAngle(azimuthDeg, elevationDeg);

    if (previousElevation <= 0 && elevationDeg > 0) {
        trayIcon->showMessage("ISS visible !",
                              "L'ISS vient de passer au-dessus de l'horizon.",
                              QSystemTrayIcon::Information, 5000);
    }
    previousElevation = elevationDeg;

    QString texte = QString("Latitude : %1°\nLongitude : %2°\nAltitude : %3 km")
                        .arg(libsgp4::Util::RadiansToDegrees(geo.latitude), 0, 'f', 2)
                        .arg(libsgp4::Util::RadiansToDegrees(geo.longitude), 0, 'f', 2)
                        .arg(geo.altitude, 0, 'f', 1);

    ui->labelPosition->setGeometry(20, 10, 500, 60);
    ui->labelPosition->setText(texte);
    mapWidget->setPosition(libsgp4::Util::RadiansToDegrees(geo.latitude),
                           libsgp4::Util::RadiansToDegrees(geo.longitude));
}

void MainWindow::fetchTle()
{
    QUrl url("https://celestrak.org/NORAD/elements/gp.php?CATNR=25544&FORMAT=tle");
    networkManager->get(QNetworkRequest(url));
}

void MainWindow::onTleReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Erreur reseau :" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QString data = QString::fromUtf8(reply->readAll());
    QStringList lines = data.split('\n', Qt::SkipEmptyParts);

    if (lines.size() >= 3) {
        currentTleName = lines[0].trimmed();
        currentTleLine1 = lines[1].trimmed();
        currentTleLine2 = lines[2].trimmed();
        refreshPassesList();
        qDebug() << "TLE recupere :" << currentTleName;
    }

    reply->deleteLater();
}

void MainWindow::refreshPassesList()
{
    libsgp4::Tle tle(currentTleName.toStdString(), currentTleLine1.toStdString(), currentTleLine2.toStdString());
    libsgp4::SGP4 sgp4(tle);
    libsgp4::Observer observer(OBSERVER_LATITUDE, OBSERVER_LONGITUDE, OBSERVER_ALTITUDE_KM);

    passesListWidget->clear();

    libsgp4::DateTime t = libsgp4::DateTime::Now();
    libsgp4::DateTime end = t.AddHours(48);

    bool inPass = false;
    libsgp4::DateTime passStart;
    double maxElev = 0.0;
    int count = 0;

    while (t < end && count < 10) {
        libsgp4::Eci eci = sgp4.FindPosition(t);
        libsgp4::CoordTopocentric look = observer.GetLookAngle(eci);
        double elevDeg = libsgp4::Util::RadiansToDegrees(look.elevation);

        if (!inPass && elevDeg > 0) {
            inPass = true;
            passStart = t;
            maxElev = elevDeg;
        } else if (inPass) {
            if (elevDeg > maxElev) maxElev = elevDeg;
            if (elevDeg <= 0) {
                inPass = false;
                QString line = QString("%1  ->  %2   (max %3°)")
                                   .arg(QString::fromStdString(passStart.ToString()))
                                   .arg(QString::fromStdString(t.ToString()))
                                   .arg(maxElev, 0, 'f', 1);
                passesListWidget->addItem(line);
                count++;
            }
        }
        t = t.AddSeconds(30);
    }
}