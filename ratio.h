
#ifndef RATIO_H
#define RATIO_H

#include <QtGlobal>

qreal degreesToRadians(qreal degrees);

qint32 ratioI32(qint32 value, qreal ratio);
qint32 percentageI32(qint32 value, qreal percentage);

qint32 hertzToMilliseconds(qint32 hertz);

#endif // RATIO_H
