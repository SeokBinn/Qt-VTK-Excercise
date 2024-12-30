#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtVTKProject.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <QKeyEvent>
#include <qfiledialog.h>
#include <QDebug>
#include <vtkAutoInit.h>


VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)

class QtVTKProject : public QMainWindow
{
    Q_OBJECT

private:
    Ui::QtVTKProjectClass ui;

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<QVTKInteractor> interactor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> interactorStyle;
	QFileDialog* dicomFileDialog;

    QVector<double> transferX, transferY;
    double currentWidth;

public:
    QtVTKProject(QWidget *parent = nullptr);
    ~QtVTKProject();

public slots:
    void onLoadDICOMClicked();
	void onMousePressinGraph(QMouseEvent* event);
	void onMinValueChanged(int min);
	void onMaxValueChanged(int max);

protected:
    void keyPressEvent(QKeyEvent* event) override;

};
