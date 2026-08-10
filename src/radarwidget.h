#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>

class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget *parent = nullptr);
    void setLookAngle(double azimuthDeg, double elevationDeg);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_azimuth = 0.0;
    double m_elevation = -90.0; // en dessous de l'horizon par défaut
};

#endif // RADARWIDGET_H