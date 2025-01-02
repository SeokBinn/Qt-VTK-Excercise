#include "TransferFunction.h"

TransferFunction::TransferFunction(QCustomPlot* plot, QObject* parent)
	:QObject(parent), mPlot(plot)
{

}

void TransferFunction::updatePlot(const QVector<double>& x, const QVector<double>& y)
{
	mX = x;
	mY = y;

	mPlot->graph(0)->setData(mX, mY);
	mPlot->replot();

	emit plotValueChanged(mX, mY);
}