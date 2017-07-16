#ifndef TESTKDTREE_H
#define TESTKDTREE_H

#include <QObject>
#include <QTest>
#include <QList>

class TestKDTree: public QObject
{
    Q_OBJECT
public:
    explicit TestKDTree(QObject *parent = 0);
private slots:
    void testKDTree();
};
#endif // TESTKDTREE_H
