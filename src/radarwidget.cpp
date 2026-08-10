#include "radarwidget.h"
#include <QPainter>
#include <cmath>

RadarWidget::RadarWidget(QWidget *parent) : QWidget(parent)
{
}

void RadarWidget::setLookAngle(double azimuthDeg, double elevationDeg)
{
    m_azimuth = azimuthDeg;
    m_elevation = elevationDeg;
    update();
}

void RadarWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(15, 25, 45));

    int side = qMin(width(), height());
    QPointF center(width() / 2.0, height() / 2.0);
    double radius = side / 2.0 - 20;

    // Cercles d'élévation : 0° (horizon, bord) à 90° (zénith, centre)
    painter.setPen(QColor(60, 80, 110));
    painter.setBrush(Qt::NoBrush);
    for (int elevRing = 0; elevRing <= 90; elevRing += 30) {
        double r = radius * (90 - elevRing) / 90.0;
        painter.drawEllipse(center, r, r);
    }

    // Points cardinaux
    painter.setPen(QColor(150, 170, 200));
    painter.drawText(QRectF(center.x() - 10, center.y() - radius - 20, 20, 20), Qt::AlignCenter, "N");
    painter.drawText(QRectF(center.x() - 10, center.y() + radius, 20, 20), Qt::AlignCenter, "S");
    painter.drawText(QRectF(center.x() + radius, center.y() - 10, 20, 20), Qt::AlignCenter, "E");
    painter.drawText(QRectF(center.x() - radius - 20, center.y() - 10, 20, 20), Qt::AlignCenter, "O");

    // Le satellite, seulement s'il est au-dessus de l'horizon (visible)
    if (m_elevation > 0) {
        double r = radius * (90 - m_elevation) / 90.0;
        double angleRad = m_azimuth * M_PI / 180.0; // 0° = Nord, en haut
        double x = center.x() + r * std::sin(angleRad);
        double y = center.y() - r * std::cos(angleRad);

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(80, 220, 120));
        painter.drawEllipse(QPointF(x, y), 6, 6);
    }
}