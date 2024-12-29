#include "MainWindow.h"
#include <QtWidgets/QApplication>

//Load DICOM files and Volume Rendering
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtVTKProject w;
    w.show();
    return a.exec();
}
