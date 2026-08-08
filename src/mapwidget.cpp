#include "mapwidget.h"
#include <QPainter>

MapWidget::MapWidget(QWidget *parent) : QWidget(parent)
{
}

void MapWidget::setPosition(double latitude, double longitude)
{
    m_latitude = latitude;
    m_longitude = longitude;
    update(); // demande à Qt de redessiner le widget (appelle paintEvent)
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fond "océan"
    painter.fillRect(rect(), QColor(15, 25, 45));

    // Grille tous les 30 degrés
    painter.setPen(QColor(60, 80, 110));
    for (int lon = -180; lon <= 180; lon += 30) {
        int x = static_cast<int>((lon + 180) / 360.0 * width());
        painter.drawLine(x, 0, x, height());
    }
    for (int lat = -90; lat <= 90; lat += 30) {
        int y = static_cast<int>((90 - lat) / 180.0 * height());
        painter.drawLine(0, y, width(), y);
    }

    // Équateur / méridien de Greenwich, plus visibles
    painter.setPen(QColor(100, 130, 170));
    painter.drawLine(0, height() / 2, width(), height() / 2);
    painter.drawLine(width() / 2, 0, width() / 2, height());

    // Le point du satellite
    int x = static_cast<int>((m_longitude + 180) / 360.0 * width());
    int y = static_cast<int>((90 - m_latitude) / 180.0 * height());

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 80, 80));
    painter.drawEllipse(QPoint(x, y), 6, 6);
}