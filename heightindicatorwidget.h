
#ifndef HEIGHTINDICATORWIDGET_H
#define HEIGHTINDICATORWIDGET_H

#include <QWidget>

#include <array>

class HeightIndicatorWidget : public QWidget

{
    Q_OBJECT

public:
    HeightIndicatorWidget(QWidget *parent = nullptr);
    ~HeightIndicatorWidget();

public:
    void setHeight(qint32 height);
    qint32 getHeight() const;

protected:
    void paintEvent(QPaintEvent* event) override;


private:
    qint32 m_height = 0;

    const qint32 m_defaultWidth;
    const qint32 m_defaultHeight;

    const qreal m_bigCircleRadiusPercentage;
    const qreal m_smallCircleRadiusPercentage;

    std::array<QPoint, 4> m_hourHandPoints;
    std::array<QPoint, 4> m_minuteHandPoints;

    QColor m_hourHandColor;
    QColor m_hourLineColor;
    QColor m_minuteHandColor;
    QColor m_minuteLineColor;

    QColor m_backgroundColor;
    QColor m_lineColor;
    QColor m_numberColor;
    QColor m_unitColor;
};

#endif // HEIGHTINDICATORWIDGET_H
