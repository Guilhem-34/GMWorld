#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);
    void setPosition(double latitude, double longitude);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_latitude = 0.0;
    double m_longitude = 0.0;
};

#endif // MAPWIDGET_H