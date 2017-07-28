#include "gradientdescent.h"

template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

ProblemVector GradientDescent::getGradientAnalytical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x)
{
    AutoDiff shiftXLocal(x.shiftX);
    AutoDiff shiftYLocal(x.shiftY);
    AutoDiff shiftZLocal(x.shiftZ);
    AutoDiff alphaLocal(x.alpha);
    AutoDiff alpha = AutoDiff::IndependendVariable(x.alpha);
    AutoDiff shiftX = AutoDiff::IndependendVariable(x.shiftX);
    AutoDiff shiftY = AutoDiff::IndependendVariable(x.shiftY);
    AutoDiff shiftZ = AutoDiff::IndependendVariable(x.shiftZ);
    AutoDiff axisX = AutoDiff::IndependendVariable(x.axisProblem[0]);
    AutoDiff axisY = AutoDiff::IndependendVariable(x.axisProblem[1]);
    AutoDiff axisZ = AutoDiff::IndependendVariable(x.axisProblem[2]);
    Vector3<AutoDiff> axisProblemLocal(x.axisProblem[0], x.axisProblem[1], x.axisProblem[2]);
    Vector3<AutoDiff> axisProblemX (axisX, axisProblemLocal[1], axisProblemLocal[2]);
    Vector3<AutoDiff> axisProblemY (axisProblemLocal[0], axisY, axisProblemLocal[2]);
    Vector3<AutoDiff> axisProblemZ (axisProblemLocal[0], axisProblemLocal[1], axisZ);

    float gradientLocalAlpha = GradientDescent::getFunctionRodrigues(meshFrom, meshTo, alpha, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal).getDerivative();
    float gradientLocalShiftX = GradientDescent::getFunctionRodrigues(meshFrom, meshTo, alphaLocal, shiftX, shiftYLocal, shiftZLocal, axisProblemLocal).getDerivative();
    float gradientLocalShiftY = GradientDescent::getFunctionRodrigues(meshFrom, meshTo, alphaLocal, shiftXLocal, shiftY, shiftZLocal, axisProblemLocal).getDerivative();
    float gradientLocalShiftZ = GradientDescent::getFunctionRodrigues(meshFrom, meshTo, alphaLocal, shiftXLocal, shiftYLocal, shiftZ, axisProblemLocal).getDerivative();
    float gradientLocalAxisProblemX = GradientDescent::getFunctionRodrigues(meshFrom, meshTo, alphaLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemX).getDerivative();
    float gradientLocalAxisProblemY = GradientDescent::getFunctionRodrigues(meshFrom, meshTo, alphaLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemY).getDerivative();
    float gradientLocalAxisProblemZ = GradientDescent::getFunctionRodrigues(meshFrom, meshTo, alphaLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemZ).getDerivative();
    Vector3<float> axisProblem(gradientLocalAxisProblemX, gradientLocalAxisProblemY, gradientLocalAxisProblemZ);
    ProblemVector gradientEntire(gradientLocalAlpha, gradientLocalShiftX, gradientLocalShiftY, gradientLocalShiftZ, axisProblem);
    return gradientEntire;
}



