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
        qDebug() << "TLE recupere :" << currentTleName;
    }

    reply->deleteLater();
}