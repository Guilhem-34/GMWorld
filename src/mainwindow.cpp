#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <libsgp4/Tle.h>
#include <libsgp4/SGP4.h>
#include <libsgp4/Eci.h>
#include <libsgp4/CoordGeodetic.h>
#include <libsgp4/DateTime.h>
#include <libsgp4/Util.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        "ISS (ZARYA)",
        "1 25544U 98067A   26220.50489838  .00004539  00000+0  89319-4 0  9991",
        "2 25544  51.6323  41.1734 0007358  25.6275 334.5077 15.49385107579869"
        );
    libsgp4::SGP4 sgp4(tle);
    libsgp4::Eci position = sgp4.FindPosition(libsgp4::DateTime::Now());
    libsgp4::CoordGeodetic geo = position.ToGeodetic();

    QString texte = QString("Latitude : %1°\nLongitude : %2°\nAltitude : %3 km")
                        .arg(libsgp4::Util::RadiansToDegrees(geo.latitude), 0, 'f', 2)
                        .arg(libsgp4::Util::RadiansToDegrees(geo.longitude), 0, 'f', 2)
                        .arg(geo.altitude, 0, 'f', 1);

    ui->labelPosition->setText(texte);
}