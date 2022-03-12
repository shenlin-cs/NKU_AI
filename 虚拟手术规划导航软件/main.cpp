#ifndef INITIAL_OPENGL
#define INITIAL_OPENGL
#include <vtkAutoInit.h>
#include "widget.h"
#include "mainwindow.h"
#include <QApplication>
#include "TestVTK.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkRenderingContextOpenGL2)
#endif

//! Qtµ÷ÓÃvtk8.2
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	//TestVTK tvb;
	//tvb.test();

	return a.exec();
}