ProblemVector GradientDescent::getGradientNumerical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x)
{
    float delta = 0.001;
    float alphaWithDelta = x.alpha + delta;
    float shiftXWithDelta = x.shiftX + delta;
    float shiftYWithDelta = x.shiftY + delta;
    float shiftZWithDelta = x.shiftZ + delta;
    float axisXWithDelta = x.axisProblem[0] + delta;
    float axisYWithDelta = x.axisProblem[1] + delta;
    float axisZWithDelta = x.axisProblem[2] + delta;
    Vector3<float> axisProblemX (axisXWithDelta, x.axisProblem[1], x.axisProblem[2]);
    Vector3<float> axisProblemY (x.axisProblem[0], axisYWithDelta, x.axisProblem[2]);
    Vector3<float> axisProblemZ (x.axisProblem[0], x.axisProblem[1], axisZWithDelta);
    Vector3<float> axisProblem =  x.axisProblem;

    float migratedAlphaGradient = (GradientDescent::getFunctionRodrigues(meshFrom, meshTo, alphaWithDelta, x.shiftX, x.shiftY, x.shiftZ, axisProblem)
                                   - GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;

    float migratedXGradient = (GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, shiftXWithDelta, x.shiftY, x.shiftZ, axisProblem)
                               - GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;

    float migratedYGradient = (GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, shiftYWithDelta, x.shiftZ, axisProblem)
                               - GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;

    float migratedZGradient = (GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, shiftZWithDelta, axisProblem)
                               - GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;

    float migratedAxisProblemXGradient = (GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblemX)
                               - GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;

    float migratedAxisProblemYGradient = (GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblemY)
                               - GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;

    float migratedAxisProblemZGradient = (GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblemZ)
                               - GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;

    Vector3<float> axisProblemNew(migratedAxisProblemXGradient, migratedAxisProblemYGradient, migratedAxisProblemZGradient);

    ProblemVector gradientEntire(migratedAlphaGradient, migratedXGradient, migratedYGradient, migratedZGradient, axisProblemNew);
    return gradientEntire;
}



ProblemVector GradientDescent::comparisonGradients(ProblemVector &gradientFirst, ProblemVector &gradientSecond)
{
    return gradientSecond-gradientFirst;
}



ProblemVector GradientDescent::gradientDescent(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, UpdateFunctor *update)
{
    Settings constans(1000000, 0.0001, 0.01);
    float stop;
    AutoDiff function(0,0);
    ProblemVector entireGradient(0,0,0,0,Vector3<float>(0,0,0));
    ProblemVector entireGradientSecond(0,0,0,0,Vector3<float>(0,0,0));
    ProblemVector x(0,0,0,0,Vector3<float>(1,0,0));
    ProblemVector comparisonGradients(0,0,0,0,Vector3<float>(0,0,0));
    for (int index = 0; index < constans.steps; index++){
        function = GradientDescent::getFunctionRodrigues(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY, x.shiftZ, x.axisProblem);
        entireGradient = GradientDescent::getGradientAnalytical(meshFrom, meshTo, x);
        entireGradientSecond = GradientDescent::getGradientNumerical(meshFrom, meshTo, x);
        comparisonGradients = GradientDescent::comparisonGradients(entireGradient, entireGradientSecond);
        qDebug() << "count step: " << index;
        qDebug() << "function: " << function.getValue();
        qDebug() << "axis Problem X: " << x.axisProblem[0];
        qDebug() << "axis Problem Y: " << x.axisProblem[1];
        qDebug() << "axis Problem Z: " << x.axisProblem[2];
        qDebug() << "comparsion gradients: " << comparisonGradients.getLen();
        qDebug() << "comparsion angle: " << comparisonGradients.alpha;
        qDebug() << "comparsion axis X: " << comparisonGradients.axisProblem[0];
        qDebug() << "comparsion axis Y: " << comparisonGradients.axisProblem[1];
        qDebug() << "comparsion axis Z: " << comparisonGradients.axisProblem[2];
        qDebug() << "comparsion X: " << comparisonGradients.shiftX;
        qDebug() << "comparsion Y: " << comparisonGradients.shiftY;
        qDebug() << "comparsion Z: " << comparisonGradients.shiftZ;
        qDebug() << "gradient len: " << entireGradient.getLen();
        qDebug() << "gradient X: " << entireGradient.axisProblem[0];
        qDebug() << "gradient Y: " << entireGradient.axisProblem[1];
        qDebug() << "gradient Z: " << entireGradient.axisProblem[2];
        qDebug() << "----------------------------------";
        x = x - (entireGradient*constans.lambda);
        stop = entireGradient.getLen();
        x.axisProblem = x.axisProblem/x.axisProblem.norm();
        if(update != NULL)
            update->update(x.alpha, x.shiftX, x.shiftY, x.shiftZ, x.axisProblem);
        if (stop < constans.epsilon)
            return x;
    }
    return x;
}



ProblemVector::ProblemVector(float alpha, float shiftX, float shiftY, float shiftZ, Vector3<float> axisProblem)
    :alpha(alpha), shiftX(shiftX), shiftY(shiftY), shiftZ(shiftZ), axisProblem(axisProblem)
{
}


Settings::Settings(int steps, float lambda, float epsilon)
    :steps(steps), lambda(lambda), epsilon(epsilon)
{

}
