
#include "heightindicatorwidget.h"

#include "ratio.h"

#include <QtWidgets>

#include <cmath>

// 1 Hz = 1000 ms

HeightIndicatorWidget::HeightIndicatorWidget(QWidget *parent)
    : QWidget(parent)
    , m_defaultWidth(450)
    , m_defaultHeight(450)
    , m_bigCircleRadiusPercentage(95)
    , m_smallCircleRadiusPercentage(50)
{
    m_backgroundColor = Qt::black;
    m_lineColor = Qt::white;
    m_numberColor = Qt::white;
    m_unitColor = Qt::gray;

    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, QOverload<>::of(&HeightIndicatorWidget::update));

    //timer->start(1000);
    //timer->start(10);
    timer->start(1);

    resize(m_defaultWidth, m_defaultHeight);

    const qint32 halfDefaultHeight = m_defaultHeight / 2;

    const qreal hundredArrowHeightPercentaqge = m_bigCircleRadiusPercentage - 5;

    const qint32 hundredArrowHeight = percentageI32(hundredArrowHeightPercentaqge, (qreal)halfDefaultHeight);

    m_minuteHandPoints[0] = QPoint( 8,   0);
    m_minuteHandPoints[1] = QPoint(-8,   0);
    m_minuteHandPoints[2] = QPoint(-4, -hundredArrowHeight);
    m_minuteHandPoints[3] = QPoint( 4, -hundredArrowHeight);

    const qreal thousandArrowHeightPercentaqge = m_smallCircleRadiusPercentage - 5;

    const qint32 thousandArrowHeight = percentageI32(thousandArrowHeightPercentaqge, (qreal)halfDefaultHeight);

    m_hourHandPoints[0] = QPoint( 8,   0);
    m_hourHandPoints[1] = QPoint(-8,   0);
    m_hourHandPoints[2] = QPoint(-4, -thousandArrowHeight);
    m_hourHandPoints[3] = QPoint( 4, -thousandArrowHeight);

    m_minuteHandColor = QColor(  0, 200, 200, 125);
    m_minuteLineColor = QColor(230,   0,   0, 125);

    m_hourHandColor   = QColor(  0, 230, 230, 125);
    m_hourLineColor   = QColor(230,   0,   0);

    QPalette palette = QPalette();

    palette.setColor(QPalette::Window, m_backgroundColor);

    setAutoFillBackground(true);
    setPalette(palette);
}

HeightIndicatorWidget::~HeightIndicatorWidget()
{
}

void HeightIndicatorWidget::setHeight(qint32 height)
{
    m_height = height;
}

qint32 HeightIndicatorWidget::getHeight() const
{
    return m_height;
}

