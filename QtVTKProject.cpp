#include "MainWindow.h"

#include <vtkCubeSource.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

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

	QObject::connect(ui.drawCube_button, &QPushButton::clicked, this, &QtVTKProject::onDrawCubeClick);
}

QtVTKProject::~QtVTKProject()
{}

void QtVTKProject::onDrawCubeClick()
{
	std::string dicomDirectory = "C:/DICOM/ScalarVolume_9/";

	vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	dicomReader->SetDirectoryName(dicomDirectory.c_str());
	dicomReader->Update();

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(0.0, 0.0);
	compositeOpacity->AddPoint(1232.0, 1.0);

	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	color->AddRGBPoint(255.0, 1.0, 1.0, 1.0);

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(color);
	volumeProperty->SetScalarOpacity(compositeOpacity);
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cubeSource->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	renderer->AddActor(actor);
	renderer->ResetCamera();
	renderWindow->Render();
}

void QtVTKProject::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		QApplication::quit();
	}
}