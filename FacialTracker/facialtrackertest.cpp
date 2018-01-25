#include "facialtrackertest.h"

FacialTrackerTest::FacialTrackerTest(QObject *parent) : QObject(parent)
{

}



void FacialTrackerTest::PCATest()
{
    MatrixXf data(6, 2);
    for (int indPoint = 0; indPoint < data.rows(); indPoint++){
        data(indPoint, 0) = indPoint+1;
        data(indPoint, 1) = indPoint+1;
    }

    PCA pca(data);
    MatrixXf reducedData = pca.transform(data, 1);
    MatrixXf unReducedData = pca.transformInverse(reducedData);

    for (int indPoint = 0; indPoint < data.rows(); indPoint++){
        QCOMPARE(unReducedData(indPoint, 0), data(indPoint, 0));
        QCOMPARE(unReducedData(indPoint, 1), data(indPoint, 1));
    }
}
