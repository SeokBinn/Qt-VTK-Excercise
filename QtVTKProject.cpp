#include "MainWindow.h"

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkDICOMImageReader.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkColorTransferFunction.h>


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

	QObject::connect(ui.loadDICOM_button, &QPushButton::clicked, this, &QtVTKProject::onLoadDICOMClicked);
}

QtVTKProject::~QtVTKProject()
{}

void QtVTKProject::onLoadDICOMClicked()
{
	QString directory = QFileDialog::getExistingDirectory(nullptr, "Select DICOM Directory", QString(), QFileDialog::ShowDirsOnly);

	vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	dicomReader->SetDirectoryName(directory.toStdString().c_str());
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

	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapper->SetInputConnection(dicomReader->GetOutputPort());

	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	renderer->AddVolume(volume);
	renderer->ResetCamera();
	renderer->Render();

}

void QtVTKProject::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		QApplication::quit();
	}
}