#include "MainWindow.h"
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkDICOMImageReader.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkImageData.h>


QtVTKProject::QtVTKProject(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	renderer = vtkSmartPointer<vtkRenderer>::New();
	interactor = vtkSmartPointer<QVTKInteractor>::New();
	interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

	renderWindow->AddRenderer(renderer);
	renderWindow->SetInteractor(interactor);

	ui.openGLWidget->setRenderWindow(renderWindow);
	interactor->SetInteractorStyle(interactorStyle);
	interactor->Initialize();

	renderer->SetBackground(0.1, 0.2, 0.4);
	//renderer->SetBackground(1.0, 1.0, 1.0);

	QObject::connect(ui.loadDICOM_button, &QPushButton::clicked, this, &QtVTKProject::onLoadDICOMClicked);
}

QtVTKProject::~QtVTKProject()
{}

void QtVTKProject::onLoadDICOMClicked()
{
	QString directory = QFileDialog::getExistingDirectory(nullptr, "Select DICOM Directory", QString(), QFileDialog::ShowDirsOnly);

	if (directory.isEmpty()) { return; }

	vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	dicomReader->SetDirectoryName(directory.toStdString().c_str());
	dicomReader->Update();

	vtkSmartPointer<vtkImageData> imageData = dicomReader->GetOutput();

	double scalarRange[2];
	imageData->GetScalarRange(scalarRange);

	double min = scalarRange[0];
	double max = scalarRange[1];

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(0.0, 0.0);
	compositeOpacity->AddPoint((min + max) / 2, 1.0);
	compositeOpacity->AddPoint(max, 1.0);

	color = vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	color->AddRGBPoint(255.0, 1.0, 1.0, 1.0);

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(color);
	volumeProperty->SetScalarOpacity(compositeOpacity);
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();

	volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapper->SetInputData(imageData);

	volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	renderer->AddVolume(volume);
	renderer->ResetCamera();
	renderer->Render();

	currentWidth = 500;

	ui.transferFunctionWidget->xAxis->setRange(min, max);
	ui.transferFunctionWidget->yAxis->setRange(-0.25, 1.25);

	if (ui.transferFunctionWidget->graphCount() == 0)
	{
		ui.transferFunctionWidget->addGraph();
	}

	transferX.push_back(0.0);
	transferX.push_back((min + max) / 2);
	transferX.push_back(max);

	transferY.push_back(0.0);
	transferY.push_back(1.0);
	transferY.push_back(1.0);

	ui.transferFunctionWidget->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
	ui.transferFunctionWidget->graph(0)->setData(transferX, transferY);
	ui.transferFunctionWidget->setInteraction(QCP::iSelectPlottables, true);
	
	ui.transferFunctionWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.transferFunctionWidget->installEventFilter(this);
	
	ui.xAxisRangeSlider->SetRange(min, max);

	connect(ui.transferFunctionWidget, &QCustomPlot::mousePress, this, &QtVTKProject::onMouseLeftPressAndDraggedinGraph);
	connect(ui.transferFunctionWidget, &QCustomPlot::mousePress, this, &QtVTKProject::onMouseRightPressinGraph);
	connect(ui.xAxisRangeSlider, &RangeSlider::lowerValueChanged, this, &QtVTKProject::onMinValueChanged);
	connect(ui.xAxisRangeSlider, &RangeSlider::upperValueChanged, this, &QtVTKProject::onMaxValueChanged);

	ui.transferFunctionWidget->replot();
}

void QtVTKProject::onMinValueChanged(int min)
{
	ui.transferFunctionWidget->xAxis->setRange(min, ui.xAxisRangeSlider->GetUpperValue());
	ui.transferFunctionWidget->replot();
}

void QtVTKProject::onMaxValueChanged(int max)
{
	ui.transferFunctionWidget->xAxis->setRange(ui.xAxisRangeSlider->GetLowerValue(), max);
	ui.transferFunctionWidget->replot();
}

