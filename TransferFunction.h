#pragma once

#include <QObject>
#include <QVector>
#include <qcustomplot.h>

class TransferFunction : public QObject
{
	Q_OBJECT

public:
	TransferFunction(QCustomPlot* plot, QObject* parent = nullptr);
	
	void updatePlot(const QVector<double>& x, const QVector<double>& y);


signals:
	void plotValueChanged(const QVector<double>& x, const QVector<double>& y);

private:
	QCustomPlot* mPlot;
	QVector<double> mX;
	QVector<double> mY;
};