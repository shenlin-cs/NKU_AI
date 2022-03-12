#ifndef TESTVTK_H
#define TESTVTK_H

#include <vtkSmartPointer.h>
#include <vtkCameraActor.h>
#include <vtkNamedColors.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkPlanes.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageViewer2.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkAutoInit.h>
#include <vtkImageThreshold.h>

#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <sstream>


#include"vtkConeSource.h"
#include"vtkPolyDataMapper.h"
#include"vtkRenderWindow.h"
#include"vtkCamera.h"
#include"vtkActor.h"
#include"vtkRenderer.h"
#include"vtkRenderWindowInteractor.h"
#include"vtkInteractorStyleTrackballCamera.h"

//»¬¿é
#include"vtkCommand.h"
#include"vtkSliderWidget.h"
#include"vtkSliderRepresentation3D.h"
#include"vtkProperty.h"
#include"vtkLight.h"
#include"vtkMetaImageReader.h"
#include"vtkMarchingCubes.h"


//Ë«view
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>
#include <vtkTransform.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <QDateTime>
#include <vtkCubeSource.h>

//3Î¬
#include <vtkContourFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkStripper.h>

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActor.h"
#include "vtkSmartPointer.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageCast.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkVolume.h"
#include "vtkBoxWidget.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkColorTransferFunction.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageData.h"
#include "vtkImageResample.h"
#include "vtkMetaImageReader.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPlanes.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkXMLImageDataReader.h"
#include "vtkGPUVolumeRayCastMapper.h"

#define VTI_FILETYPE 1
#define MHA_FILETYPE 2


//Ðý×ª
#include <vtkAxesActor.h>
#include <vtkConeSource.h>
#include <vtkTextActor.h>
#include <vtkOutlineFilter.h>
#include <vtkLookupTable.h>
#include <vtkVolume16Reader.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageReslice.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageMapToWindowLevelColors.h>


//ÇÐ¸î

#include "vtkPlane.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkContourFilter.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkStripper.h"
#include "vtkCamera.h"
#include "vtkSmoothPolyDataFilter.h"
#include <vtkImagePlaneWidget.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include "vtkActor.h"
#include <vtkSmartPointer.h>
#include <vtkMetaImageReader.h>
using namespace std;
#include "vtkImplicitPlaneWidget.h"
#include "vtkClipPolyData.h"



//demo
#include "vtkCellPicker.h"
#include "vtkSliderRepresentation2D.h"

class TestVTK
{
public:
    TestVTK();

    void test();
};

#endif // TESTVTK_H


#include <vtkImageData.h>
#include <vtkProperty.h>
#include <vtkDataSetMapper.h>
#include <vtkRendererCollection.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkMarchingCubes.h"
#include "vtkStripper.h"
#include "vtkActor.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkImageShrink3D.h"
#include "vtkDecimatePro.h"
#include "vtkProperty.h"
#include <vtkInteractorStyleImage.h>
#include "itkImageToVTKImageFilter.h"
#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"