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
	vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	cubeSource->SetXLength(1.0);
	cubeSource->SetYLength(1.0);
	cubeSource->SetZLength(1.0);
	cubeSource->Update();

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