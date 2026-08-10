#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QNetworkAccessManager>
#include<QNetworkReply>
#include "mapwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void updatePosition();
    void onTleReplyFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    MapWidget *mapWidget;
    QNetworkAccessManager *networkManager;
    QString currentTleName = "ISS (ZARYA)";
    QString currentTleLine1 = "1 25544U 98067A   26220.50489838  .00004539  00000+0  89319-4 0  9991";
    QString currentTleLine2 = "2 25544  51.6323  41.1734 0007358  25.6275 334.5077 15.49385107579869";
    void fetchTle();
};
#endif // MAINWINDOW_H
