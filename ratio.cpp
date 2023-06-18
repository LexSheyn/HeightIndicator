#include "ratio.h"

qreal degreesToRadians(qreal degrees)
{
    constexpr qreal PI = 3.14159265358979;

    const qreal radians = degrees * PI / 180.0;

    return radians;
}

qint32 ratioI32(qint32 value, qreal ratio)
{
    const qreal valueFloat = (qreal)value;

    const qint32 ratioInt = (qint32)(valueFloat * ratio);

    return ratioInt;
}

qint32 percentageI32(qint32 value, qreal percentage)
{
    const qreal valueFloat = (qreal)value;

    const qreal onePercent = valueFloat / 100.0;

    const qint32 percentageInt = (qint32)(onePercent * percentage);

    return percentageInt;
}

qint32 hertzToMilliseconds(qint32 hertz)
{
    const qint32 milliseconds = hertz * 1000;

    return milliseconds;
}
