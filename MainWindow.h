#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtVTKProject.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <QKeyEvent>

class QtVTKProject : public QMainWindow
{
    Q_OBJECT

private:
    Ui::QtVTKProjectClass ui;

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<QVTKInteractor> interactor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> interactorStyle;

public:
    QtVTKProject(QWidget *parent = nullptr);
    ~QtVTKProject();

public slots:
    void onDrawCubeClick();

protected:
    void keyPressEvent(QKeyEvent* event) override;

};