void HeightIndicatorWidget::paintEvent(QPaintEvent *event)
{
    constexpr qint32 hundredCount = 10;
    constexpr qint32 twentyCount = hundredCount * 5;

    const qint32 smallerSide = qMin(width(), height());

    const qint32 halfDefaultWidth  = m_defaultWidth / 2;
    const qint32 halfDefaultHeight = m_defaultHeight / 2;

    const qint32 halfWidth  = width() / 2;
    const qint32 halfHeight = height() / 2;

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(halfWidth, halfHeight);
    painter.scale(smallerSide / (qreal)m_defaultWidth, smallerSide / (qreal)m_defaultHeight);

    // Draw big circle ////////////// Begin

    constexpr qreal bigCirclePenWidthPercentage = 1;

    const qint32 bigCircleRadiusX = percentageI32(halfDefaultWidth, m_bigCircleRadiusPercentage);
    const qint32 bigCircleRadiusY = percentageI32(halfDefaultHeight, m_bigCircleRadiusPercentage);
    const qint32 bigCirclePenWidth = percentageI32(halfDefaultWidth, bigCirclePenWidthPercentage);

    QPen bigCirclePen;

    bigCirclePen.setBrush(Qt::SolidPattern);
    bigCirclePen.setColor(m_lineColor);
    bigCirclePen.setWidth(bigCirclePenWidth);

    painter.save();
    painter.setPen(bigCirclePen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPoint(), bigCircleRadiusX, bigCircleRadiusY);
    painter.restore();

    // Draw big circle ////////////// End

    // Draw small circle ////////////// Begin

    const qint32 smallCircleRadiusX = percentageI32(halfDefaultWidth, m_smallCircleRadiusPercentage);
    const qint32 smallCircleRadiusY = percentageI32(halfDefaultHeight, m_smallCircleRadiusPercentage);

    painter.save();
    painter.setPen(bigCirclePen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPoint(), smallCircleRadiusX, smallCircleRadiusY);
    painter.restore();

    // Draw small circle ////////////// End

    // Draw big hundred marks ////////////// Begin

    constexpr qreal bigHundredMarkAngle = 360.0 / (qreal)hundredCount;

    const qreal bigHundredMarkBeginYPercentage = m_bigCircleRadiusPercentage - 15;
    const qreal bigHundredMarkEndYPercentage = m_bigCircleRadiusPercentage - 1;

    constexpr qreal bigHundredMarkPenWidthPercentage = 2;

    const qint32 bigHundredMarkBeginY = percentageI32(halfDefaultHeight, bigHundredMarkBeginYPercentage);
    const qint32 bigHundredMarkEndY = percentageI32(halfDefaultHeight, bigHundredMarkEndYPercentage);

    const qint32 bigHundredMarkPenWidth = percentageI32(halfDefaultWidth, bigHundredMarkPenWidthPercentage);

    QPen bigHundredMarkPen;

    bigHundredMarkPen.setBrush(Qt::SolidPattern);
    bigHundredMarkPen.setColor(m_lineColor);
    bigHundredMarkPen.setWidth(bigHundredMarkPenWidth);

    painter.save();
    painter.setPen(bigHundredMarkPen);
    painter.setBrush(Qt::NoBrush);

    for (qint32 i = 0; i < hundredCount; ++i)
    {
        painter.drawLine(0, bigHundredMarkBeginY, 0, bigHundredMarkEndY);
        painter.rotate(bigHundredMarkAngle);
    }

    painter.restore();

    // Draw big hundred marks ////////////// End

    // Draw small hundred marks ////////////// Begin

    const qreal smallHundredMarkBeginYPercentage = m_smallCircleRadiusPercentage - 5;
    const qreal smallHundredMarkEndYPercentage = m_smallCircleRadiusPercentage + 5;

    const qint32 smallHundredMarkBeginY = percentageI32(halfDefaultHeight, smallHundredMarkBeginYPercentage);
    const qint32 smallHundredMarkEndY = percentageI32(halfDefaultHeight, smallHundredMarkEndYPercentage);

    painter.save();
    painter.setPen(bigHundredMarkPen);
    painter.setBrush(Qt::NoBrush);

    for (qint32 i = 0; i < hundredCount; ++i)
    {
        painter.drawLine(0, smallHundredMarkBeginY, 0, smallHundredMarkEndY);
        painter.rotate(bigHundredMarkAngle);
    }

    painter.restore();

    // Draw small hundred marks ////////////// End

    // Draw twenty marks ////////////// Begin

    constexpr qreal twentyMarkPenWidthPercenatage = 1;

    const qint32 twentyMarkPenWidth = percentageI32(halfDefaultWidth, twentyMarkPenWidthPercenatage);

    QPen twentyMarkPen;

    twentyMarkPen.setBrush(Qt::SolidPattern);
    twentyMarkPen.setColor(m_lineColor);
    twentyMarkPen.setWidth(twentyMarkPenWidth);

    painter.save();

    painter.setPen(twentyMarkPen);
    painter.setBrush(Qt::NoBrush);

    constexpr qreal twentyMarkAngle = 360.0 / (qreal)twentyCount;

    const qreal twentyMarkBeginYPercentage = m_bigCircleRadiusPercentage - 7;
    const qreal twentyMarkEndYPercentage = m_bigCircleRadiusPercentage - 1;

    const qint32 twentyMarkBeginY = percentageI32(halfDefaultHeight, twentyMarkBeginYPercentage);
    const qint32 twentyMarkEndY = percentageI32(halfDefaultHeight, twentyMarkEndYPercentage);

    for (qint32 i = 0; i < twentyCount; ++i)
    {
        painter.drawLine(0, twentyMarkBeginY, 0, twentyMarkEndY);
        painter.rotate(twentyMarkAngle);
    }

    painter.restore();

    // Draw twenty marks ////////////// End

    // Draw numbers ////////////// Begin

    const qreal textDistanceFromCenterPercentage = smallHundredMarkBeginYPercentage + 22;
    constexpr qreal hundredFontHeightPercentage = 15;
    constexpr qreal textPenWidthPercentage = 1;

    const qint32 textDistanceFromCenter = percentageI32(halfDefaultHeight, textDistanceFromCenterPercentage);
    const qint32 hundredFontHeight = percentageI32(halfDefaultHeight, hundredFontHeightPercentage);

    constexpr qreal hundredAngle = 360.0 / (qreal)hundredCount;
    qreal hundredAccumulatedAngle = -90.0;

    const qint32 textPenWidth = percentageI32(halfDefaultWidth, textPenWidthPercentage);

    QPen textPen;

    textPen.setBrush(Qt::SolidPattern);
    textPen.setColor(m_numberColor);
    textPen.setWidth(textPenWidth);

    QFont hundredTextFont = painter.font();

    hundredTextFont.setPointSize(hundredFontHeight);

    QTextOption hundredNumberTextOption;

    hundredNumberTextOption.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    painter.save();
    painter.setPen(textPen);
    painter.setBrush(Qt::NoBrush);
    painter.setFont(hundredTextFont);

    for (qint32 i = 0; i < hundredCount; ++i)
    {
        QStaticText hundredCharacter;

        hundredCharacter.setText(QString::number(i));
        hundredCharacter.setTextOption(hundredNumberTextOption);
        hundredCharacter.setTextWidth(0);

        const qreal angleInRadians = degreesToRadians(hundredAccumulatedAngle);
        const qreal sinAngle = std::sin(angleInRadians);
        const qreal cosAngle = std::cos(angleInRadians);

        const qint32 textX = textDistanceFromCenter * cosAngle;
        const qint32 textY = (textDistanceFromCenter * sinAngle) - hundredFontHeight;

        painter.drawStaticText(textX, textY, hundredCharacter);

        hundredAccumulatedAngle += hundredAngle;
    }

    painter.restore();

    // Draw numbers ////////////// End

    // Draw units text ////////////// Begin

    QPen unitPen;

    unitPen.setBrush(Qt::SolidPattern);
    unitPen.setColor(m_unitColor);

    QFont unitTextFont = painter.font();

    unitTextFont.setPointSize(hundredFontHeight);

    QStaticText heightText;

    heightText.setText("H");
    heightText.setTextOption(hundredNumberTextOption);
    heightText.setTextWidth(0);

    QStaticText unitText;

    unitText.setText("Метры");
    unitText.setTextOption(hundredNumberTextOption);
    unitText.setTextWidth(0);

    painter.save();
    painter.setPen(unitPen);
    painter.setBrush(Qt::NoBrush);
    painter.setFont(unitTextFont);
    painter.drawStaticText(0, -80, heightText); // TO DO: Replace hardcoded Y value by percentage!
    painter.drawStaticText(0, 20, unitText);    // TO DO: Replace hardcoded Y value by percentage!
    painter.restore();

    // Draw units text ////////////// End

    // Draw hundred arrow ////////////// Begin

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_numberColor);
    painter.rotate(hundredAngle * (qreal)m_height / 100.0);
    painter.drawConvexPolygon(m_minuteHandPoints.data(), (qint32)m_minuteHandPoints.size());
    painter.restore();

    // Draw hundred arrow ////////////// End

    // Draw thousand arrow ////////////// Begin

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_numberColor);
    painter.rotate(hundredAngle * (qreal)m_height / 1000.0);
    painter.drawConvexPolygon(m_hourHandPoints.data(), (qint32)m_hourHandPoints.size());
    painter.restore();

    // Draw thousand arrow ////////////// End

    // Draw rounded rectangle ////////////// Begin

    // TO DO: Replace hardcoded values by percentage!

    QBrush rectangleBrush;

    rectangleBrush.setColor(Qt::black);
    rectangleBrush.setStyle(Qt::SolidPattern);

    painter.save();
    painter.setPen(textPen);
    painter.setBrush(rectangleBrush);
    painter.drawRoundedRect(-70, -25, 140, 50, 5, 5);
    painter.restore();

    // Draw rounded rectangle ////////////// End

    // Draw height value ////////////// Begin

    // TO DO: Replace hardcoded values by percentage!

    QFont heightValueTextFont = painter.font();

    heightValueTextFont.setPointSize(hundredFontHeight);

    QStaticText heightValueText;

    heightValueText.setText(QString::number(m_height));
    heightValueText.setTextOption(hundredNumberTextOption);
    heightValueText.setTextWidth(0);

    painter.save();
    painter.setPen(textPen);
    painter.setBrush(Qt::NoBrush);
    painter.setFont(heightValueTextFont);
    painter.drawStaticText(0, -30, heightValueText);
    painter.restore();

    // Draw height value ////////////// End
}


