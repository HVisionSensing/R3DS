#include "gradientdescent.h"

ProblemVector GradientDescent::getGradientAnalytical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x)
{
    AutoDiff shiftXLocal(x.shiftX);
    AutoDiff shiftYLocal(x.shiftY);
    AutoDiff alphaLocal(x.alpha);
    AutoDiff alpha = AutoDiff::IndependendVariable(x.alpha);
    AutoDiff shiftX = AutoDiff::IndependendVariable(x.shiftX);
    AutoDiff shiftY = AutoDiff::IndependendVariable(x.shiftY);

    float gradientLocalAlpha = GradientDescent::getFunction(meshFrom, meshTo, alpha, shiftXLocal, shiftYLocal).getDerivative();
    float gradientLocalShiftX = GradientDescent::getFunction(meshFrom, meshTo, alphaLocal, shiftX, shiftYLocal).getDerivative();
    float gradientLocalShiftY = GradientDescent::getFunction(meshFrom, meshTo, alphaLocal, shiftXLocal, shiftY).getDerivative();
    ProblemVector gradientEntire(gradientLocalAlpha, gradientLocalShiftX, gradientLocalShiftY);
    return gradientEntire;
}



ProblemVector GradientDescent::getGradientNumerical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x)
{
    float delta = 0.001;
    float alphaWithDelta = x.alpha + delta;
    float shiftXWithDelta = x.shiftX + delta;
    float shfitYWithDelta = x.shiftY + delta;

    float migratedAlphaGradient = (GradientDescent::getFunction(meshFrom, meshTo, alphaWithDelta, x.shiftX, x.shiftY) - GradientDescent::getFunction(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY))/delta;
    float migratedXGradient = (GradientDescent::getFunction(meshFrom, meshTo, x.alpha, shiftXWithDelta, x.shiftY) - GradientDescent::getFunction(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY))/delta;
    float migratedYGradient = (GradientDescent::getFunction(meshFrom, meshTo, x.alpha, x.shiftX, shfitYWithDelta) - GradientDescent::getFunction(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY))/delta;
    ProblemVector gradientEntire(migratedAlphaGradient,migratedXGradient,migratedYGradient);
    return gradientEntire;
}



ProblemVector GradientDescent::comparisonGradients(ProblemVector &gradientFirst, ProblemVector &gradientSecond)
{
    return gradientSecond-gradientFirst;
}



ProblemVector GradientDescent::gradientDescent(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, UpdateFunctor *update)
{
    Settings settings(1000000, 0.001, 0.01);
    float stop;
    AutoDiff function(0,0);
    ProblemVector entireGradient(0,0,0);
    ProblemVector entireGradientSecond(0,0,0);
    ProblemVector x(0,0,0);
    ProblemVector comparisonGradients(0,0,0);
    for (int index = 0; index < settings.steps; index++){
        function = GradientDescent::getFunction(meshFrom, meshTo, x.alpha, x.shiftX, x.shiftY);
        entireGradient = GradientDescent::getGradientAnalytical(meshFrom, meshTo, x);
        entireGradientSecond = GradientDescent::getGradientNumerical(meshFrom, meshTo, x);
        comparisonGradients = GradientDescent::comparisonGradients(entireGradient, entireGradientSecond);
        qDebug() << "count step: " << index;
        qDebug() << "function: " << function.getValue();
        qDebug() << "comparsion gradients: " << comparisonGradients.getLen();
        qDebug() << "comparsion angle: " << comparisonGradients.alpha;
        qDebug() << "comparsion X: " << comparisonGradients.shiftX;
        qDebug() << "comparsion Y: " << comparisonGradients.shiftY;
        qDebug() << "----------------------------------";
        x = x - (entireGradient*settings.lambda);
        stop = entireGradient.getLen();
        if(update != NULL)
            update->update(x.alpha, x.shiftX, x.shiftY);
        if (stop < settings.epsilon)
            return x;
    }
    return x;
}



ProblemVector::ProblemVector(float alpha, float shiftX, float shiftY)
    :alpha(alpha), shiftX(shiftX), shiftY(shiftY)
{
}


Settings::Settings(int steps, float lambda, float epsilon)
    :steps(steps), lambda(lambda), epsilon(epsilon)
{

}
