#include "testkdtree.h"
#include "kdtree.h"
#include "pointfinder.h"

TestKDTree::TestKDTree(QObject *parent):QObject(parent)
{
}

void TestKDTree::testKDTree()
{
    QVector<QVector3D> listOfPoints;

    QVector3D v21(2, 3, 0);
    QVector3D v22(4, 2, 0);
    QVector3D v23(6, 5, 0);
    QVector3D v24(15, 4, 0);
    QVector3D v25(8, 7, 0);
    QVector3D v26(5.5, 10, 0);
    QVector3D v27(9, 12 , 0);

    listOfPoints.push_back(v21);
    listOfPoints.push_back(v22);
    listOfPoints.push_back(v23);
    listOfPoints.push_back(v24);
    listOfPoints.push_back(v25);
    listOfPoints.push_back(v26);
    listOfPoints.push_back(v27);

    QVector<int> listOfIndex;
    for (int i = 0; i < listOfPoints.size(); i++)
        listOfIndex.append(i);

    Node *tree = KdTree::kdTreeBuild(listOfPoints, listOfIndex);

    QVector3D point(9, 8, 0);
    Node *best = NULL;
    tree->nearestNeighborSearch(point, best);
    QCOMPARE(best->item, v25);

    QVector3D point2(1, 2, 0);
    Node *best2 = NULL;
    tree->nearestNeighborSearch(point2, best2);
    QCOMPARE(best2->item, v21);

    QVector3D point3(7, 2, 0);
    Node *best3 = NULL;
    tree->nearestNeighborSearch(point3, best3);
    QCOMPARE(best3->item, v22);

    QVector3D point4(7, 10, 0);
    Node *best4 = NULL;
    tree->nearestNeighborSearch(point4, best4);
    QCOMPARE(best4->item, v26);

    QVector3D point5(15, 8, 0);
    Node *best5 = NULL;
    tree->nearestNeighborSearch(point5, best5);
    QCOMPARE(best5->item, v24);

    QVector3D point6(11, 13, 0);
    Node *best6 = NULL;
    tree->nearestNeighborSearch(point6, best6);
    QCOMPARE(best6->item, v27);

    QVector3D v28(8, 9, 0);
    QVector3D v29(10, 10, 0);
    QVector3D v30(2, 5, 0);
    QVector3D v31(3, 7, 0);
    QVector3D v32(10, 2, 0);
    QVector3D v33(8, 2, 0);
    QVector3D v34(2, 6, 0);
    QVector3D v35(3, 8, 0);


    listOfPoints.push_back(v28);
    listOfPoints.push_back(v29);
    listOfPoints.push_back(v30);
    listOfPoints.push_back(v31);
    listOfPoints.push_back(v32);
    listOfPoints.push_back(v33);
    listOfPoints.push_back(v34);
    listOfPoints.push_back(v35);

    QVector<int> listOfIndex2;
    for (int i = 0; i < listOfPoints.size(); i++)
        listOfIndex2.append(i);

    Node *tree2 = KdTree::kdTreeBuild(listOfPoints, listOfIndex2);

    QVector3D point7(10, 13, 0);
    Node *best7 = NULL;
    tree2->nearestNeighborSearch(point7, best7);
    QCOMPARE(best7->item, v27);

    QVector3D point8(12, 11, 0);
    Node *best8 = NULL;
    tree2->nearestNeighborSearch(point8, best8);
    QCOMPARE(best8->item, v29);

    QVector3D point9(6.1, 9.8, 0);
    Node *best9 = NULL;
    tree2->nearestNeighborSearch(point9, best9);
    QCOMPARE(best9->item, v26);

    QVector3D point10(15, 8, 0);
    Node *best10 = NULL;
    tree2->nearestNeighborSearch(point10, best10);
    QCOMPARE(best10->item, v24);
}
