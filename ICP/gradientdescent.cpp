#include "gradientdescent.h"

void GradientDescent::getDerivativeCol(const int &col, const VectorS<AutoDiff> &corCol, MatrixS<float> &j)
{
    for (int row = 0; row < corCol.rows(); row++)
        j(row, col) = corCol(row).Derivative();
}

void GradientDescent::getValueCol(const int &col, const VectorS<float> &corCol, MatrixS<float> &j)
{
    for (int row = 0; row < corCol.rows(); row++)
        j(row, col) = corCol(row);
}
