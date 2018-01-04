#include "linearmodel.h"



LinearRegression::LinearRegression()
{

}



void LinearRegression::deleteParams()
{
    params.clear();
}



void LinearRegression::setParams(const vector<MatrixXf> params)
{
    this->params = params;
}



vector<MatrixXf> LinearRegression::getParams() const
{
    return params;
}



void LinearRegression::fit(const MatrixXf &x, const MatrixXf &y)
{
    QElapsedTimer timer;
    timer.start();
    MatrixXf a(x.rows(), x.cols()+1);
    a.block(0, 1, a.rows(), a.cols()-1) = x;
    a.col(0) = MatrixXf::Ones(a.rows(), 1);
    qDebug() << "Times a-matrix creating = " << timer.elapsed()/1000.0;

    timer.start();
    MatrixXf aTranspose = a.transpose();
    MatrixXf decomposMat = aTranspose * a;
    qDebug() << "Times a.transpose * a = " << timer.elapsed()/1000.0;

    timer.start();
    LLT<MatrixXf> llt(decomposMat);
    qDebug() << "Times llt creating = " << timer.elapsed()/1000.0;


    int countMarks = y.cols()/2;
    for (int indMark = 0, indMarkAxis = 0; indMark < countMarks; indMark++){
        int countOut = 2;
        int countIn = x.cols();
        MatrixXf paramsOfMark(countOut, countIn+1);

        timer.start();
        for (int markAxis = 0; markAxis < countOut; markAxis++){
            MatrixXf m = aTranspose * y.col(indMarkAxis++);
            VectorXf paramsOfMarkAxis = llt.solve(m);
            paramsOfMark.row(markAxis) = paramsOfMarkAxis;
        }

        qDebug() << "Times solve point " << indMark << " = "<< timer.elapsed()/1000.0;
        params.push_back(paramsOfMark);
    }
}



MatrixXf LinearRegression::predict(const MatrixXf &x) const
{
    MatrixXf deltaTransform(x.rows(), 68*2);
        for (int indFace = 0; indFace < x.rows(); indFace++)
            for (int indMark = 0, indMarkAxis = 0; indMark < 68; indMark++){
                MatrixXf paramsOfMark = params[indMark];
                deltaTransform(indFace, indMarkAxis) = paramsOfMark(0,0) + paramsOfMark.row(0).segment(1, paramsOfMark.cols()-1).dot(x.row(indFace).segment(0, x.cols()));
                indMarkAxis++;
                deltaTransform(indFace, indMarkAxis) = paramsOfMark(1,0) + paramsOfMark.row(1).segment(1, paramsOfMark.cols()-1).dot(x.row(indFace).segment(0, x.cols()));
                indMarkAxis++;
            }

        return deltaTransform;
}