void QtVTKProject::onMouseLeftPressAndDraggedinGraph(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		double xPoint = ui.transferFunctionWidget->xAxis->pixelToCoord(event->pos().x());
		double yPoint = ui.transferFunctionWidget->yAxis->pixelToCoord(event->pos().y());
		double minDistance = 30;
		int minIndex = -1;
		for (int i = 0; i < transferX.size(); i++)
		{
			double distance = sqrt(pow(transferX[i] - xPoint, 2) + pow(transferY[i] - yPoint, 2));
			if (distance < minDistance)
			{
				minDistance = distance;
				minIndex = i;
			}
		}
		if (minIndex != -1)
		{
			selectedPointIndex = minIndex;
			isDragging = true;
		}
	}
}

void QtVTKProject::mouseMoveEvent(QMouseEvent* event)
{
	if (isDragging)
	{
		double xPoint = ui.transferFunctionWidget->xAxis->pixelToCoord(event->pos().x());
		double yPoint = ui.transferFunctionWidget->yAxis->pixelToCoord(event->pos().y());

		if (yPoint < 0.0)
		{
			yPoint = 0.0;
		}
		if (yPoint > 1.0)
		{
			yPoint = 1.0;
		}

		transferX[selectedPointIndex] = xPoint;
		transferY[selectedPointIndex] = yPoint;
		ui.transferFunctionWidget->graph(0)->setData(transferX, transferY);
		ui.transferFunctionWidget->replot();

		updateVolumeRendering();
	}
}

void QtVTKProject::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		isDragging = false;
	}
}

void QtVTKProject::onMouseRightPressinGraph(QMouseEvent* event)
{
	if (event->button() != Qt::RightButton) { return; }

	QMenu menu;
	QAction* addAction = menu.addAction("Add Point");
	QAction* deleteAction = menu.addAction("Delete Point");
	QAction* selectedItem = menu.exec(ui.transferFunctionWidget->mapToGlobal(event->pos()));
	if (selectedItem == addAction)
	{
		double xPoint = ui.transferFunctionWidget->xAxis->pixelToCoord(event->pos().x());
		double yPoint = ui.transferFunctionWidget->yAxis->pixelToCoord(event->pos().y());
		transferX.push_back(xPoint);
		transferY.push_back(yPoint);
		ui.transferFunctionWidget->graph(0)->setData(transferX, transferY);
		ui.transferFunctionWidget->replot();

		updateVolumeRendering();
	}
	else if (selectedItem == deleteAction)
	{
		if (transferX.size() == 3) { return; }
		double xPoint = ui.transferFunctionWidget->xAxis->pixelToCoord(event->pos().x());
		double yPoint = ui.transferFunctionWidget->yAxis->pixelToCoord(event->pos().y());
		
		double minDistance = 30;
		int minIndex = -1;
		for (int i = 0; i < transferX.size(); i++)
		{
			double distance = sqrt(pow(transferX[i] - xPoint, 2) + pow(transferY[i] - yPoint, 2));
			if (distance < minDistance)
			{
				minDistance = distance;
				minIndex = i;
			}
		}
		if (minIndex != -1)
		{
			transferX.erase(transferX.begin() + minIndex);
			transferY.erase(transferY.begin() + minIndex);
			ui.transferFunctionWidget->graph(0)->setData(transferX, transferY);
			ui.transferFunctionWidget->replot();
		}

		updateVolumeRendering();
	}
}

bool QtVTKProject::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == ui.transferFunctionWidget)
	{
		if (event->type() == QEvent::MouseMove)
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
			mouseMoveEvent(mouseEvent);
			return true;
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
			mouseReleaseEvent(mouseEvent);
			return true;
		}
	}
	return QMainWindow::eventFilter(obj, event);
}

void QtVTKProject::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		QApplication::quit();
	}
}

void QtVTKProject::updateVolumeRendering()
{
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	for (int i = 0; i < transferX.size(); i++)
	{
		compositeOpacity->AddPoint(transferX[i], transferY[i]);
	}

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(color);
	volumeProperty->SetScalarOpacity(compositeOpacity);
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();

	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	renderer->Render();
}	