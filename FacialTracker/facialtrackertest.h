#ifndef PCATEST_H
#define PCATEST_H

#include <QObject>
#include <QTest>

#include "pca.h"

class FacialTrackerTest : public QObject
{
    Q_OBJECT
public:
    explicit FacialTrackerTest(QObject *parent = 0);

private slots:
    void PCATest();

};
#endif // PCATEST_H
