#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QFile>
#include <QBitmap>
#include <QPainter>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TestVTK.h"
#include <Windows.h>

std::string folder = "D:\\project\\brain";

//UI界面
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	QString qss;
	QFile qssFile(":/Resource/style.qss");
	qssFile.open(QFile::ReadOnly);
	if (qssFile.isOpen())
	{
		qss = QLatin1String(qssFile.readAll());
		qApp->setStyleSheet(qss);
		qssFile.close();
	}

	ui->setupUi(this);
	ui->pb_Normal->hide();
	ui->pb_stop_Diction->hide();
	ui->pb_stop_cartoon->hide();

	ui->setupUi(this);
	//初始化server并监听
	tcpServer = new QTcpServer(this); //qt自己内存管理
	if (!tcpServer->listen(QHostAddress::Any, 6666)) //监听所有网络地址，端口6666
		qDebug() << tcpServer->errorString();
	statusText = statusText + "wait for connecting..." + "\n";
	//ui->statusLabel->setText(statusText);
	//绑定信号槽，当有连接时作出反应
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(SocketConnet()));
			float data[1500][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
		ifstream infile;//定义读取文件流，相对于程序来说是in
}

MainWindow::~MainWindow()
{
	delete ui;
}

#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")

void MainWindow::Listen()
{
	//try
	//{
	//	auto socketSend = socket(AF_INET, SOCK_STREAM, 0);
		//auto socketWatch = socket(AF_INET, SOCK_STREAM, 0);

		//while (true)
		//{
		//	socketSend = socketWatch->Accept();//等待接收客户端连接
		//	Console.WriteLine(socketSend.RemoteEndPoint.ToString() + ":" + "连接成功!");
		//	//开启一个新线程，执行接收消息方法
		//	//Thread r_thread = new Thread(Received);
		//	//r_thread.IsBackground = true;
		//	//r_thread.Start(socketSend);
		//}
	//}
	//catch { }
}

void MainWindow::SocketConnet()
{
//	//获得client socket
//	clientTcpSocket = tcpServer->nextPendingConnection();
//	//绑定信号槽，接收数据，并且当连接关闭是删除连接
//	connect(clientTcpSocket, SIGNAL(readyRead()), this, SLOT(SocketReceive()));
//	connect(clientTcpSocket, SIGNAL(disconnected()), clientTcpSocket, SLOT(deleteLater()));
//	//显示客户端连接信息
//	QString clientIp = clientTcpSocket->peerAddress().toString();
//	QString clientPort = QString::number(clientTcpSocket->peerPort());
//	statusText = statusText + "conneted with " + clientIp + ":" + clientPort + "\n";
//	//ui->statusLabel->setText(statusText);
//}
//
//void MainWindow::SocketSend(QString sendStr)
//{
//	clientTcpSocket->write(sendStr.toStdString().c_str());
}

void MainWindow::SocketReceive()
{
//	//接收数据并显示，字节转换成了字符串
//	QString recvStr = clientTcpSocket->readAll();
//	statusText = statusText + recvStr + "\n";
//	//ui->statusLabel->setText(statusText);
//	//经处理后发送回去
//	SocketSend("From server: " + recvStr);
}

//void MainWindow::brain4()
//{
//	try
//	{
//		//点击开始监听时 在服务端创建一个负责监听IP和端口号的Socket
//		auto socketWatch = socket(AF_INET, SOCK_STREAM, 0);
//		server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//
//		server_addr.sin_port = htons(8080);
//
//		socketWatch.Bind(point);//绑定端口号
//		socketWatch.Listen(10);//设置监听
//		Console.WriteLine("Listen");
//		//创建监听线程
//		Thread thread = new Thread(Listen);
//		thread.IsBackground = true;
//		thread.Start(socketWatch);
//
//		simpleButton2.Enabled = false;
//	}
//	catch { }
//}

class StatusMessage {
public:
	static std::string Format(int slice, int maxSlice);
};

class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
	static myVtkInteractorStyleImage* New();
	vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);

protected:
	vtkImageViewer2* _ImageViewer;
	vtkTextMapper* _StatusMapper;
	int _Slice;
	int _MinSlice;
	int _MaxSlice;

public:
	void SetImageViewer(vtkImageViewer2* imageViewer);
	void SetStatusMapper(vtkTextMapper* statusMapper);

protected:
	void MoveSliceForward();
	void MoveSliceBackward();
	virtual void OnKeyDown();
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
};

vtkStandardNewMacro(myVtkInteractorStyleImage);

std::string StatusMessage::Format(int slice, int maxSlice) {
	std::stringstream tmp;
	tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
	return tmp.str();
}

void myVtkInteractorStyleImage::SetImageViewer(vtkImageViewer2* imageViewer) {
	_ImageViewer = imageViewer;
	_MinSlice = imageViewer->GetSliceMin();
	_MaxSlice = imageViewer->GetSliceMax();
	_Slice = int((_MinSlice+ _MaxSlice)/2);
	cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
}

void myVtkInteractorStyleImage::SetStatusMapper(vtkTextMapper* statusMapper) {
	_StatusMapper = statusMapper;
}

void myVtkInteractorStyleImage::MoveSliceForward() {
	if (_Slice < _MaxSlice) {
		_Slice += 1;
		cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
		_ImageViewer->SetSlice(_Slice);
		std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
		_StatusMapper->SetInput(msg.c_str());
		_ImageViewer->Render();
	}
}

void myVtkInteractorStyleImage::MoveSliceBackward() {
	if (_Slice > _MinSlice) {
		_Slice -= 1;
		cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
		_ImageViewer->SetSlice(_Slice);
		std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
		_StatusMapper->SetInput(msg.c_str());
		_ImageViewer->Render();
	}
}


void myVtkInteractorStyleImage::OnKeyDown() {
	std::string key = this->GetInteractor()->GetKeySym();
	if (key.compare("Up") == 0) {
		for (int i = _Slice; i < _MaxSlice; i++)
		{
			MoveSliceForward();
			Sleep(10);
		}
	}
	else if (key.compare("Down") == 0) {
		for (int i = _Slice; i > _MinSlice; i--)
		{
			MoveSliceBackward();
			Sleep(10);
		}
	}
	vtkInteractorStyleImage::OnKeyDown();
}

void myVtkInteractorStyleImage::OnMouseWheelForward() {
	MoveSliceForward();
}


void myVtkInteractorStyleImage::OnMouseWheelBackward() {
	//std::cout << "Scrolled mouse wheel backward." << std::endl;
	if (_Slice > _MinSlice) {
		MoveSliceBackward();
	}
}



//设置滑动条
//class vtkSliderCallback :public vtkCommand
//{
//public:
//	static vtkSliderCallback *New()
//	{
//		return new vtkSliderCallback;
//	}
//
//	virtual void Execute(vtkObject*caller, unsigned long, void*)
//	{
//		vtkSliderWidget*sliderWidget = reinterpret_cast<vtkSliderWidget*>(caller);
//		int value = static_cast<int>(static_cast<vtkSliderRepresentation *>
//			(sliderWidget->GetRepresentation())->GetValue());
//		//this->anActor->GetProperty()->SetDiffuse(static_cast<vtkSliderRepresentation*>
//		//	(sliderWidget->GetRepresentation())->GetValue());
//	}
//	vtkSliderCallback() :anActor(0) {}
//	vtkActor *anActor;
//};


class vtkSliderCallback :public vtkCommand
{
public:
	static vtkSliderCallback *New()
	{
		return new vtkSliderCallback;
	}

	virtual void Execute(vtkObject*caller, unsigned long, void*)
	{
		vtkSliderWidget*sliderWidget = reinterpret_cast<vtkSliderWidget*>(caller);

		this->vesselExtractor->SetSlice(int(static_cast<vtkSliderRepresentation*>(sliderWidget->GetRepresentation())->GetValue()));
	}
	vtkSliderCallback() :vesselExtractor(0) {}
	vtkImageViewer2 *vesselExtractor;

};
/*
class vtkSliderCallback : public vtkCommand
{
public:
	static vtkSliderCallback *New()
	{
		return new vtkSliderCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkSliderWidget *sliderWidget =
			reinterpret_cast<vtkSliderWidget*>(caller);
		if (!sliderWidget) return;
		int value = static_cast<int>(static_cast<vtkSliderRepresentation *>
			(sliderWidget->GetRepresentation())->GetValue());

		if (this->WidgetX)
		{
			this->WidgetX->SetSliceIndex(value);
		}
		else if (this->WidgetY)
		{
			this->WidgetY->SetSliceIndex(value);
		}
		else if (this->WidgetZ)
		{
			this->WidgetZ->SetSliceIndex(value);
		}
	}

	vtkSliderCallback() :WidgetX(0), WidgetY(0), WidgetZ(0) {}

	vtkImagePlaneWidget* WidgetX;
	vtkImagePlaneWidget* WidgetY;
	vtkImagePlaneWidget* WidgetZ;
};
*/
class vtkSliderCallback2 :public vtkCommand
{
public:
	static vtkSliderCallback2 *New()
	{
		return new vtkSliderCallback2;
	}

	virtual void Execute(vtkObject*caller, unsigned long, void*)
	{
		vtkSliderWidget*sliderWidget = reinterpret_cast<vtkSliderWidget*>(caller);

		this->vesselExtractor = (int(static_cast<vtkSliderRepresentation*>(sliderWidget->GetRepresentation())->GetValue()));
	}
	vtkSliderCallback2() :vesselExtractor(0) {}
	int vesselExtractor;
};

//联动

class myCallbackFunc :public vtkCommand
{
public:
	static  myCallbackFunc *New()
	{
		return new myCallbackFunc;
	}

	void SetLeftRender(vtkRenderer *leftRender)
	{
		this->m_leftRender = leftRender;
	}
	void SetRightRender(vtkRenderer *rightRender)
	{
		this->m_rightRender = rightRender;
	}


	virtual void Execute(vtkObject *caller, unsigned long eventId, void* callData)
	{
		vtkCamera *camera = reinterpret_cast<vtkCamera*>(caller);
		vtkCamera *leftCamera = m_leftRender->GetActiveCamera();
		vtkCamera *rightCamera = m_rightRender->GetActiveCamera();

		double *act_position = camera->GetPosition();
		double *act_FocalPoint = camera->GetFocalPoint();
		double *act_ViewUp = camera->GetViewUp();

		//方法一：指针判断
		if (leftCamera == camera)
		{
			if (rightCamera->GetPosition() != act_position)
			{
				rightCamera->SetPosition(act_position);
			}
			if (rightCamera->GetViewUp() != act_ViewUp)
			{
				rightCamera->SetFocalPoint(act_FocalPoint);
			}
			if (rightCamera->GetViewUp() != act_ViewUp)
			{
				rightCamera->SetViewUp(act_ViewUp);
			}
		}
		else if (rightCamera == camera)
		{
			if (leftCamera->GetPosition() != act_position)
			{
				leftCamera->SetPosition(act_position);
			}
			if (leftCamera->GetViewUp() != act_ViewUp)
			{
				leftCamera->SetFocalPoint(act_FocalPoint);
			}
			if (leftCamera->GetViewUp() != act_ViewUp)
			{
				leftCamera->SetViewUp(act_ViewUp);
			}
		}

		//方法二：camera具体细节判断

//        if (    (isSame(act_position, leftCamera->GetPosition()))
//              &&(isSame(act_FocalPoint, leftCamera->GetFocalPoint()))
//              &&(isSame(act_ViewUp,leftCamera->GetViewUp())))
//        {
//            rightCamera->SetPosition(act_position);
//            rightCamera->SetFocalPoint(act_FocalPoint);
//            rightCamera->SetViewUp(act_ViewUp);
//            this->m_rightRender->ResetCameraClippingRange();
//        }
//        else
//        {
//            leftCamera->SetPosition(act_position);
//            leftCamera->SetFocalPoint(act_FocalPoint);
//            leftCamera->SetViewUp(act_ViewUp);
//            this->m_leftRender->ResetCameraClippingRange();
//        }

	}

	//private:

	//    //计算获取到的相机是否为leftRender或rightRender的相机
	//    static bool isSame(double *value0, double *value1) {
	//        bool result = true;
	//        for (int idx = 0; idx != 3; ++idx) {
	//            result = result && qFuzzyCompare(value0[idx], value1[idx]);
	//        }
	//        return result;
	//    }

private:
	//此处为双view，若为多view，可设置数组，通过提供参数进行设置
	vtkRenderer     *m_leftRender;
	vtkRenderer     *m_rightRender;
};

//切割屏幕
class BuildVTKWidgetCall : public vtkCommand
{
public:

	static BuildVTKWidgetCall *New()
	{
		return new BuildVTKWidgetCall;
	}
public:
	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
	{
		vtkImplicitPlaneWidget *pWidget = reinterpret_cast<vtkImplicitPlaneWidget*>(caller);
		if (pWidget)
		{
			vtkSmartPointer<vtkPlane> planeNew = vtkPlane::New();
			pWidget->GetPlane(planeNew);

			cliper->SetClipFunction(planeNew);
			cliper->Update();

			vtkSmartPointer<vtkPolyData> clipedData = vtkPolyData::New();
			clipedData->DeepCopy(cliper->GetOutput());

			vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkPolyDataMapper::New();
			coneMapper->SetInputData(clipedData);
			coneMapper->ScalarVisibilityOff();
			actor->SetMapper(coneMapper);
		}
	}
	void setCliper(vtkSmartPointer<vtkClipPolyData> other) { cliper = other; }
	void setPlane(vtkSmartPointer<vtkPlane> other) { pPlane = other; }
	void setActor(vtkSmartPointer<vtkActor> other) { actor = other; }
private:
	vtkSmartPointer<vtkPlane> pPlane;
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkClipPolyData> cliper;
};


// FB: comment for testing svn

// Callback for moving the planes from the box widget to the mapper
class vtkBoxWidgetCallback : public vtkCommand
{
public:
	static vtkBoxWidgetCallback *New()
	{
		return new vtkBoxWidgetCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
		if (this->Mapper)
		{
			vtkPlanes *planes = vtkPlanes::New();
			widget->GetPlanes(planes);
			this->Mapper->SetClippingPlanes(planes);
			planes->Delete();
		}
	}
	void SetMapper(vtkGPUVolumeRayCastMapper* m)
	{
		this->Mapper = m;
	}

protected:
	vtkBoxWidgetCallback()
	{
		this->Mapper = 0;
	}

	vtkGPUVolumeRayCastMapper *Mapper;
};

//双view
/*
void MainWindow::test()
{		
		vtkSmartPointer<vtkCubeSource>                      cube        = vtkSmartPointer<vtkCubeSource>::New();
		vtkSmartPointer<vtkPolyDataMapper>                  mapper      = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkSmartPointer<vtkActor>                           actor1      = vtkSmartPointer<vtkActor>::New();
		vtkSmartPointer<vtkActor>                           actor2      = vtkSmartPointer<vtkActor>::New();
		vtkSmartPointer<vtkRenderer>                        leftRender  = vtkSmartPointer<vtkRenderer>::New();
		vtkSmartPointer<vtkRenderer>                        rightRender = vtkSmartPointer<vtkRenderer>::New();
		vtkSmartPointer<vtkRenderWindow>                    renwindow   = vtkSmartPointer<vtkRenderWindow>::New();
		vtkSmartPointer<vtkCamera>                          camera      = vtkSmartPointer<vtkCamera>::New();
		vtkSmartPointer<vtkRenderWindowInteractor>          interactor  = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		vtkSmartPointer<myCallbackFunc>                     callback    = vtkSmartPointer<myCallbackFunc>::New();
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>  style       = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		
		vtkSmartPointer<vtkRenderer> aRenderer =
			vtkSmartPointer<vtkRenderer>::New();
		aRenderer->SetViewport(0, 0.0, 0.5, 1.0);
		vtkSmartPointer<vtkRenderWindow> renWin =
			vtkSmartPointer<vtkRenderWindow>::New();
		renWin->AddRenderer(aRenderer);

		vtkSmartPointer<vtkRenderWindowInteractor> iren =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		iren->SetRenderWindow(renWin);

		std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
		vtkSmartPointer<vtkDICOMImageReader> reader =
			vtkSmartPointer<vtkDICOMImageReader>::New();
		reader->SetDirectoryName(folder.c_str());
		reader->Update();


		if (!reader->GetOutputPort())
		{
			cout << "open failed" << endl;
		}
		cout << "读取数据完成" << endl;

		vtkSmartPointer<vtkContourFilter> skinExtractor =
			vtkSmartPointer<vtkContourFilter>::New();
		skinExtractor->SetInputConnection(reader->GetOutputPort());
		skinExtractor->SetValue(0, 1);    //值越大，保留的部分越少。

		//做平滑处理
		vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
		smooth->SetInputConnection(skinExtractor->GetOutputPort());
		smooth->SetNumberOfIterations(100);

		//重新计算法向量
		vtkSmartPointer<vtkPolyDataNormals> skinNormals =
			vtkSmartPointer<vtkPolyDataNormals>::New();

		skinNormals->SetInputConnection(smooth->GetOutputPort());
		skinNormals->SetFeatureAngle(180);

		vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
			vtkSmartPointer<vtkStripper>::New();
		skinStripper->SetInputConnection(skinNormals->GetOutputPort());

		vtkSmartPointer<vtkPolyDataMapper> skinMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		skinMapper->SetInputConnection(skinStripper->GetOutputPort());
		skinMapper->ScalarVisibilityOff();    //这样不会带颜色

		vtkSmartPointer<vtkActor> skin =
			vtkSmartPointer<vtkActor>::New();
		skin->SetMapper(skinMapper);

		vtkSmartPointer<vtkOutlineFilter> outlineData =
			vtkSmartPointer<vtkOutlineFilter>::New();
		outlineData->SetInputConnection(reader->GetOutputPort());

		vtkSmartPointer<vtkPolyDataMapper> mapOutline =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapOutline->SetInputConnection(outlineData->GetOutputPort());

		vtkSmartPointer<vtkActor> outline =
			vtkSmartPointer<vtkActor>::New();
		outline->SetMapper(mapOutline);
		outline->GetProperty()->SetColor(0, 0, 0);

		vtkSmartPointer<vtkCamera> aCamera =
			vtkSmartPointer<vtkCamera>::New();
		aCamera->SetViewUp(0, 0, -1);
		aCamera->SetPosition(0, 1, 0);
		aCamera->SetFocalPoint(0, 0, 0);
		aCamera->ComputeViewPlaneNormal();
		aCamera->Azimuth(30.0);
		aCamera->Elevation(30.0);
		aCamera->Dolly(1.5);
		aRenderer->AddActor(outline);
		aRenderer->AddActor(skin);
		aRenderer->SetActiveCamera(aCamera);
		aRenderer->ResetCamera();
		aRenderer->SetBackground(.2, .3, .4);
		aRenderer->ResetCameraClippingRange();

		renWin->SetSize(1000, 1000);

		iren->SetInteractorStyle(style);

		

		/////////设置截面
		vtkSmartPointer<vtkClipPolyData> cliper = vtkClipPolyData::New();
		cliper->SetInputData(skinStripper->GetOutput());

		// 此平面box可以通过右键来进行放大缩小处理（只有当鼠标控制区域只有切割体才单一有效）
		vtkSmartPointer<vtkImplicitPlaneWidget> implicitPlaneWidget = vtkImplicitPlaneWidget::New();
		implicitPlaneWidget->SetInteractor(iren);
		implicitPlaneWidget->SetPlaceFactor(1.25);
		//initially position the widget
		implicitPlaneWidget->SetInputData(skinStripper->GetOutput());
		implicitPlaneWidget->PlaceWidget();

		//////Render2
		vtkSmartPointer<vtkActor> coneSkinActor = vtkActor::New();
		coneSkinActor->SetMapper(skinMapper);

		vtkSmartPointer<vtkRenderer> rRenderer =
			vtkSmartPointer<vtkRenderer>::New();
		rRenderer->SetBackground(0.2, 0.3, 0.5);
		rRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
		rRenderer->AddActor(coneSkinActor);

		vtkSmartPointer<BuildVTKWidgetCall> pCall = BuildVTKWidgetCall::New();
		pCall->setActor(coneSkinActor);
		pCall->setCliper(cliper);

		renWin->AddRenderer(rRenderer);
		///////

		implicitPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);
		implicitPlaneWidget->On();

		//ui->qvtkWidget_4->SetRenderWindow(renWin);		// make the link between VTK and QT
		//ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(rRenderer);
		//iren->SetRenderWindow(renWin);

		actor1->SetMapper(skinMapper);
		actor2->SetMapper(skinMapper);

		double leftViewStation[4] = { 0.0, 0.0, 0.5, 1.0 };
		double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };

		leftRender->AddActor(actor1);
		leftRender->SetBackground(.2, .3, .5);
		leftRender->SetViewport(leftViewStation);
		rightRender->AddActor(actor2);
		rightRender->SetBackground(.2, .4, .6);
		rightRender->SetViewport(rightViewStation);

		rightRender->SetActiveCamera(aCamera);
		leftRender->SetActiveCamera(aCamera);

		renWin->AddRenderer(leftRender);
		renWin->AddRenderer(rightRender);
		renWin->SetSize(600, 300);
		renWin->Render();

		callback->SetLeftRender(leftRender);
		callback->SetRightRender(rightRender);
		leftRender->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, callback);
		rightRender->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent,callback);

		iren->SetInteractorStyle(style);//用户通过控制相机对物体旋转放大、缩小等操作。 
		renWin->SetInteractor(iren);

		iren->Initialize();
		iren->Start();

		ui->qvtkWidget_4->SetRenderWindow(renWin);		// make the link between VTK and QT
		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(rRenderer);
		iren->SetRenderWindow(renWin);
		// Render
		renWin->Render();
		// Initialize the event loop and then start it.
		iren->Initialize();

		iren->Start();

}
*/

//导入数据
#include "QDir"
#include "QFileDialog"
#include "QTextCursor"
#include <iostream>

using namespace std;

//加载文件
void MainWindow::load()
{
	QString path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("view file"), QDir::currentPath()));
	ui->label_2->setText(path);
	std::string folder = path.toStdString();
}

void MainWindow::conn()
{}
////滑块
//void MainWindow::conn()
//	{
//	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
//	folder = ui->label_2->text().toStdString();
//	vtkSmartPointer<vtkDICOMImageReader> reader =
//		vtkSmartPointer<vtkDICOMImageReader>::New();
//	reader->SetDirectoryName(folder.c_str());
//	reader->Update();
//
//	vtkSmartPointer<vtkImageViewer2> imageViewer =
//		vtkSmartPointer<vtkImageViewer2>::New();
//	imageViewer->SetInputConnection(reader->GetOutputPort());
//
//	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
//	sliceTextProp->SetFontFamilyToCourier();
//	sliceTextProp->SetFontSize(20);
//	sliceTextProp->SetVerticalJustificationToBottom();
//	sliceTextProp->SetJustificationToLeft();
//	imageViewer->SetSlice(int((imageViewer->GetSliceMin() + imageViewer->GetSliceMax()) / 2));
//	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
//	std::string msg = StatusMessage::Format(int((imageViewer->GetSliceMin() + imageViewer->GetSliceMax()) / 2), imageViewer->GetSliceMax());
//	sliceTextMapper->SetInput(msg.c_str());
//	sliceTextMapper->SetTextProperty(sliceTextProp);
//
//	vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
//	sliceTextActor->SetMapper(sliceTextMapper);
//	sliceTextActor->SetPosition(15, 10);
//
//	vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
//	usageTextProp->SetFontFamilyToCourier();
//	usageTextProp->SetFontSize(14);
//	usageTextProp->SetVerticalJustificationToTop();
//	usageTextProp->SetJustificationToLeft();
//
//	vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
//	usageTextMapper->SetInput("- Slice with mouse wheel\n or Up/Down-Key\n- Zoom with pressed right\n mouse button while dragging");
//	usageTextMapper->SetTextProperty(usageTextProp);
//
//	vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
//	usageTextActor->SetMapper(usageTextMapper);
//	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
//	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);
//
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//		vtkSmartPointer<vtkRenderWindowInteractor>::New();
//
//	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle =
//		vtkSmartPointer<myVtkInteractorStyleImage>::New();
//
//	myInteractorStyle->SetImageViewer(imageViewer);
//	myInteractorStyle->SetStatusMapper(sliceTextMapper);
//
//	imageViewer->SetupInteractor(renderWindowInteractor);
//	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
//	//imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
//	imageViewer->GetRenderer()->AddActor2D(usageTextActor);
//
//
//	vtkRenderWindow*renWin = vtkRenderWindow::New();
//	renWin->AddRenderer(imageViewer->GetRenderer());
//	renWin->SetSize(1000, 1000);
//	vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();//设定交互
//	iren->SetRenderWindow(renWin);
//	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
//	iren->SetInteractorStyle(style);
//
//	vtkSliderRepresentation3D* sliderRep = vtkSliderRepresentation3D::New();
//	sliderRep->SetValue(30);
//	//滑动条两端的值，默认0-100
//	sliderRep->SetMinimumValue(0);
//	sliderRep->SetMaximumValue(453);
//	//sliderRep->SetTitleText("vessel value");
//	//text position
//	sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToWorld();
//	sliderRep->GetPoint1Coordinate()->SetValue(-100, -10, 0);
//	sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToWorld();
//	sliderRep->GetPoint2Coordinate()->SetValue(400, -10, 0);//Titletext的坐标
//
//	sliderRep->SetSliderLength(0.05);
//	sliderRep->SetSliderWidth(0.05);
//	sliderRep->SetEndCapLength(0.05);
//
//	vtkSliderWidget*sliderWidget = vtkSliderWidget::New();
//	sliderWidget->SetInteractor(iren);
//	sliderWidget->SetRepresentation(sliderRep);
//	sliderWidget->SetAnimationModeToAnimate();
//	sliderWidget->EnabledOn();//用于使用滑块
//
//	vtkSliderCallback* callback = vtkSliderCallback::New();
//	callback->vesselExtractor = imageViewer;
//	sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback);
//
//	//vtkBoxWidget*boxWidget = vtkBoxWidget::New();//创建vtkBoxWidget实例
//	//boxWidget->SetInteractor(iren);  // 设置交互器
//	//boxWidget->SetPlaceFactor(1.25); //设置缩放因子
//
//	//boxWidget->SetProp3D(coneActor);//关联一个道具
//	//boxWidget->PlaceWidget();//对位置初始化
//	////vtkMyCallback*callback = vtkMyCallback::New();
//	//boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
//
//	//boxWidget->On();
//
//	ui->qvtkWidget->SetRenderWindow(renWin);		// make the link between VTK and QT
//	ui->qvtkWidget->GetRenderWindow()->AddRenderer(imageViewer->GetRenderer());
//	// make the link between the  renderwindow and the interactor
//	iren->SetRenderWindow(ui->qvtkWidget->GetRenderWindow());
//	//ui->qvtkWidget_4->show();
//
//	// interact with data
//	renWin->Render();
//
//	iren->Initialize();
//	iren->Start();
//
//}

//滑块
#include "vtkWindowToImageFilter.h"
#include "vtkJPEGWriter.h"
#include "vtkImageCanvasSource2D.h"
#include "vtkCylinderSource.h"
#include "vtkLineSource.h"
#include "vtkTubeFilter.h"
#include <vtkSmartPointer.h>
#include <vtkPNGReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkJPEGWriter.h>
#include <vtkRenderer.h> //定义了Camera
void MainWindow::brain4()
{
	vtkSmartPointer<vtkCylinderSource> cylinderSource =
		vtkSmartPointer<vtkCylinderSource>::New();
	cylinderSource->SetHeight(10.0);
	cylinderSource->SetCenter(0.0, 0.0, 0.0);
	cylinderSource->SetRadius(2.0);
	cylinderSource->SetResolution(50);

	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(5.0, 0.0, 0.0);
	lineSource->SetPoint2(-5.0, 0.0, 0.0);
	vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
	tubeFilter->SetInputConnection(lineSource->GetOutputPort());
	tubeFilter->SetRadius(2.0);
	tubeFilter->SetNumberOfSides(50);
	tubeFilter->CappingOn();

	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cylinderMapper1->SetInputConnection(cylinderSource->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cylinderMapper2->SetInputConnection(tubeFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> cylinderActor1 =
		vtkSmartPointer<vtkActor>::New();
	cylinderActor1->SetMapper(cylinderMapper1);

	vtkSmartPointer<vtkActor> cylinderActor2 =
		vtkSmartPointer<vtkActor>::New();
	cylinderActor2->SetMapper(cylinderMapper2);

	vtkSmartPointer<vtkRenderer> renderer1 =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> renderer2 =
		vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(600, 600);
	renderWindow->AddRenderer(renderer1);
	renderWindow->AddRenderer(renderer2);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	double leftViewport[] = { 0.0, 0.0, 0.5, 1.0 };
	double rightViewport[] = { 0.5, 0.0, 1.0, 1.0 };

	cylinderActor1->GetProperty()->SetColor(0.0, 1.0, 0.0);
	cylinderActor2->GetProperty()->SetColor(0.0, 0.0, 1.0);

	renderer1->AddActor(cylinderActor1);
	renderer2->AddActor(cylinderActor2);
	renderer1->SetBackground(.3, .3, .5); // Background color purple
	renderer2->SetBackground(.2, .4, .5);
	renderer1->SetViewport(leftViewport);
	renderer2->SetViewport(rightViewport);

	std::string outputFilename = "output.jpg";

	int extent[6] = { 0, 99, 0, 99, 0, 0 };
	vtkSmartPointer<vtkImageCanvasSource2D> imageSource =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource->SetExtent(extent);
	imageSource->SetScalarTypeToUnsignedChar();
	imageSource->SetNumberOfScalarComponents(3);

	imageSource->SetDrawColor(0, 127, 255);
	imageSource->FillBox(extent[0], extent[1], extent[2], extent[3]);

	imageSource->SetDrawColor(255, 255, 255);
	imageSource->FillBox(40, 70, 20, 50);

	vtkSmartPointer<vtkJPEGWriter> writer =
		vtkSmartPointer<vtkJPEGWriter>::New();
	writer->SetFileName("output.jpg");
	writer->SetInputConnection(imageSource->GetOutputPort());
	writer->Write();
	////读取PNG图像
	//vtkSmartPointer<vtkPNGReader> pngread =
	//	vtkSmartPointer<vtkPNGReader>::New();
	//pngread->SetFileName("logo.png");
	////显示该幅图像
	//vtkSmartPointer<vtkImageViewer2> ImageViewer =
	//	vtkSmartPointer<vtkImageViewer2>::New();
	//ImageViewer->SetInputConnection(pngread->GetOutputPort());

	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInter =
	//	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//ImageViewer->SetupInteractor(renderWindowInter);
	//ImageViewer->Render();
	//ImageViewer->GetRenderer()->ResetCamera();
	//ImageViewer->Render();

	////写图像
	//vtkSmartPointer<vtkJPEGWriter> writer =
	//	vtkSmartPointer<vtkJPEGWriter>::New();
	//writer->SetFileName("VTK-logo.jpg");
	//writer->SetInputConnection(pngread->GetOutputPort());
	//writer->Write();

	//renderWindowInter->Start();

	//auto wif =vtkSmartPointer<vtkWindowToImageFilter>::New();
	//wif->SetInput(renderWindow);

	//auto writer = vtkSmartPointer<vtkJPEGWriter>::New();
	//writer->SetFileName(outputFilename.c_str());
	////writer->SetWriteToMemory(1);
	//writer->SetInputConnection(cylinderMapper2->GetOutputPort());
	//writer->Write();
	//auto array = writer->GetResult();

	//auto bytes = new byte[array->getnumberoftuples()];
	//for (int i = 0; i < array->getnumberoftuples(); ++i)
	//{
	//	bytes[i] = (byte)array->gettuple1(i);
	//}

	//folder = ui->label_2->text().toStdString();

	//SocketSend(bytes);

	//auto folder = bytes.tostdstring();
	//QString qstr2 = QString::fromstdstring(bytes);
	//SocketSend(bytes);

	renderWindow->Render();
	renderWindowInteractor->Start();

}
void MainWindow::on_pushButton_2_clicked() {}
void MainWindow::on_pushButton_3_clicked() {}
void MainWindow::on_pushButton_4_clicked() {}
void MainWindow::on_pushButton_5_clicked() {}
void MainWindow::on_pushButton_6_clicked() {}
void MainWindow::on_pushButton_7_clicked() {}
void MainWindow::on_pushButton_8_clicked() {}
void MainWindow::on_pushButton_9_clicked() {}
void MainWindow::on_pushButton_clicked() {}


#include <windows.h>
#include <stdio.h>
//#include <stdlib.h>
#include <stdarg.h>

#define IS_USE_OUTPUT_DEBUG_PRINT   1

#if  IS_USE_OUTPUT_DEBUG_PRINT 

#define  OUTPUT_DEBUG_PRINTF(str)  OutputDebugPrintf(str)
void OutputDebugPrintf(const char * strOutputString, ...)
{
#define PUT_PUT_DEBUG_BUF_LEN   1024
	char strBuffer[PUT_PUT_DEBUG_BUF_LEN] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);  //_vsnprintf_s  _vsnprintf
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	OutputDebugStringA(strBuffer);  //OutputDebugString    // OutputDebugStringW

}
#else 
#define  OUTPUT_DEBUG_PRINTF(str) 
#endif

// 二维XY
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
using PixelType = float;
constexpr unsigned int Dimension = 3;
using ImageType = itk::Image< PixelType, Dimension >;

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    $RCSfile: otherCoordinate.cxx,v $
  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
	 This software is distributed WITHOUT ANY WARRANTY; without even
	 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	 PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
// .NAME 
// .SECTION Description
// this program tests vtkCoordinate
#include "iostream"
#include "string"
using namespace std;
#include "vtkCoordinate.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDebugLeaks.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
void ToAll(vtkCoordinate *c1, vtkViewport *ren1,
	double *from)
{
	double *value;
	int *ivalue;
	const char *whichCoord = c1->GetCoordinateSystemAsString();
	c1->SetValue(from);
	cout << "========";
	cout << *c1;
	value = c1->GetComputedWorldValue(ren1);
	cout << whichCoord << "(" << from[0] << ", " << from[1] << ", " << from[2]
		<< ") -> World(" << value[0] << ", " << value[1] << ", " << value[2]
		<< ")";
	OutputDebugPrintf("\nDEBUG_INFO | (%lf %lf %lf)-> World(%lf %lf %lf)", from[0], from[1], from[2], value[0], value[1], value[2]);

	ivalue = c1->GetComputedDisplayValue(ren1);
	cout << whichCoord << "(" << from[0] << ", " << from[1] << ", " << from[2]
		<< ") -> Display(" << ivalue[0] << ", " << ivalue[1] << ")";
	OutputDebugPrintf("\nDEBUG_INFO | (%lf %lf %lf)-> Display(%lf %lf)", from[0], from[1], from[2], ivalue[0], ivalue[1]);

	ivalue = c1->GetComputedLocalDisplayValue(ren1);
	cout << whichCoord << "(" << from[0] << ", " << from[1] << ", " << from[2]
		<< ") -> LocalDisplay(" << ivalue[0] << ", " << ivalue[1]
		<< ")";
	OutputDebugPrintf("\nDEBUG_INFO | (%lf %lf %lf)-> LocalDisplay(%lf %lf)", from[0], from[1], from[2], ivalue[0], ivalue[1]);

	ivalue = c1->GetComputedViewportValue(ren1);
	cout << whichCoord << "(" << from[0] << ", " << from[1] << ", " << from[2]
		<< ") -> Viewport(" << ivalue[0] << ", " << ivalue[1] << ")";
	OutputDebugPrintf("\nDEBUG_INFO | (%lf %lf %lf)-> Viewport(%lf %lf)", from[0], from[1], from[2], ivalue[0], ivalue[1]);
}

//
//void MainWindow::brain1()
//{
//	// actual test
//	cout << "Testing vtkCoordinate";
//	vtkCoordinate *c1 = vtkCoordinate::New();
//	vtkCoordinate *c2 = vtkCoordinate::New();
//	vtkRenderWindow *renWin = vtkRenderWindow::New();
//	vtkRenderer *ren1 = vtkRenderer::New();
//	renWin->AddRenderer(ren1);
//	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
//	iren->SetRenderWindow(renWin);
//	vtkSmartPointer<vtkLineSource> lineSource =
//		vtkSmartPointer<vtkLineSource>::New();
//	lineSource->SetPoint1(0,0,0);
//	lineSource->SetPoint2(10,10,10);
//	vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
//	tubeFilter->SetInputConnection(lineSource->GetOutputPort());
//	tubeFilter->SetRadius(1);
//	tubeFilter->SetNumberOfSides(100);
//	tubeFilter->CappingOn();
//	//vtkSphereSource *sphere = vtkSphereSource::New();
//	//sphere->SetThetaResolution(20);
//	//sphere->SetPhiResolution(20);
//	vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
//	sphereMapper->SetInputConnection(tubeFilter->GetOutputPort());
//	vtkActor *sphereActor = vtkActor::New();
//	sphereActor->SetMapper(sphereMapper);
//	ren1->AddActor(sphereActor);
//	vtkCamera *camera = vtkCamera::New();
//	double from[3];
//
//	vtkSmartPointer<vtkAxesActor> actor2 =
//		vtkSmartPointer<vtkAxesActor>::New();
//	actor2->SetPosition(0, 0, 0);
//	actor2->SetTotalLength(100, 100, 100);
//	actor2->SetShaftType(0);
//	actor2->SetAxisLabels(0);
//	actor2->SetCylinderRadius(0.02);
//
//	//ren1->AddActor(actor2);
//	//ren1->SetActiveCamera(camera);
//	renWin->AddRenderer(ren1);
//	renWin->SetSize(500, 500);
//
//
//	cout << "Origin: (" << ren1->GetOrigin()[0] << ", " << ren1->GetOrigin()[1] << ")";
//	cout << "Center: (" << ren1->GetCenter()[0] << ", " << ren1->GetOrigin()[1] << ")";
//	OutputDebugPrintf("\nDEBUG_INFO | Origin: (%d %d)    Center:(%d %d)", ren1->GetOrigin()[0], ren1->GetOrigin()[1], ren1->GetOrigin()[0], ren1->GetOrigin()[1]);
//
//	cout <<"********** A NULL Viewport **********";
//
//	//c1->SetCoordinateSystemToWorld();
//	//from[0] = 0.0; from[1] = 0.0; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToDisplay();
//	//from[0] = 50; from[1] = 50; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToNormalizedDisplay();
//	//from[0] = .5; from[1] = .5; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToViewport();
//	//from[0] = 50; from[1] = 50; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToNormalizedViewport();
//	//from[0] = .5; from[1] = .5; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//
//	//c1->SetCoordinateSystemToView();
//	//from[0] = 0.0; from[1] = 0.0; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//
//	//cout <<"********** A specified Viewport **********";
//	//c1->SetViewport(ren1);
//
//	//c1->SetCoordinateSystemToWorld();
//	//from[0] = 0.0; from[1] = 0.0; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//	//c1->SetCoordinateSystemToDisplay();
//	//from[0] = 50; from[1] = 50; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToNormalizedDisplay();
//	//from[0] = .5; from[1] = .5; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToViewport();
//	//from[0] = 50; from[1] = 50; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToNormalizedViewport();
//	//from[0] = .5; from[1] = .5; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToView();
//	//from[0] = 0.0; from[1] = 0.0; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//
//	//cout << "********** With a Reference Coordinate **********";
//	//c2->SetCoordinateSystemToNormalizedDisplay();
//	////c2->SetCoordinateSystemToWorld();
//	//c2->SetValue(0.5, 0.5, 0.0);
//	////OutputDebugPrintf("\nDEBUG_INFO | c2: (%d %d %d)", c2[0], c2[1], c2[2]);
//	//c1->SetReferenceCoordinate(c2);
//	//cout << *c2;
//
//	//c1->SetCoordinateSystemToWorld();
//	//from[0] = 0.0; from[1] = 0.0; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//	//c1->SetCoordinateSystemToDisplay();
//	//from[0] = 50; from[1] = 50; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//
//	//c1->SetCoordinateSystemToNormalizedDisplay();
//	//from[0] = .5; from[1] = .5; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToViewport();
//	//from[0] = 50; from[1] = 50; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToNormalizedViewport();
//	//from[0] = .5; from[1] = .5; from[2] = 0;
//	//ToAll(c1, ren1, from);
//
//	//c1->SetCoordinateSystemToView();
//	//from[0] = 0.0; from[1] = 0.0; from[2] = 0.0;
//	//ToAll(c1, ren1, from);
//
//	renWin->Render();
//	iren->Start();
//
//	c1->Delete();
//	c2->Delete();
//	renWin->Delete();
//	ren1->Delete();
//	camera->Delete();
//	cout << "Testing completed";
//}

//横断面
void MainWindow::brain1()
{
	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";

	//folder = ui->label_2->text().toStdString();
	//vtkSmartPointer<vtkDICOMImageReader> reader =
	//	vtkSmartPointer<vtkDICOMImageReader>::New();
	//reader->SetDirectoryName(folder.c_str());
	//reader->Update();
	folder = ui->label_2->text().toStdString();
	using ReaderType = itk::ImageSeriesReader< ImageType >;
	using ImageIOType = itk::GDCMImageIO;
	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO(dicomIO);
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->SetDirectory(folder.c_str());
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\20190327\\17240001");//膝盖
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\Dog_Ti_Mark\\CT");//狗腿
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\Xi_CT");//腿
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\kouqiang");//口腔
	using SeriesIdContainer = std::vector< std::string >;
	const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
	auto seriesItr = seriesUID.begin();
	auto seriesEnd = seriesUID.end();

	using FileNamesContainer = std::vector< std::string >;
	FileNamesContainer fileNames;
	std::string seriesIdentifier;
	while (seriesItr != seriesEnd)
	{
		seriesIdentifier = seriesItr->c_str();
		fileNames = nameGenerator->GetFileNames(seriesIdentifier);
		++seriesItr;
	}
	reader->SetFileNames(fileNames);
	reader->Update();

	ImageType::SizeType imgSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
	//cout << "read done！Original size: " << imgSize << endl;

	typedef itk::ImageToVTKImageFilter< ImageType> itkTovtkFilterType;
	itkTovtkFilterType::Pointer itkTovtkImageFilter = itkTovtkFilterType::New();
	itkTovtkImageFilter->SetInput(reader->GetOutput());
	itkTovtkImageFilter->Update();


	//vtkSmartPointer<vtkImageData> imgDataV = itkTovtkImageFilter->GetOutput();
	//vtkSmartPointer<vtkImageThreshold> thresholdFilter = vtkSmartPointer<vtkImageThreshold>::New();
	//thresholdFilter->SetInputData(imgDataV);
	//thresholdFilter->ThresholdBetween(62, 128);
	////thresholdFilter->ReplaceInOn();//阈值内的像素值替换
	////thresholdFilter->ReplaceOutOn();//阈值外的像素值替换
	////thresholdFilter->SetInValue(1);//阈值内像素值全部替换成1
	//thresholdFilter->SetOutValue(0);//阈值外像素值全部替换成0
	//thresholdFilter->Update();

	vtkSmartPointer<vtkImageViewer2> imageViewer =
		vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputData(itkTovtkImageFilter->GetOutput());
	imageViewer->SetSize(1, 1);
	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	imageViewer->SetSlice(int((imageViewer->GetSliceMin() + imageViewer->GetSliceMax()) / 2));

	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(int((imageViewer->GetSliceMin()+ imageViewer->GetSliceMax())/2), imageViewer->GetSliceMax());
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);
	//sliceTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	//sliceTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);

	vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput("- Slice with mouse wheel\n or Up/Down-Key\n- Zoom with pressed right\n mouse button while dragging");
	usageTextMapper->SetTextProperty(usageTextProp);

	vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle =
		vtkSmartPointer<myVtkInteractorStyleImage>::New();

	myInteractorStyle->SetImageViewer(imageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);
	
	imageViewer->SetupInteractor(renderWindowInteractor);
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	imageViewer->GetRenderer()->SetBackground(0.15, 0.15, 0.15);
	imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	imageViewer->GetRenderer()->AddActor2D(usageTextActor);
	//ui->horizontalSlider_6->setMinimum(imageViewer->GetSliceMin());
	//ui->horizontalSlider_6->setMaximum(imageViewer->GetSliceMax());
	//ui->horizontalSlider_6->setValue(int((imageViewer->GetSliceMin() + imageViewer->GetSliceMax()) / 2));
	
	ui->qvtkWidget->SetRenderWindow(imageViewer->GetRenderWindow());		// make the link between VTK and QT
	ui->qvtkWidget->GetRenderWindow()->AddRenderer(imageViewer->GetRenderer());
	// make the link between the  renderwindow and the interactor
	//renderWindowInteractor->SetRenderWindow(imageViewer->GetRenderWindow());

	//vtkRenderWindow*renWin = vtkRenderWindow::New();
	//renWin->AddRenderer(imageViewer->GetRenderer());
	////renWin->AddRenderer(ren2);
	//renWin->SetSize(600, 300);
	//vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();//设定交互
	//iren->SetRenderWindow(renWin);
	//vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	//iren->SetInteractorStyle(style);//指定交互方式

	

	//显示在窗体
	//ui->qvtkWidget->GetRenderWindow()->AddRenderer(imageViewer->GetRenderer());
	//ui->qvtkWidget->show();
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	renderWindowInteractor->Start();
}


//二维YZ
void MainWindow::brain2()
{
	folder = ui->label_2->text().toStdString();
	using ReaderType = itk::ImageSeriesReader< ImageType >;
	using ImageIOType = itk::GDCMImageIO;
	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO(dicomIO);
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->SetDirectory(folder.c_str());
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\20190327\\17240001");//膝盖
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\Dog_Ti_Mark\\CT");//狗腿
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\Xi_CT");//腿
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\kouqiang");//口腔
	using SeriesIdContainer = std::vector< std::string >;
	const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
	auto seriesItr = seriesUID.begin();
	auto seriesEnd = seriesUID.end();

	using FileNamesContainer = std::vector< std::string >;
	FileNamesContainer fileNames;
	std::string seriesIdentifier;
	while (seriesItr != seriesEnd)
	{
		seriesIdentifier = seriesItr->c_str();
		fileNames = nameGenerator->GetFileNames(seriesIdentifier);
		++seriesItr;
	}
	reader->SetFileNames(fileNames);
	reader->Update();

	ImageType::SizeType imgSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
	//cout << "read done！Original size: " << imgSize << endl;

	typedef itk::ImageToVTKImageFilter< ImageType> itkTovtkFilterType;
	itkTovtkFilterType::Pointer itkTovtkImageFilter = itkTovtkFilterType::New();
	itkTovtkImageFilter->SetInput(reader->GetOutput());
	itkTovtkImageFilter->Update();

	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";

	//folder = ui->label_2->text().toStdString();
	//vtkSmartPointer<vtkDICOMImageReader> reader =
	//	vtkSmartPointer<vtkDICOMImageReader>::New();
	//reader->SetDirectoryName(folder.c_str());
	//reader->Update();

	vtkSmartPointer<vtkImageViewer2> imageViewer =
		vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetSize(1, 1);
	//auto bounds = reader->GetOutput()->GetBounds();
	auto bounds = itkTovtkImageFilter->GetOutput()->GetBounds();
	double center[] = { (bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, (bounds[5] + bounds[4]) / 2.0 };

	//vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
	//trans->Translate(center[0], center[1], center[2]);
	//trans->RotateX(180);
	//trans->Translate(-center[0], -center[1], -center[2]);

	auto vir = vtkImageReslice::New();
	//vir->SetInputConnection(reader->GetOutputPort());
	vir->SetInputData(itkTovtkImageFilter->GetOutput());
	//vir->SetResliceTransform(trans);
	vir->SetInterpolationModeToCubic();
	vir->SetOutputSpacing(reader->GetOutput()->GetSpacing()[0],
		reader->GetOutput()->GetSpacing()[1],
		reader->GetOutput()->GetSpacing()[2]);
	vir->SetOutputOrigin(reader->GetOutput()->GetOrigin()[0],
		reader->GetOutput()->GetOrigin()[1],
		reader->GetOutput()->GetOrigin()[2]);
	//auto a = reader->GetOutput()->GetExtent();
	auto a = itkTovtkImageFilter->GetOutput()->GetExtent();
	vir->SetOutputExtent(a[0], a[1], a[2], a[3], a[4], a[5]);

	imageViewer->SetInputConnection(vir->GetOutputPort());
	imageViewer->SetSliceOrientationToYZ();


	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(int((imageViewer->GetSliceMin() + imageViewer->GetSliceMax()) / 2), imageViewer->GetSliceMax());
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);

	vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput("- Slice with mouse wheel\n or Up/Down-Key\n- Zoom with pressed right\n mouse button while dragging");
	usageTextMapper->SetTextProperty(usageTextProp);

	vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle =
		vtkSmartPointer<myVtkInteractorStyleImage>::New();

	myInteractorStyle->SetImageViewer(imageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);

	imageViewer->SetupInteractor(renderWindowInteractor);
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	imageViewer->GetRenderer()->SetBackground(0.15, 0.15, 0.15);
	imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	imageViewer->GetRenderer()->AddActor2D(usageTextActor);


	ui->qvtkWidget_2->SetRenderWindow(imageViewer->GetRenderWindow());		// make the link between VTK and QT
	ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(imageViewer->GetRenderer());
	// make the link between the  renderwindow and the interactor
	renderWindowInteractor->SetRenderWindow(imageViewer->GetRenderWindow());

	ui->qvtkWidget->show();
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	renderWindowInteractor->Start();

}

//xy方向的滑块
void MainWindow::change1()
{
/*
	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
	//folder = ui->label_2->text().toStdString();
	//vtkSmartPointer<vtkDICOMImageReader> reader =
	//	vtkSmartPointer<vtkDICOMImageReader>::New();
	//reader->SetDirectoryName(folder.c_str());
	//reader->Update();

	folder = ui->label_2->text().toStdString();
	using ReaderType = itk::ImageSeriesReader< ImageType >;
	using ImageIOType = itk::GDCMImageIO;
	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO(dicomIO);
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->SetDirectory(folder.c_str());
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\20190327\\17240001");//膝盖
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\Dog_Ti_Mark\\CT");//狗腿
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\Xi_CT");//腿
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\kouqiang");//口腔
	using SeriesIdContainer = std::vector< std::string >;
	const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
	auto seriesItr = seriesUID.begin();
	auto seriesEnd = seriesUID.end();

	using FileNamesContainer = std::vector< std::string >;
	FileNamesContainer fileNames;
	std::string seriesIdentifier;
	while (seriesItr != seriesEnd)
	{
		seriesIdentifier = seriesItr->c_str();
		fileNames = nameGenerator->GetFileNames(seriesIdentifier);
		++seriesItr;
	}
	reader->SetFileNames(fileNames);
	reader->Update();

	ImageType::SizeType imgSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
	//cout << "read done！Original size: " << imgSize << endl;

	typedef itk::ImageToVTKImageFilter< ImageType> itkTovtkFilterType;
	itkTovtkFilterType::Pointer itkTovtkImageFilter = itkTovtkFilterType::New();
	itkTovtkImageFilter->SetInput(reader->GetOutput());
	itkTovtkImageFilter->Update();

	vtkSmartPointer<vtkImageViewer2> imageViewer =
		vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputData(itkTovtkImageFilter->GetOutput());
	imageViewer->SetSize(1, 1);
	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	int value = ui->horizontalSlider_6->value();
	ui->horizontalSlider_6->setValue(value);

	//imageViewer->SetSlice(int((imageViewer->GetSliceMin() + imageViewer->GetSliceMax()) / 2));
	imageViewer->SetSlice(value);

	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(int(value), imageViewer->GetSliceMax());
	//std::string msg = StatusMessage::Format(int((imageViewer->GetSliceMin()+ imageViewer->GetSliceMax())/2), imageViewer->GetSliceMax());
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);

	vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput("- Slice with mouse wheel\n or Up/Down-Key\n- Zoom with pressed right\n mouse button while dragging");
	usageTextMapper->SetTextProperty(usageTextProp);

	vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle =
		vtkSmartPointer<myVtkInteractorStyleImage>::New();

	myInteractorStyle->SetImageViewer(imageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);

	imageViewer->SetupInteractor(renderWindowInteractor);
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	imageViewer->GetRenderer()->AddActor2D(usageTextActor);
	ui->horizontalSlider_6->setMinimum(imageViewer->GetSliceMin());
	ui->horizontalSlider_6->setMaximum(imageViewer->GetSliceMax());
	//ui->horizontalSlider_6->setValue(int((imageViewer->GetSliceMin() + imageViewer->GetSliceMax()) / 2));

	ui->qvtkWidget->SetRenderWindow(imageViewer->GetRenderWindow());		// make the link between VTK and QT
	ui->qvtkWidget->GetRenderWindow()->AddRenderer(imageViewer->GetRenderer());
	// make the link between the  renderwindow and the interactor

	//imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	//imageViewer->Render();
	renderWindowInteractor->Start();
	*/
}

//二维XZ
void MainWindow::brain3()
{
	folder = ui->label_2->text().toStdString();
	using ReaderType = itk::ImageSeriesReader< ImageType >;
	using ImageIOType = itk::GDCMImageIO;
	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO(dicomIO);
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->SetDirectory(folder.c_str());
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\20190327\\17240001");//膝盖
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\Dog_Ti_Mark\\CT");//狗腿
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\Xi_CT");//腿
	//nameGenerator->SetDirectory("E:\\NK\\04_data\\kouqiang");//口腔
	using SeriesIdContainer = std::vector< std::string >;
	const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
	auto seriesItr = seriesUID.begin();
	auto seriesEnd = seriesUID.end();

	using FileNamesContainer = std::vector< std::string >;
	FileNamesContainer fileNames;
	std::string seriesIdentifier;
	while (seriesItr != seriesEnd)
	{
		seriesIdentifier = seriesItr->c_str();
		fileNames = nameGenerator->GetFileNames(seriesIdentifier);
		++seriesItr;
	}
	reader->SetFileNames(fileNames);
	reader->Update();

	ImageType::SizeType imgSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
	//cout << "read done！Original size: " << imgSize << endl;

	typedef itk::ImageToVTKImageFilter< ImageType> itkTovtkFilterType;
	itkTovtkFilterType::Pointer itkTovtkImageFilter = itkTovtkFilterType::New();
	itkTovtkImageFilter->SetInput(reader->GetOutput());
	itkTovtkImageFilter->Update();

	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";

	//folder = ui->label_2->text().toStdString();
	//vtkSmartPointer<vtkDICOMImageReader> reader =
	//	vtkSmartPointer<vtkDICOMImageReader>::New();
	//reader->SetDirectoryName(folder.c_str());
	//reader->Update();

	vtkSmartPointer<vtkImageViewer2> imageViewer =
		vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetSize(1, 1);
	//auto bounds = reader->GetOutput()->GetBounds();
	auto bounds = itkTovtkImageFilter->GetOutput()->GetBounds();
	double center[] = { (bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, (bounds[5] + bounds[4]) / 2.0 };

	//vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
	//trans->Translate(center[0], center[1], center[2]);
	//trans->RotateY(180);
	//trans->Translate(-center[0], -center[1], -center[2]);

	auto vir = vtkImageReslice::New();
	//vir->SetInputConnection(reader->GetOutputPort());
	vir->SetInputData(itkTovtkImageFilter->GetOutput());
	//vir->SetResliceTransform(trans);
	vir->SetInterpolationModeToCubic();
	vir->SetOutputSpacing(reader->GetOutput()->GetSpacing()[0],
		reader->GetOutput()->GetSpacing()[1],
		reader->GetOutput()->GetSpacing()[2]);
	vir->SetOutputOrigin(reader->GetOutput()->GetOrigin()[0],
		reader->GetOutput()->GetOrigin()[1],
		reader->GetOutput()->GetOrigin()[2]);
	//auto a = reader->GetOutput()->GetExtent();
	auto a = itkTovtkImageFilter->GetOutput()->GetExtent();
	vir->SetOutputExtent(a[0], a[1], a[2], a[3], a[4], a[5]);

	imageViewer->SetInputConnection(vir->GetOutputPort());
	imageViewer->SetSliceOrientationToXZ();


	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(int((imageViewer->GetSliceMin() + imageViewer->GetSliceMax()) / 2), imageViewer->GetSliceMax());
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);

	vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput("- Slice with mouse wheel\n or Up/Down-Key\n- Zoom with pressed right\n mouse button while dragging");
	usageTextMapper->SetTextProperty(usageTextProp);

	vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle =
		vtkSmartPointer<myVtkInteractorStyleImage>::New();

	myInteractorStyle->SetImageViewer(imageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);

	imageViewer->SetupInteractor(renderWindowInteractor);
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	imageViewer->GetRenderer()->SetBackground(0.15, 0.15, 0.15);
	imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	imageViewer->GetRenderer()->AddActor2D(usageTextActor);


	ui->qvtkWidget_3->SetRenderWindow(imageViewer->GetRenderWindow());		// make the link between VTK and QT
	ui->qvtkWidget_3->GetRenderWindow()->AddRenderer(imageViewer->GetRenderer());
	// make the link between the  renderwindow and the interactor
	renderWindowInteractor->SetRenderWindow(imageViewer->GetRenderWindow());

	ui->qvtkWidget->show();
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	renderWindowInteractor->Start();

}

//3D联动
static vtkSmartPointer<vtkRenderer> renderer01;
static vtkSmartPointer<vtkRenderer> renderer02;
static vtkSmartPointer<vtkRenderer> renderer03;
static vtkSmartPointer<vtkRenderWindow> renderWindow01;
static vtkSmartPointer<vtkRenderWindow> renderWindow02;

int dist = 0;
class vtkMyCameraCallback : public vtkCommand
{
public:
	static vtkMyCameraCallback *New()
	{
		return new vtkMyCameraCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		auto cam = renderer01->GetActiveCamera();
		auto lcam = renderer02->GetActiveCamera();
		auto rcam = renderer03->GetActiveCamera();
		auto p = cam->GetPosition();
		auto f = cam->GetFocalPoint();
		auto v = cam->GetViewUp();

		lcam->SetViewUp(v[0], v[1], v[2]);
		lcam->SetPosition(p[0], p[1], p[2]);
		lcam->SetFocalPoint(f[0], f[1], f[2]);

		rcam->SetViewUp(v[0], v[1], v[2]);
		rcam->SetFocalPoint(f[0], f[1], f[2]);
		rcam->SetPosition(p[0], p[1], p[2]);
		
		#include <math.h>
		
		//以下为计算右眼相机移动距离
		auto n = cam->GetViewPlaneNormal();
		double l[3] = { v[1] * n[2] - v[2] * n[1], v[2] * n[0] - v[0] * n[2], v[0] * n[1] - v[1] * n[0] };
		auto na = sqrt(l[0] * l[0] + l[1] * l[1] + l[2] * l[2]);
		cout << dist;
		l[0] /= (na / (dist / 100.0));
		l[1] /= (na / (dist / 100.0));
		l[2] /= (na / (dist / 100.0));

		rcam->SetPosition(p[0] + l[0], p[1] + l[1], p[2] + l[2]);

		vtkCamera *camera = static_cast<vtkCamera*>(caller);
		double *camPos;
		double *focalPos;
		double *upVector;
		camPos = camera->GetPosition();
		focalPos = camera->GetFocalPoint();
		upVector = camera->GetViewUp();

		//camera->SetFocalPoint(focalPos[0], focalPos[1], focalPos[2]+100);

		//遍历所有renderer，都设置一遍最新的相机
		renderer01->SetActiveCamera(camera);
		renderer02->SetActiveCamera(camera);
		renderer03->SetActiveCamera(rcam);
		renderWindow01->Render();
		renderWindow02->Render();
	}
	vtkSmartPointer<vtkRenderer> m_renderer;
};


//改变双眼距离
void MainWindow::change2()
{
	//一种渲染
	//ui->horizontalSlider-> = 2;
	ui->horizontalSlider->setMinimum(-2000);
	ui->horizontalSlider->setMaximum(10000);
	//dist = ui->horizontalSlider->value();

	//test3(dist);//重新渲染
	test3();//重新渲染
	/*
	
	vtkVolume *volume = vtkVolume::New();
	vtkRenderWindow*renWin = vtkRenderWindow::New();
	vtkRenderWindow*renWin2 = vtkRenderWindow::New();
	vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();//设定交互
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	if (ui->comboBox->currentIndex() == 0)
	{
		int count = 1;
		const char *dirname = folder.data();
		double opacityWindow = 4096;
		double opacityLevel = 2048;
		int blendType = 0;
		int clip = 0;
		double reductionFactor = 1.0;
		double frameRate = 10.0;
		char *fileName = 0;
		int fileType = 0;

		bool independentComponents = true;

		// Create the renderer, render window and interactor
		vtkRenderer *renderer = vtkRenderer::New();
		vtkRenderWindow *renWin = vtkRenderWindow::New();
		renWin->AddRenderer(renderer);

		// Connect it all. Note that funny arithematic on the 
		// SetDesiredUpdateRate - the vtkRenderWindow divides it
		// allocated time across all renderers, and the renderer
		// divides it time across all props. If clip is
		// true then there are two props
		vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
		iren->SetRenderWindow(renWin);
		iren->SetDesiredUpdateRate(frameRate / (1 + clip));

		iren->GetInteractorStyle()->SetDefaultRenderer(renderer);

		// Read the data
		vtkAlgorithm *reader = 0;
		vtkImageData *input = 0;

		vtkDICOMImageReader *dicomReader = vtkDICOMImageReader::New();
		dicomReader->SetDirectoryName(dirname);
		dicomReader->Update();
		input = dicomReader->GetOutput();
		reader = dicomReader;


		// Verify that we actually have a volume
		int dim[3];
		input->GetDimensions(dim);
		if (dim[0] < 2 ||
			dim[1] < 2 ||
			dim[2] < 2)
		{
			cout << "Error loading data!" << endl;
			exit(EXIT_FAILURE);
		}

		vtkImageResample *resample = vtkImageResample::New();
		if (reductionFactor < 1.0)
		{
			resample->SetInputConnection(reader->GetOutputPort());
			resample->SetAxisMagnificationFactor(0, reductionFactor);
			resample->SetAxisMagnificationFactor(1, reductionFactor);
			resample->SetAxisMagnificationFactor(2, reductionFactor);
		}

		// Create our volume and mapper

		vtkGPUVolumeRayCastMapper *mapper = vtkGPUVolumeRayCastMapper::New();

		// Add a box widget if the clip option was selected
		vtkBoxWidget *box = vtkBoxWidget::New();
		if (clip)
		{
			box->SetInteractor(iren);
			box->SetPlaceFactor(1.01);

			box->SetDefaultRenderer(renderer);
			box->InsideOutOn();
			box->PlaceWidget();
			vtkBoxWidgetCallback *callback = vtkBoxWidgetCallback::New();
			callback->SetMapper(mapper);
			box->AddObserver(vtkCommand::InteractionEvent, callback);
			callback->Delete();
			box->EnabledOn();
			box->GetSelectedFaceProperty()->SetOpacity(0.0);
		}

		if (reductionFactor < 1.0)
		{
			mapper->SetInputConnection(resample->GetOutputPort());
		}
		else
		{
			mapper->SetInputConnection(reader->GetOutputPort());
		}


		// Set the sample distance on the ray to be 1/2 the average spacing
		double spacing[3];
		if (reductionFactor < 1.0)
		{
			resample->GetOutput()->GetSpacing(spacing);
		}
		else
		{
			input->GetSpacing(spacing);
		}

		//  mapper->SetSampleDistance( (spacing[0]+spacing[1]+spacing[2])/6.0 );
		//  mapper->SetMaximumImageSampleDistance(10.0);


		  // Create our transfer function
		vtkColorTransferFunction *colorFun = vtkColorTransferFunction::New();
		vtkPiecewiseFunction *opacityFun = vtkPiecewiseFunction::New();

		// Create the property and attach the transfer functions
		vtkVolumeProperty *property = vtkVolumeProperty::New();
		property->SetIndependentComponents(independentComponents);
		property->SetColor(colorFun);
		property->SetScalarOpacity(opacityFun);
		property->SetInterpolationTypeToLinear();

		// connect up the volume to the property and the mapper
		volume->SetProperty(property);
		volume->SetMapper(mapper);

		// Depending on the blend type selected as a command line option, 
		// adjust the transfer function
		switch (blendType)
		{
		case 0:
			colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0);
			opacityFun->AddSegment(opacityLevel - 0.5*opacityWindow, 0.0,
				opacityLevel + 0.5*opacityWindow, 1.0);
			mapper->SetBlendModeToMaximumIntensity();
			break;
		default:
			vtkGenericWarningMacro("Unknown blend type.");
			break;
		}

		if (!mapper->IsRenderSupported(renWin, property))
		{
			cout << "This mapper is unsupported on this platform" << endl;
			exit(EXIT_FAILURE);
		}
	}

	//一种渲染
	else
	{
		//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
		folder = ui->label_2->text().toStdString();
		vtkSmartPointer<vtkDICOMImageReader> reader =
			vtkSmartPointer<vtkDICOMImageReader>::New();
		reader->SetDirectoryName(folder.c_str());
		reader->Update();

		volumeMapper->SetInputData(reader->GetOutput());;
		volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 2);	//设置光线采样距离
		//volumeMapper->SetAutoAdjustSampleDistances(0);//设置图像采样步长
		//volumeMapper->SetImageSampleDistance(4);


		volumeProperty->SetInterpolationTypeToLinear();
		volumeProperty->ShadeOn();  //打开或者关闭阴影测试
		volumeProperty->SetAmbient(0.4);
		volumeProperty->SetDiffuse(0.6);  //漫反射
		volumeProperty->SetSpecular(0.2); //镜面反射
		//设置不透明度
		vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
			vtkSmartPointer<vtkPiecewiseFunction>::New();
		compositeOpacity->AddPoint(70, 0.00);
		compositeOpacity->AddPoint(90, 0.40);
		compositeOpacity->AddPoint(180, 0.60);
		volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数
		//设置梯度不透明属性
		vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
			vtkSmartPointer<vtkPiecewiseFunction>::New();
		volumeGradientOpacity->AddPoint(10, 0.0);
		volumeGradientOpacity->AddPoint(90, 0.5);
		volumeGradientOpacity->AddPoint(100, 1.0);
		volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比
		//设置颜色属性
		vtkSmartPointer<vtkColorTransferFunction> color =
			vtkSmartPointer<vtkColorTransferFunction>::New();
		color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
		color->AddRGBPoint(64.00, 1.00, 0.52, 0.30);
		color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
		color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
		volumeProperty->SetColor(color);

		volume->SetMapper(volumeMapper);
		volume->SetProperty(volumeProperty);
		//for (int i = 500; i >= -110; i--)
		//{
		//	auto arr = volumeMapper->GetCroppingRegionPlanes();
		//	arr[4] = i / 2.0;
		//	volumeMapper->SetCroppingRegionPlanes(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
		//	renWin->Render();
		//	Sleep(10);
	}
	
	vtkSmartPointer<vtkTransform> trans =
		vtkSmartPointer<vtkTransform>::New();
	trans->PostMultiply();
	trans->Translate(1, 0, 0);
	trans->RotateX(90);
	volume->SetUserTransform(trans);

	if (1)
	{
		double leftViewStation[4] = { 0.0, 0.0, 0.5, 1.0 };
		double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };

		renderer01 = vtkSmartPointer<vtkRenderer>::New();
		renderer02 = vtkSmartPointer<vtkRenderer>::New();
		renderer03 = vtkSmartPointer<vtkRenderer>::New();

		//渲染时使用同样的数据
		renderer01->AddVolume(volume);
		renderer02->AddVolume(volume);
		//renderer02->SetBackground(.2, .3, .5);
		renderer02->SetViewport(leftViewStation);
		renderer03->AddVolume(volume);
		//renderer03->SetBackground(.2, .3, .5);
		renderer03->SetViewport(rightViewStation);

		renderWindow01 = vtkSmartPointer<vtkRenderWindow>::New();
		renderWindow02 = vtkSmartPointer<vtkRenderWindow>::New();

		renderWindow01->AddRenderer(renderer01);
		renderWindow02->AddRenderer(renderer02);
		renderWindow02->AddRenderer(renderer03);
		vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		//windowInteractor01->SetRenderWindow(renderWindow01);
		windowInteractor02->SetRenderWindow(renderWindow02);



		
		auto oriPosCam1 = renderer02->GetActiveCamera()->GetPosition();
		auto oriFocCam1 = renderer02->GetActiveCamera()->GetFocalPoint();
		auto oriPosCam2 = renderer03->GetActiveCamera()->GetPosition();
		auto oriFocCam2 = renderer03->GetActiveCamera()->GetFocalPoint();

		auto vt = vtkTransform::New();
		auto mat1 = vtkMatrix4x4::New();
		mat1->SetElement(0, 0, 0.5);
		mat1->SetElement(1, 1, 1);
		mat1->SetElement(2, 2, 1);
		mat1->SetElement(3, 3, 1);
		vt->SetMatrix(mat1);
		//renderer02->GetActiveCamera()->SetUserTransform(vt);



		vtkSmartPointer<vtkMyCameraCallback> cameraCallback1 = vtkSmartPointer<vtkMyCameraCallback>::New();
		cameraCallback1->m_renderer = renderer01;
		vtkSmartPointer<vtkMyCameraCallback> cameraCallback2 = vtkSmartPointer<vtkMyCameraCallback>::New();
		cameraCallback2->m_renderer = renderer02;
		renderer01->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback1);
		renderer02->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback2);
		renderer02->GetActiveCamera()->SetUserTransform(vt);
		renderer03->GetActiveCamera()->SetUserTransform(vt);

		vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
			vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		windowInteractor01->SetInteractorStyle(style);
		ui->horizontalSlider->setMinimum(0);
		ui->horizontalSlider->setMaximum(100);
		int value = ui->horizontalSlider->value();
		auto camLeft = renderer02->GetActiveCamera();
		auto camRight = renderer03->GetActiveCamera();

		double x = 0 + value ;
		//ui->horizontalSlider_4->setValue(int(x));
		camRight->SetPosition(camLeft->GetPosition()[0] + x, camLeft->GetPosition()[1], camLeft->GetPosition()[2]);
		camRight->SetRoll(camLeft->GetRoll());
		renderer02->ResetCameraClippingRange();
		renderer03->ResetCameraClippingRange();

		windowInteractor02->Render();

		
		//auto oriPosCam1 = renderer02->GetActiveCamera()->GetPosition();
		//auto oriFocCam1 = renderer02->GetActiveCamera()->GetFocalPoint();
		//auto oriPosCam2 = renderer03->GetActiveCamera()->GetPosition();
		//auto oriFocCam2 = renderer03->GetActiveCamera()->GetFocalPoint();

		//auto vt = vtkTransform::New();
		//auto mat1 = vtkMatrix4x4::New();
		//mat1->SetElement(0, 0, 0.5);
		//mat1->SetElement(1, 1, 1);
		//mat1->SetElement(2, 2, 1);
		//mat1->SetElement(3, 3, 1);
		//vt->SetMatrix(mat1);
		//renderer02->GetActiveCamera()->SetUserTransform(vt);
		

		ui->qvtkWidget_4->SetRenderWindow(renderWindow01);		// make the link between VTK and QT
		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer01);
		windowInteractor01->SetRenderWindow(renderWindow01);

		ui->qvtkWidget_4->show();
		renderWindow01->Render();
		renderer01->ResetCamera();
		renderer02->ResetCamera();
		renderer03->ResetCamera();

		windowInteractor01->Initialize();
		windowInteractor02->Initialize();
		windowInteractor01->Start();
		windowInteractor02->Start();
	}
	else {
		vtkRenderer*ren1 = vtkRenderer::New();
		ren1->AddVolume(volume);
		//vtkRenderWindow*renWin = vtkRenderWindow::New();
		renWin->AddRenderer(ren1);
		renWin->SetSize(600, 300);

		vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();//设定交互
		iren->SetRenderWindow(renWin);
		vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
		iren->SetInteractorStyle(style);//指定交互方式

		ui->qvtkWidget_4->SetRenderWindow(renWin);		// make the link between VTK and QT
		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(ren1);
		// make the link between the  renderwindow and the interactor
		iren->SetRenderWindow(renWin);
		//ui->qvtkWidget_4->show();

		// interact with data
		renWin->Render();
		iren->Initialize();
		iren->Start();
	}
	*/
}

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSTLReader.h>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCubeAxesActor.h >


// 两种渲染
void MainWindow::test3()
{
	//一种渲染
	vtkVolume *volume = vtkVolume::New();
	vtkRenderWindow*renWin = vtkRenderWindow::New();
	vtkRenderWindow*renWin2 = vtkRenderWindow::New();
	vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();//设定交互
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();

	if (ui->comboBox->currentIndex() == 0)
	{
		//folder = ui->label_2->text().toStdString();
		//using ReaderType = itk::ImageSeriesReader< ImageType >;
		//using ImageIOType = itk::GDCMImageIO;
		//using NamesGeneratorType = itk::GDCMSeriesFileNames;
		//ReaderType::Pointer reader = ReaderType::New();
		//ImageIOType::Pointer dicomIO = ImageIOType::New();
		//reader->SetImageIO(dicomIO);
		//NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
		//nameGenerator->SetUseSeriesDetails(true);
		//nameGenerator->SetDirectory(folder.c_str());
		//using SeriesIdContainer = std::vector< std::string >;
		//const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
		//auto seriesItr = seriesUID.begin();
		//auto seriesEnd = seriesUID.end();

		//using FileNamesContainer = std::vector< std::string >;
		//FileNamesContainer fileNames;
		//std::string seriesIdentifier;
		//while (seriesItr != seriesEnd)
		//{
		//	seriesIdentifier = seriesItr->c_str();
		//	fileNames = nameGenerator->GetFileNames(seriesIdentifier);
		//	++seriesItr;
		//}
		//reader->SetFileNames(fileNames);
		//reader->Update();

		//ImageType::SizeType imgSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
		////cout << "read done！Original size: " << imgSize << endl;

		//typedef itk::ImageToVTKImageFilter< ImageType> itkTovtkFilterType;
		//itkTovtkFilterType::Pointer itkTovtkImageFilter = itkTovtkFilterType::New();
		//itkTovtkImageFilter->SetInput(reader->GetOutput());
		//itkTovtkImageFilter->Update();


		int count = 1;
		const char *dirname = folder.data();//ITK注释
		double opacityWindow = 4096;
		double opacityLevel = 2048;
		int blendType = 0;
		int clip = 0;
		double reductionFactor = 1.0;
		double frameRate = 10.0;
		char *fileName = 0;
		int fileType = 0;

		bool independentComponents = true;

		// Create the renderer, render window and interactor
		vtkRenderer *renderer = vtkRenderer::New();
		vtkRenderWindow *renWin = vtkRenderWindow::New();
		renWin->AddRenderer(renderer);

		// Connect it all. Note that funny arithematic on the 
		// SetDesiredUpdateRate - the vtkRenderWindow divides it
		// allocated time across all renderers, and the renderer
		// divides it time across all props. If clip is
		// true then there are two props
		vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
		iren->SetRenderWindow(renWin);
		iren->SetDesiredUpdateRate(frameRate / (1 + clip));

		iren->GetInteractorStyle()->SetDefaultRenderer(renderer);

		// Read the data

		vtkAlgorithm *reader = 0;//ITK注释
		vtkImageData *input = 0;

		vtkDICOMImageReader *dicomReader = vtkDICOMImageReader::New();
		dicomReader->SetDirectoryName(dirname);
		dicomReader->Update();

		input = dicomReader->GetOutput();
		reader = dicomReader;
		//input = itkTovtkImageFilter->GetOutput();//ITK注释
		

		// Verify that we actually have a volume
		int dim[3];
		input->GetDimensions(dim);
		if (dim[0] < 2 ||
			dim[1] < 2 ||
			dim[2] < 2)
		{
			printf("Error loading data!");
			exit(EXIT_FAILURE);
		}

		vtkImageResample *resample = vtkImageResample::New();
		if (reductionFactor < 1.0)
		{
			resample->SetInputConnection(reader->GetOutputPort());
			//resample->SetInputData(itkTovtkImageFilter->GetOutput());//ITK注释
			resample->SetAxisMagnificationFactor(0, reductionFactor);
			resample->SetAxisMagnificationFactor(1, reductionFactor);
			resample->SetAxisMagnificationFactor(2, reductionFactor);
		}

		// Create our volume and mapper
		
		vtkGPUVolumeRayCastMapper *mapper = vtkGPUVolumeRayCastMapper::New();

		// Add a box widget if the clip option was selected
		vtkBoxWidget *box = vtkBoxWidget::New();
		if (clip)
		{
			box->SetInteractor(iren);
			box->SetPlaceFactor(1.01);

			box->SetDefaultRenderer(renderer);
			box->InsideOutOn();
			box->PlaceWidget();
			vtkBoxWidgetCallback *callback = vtkBoxWidgetCallback::New();
			callback->SetMapper(mapper);
			box->AddObserver(vtkCommand::InteractionEvent, callback);
			callback->Delete();
			box->EnabledOn();
			box->GetSelectedFaceProperty()->SetOpacity(0.0);
		}

		if (reductionFactor < 1.0)
		{
			mapper->SetInputConnection(resample->GetOutputPort());
		}
		else
		{
			mapper->SetInputConnection(reader->GetOutputPort());
			//mapper->SetInputData(itkTovtkImageFilter->GetOutput());//ITK注释
		}


		// Set the sample distance on the ray to be 1/2 the average spacing
		double spacing[3];
		if (reductionFactor < 1.0)
		{
			resample->GetOutput()->GetSpacing(spacing);
		}
		else
		{
			input->GetSpacing(spacing);
		}

		//  mapper->SetSampleDistance( (spacing[0]+spacing[1]+spacing[2])/6.0 );
		//  mapper->SetMaximumImageSampleDistance(10.0);


		  // Create our transfer function
		vtkColorTransferFunction *colorFun = vtkColorTransferFunction::New();
		vtkPiecewiseFunction *opacityFun = vtkPiecewiseFunction::New();

		// Create the property and attach the transfer functions
		vtkVolumeProperty *property = vtkVolumeProperty::New();
		property->SetIndependentComponents(independentComponents);
		property->SetColor(colorFun);
		property->SetScalarOpacity(opacityFun);
		property->SetInterpolationTypeToLinear();

		// connect up the volume to the property and the mapper
		volume->SetProperty(property);
		volume->SetMapper(mapper);

		// Depending on the blend type selected as a command line option, 
		// adjust the transfer function
		switch (blendType)
		{
		case 0:
			//colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0);
			//opacityFun->AddSegment(opacityLevel - 0.5*opacityWindow, 0.0,
			//	opacityLevel + 0.5*opacityWindow, 1.0);
			//mapper->SetBlendModeToMaximumIntensity();

			//colorFun->AddRGBSegment(opacityLevel - 0.5*opacityWindow, 0.0, 0.0, 0.0,
			//	opacityLevel + 0.5*opacityWindow, 1.0, 1.0, 1.0);
			//opacityFun->AddSegment(opacityLevel - 0.5*opacityWindow, 0.0,
			//	opacityLevel + 0.5*opacityWindow, 1.0);
			//mapper->SetBlendModeToComposite();
			//property->ShadeOff();

			colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0);
			opacityFun->AddSegment(opacityLevel - 0.5*opacityWindow, 0.0,
				opacityLevel + 0.5*opacityWindow, 1.0);
			mapper->SetBlendModeToComposite();
			property->ShadeOn();

			//colorFun->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
			//colorFun->AddRGBPoint(-1000, .62, .36, .18, 0.5, 0.0);
			//colorFun->AddRGBPoint(-500, .88, .60, .29, 0.33, 0.45);
			//colorFun->AddRGBPoint(3071, .83, .66, 1, 0.5, 0.0);
			//opacityFun->AddPoint(-3024, 0, 0.5, 0.0);
			//opacityFun->AddPoint(-1000, 0, 0.5, 0.0);
			//opacityFun->AddPoint(-500, 1.0, 0.33, 0.45);
			//opacityFun->AddPoint(3071, 1.0, 0.5, 0.0);
			//mapper->SetBlendModeToComposite();
			//property->ShadeOn();
			//property->SetAmbient(0.1);
			//property->SetDiffuse(0.9);
			//property->SetSpecular(0.2);
			//property->SetSpecularPower(10.0);
			//property->SetScalarOpacityUnitDistance(0.8919);

			break;
		default:
			vtkGenericWarningMacro("Unknown blend type.");
			break;
		}

		if (!mapper->IsRenderSupported(renWin, property))
		{
			printf("This mapper is unsupported on this platform");
			exit(EXIT_FAILURE);
		}
	}
	
	//一种渲染
	else
	{
		//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
		folder = ui->label_2->text().toStdString();
		vtkSmartPointer<vtkDICOMImageReader> reader =
			vtkSmartPointer<vtkDICOMImageReader>::New();
		reader->SetDirectoryName(folder.c_str());
		reader->Update();

		//folder = ui->label_2->text().toStdString();
		//using ReaderType = itk::ImageSeriesReader< ImageType >;
		//using ImageIOType = itk::GDCMImageIO;
		//using NamesGeneratorType = itk::GDCMSeriesFileNames;
		//ReaderType::Pointer reader = ReaderType::New();
		//ImageIOType::Pointer dicomIO = ImageIOType::New();
		//reader->SetImageIO(dicomIO);
		//NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
		//nameGenerator->SetUseSeriesDetails(true);
		////nameGenerator->SetDirectory("E:/NK/04_data/Juan/Xi_CT");
		//nameGenerator->SetDirectory(folder.c_str());

		//using SeriesIdContainer = std::vector< std::string >;
		//const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
		//auto seriesItr = seriesUID.begin();
		//auto seriesEnd = seriesUID.end();

		//using FileNamesContainer = std::vector< std::string >;
		//FileNamesContainer fileNames;
		//std::string seriesIdentifier;
		//while (seriesItr != seriesEnd)
		//{
		//	seriesIdentifier = seriesItr->c_str();
		//	fileNames = nameGenerator->GetFileNames(seriesIdentifier);
		//	++seriesItr;
		//}
		//reader->SetFileNames(fileNames);
		//reader->Update();
		//ImageType::SizeType imgSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
		////cout << "read done！Original size: " << imgSize << endl;

		//typedef itk::ImageToVTKImageFilter< ImageType> itkTovtkFilterType;
		//itkTovtkFilterType::Pointer itkTovtkImageFilter = itkTovtkFilterType::New();
		//itkTovtkImageFilter->SetInput(reader->GetOutput());
		//itkTovtkImageFilter->Update();



		//volumeMapper->SetInputData(itkTovtkImageFilter->GetOutput());
		volumeMapper->SetInputData(reader->GetOutput());//ITK注释
		volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 2);	//设置光线采样距离
		//volumeMapper->SetAutoAdjustSampleDistances(0);//设置图像采样步长
		//volumeMapper->SetImageSampleDistance(4);


		volumeProperty->SetInterpolationTypeToLinear();
		volumeProperty->ShadeOn();  //打开或者关闭阴影测试
		volumeProperty->SetAmbient(0.4);
		volumeProperty->SetDiffuse(0.6);  //漫反射
		volumeProperty->SetSpecular(0.2); //镜面反射
		//设置不透明度
		vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
			vtkSmartPointer<vtkPiecewiseFunction>::New();
		compositeOpacity->AddPoint(70, 0.00);
		compositeOpacity->AddPoint(90, 0.40);
		compositeOpacity->AddPoint(180, 0.60);
		volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数
		//设置梯度不透明属性
		vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
			vtkSmartPointer<vtkPiecewiseFunction>::New();
		volumeGradientOpacity->AddPoint(10, 0.0);
		volumeGradientOpacity->AddPoint(90, 0.5);
		volumeGradientOpacity->AddPoint(100, 1.0);
		volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比
		//设置颜色属性
		vtkSmartPointer<vtkColorTransferFunction> color =
			vtkSmartPointer<vtkColorTransferFunction>::New();
		color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
		color->AddRGBPoint(64.00, 1.00, 0.52, 0.30);
		color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
		color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
		volumeProperty->SetColor(color);

		volume->SetMapper(volumeMapper);
		volume->SetProperty(volumeProperty);
		/*for (int i = 500; i >= -110; i--)
		{
			auto arr = volumeMapper->GetCroppingRegionPlanes();
			arr[4] = i / 2.0;
			volumeMapper->SetCroppingRegionPlanes(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
			renWin->Render();
			Sleep(10);*/
	}
	//耳蜗入路E:\NK\04_data\301\CT&MR\20190910\WANG ZI HE_K0368666_135624\0.6 x 0.6_201 test
	vtkSmartPointer<vtkTransform> trans =
		vtkSmartPointer<vtkTransform>::New();

	trans->PostMultiply();	//右乘
	//volume->SetPosition(0, 0, 0);
	trans->RotateX(90);
	trans->RotateY(20);
	trans->Translate(-180, 150, 0);	
	//volume->SetUserTransform(trans);

	int select = 4;
	//膝关节对应数据：E:\NK\04_data\xiguanjie_model\__18991230_DICOM
	//耳蜗对应数据：E:\NK\04_data\brain_model\Brain__20210108_DICOM_Trans
	//1：膝关节3D	2：耳蜗3D	3：膝关节2D		4：耳蜗2D
	if (select==1)
	{
		dist = ui->horizontalSlider->value();

		double leftViewStation[4] = { 0.0, 0.0, 0.5, 1.0 };
		double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };

		int row = 816, col = 816, slicer_num = 627;
		double space[3] = { 0.3,0.3,0.3 };
		//切面
		vtkSmartPointer<vtkPlaneSource>planeSource1;
		vtkSmartPointer<vtkPolyData> polydataPlane1;
		vtkSmartPointer<vtkPolyDataMapper> mapperPlane1;
		vtkSmartPointer<vtkActor> actorPlane_le;
		///创建一个平面(le)
		planeSource1 = vtkSmartPointer<vtkPlaneSource>::New();//下面是根据点法式创建的平面
		//切膝关节
		//planeSource1->SetOrigin(0 * space[0], 0 * space[1], -445.193901165712* space[2]);
		////planeSource1->SetOrigin((matlab_x_y_n[2] - 1)* space[0], (col - matlab_x_y_n[1])* space[1], (slicer_num - matlab_x_y_n[2])* space[2]);
		//planeSource1->SetPoint1(1000 * space[0], 0 * space[1], -278.260432807163* space[2]);
		//planeSource1->SetPoint2(0 * space[0], 1000 * space[1], 397.231540678908* space[2]);

		//全人CT
		planeSource1->SetOrigin(-1000, -1000, 800);
		planeSource1->SetPoint1(-1000,0,800);
		planeSource1->SetPoint2(1000, 0, 800);


		planeSource1->Update();
		polydataPlane1 = vtkSmartPointer<vtkPolyData>::New();
		polydataPlane1 = planeSource1->GetOutput();
		mapperPlane1 = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapperPlane1->SetInputData(polydataPlane1);
		actorPlane_le = vtkSmartPointer<vtkActor>::New();
		actorPlane_le->SetMapper(mapperPlane1);
		actorPlane_le->GetProperty()->SetOpacity(1);
		actorPlane_le->GetProperty()->SetColor(0, 200, 220);

		vtkSmartPointer<vtkAxesActor> actor2 =
			vtkSmartPointer<vtkAxesActor>::New();
		actor2->SetPosition(0, 0, 0);
		actor2->SetTotalLength(100, 100, 100);
		actor2->SetShaftType(0);
		actor2->SetAxisLabels(0);
		actor2->SetCylinderRadius(0.02);


		renderer01 = vtkSmartPointer<vtkRenderer>::New();
		renderer02 = vtkSmartPointer<vtkRenderer>::New();
		renderer03 = vtkSmartPointer<vtkRenderer>::New();


		renderer01->AddActor(actor2);

		//renderer01->AddActor(actorPlane_le);
		//renderer02->AddActor(actorPlane_le);
		//renderer03->AddActor(actorPlane_le);


		vtkSmartPointer<vtkTransform> trans2 =
			vtkSmartPointer<vtkTransform>::New();
		//trans->PostMultiply();	//右乘
		//trans->RotateX(90);
		trans2->RotateX(60);
		trans2->RotateY(90);
		//trans->Translate(-180, 150, 0);

		//trans->Translate(0, 25, -120);
		volume->SetUserTransform(trans2);



		renderer01->SetBackground(0.1,0.1,0.1);
		//渲染时使用同样的数据
		renderer01->AddVolume(volume);
		renderer02->AddVolume(volume);
		//renderer02->SetBackground(.2, .3, .5);
		renderer02->SetViewport(leftViewStation);
		renderer03->AddVolume(volume);
		//renderer03->SetBackground(.2, .3, .5);
		renderer03->SetViewport(rightViewStation);

		renderWindow01 = vtkSmartPointer<vtkRenderWindow>::New();
		renderWindow02 = vtkSmartPointer<vtkRenderWindow>::New();

		renderWindow01->AddRenderer(renderer01);
		renderWindow02->AddRenderer(renderer02);
		renderWindow02->AddRenderer(renderer03);
		vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		//windowInteractor01->SetRenderWindow(renderWindow01);
		windowInteractor02->SetRenderWindow(renderWindow02);

		vtkSmartPointer<vtkMyCameraCallback> cameraCallback1 = vtkSmartPointer<vtkMyCameraCallback>::New();
		cameraCallback1->m_renderer = renderer01;
		vtkSmartPointer<vtkMyCameraCallback> cameraCallback2 = vtkSmartPointer<vtkMyCameraCallback>::New();
		cameraCallback2->m_renderer = renderer02;
		renderer01->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback1);
		renderer02->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback2);

		vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
			vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		windowInteractor01->SetInteractorStyle(style);

		auto vt = vtkTransform::New();
		auto mat1 = vtkMatrix4x4::New();
		mat1->SetElement(0, 0, 0.5);
		mat1->SetElement(1, 1, 1);
		mat1->SetElement(2, 2, 1);
		mat1->SetElement(3, 3, 1);
		vt->SetMatrix(mat1);
		renderer02->GetActiveCamera()->SetUserTransform(vt);
		renderer03->GetActiveCamera()->SetUserTransform(vt);

		//双眼距离
		//int value = 0;
		auto camLeft = renderer02->GetActiveCamera();
		auto camRight = renderer03->GetActiveCamera();
		double x = 0 + dist / 100.0;

		//ui->horizontalSlider->setMinimum(0);
		//ui->horizontalSlider->setMaximum(1000);
		//ui->horizontalSlider->setValue(value);

		camRight->SetPosition(camLeft->GetPosition()[0] + x, camLeft->GetPosition()[1], camLeft->GetPosition()[2]);
		camRight->SetRoll(camLeft->GetRoll());
		renderer02->ResetCameraClippingRange();
		renderer03->ResetCameraClippingRange();
		windowInteractor02->Render();

		ui->qvtkWidget_4->SetRenderWindow(renderWindow01);		// make the link between VTK and QT
		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer01);
		windowInteractor01->SetRenderWindow(renderWindow01);

		ui->qvtkWidget_4->show();
		renderWindow01->Render();
		renderer01->ResetCamera();
		//renderer02->ResetCamera();

		windowInteractor01->Initialize();
		windowInteractor02->Initialize();
		windowInteractor01->Start();
		windowInteractor02->Start();
	}

	else if (select == 2)
	{
		dist = ui->horizontalSlider->value();

		double leftViewStation[4] = { 0.0, 0.0, 0.5, 1.0 };
		double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };

		vtkSmartPointer<vtkAxesActor> actor2 =
			vtkSmartPointer<vtkAxesActor>::New();
		actor2->SetPosition(0, 0, 0);
		actor2->SetTotalLength(100, 100, 100);
		actor2->SetShaftType(0);
		actor2->SetAxisLabels(0);
		actor2->SetCylinderRadius(0.02);


		vtkSmartPointer<vtkLineSource> line10 = vtkSmartPointer<vtkLineSource>::New();
		line10->SetPoint1(290 * 0.3, (816 - 510) * 0.3, (627 - 320) * 0.3);
		line10->SetPoint2(380 * 0.3, (816 - 470) * 0.3, (627 - 320) * 0.3);
		vtkSmartPointer<vtkTubeFilter> tubeFilter10 = vtkSmartPointer<vtkTubeFilter>::New();
		tubeFilter10->SetInputConnection(line10->GetOutputPort());
		tubeFilter10->SetRadius(0.5);
		tubeFilter10->SetNumberOfSides(100);
		tubeFilter10->CappingOn();
		vtkSmartPointer<vtkDataSetMapper> line10_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line10_mapper->SetInputConnection(tubeFilter10->GetOutputPort());
		vtkSmartPointer<vtkActor> line10_actor = vtkSmartPointer<vtkActor>::New();
		line10_actor->SetMapper(line10_mapper);
		line10_actor->GetProperty()->SetColor(0, 1.0, 0);

		//trans->PostMultiply();	//右乘
		//trans->RotateX(90);
		//trans->RotateY(20);
		//trans->Translate(-180, 150, 0);

		//trans->Translate(0, 25, -120);
		volume->SetUserTransform(trans);
		line10_actor->SetUserTransform(trans);

		renderer01 = vtkSmartPointer<vtkRenderer>::New();
		renderer02 = vtkSmartPointer<vtkRenderer>::New();
		renderer03 = vtkSmartPointer<vtkRenderer>::New();
		
		renderer01->AddActor(line10_actor);
		renderer02->AddActor(line10_actor);
		renderer03->AddActor(line10_actor);
		renderer01->AddActor(actor2);

		renderer01->SetBackground(0.1, 0.1, 0.1);
		//渲染时使用同样的数据
		renderer01->AddVolume(volume);
		renderer02->AddVolume(volume);
		//renderer02->SetBackground(.2, .3, .5);
		renderer02->SetViewport(leftViewStation);
		renderer03->AddVolume(volume);
		//renderer03->SetBackground(.2, .3, .5);
		renderer03->SetViewport(rightViewStation);

		renderWindow01 = vtkSmartPointer<vtkRenderWindow>::New();
		renderWindow02 = vtkSmartPointer<vtkRenderWindow>::New();

		renderWindow01->AddRenderer(renderer01);
		renderWindow02->AddRenderer(renderer02);
		renderWindow02->AddRenderer(renderer03);
		vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		//windowInteractor01->SetRenderWindow(renderWindow01);
		windowInteractor02->SetRenderWindow(renderWindow02);

		vtkSmartPointer<vtkMyCameraCallback> cameraCallback1 = vtkSmartPointer<vtkMyCameraCallback>::New();
		cameraCallback1->m_renderer = renderer01;
		vtkSmartPointer<vtkMyCameraCallback> cameraCallback2 = vtkSmartPointer<vtkMyCameraCallback>::New();
		cameraCallback2->m_renderer = renderer02;
		renderer01->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback1);
		renderer02->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback2);

		vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
			vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		windowInteractor01->SetInteractorStyle(style);

		auto vt = vtkTransform::New();
		auto mat1 = vtkMatrix4x4::New();
		mat1->SetElement(0, 0, 0.5);
		mat1->SetElement(1, 1, 1);
		mat1->SetElement(2, 2, 1);
		mat1->SetElement(3, 3, 1);
		vt->SetMatrix(mat1);
		renderer02->GetActiveCamera()->SetUserTransform(vt);
		renderer03->GetActiveCamera()->SetUserTransform(vt);

		//双眼距离
		//int value = 0;
		auto camLeft = renderer02->GetActiveCamera();
		auto camRight = renderer03->GetActiveCamera();
		double x = 0 + dist / 100.0;

		camRight->SetPosition(camLeft->GetPosition()[0] + x, camLeft->GetPosition()[1], camLeft->GetPosition()[2]);
		camRight->SetRoll(camLeft->GetRoll());
		renderer02->ResetCameraClippingRange();
		renderer03->ResetCameraClippingRange();
		windowInteractor02->Render();

		ui->qvtkWidget_4->SetRenderWindow(renderWindow01);		// make the link between VTK and QT
		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer01);
		windowInteractor01->SetRenderWindow(renderWindow01);

		ui->qvtkWidget_4->show();
		renderWindow01->Render();
		renderer01->ResetCamera();
		//renderer02->ResetCamera();

		windowInteractor01->Initialize();
		windowInteractor02->Initialize();
		windowInteractor01->Start();
		windowInteractor02->Start();
	}
	
	else if (select == 3) {


		int row = 816, col = 816, slicer_num = 627;
		double space[3] = { 0.3,0.3,0.3 };
		//切面
		vtkSmartPointer<vtkPlaneSource>planeSource1;
		vtkSmartPointer<vtkPolyData> polydataPlane1;
		vtkSmartPointer<vtkPolyDataMapper> mapperPlane1;
		vtkSmartPointer<vtkActor> actorPlane_le;
		///创建一个平面(le)
		planeSource1 = vtkSmartPointer<vtkPlaneSource>::New();//下面是根据点法式创建的平面
		planeSource1->SetOrigin(0 * space[0], 0 * space[1], -445.193901165712* space[2]);
		//planeSource1->SetOrigin((matlab_x_y_n[2] - 1)* space[0], (col - matlab_x_y_n[1])* space[1], (slicer_num - matlab_x_y_n[2])* space[2]);
		planeSource1->SetPoint1(1000 * space[0], 0 * space[1], -278.260432807163* space[2]);
		planeSource1->SetPoint2(0 * space[0], 1000 * space[1], 397.231540678908* space[2]);
		planeSource1->Update();
		polydataPlane1 = vtkSmartPointer<vtkPolyData>::New();
		polydataPlane1 = planeSource1->GetOutput();
		mapperPlane1 = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapperPlane1->SetInputData(polydataPlane1);
		actorPlane_le = vtkSmartPointer<vtkActor>::New();
		actorPlane_le->SetMapper(mapperPlane1);
		actorPlane_le->GetProperty()->SetOpacity(1);
		actorPlane_le->GetProperty()->SetColor(0, 200, 220);

		vtkRenderer*ren1 = vtkRenderer::New();
		vtkSmartPointer<vtkAxesActor> actorWorld;
		actorWorld = vtkSmartPointer<vtkAxesActor>::New();
		actorWorld->SetPosition(0, 0, 0);
		actorWorld->SetTotalLength(20, 20, 20);
		actorWorld->SetShaftType(0);
		actorWorld->SetAxisLabels(1);
		actorWorld->SetCylinderRadius(0.02);
		actorWorld->SetVisibility(1);
		ren1->AddActor(actorWorld);
		ren1->AddVolume(volume);
		ren1->AddVolume(actorPlane_le);

		ren1->SetBackground(0.1, 0.1, 0.1);
		vtkRenderWindow*renWin = vtkRenderWindow::New();
		renWin->AddRenderer(ren1);

		vtkSmartPointer<vtkTransform> trans =
			vtkSmartPointer<vtkTransform>::New();
		trans->PostMultiply();

		vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();//设定交互
		iren->SetRenderWindow(renWin);
		vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
		iren->SetInteractorStyle(style);//指定交互方式

		ui->qvtkWidget_4->SetRenderWindow(renWin);		// make the link between VTK and QT
		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(ren1);
		// make the link between the  renderwindow and the interactor
		iren->SetRenderWindow(renWin);
		ui->qvtkWidget_4->show();

		// interact with data
		renWin->Render();
		//iren->Initialize();
		//iren->Start();
	}

	else if (select == 4) {

		// create a renderer, render window, and interactor

		vtkRenderer*ren1 = vtkRenderer::New();

				//世界坐标系
		vtkSmartPointer<vtkAxesActor> actorWorld;
		actorWorld = vtkSmartPointer<vtkAxesActor>::New();
		actorWorld->SetPosition(0, 0, 0);
		actorWorld->SetTotalLength(20, 20, 20);
		actorWorld->SetShaftType(0);
		actorWorld->SetAxisLabels(1);
		actorWorld->SetCylinderRadius(0.02);
		actorWorld->SetVisibility(1);
		//renderer->AddActor(actorWorld);
		vtkSmartPointer<vtkAxesActor> actorWorld2;
		actorWorld2 = vtkSmartPointer<vtkAxesActor>::New();
		actorWorld2->SetPosition(0, 0, 0);
		actorWorld2->SetTotalLength(20, 20, 20);
		actorWorld2->SetShaftType(0);
		actorWorld2->SetAxisLabels(1);
		actorWorld2->SetCylinderRadius(0.02);
		actorWorld2->SetVisibility(1);

	//	圆柱
		vtkSmartPointer<vtkLineSource> lineSource =
			vtkSmartPointer<vtkLineSource>::New();
		lineSource->SetPoint1(-20, 12.25, -51.25);
		lineSource->SetPoint2(-50, 12.25, -70.12);
		vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
		tubeFilter->SetInputConnection(lineSource->GetOutputPort());
		tubeFilter->SetRadius(1.0);
		tubeFilter->SetNumberOfSides(100);
		tubeFilter->CappingOn();

		vtkSmartPointer<vtkLineSource> linex = vtkSmartPointer<vtkLineSource>::New();
		linex->SetPoint1(0, 0, 0);
		linex->SetPoint2(100, 0, 0);
		vtkSmartPointer<vtkLineSource> liney = vtkSmartPointer<vtkLineSource>::New();
		liney->SetPoint1(0, 0, 0);
		liney->SetPoint2(0, 100, 0);
		vtkSmartPointer<vtkLineSource> linez = vtkSmartPointer<vtkLineSource>::New();
		linez->SetPoint1(0, 0, 0);
		linez->SetPoint2(0, 0, 100);
		vtkSmartPointer<vtkLineSource> line1 = vtkSmartPointer<vtkLineSource>::New();
		line1->SetPoint1(161 * 0.15, 391 * 0.15, 178 * 0.2);
		line1->SetPoint2(243 * 0.15, 362 * 0.15, 193 * 0.2);
		vtkSmartPointer<vtkLineSource> line2 = vtkSmartPointer<vtkLineSource>::New();
		line2->SetPoint1(146 * 0.15, 321 * 0.15, 179 * 0.2);
		line2->SetPoint2(243 * 0.15, 290 * 0.15, 189 * 0.2);
		vtkSmartPointer<vtkLineSource> line3 = vtkSmartPointer<vtkLineSource>::New();
		line3->SetPoint1(128 * 0.15, 327 * 0.15, 130 * 0.2);
		line3->SetPoint2(221 * 0.15, 295 * 0.15, 143 * 0.2);
		vtkSmartPointer<vtkLineSource> line4 = vtkSmartPointer<vtkLineSource>::New();
		line4->SetPoint1(132 * 0.15, 326 * 0.15, 80 * 0.2);
		line4->SetPoint2(201 * 0.15, 302 * 0.15, 81 * 0.2);
		vtkSmartPointer<vtkLineSource> line5 = vtkSmartPointer<vtkLineSource>::New();
		line5->SetPoint1(138 * 0.15, 242 * 0.15, 148 * 0.2);
		line5->SetPoint2(185 * 0.15, 247 * 0.15, 162 * 0.2);
		vtkSmartPointer<vtkLineSource> line6 = vtkSmartPointer<vtkLineSource>::New();
		line6->SetPoint1(135 * 0.15, 241 * 0.15, 103 * 0.2);
		line6->SetPoint2(205 * 0.15, 250 * 0.15, 81 * 0.2);
		vtkSmartPointer<vtkLineSource> line7 = vtkSmartPointer<vtkLineSource>::New();
		line7->SetPoint1(156 * 0.15, 254 * 0.15, 85 * 0.2);
		line7->SetPoint2(236 * 0.15, 255 * 0.15, 83 * 0.2);

		vtkSmartPointer<vtkLineSource> line8 = vtkSmartPointer<vtkLineSource>::New();
		line8->SetPoint1(195 * 0.15, 297 * 0.15, 102 * 0.2);
		line8->SetPoint2(255 * 0.15, 339 * 0.15, 112 * 0.2);
		vtkSmartPointer<vtkLineSource> line9 = vtkSmartPointer<vtkLineSource>::New();
		line9->SetPoint1(213 * 0.15, 404 * 0.15, 157 * 0.2);
		line9->SetPoint2(313 * 0.15, 371 * 0.15, 153 * 0.2);
		vtkSmartPointer<vtkLineSource> line10 = vtkSmartPointer<vtkLineSource>::New();
		//line10->SetPoint1(-20, 13.5, -50);
		//line10->SetPoint2(-50, 13.5, -72);		
		//line10->SetPoint1(380 * 0.1953, (1024 - 525) * 0.1953, (100 - 53) * 0.6);
		//line10->SetPoint2(324 * 0.1953, (1024 - 550) * 0.1953, (100 - 53) * 0.6);
		////y=-0.45 (x - 380) + 525
		//line10->SetPoint2(250 * 0.1953, (1024 - (-0.4566 * (250 - 380) + 525)) * 0.1953, (100 - 53) * 0.6);
		//line10->SetPoint2(100 * 0.1953, (1024 - (-0.4566 * (100 - 380) + 525)) * 0.1953, (100 - 53) * 0.6);

		//在slicer中点出标记的点x,y,z
		//line10->SetPoint1(136 * 0.484, (512 - 280) * 0.484, (400 - 160) * 0.6);
		//line10->SetPoint2(10 * 0.484, (512 - 345) * 0.484, (400 - 160) * 0.6);


		float data[2][3] = { 0 };
		ifstream infile;//定义读取文件流，相对于程序来说是in
		infile.open("C:\\Users\\shenlin\\Desktop\\0124.txt");//打开文件
		for (int i = 0; i < 2; i++)//定义行循环
		{
			for (int j = 0; j < 3; j++)//定义列循环
			{
				infile >> data[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
			}
		}
		infile.close();//读取完成之后关闭文件

		//哈工大联调
		line10->SetPoint1(data[0][0] * 0.15, data[0][1] * 0.15, data[0][2] * 0.2);
		line10->SetPoint2(data[1][0] * 0.15, data[1][1] * 0.15, data[1][2] * 0.2);
		vtkSmartPointer<vtkTubeFilter> tubeFilter10 = vtkSmartPointer<vtkTubeFilter>::New();
		tubeFilter10->SetInputConnection(line10->GetOutputPort());
		tubeFilter10->SetRadius(0.5);
		tubeFilter10->SetNumberOfSides(100);
		tubeFilter10->CappingOn();

		vtkSmartPointer<vtkDataSetMapper> linex_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		linex_mapper->SetInputConnection(linex->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> liney_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		liney_mapper->SetInputConnection(liney->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> linez_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		linez_mapper->SetInputConnection(linez->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line1_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line1_mapper->SetInputConnection(line1->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line2_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line2_mapper->SetInputConnection(line2->GetOutputPort());		
		vtkSmartPointer<vtkDataSetMapper> line3_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line3_mapper->SetInputConnection(line3->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line4_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line4_mapper->SetInputConnection(line4->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line5_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line5_mapper->SetInputConnection(line5->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line6_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line6_mapper->SetInputConnection(line6->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line7_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line7_mapper->SetInputConnection(line7->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line8_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line8_mapper->SetInputConnection(line8->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line9_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line9_mapper->SetInputConnection(line9->GetOutputPort());
		vtkSmartPointer<vtkDataSetMapper> line10_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		line10_mapper->SetInputConnection(tubeFilter10->GetOutputPort());
		vtkSmartPointer<vtkActor> linex_actor = vtkSmartPointer<vtkActor>::New();
		linex_actor->SetMapper(linex_mapper);
		vtkSmartPointer<vtkActor> liney_actor = vtkSmartPointer<vtkActor>::New();
		liney_actor->SetMapper(liney_mapper);
		vtkSmartPointer<vtkActor> linez_actor = vtkSmartPointer<vtkActor>::New();
		linez_actor->SetMapper(linez_mapper);
		vtkSmartPointer<vtkActor> line1_actor = vtkSmartPointer<vtkActor>::New();
		line1_actor->SetMapper(line1_mapper);
		vtkSmartPointer<vtkActor> line2_actor = vtkSmartPointer<vtkActor>::New();
		line2_actor->SetMapper(line2_mapper);
		vtkSmartPointer<vtkActor> line3_actor = vtkSmartPointer<vtkActor>::New();
		line3_actor->SetMapper(line3_mapper);
		vtkSmartPointer<vtkActor> line4_actor = vtkSmartPointer<vtkActor>::New();
		line4_actor->SetMapper(line4_mapper);
		vtkSmartPointer<vtkActor> line5_actor = vtkSmartPointer<vtkActor>::New();
		line5_actor->SetMapper(line5_mapper);
		vtkSmartPointer<vtkActor> line6_actor = vtkSmartPointer<vtkActor>::New();
		line6_actor->SetMapper(line6_mapper);
		vtkSmartPointer<vtkActor> line7_actor = vtkSmartPointer<vtkActor>::New();
		line7_actor->SetMapper(line7_mapper);
		vtkSmartPointer<vtkActor> line8_actor = vtkSmartPointer<vtkActor>::New();
		line8_actor->SetMapper(line8_mapper);
		vtkSmartPointer<vtkActor> line9_actor = vtkSmartPointer<vtkActor>::New();
		line9_actor->SetMapper(line9_mapper);
		vtkSmartPointer<vtkActor> line10_actor = vtkSmartPointer<vtkActor>::New();
		line10_actor->SetMapper(line10_mapper);
		linex_actor->GetProperty()->SetColor(1.0, 0, 0);
		liney_actor->GetProperty()->SetColor(0, 1.0, 0);
		linez_actor->GetProperty()->SetColor(0, 0, 1.0);
		line1_actor->GetProperty()->SetColor(1.0, 0, 0);

		line2_actor->GetProperty()->SetColor(1.0, 0, 0);
		line3_actor->GetProperty()->SetColor(1.0, 0, 0);
		line4_actor->GetProperty()->SetColor(1.0, 0, 0);
		line5_actor->GetProperty()->SetColor(1.0, 0, 0);
		line6_actor->GetProperty()->SetColor(1.0, 0, 0);
		line7_actor->GetProperty()->SetColor(1.0, 0, 0);
		line8_actor->GetProperty()->SetColor(1.0, 0, 0);
		line9_actor->GetProperty()->SetColor(0, 1.0, 0);
		line10_actor->GetProperty()->SetColor(0, 1.0, 0);
		//ren1->AddActor(linex_actor);
		//ren1->AddActor(liney_actor);
		//ren1->AddActor(linez_actor);
		//ren1->AddActor(line1_actor);
		//ren1->AddActor(line2_actor);
		//ren1->AddActor(line3_actor);
		//ren1->AddActor(line4_actor);
		//ren1->AddActor(line5_actor);
		//ren1->AddActor(line6_actor);
		//ren1->AddActor(line7_actor);
		//ren1->AddActor(line8_actor);
		//ren1->AddActor(line9_actor);
		ren1->AddActor(line10_actor); 
		//ren1->AddActor(actorWorld);
		//ren1->AddActor(actorWorld2);
		ren1->AddVolume(volume);
		ren1->SetBackground(0.15,0.15,0.15);
		vtkRenderWindow*renWin = vtkRenderWindow::New();
		renWin->AddRenderer(ren1);
		//renWin->SetSize(600, 300);


		vtkSmartPointer<vtkTransform> trans =
			vtkSmartPointer<vtkTransform>::New();
		trans->PostMultiply();

		//double A[4][4] = { { 1,0,0,0},{ 0,1,0,0},{0,0,1,0},{ 0,0,0,1 } };
		//double B[4][4] = { { 0.9998,0.0147,0.0109,136.0051},{ -0.0143,-0.9992,-0.0362,222.9823},{-0.0114,0.0360,0.9993,84.0862},{ 0,0,0,1 } };
		double A[4][4] = { { 0.9992,0.0410,0.0014,118.9863},{ -0.0410,0.9989,-0.0229,20.2219},{-0.0023,0.0228,0.9997,79.7040},{ 0,0,0,1 } };
		double Ainv[4][4] = { { 0.9992,-0.0410,-0.0023,-117.8721},{ 0.0410,0.9989,0.0228,-26.8954},{0.0014,-0.0229,0.9997,-79.3864},{ 0,0,0,1 } };


		//vtkSmartPointer<vtkLineSource> linex = vtkSmartPointer<vtkLineSource>::New();
		//linex->SetPoint1(136.0051, 222.9823, 84.0862);
		//linex->SetPoint2(136.0051 +27.0964, 222.9823 +29.0227, 84.0862 -30.3887);
		linex->SetPoint1(118.9863, 20.2219, 79.7040);
		linex->SetPoint2(118.9863 -34.7634, 20.2219 -35.3180, 79.7040 +6.6443);
		//vtkSmartPointer<vtkLineSource> liney = vtkSmartPointer<vtkLineSource>::New();
		//liney->SetPoint1(136.0051, 222.9823, 84.0862);
		//liney->SetPoint2(136.0051 + 41.4806, 222.9823 - 24.2553, 84.0862 + 13.8216);
		liney->SetPoint1(118.9863, 20.2219, 79.7040);
		liney->SetPoint2(118.9863 -4.3570, 20.2219 +13.3181, 79.7040 +47.9963);
		//vtkSmartPointer<vtkLineSource> linez = vtkSmartPointer<vtkLineSource>::New();
		//linez->SetPoint1(136.0051, 222.9823, 84.0862);
		//linez->SetPoint2(136.0051 - 6.7190, 222.9823 - 32.7011, 84.0862 - 37.2222);
		linez->SetPoint1(118.9863, 20.2219, 79.7040);
		linez->SetPoint2(118.9863 -35.6724, 20.2219 +32.7913, 79.7040 -12.3372);
		//vtkSmartPointer<vtkDataSetMapper> linex_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		linex_mapper->SetInputConnection(linex->GetOutputPort());
		//vtkSmartPointer<vtkDataSetMapper> liney_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		liney_mapper->SetInputConnection(liney->GetOutputPort());
		//vtkSmartPointer<vtkDataSetMapper> linez_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		linez_mapper->SetInputConnection(linez->GetOutputPort());
		//vtkSmartPointer<vtkActor> linex_actor = vtkSmartPointer<vtkActor>::New();
		linex_actor->SetMapper(linex_mapper);
		//vtkSmartPointer<vtkActor> liney_actor = vtkSmartPointer<vtkActor>::New();
		liney_actor->SetMapper(liney_mapper);
		//vtkSmartPointer<vtkActor> linez_actor = vtkSmartPointer<vtkActor>::New();
		linez_actor->SetMapper(linez_mapper);
		linex_actor->GetProperty()->SetColor(1.0, 0, 0);
		liney_actor->GetProperty()->SetColor(0, 1.0, 0);
		linez_actor->GetProperty()->SetColor(0, 0, 1.0);
		//ren1->AddActor(linex_actor);
		//ren1->AddActor(liney_actor);
		//ren1->AddActor(linez_actor);

		//trans->Translate(C[0][3], C[1][3], C[2][3]);
		//trans->RotateX(90);
		//actorWorld2->SetUserTransform(trans);

		vtkSmartPointer<vtkSphereSource> sphereSource1 =
			vtkSmartPointer<vtkSphereSource>::New();
		//sphereSource1->SetCenter(136.0051, 222.9823, 84.0862);   // 设置中心
		//sphereSource1->SetCenter(118.9863, 20.2219, 79.7040);   // 设置中心
		//sphereSource1->SetCenter((60 - 1)*0.15, (512 - 134)*0.15, (400 - 193)*0.2);//哈工大联调
		sphereSource1->SetCenter(data[0][0] * 0.15, data[0][1] * 0.15, data[0][2] * 0.2);//哈工大联调
		sphereSource1->SetRadius(2.0);             // 设置半径
		vtkSmartPointer<vtkPolyDataMapper> mapper1 =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper1->SetInputConnection(sphereSource1->GetOutputPort());    // 设置映射的渲染数据
		vtkSmartPointer<vtkActor> actor1 =
			vtkSmartPointer<vtkActor>::New();
		actor1->SetMapper(mapper1);
		actor1->GetProperty()->SetColor(1, 0, 0);
		ren1->AddActor(actor1);

		vtkSmartPointer<vtkSphereSource> sphereSource2 =
			vtkSmartPointer<vtkSphereSource>::New();
		//sphereSource2->SetCenter(171.6105, 180.7488, 75.4992);   // 设置中心
		//sphereSource2->SetCenter(94.2400, 51.8401, 118.2966);   // 设置中心
		sphereSource2->SetCenter(data[1][0] * 0.15, data[1][1] * 0.15, data[1][2] * 0.2);//哈工大联调
		sphereSource2->SetRadius(2.0);             // 设置半径
		vtkSmartPointer<vtkPolyDataMapper> mapper2 =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper2->SetInputConnection(sphereSource2->GetOutputPort());    // 设置映射的渲染数据
		vtkSmartPointer<vtkActor> actor2 =
			vtkSmartPointer<vtkActor>::New();
		actor2->SetMapper(mapper2);
		actor2->GetProperty()->SetColor(1, 0, 0);
		ren1->AddActor(actor2);

		vtkSmartPointer<vtkSphereSource> sphereSource3 =
			vtkSmartPointer<vtkSphereSource>::New();
		//sphereSource3->SetCenter(124.0133, 164.6181, 17.6531);   // 设置中心
		sphereSource3->SetCenter(55.3774, 78.6934, 57.7049);   // 设置中心
		sphereSource3->SetRadius(5.0);             // 设置半径
		vtkSmartPointer<vtkPolyDataMapper> mapper3 =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper3->SetInputConnection(sphereSource3->GetOutputPort());    // 设置映射的渲染数据
		vtkSmartPointer<vtkActor> actor3 =
			vtkSmartPointer<vtkActor>::New();
		actor3->SetMapper(mapper3);
		actor3->GetProperty()->SetColor(0, 0, 1);
		//ren1->AddActor(actor3);

		vtkSmartPointer<vtkSphereSource> sphereSource4 =
			vtkSmartPointer<vtkSphereSource>::New();
		//sphereSource4->SetCenter(101.9393, 216.9324, 48.3700);   // 设置中心
		sphereSource4->SetCenter(97.5348, 34.1370, 36.8968);   // 设置中心
		sphereSource4->SetRadius(5.0);             // 设置半径
		vtkSmartPointer<vtkPolyDataMapper> mapper4 =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper4->SetInputConnection(sphereSource4->GetOutputPort());    // 设置映射的渲染数据
		vtkSmartPointer<vtkActor> actor4 =
			vtkSmartPointer<vtkActor>::New();
		actor4->SetMapper(mapper4);
		actor4->GetProperty()->SetColor(1, 1, 0);
		//ren1->AddActor(actor4);
		






		vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();//设定交互
		iren->SetRenderWindow(renWin);
		vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
		iren->SetInteractorStyle(style);//指定交互方式

		ui->qvtkWidget_4->SetRenderWindow(renWin);		// make the link between VTK and QT
		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(ren1);
		// make the link between the  renderwindow and the interactor
		iren->SetRenderWindow(renWin);
		ui->qvtkWidget_4->show();

		// interact with data
		renWin->Render();
		//iren->Initialize();
		//iren->Start();
	}

	else if (select == 5) {

	// create a renderer, render window, and interactor
	dist = ui->horizontalSlider->value();
	double leftViewStation[4] = { 0.0, 0.0, 0.5, 1.0 };
	double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };

	//vtkRenderer*ren1 = vtkRenderer::New();

	//世界坐标系
	vtkSmartPointer<vtkAxesActor> actorWorld;
	actorWorld = vtkSmartPointer<vtkAxesActor>::New();
	actorWorld->SetPosition(0, 0, 0);
	actorWorld->SetTotalLength(20, 20, 20);
	actorWorld->SetShaftType(0);
	actorWorld->SetAxisLabels(1);
	actorWorld->SetCylinderRadius(0.02);
	actorWorld->SetVisibility(1);
	//renderer->AddActor(actorWorld);
	vtkSmartPointer<vtkAxesActor> actorWorld2;
	actorWorld2 = vtkSmartPointer<vtkAxesActor>::New();
	actorWorld2->SetPosition(0, 0, 0);
	actorWorld2->SetTotalLength(20, 20, 20);
	actorWorld2->SetShaftType(0);
	actorWorld2->SetAxisLabels(1);
	actorWorld2->SetCylinderRadius(0.02);
	actorWorld2->SetVisibility(1);

	//	圆柱
	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(-20, 12.25, -51.25);
	lineSource->SetPoint2(-50, 12.25, -70.12);
	vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
	tubeFilter->SetInputConnection(lineSource->GetOutputPort());
	tubeFilter->SetRadius(0.5);
	tubeFilter->SetNumberOfSides(100);
	tubeFilter->CappingOn();

	vtkSmartPointer<vtkLineSource> line10 = vtkSmartPointer<vtkLineSource>::New();
	line10->SetPoint1(290 * 0.3, (816 - 510) * 0.3, (627 - 320) * 0.3);
	line10->SetPoint2(380 * 0.3, (816 - 470) * 0.3, (627 - 320) * 0.3);
	vtkSmartPointer<vtkTubeFilter> tubeFilter10 = vtkSmartPointer<vtkTubeFilter>::New();
	tubeFilter10->SetInputConnection(line10->GetOutputPort());
	tubeFilter10->SetRadius(0.5);
	tubeFilter10->SetNumberOfSides(100);
	tubeFilter10->CappingOn();

	vtkSmartPointer<vtkDataSetMapper> line10_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	line10_mapper->SetInputConnection(tubeFilter10->GetOutputPort());
	vtkSmartPointer<vtkActor> line10_actor = vtkSmartPointer<vtkActor>::New();
	line10_actor->SetMapper(line10_mapper);
	line10_actor->GetProperty()->SetColor(0, 1.0, 0);

	vtkSmartPointer<vtkTransform> trans =
		vtkSmartPointer<vtkTransform>::New();
	trans->PostMultiply();

	//double A[4][4] = { { 1,0,0,0},{ 0,1,0,0},{0,0,1,0},{ 0,0,0,1 } };
	//double B[4][4] = { { 0.9998,0.0147,0.0109,136.0051},{ -0.0143,-0.9992,-0.0362,222.9823},{-0.0114,0.0360,0.9993,84.0862},{ 0,0,0,1 } };
	double A[4][4] = { { 0.9992,0.0410,0.0014,118.9863},{ -0.0410,0.9989,-0.0229,20.2219},{-0.0023,0.0228,0.9997,79.7040},{ 0,0,0,1 } };
	double Ainv[4][4] = { { 0.9992,-0.0410,-0.0023,-117.8721},{ 0.0410,0.9989,0.0228,-26.8954},{0.0014,-0.0229,0.9997,-79.3864},{ 0,0,0,1 } };


	vtkSmartPointer<vtkLineSource> linex = vtkSmartPointer<vtkLineSource>::New();
	linex->SetPoint1(118.9863, 20.2219, 79.7040);
	linex->SetPoint2(118.9863 - 34.7634, 20.2219 - 35.3180, 79.7040 + 6.6443);
	vtkSmartPointer<vtkLineSource> liney = vtkSmartPointer<vtkLineSource>::New();
	liney->SetPoint1(118.9863, 20.2219, 79.7040);
	liney->SetPoint2(118.9863 - 4.3570, 20.2219 + 13.3181, 79.7040 + 47.9963);
	vtkSmartPointer<vtkLineSource> linez = vtkSmartPointer<vtkLineSource>::New();
	linez->SetPoint1(118.9863, 20.2219, 79.7040);
	linez->SetPoint2(118.9863 - 35.6724, 20.2219 + 32.7913, 79.7040 - 12.3372);
	vtkSmartPointer<vtkDataSetMapper> linex_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	linex_mapper->SetInputConnection(linex->GetOutputPort());
	vtkSmartPointer<vtkDataSetMapper> liney_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	liney_mapper->SetInputConnection(liney->GetOutputPort());
	vtkSmartPointer<vtkDataSetMapper> linez_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	linez_mapper->SetInputConnection(linez->GetOutputPort());
	vtkSmartPointer<vtkActor> linex_actor = vtkSmartPointer<vtkActor>::New();
	linex_actor->SetMapper(linex_mapper);
	vtkSmartPointer<vtkActor> liney_actor = vtkSmartPointer<vtkActor>::New();
	liney_actor->SetMapper(liney_mapper);
	vtkSmartPointer<vtkActor> linez_actor = vtkSmartPointer<vtkActor>::New();
	linez_actor->SetMapper(linez_mapper);
	linex_actor->GetProperty()->SetColor(1.0, 0, 0);
	liney_actor->GetProperty()->SetColor(0, 1.0, 0);
	linez_actor->GetProperty()->SetColor(0, 0, 1.0);

	vtkSmartPointer<vtkSphereSource> sphereSource1 =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource1->SetCenter(118.9863, 20.2219, 79.7040);   // 设置中心
	sphereSource1->SetRadius(5.0);             // 设置半径
	vtkSmartPointer<vtkPolyDataMapper> mapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputConnection(sphereSource1->GetOutputPort());    // 设置映射的渲染数据
	vtkSmartPointer<vtkActor> actor1 =
		vtkSmartPointer<vtkActor>::New();
	actor1->SetMapper(mapper1);
	actor1->GetProperty()->SetColor(1, 0, 0);

	vtkSmartPointer<vtkSphereSource> sphereSource2 =
		vtkSmartPointer<vtkSphereSource>::New();
	//sphereSource2->SetCenter(171.6105, 180.7488, 75.4992);   // 设置中心
	sphereSource2->SetCenter(94.2400, 51.8401, 118.2966);   // 设置中心
	sphereSource2->SetRadius(5.0);             // 设置半径
	vtkSmartPointer<vtkPolyDataMapper> mapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper2->SetInputConnection(sphereSource2->GetOutputPort());    // 设置映射的渲染数据
	vtkSmartPointer<vtkActor> actor2 =
		vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper(mapper2);
	actor2->GetProperty()->SetColor(0, 1, 0);

	vtkSmartPointer<vtkSphereSource> sphereSource3 =
		vtkSmartPointer<vtkSphereSource>::New();
	//sphereSource3->SetCenter(124.0133, 164.6181, 17.6531);   // 设置中心
	sphereSource3->SetCenter(55.3774, 78.6934, 57.7049);   // 设置中心
	sphereSource3->SetRadius(5.0);             // 设置半径
	vtkSmartPointer<vtkPolyDataMapper> mapper3 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper3->SetInputConnection(sphereSource3->GetOutputPort());    // 设置映射的渲染数据
	vtkSmartPointer<vtkActor> actor3 =
		vtkSmartPointer<vtkActor>::New();
	actor3->SetMapper(mapper3);
	actor3->GetProperty()->SetColor(0, 0, 1);

	vtkSmartPointer<vtkSphereSource> sphereSource4 =
		vtkSmartPointer<vtkSphereSource>::New();
	//sphereSource4->SetCenter(101.9393, 216.9324, 48.3700);   // 设置中心
	sphereSource4->SetCenter(97.5348, 34.1370, 36.8968);   // 设置中心
	sphereSource4->SetRadius(5.0);             // 设置半径
	vtkSmartPointer<vtkPolyDataMapper> mapper4 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper4->SetInputConnection(sphereSource4->GetOutputPort());    // 设置映射的渲染数据
	vtkSmartPointer<vtkActor> actor4 =
		vtkSmartPointer<vtkActor>::New();
	actor4->SetMapper(mapper4);
	actor4->GetProperty()->SetColor(1, 1, 0);


	renderer01 = vtkSmartPointer<vtkRenderer>::New();
	renderer02 = vtkSmartPointer<vtkRenderer>::New();
	renderer03 = vtkSmartPointer<vtkRenderer>::New();

	renderer01->AddActor(line10_actor);
	renderer01->AddActor(actorWorld);
	renderer01->AddActor(actorWorld2);
	renderer01->SetBackground(0.1, 0.1, 0.1);
	renderer01->AddActor(linex_actor);
	renderer01->AddActor(liney_actor);
	renderer01->AddActor(linez_actor);
	renderer01->AddActor(actor1);
	renderer01->AddActor(actor2);
	renderer01->AddActor(actor3);
	renderer01->AddActor(actor4);

	renderer02->AddActor(line10_actor);
	renderer03->AddActor(line10_actor);
	renderer01->AddActor(actor2);

	renderer01->SetBackground(0.1, 0.1, 0.1);
	//渲染时使用同样的数据
	renderer01->AddVolume(volume);
	renderer02->AddVolume(volume);
	//renderer02->SetBackground(.2, .3, .5);
	renderer02->SetViewport(leftViewStation);
	renderer03->AddVolume(volume);
	//renderer03->SetBackground(.2, .3, .5);
	renderer03->SetViewport(rightViewStation);

	renderWindow01 = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow02 = vtkSmartPointer<vtkRenderWindow>::New();

	renderWindow01->AddRenderer(renderer01);
	renderWindow02->AddRenderer(renderer02);
	renderWindow02->AddRenderer(renderer03);
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//windowInteractor01->SetRenderWindow(renderWindow01);
	windowInteractor02->SetRenderWindow(renderWindow02);

	vtkSmartPointer<vtkMyCameraCallback> cameraCallback1 = vtkSmartPointer<vtkMyCameraCallback>::New();
	cameraCallback1->m_renderer = renderer01;
	vtkSmartPointer<vtkMyCameraCallback> cameraCallback2 = vtkSmartPointer<vtkMyCameraCallback>::New();
	cameraCallback2->m_renderer = renderer02;
	renderer01->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback1);
	renderer02->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback2);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	windowInteractor01->SetInteractorStyle(style);

	auto vt = vtkTransform::New();
	auto mat1 = vtkMatrix4x4::New();
	mat1->SetElement(0, 0, 0.5);
	mat1->SetElement(1, 1, 1);
	mat1->SetElement(2, 2, 1);
	mat1->SetElement(3, 3, 1);
	vt->SetMatrix(mat1);
	renderer02->GetActiveCamera()->SetUserTransform(vt);
	renderer03->GetActiveCamera()->SetUserTransform(vt);

	//双眼距离
	//int value = 0;
	auto camLeft = renderer02->GetActiveCamera();
	auto camRight = renderer03->GetActiveCamera();
	double x = 0 + dist / 100.0;

	camRight->SetPosition(camLeft->GetPosition()[0] + x, camLeft->GetPosition()[1], camLeft->GetPosition()[2]);
	camRight->SetRoll(camLeft->GetRoll());
	renderer02->ResetCameraClippingRange();
	renderer03->ResetCameraClippingRange();
	windowInteractor02->Render();

	ui->qvtkWidget_4->SetRenderWindow(renderWindow01);		// make the link between VTK and QT
	ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer01);
	windowInteractor01->SetRenderWindow(renderWindow01);

	ui->qvtkWidget_4->show();
	renderWindow01->Render();
	renderer01->ResetCamera();
	//renderer02->ResetCamera();

	windowInteractor01->Initialize();
	windowInteractor02->Initialize();
	windowInteractor01->Start();
	windowInteractor02->Start();



	}

}

//切割平面+双眼三维显示
void MainWindow::test2()
{
	//MRI面绘制读取实例
	//vtkSmartPointer<vtkRenderer> aRenderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	//renWin->AddRenderer(aRenderer);

	//vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//iren->SetRenderWindow(renWin);

	//vtkSmartPointer<vtkDICOMImageReader> v16 = vtkSmartPointer<vtkDICOMImageReader>::New();
	//v16->SetDataByteOrderToLittleEndian();
	//folder = ui->label_2->text().toStdString();
	//using ReaderType = itk::ImageSeriesReader< ImageType >;
	//using ImageIOType = itk::GDCMImageIO;
	//using NamesGeneratorType = itk::GDCMSeriesFileNames;
	//ReaderType::Pointer reader = ReaderType::New();
	//ImageIOType::Pointer dicomIO = ImageIOType::New();
	//reader->SetImageIO(dicomIO);
	//NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	//nameGenerator->SetUseSeriesDetails(true);
	//nameGenerator->SetDirectory(folder.c_str());
	//v16->SetDirectoryName(folder.c_str());// 文件目录path
	//v16->SetDataSpacing(3.2, 3.2, 1.5);

	//vtkSmartPointer<vtkContourFilter> skinExtractor = vtkSmartPointer<vtkContourFilter>::New();
	//skinExtractor->SetInputConnection(v16->GetOutputPort());
	//skinExtractor->SetValue(0, 500);

	//vtkSmartPointer<vtkPolyDataNormals> skinNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
	//skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
	//skinNormals->SetFeatureAngle(60.0);

	//vtkSmartPointer<vtkPolyDataMapper> skinMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//skinMapper->SetInputConnection(skinNormals->GetOutputPort());
	//skinMapper->ScalarVisibilityOff();

	//vtkSmartPointer<vtkActor> skin = vtkSmartPointer<vtkActor>::New();
	//skin->SetMapper(skinMapper);

	//// 
	//vtkSmartPointer<vtkOutlineFilter> outlineData = vtkSmartPointer<vtkOutlineFilter>::New();
	//outlineData->SetInputConnection(v16->GetOutputPort());

	//vtkSmartPointer<vtkPolyDataMapper> mapOutline = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapOutline->SetInputConnection(outlineData->GetOutputPort());

	//vtkSmartPointer<vtkActor> outline = vtkSmartPointer<vtkActor>::New();
	//outline->SetMapper(mapOutline);
	//outline->GetProperty()->SetColor(0, 0, 0);

	////
	//vtkSmartPointer<vtkCamera> aCamera = vtkSmartPointer<vtkCamera>::New();
	//aCamera->SetViewUp(0, 0, -1);
	//aCamera->SetPosition(0, 1, 0);
	//aCamera->SetFocalPoint(0, 0, 0);
	//aCamera->ComputeViewPlaneNormal();
	//aCamera->Azimuth(30.0); //New
	//aCamera->Elevation(30.0);//New

	////
	//aRenderer->AddActor(outline);
	//aRenderer->AddActor(skin);
	//aRenderer->SetActiveCamera(aCamera);
	//aRenderer->ResetCamera();
	//aCamera->Dolly(1.5);

	////
	//aRenderer->SetBackground(1, 1, 1);// can change
	//renWin->SetSize(640, 480);

	////
	//aRenderer->ResetCameraClippingRange();

	////
	//iren->Initialize();
	//iren->Start();

	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	aRenderer->SetViewport(0, 0.0, 0.5, 1.0);
	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(aRenderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	windowInteractor01->SetRenderWindow(renWin);

	folder = ui->label_2->text().toStdString();
	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();

	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputData(reader->GetOutput());
	skinExtractor->SetValue(0, 200);    //值越大，保留的部分越少。

	/**做平滑处理**/
	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
	smooth->SetInputConnection(skinExtractor->GetOutputPort());
	smooth->SetNumberOfIterations(100);

	////重新计算法向量
	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();

	skinNormals->SetInputConnection(smooth->GetOutputPort());
	skinNormals->SetFeatureAngle(60);

	vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();    //这样不会带颜色

	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);

	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(reader->GetOutputPort());
	//outlineData->SetInputData(itkTovtkImageFilter->GetOutput());
	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0, 0, 0);

	vtkSmartPointer<vtkCamera> aCamera =
		vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, 1, 0);
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);
	aCamera->Dolly(1.5);
	aRenderer->AddActor(outline);
	aRenderer->AddActor(skin);
	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	//aRenderer->SetBackground(.2, .3, .4);
	aRenderer->SetBackground(.15, .15, .15);
	aRenderer->ResetCameraClippingRange();

	renWin->SetSize(1000, 1000);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	windowInteractor01->SetInteractorStyle(style);

	/////////设置截面
	vtkSmartPointer<vtkClipPolyData> cliper = vtkClipPolyData::New();
	cliper->SetInputData(skinStripper->GetOutput());

	// 此平面box可以通过右键来进行放大缩小处理（只有当鼠标控制区域只有切割体才单一有效）
	vtkSmartPointer<vtkImplicitPlaneWidget> implicitPlaneWidget = vtkImplicitPlaneWidget::New();

	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	implicitPlaneWidget->SetInteractor(windowInteractor01);
	implicitPlaneWidget->SetPlaceFactor(1.25);
	//initially position the widget
	implicitPlaneWidget->SetInputData(skinStripper->GetOutput());
	implicitPlaneWidget->PlaceWidget();


	//////Render2
	vtkSmartPointer<vtkActor> coneSkinActor = vtkActor::New();

	vtkSmartPointer<vtkTransform> trans =
		vtkSmartPointer<vtkTransform>::New();
	trans->PostMultiply();
	trans->Translate(1, 0, 0);
	trans->RotateX(90);
	coneSkinActor->SetUserTransform(trans);

	coneSkinActor->SetMapper(skinMapper);

	vtkSmartPointer<vtkRenderer> rRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	rRenderer->SetBackground(0.2, 0.2, 0.2);
	//rRenderer->SetBackground(0.2, 0.3, 0.5);
	rRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	rRenderer->AddActor(coneSkinActor);


	vtkSmartPointer<BuildVTKWidgetCall> pCall = BuildVTKWidgetCall::New();
	pCall->setActor(coneSkinActor);
	pCall->setCliper(cliper);

	renWin->AddRenderer(rRenderer);
	///////
	implicitPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);
	implicitPlaneWidget->On();


	renderWindow01 = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow01->AddRenderer(aRenderer);
	renderWindow01->AddRenderer(rRenderer);
	windowInteractor01->SetRenderWindow(renderWindow01);

	////3D联动
	//vtkSmartPointer<vtkMyCameraCallback> cameraCallback1 = vtkSmartPointer<vtkMyCameraCallback>::New();
	//cameraCallback1->m_renderer = rRenderer;
	//rRenderer->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback1);

	//double leftViewStation[4] = { 0.0, 0.0, 0.5, 1.0 };
	//double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };

	//renderer01 = vtkSmartPointer<vtkRenderer>::New();
	//renderer02 = vtkSmartPointer<vtkRenderer>::New();
	//renderer03 = vtkSmartPointer<vtkRenderer>::New();

	////渲染时使用同样的数据
	//renderer01->AddActor(coneSkinActor);
	//renderer02->AddActor(coneSkinActor);
	////renderer02->SetBackground(.2, .3, .5);
	//renderer02->SetViewport(leftViewStation);
	//renderer03->AddActor(coneSkinActor);
	////renderer03->SetBackground(.2, .3, .5);
	//renderer03->SetViewport(rightViewStation);

	//renderWindow02 = vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindow02->SetSize(2000, 1000);
	////renderWindow02->SetFullScreen(1);
	//renderWindow02->AddRenderer(renderer02);
	//renderWindow02->AddRenderer(renderer03);
	////vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	////vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//windowInteractor02->SetRenderWindow(renderWindow02);

	//vtkSmartPointer<vtkMyCameraCallback> cameraCallback2 = vtkSmartPointer<vtkMyCameraCallback>::New();
	//cameraCallback2->m_renderer = renderer02;
	//renderer02->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback2);

	////implicitPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);
	////implicitPlaneWidget->On();
	//	
	////vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
	////	vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

	//auto oriPosCam1 = renderer02->GetActiveCamera()->GetPosition();
	//auto oriFocCam1 = renderer02->GetActiveCamera()->GetFocalPoint();
	//auto oriPosCam2 = renderer03->GetActiveCamera()->GetPosition();
	//auto oriFocCam2 = renderer03->GetActiveCamera()->GetFocalPoint();

	//auto vt = vtkTransform::New();
	//auto mat1 = vtkMatrix4x4::New();
	//mat1->SetElement(0, 0, 0.5);
	//mat1->SetElement(1, 1, 1);
	//mat1->SetElement(2, 2, 1);
	//mat1->SetElement(3, 3, 1);
	//vt->SetMatrix(mat1);

	//rRenderer->ResetCamera();
	////偏振
	//renderer02->GetActiveCamera()->SetUserTransform(vt);
	//renderer03->GetActiveCamera()->SetUserTransform(vt);

	//renderer02->ResetCamera();
	//renderer03->ResetCamera();

	windowInteractor01->SetInteractorStyle(style);

	ui->qvtkWidget_4->SetRenderWindow(renderWindow01);		// make the link between VTK and QT
	ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(rRenderer);
	windowInteractor01->SetRenderWindow(renderWindow01);

	ui->qvtkWidget_4->show();
	renderWindow01->Render();

	windowInteractor01->Initialize();
	windowInteractor02->Initialize();
	windowInteractor01->Start();
	windowInteractor02->Start();
}


//插一根管子
#include <vtkCylinderSource.h>
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>
#include <vtkDoubleArray.h>
#include <vtkTupleInterpolator.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkSmartPointer.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkTubeFilter.h>
#include <vtkLineSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
/*
void MainWindow::test4()
{
	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	aRenderer->SetViewport(0, 0.0, 0.5, 1.0);
	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(aRenderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	windowInteractor01->SetRenderWindow(renWin);

	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
	folder = ui->label_2->text().toStdString();
	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();


	if (!reader->GetOutputPort())
	{
		cout << "open failed" << endl;
	}
	cout << "读取数据完成" << endl;

	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection(reader->GetOutputPort());
	skinExtractor->SetValue(0, 1);    //值越大，保留的部分越少。

	//做平滑处理
	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
	smooth->SetInputConnection(skinExtractor->GetOutputPort());
	smooth->SetNumberOfIterations(100);

	//重新计算法向量
	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();

	skinNormals->SetInputConnection(smooth->GetOutputPort());
	skinNormals->SetFeatureAngle(180);

	vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();    //这样不会带颜色

	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);

	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0, 0, 0);

	vtkSmartPointer<vtkCamera> aCamera =
		vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, 1, 0);
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);
	aCamera->Dolly(1.5);
	aRenderer->AddActor(outline);
	aRenderer->AddActor(skin);


	
	// Create a line
	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(65, 130, 90);
	lineSource->SetPoint2(95, 130, 90);

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> lineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	lineMapper->SetInputConnection(lineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> lineActor =
		vtkSmartPointer<vtkActor>::New();
	lineActor->GetProperty()->SetColor(0.0, 0.0, 0.1); // Give some color to the line
	lineActor->SetMapper(lineMapper);

	// Create a tube (cylinder) around the line
	vtkSmartPointer<vtkTubeFilter> tubeFilter =
		vtkSmartPointer<vtkTubeFilter>::New();
	tubeFilter->SetInputConnection(lineSource->GetOutputPort());
	tubeFilter->SetRadius(2); //default is .5
	tubeFilter->SetNumberOfSides(50);
	tubeFilter->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> tubeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	tubeMapper->SetInputConnection(tubeFilter->GetOutputPort());
	
	vtkSmartPointer<vtkActor> tubeActor =
		vtkSmartPointer<vtkActor>::New();
	tubeActor->GetProperty()->SetOpacity(0.5); //Make the tube have some transparency.
	tubeActor->SetMapper(tubeMapper);
	tubeActor->GetProperty()->SetColor(0.0, 0.4, 0.0); // Give some color to the line

	// Add the actor to the scene
	aRenderer->AddActor(tubeActor);
	aRenderer->AddActor(lineActor);
	
	










	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	points->InsertPoint(80, 150, 100,0);
	points->InsertPoint(90, 140, 100, 0);
	points->InsertPoint(100, 150, 100, 0);
	points->InsertPoint(110, 160, 100, 0);
	points->InsertPoint(120, 140, 100, 0);
	points->InsertPoint(130, 150, 100, 0);

	// Fit a spline to the points
	
	vtkSmartPointer<vtkParametricSpline> spline =
		vtkSmartPointer<vtkParametricSpline>::New();
	spline->SetPoints(points);
	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(spline);
	functionSource->SetUResolution(10 * points->GetNumberOfPoints());
	functionSource->Update();

	// Interpolate the scalars
	double rad;
	vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius =
		vtkSmartPointer<vtkTupleInterpolator> ::New();
	interpolatedRadius->SetInterpolationTypeToLinear();
	interpolatedRadius->SetNumberOfComponents(1);
	rad = 3; interpolatedRadius->AddTuple(0, &rad);
	rad = 2; interpolatedRadius->AddTuple(1, &rad);
	rad = 1.5; interpolatedRadius->AddTuple(2, &rad);
	rad = 1.5; interpolatedRadius->AddTuple(3, &rad);
	rad = 1; interpolatedRadius->AddTuple(4, &rad);
	rad = 1; interpolatedRadius->AddTuple(5, &rad);

	// Generate the radius scalars
	vtkSmartPointer<vtkDoubleArray> tubeRadius =
		vtkSmartPointer<vtkDoubleArray>::New();
	unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
	tubeRadius->SetNumberOfTuples(n);
	tubeRadius->SetName("TubeRadius");
	double tMin = interpolatedRadius->GetMinimumT(); std::cout << "tMin:" << tMin;
	double tMax = interpolatedRadius->GetMaximumT(); std::cout << "tMax:" << tMax;
	double r;
	for (unsigned int i = 0; i < n; ++i)
	{
		double t = (tMax - tMin) / (n - 1) * i + tMin;
		interpolatedRadius->InterpolateTuple(t, &r);
		tubeRadius->SetTuple1(i, r);
	}

	// Add the scalars to the polydata
	vtkSmartPointer<vtkPolyData> tubePolyData =
		vtkSmartPointer<vtkPolyData>::New();
	tubePolyData = functionSource->GetOutput();
	tubePolyData->GetPointData()->AddArray(tubeRadius);
	tubePolyData->GetPointData()->SetActiveScalars("TubeRadius");

	// Create the tubes
	vtkSmartPointer<vtkTubeFilter> tuber =
		vtkSmartPointer<vtkTubeFilter>::New();
#if VTK_MAJOR_VERSION <= 5
	tuber->SetInput(tubePolyData);
#else
	tuber->SetInputData(tubePolyData);
#endif
	tuber->SetNumberOfSides(50);
	tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();

	//--------------
	// Setup actors and mappers
	vtkSmartPointer<vtkPolyDataMapper> lineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
	lineMapper->SetInput(tubePolyData);
#else
	lineMapper->SetInputData(tubePolyData);
#endif
	lineMapper->SetScalarRange(tubePolyData->GetScalarRange());

	vtkSmartPointer<vtkPolyDataMapper> tubeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	tubeMapper->SetInputConnection(tuber->GetOutputPort());
	tubeMapper->SetScalarRange(tubePolyData->GetScalarRange());
	
	vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
	lineActor->SetMapper(lineMapper);
	vtkSmartPointer<vtkActor> tubeActor = vtkSmartPointer<vtkActor>::New();
	tubeActor->SetMapper(tubeMapper);
	tubeActor->GetProperty()->SetOpacity(0.3);

	aRenderer->AddActor(tubeActor);
	aRenderer->AddActor(lineActor);

	















	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	aRenderer->SetBackground(.2, .3, .4);
	aRenderer->ResetCameraClippingRange();

	renWin->SetSize(1000, 1000);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	windowInteractor01->SetInteractorStyle(style);

	/////////设置截面
	vtkSmartPointer<vtkClipPolyData> cliper = vtkClipPolyData::New();
	cliper->SetInputData(skinStripper->GetOutput());

	// 此平面box可以通过右键来进行放大缩小处理（只有当鼠标控制区域只有切割体才单一有效）
	vtkSmartPointer<vtkImplicitPlaneWidget> implicitPlaneWidget = vtkImplicitPlaneWidget::New();

	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	implicitPlaneWidget->SetInteractor(windowInteractor01);
	implicitPlaneWidget->SetPlaceFactor(1.25);
	//initially position the widget
	implicitPlaneWidget->SetInputData(skinStripper->GetOutput());
	implicitPlaneWidget->PlaceWidget();


	//////Render2
	vtkSmartPointer<vtkActor> coneSkinActor = vtkActor::New();

	vtkSmartPointer<vtkTransform> trans =
		vtkSmartPointer<vtkTransform>::New();
	trans->PostMultiply();
	trans->Translate(1, 0, 0);
	trans->RotateX(90);
	coneSkinActor->SetUserTransform(trans);

	coneSkinActor->SetMapper(skinMapper);

	vtkSmartPointer<vtkRenderer> rRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	rRenderer->SetBackground(0.2, 0.3, 0.5);
	rRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	rRenderer->AddActor(coneSkinActor);

	//vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	//actor2->SetMapper(vesselMapper);
	//actor2->GetProperty()->SetColor(0, 0, 0);
	//actor2->SetUserTransform(trans);
	vtkSmartPointer<vtkActor> lineActor2 =
		vtkSmartPointer<vtkActor>::New();
	lineActor2->GetProperty()->SetColor(0.0, 0.0, 0.1); // Give some color to the line
	lineActor2->SetMapper(lineMapper);
	vtkSmartPointer<vtkActor> tubeActor2 =
		vtkSmartPointer<vtkActor>::New();
	tubeActor2->GetProperty()->SetOpacity(0.5); //Make the tube have some transparency.
	tubeActor2->SetMapper(tubeMapper);
	tubeActor2->GetProperty()->SetColor(0.0, 0.4, 0.0); // Give some color to the line

	tubeActor2->SetUserTransform(trans);
	lineActor2->SetUserTransform(trans);

	rRenderer->AddActor(tubeActor2);
	rRenderer->AddActor(lineActor2);

	//rRenderer->AddActor(actor2);

	vtkSmartPointer<BuildVTKWidgetCall> pCall = BuildVTKWidgetCall::New();
	pCall->setActor(coneSkinActor);
	pCall->setCliper(cliper);

	renWin->AddRenderer(rRenderer);
	///////
	implicitPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);
	implicitPlaneWidget->On();

	//renderWindow01 = vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindow02 = vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindow02->SetSize(2000, 1000);
	////renderWindow02->SetFullScreen(1);
	//renderWindow01->AddRenderer(aRenderer);
	//renderWindow01->AddRenderer(rRenderer);
	////vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	////vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//windowInteractor01->SetRenderWindow(renderWindow01);
	//windowInteractor02->SetRenderWindow(renderWindow02);

	windowInteractor01->SetInteractorStyle(style);

	ui->qvtkWidget_4->SetRenderWindow(renWin);		// make the link between VTK and QT
	ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(rRenderer);
	windowInteractor01->SetRenderWindow(renWin);

	ui->qvtkWidget_4->show();
	renWin->Render();

	windowInteractor01->Initialize();
	windowInteractor02->Initialize();
	windowInteractor01->Start();
	windowInteractor02->Start();
}

*/

#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkCamera.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkInteractorStyleJoystickCamera.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkLODActor.h"
#include "vtkLight.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPropPicker.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkSTLReader.h"
#include "vtkOBJReader.h"
#include "vtkShrinkPolyData.h"

#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkPoints.h"
#include "vtkPolyVertex.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"


#include <vtkNew.h>
#include <vtkConeSource.h>
#include <vtkRenderWindow.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballActor.h>

#include <iostream>
#include <vector>
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkPoints.h"
#include "vtkPolyVertex.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkInteractorStyleTrackball.h"


void MainWindow::test4()
{

	//std::string inputFilename1 = "E:\\NK\\18-L\\18-L-股骨髁间窝切骨导板.stl";
	//std::string inputFilename2 = "E:\\NK\\18-L\\18-L-股骨四合一切骨导板.stl";
	//std::string inputFilename3 = "E:\\NK\\18-L\\18-L-股骨远端切骨导板(软骨).stl";
	//std::string inputFilename4 = "E:\\NK\\18-L\\18-L-胫骨平台二合一切骨导板.stl";
	//std::string inputFilename5 = "E:\\NK\\18-L\\18-L-胫骨平台后倾切骨导板(软骨).stl";
	//std::string inputFilename6 = "E:\\NK\\18-L\\标准化飞机翼.stl";

	//vtkSmartPointer<vtkSTLReader> reader1 =
	//	vtkSmartPointer<vtkSTLReader>::New();
	//reader1->SetFileName(inputFilename1.c_str());
	//reader1->Update();
	//vtkSmartPointer<vtkSTLReader> reader2 =
	//	vtkSmartPointer<vtkSTLReader>::New();
	//reader2->SetFileName(inputFilename2.c_str());
	//reader2->Update();
	//vtkSmartPointer<vtkSTLReader> reader3 =
	//	vtkSmartPointer<vtkSTLReader>::New();
	//reader3->SetFileName(inputFilename3.c_str());
	//reader3->Update();
	//vtkSmartPointer<vtkSTLReader> reader4 =
	//	vtkSmartPointer<vtkSTLReader>::New();
	//reader4->SetFileName(inputFilename4.c_str());
	//reader4->Update();
	//vtkSmartPointer<vtkSTLReader> reader5 =
	//	vtkSmartPointer<vtkSTLReader>::New();
	//reader5->SetFileName(inputFilename5.c_str());
	//reader5->Update();
	//vtkSmartPointer<vtkSTLReader> reader6 =
	//	vtkSmartPointer<vtkSTLReader>::New();
	//reader6->SetFileName(inputFilename6.c_str());
	//reader6->Update();
	//
	//vtkSmartPointer<vtkPolyDataMapper> mapper1 =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper1->SetInputConnection(reader1->GetOutputPort());
	//vtkSmartPointer<vtkPolyDataMapper> mapper2 =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper2->SetInputConnection(reader2->GetOutputPort());
	//vtkSmartPointer<vtkPolyDataMapper> mapper3 =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper3->SetInputConnection(reader3->GetOutputPort());
	//vtkSmartPointer<vtkPolyDataMapper> mapper4 =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper4->SetInputConnection(reader4->GetOutputPort());
	//vtkSmartPointer<vtkPolyDataMapper> mapper5 =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper5->SetInputConnection(reader5->GetOutputPort());
	//vtkSmartPointer<vtkPolyDataMapper> mapper6 =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper6->SetInputConnection(reader6->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor1 =
	//	vtkSmartPointer<vtkActor>::New();
	//actor1->SetMapper(mapper1);
	//vtkSmartPointer<vtkActor> actor2 =
	//	vtkSmartPointer<vtkActor>::New();
	//actor2->SetMapper(mapper2);
	//vtkSmartPointer<vtkActor> actor3 =
	//	vtkSmartPointer<vtkActor>::New();
	//actor3->SetMapper(mapper3);
	//vtkSmartPointer<vtkActor> actor4 =
	//	vtkSmartPointer<vtkActor>::New();
	//actor4->SetMapper(mapper4);
	//vtkSmartPointer<vtkActor> actor5 =
	//	vtkSmartPointer<vtkActor>::New();
	//actor5->SetMapper(mapper5);
	//vtkSmartPointer<vtkActor> actor6 =
	//	vtkSmartPointer<vtkActor>::New();
	//actor6->SetMapper(mapper6);

	//vtkTransform *myTrans1 = vtkTransform::New();
	//myTrans1->Translate(0, 0, 0);
	//myTrans1->RotateY(-90);
	//actor1->SetUserTransform(myTrans1);
	//vtkTransform *myTrans2 = vtkTransform::New();
	//myTrans2->Translate(0, 0, 200);
	//myTrans2->RotateY(-90);
	//actor2->SetUserTransform(myTrans2);
	//vtkTransform *myTrans3 = vtkTransform::New();
	//myTrans3->Translate(0, 0, 400);
	//myTrans3->RotateY(-90);
	//actor3->SetUserTransform(myTrans3);
	//vtkTransform *myTrans4 = vtkTransform::New();
	//myTrans4->Translate(0, 100, 0);
	//myTrans4->RotateY(-90);
	//actor4->SetUserTransform(myTrans4);
	//vtkTransform *myTrans5 = vtkTransform::New();
	//myTrans5->Translate(0, 100, 200);
	//myTrans5->RotateY(-90);
	//actor5->SetUserTransform(myTrans5);
	//vtkTransform *myTrans6 = vtkTransform::New();
	//myTrans6->Translate(0, 100, 400);
	//myTrans6->RotateY(-90);
	////myTrans6->RotateZ(90);
	////myTrans6->Scale(0.1, 0.1, 0.1);
	//actor6->SetUserTransform(myTrans6);

	//vtkSmartPointer<vtkRenderer> renderer =
	//	vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow =
	//	vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindow->AddRenderer(renderer);

	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
	//	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//renderWindowInteractor->SetRenderWindow(renderWindow);

	//renderer->AddActor(actor1);
	//renderer->AddActor(actor2);
	//renderer->AddActor(actor3);
	//renderer->AddActor(actor4);
	//renderer->AddActor(actor5);
	//renderer->AddActor(actor6);
	//renderer->SetBackground(0.2, 0.3, 0.5);

	//vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
	//	vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

	//renderWindowInteractor->SetInteractorStyle(style);

	//ui->qvtkWidget_4->SetRenderWindow(renderWindow);		// make the link between VTK and QT
	//ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer);
	////windowInteractor01->SetRenderWindow(renWin);

	//ui->qvtkWidget_4->show();

	//renderWindow->Render();
	//renderWindowInteractor->Start();
	//
	vtkSmartPointer<vtkRenderer> aRenderer =
	vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> rRenderer =
	vtkSmartPointer<vtkRenderer>::New();
	aRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);
	rRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	vtkSmartPointer<vtkRenderWindow> renWin =
	vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(aRenderer);
	vtkSmartPointer<vtkRenderWindowInteractor> iren =
	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	windowInteractor01->SetRenderWindow(renWin);

	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
	folder = ui->label_2->text().toStdString();
	vtkSmartPointer<vtkDICOMImageReader> reader =
	vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();

	vtkSmartPointer<vtkContourFilter> skinExtractor =
	vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection(reader->GetOutputPort());
	skinExtractor->SetValue(0, 1);    //值越大，保留的部分越少。

	//做平滑处理
	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
	smooth->SetInputConnection(skinExtractor->GetOutputPort());
	smooth->SetNumberOfIterations(100);

	//重新计算法向量
	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
	vtkSmartPointer<vtkPolyDataNormals>::New();

	skinNormals->SetInputConnection(smooth->GetOutputPort());
	skinNormals->SetFeatureAngle(180);

	vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
	vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
	vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();    //这样不会带颜色

	vtkSmartPointer<vtkActor> skin =
	vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);

	vtkSmartPointer<vtkOutlineFilter> outlineData =
	vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
	vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
	vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0, 0, 0);

	vtkSmartPointer<vtkCamera> aCamera =
	vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, 1, 0);
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);
	aCamera->Dolly(1.5);
	aRenderer->AddActor(outline);
	aRenderer->AddActor(skin);


	auto colors =
	vtkSmartPointer<vtkNamedColors>::New();
	
	std::string filename = "E:\\NK\\03_material\\brain\\eight.obj";

	vtkSmartPointer<vtkOBJReader> reader2 =
	vtkSmartPointer<vtkOBJReader>::New();
	reader2->SetFileName(filename.c_str());
	reader2->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper =
	vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader2->GetOutputPort());

	vtkSmartPointer<vtkActor> actor =
	vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkTransform *myTrans = vtkTransform::New();

	myTrans->Translate(200, 100, -780);
	//myTrans->RotateZ(90);
	//myTrans->Scale(0.3, 0.3, 0.3);
	actor->SetUserTransform(myTrans);
	//vtkPoints *m_Points = vtkPoints::New();
	//vtkCellArray *vertices = vtkCellArray::New();	//_存放细胞顶点，用于渲染（显示点云所必须的）
	//vtkPolyData *polyData = vtkPolyData::New();
	//vtkPolyDataMapper *pointMapper = vtkPolyDataMapper::New();
	//vtkActor *pointActor = vtkActor::New();

	////_读进点云数据信息
	//FILE*fp = NULL; fp = fopen("E:\NK\301\point.txt", "r");	//2DpointDatas.txt
	//if (!fp)
	//{
	//	printf("打开文件失败！！\n");
	//	int m;
	//	cin >> m;
	//	exit(0);
	//}
	//double x = 0, y = 0, z = 0;
	//int i = 0;
	//while (!feof(fp))
	//{
	//	fscanf(fp, "%lf %lf %lf", &x, &y, &z);
	//	m_Points->InsertPoint(i, x, y, z);		//_加入点信息
	//	vertices->InsertNextCell(1);		//_加入细胞顶点信息----用于渲染点集
	//	vertices->InsertCellPoint(i);
	//	i++;
	//}
	//fclose(fp);

	////_创建待显示数据源

	//polyData->SetPoints(m_Points);		//_设置点集
	//polyData->SetVerts(vertices);		//_设置渲染顶点
	//pointMapper->SetInputData(polyData);

	//pointActor->SetMapper(pointMapper);
	//pointActor->GetProperty()->SetColor(0.0, 0.1, 1.0);
	//pointActor->GetProperty()->SetAmbient(0.5);
	//pointActor->GetProperty()->SetPointSize(2);
	////pointActor->GetProperty()->SetRepresentationToWireframe();
	////pointActor->GetProperty()->SetRepresentationToSurface();

	
	// Add the actor to the scene
	//rRenderer->AddActor(pointActor);
	rRenderer->AddActor(actor);
	rRenderer->SetBackground(colors->GetColor3d("AliceBlue").GetData());

	// Render and interact
	renWin->Render();

	// Initialize must be called prior to creating timer events.
	windowInteractor01->Initialize();



	renWin->SetSize(1000, 1000);


	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	aRenderer->SetBackground(.2, .3, .4);
	aRenderer->ResetCameraClippingRange();

	renWin->SetSize(1000, 1000);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
	vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	windowInteractor01->SetInteractorStyle(style);

	/////////设置截面
	vtkSmartPointer<vtkClipPolyData> cliper = vtkClipPolyData::New();
	cliper->SetInputData(skinStripper->GetOutput());

	// 此平面box可以通过右键来进行放大缩小处理（只有当鼠标控制区域只有切割体才单一有效）
	vtkSmartPointer<vtkImplicitPlaneWidget> implicitPlaneWidget = vtkImplicitPlaneWidget::New();

	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	implicitPlaneWidget->SetInteractor(windowInteractor01);
	implicitPlaneWidget->SetPlaceFactor(1.25);
	//initially position the widget
	implicitPlaneWidget->SetInputData(skinStripper->GetOutput());
	implicitPlaneWidget->PlaceWidget();


	//////Render2
	vtkSmartPointer<vtkActor> coneSkinActor = vtkActor::New();

	vtkSmartPointer<vtkTransform> trans =
	vtkSmartPointer<vtkTransform>::New();
	trans->PostMultiply();
	//trans->Translate(1, 0, 0);
	trans->RotateX(90);
	//coneSkinActor->SetUserTransform(trans);

	coneSkinActor->SetMapper(skinMapper);


	rRenderer->SetBackground(0.2, 0.3, 0.5);
	rRenderer->AddActor(coneSkinActor);
	
	vtkSmartPointer<BuildVTKWidgetCall> pCall = BuildVTKWidgetCall::New();
	pCall->setActor(coneSkinActor);
	pCall->setCliper(cliper);

	renWin->AddRenderer(rRenderer);
	///////
	implicitPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);
	implicitPlaneWidget->On();

	windowInteractor01->Initialize();
	windowInteractor01->Start();

	//std::string filename = "E:\\NK\\brain\\eight.obj";
	//vtkSmartPointer<vtkOBJReader> reader =
	//	vtkSmartPointer<vtkOBJReader>::New();
	//reader->SetFileName(filename.c_str());
	//reader->Update();

	//vtkSmartPointer<vtkPolyDataMapper> mapper =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(reader->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor =
	//	vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//vtkSmartPointer<vtkRenderer> renderer =
	//	vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow =
	//	vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindow->AddRenderer(renderer);
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
	//	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//renderWindowInteractor->SetRenderWindow(renderWindow);

	//renderer->AddActor(actor);
	//renderer->SetBackground(.3, .6, .3);

	//renderWindow->Render();
	//renderWindowInteractor->Start();


	//FILE *fp = NULL;
	//fp = fopen("E:\\NK\\brain\\eight.asc", "r");
	//
	//vtkRenderer *ren = vtkRenderer::New();
	//double arr[3];

	//vtkPoints * points = vtkPoints::New();
	//int n = 0;
	//while (!feof(fp))//首先读取点云数据到点表points同时指定点对应的id:
	//{
	//	int ret = fscanf(fp, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
	//	if (ret != 3)
	//		break;
	//	points->InsertPoint(n, arr[0], arr[1], arr[2]);
	//	n++;
	//}
	//printf("%d\n", n);
	//fclose(fp);

	//vtkPolyVertex * polyvertex = vtkPolyVertex::New();
	//polyvertex->GetPointIds()->SetNumberOfIds(n);
	//int i = 0;
	//for (i = 0; i < n; i++)//建立拓扑关系
	//{
	//	polyvertex->GetPointIds()->SetId(i, i);
	//}

	//vtkUnstructuredGrid * grid = vtkUnstructuredGrid::New();
	//grid->SetPoints(points);
	//grid->InsertNextCell(polyvertex->GetCellType(),
	//	polyvertex->GetPointIds());

	//vtkDataSetMapper *map1 = vtkDataSetMapper::New();
	//map1->SetInputData(grid);

	//vtkActor *actor1 = vtkActor::New();
	//actor1->SetMapper(map1);
	//actor1->GetProperty()->SetColor(0.194, 0.562, 0.75);

	//ren->AddActor(actor1);
	//ren->SetBackground(1, 1, 1);

	//vtkRenderWindow* win = vtkRenderWindow::New();
	//win->AddRenderer(ren);
	//win->SetSize(400, 400);
	//win->BordersOn();
	////win->FullScreenOn();
	////win->HideCursor();

	//vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

	//iren->SetRenderWindow(win);
	//vtkInteractorStyleTrackballCamera *style =
	//	vtkInteractorStyleTrackballCamera::New();
	//iren->SetInteractorStyle(style);

	//iren->Start();
	//ren->Delete();
	//win->Delete();
	//iren->Delete();
}


class renderCone
{
public:
	renderCone()
	{
		renderer =
			vtkSmartPointer<vtkRenderer>::New();
	}
	void SetChild(renderCone* child)
	{
		next = child;
	}
	void SetColor(double r, double g, double b)
	{
		renderer->SetBackground(r, g, b); // Background color dark red
	}
	void Render()
	{
		//Create a cone
		coneSource =
			vtkSmartPointer<vtkConeSource>::New();
		coneSource->Update();

		//Create a mapper and actor
		mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(coneSource->GetOutputPort());

		actor =
			vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);


		renderWindow =
			vtkSmartPointer<vtkRenderWindow>::New();
		renderWindow->AddRenderer(renderer);

		renderWindowInteractor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		renderWindowInteractor->SetRenderWindow(renderWindow);

		//Add the actors to the scene
		renderer->AddActor(actor);

		//Render and interact
		renderWindow->Render();
		if (next == nullptr)
		{
			renderWindowInteractor->Start();
		}
	}

private:
	vtkSmartPointer<vtkConeSource>		coneSource;
	vtkSmartPointer<vtkPolyDataMapper>	mapper;
	vtkSmartPointer<vtkActor>			actor;
	vtkSmartPointer<vtkRenderer>		renderer;
	vtkSmartPointer<vtkRenderWindow>	renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor>	renderWindowInteractor;
	renderCone* next{ nullptr };
};


/*
void MainWindow::test5()
{
	vtkSmartPointer<vtkConeSource> cone1 = vtkSmartPointer<vtkConeSource>::New();
	cone1->SetResolution(20);
	cone1->Update();
	vtkSmartPointer<vtkPolyDataMapper> coneMapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	coneMapper1->SetInputData(cone1->GetOutput());
	coneMapper1->Update();
	vtkSmartPointer<vtkActor> coneActor1 = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> cubeActor2 = vtkSmartPointer<vtkActor>::New();
	coneActor1->SetMapper(coneMapper1);

	renderer01 = vtkSmartPointer<vtkRenderer>::New();
	renderer02 = vtkSmartPointer<vtkRenderer>::New();

	//渲染时使用同样的数据
	renderer01->AddActor(coneActor1);
	renderer02->AddActor(coneActor1);

	renderWindow01 = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow02 = vtkSmartPointer<vtkRenderWindow>::New();

	renderWindow01->AddRenderer(renderer01);
	renderWindow02->AddRenderer(renderer02);

	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//windowInteractor01->SetRenderWindow(renderWindow01);
	windowInteractor02->SetRenderWindow(renderWindow02);

	vtkSmartPointer<vtkMyCameraCallback> cameraCallback1 = vtkSmartPointer<vtkMyCameraCallback>::New();
	cameraCallback1->m_renderer = renderer01;
	vtkSmartPointer<vtkMyCameraCallback> cameraCallback2 = vtkSmartPointer<vtkMyCameraCallback>::New();
	cameraCallback2->m_renderer = renderer02;
	renderer01->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback1);
	renderer02->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, cameraCallback2);

	//ui->qvtkWidget_4->SetRenderWindow(renderWindow01);		// make the link between VTK and QT
	//ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer01);
	windowInteractor01->SetRenderWindow(renderWindow01);

	ui->qvtkWidget_4->show();
	renderWindow01->Render();
	renderer01->ResetCamera();
	renderer02->ResetCamera();

	windowInteractor01->Initialize();
	windowInteractor02->Initialize();
	windowInteractor01->Start();
	windowInteractor02->Start();

}
*/
/*
void MainWindow::test5()
{
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->InsertPoint(0, 1, 0, 0);
	points->InsertPoint(1, 2, 0, 0);
	points->InsertPoint(2, 3, 1, 0);
	points->InsertPoint(3, 4, 1, 0);
	points->InsertPoint(4, 5, 0, 0);
	points->InsertPoint(5, 6, 0, 0);

	// Fit a spline to the points
	vtkSmartPointer<vtkParametricSpline> spline =
		vtkSmartPointer<vtkParametricSpline>::New();
	spline->SetPoints(points);
	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(spline);
	functionSource->SetUResolution(10 * points->GetNumberOfPoints());
	functionSource->Update();

	// Interpolate the scalars
	double rad;
	vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius =
		vtkSmartPointer<vtkTupleInterpolator> ::New();
	interpolatedRadius->SetInterpolationTypeToLinear();
	interpolatedRadius->SetNumberOfComponents(1);
	rad = .2; interpolatedRadius->AddTuple(0, &rad);
	rad = .2; interpolatedRadius->AddTuple(1, &rad);
	rad = .15; interpolatedRadius->AddTuple(2, &rad);
	rad = .15; interpolatedRadius->AddTuple(3, &rad);
	rad = .1; interpolatedRadius->AddTuple(4, &rad);
	rad = .1; interpolatedRadius->AddTuple(5, &rad);

	// Generate the radius scalars
	vtkSmartPointer<vtkDoubleArray> tubeRadius =
		vtkSmartPointer<vtkDoubleArray>::New();
	unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
	tubeRadius->SetNumberOfTuples(n);
	tubeRadius->SetName("TubeRadius");
	double tMin = interpolatedRadius->GetMinimumT(); std::cout << "tMin:" << tMin;
	double tMax = interpolatedRadius->GetMaximumT(); std::cout << "tMax:" << tMax;
	double r;
	for (unsigned int i = 0; i < n; ++i)
	{
		double t = (tMax - tMin) / (n - 1) * i + tMin;
		interpolatedRadius->InterpolateTuple(t, &r);
		tubeRadius->SetTuple1(i, r);
	}

	// Add the scalars to the polydata
	vtkSmartPointer<vtkPolyData> tubePolyData =
		vtkSmartPointer<vtkPolyData>::New();
	tubePolyData = functionSource->GetOutput();
//	tubePolyData->GetPointData()->AddArray(tubeRadius);
//	tubePolyData->GetPointData()->SetActiveScalars("TubeRadius");

	// Create the tubes
	vtkSmartPointer<vtkTubeFilter> tuber =
		vtkSmartPointer<vtkTubeFilter>::New();
#if VTK_MAJOR_VERSION <= 5
	tuber->SetInput(tubePolyData);
#else
	tuber->SetInputData(tubePolyData);
#endif
	tuber->SetNumberOfSides(50);
	tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();

	//--------------
	// Setup actors and mappers
	vtkSmartPointer<vtkPolyDataMapper> lineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
	lineMapper->SetInput(tubePolyData);
#else
	lineMapper->SetInputData(tubePolyData);
#endif
	lineMapper->SetScalarRange(tubePolyData->GetScalarRange());

	vtkSmartPointer<vtkPolyDataMapper> tubeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	tubeMapper->SetInputConnection(tuber->GetOutputPort());
	tubeMapper->SetScalarRange(tubePolyData->GetScalarRange());

	vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
	lineActor->SetMapper(lineMapper);
	vtkSmartPointer<vtkActor> tubeActor = vtkSmartPointer<vtkActor>::New();
	tubeActor->SetMapper(tubeMapper);
	tubeActor->GetProperty()->SetOpacity(0.3);

	// Setup render window, renderer, and interactor
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderer->AddActor(lineActor);
	renderer->AddActor(tubeActor);
	renderer->SetBackground(.2, .3, .4);
	renderWindow->Render();
	renderWindowInteractor->Start();
}
*/

#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

class vtkTimerCallback2 : public vtkCommand
{
public:
	vtkTimerCallback2() = default;

	static vtkTimerCallback2* New()
	{
		vtkTimerCallback2* cb = new vtkTimerCallback2;
		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject* caller, unsigned long eventId,
		void* vtkNotUsed(callData))
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			++this->TimerCount;
		}
		std::cout << this->TimerCount << std::endl;
		//actor->GetProperty()->SetOpacity((this->TimerCount) / 150.0);
		if (this->TimerCount < this->maxCount)
		{
			actor->SetPosition(0, -this->TimerCount, 0);
			vtkRenderWindowInteractor* iren =
				dynamic_cast<vtkRenderWindowInteractor*>(caller);
			iren->GetRenderWindow()->Render();
		}
		//椎板动画
		//if (this->TimerCount < this->maxCount/2)
		//{
		//	actor->GetProperty()->SetOpacity((this->TimerCount) / 150.0);
		//	//actor->SetPosition(0, 0, this->TimerCount);
		//	vtkRenderWindowInteractor* iren =
		//		dynamic_cast<vtkRenderWindowInteractor*>(caller);
		//	iren->GetRenderWindow()->Render();
		//}
		//else if ((this->TimerCount > this->maxCount / 2) && (this->TimerCount < (this->maxCount)-20))
		//{
		//	actor->SetPosition(0, 0, (this->TimerCount)-(this->maxCount/2));
		//	vtkRenderWindowInteractor* iren =
		//		dynamic_cast<vtkRenderWindowInteractor*>(caller);
		//	iren->GetRenderWindow()->Render();
		//}
		else if (this->TimerCount == this->maxCount) {
			this->TimerCount = 0;
			//actor->SetPosition(0, 0, this->TimerCount);
			vtkRenderWindowInteractor* iren =
				dynamic_cast<vtkRenderWindowInteractor*>(caller);
			iren->GetRenderWindow()->Render();
		}
		else
		{
			vtkRenderWindowInteractor* iren =
				dynamic_cast<vtkRenderWindowInteractor*>(caller);
			if (this->timerId > -1)
			{
				iren->DestroyTimer(this->timerId);
			}
		}
	}

private:
	int TimerCount = 0;

public:
	vtkActor* actor = nullptr;
	int timerId = 0;
	int maxCount = -1;
};

class vtkTimerCallback3 : public vtkCommand
{
public:
	vtkTimerCallback3() = default;

	static vtkTimerCallback3* New()
	{
		vtkTimerCallback3* cb = new vtkTimerCallback3;
		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject* caller, unsigned long eventId,
		void* vtkNotUsed(callData))
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			++this->TimerCount;
		}
		std::cout << this->TimerCount << std::endl;
		actor->GetProperty()->SetOpacity((this->TimerCount) / 140.0);
		if (this->TimerCount < this->maxCount)
		{

			vtkRenderWindowInteractor* iren =
				dynamic_cast<vtkRenderWindowInteractor*>(caller);
			iren->GetRenderWindow()->Render();
		}
		else if (this->TimerCount == this->maxCount) {
			this->TimerCount = 0;
			vtkRenderWindowInteractor* iren =
				dynamic_cast<vtkRenderWindowInteractor*>(caller);
			iren->GetRenderWindow()->Render();
		}
		else
		{
			vtkRenderWindowInteractor* iren =
				dynamic_cast<vtkRenderWindowInteractor*>(caller);
			if (this->timerId > -1)
			{
				iren->DestroyTimer(this->timerId);
			}
		}
	}

private:
	int TimerCount = 0;

public:
	vtkActor* actor = nullptr;
	int timerId = 0;
	int maxCount = -1;
};


class StyleTrackballActor :public   vtkInteractorStyleTrackballActor
{
public:
	static StyleTrackballActor* New();
	//vtkTypeMacro(InteractorStyle, vtkInteractorStyleRubberBandPick);
	vtkTypeMacro(StyleTrackballActor, vtkInteractorStyleTrackballActor);

	virtual void OnLeftButtonDown()
	{
		// Forward events
		vtkInteractorStyleTrackballActor::OnLeftButtonDown();

		if (this->InteractionProp == this->ActorA)
		{
			std::cout << "Picked ActorA." << std::endl;
		}
		else if (this->InteractionProp == this->ActorB)
		{
			std::cout << "Picked ActorB." << std::endl;


		}
	}

	vtkActor* ActorA;
	vtkActor* ActorB;

};
vtkStandardNewMacro(StyleTrackballActor);


//动画模拟
#include <vtkPointPicker.h>
#include<vtkPNGReader.h>
#include<vtkTexture.h>

//#include <odbcinst.h>
//#include <afxdb.h>






//pickpoint

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkCellPicker.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkRendererCollection.h>
#include <vtkExtractSelection.h>
#include <vtkObjectFactory.h>

/**************************************************************************/
class CellPickerInteractorStyle :public vtkInteractorStyleTrackballCamera
{
public:
	static CellPickerInteractorStyle* New();

	CellPickerInteractorStyle()
	{
		selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		selectedActor = vtkSmartPointer<vtkActor>::New();
	}
	virtual void OnLeftButtonDown()
	{
		int* pos = this->GetInteractor()->GetEventPosition();
		vtkSmartPointer<vtkCellPicker> picker =
			vtkSmartPointer<vtkCellPicker>::New();
		picker->SetTolerance(0.0005);
		picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

		if (picker->GetCellId() != -1)
		{
			vtkSmartPointer<vtkIdTypeArray> ids =
				vtkSmartPointer<vtkIdTypeArray>::New();
			ids->SetNumberOfComponents(1);
			ids->InsertNextValue(picker->GetCellId());

			vtkSmartPointer<vtkSelectionNode> selectionNode =
				vtkSmartPointer<vtkSelectionNode>::New();
			selectionNode->SetFieldType(vtkSelectionNode::CELL);
			selectionNode->SetContentType(vtkSelectionNode::INDICES);
			selectionNode->SetSelectionList(ids);

			vtkSmartPointer<vtkSelection> selection =
				vtkSmartPointer<vtkSelection>::New();
			selection->AddNode(selectionNode);

			vtkSmartPointer<vtkExtractSelection> extractSelection =
				vtkSmartPointer<vtkExtractSelection>::New();
			extractSelection->SetInputData(0, polyData);
			extractSelection->SetInputData(1, selection);
			extractSelection->Update();

			selectedMapper->SetInputData((vtkDataSet*)extractSelection->GetOutput());
			selectedActor->SetMapper(selectedMapper);
			selectedActor->GetProperty()->EdgeVisibilityOn();
			selectedActor->GetProperty()->SetEdgeColor(1, 0, 0);
			selectedActor->GetProperty()->SetLineWidth(3);

			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);
		}
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
private:
	vtkSmartPointer<vtkPolyData>      polyData;
	vtkSmartPointer<vtkDataSetMapper> selectedMapper;
	vtkSmartPointer<vtkActor>         selectedActor;
};
/*********************************************************************************/

vtkStandardNewMacro(CellPickerInteractorStyle);

void MainWindow::test5()
{
	if (ui->comboBox_2->currentIndex() == 0)
	{
		vtkSmartPointer<vtkRenderer> aRenderer =
			vtkSmartPointer<vtkRenderer>::New();
		vtkSmartPointer<vtkRenderer> rRenderer =
			vtkSmartPointer<vtkRenderer>::New();
		aRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);

		rRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
		//rRenderer->SetViewport(0.0, 0.0, 1.0, 1.0);

		//vtkSmartPointer<vtkRenderer> a2Renderer =
		//	vtkSmartPointer<vtkRenderer>::New();
		//a2Renderer->SetViewport(0.0, 0.0, 0.5, 0.5);

		//aRenderer->SetViewport(0, 0.0, 0.5, 1.0);
		vtkSmartPointer<vtkRenderWindow> renWin =
			vtkSmartPointer<vtkRenderWindow>::New();
		renWin->AddRenderer(aRenderer);
		//renWin->AddRenderer(a2Renderer);
		vtkSmartPointer<vtkRenderWindowInteractor> iren =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		windowInteractor01->SetRenderWindow(renWin);

		//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
		folder = ui->label_2->text().toStdString();
		vtkSmartPointer<vtkDICOMImageReader> reader =
			vtkSmartPointer<vtkDICOMImageReader>::New();
		reader->SetDirectoryName(folder.c_str());
		reader->Update();


		if (!reader->GetOutputPort())
		{
			printf("open failed");
		}
		printf("读取数据完成");

		vtkSmartPointer<vtkContourFilter> skinExtractor =
			vtkSmartPointer<vtkContourFilter>::New();
		skinExtractor->SetInputConnection(reader->GetOutputPort());
		//skinExtractor->SetValue(0, 500);    //值越大，保留的部分越少。
		skinExtractor->SetValue(0, 200);    //值越大，保留的部分越少。

		//做平滑处理
		vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
		smooth->SetInputConnection(skinExtractor->GetOutputPort());
		smooth->SetNumberOfIterations(100);

		//重新计算法向量
		vtkSmartPointer<vtkPolyDataNormals> skinNormals =
			vtkSmartPointer<vtkPolyDataNormals>::New();

		skinNormals->SetInputConnection(smooth->GetOutputPort());
		skinNormals->SetFeatureAngle(180);

		vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
			vtkSmartPointer<vtkStripper>::New();
		skinStripper->SetInputConnection(skinNormals->GetOutputPort());

		vtkSmartPointer<vtkPolyDataMapper> skinMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		skinMapper->SetInputConnection(skinStripper->GetOutputPort());
		skinMapper->ScalarVisibilityOff();    //这样不会带颜色

		vtkSmartPointer<vtkActor> skin =
			vtkSmartPointer<vtkActor>::New();
		skin->SetMapper(skinMapper);

		vtkSmartPointer<vtkOutlineFilter> outlineData =
			vtkSmartPointer<vtkOutlineFilter>::New();
		outlineData->SetInputConnection(reader->GetOutputPort());

		vtkSmartPointer<vtkPolyDataMapper> mapOutline =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapOutline->SetInputConnection(outlineData->GetOutputPort());

		vtkSmartPointer<vtkActor> outline =
			vtkSmartPointer<vtkActor>::New();
		outline->SetMapper(mapOutline);
		outline->GetProperty()->SetColor(0, 0, 0);

		vtkSmartPointer<vtkCamera> aCamera =
			vtkSmartPointer<vtkCamera>::New();
		aCamera->SetViewUp(0, 0, -1);
		aCamera->SetPosition(0, 1, 0);
		aCamera->SetFocalPoint(0, 0, 0);
		aCamera->ComputeViewPlaneNormal();
		aCamera->Azimuth(30.0);
		aCamera->Elevation(30.0);
		aCamera->Dolly(1.5);
		aRenderer->AddActor(outline);
		aRenderer->AddActor(skin);


		auto colors =
			vtkSmartPointer<vtkNamedColors>::New();
		/*
		// Create a line
		vtkSmartPointer<vtkLineSource> lineSource =
			vtkSmartPointer<vtkLineSource>::New();
		lineSource->SetPoint1(15, 130, 90);
		lineSource->SetPoint2(50, 130, 90);

		// Create a mapper and actor
		vtkSmartPointer<vtkPolyDataMapper> lineMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		lineMapper->SetInputConnection(lineSource->GetOutputPort());

		vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
		tubeFilter->SetInputConnection(lineSource->GetOutputPort());
		tubeFilter->SetRadius(0.9);
		tubeFilter->SetNumberOfSides(100);
		tubeFilter->CappingOn();

		// Create a mapper and actor
		auto mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(tubeFilter->GetOutputPort());
		auto actor =
			vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(0.1, 0.1, 0.1);
		*/

		std::string inputFilename = "E:\\NK\\03_material\\brain\\zuan.stl";
		//std::string inputFilename = "E:\\NK\\04_data\\Juan\\18-L\\18-L-股骨四合一切骨导板.stl";

		vtkSmartPointer<vtkSTLReader> reader2 =
			vtkSmartPointer<vtkSTLReader>::New();
		reader2->SetFileName(inputFilename.c_str());
		reader2->Update();

		vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(reader2->GetOutputPort());

		vtkSmartPointer<vtkActor> actor =
			vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

		mapper->ScalarVisibilityOff();
		//设置模型颜色actor->GetProperty()->SetColor(.2,.3,.6);

		vtkTransform *myTrans = vtkTransform::New();
		//头颅模型
		//myTrans->Translate(60, 127, 48);
		//myTrans->RotateZ(90);
		//myTrans->Scale(0.3, 0.3, 0.3);
		//完整头颅标本
		//myTrans->Translate(32, 65, 47);
		//myTrans->RotateZ(90);
		//myTrans->Scale(0.3,0.3,0.3);
		//部分头颅标本
		//myTrans->Translate(35, 74, 7);
		//myTrans->RotateZ(105);
		//myTrans->Scale(0.3, 0.3, 0.3);
		//协和尸头标本
		myTrans->Translate(20, 90, 97);
		myTrans->RotateZ(105);
		myTrans->Scale(0.3, 0.3, 0.3);
		actor->SetUserTransform(myTrans);
		//椎板
		//myTrans->Translate(150, 300, 250);
		//myTrans->RotateX(180);
		//myTrans->RotateZ(60);
		//myTrans->Scale(2, 2, 2);
		actor->SetUserTransform(myTrans);

		// Create a renderer, render window, and interactor


		// 把骨钻加进去
		rRenderer->AddActor(actor);
		//rRenderer->SetBackground(colors->GetColor3d("AliceBlue").GetData());
		rRenderer->SetBackground(.2, .2, .2);

		// Render and interact
		renWin->Render();

		// Initialize must be called prior to creating timer events.
		windowInteractor01->Initialize();


		// Sign up to receive TimerEvent
		//骨钻动画
		auto cb = vtkSmartPointer<vtkTimerCallback2>::New();
		cb->actor = actor;
		windowInteractor01->AddObserver(vtkCommand::TimerEvent, cb);

		int timerId = windowInteractor01->CreateRepeatingTimer(100);
		std::cout << "timerId: " << timerId << std::endl;
		// Destroy the timer when maxCount is reached.
		cb->maxCount = 70;
		cb->timerId = timerId;

		// Start the interaction and timer
		//windowInteractor01->Start();

		renWin->SetSize(1000, 1000);


		// Create a line
		vtkSmartPointer<vtkLineSource> lineSource1 =
			vtkSmartPointer<vtkLineSource>::New();
		//协和尸头标本
		lineSource1->SetPoint1(30, 96, 138);
		lineSource1->SetPoint2(80, 110, 138);
		//头颅模型
		//lineSource1->SetPoint1(65, 130, 90);
		//lineSource1->SetPoint2(95, 130, 90);
		//完整头颅标本
		//lineSource1->SetPoint1(43, 69, 89);
		//lineSource1->SetPoint2(73, 69, 89);
		//部分头颅标本
		//lineSource1->SetPoint1(43, 80, 48);
		//lineSource1->SetPoint2(73, 88, 48);

		// Create a mapper and actor
		vtkSmartPointer<vtkPolyDataMapper> lineMapper1 =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		lineMapper1->SetInputConnection(lineSource1->GetOutputPort());
		vtkSmartPointer<vtkActor> lineActor1 =
			vtkSmartPointer<vtkActor>::New();
		lineActor1->GetProperty()->SetColor(0.0, 0.0, 0.1); // Give some color to the line
		//lineActor1->SetMapper(lineMapper);

		// Create a tube (cylinder) around the line
		vtkSmartPointer<vtkTubeFilter> tubeFilter1 =
			vtkSmartPointer<vtkTubeFilter>::New();
		tubeFilter1->SetInputConnection(lineSource1->GetOutputPort());
		tubeFilter1->SetRadius(2); //default is .5
		tubeFilter1->SetNumberOfSides(50);
		tubeFilter1->Update();

		//// Create a mapper and actor
		vtkSmartPointer<vtkPolyDataMapper> tubeMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		tubeMapper->SetInputConnection(tubeFilter1->GetOutputPort());

		//vtkSmartPointer<vtkActor> tubeActor =
		//	vtkSmartPointer<vtkActor>::New();
		//tubeActor->GetProperty()->SetOpacity(0.5); //Make the tube have some transparency.
		//tubeActor->SetMapper(tubeMapper);
		//tubeActor->GetProperty()->SetColor(0.0, 0.4, 0.0); // Give some color to the line

		// Add the actor to the scene
		//rRenderer->AddActor(tubeActor);
		//rRenderer->AddActor(lineActor);


		aRenderer->SetActiveCamera(aCamera);
		aRenderer->ResetCamera();
		//aRenderer->SetBackground(.2, .3, .4);
		aRenderer->SetBackground(.15, .15, .15);
		aRenderer->ResetCameraClippingRange();

		renWin->SetSize(1000, 1000);

		vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
			vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		windowInteractor01->SetInteractorStyle(style);

		/////////设置截面
		vtkSmartPointer<vtkClipPolyData> cliper = vtkClipPolyData::New();
		cliper->SetInputData(skinStripper->GetOutput());

		// 此平面box可以通过右键来进行放大缩小处理（只有当鼠标控制区域只有切割体才单一有效）
		vtkSmartPointer<vtkImplicitPlaneWidget> implicitPlaneWidget = vtkImplicitPlaneWidget::New();

		//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();

		implicitPlaneWidget->SetInteractor(windowInteractor01);
		implicitPlaneWidget->SetPlaceFactor(1.25);
		//initially position the widget
		implicitPlaneWidget->SetInputData(skinStripper->GetOutput());
		implicitPlaneWidget->PlaceWidget();


		//////Render2
		vtkSmartPointer<vtkActor> coneSkinActor = vtkActor::New();

		vtkSmartPointer<vtkTransform> trans =
			vtkSmartPointer<vtkTransform>::New();
		trans->PostMultiply();
		//trans->Translate(1, 0, 0);
		trans->RotateX(90);
		//coneSkinActor->SetUserTransform(trans);

		coneSkinActor->SetMapper(skinMapper);


		rRenderer->SetBackground(.2, .2, .2); 
		//rRenderer->SetBackground(0.2, 0.3, 0.5);
		rRenderer->AddActor(coneSkinActor);

		//vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
		//actor2->SetMapper(vesselMapper);
		//actor2->GetProperty()->SetColor(0, 0, 0);
		//actor2->SetUserTransform(trans);
		vtkSmartPointer<vtkActor> lineActor2 =
			vtkSmartPointer<vtkActor>::New();
		lineActor2->GetProperty()->SetColor(0.0, 0.0, 0.1); // Give some color to the line
		//lineActor2->SetMapper(lineMapper);
		vtkSmartPointer<vtkActor> tubeActor2 =
			vtkSmartPointer<vtkActor>::New();
		tubeActor2->GetProperty()->SetOpacity(0.5); //Make the tube have some transparency.
		tubeActor2->SetMapper(tubeMapper);
		tubeActor2->GetProperty()->SetColor(0.0, 0.4, 0.0); // Give some color to the line


		// Sign up to receive TimerEvent
		//管子浮现
		//auto cb2 = vtkSmartPointer<vtkTimerCallback3>::New();
		//cb2->actor = tubeActor2;
		//windowInteractor01->AddObserver(vtkCommand::TimerEvent, cb2);

		//int timerId2 = windowInteractor01->CreateRepeatingTimer(100);
		//std::cout << "timerId: " << timerId2 << std::endl;
		//// Destroy the timer when maxCount is reached.
		//cb2->maxCount = 90;
		//cb2->timerId = timerId2;

		//tubeActor2->SetUserTransform(trans);
		//lineActor2->SetUserTransform(trans);

		rRenderer->AddActor(tubeActor2);
		//rRenderer->AddActor(lineActor2);

		//rRenderer->AddActor(actor2);

		vtkSmartPointer<BuildVTKWidgetCall> pCall = BuildVTKWidgetCall::New();
		pCall->setActor(coneSkinActor);
		pCall->setCliper(cliper);

		renWin->AddRenderer(rRenderer);
		///////
		implicitPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);
		implicitPlaneWidget->On();

		//windowInteractor01->SetInteractorStyle(style);

		//ui->qvtkWidget_4->SetRenderWindow(renWin);		// make the link between VTK and QT
		//ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(rRenderer);
		//windowInteractor01->SetRenderWindow(renWin);

		//ui->qvtkWidget_4->show();
		//renWin->Render();

		windowInteractor01->Initialize();
		windowInteractor01->Start();
	}
	
	//机械臂
	else if(ui->comboBox_2->currentIndex() == 1) {
	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> rRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	aRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);

	rRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(aRenderer);
	//renWin->AddRenderer(a2Renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	windowInteractor01->SetRenderWindow(renWin);

	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
	folder = ui->label_2->text().toStdString();
	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();

	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection(reader->GetOutputPort());
	//skinExtractor->SetValue(0, 500);    //值越大，保留的部分越少。
	skinExtractor->SetValue(0, 1);    //值越大，保留的部分越少。

	//做平滑处理
	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
	smooth->SetInputConnection(skinExtractor->GetOutputPort());
	smooth->SetNumberOfIterations(100);

	//重新计算法向量
	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();

	skinNormals->SetInputConnection(smooth->GetOutputPort());
	skinNormals->SetFeatureAngle(180);

	vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();    //这样不会带颜色

	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);

	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0, 0, 0);

	vtkSmartPointer<vtkCamera> aCamera =
		vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, 1, 0);
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);
	aCamera->Dolly(1.5);
	aRenderer->AddActor(outline);
	aRenderer->AddActor(skin);


	auto colors =
		vtkSmartPointer<vtkNamedColors>::New();

	std::string inputFilename = "E:\\NK\\03_material\\VTK\\robot\\base.stl";
	std::string inputFilename1 = "E:\\NK\\03_material\\VTK\\robot\\1.stl";
	std::string inputFilename2 = "E:\\NK\\03_material\\VTK\\robot\\2.stl";
	std::string inputFilename3 = "E:\\NK\\03_material\\VTK\\robot\\3.stl";
	std::string inputFilename4 = "E:\\NK\\03_material\\VTK\\robot\\4.stl";
	std::string inputFilename5 = "E:\\NK\\03_material\\VTK\\robot\\5.stl";
	std::string inputFilename6 = "E:\\NK\\03_material\\VTK\\robot\\6.stl";
	//std::string inputFilename = "E:\\NK\\04_data\\Juan\\18-L\\18-L-股骨四合一切骨导板.stl";

	vtkSmartPointer<vtkSTLReader> reader2 =
		vtkSmartPointer<vtkSTLReader>::New();
	reader2->SetFileName(inputFilename.c_str());
	reader2->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader2->GetOutputPort());

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkSTLReader> reader21 =
		vtkSmartPointer<vtkSTLReader>::New();
	reader21->SetFileName(inputFilename1.c_str());
	reader21->Update();
	vtkSmartPointer<vtkSTLReader> reader22 =
		vtkSmartPointer<vtkSTLReader>::New();
	reader22->SetFileName(inputFilename2.c_str());
	reader22->Update();	
	vtkSmartPointer<vtkSTLReader> reader23 =
		vtkSmartPointer<vtkSTLReader>::New();
	reader23->SetFileName(inputFilename3.c_str());
	reader23->Update();
	vtkSmartPointer<vtkSTLReader> reader24 =
		vtkSmartPointer<vtkSTLReader>::New();
	reader24->SetFileName(inputFilename4.c_str());
	reader24->Update();
	vtkSmartPointer<vtkSTLReader> reader25 =
		vtkSmartPointer<vtkSTLReader>::New();
	reader25->SetFileName(inputFilename5.c_str());
	reader25->Update();
	vtkSmartPointer<vtkSTLReader> reader26 =
		vtkSmartPointer<vtkSTLReader>::New();
	reader26->SetFileName(inputFilename6.c_str());
	reader26->Update();
	vtkSmartPointer<vtkPolyDataMapper> mapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputConnection(reader21->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> mapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper2->SetInputConnection(reader22->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> mapper3 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper3->SetInputConnection(reader23->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> mapper4 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper4->SetInputConnection(reader24->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> mapper5 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper5->SetInputConnection(reader25->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> mapper6 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper6->SetInputConnection(reader26->GetOutputPort());
	vtkSmartPointer<vtkActor> actor1 =
		vtkSmartPointer<vtkActor>::New();
	actor1->SetMapper(mapper1);
	vtkSmartPointer<vtkActor> actor2 =
		vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper(mapper2);
	vtkSmartPointer<vtkActor> actor3 =
		vtkSmartPointer<vtkActor>::New();
	actor3->SetMapper(mapper3);
	vtkSmartPointer<vtkActor> actor4 =
		vtkSmartPointer<vtkActor>::New();
	actor4->SetMapper(mapper4);
	vtkSmartPointer<vtkActor> actor5 =
		vtkSmartPointer<vtkActor>::New();
	actor5->SetMapper(mapper5);
	vtkSmartPointer<vtkActor> actor6 =
		vtkSmartPointer<vtkActor>::New();
	actor6->SetMapper(mapper6);

	mapper->ScalarVisibilityOff();
	//设置模型颜色actor->GetProperty()->SetColor(.2,.3,.6);
	qDebug("actor1 =%lf,%lf,%lf ", actor1->GetPosition()[0], actor1->GetPosition()[1], actor1->GetPosition()[2]);
	qDebug("actor2 =%lf,%lf,%lf ", actor2->GetPosition()[0], actor2->GetPosition()[1], actor2->GetPosition()[2]);
	qDebug("actor3 =%lf,%lf,%lf ", actor3->GetPosition()[0], actor3->GetPosition()[1], actor3->GetPosition()[2]);
	qDebug("actor4 =%lf,%lf,%lf ", actor4->GetPosition()[0], actor4->GetPosition()[1], actor4->GetPosition()[2]);
	qDebug("actor5 =%lf,%lf,%lf ", actor5->GetPosition()[0], actor5->GetPosition()[1], actor5->GetPosition()[2]);

	actor6->SetOrigin(40, 0, 0);
	qDebug("actor6_1 =%lf,%lf,%lf ", actor6->GetPosition()[0], actor6->GetPosition()[1], actor6->GetPosition()[2]);
	qDebug("actor6_1 =%lf,%lf,%lf ", actor6->GetOrientation()[0], actor6->GetOrientation()[1], actor6->GetOrientation()[2]);

	//指定物体在世界坐标系中的位置
	actor6->SetPosition(40, 0, 0);
	qDebug("actor6_2 =%lf,%lf,%lf ", actor6->GetPosition()[0], actor6->GetPosition()[1], actor6->GetPosition()[2]);
	qDebug("actor6_2 =%lf,%lf,%lf ", actor6->GetOrientation()[0], actor6->GetOrientation()[1], actor6->GetOrientation()[2]);

	//按照指定的平移量在x 轴、y 轴、z 轴平移物体
	//actor6->AddPosition(100, 0, 0);
	qDebug("actor6_3 =%lf,%lf,%lf ", actor6->GetPosition()[0], actor6->GetPosition()[1], actor6->GetPosition()[2]);
	qDebug("actor6_3 =%lf,%lf,%lf ", actor6->GetOrientation()[0], actor6->GetOrientation()[1], actor6->GetOrientation()[2]);
	//设定物体的方位，物体绕z、x、y 轴的顺序旋转
	//actor6->SetOrientation(1, 0, 0);
	qDebug("actor6_4 =%lf,%lf,%lf ", actor6->GetPosition()[0], actor6->GetPosition()[1], actor6->GetPosition()[2]);
	qDebug("actor6_4 =%lf,%lf,%lf ", actor6->GetOrientation()[0], actor6->GetOrientation()[1], actor6->GetOrientation()[2]);
	/*************************/
	//绕X轴旋转90度
	actor6->RotateY(90);
	actor6->RotateX(90);
	qDebug("actor6_5 =%lf,%lf,%lf ", actor6->GetPosition()[0], actor6->GetPosition()[1], actor6->GetPosition()[2]);
	qDebug("actor6_5 =%lf,%lf,%lf ", actor6->GetOrientation()[0], actor6->GetOrientation()[1], actor6->GetOrientation()[2]);
	//绕Y轴旋转90度
	//actor6->RotateY(90);	
	qDebug("actor6_6 =%lf,%lf,%lf ", actor6->GetPosition()[0], actor6->GetPosition()[1], actor6->GetPosition()[2]);
	qDebug("actor6_6 =%lf,%lf,%lf ", actor6->GetOrientation()[0], actor6->GetOrientation()[1], actor6->GetOrientation()[2]);
	//绕Z轴旋转90度
	//actor6->RotateZ(90);	
	qDebug("actor6_7 =%lf,%lf,%lf ", actor6->GetPosition()[0], actor6->GetPosition()[1], actor6->GetPosition()[2]);
	qDebug("actor6_7 =%lf,%lf,%lf ", actor6->GetOrientation()[0], actor6->GetOrientation()[1], actor6->GetOrientation()[2]);
	//物体绕用户定义的轴(x,y,z)旋转theat 角度
	actor6->RotateWXYZ(-90, 1, 1, 0);
	qDebug("actor6_8 =%lf,%lf,%lf ", actor6->GetPosition()[0], actor6->GetPosition()[1], actor6->GetPosition()[2]);
	qDebug("actor6_8 =%lf,%lf,%lf ", actor6->GetOrientation()[0], actor6->GetOrientation()[1], actor6->GetOrientation()[2]);

	vtkTransform *myTrans = vtkTransform::New();
	//头颅模型
	myTrans->Translate(500, -250, 200);
	myTrans->RotateZ(90);
	myTrans->RotateX(-90);
	myTrans->RotateY(-10);
	myTrans->Scale(0.5, 0.5, 0.5);

	actor->GetProperty()->SetColor(0.9, 0.9, 0.98);
	actor1->GetProperty()->SetColor(0.78, 0.27, 0.35);
	actor2->GetProperty()->SetColor(0.9, 0.9, 0.98);
	actor3->GetProperty()->SetColor(0.78, 0.27, 0.35);
	actor4->GetProperty()->SetColor(0.9, 0.9, 0.98);
	actor5->GetProperty()->SetColor(0.78, 0.27, 0.35);
	actor6->GetProperty()->SetColor(0.78, 0.27, 0.35);

	//actor->SetUserTransform(myTrans);
	//actor1->SetUserTransform(myTrans);
	//actor2->SetUserTransform(myTrans);
	
	//actor3->SetUserTransform(myTrans);
	//actor4->SetUserTransform(myTrans);
	//actor5->SetUserTransform(myTrans);
	//actor6->SetUserTransform(myTrans);

	// Create a renderer, render window, and interactor


	// Add the actor to the scene
	rRenderer->AddActor(actor);
	rRenderer->AddActor(actor1);
	rRenderer->AddActor(actor2);
	rRenderer->AddActor(actor3);
	rRenderer->AddActor(actor4);
	rRenderer->AddActor(actor5);
	rRenderer->AddActor(actor6);
	rRenderer->SetBackground(colors->GetColor3d("AliceBlue").GetData());

	// Render and interact
	renWin->Render();

	// Initialize must be called prior to creating timer events.
	windowInteractor01->Initialize();


	//// Sign up to receive TimerEvent
	//auto cb = vtkSmartPointer<vtkTimerCallback2>::New();
	//cb->actor = actor;
	//windowInteractor01->AddObserver(vtkCommand::TimerEvent, cb);

	//int timerId = windowInteractor01->CreateRepeatingTimer(100);
	//std::cout << "timerId: " << timerId << std::endl;
	//// Destroy the timer when maxCount is reached.
	//cb->maxCount = 60;
	//cb->timerId = timerId;

	// Start the interaction and timer
	//windowInteractor01->Start();

	renWin->SetSize(1000, 1000);


	// Create a line
	vtkSmartPointer<vtkLineSource> lineSource1 =
		vtkSmartPointer<vtkLineSource>::New();
	//头颅模型
	lineSource1->SetPoint1(65, 130, 90);
	lineSource1->SetPoint2(95, 130, 90);

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> lineMapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	lineMapper1->SetInputConnection(lineSource1->GetOutputPort());
	vtkSmartPointer<vtkActor> lineActor1 =
		vtkSmartPointer<vtkActor>::New();
	lineActor1->GetProperty()->SetColor(0.0, 0.0, 0.1); // Give some color to the line
	//lineActor1->SetMapper(lineMapper);

	// Create a tube (cylinder) around the line
	vtkSmartPointer<vtkTubeFilter> tubeFilter1 =
		vtkSmartPointer<vtkTubeFilter>::New();
	tubeFilter1->SetInputConnection(lineSource1->GetOutputPort());
	tubeFilter1->SetRadius(2); //default is .5
	tubeFilter1->SetNumberOfSides(50);
	tubeFilter1->Update();

	//// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> tubeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	tubeMapper->SetInputConnection(tubeFilter1->GetOutputPort());

	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	aRenderer->SetBackground(.2, .3, .4);
	aRenderer->ResetCameraClippingRange();

	renWin->SetSize(1000, 1000);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	windowInteractor01->SetInteractorStyle(style);

	/////////设置截面
	vtkSmartPointer<vtkClipPolyData> cliper = vtkClipPolyData::New();
	cliper->SetInputData(skinStripper->GetOutput());

	// 此平面box可以通过右键来进行放大缩小处理（只有当鼠标控制区域只有切割体才单一有效）
	vtkSmartPointer<vtkImplicitPlaneWidget> implicitPlaneWidget = vtkImplicitPlaneWidget::New();

	implicitPlaneWidget->SetInteractor(windowInteractor01);
	implicitPlaneWidget->SetPlaceFactor(1.25);
	//initially position the widget
	implicitPlaneWidget->SetInputData(skinStripper->GetOutput());
	implicitPlaneWidget->PlaceWidget();


	//////Render2
	vtkSmartPointer<vtkActor> coneSkinActor = vtkActor::New();

	vtkSmartPointer<vtkTransform> trans =
		vtkSmartPointer<vtkTransform>::New();
	trans->PostMultiply();
	//trans->Translate(1, 0, 0);
	trans->RotateX(90);
	//coneSkinActor->SetUserTransform(trans);

	coneSkinActor->SetMapper(skinMapper);


	rRenderer->SetBackground(0.2, 0.3, 0.5);
	rRenderer->AddActor(coneSkinActor);

	vtkSmartPointer<vtkActor> lineActor2 =
		vtkSmartPointer<vtkActor>::New();
	lineActor2->GetProperty()->SetColor(0.0, 0.0, 0.1); // Give some color to the line
	//lineActor2->SetMapper(lineMapper);
	vtkSmartPointer<vtkActor> tubeActor2 =
		vtkSmartPointer<vtkActor>::New();
	tubeActor2->GetProperty()->SetOpacity(0.5); //Make the tube have some transparency.
	tubeActor2->SetMapper(tubeMapper);
	tubeActor2->GetProperty()->SetColor(0.0, 0.4, 0.0); // Give some color to the line


	// Sign up to receive TimerEvent
	//管子浮现
	auto cb2 = vtkSmartPointer<vtkTimerCallback3>::New();
	cb2->actor = tubeActor2;
	windowInteractor01->AddObserver(vtkCommand::TimerEvent, cb2);

	int timerId2 = windowInteractor01->CreateRepeatingTimer(100);
	std::cout << "timerId: " << timerId2 << std::endl;
	// Destroy the timer when maxCount is reached.
	cb2->maxCount = 60;
	cb2->timerId = timerId2;

	rRenderer->AddActor(tubeActor2);

	vtkSmartPointer<BuildVTKWidgetCall> pCall = BuildVTKWidgetCall::New();
	pCall->setActor(coneSkinActor);
	pCall->setCliper(cliper);

	renWin->AddRenderer(rRenderer);
	///////
	implicitPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);
	implicitPlaneWidget->On();

	windowInteractor01->Initialize();
	windowInteractor01->Start();
	}

	//配准
	else if (ui->comboBox_2->currentIndex() == 2) {

	vtkSmartPointer<vtkAxesActor> AxesActor =
		vtkSmartPointer<vtkAxesActor>::New();
	AxesActor->SetPosition(0, 0, 0);
	AxesActor->SetTotalLength(100, 100, 100);
	AxesActor->SetShaftType(0);
	AxesActor->SetAxisLabels(0);
	AxesActor->SetCylinderRadius(0.02);

	std::string folder = "E:\\NK\\04_data\\kouqiangyiyuanCT\\ZhangLinkun\\CT";
	//folder = ui->label_2->text().toStdString();
	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();

	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection(reader->GetOutputPort());
	//skinExtractor->SetValue(0, 500);    //值越大，保留的部分越少。
	skinExtractor->SetValue(0, 1);    //值越大，保留的部分越少。

	//做平滑处理
	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
	smooth->SetInputConnection(skinExtractor->GetOutputPort());
	smooth->SetNumberOfIterations(100);

	//重新计算法向量
	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();

	skinNormals->SetInputConnection(smooth->GetOutputPort());
	skinNormals->SetFeatureAngle(180);

	vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();    //这样不会带颜色

	
	//std::string folder2 = "E:\\NK\\04_data\\kouqiangyiyuanCT\\ZhangLinkun\\01_MRI\\DICOM\\PA0\\ST0\\SE4"; 
	//vtkSmartPointer<vtkDICOMImageReader> reader2 =
	//	vtkSmartPointer<vtkDICOMImageReader>::New();
	//reader2->SetDirectoryName(folder2.c_str());
	//reader2->Update();

	//vtkSmartPointer<vtkContourFilter> skinExtractor2 =
	//	vtkSmartPointer<vtkContourFilter>::New();
	//skinExtractor2->SetInputConnection(reader2->GetOutputPort());
	////skinExtractor->SetValue(0, 500);    //值越大，保留的部分越少。
	//skinExtractor2->SetValue(0, 100);    //值越大，保留的部分越少。
	//vtkSmartPointer<vtkSmoothPolyDataFilter> smooth2 = vtkSmoothPolyDataFilter::New();
	//smooth2->SetInputConnection(skinExtractor2->GetOutputPort());
	//smooth2->SetNumberOfIterations(100);
	//vtkSmartPointer<vtkPolyDataMapper> skinMapper2 =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//skinMapper2->SetInputConnection(smooth2->GetOutputPort());
	//skinMapper2->ScalarVisibilityOff();    //这样不会带颜色


	std::string filename = "E:\\NK\\04_data\\kouqiangyiyuanCT\\ZhangLinkun\\3dmed\\201206131923new.obj";
	vtkSmartPointer<vtkOBJReader> reader2 =
		vtkSmartPointer<vtkOBJReader>::New();
	reader2->SetFileName(filename.c_str());
	reader2->Update();

	vtkSmartPointer<vtkPolyDataMapper> skinMapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper2->SetInputConnection(reader2->GetOutputPort());


	vtkActor *coneActor = vtkActor::New();
	coneActor->SetMapper(skinMapper);
	coneActor->GetProperty()->SetColor(0.2, 0.63, 0.79);
	coneActor->GetProperty()->SetDiffuse(0.7);
	coneActor->GetProperty()->SetSpecular(0.4);
	coneActor->GetProperty()->SetSpecularPower(20);
	double *origin = coneActor->GetOrigin();

	vtkActor *coneActor2 = vtkActor::New();
	coneActor2->SetMapper(skinMapper2);
	//coneActor2->GetProperty()->SetColor(0.2, 0.1, 0.1);
	//coneActor2->SetProperty(property);
	//coneActor2->SetPosition(0, 2, 0);
	coneActor2->SetOrigin(origin);

	//
	vtkSmartPointer<vtkPNGReader>bmpReader = vtkSmartPointer<vtkPNGReader>::New();
	bmpReader->SetFileName("E:\\NK\\04_data\\kouqiangyiyuanCT\\ZhangLinkun\\3dmed\\201206131923new.png");//读入纹理图
	vtkSmartPointer<vtkTexture>texture = vtkSmartPointer<vtkTexture>::New();
	texture->SetInputConnection(bmpReader->GetOutputPort());
	texture->InterpolateOn();

	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cylinderMapper->SetInputConnection(reader2->GetOutputPort());
	//cylinderMapper->SetInputConnection( sphere->GetOutputPort() );//对球体渲染

	vtkSmartPointer<vtkActor> cylinderActor =
		vtkSmartPointer<vtkActor>::New();
	cylinderActor->SetMapper(cylinderMapper);
	cylinderActor->SetTexture(texture);//纹理为actor的一种属性

	
	vtkTransform *myTrans = vtkTransform::New();
	//头颅模型
	//myTrans->RotateX(-90);
	myTrans->Translate(0, 0, 100);
	cylinderActor->SetUserTransform(myTrans);
	//coneActor->SetUserTransform(myTrans);
	//coneActor2->SetUserTransform(myTrans);

	vtkRenderer *ren1 = vtkRenderer::New();

	ren1->SetBackground(0.1, 0.2, 0.4);
	//ren1->SetBackground(1, 1, 1);

	//
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(300, 300);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<StyleTrackballActor> style = vtkSmartPointer <StyleTrackballActor>::New();
	style->SetDefaultRenderer(ren1);
	style->ActorA = coneActor;
	style->ActorB = cylinderActor;
	iren->SetInteractorStyle(style);
	ren1->AddActor(coneActor);
	ren1->AddActor(cylinderActor);
	ren1->AddActor(AxesActor);
	//ren1->AddActor(Actor1);
	//ren1->AddActor(Actor2);


	iren->Initialize();
	iren->Start();
	coneActor->Delete();
	// property->Delete();
	coneActor2->Delete();
	ren1->Delete();
	renWin->Delete();

	}

	//渲染动画
//	else if (ui->comboBox_2->currentIndex() == 3) {
//
//		//folder = ui->label_2->text().toStdString();
//		//folder = "E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\gugu+jinggu";
//		folder = "E:\\NK\\04_data\\xiguanjie_model\\GuMark_dicom";
//		vtkSmartPointer<vtkDICOMImageReader> reader1 =
//			vtkSmartPointer<vtkDICOMImageReader>::New();
//		reader1->SetDirectoryName(folder.c_str());
//		reader1->Update();
//		vtkSmartPointer<vtkPolyData> input1 = vtkSmartPointer<vtkPolyData>::New();
//		input1->DeepCopy(reader1->GetOutput());
//		vtkSmartPointer<vtkContourFilter> skinExtractor =
//			vtkSmartPointer<vtkContourFilter>::New();
//		skinExtractor->SetInputConnection(reader1->GetOutputPort());
//		//skinExtractor->SetValue(0, 500);    //值越大，保留的部分越少。
//		skinExtractor->SetValue(0, 1);    //值越大，保留的部分越少。
//
//		//做平滑处理
//		vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
//		smooth->SetInputConnection(skinExtractor->GetOutputPort());
//		smooth->SetNumberOfIterations(100);
//
//		//重新计算法向量
//		vtkSmartPointer<vtkPolyDataNormals> skinNormals =
//			vtkSmartPointer<vtkPolyDataNormals>::New();
//
//		skinNormals->SetInputConnection(smooth->GetOutputPort());
//		skinNormals->SetFeatureAngle(180);
//
//		vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
//			vtkSmartPointer<vtkStripper>::New();
//		skinStripper->SetInputConnection(skinNormals->GetOutputPort());
//
//		vtkSmartPointer<vtkPolyDataMapper> skinMapper =
//			vtkSmartPointer<vtkPolyDataMapper>::New();
//		skinMapper->SetInputConnection(skinStripper->GetOutputPort());
//		skinMapper->ScalarVisibilityOff();
//
//		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//		actor->SetMapper(skinMapper);
//		actor->GetProperty()->SetOpacity(1);
//		// Create a renderer, render window, and interactor
//		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//
//		// Add the actor to the scene
//		renderer->AddActor(actor);
//
//		/*------------------------------------------------------------------------------------------------------------*/
//		////std::string folder2 = "E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\jinggu";
//		//std::string folder2 = "E:\\NK\\04_data\\xiguanjie_model\\JingMark_dicom";
//		//vtkSmartPointer<vtkDICOMImageReader> reader2 =
//		//	vtkSmartPointer<vtkDICOMImageReader>::New();
//		//reader2->SetDirectoryName(folder2.c_str());
//		//reader2->Update();
//
//		//vtkSmartPointer<vtkContourFilter> skinExtractor2 =
//		//	vtkSmartPointer<vtkContourFilter>::New();
//		//skinExtractor2->SetInputConnection(reader2->GetOutputPort());
//		////skinExtractor->SetValue(0, 500);    //值越大，保留的部分越少。
//		//skinExtractor2->SetValue(0, 100);    //值越大，保留的部分越少。
//
//		//vtkSmartPointer<vtkSmoothPolyDataFilter> smooth2 = vtkSmoothPolyDataFilter::New();
//		//smooth2->SetInputConnection(skinExtractor2->GetOutputPort());
//		//smooth2->SetNumberOfIterations(100);
//		//vtkSmartPointer<vtkPolyDataMapper> skinMapper2 =
//		//	vtkSmartPointer<vtkPolyDataMapper>::New();
//		//skinMapper2->SetInputConnection(smooth2->GetOutputPort());
//		//skinMapper2->ScalarVisibilityOff();    //这样不会带颜色
//		//vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
//		//actor2->SetMapper(skinMapper2);
//		//actor2->GetProperty()->SetOpacity(1);
//		//actor2->GetProperty()->SetColor(0.56, 0.93, 0.56);
//		////actor2->SetPosition(-105, -70, -35);
//		//renderer->AddActor(actor2);
//		/*------------------------------------------------------------------------------------------------------------*/
//		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer);
//
//		vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();
//
//		vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//		renderWindowInteractor->SetPicker(pointPicker);
//		renderWindowInteractor->SetRenderWindow(ui->qvtkWidget_4->GetRenderWindow());
//
//		//base
//		vtkSmartPointer<vtkActor> actorLocal[3];
//		vtkSmartPointer<vtkLineSource> lineSourceX, lineSourceY, lineSourceZ;
//		vtkSmartPointer<vtkPolyDataMapper> MapperX, MapperY, MapperZ;
//		double pos[100][3];//[ 100 ]坐标编号：W（世界坐标系）、   [3] :x,y,z轴
//		float LINE_LEN; int i = 0;
//
//		pos[i][0] = 0; pos[i][1] = 0; pos[i][2] = 0;//xyz初始坐标
//		//世界坐标系
//		vtkSmartPointer<vtkAxesActor> actorWorld = vtkSmartPointer<vtkAxesActor>::New();
//		actorWorld->SetPosition(0, 0, 0);
//		actorWorld->SetTotalLength(30., 30., 30.);
//		actorWorld->SetShaftType(0);
//		actorWorld->SetAxisLabels(1);
//		actorWorld->SetCylinderRadius(0.02);
//		actorWorld->SetXAxisLabelText("x");
//		actorWorld->SetYAxisLabelText("y");
//		actorWorld->SetZAxisLabelText("z");
//		renderer->AddActor(actorWorld);
//
//		/*------------------------------------------------------------------------------------------------------------*/	
//		vtkTransform *myTrans = vtkTransform::New();
//		myTrans->RotateZ(-250);
//		myTrans->RotateY(180);
//		//myTrans->RotateX(90);
//		//actor->SetUserTransform(myTrans);
//		//actor2->SetUserTransform(myTrans);
//
//		renderer->AddActor(actor);
//		//renderer->AddActor(actor2);
//
//		//相机位置
//		vtkCamera *aCamera = vtkCamera::New();
//		aCamera->SetViewUp(1, 0, 0);//设视角位置 
//		//aCamera->SetPosition(-200, -200, 100);//设观察对象位(-120, -240, -60)
//		aCamera->SetPosition(-200, -300, 0);//设观察对象位(-120, -240, -60)
//		aCamera->SetFocalPoint(0, 100, 3);//设焦点 
//		aCamera->ComputeViewPlaneNormal();//自动
//
//		renderer->SetActiveCamera(aCamera);
//		//渲染
//		renderer->SetBackground(.1, .1, .1); // Background color dark blue
//		ui->qvtkWidget_4->GetRenderWindow()->Render();
//		//renderWindowInteractor->Start();
//
////此处剪切读取文件
//
//		float data[1500][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
//		ifstream infile;//定义读取文件流，相对于程序来说是in
//
//
//		infile.open("E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\0122_gugu.txt");//打开文件
//		for (int i = 0; i < 1500; i++)//定义行循环
//		{
//			for (int j = 0; j < 6; j++)//定义列循环
//			{
//				infile >> data[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
//			}
//		}
//		infile.close();//读取完成之后关闭文件
//		//读胫骨
//		float data_jing[1500][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
//		ifstream infile_jing;//定义读取文件流，相对于程序来说是in
//
//		infile_jing.open("E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\0122_jinggu.txt");//打开文件
//		for (int i = 0; i < 1500; i++)//定义行循环
//		{
//			for (int j = 0; j < 6; j++)//定义列循环
//			{
//				infile_jing >> data_jing[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
//			}
//		}
//		infile_jing.close();//读取完成之后关闭文件
//
//
//		clock_t t1, t2;
//		double t;//经历时间
//		int n = 1;//经历多少个时间单位
//		int  n_qie = 1, n_up = 1;//切骨迭代次数/被切下来part上移
//		double step = 0.1;//步长
//		double z_step = 0.2;//Z轴负向步长
//
//		t1 = clock();	//for循环开始前
//		double *cur_cyPos;
//		//cur_cyPos = actor2->GetPosition();
//		actor->SetPosition(data[0][4], data[0][5], data[0][6]);
//		//actor2->SetPosition(data_jing[0][4], data_jing[0][5], data_jing[0][6]);
//		while (1)
//		{
//			t2 = clock();	//for循环结束
//			t = (double)(t2 - t1) / CLOCKS_PER_SEC;
//
//			//if (t >= 1 && n <= 1000)//0.1秒运行一次 (刀切）
//			if (t >= 0.017 &&n <= 316)//0.1秒运行一次 (刀切）
//			{
//				t1 = t2; n++; n_qie++;
//
//				actor->SetPosition(data[n * 1][3], data[n * 1][4], data[n * 1][5] + 2000);
//				//actor2->SetPosition(data_jing[n * 1][3], data_jing[n * 1][4], data_jing[n * 1][5] + 2000);
//			//	actor2->SetPosition(data[n * 1][3] / 20 - 100, data[n * 1][4] / 20 + 200, data[n * 1][5] / 20);
//				//actorSTL2->SetOrientation(data[n * 20][0], data[n * 20][1], data[n * 20][2]);
//			//	actor->SetOrientation(actor->GetOrientation()[0], (data[n * 1][1] + 90), actor->GetOrientation()[2]);
//				actor->RotateZ(data[n * 1][1] - data[n - 1][1]); actor->RotateY(data[n * 1][2] - data[n - 1][2]); actor->RotateX(data[n * 1][3] - data[n - 1][3]);
//				//	actor2->RotateZ(data_jing[n * 1][1] - data_jing[n - 1][1]); actor2->RotateY(data_jing[n * 1][2] - data_jing[n - 1][2]); actor2->RotateX(data_jing[n * 1][3] - data_jing[n - 1][3]);
//				//	actor2->SetOrientation(actor2->GetOrientation()[0], (data[n * 1][1] + 90) * 2-5, actor2->GetOrientation()[2]);
//				vtkSmartPointer<vtkRenderWindow> temRW = vtkSmartPointer<vtkRenderWindow>::New();
//				temRW = renderer->GetRenderWindow();
//				renderer->AddActor(actor);
//				//renderer->AddActor(actor2);
//				temRW->Render();
//			}
//
//
//		////时间
//		//clock_t t1, t2;
//		//double t;//经历时间
//		//int n = 1;//经历多少个时间单位
//		//int  n_qie = 1, n_up = 1;//切骨迭代次数/被切下来part上移
//		//double step = 0.1;//步长
//		//double z_step = 0.2;//Z轴负向步长
//
//		//t1 = clock();	//for循环开始前
//		//double *cur_cyPos;
//		////cur_cyPos = actor2->GetPosition();
//		////actor->SetPosition(data[0][4], data[0][5], data[0][6]);
//		////actor2->SetPosition(data_jing[0][4], data_jing[0][5], data_jing[0][6]);
//		//while (1)
//		//{
//		//	t2 = clock();	//for循环结束
//		//	t = (double)(t2 - t1) / CLOCKS_PER_SEC;
//
//		//	if (t >= 1 &&n <= 1000)//0.1秒运行一次 (刀切）
//		//	{
//		//		t1 = t2; n++; n_qie++;
//
//		//		float data[100][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
//		//		ifstream infile;//定义读取文件流，相对于程序来说是in
//
//
//		//		infile.open("E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\0122_gugu.txt");//打开文件
//		//		for (int i = 0; i < 100; i++)//定义行循环
//		//		{
//		//			for (int j = 0; j < 6; j++)//定义列循环
//		//			{
//		//				infile >> data[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
//		//			}
//		//		}
//		//		infile.close();//读取完成之后关闭文件
//		//		//读胫骨
//		//		float data_jing[100][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
//		//		ifstream infile_jing;//定义读取文件流，相对于程序来说是in
//
//		//		infile_jing.open("E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\0122_jinggu.txt");//打开文件
//		//		for (int i = 0; i < 100; i++)//定义行循环
//		//		{
//		//			for (int j = 0; j < 6; j++)//定义列循环
//		//			{
//		//				infile_jing >> data_jing[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
//		//			}
//		//		}
//		//		infile_jing.close();//读取完成之后关闭文件
//
//		//		//actor->SetPosition(data[n * 1][0]-125, data[n * 1][1]+40, data[n * 1][2]+2000);
//		//		actor->SetPosition(data_jing[n * 1][3], data_jing[n * 1][4], data_jing[n * 1][5] + 2000);
//		//		//actor2->SetPosition(data[n * 1][3] / 20 - 100, data[n * 1][4] / 20 + 200, data[n * 1][5] / 20);
//		//		//actorSTL2->SetOrientation(data[n * 20][0], data[n * 20][1], data[n * 20][2]);
//		//	//	actor->SetOrientation(actor->GetOrientation()[0], (data[n * 1][1] + 90), actor->GetOrientation()[2]);
//
//		//		actor->RotateZ(data[n * 1][1]- data[n -1][1]); actor->RotateY(data[n * 1][2] - data[n - 1][2]); actor->RotateX(data[n * 1][3] - data[n - 1][3]);
//		//	//	actor2->RotateZ(data_jing[n * 1][1] - data_jing[n - 1][1]); actor2->RotateY(data_jing[n * 1][2] - data_jing[n - 1][2]); actor2->RotateX(data_jing[n * 1][3] - data_jing[n - 1][3]);
//		//	//	actor2->SetOrientation(actor2->GetOrientation()[0], (data[n * 1][1] + 90) * 2-5, actor2->GetOrientation()[2]);
//		//		vtkSmartPointer<vtkRenderWindow> temRW = vtkSmartPointer<vtkRenderWindow>::New();
//		//		temRW = renderer->GetRenderWindow();
//		//		renderer->AddActor(actor);
//		//		//renderer->AddActor(actor2);
//		//		temRW->Render();
//		//	}
//			if (n > 1000)
//			{
//				break;
//			}
//		}
//		renderWindowInteractor->Start();
//	}


	else if (ui->comboBox_2->currentIndex() == 3) {


	vtkSmartPointer<vtkAxesActor> actorWorld = vtkSmartPointer<vtkAxesActor>::New();
	actorWorld->SetPosition(0, 0, 0);
	actorWorld->SetTotalLength(30., 30., 30.);
	actorWorld->SetShaftType(0);
	actorWorld->SetAxisLabels(1);
	actorWorld->SetCylinderRadius(0.02);
	actorWorld->SetXAxisLabelText("x");
	actorWorld->SetYAxisLabelText("y");
	actorWorld->SetZAxisLabelText("z");

	folder = "E:\\NK\\04_data\\kouqiangyiyuanCT\\21-0430_Zhanglinkun\\xiahe_except";
	vtkSmartPointer<vtkDICOMImageReader> reader1 =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader1->SetDirectoryName(folder.c_str());
	reader1->Update();
	vtkSmartPointer<vtkPolyData> input1 = vtkSmartPointer<vtkPolyData>::New();
	input1->DeepCopy(reader1->GetOutput());
	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection(reader1->GetOutputPort());
	//skinExtractor->SetValue(0, 500);    //值越大，保留的部分越少。
	skinExtractor->SetValue(0, 100);    //值越大，保留的部分越少。

	//做平滑处理
	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
	smooth->SetInputConnection(skinExtractor->GetOutputPort());
	smooth->SetNumberOfIterations(100);

	//重新计算法向量
	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();

	skinNormals->SetInputConnection(smooth->GetOutputPort());
	skinNormals->SetFeatureAngle(180);

	vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(skinMapper);
	actor->GetProperty()->SetOpacity(1);
	// Create a renderer, render window, and interactor
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	// Add the actor to the scene
	//renderer->AddActor(actor);
	renderer->AddActor(actorWorld);
	std::string folder2 = "E:\\NK\\04_data\\kouqiangyiyuanCT\\21-0430_Zhanglinkun\\xiahe";
	vtkSmartPointer<vtkDICOMImageReader> reader2 =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader2->SetDirectoryName(folder2.c_str());
	reader2->Update();

	vtkSmartPointer<vtkContourFilter> skinExtractor2 =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor2->SetInputConnection(reader2->GetOutputPort());
	//skinExtractor->SetValue(0, 500);    //值越大，保留的部分越少。
	skinExtractor2->SetValue(0, 100);    //值越大，保留的部分越少。


	vtkSmartPointer<vtkCubeAxesActor> axes = vtkSmartPointer<vtkCubeAxesActor>::New();
	axes->SetBounds(skinExtractor2->GetOutput()->GetBounds());
	axes->DrawXGridlinesOn();
	axes->DrawYGridlinesOn();
	axes->DrawZGridlinesOn();
	axes->XAxisMinorTickVisibilityOff();
	axes->YAxisMinorTickVisibilityOff();
	axes->ZAxisMinorTickVisibilityOff();
	axes->SetXLabelFormat("%6.1f");
	qDebug("skinExtractor2->GetOutput()->GetBounds() =%lf,%lf,%lf ", skinExtractor2->GetOutput()->GetBounds()[0], skinExtractor2->GetOutput()->GetBounds()[1], skinExtractor2->GetOutput()->GetBounds()[2]);


	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth2 = vtkSmoothPolyDataFilter::New();
	smooth2->SetInputConnection(skinExtractor2->GetOutputPort());
	smooth2->SetNumberOfIterations(100);
	vtkSmartPointer<vtkPolyDataMapper> skinMapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper2->SetInputConnection(smooth2->GetOutputPort());
	skinMapper2->ScalarVisibilityOff();    //这样不会带颜色
	vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper(skinMapper2);
	actor2->GetProperty()->SetOpacity(1);
	actor2->GetProperty()->SetColor(0.56, 0.93, 0.56);
	//actor2->SetPosition(-105, -70, -35);
	renderer->AddActor(actor2);
	ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer);

	//vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();

	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//renderWindowInteractor->SetPicker(pointPicker);
	//renderWindowInteractor->SetRenderWindow(ui->qvtkWidget_4->GetRenderWindow());


	vtkTransform *myTrans = vtkTransform::New();
	//头颅模型
	myTrans->RotateX(90);
	myTrans->RotateZ(30);
	actor->SetUserTransform(myTrans);

	vtkTransform *myTrans2 = vtkTransform::New();
	//头颅模型
	//myTrans2->Scale(1.5, 1.5, 1.5);
	myTrans2->RotateX(90);
	myTrans2->RotateZ(30);
	//myTrans2->Translate(0, 0, 10);
	actor2->SetUserTransform(myTrans2);



	float kaibikou[1500][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
	ifstream infile_kaibikou;//定义读取文件流，相对于程序来说是in

	infile_kaibikou.open("E:\\NK\\04_data\\kouqiangyiyuanCT\\21-0430_Zhanglinkun\\kaibikou.txt");//打开文件
	for (int i = 0; i < 410; i++)//定义行循环
	{
		for (int j = 0; j < 6; j++)//定义列循环
		{
			infile_kaibikou >> kaibikou[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
		}
	}
	infile_kaibikou.close();//读取完成之后关闭文件


	clock_t t1, t2;
	double t;//经历时间
	int n = 1;//经历多少个时间单位
	int  n_qie = 1, n_up = 1;//切骨迭代次数/被切下来part上移
	double step = 0.1;//步长
	double z_step = 0.2;//Z轴负向步长

	t1 = clock();	//for循环开始前
	double *cur_cyPos, *cur_cyRot;
	//cur_cyPos = actor2->GetPosition();
	//cur_cyRot = actor2->GetOrientation();
	//actor2->SetPosition(kaibikou[0][4], kaibikou[0][5], kaibikou[0][6]);

	////侧向相机位置
	//vtkCamera *aCamera = vtkCamera::New();
	//aCamera->SetViewUp(0, 1, 0);//设视角位置 
	//aCamera->SetPosition(-200, -50, -50);//设观察对象位
	//aCamera->SetFocalPoint(0, -100, 100);//设焦点 
	//aCamera->ComputeViewPlaneNormal();//自动
	////renderer->ResetCamera();
	//renderer->SetActiveCamera(aCamera);


	////正向相机位置
	//vtkCamera *aCamera = vtkCamera::New();
	//aCamera->SetViewUp(0, 1, 0);//设视角位置 
	//aCamera->SetPosition(-160, 0, 600);//设观察对象位
	//aCamera->SetFocalPoint(0, -100, 0);//设焦点 
	//aCamera->ComputeViewPlaneNormal();//自动
	////renderer->ResetCamera();
	//renderer->SetActiveCamera(aCamera);


	//俯视相机位置
	vtkCamera *aCamera = vtkCamera::New();
	aCamera->SetViewUp(4, 0, -7);//设视角位置 
	aCamera->SetPosition(130, 200, 30);//设观察对象位
	aCamera->SetFocalPoint(100, 0, 100);//设焦点 
	aCamera->ComputeViewPlaneNormal();//自动
	//renderer->ResetCamera();
	renderer->SetActiveCamera(aCamera);


	////相机位置
	//vtkCamera *aCamera = vtkCamera::New();
	//aCamera->SetViewUp(0, 1, 0);//设视角位置 
	//aCamera->SetPosition(100, 0, 800);//设观察对象位
	//aCamera->SetFocalPoint(100, -100, 0);//设焦点 
	//aCamera->ComputeViewPlaneNormal();//自动
	////renderer->ResetCamera();
	//renderer->SetActiveCamera(aCamera);
	while (1)
	{
		t2 = clock();	//for循环结束
		t = (double)(t2 - t1) / CLOCKS_PER_SEC;

		//if (t >= 1 && n <= 1000)//0.1秒运行一次 (刀切）
		if (t >= 0.255 &&n <= 2000)//1.5秒运行一次 (刀切）
		{
			t1 = t2; n++; n_qie++;
			double posx = kaibikou[n % 400][3] - kaibikou[3][3];// -actor2->GetPosition()[0];
			double posy = kaibikou[n % 400][4] - kaibikou[3][4];
			double posz = kaibikou[n % 400][5] - kaibikou[3][5];
			double rotx = kaibikou[n % 400][0] - kaibikou[3][0]; //-actor2->GetOrientation()[0];
			double roty = kaibikou[n % 400][1] - kaibikou[3][1];
			double rotz = kaibikou[n % 400][2] - kaibikou[3][2];
			qDebug("kaibikouPPP22 =%lf,%lf,%lf ", actor2->GetPosition()[0], actor2->GetPosition()[1], actor2->GetPosition()[2]);
			qDebug("kaibikouRRR22 =%lf,%lf,%lf ", actor2->GetOrientation()[0], actor2->GetOrientation()[1], actor2->GetOrientation()[2]);
			//actor2->SetPosition(posx, posy/4, posz/4);// - actor2->GetPosition()[0]
			//actor2->SetOrientation(rotx, roty, rotz);//(kaibikou[n % 400][2])//actor2->GetOrientation()[2]
			actor2->SetPosition(posx / 10 , posy / 2, posz / 10);// - actor2->GetPosition()[0]
			actor2->SetOrientation(rotx / 2, roty /32, rotz / 2);//(kaibikou[n % 400][2])//actor2->GetOrientation()[2]
			vtkSmartPointer<vtkRenderWindow> temRW = vtkSmartPointer<vtkRenderWindow>::New();
			temRW = renderer->GetRenderWindow();
			renderer->AddActor(actor2);
			temRW->Render();



			//renderer->GetActiveCamera()->Elevation(290);

			//renderer->AddViewProp(axes);
			//axes->SetCamera(renderer->GetActiveCamera());

			//vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
			//iren->SetRenderWindow(temRW);
			
		}
		if (n > 2000)
		{
			break;
		}
	}


	//vtkRenderWindow *renWin = vtkRenderWindow::New();
	//renWin->AddRenderer(renderer);
	//renWin->SetSize(300, 300);

	//vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	//iren->SetRenderWindow(temRW);

	//iren->Initialize();
	//iren->Start();



	////base
	//vtkSmartPointer<vtkActor> actorLocal[3];
	//vtkSmartPointer<vtkLineSource> lineSourceX, lineSourceY, lineSourceZ;
	//vtkSmartPointer<vtkPolyDataMapper> MapperX, MapperY, MapperZ;
	//double pos[100][3];//[ 100 ]坐标编号：W（世界坐标系）、   [3] :x,y,z轴
	//float LINE_LEN; int i = 0;

	//pos[i][0] = 0; pos[i][1] = 0; pos[i][2] = 0;//xyz初始坐标
	////世界坐标系
	//vtkSmartPointer<vtkAxesActor> actorWorld = vtkSmartPointer<vtkAxesActor>::New();
	//actorWorld->SetPosition(0, 0, 0);
	//actorWorld->SetTotalLength(30., 30., 30.);
	//actorWorld->SetShaftType(0);
	//actorWorld->SetAxisLabels(1);
	//actorWorld->SetCylinderRadius(0.02);
	//actorWorld->SetXAxisLabelText("x");
	//actorWorld->SetYAxisLabelText("y");
	//actorWorld->SetZAxisLabelText("z");
	//renderer->AddActor(actorWorld);

	///*------------------------------------------------------------------------------------------------------------*/
	//vtkTransform *myTrans = vtkTransform::New();
	//myTrans->RotateZ(-250);
	//myTrans->RotateY(180);
	////myTrans->RotateX(90);
	////actor->SetUserTransform(myTrans);
	////actor2->SetUserTransform(myTrans);

	//renderer->AddActor(actor);
	////renderer->AddActor(actor2);

	////相机位置
	//vtkCamera *aCamera = vtkCamera::New();
	//aCamera->SetViewUp(1, 0, 0);//设视角位置 
	////aCamera->SetPosition(-200, -200, 100);//设观察对象位(-120, -240, -60)
	//aCamera->SetPosition(-200, -300, 0);//设观察对象位(-120, -240, -60)
	//aCamera->SetFocalPoint(0, 100, 3);//设焦点 
	//aCamera->ComputeViewPlaneNormal();//自动

	//renderer->SetActiveCamera(aCamera);
	////渲染
	//renderer->SetBackground(.1, .1, .1); // Background color dark blue
	//ui->qvtkWidget_4->GetRenderWindow()->Render();
	////renderWindowInteractor->Start();

//此处剪切读取文件

	//float data[1500][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
	//ifstream infile;//定义读取文件流，相对于程序来说是in


	//infile.open("E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\0122_gugu.txt");//打开文件
	//for (int i = 0; i < 1500; i++)//定义行循环
	//{
	//	for (int j = 0; j < 6; j++)//定义列循环
	//	{
	//		infile >> data[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
	//	}
	//}
	//infile.close();//读取完成之后关闭文件
	////读胫骨
	//float data_jing[1500][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
	//ifstream infile_jing;//定义读取文件流，相对于程序来说是in

	//infile_jing.open("E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\0122_jinggu.txt");//打开文件
	//for (int i = 0; i < 1500; i++)//定义行循环
	//{
	//	for (int j = 0; j < 6; j++)//定义列循环
	//	{
	//		infile_jing >> data_jing[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
	//	}
	//}
	//infile_jing.close();//读取完成之后关闭文件


	//clock_t t1, t2;
	//double t;//经历时间
	//int n = 1;//经历多少个时间单位
	//int  n_qie = 1, n_up = 1;//切骨迭代次数/被切下来part上移
	//double step = 0.1;//步长
	//double z_step = 0.2;//Z轴负向步长

	//t1 = clock();	//for循环开始前
	//double *cur_cyPos;
	////cur_cyPos = actor2->GetPosition();
	//actor->SetPosition(data[0][4], data[0][5], data[0][6]);
	////actor2->SetPosition(data_jing[0][4], data_jing[0][5], data_jing[0][6]);
	//while (1)
	//{
	//	t2 = clock();	//for循环结束
	//	t = (double)(t2 - t1) / CLOCKS_PER_SEC;

	//	//if (t >= 1 && n <= 1000)//0.1秒运行一次 (刀切）
	//	if (t >= 0.017 &&n <= 316)//0.1秒运行一次 (刀切）
	//	{
	//		t1 = t2; n++; n_qie++;

	//		actor->SetPosition(data[n * 1][3], data[n * 1][4], data[n * 1][5] + 2000);
	//		//actor2->SetPosition(data_jing[n * 1][3], data_jing[n * 1][4], data_jing[n * 1][5] + 2000);
	//	//	actor2->SetPosition(data[n * 1][3] / 20 - 100, data[n * 1][4] / 20 + 200, data[n * 1][5] / 20);
	//		//actorSTL2->SetOrientation(data[n * 20][0], data[n * 20][1], data[n * 20][2]);
	//	//	actor->SetOrientation(actor->GetOrientation()[0], (data[n * 1][1] + 90), actor->GetOrientation()[2]);
	//		actor->RotateZ(data[n * 1][1] - data[n - 1][1]); actor->RotateY(data[n * 1][2] - data[n - 1][2]); actor->RotateX(data[n * 1][3] - data[n - 1][3]);
	//		//	actor2->RotateZ(data_jing[n * 1][1] - data_jing[n - 1][1]); actor2->RotateY(data_jing[n * 1][2] - data_jing[n - 1][2]); actor2->RotateX(data_jing[n * 1][3] - data_jing[n - 1][3]);
	//		//	actor2->SetOrientation(actor2->GetOrientation()[0], (data[n * 1][1] + 90) * 2-5, actor2->GetOrientation()[2]);
	//		vtkSmartPointer<vtkRenderWindow> temRW = vtkSmartPointer<vtkRenderWindow>::New();
	//		temRW = renderer->GetRenderWindow();
	//		renderer->AddActor(actor);
	//		//renderer->AddActor(actor2);
	//		temRW->Render();
	//	}
	//	if (n > 1000)
	//	{
	//		break;
	//	}
	//}
	//renderWindowInteractor->Start();
	}

	else if (ui->comboBox_2->currentIndex() == 4) {
		
		vtkSmartPointer<vtkRenderer> renderer;
		vtkSmartPointer<vtkAxesActor> actorWorld;
		vtkSmartPointer<vtkDICOMImageReader> readerdcm;
		vtkSmartPointer<vtkVolume> bone;
		vtkSmartPointer<vtkPolyData> inputSTL1;
		vtkSmartPointer<vtkSTLReader> readerSTL1;
		vtkSmartPointer<vtkPolyDataMapper> mapperSTL1;
		vtkSmartPointer<vtkActor> actorSTL1;
		vtkSmartPointer<vtkActor> actorSTL2;
		vtkSmartPointer<vtkPolyData> inputSTL2;
		vtkSmartPointer<vtkSTLReader> readerSTL2;
		vtkSmartPointer<vtkPolyDataMapper> mapperSTL2;
		vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		renderWindowInteractor->SetRenderWindow(renderWindow);

		vtkActor* a = nullptr;
		vtkActor* b = nullptr;
		double p[3];
		double p2[3];
		/*------------stl原点-------------*/
		double stl1[3] = { -105, -70, -35 };
		double stl2[3] = { -105, -70, -35 };
		//创建显示窗口
		renderer = vtkSmartPointer<vtkRenderer>::New();

		//世界坐标系
		//actorWorld = vtkSmartPointer<vtkAxesActor>::New();
		//actorWorld->SetPosition(0, 0, 0);
		//actorWorld->SetTotalLength(200, 200, 200);
		//actorWorld->SetShaftType(0);
		//actorWorld->SetAxisLabels(1);
		//actorWorld->SetCylinderRadius(0.1);
		//actorWorld->SetVisibility(1);
		//renderer->AddActor(actorWorld);

		// Render and interact
		renderer->SetBackground(.1, .1, .1); // Background color dark
		//renderWindow->AddRenderer(renderer);
		ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(renderer);
		renderer->ResetCamera();
		//renderWindow->Render();
		ui->qvtkWidget_4->GetRenderWindow()->Render();

		inputSTL1 = vtkSmartPointer<vtkPolyData>::New();
		readerSTL1 = vtkSmartPointer<vtkSTLReader>::New();
		readerSTL1->SetFileName("E:\\NK\\03_material\\Model_STL\\gujing.stl");
		readerSTL1->Update();
		//input1->DeepCopy(reader1->GetOutput());
		// Create a mapper and actor1
		mapperSTL1 = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapperSTL1->SetInputConnection(readerSTL1->GetOutputPort());
		actorSTL1 = vtkSmartPointer<vtkActor>::New();
		actorSTL1->SetMapper(mapperSTL1);
		actorSTL1->GetProperty()->SetOpacity(1);
		actorSTL1->GetProperty()->SetAmbientColor(0.935, 0.935, 0.935);
		actorSTL1->GetProperty()->SetColor(0.935, 0.935, 0.935);
		actorSTL1->GetProperty()->SetDiffuseColor(0.935, 0.935, 0.935);
		/*---------------------------模型 放 世界坐标系原点------------------------------*/
		//获取STL模型在模型坐标系下的位置
		p[0] = 0; p[1] = 0; p[2] = 0;
		inputSTL1->DeepCopy(readerSTL1->GetOutput());
		for (int j = 0; j < inputSTL1->GetNumberOfPoints(); j++)
		{
			inputSTL1->GetPoint(j, p);
		}
		//更改在局部坐标系下的位置，使之对其世界坐标系
		//actorSTL1->SetPosition(-p[0], -p[1], 0);
		actorSTL1->SetPosition(stl1[0], stl1[1], stl1[2]);
		/*---------------------------模型 放 世界坐标系原点end------------------------------*/

		// Read a stl file2.
		inputSTL2 = vtkSmartPointer<vtkPolyData>::New();
		readerSTL2 = vtkSmartPointer<vtkSTLReader>::New();
		readerSTL2->SetFileName("E:\\NK\\03_material\\Model_STL\\jinggu.stl");
		readerSTL2->Update();
		// Create a mapper2 and actor2
		mapperSTL2 = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapperSTL2->SetInputConnection(readerSTL2->GetOutputPort());
		actorSTL2 = vtkSmartPointer<vtkActor>::New();
		actorSTL2->SetMapper(mapperSTL2);
		//actorSTL2->GetProperty()->SetOpacity(1);
		//actorSTL2->GetProperty()->SetAmbientColor(0.635, 0.635, 0.635);
		actorSTL2->GetProperty()->SetColor(0.56, 0.93, 0.56);
		//actorSTL2->GetProperty()->SetDiffuseColor(0.635, 0.635, 0.635);
		/*---------------------------模型 放 世界坐标系原点------------------------------*/
		//获取STL模型在模型坐标系下的位置
		p2[0] = 0; p2[1] = 0; p2[2] = 0;
		inputSTL2->DeepCopy(readerSTL2->GetOutput());
		for (int j = 0; j < inputSTL2->GetNumberOfPoints(); j++)
		{
			inputSTL2->GetPoint(j, p2);
		}
		//更改在局部坐标系下的位置，使之对其世界坐标系
		//actorSTL2->SetPosition(0, -p2[1] + p[1] / 2, -p2[2]); //-p2[2]    
		actorSTL2->SetPosition(stl1[0], stl1[1], stl1[2]);
		b = actorSTL2;
		/*---------------------------模型 放 世界坐标系原点end------------------------------*/

		// Create a renderer, render window, and interactor

		vtkTransform *myTrans = vtkTransform::New();
		//头颅模型
		myTrans->RotateZ(-190);
		myTrans->RotateY(180);
		//myTrans->RotateX(90);
		actorSTL1->SetUserTransform(myTrans);
		actorSTL2->SetUserTransform(myTrans);

		renderer->AddActor(actorSTL1);
		renderer->AddActor(actorSTL2);

		//相机位置
		vtkCamera *aCamera = vtkCamera::New();
		aCamera->SetViewUp(0, 0, -1);//设视角位置 
		aCamera->SetPosition(-300, -600, -200);//设观察对象位
		aCamera->SetFocalPoint(0, 100, 3);//设焦点 
		aCamera->ComputeViewPlaneNormal();//自动
		//renderer->ResetCamera();
		renderer->SetActiveCamera(aCamera);
		//渲染
		//renderer->SetBackground(0, 0, 0);
		//renderer->ResetCamera();
		// Render and interact
		renderWindow->SetSize(800, 800);
		//renderWindow->SetWindowName("股骨第一刀");
		ui->qvtkWidget_4->GetRenderWindow()->Render();
		//renderWindowInteractor->Start();

		float data[1500][6] = { 0 };//定义一个1500*2的矩阵，用于存放数据
		ifstream infile;//定义读取文件流，相对于程序来说是in

		infile.open("E:\\NK\\04_data\\kouqiangyiyuanCT\\xiguanjie\\data.txt");//打开文件
		for (int i = 0; i < 1500; i++)//定义行循环
		{
			for (int j = 0; j < 6; j++)//定义列循环
			{
				infile >> data[i][j];//读取一个值（空格、制表符、换行隔开）就写入到矩阵中，行列不断循环进行
			}
		}
		infile.close();//读取完成之后关闭文件

		//时间
		clock_t t1, t2;
		double t;//经历时间
		int n = 1;//经历多少个时间单位
		int  n_qie = 1, n_up = 1;//切骨迭代次数/被切下来part上移
		double step = 0.1;//步长
		double z_step = 0.2;//Z轴负向步长

		t1 = clock();	//for循环开始前
		while (1)
		{
			t2 = clock();	//for循环结束
			t = (double)(t2 - t1) / CLOCKS_PER_SEC;

			if (t >= 0.1&&n <= 1200)//0.1秒运行一次 (刀切）
			{
				t1 = t2; n++; n_qie++;
				actorSTL1->SetPosition(data[n * 1][3] / 20 - 100, data[n * 1][4] / 20 + 150, data[n * 1][5] / 20 + 140);
				actorSTL2->SetPosition(data[n * 1][3] / 20 - 100, data[n * 1][4] / 20 + 150, data[n * 1][5] / 20 + 140);
				//actorSTL2->SetOrientation(data[n * 20][0], data[n * 20][1], data[n * 20][2]);
				actorSTL1->SetOrientation(actorSTL1->GetOrientation()[0], data[n][1]+90, actorSTL1->GetOrientation()[2]);
				//actorSTL2->SetOrientation(actorSTL2->GetOrientation()[0], (data[n * 4][1]+90)*4, actorSTL2->GetOrientation()[2]);
				actorSTL2->SetOrientation(actorSTL2->GetOrientation()[0], (data[n][1]+90)*2, actorSTL2->GetOrientation()[2]);
				vtkSmartPointer<vtkRenderWindow> temRW = vtkSmartPointer<vtkRenderWindow>::New();
				temRW = renderer->GetRenderWindow();
				renderer->AddActor(actorSTL1);
				renderer->AddActor(actorSTL2);
				temRW->Render();
			}
			if (n > 1210)
			{
				break;
			}
		}
	}





	else if (ui->comboBox_2->currentIndex() == 5) {
		vtkSmartPointer<vtkSphereSource> sphereSource =
			vtkSmartPointer<vtkSphereSource>::New();
		sphereSource->Update();

		vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(sphereSource->GetOutput());

		vtkSmartPointer<vtkActor> actor =
			vtkSmartPointer<vtkActor>::New();
		actor->GetProperty()->SetColor(0, 1, 0);
		actor->SetMapper(mapper);

		vtkSmartPointer<vtkRenderer> renderer =
			vtkSmartPointer<vtkRenderer>::New();
		renderer->AddActor(actor);
		renderer->SetBackground(1, 1, 1);

		vtkSmartPointer<vtkRenderWindow> rw =
			vtkSmartPointer<vtkRenderWindow>::New();
		rw->Render();
		rw->SetWindowName("CellPicker Interaction");
		rw->AddRenderer(renderer);

		vtkSmartPointer<vtkRenderWindowInteractor> rwi =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		rwi->SetRenderWindow(rw);
		/****************************************************************************/
		vtkSmartPointer<CellPickerInteractorStyle> style =
			vtkSmartPointer<CellPickerInteractorStyle>::New();
		style->SetDefaultRenderer(renderer);
		//style->polyData = sphereSource->GetOutput();
		rwi->SetInteractorStyle(style);

		rw->Render();
		rwi->Initialize();
		rwi->Start();
	
	}

}


//四视图联动
class vtkWidgetWindowLevelCallback :public vtkCommand
{
public:static vtkWidgetWindowLevelCallback *New()
{
	return new vtkWidgetWindowLevelCallback;
}
	   void Execute(vtkObject *caller, unsigned long vtkNotUsed(event), void*callData) {
		   vtkImagePlaneWidget*self = reinterpret_cast<vtkImagePlaneWidget*>(caller);
		   if (!self)return;
		   double* wl = static_cast<double*>(callData);
		   if (self == this->WidgetX)
		   {
			   this->WidgetY->SetWindowLevel(wl[0], wl[1]);
			   this->WidgetZ->SetWindowLevel(wl[0], wl[1]);
		   }
		   else if(self == this->WidgetY)
		   {
			   this->WidgetX->SetWindowLevel(wl[0], wl[1]);
			   this->WidgetZ->SetWindowLevel(wl[0], wl[1]);
		   }
		   else if(self == this->WidgetZ)
		   {
			   this->WidgetX->SetWindowLevel(wl[0], wl[1]);
			   this->WidgetY->SetWindowLevel(wl[0], wl[1]);
		   }
	   }
	   vtkWidgetWindowLevelCallback() :WidgetX(0), WidgetY(0), WidgetZ(0) {}
	   vtkImagePlaneWidget*WidgetX;
	   vtkImagePlaneWidget*WidgetY;
	   vtkImagePlaneWidget*WidgetZ;
};




/*=========================================================================

  Program:   Visualization Toolkit
  Module:    ImagePlaneWidget.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

	 This software is distributed WITHOUT ANY WARRANTY; without even
	 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	 PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSmartPointer.h"

#include "vtkActor.h"
#include "vtkBiDimensionalWidget.h"
#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkCommand.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapToColors.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageReader.h"
#include "vtkImageReslice.h"
#include "vtkInteractorEventRecorder.h"
#include "vtkInteractorStyleImage.h"
#include "vtkLookupTable.h"
#include "vtkOutlineFilter.h"
#include "vtkPlane.h"
#include "vtkPlaneSource.h"
#include "vtkPointData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkResliceCursor.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursorWidget.h"
#include "vtkVolume16Reader.h"


//----------------------------------------------------------------------------
class vtkResliceCursorCallback : public vtkCommand
{
public:
	static vtkResliceCursorCallback* New() { return new vtkResliceCursorCallback; }

	void Execute(vtkObject* caller, unsigned long /*ev*/, void* callData) override
	{
		vtkImagePlaneWidget* ipw = dynamic_cast<vtkImagePlaneWidget*>(caller);
		if (ipw)
		{
			double* wl = static_cast<double*>(callData);

			if (ipw == this->IPW[0])
			{
				this->IPW[1]->SetWindowLevel(wl[0], wl[1], 1);
				this->IPW[2]->SetWindowLevel(wl[0], wl[1], 1);
			}
			else if (ipw == this->IPW[1])
			{
				this->IPW[0]->SetWindowLevel(wl[0], wl[1], 1);
				this->IPW[2]->SetWindowLevel(wl[0], wl[1], 1);
			}
			else if (ipw == this->IPW[2])
			{
				this->IPW[0]->SetWindowLevel(wl[0], wl[1], 1);
				this->IPW[1]->SetWindowLevel(wl[0], wl[1], 1);
			}
		}

		vtkResliceCursorWidget* rcw = dynamic_cast<vtkResliceCursorWidget*>(caller);
		if (rcw)
		{
			vtkResliceCursorLineRepresentation* rep =
				dynamic_cast<vtkResliceCursorLineRepresentation*>(rcw->GetRepresentation());
			vtkResliceCursor* rc = rep->GetResliceCursorActor()->GetCursorAlgorithm()->GetResliceCursor();
			for (int i = 0; i < 3; i++)
			{
				vtkPlaneSource* ps = static_cast<vtkPlaneSource*>(this->IPW[i]->GetPolyDataAlgorithm());
				ps->SetNormal(rc->GetPlane(i)->GetNormal());
				ps->SetCenter(rc->GetPlane(i)->GetOrigin());

				// If the reslice plane has modified, update it on the 3D widget
				this->IPW[i]->UpdatePlacement();

				// std::cout << "Updating placement of plane: " << i << " " <<
				//  rc->GetPlane(i)->GetNormal()[0] << " " <<
				//  rc->GetPlane(i)->GetNormal()[1] << " " <<
				//  rc->GetPlane(i)->GetNormal()[2] << std::endl;
				// this->IPW[i]->GetReslice()->Print(cout);
				// rep->GetReslice()->Print(cout);
				// std::cout << "---------------------" << std::endl;
			}
		}

		// Render everything
		this->RCW[0]->Render();
	}

	vtkResliceCursorCallback() = default;
	vtkImagePlaneWidget* IPW[3];
	vtkResliceCursorWidget* RCW[3];
};

//----------------------------------------------------------------------------
void MainWindow::test()
{
	
	vtkVolume *volume = vtkVolume::New();
	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
	folder = ui->label_2->text().toStdString();
	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();






	int count = 1;
	const char *dirname = folder.data();//ITK注释
	double opacityWindow = 4096;
	double opacityLevel = 2048;
	int blendType = 0;
	int clip = 0;
	double reductionFactor = 1.0;
	double frameRate = 10.0;
	char *fileName = 0;
	int fileType = 0;

	bool independentComponents = true;

	vtkGPUVolumeRayCastMapper *mapper = vtkGPUVolumeRayCastMapper::New();
	mapper->SetInputConnection(reader->GetOutputPort());

	vtkColorTransferFunction *colorFun = vtkColorTransferFunction::New();
	vtkPiecewiseFunction *opacityFun = vtkPiecewiseFunction::New();

	// Create the property and attach the transfer functions
	vtkVolumeProperty *property = vtkVolumeProperty::New();
	property->SetIndependentComponents(independentComponents);
	property->SetColor(colorFun);
	property->SetScalarOpacity(opacityFun);
	property->SetInterpolationTypeToLinear();

	volume->SetProperty(property);
	volume->SetMapper(mapper);

	colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0);
	opacityFun->AddSegment(opacityLevel - 0.5*opacityWindow, 0.0,
		opacityLevel + 0.5*opacityWindow, 1.0);
	mapper->SetBlendModeToComposite();
	property->ShadeOn();






	//vtkSmartPointer<vtkContourFilter> skinExtractor =
	//	vtkSmartPointer<vtkContourFilter>::New();
	//skinExtractor->SetInputConnection(reader->GetOutputPort());
	//skinExtractor->SetValue(0, 1500);    //值越大，保留的部分越少。

	////做平滑处理
	//vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
	//smooth->SetInputConnection(skinExtractor->GetOutputPort());
	//smooth->SetNumberOfIterations(100);

	////重新计算法向量
	//vtkSmartPointer<vtkPolyDataNormals> skinNormals =
	//	vtkSmartPointer<vtkPolyDataNormals>::New();

	//skinNormals->SetInputConnection(smooth->GetOutputPort());
	//skinNormals->SetFeatureAngle(180);

	//vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
	//	vtkSmartPointer<vtkStripper>::New();
	//skinStripper->SetInputConnection(skinNormals->GetOutputPort());

	//vtkSmartPointer<vtkPolyDataMapper> skinMapper =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	//skinMapper->ScalarVisibilityOff();    //这样不会带颜色

	//vtkSmartPointer<vtkActor> skin =
	//	vtkSmartPointer<vtkActor>::New();
	//skin->SetMapper(skinMapper);

	//vtkSmartPointer<vtkOutlineFilter> outlineData =
	//	vtkSmartPointer<vtkOutlineFilter>::New();
	//outlineData->SetInputConnection(reader->GetOutputPort());

	//vtkSmartPointer<vtkPolyDataMapper> mapOutline =
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapOutline->SetInputConnection(outlineData->GetOutputPort());

	//vtkSmartPointer<vtkActor> outline =
	//	vtkSmartPointer<vtkActor>::New();
	//outline->SetMapper(mapOutline);
	//outline->GetProperty()->SetColor(0, 0, 0);

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	iren->SetInteractorStyle(style);

	vtkSmartPointer<vtkRenderer> ren[4];

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetMultiSamples(0);

	for (int i = 0; i < 4; i++)
	{
		ren[i] = vtkSmartPointer<vtkRenderer>::New();
		renWin->AddRenderer(ren[i]);
	}

	
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.005);

	vtkSmartPointer<vtkProperty> ipwProp = vtkSmartPointer<vtkProperty>::New();

	// assign default props to the ipw's texture plane actor
	vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];
	int imageDims[3];
	reader->GetOutput()->GetDimensions(imageDims);

	for (int i = 0; i < 3; i++)
	{
		planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
		planeWidget[i]->SetInteractor(iren);
		planeWidget[i]->SetPicker(picker);
		planeWidget[i]->RestrictPlaneToVolumeOn();
		double color[3] = { 0, 0, 0 };
		color[i] = 1;
		planeWidget[i]->GetPlaneProperty()->SetColor(color);
		planeWidget[i]->SetTexturePlaneProperty(ipwProp);
		planeWidget[i]->TextureInterpolateOff();
		planeWidget[i]->SetResliceInterpolateToLinear();
		planeWidget[i]->SetInputConnection(reader->GetOutputPort());
		planeWidget[i]->SetPlaneOrientation(i);
		planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
		planeWidget[i]->DisplayTextOn();
		planeWidget[i]->SetDefaultRenderer(ren[3]);
		planeWidget[i]->SetWindowLevel(1358, -27);
		planeWidget[i]->On();
		planeWidget[i]->InteractionOn();
	}

	planeWidget[1]->SetLookupTable(planeWidget[0]->GetLookupTable());
	planeWidget[2]->SetLookupTable(planeWidget[0]->GetLookupTable());

	vtkSmartPointer<vtkResliceCursorCallback> cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();

	// Create the reslice cursor, widget and rep

	vtkSmartPointer<vtkResliceCursor> resliceCursor = vtkSmartPointer<vtkResliceCursor>::New();
	resliceCursor->SetCenter(reader->GetOutput()->GetCenter());
	resliceCursor->SetThickMode(0);
	resliceCursor->SetThickness(0.1, 0.1, 0.1);
	resliceCursor->SetImage(reader->GetOutput());

	vtkSmartPointer<vtkResliceCursorWidget> resliceCursorWidget[3];
	vtkSmartPointer<vtkResliceCursorLineRepresentation> resliceCursorRep[3];

	double viewUp[3][3] = { { 0, 0, -1 }, { 0, 0, 1 }, { 0, 1, 0 } };
	for (int i = 0; i < 3; i++)
	{
		resliceCursorWidget[i] = vtkSmartPointer<vtkResliceCursorWidget>::New();
		resliceCursorWidget[i]->SetInteractor(iren);

		resliceCursorRep[i] = vtkSmartPointer<vtkResliceCursorLineRepresentation>::New();
		resliceCursorWidget[i]->SetRepresentation(resliceCursorRep[i]);
		resliceCursorRep[i]->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(
			resliceCursor);
		resliceCursorRep[i]->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);

		const double minVal = reader->GetOutput()->GetScalarRange()[0];
		if (vtkImageReslice* reslice = vtkImageReslice::SafeDownCast(resliceCursorRep[i]->GetReslice()))
		{
			reslice->SetBackgroundColor(minVal, minVal, minVal, minVal);
		}

		resliceCursorWidget[i]->SetDefaultRenderer(ren[i]);
		resliceCursorWidget[i]->SetEnabled(1);

		ren[i]->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		double camPos[3] = { 0, 0, 0 };
		camPos[i] = 1;
		ren[i]->GetActiveCamera()->SetPosition(camPos);

		ren[i]->GetActiveCamera()->ParallelProjectionOn();
		ren[i]->GetActiveCamera()->SetViewUp(viewUp[i][0], viewUp[i][1], viewUp[i][2]);
		ren[i]->ResetCamera();
		// ren[i]->ResetCameraClippingRange();

		// Tie the Image plane widget and the reslice cursor widget together
		cbk->IPW[i] = planeWidget[i];
		cbk->RCW[i] = resliceCursorWidget[i];
		resliceCursorWidget[i]->AddObserver(vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		// Initialize the window level to a sensible value
		double range[2];
		reader->GetOutput()->GetScalarRange(range);
		resliceCursorRep[i]->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
		planeWidget[i]->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);

		// Make them all share the same color map.
		resliceCursorRep[i]->SetLookupTable(resliceCursorRep[0]->GetLookupTable());
		planeWidget[i]->GetColorMap()->SetLookupTable(resliceCursorRep[0]->GetLookupTable());
	}

	// Add the actors
	//
	ren[0]->SetBackground(0.3, 0.1, 0.1);
	ren[1]->SetBackground(0.1, 0.3, 0.1);
	ren[2]->SetBackground(0.1, 0.1, 0.3);
	//ren[0]->SetBackground(0, 0, 0);
	//ren[1]->SetBackground(0, 0, 0);
	//ren[2]->SetBackground(0, 0, 0);
	//ren[3]->AddActor(outline);
	//ren[3]->AddActor(skin);
	ren[3]->AddVolume(volume);
	ren[3]->SetBackground(0.1, 0.1, 0.1);
	//ren[3]->SetBackground(0, 0, 0);
	renWin->SetSize(2000, 1000);
	//renWin->SetFullScreen(1);

	ren[0]->SetViewport(0, 0, 0.5, 0.5);
	ren[1]->SetViewport(0.5, 0, 1, 0.5);
	ren[2]->SetViewport(0, 0.5, 0.5, 1);
	ren[3]->SetViewport(0.5, 0.5, 1, 1);

	// Set the actors' positions
	//
	renWin->Render();

	ren[3]->GetActiveCamera()->Elevation(110);
	ren[3]->GetActiveCamera()->SetViewUp(0, 0, -1);
	ren[3]->GetActiveCamera()->Azimuth(45);
	ren[3]->GetActiveCamera()->Dolly(1.15);
	ren[3]->ResetCameraClippingRange();

	//vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	iren->SetInteractorStyle(style);

	iren->Initialize();

	iren->Start();
}

//#include "vtkWindowToImageFilter.h"
//#include "vtkJPEGWriter.h"
//#include "vtkImageCanvasSource2D.h"
//void MainWindow::brain4()
//{
//	vtkSmartPointer<vtkCylinderSource> cylinderSource =
//		vtkSmartPointer<vtkCylinderSource>::New();
//	cylinderSource->SetHeight(10.0);
//	cylinderSource->SetCenter(0.0, 0.0, 0.0);
//	cylinderSource->SetRadius(2.0);
//	cylinderSource->SetResolution(50);
//
//	vtkSmartPointer<vtkLineSource> lineSource =
//		vtkSmartPointer<vtkLineSource>::New();
//	lineSource->SetPoint1(5.0, 0.0, 0.0);
//	lineSource->SetPoint2(-5.0, 0.0, 0.0);
//	vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
//	tubeFilter->SetInputConnection(lineSource->GetOutputPort());
//	tubeFilter->SetRadius(2.0);
//	tubeFilter->SetNumberOfSides(50);
//	tubeFilter->CappingOn();
//
//	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper1 =
//		vtkSmartPointer<vtkPolyDataMapper>::New();
//	cylinderMapper1->SetInputConnection(cylinderSource->GetOutputPort());
//
//	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper2 =
//		vtkSmartPointer<vtkPolyDataMapper>::New();
//	cylinderMapper2->SetInputConnection(tubeFilter->GetOutputPort());
//
//	vtkSmartPointer<vtkActor> cylinderActor1 =
//		vtkSmartPointer<vtkActor>::New();
//	cylinderActor1->SetMapper(cylinderMapper1);
//
//	vtkSmartPointer<vtkActor> cylinderActor2 =
//		vtkSmartPointer<vtkActor>::New();
//	cylinderActor2->SetMapper(cylinderMapper2);
//
//	vtkSmartPointer<vtkRenderer> renderer1 =
//		vtkSmartPointer<vtkRenderer>::New();
//	vtkSmartPointer<vtkRenderer> renderer2 =
//		vtkSmartPointer<vtkRenderer>::New();
//
//	vtkSmartPointer<vtkRenderWindow> renderWindow =
//		vtkSmartPointer<vtkRenderWindow>::New();
//	renderWindow->SetSize(600, 600);
//	renderWindow->AddRenderer(renderer1);
//	renderWindow->AddRenderer(renderer2);
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//		vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	renderWindowInteractor->SetRenderWindow(renderWindow);
//
//	double leftViewport[] = { 0.0, 0.0, 0.5, 1.0 };
//	double rightViewport[] = { 0.5, 0.0, 1.0, 1.0 };
//
//	cylinderActor1->GetProperty()->SetColor(0.0, 1.0, 0.0);
//	cylinderActor2->GetProperty()->SetColor(0.0, 0.0, 1.0);
//
//	renderer1->AddActor(cylinderActor1);
//	renderer2->AddActor(cylinderActor2);
//	renderer1->SetBackground(.3, .3, .5); // Background color purple
//	renderer2->SetBackground(.2, .4, .5);
//	renderer1->SetViewport(leftViewport);
//	renderer2->SetViewport(rightViewport);
//
//	std::string outputFilename = "output.jpg";
//
//	//int extent[6] = { 0, 99, 0, 99, 0, 0 };
//	//vtkSmartPointer<vtkImageCanvasSource2D> imageSource =
//	//	vtkSmartPointer<vtkImageCanvasSource2D>::New();
//	//imageSource->SetExtent(extent);
//	//imageSource->SetScalarTypeToUnsignedChar();
//	//imageSource->SetNumberOfScalarComponents(3);
//
//	//imageSource->SetDrawColor(0, 127, 255);
//	//imageSource->FillBox(extent[0], extent[1], extent[2], extent[3]);
//
//	//imageSource->SetDrawColor(255, 255, 255);
//	//imageSource->FillBox(40, 70, 20, 50);
//
//	//vtkSmartPointer<vtkJPEGWriter> writer =
//	//	vtkSmartPointer<vtkJPEGWriter>::New();
//	//writer->SetFileName(outputFilename.c_str());
//	//writer->SetInputConnection(imageSource->GetOutputPort());
//	//writer->Write();
//
//	auto wif =vtkSmartPointer<vtkWindowToImageFilter>::New();
//	wif->SetInput(renderWindow);
//
//	auto writer = vtkSmartPointer<vtkJPEGWriter>::New();
//	writer->SetFileName(outputFilename.c_str());
//	//writer->SetWriteToMemory(1);
//	writer->SetInputConnection(cylinderMapper2->GetOutputPort());
//	writer->Write();
//	auto array = writer->GetResult();
//
//	//auto bytes = new byte[array->getnumberoftuples()];
//	//for (int i = 0; i < array->getnumberoftuples(); ++i)
//	//{
//	//	bytes[i] = (byte)array->gettuple1(i);
//	//}
//
//	//folder = ui->label_2->text().toStdString();
//
//	//SocketSend(bytes);
//
//	//auto folder = bytes.tostdstring();
//	//QString qstr2 = QString::fromstdstring(bytes);
//	//SocketSend(bytes);
//
//	renderWindow->Render();
//	renderWindowInteractor->Start();
//}

#include "itkImage.h"
#include <iostream>
#include<itkAndImageFilter.h>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>
#include "vtkVersion.h"
#include "vtkImageViewer.h"
#include "vtkImageMapper3D.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "itkRGBPixel.h"
#include "itkGDCMImageIO.h"

//void MainWindow::brain4()
//{
//	typedef itk::Image<itk::RGBPixel<unsigned char>, 2> ImageType;
//	typedef itk::ImageFileReader<ImageType>             ReaderType;
//	typedef itk::ImageToVTKImageFilter<ImageType>       ConnectorType;
//	//itk::ImageIOFactory::Register;
//	ReaderType::Pointer reader = ReaderType::New();
//	ConnectorType::Pointer connector = ConnectorType::New();
//	reader->SetFileName("E:\\NK\\04_data\\Juan\\20200519\\20190327\\17240001-\\55377351");
//	connector->SetInput(reader->GetOutput());
//
//	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
//
//	connector->Update();
//	actor->GetMapper()->SetInputData(connector->GetOutput());
//	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	renderer->AddActor(actor);
//	renderer->ResetCamera();
//	vtkSmartPointer<vtkRenderWindow> renderWindow =
//		vtkSmartPointer<vtkRenderWindow>::New();
//	renderWindow->AddRenderer(renderer);
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//		vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	vtkSmartPointer<vtkInteractorStyleImage> style =
//		vtkSmartPointer<vtkInteractorStyleImage>::New();
//	renderWindowInteractor->SetInteractorStyle(style);
//	renderWindowInteractor->SetRenderWindow(renderWindow);
//	renderWindowInteractor->Initialize();
//	renderWindowInteractor->Start();
//
//
////	typedef itk::Image< unsigned short, 3 > ImageType;
////	ImageType::Pointer image = ImageType::New();
////	std::cout << "ITK Hello World !" << std::endl;
//
//}
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "QuickView.h"


//IO相关
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "RpcRT4.lib")
#pragma  comment(lib,"Psapi.lib")

//void MainWindow::brain4()
//{
//	typedef   float           InternalPixelType;
//	const     unsigned int    Dimension = 2;
//	typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
//
//	typedef  float      OutputPixelType;
//	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
//
//	typedef  itk::ImageFileReader< InternalImageType > ReaderType;
//	typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;
//
//	ReaderType::Pointer reader = ReaderType::New();
//	WriterType::Pointer writer = WriterType::New();
//
//	//DICOM数据的ImageIO
//	typedef itk::GDCMImageIO      ImageIOType;
//	ImageIOType::Pointer gdcmIO = ImageIOType::New();
//
//	reader->SetImageIO(gdcmIO);
//	reader->SetFileName("E://NK//04_data//301//09001353//2//U0000108");
//
//	reader->Update();
//
//	//设置输出文件
//	writer->SetFileName("E://NK//04_data//301//09001353//test//84485356.dcm");
//	//设置文件头
//	writer->UseInputMetaDataDictionaryOff();
//
//	writer->SetInput(reader->GetOutput());
//	writer->SetImageIO(gdcmIO);
//	writer->Update();
//}



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
using PixelType = float;
using ImageType = itk::Image< PixelType, Dimension >;

// Software Guide : EndCodeSnippet
//ITKbrain4狗腿
//void MainWindow::brain4()
//{
//	using ReaderType = itk::ImageSeriesReader< ImageType >;
//	using ImageIOType = itk::GDCMImageIO;
//	using NamesGeneratorType = itk::GDCMSeriesFileNames;
//	ReaderType::Pointer reader = ReaderType::New();
//	ImageIOType::Pointer dicomIO = ImageIOType::New();
//	reader->SetImageIO(dicomIO);
//	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
//	nameGenerator->SetUseSeriesDetails(true);
//	nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\20190327\\17240001");//膝盖
//	nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\20200519\\Dog_Ti_Mark\\CT");//狗腿
//	nameGenerator->SetDirectory("E:\\NK\\04_data\\Juan\\Xi_CT");//腿
//	nameGenerator->SetDirectory("E:\\NK\\04_data\\kouqiang");//口腔
//	using SeriesIdContainer = std::vector< std::string >;
//	const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
//	auto seriesItr = seriesUID.begin();
//	auto seriesEnd = seriesUID.end();
//
//	using FileNamesContainer = std::vector< std::string >;
//	FileNamesContainer fileNames;
//	std::string seriesIdentifier;
//	while (seriesItr != seriesEnd)
//	{
//
//		seriesIdentifier = seriesItr->c_str();
//		fileNames = nameGenerator->GetFileNames(seriesIdentifier);
//		++seriesItr;
//	}
//
//	reader->SetFileNames(fileNames);
//
//	try
//	{
//		reader->Update();
//	}
//	catch (itk::ExceptionObject &ex)
//	{
//		std::cout << ex << std::endl;
//	}
//	ImageType::SizeType imgSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
//	cout << "read done！Original size: " << imgSize << endl;
//
//	typedef itk::ImageToVTKImageFilter< ImageType> itkTovtkFilterType;
//	itkTovtkFilterType::Pointer itkTovtkImageFilter = itkTovtkFilterType::New();
//	itkTovtkImageFilter->SetInput(reader->GetOutput());
//	itkTovtkImageFilter->Update();
//
//
//
//
//	抽取等值面为骨头的信息
//	vtkSmartPointer< vtkMarchingCubes > boneExtractor =
//		vtkSmartPointer< vtkMarchingCubes >::New();
//	boneExtractor->SetInputData(itkTovtkImageFilter->GetOutput());
//	boneExtractor->SetValue(0, 500); //设置提取的等值信息
//	boneExtractor->Update();
//
//	剔除旧的或废除的数据单元，提高绘制速度(可略去这一步)
//	vtkSmartPointer< vtkStripper > boneStripper =
//		vtkSmartPointer< vtkStripper >::New(); //三角带连接
//	boneStripper->SetInputConnection(boneExtractor->GetOutputPort());
//	boneStripper->Update();
//
//	建立映射
//	vtkSmartPointer< vtkPolyDataMapper > boneMapper =
//		vtkSmartPointer< vtkPolyDataMapper >::New();
//	boneMapper->SetInputData(boneStripper->GetOutput());
//
//	建立角色
//	vtkSmartPointer< vtkActor > bone =
//		vtkSmartPointer< vtkActor >::New();
//	bone->SetMapper(boneMapper);
//
//	bone->GetProperty()->SetDiffuseColor(.1, .94, .52);
//	bone->GetProperty()->SetSpecular(.3);
//	bone->GetProperty()->SetSpecularPower(20);
//
//	定义绘制器
//	vtkSmartPointer< vtkRenderer > aRenderer =
//		vtkSmartPointer< vtkRenderer >::New();
//	定义绘制窗口
//	vtkSmartPointer< vtkRenderWindow > renWin =
//		vtkSmartPointer< vtkRenderWindow >::New();
//	renWin->AddRenderer(aRenderer);
//	定义窗口交互器
//	vtkSmartPointer< vtkRenderWindowInteractor > iren =
//		vtkSmartPointer< vtkRenderWindowInteractor >::New();
//	iren->SetRenderWindow(renWin);
//
//	创建一个camera
//	vtkSmartPointer< vtkCamera > aCamera =
//		vtkSmartPointer< vtkCamera >::New();
//	aCamera->SetViewUp(0, 0, -1);
//	aCamera->SetPosition(0, 1, 0);
//	aCamera->SetFocalPoint(0, 0, 0);
//
//	aRenderer->AddActor(bone);
//	aRenderer->SetActiveCamera(aCamera);
//	aRenderer->ResetCamera();
//	aCamera->Dolly(1.5);
//	aRenderer->SetBackground(0, 0, 0);
//	aRenderer->ResetCameraClippingRange();
//
//	iren->Initialize();
//	iren->Start();
//	vtkSmartPointer<vtkMarchingCubes> vesselExtractor = vtkMarchingCubes::New();
//	vesselExtractor->SetInputData(itkTovtkImageFilter->GetOutput());
//	vesselExtractor->SetNumberOfContours(10);
//	vesselExtractor->SetValue(0, 1);   //轮廓
//
//	//将提取的等值面拼接成连续的
//	vtkSmartPointer<vtkStripper> vesselStripper = vtkStripper::New();                           //建立三角带对象
//	vesselStripper->SetInputConnection(vesselExtractor->GetOutputPort());
//
//
//	vtkSmartPointer<vtkPolyDataMapper>  vesselMapper = vtkPolyDataMapper::New();     //建立一个数据映射对象
//	vesselMapper->SetInputConnection(vesselStripper->GetOutputPort());                     //将三角带映射为几何数据
//	vesselMapper->SetScalarRange(0, 7);
//
//	//对象和对象属性等设置
//	vtkSmartPointer<vtkActor> vessel = vtkActor::New();
//	vessel->SetMapper(vesselMapper);
//	vessel->GetProperty()->SetColor(0, 0, 1);
//
//	// A renderer and render window
//	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//	renderWindow->AddRenderer(renderer);
//	renderWindow->SetSize(1000, 1000);
//	renderer->AddActor(vessel);
//
//	// An interactor
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//		vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	renderWindowInteractor->SetRenderWindow(renderWindow);
//	renderWindow->Render();
//	renderWindow->SetWindowName("ITK测试");//这一句一定是在render()后面，否则无效，为什么没人告诉我
//	renderWindowInteractor->Initialize();
//	renderWindow->Render();
//	renderWindowInteractor->Start();
//}
/*
void MainWindow::test()
{
	std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();


	//vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
	//outline->SetInputConnection(reader->GetOutputPort());

	//vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//outlineMapper->SetInputConnection(outline->GetOutputPort());

	//vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	//outlineActor->SetMapper(outlineMapper);

	vtkSmartPointer<vtkRenderer> ren[4];

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetMultiSamples(0);

	for (int i = 0; i < 4; i++)
	{
		ren[i] = vtkSmartPointer<vtkRenderer>::New();
		renWin->AddRenderer(ren[i]);
	}

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.005);

	vtkSmartPointer<vtkProperty> ipwProp = vtkSmartPointer<vtkProperty>::New();

	// assign default props to the ipw's texture plane actor
	vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];
	int imageDims[3];
	reader->GetOutput()->GetDimensions(imageDims);

	for (int i = 0; i < 3; i++)
	{
		planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
		planeWidget[i]->SetInteractor(iren);
		planeWidget[i]->SetPicker(picker);
		planeWidget[i]->RestrictPlaneToVolumeOn();
		double color[3] = { 0, 0, 0 };
		color[i] = 1;
		planeWidget[i]->GetPlaneProperty()->SetColor(color);
		planeWidget[i]->SetTexturePlaneProperty(ipwProp);
		planeWidget[i]->TextureInterpolateOff();
		planeWidget[i]->SetResliceInterpolateToLinear();
		planeWidget[i]->SetInputConnection(reader->GetOutputPort());
		planeWidget[i]->SetPlaneOrientation(i);
		planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
		planeWidget[i]->DisplayTextOn();
		planeWidget[i]->SetDefaultRenderer(ren[3]);
		planeWidget[i]->SetWindowLevel(1358, -27);
		planeWidget[i]->On();
		planeWidget[i]->InteractionOn();
	}

	planeWidget[1]->SetLookupTable(planeWidget[0]->GetLookupTable());
	planeWidget[2]->SetLookupTable(planeWidget[0]->GetLookupTable());

	vtkSmartPointer<vtkResliceCursorCallback> cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();

	// Create the reslice cursor, widget and rep

	vtkSmartPointer<vtkResliceCursor> resliceCursor = vtkSmartPointer<vtkResliceCursor>::New();
	resliceCursor->SetCenter(reader->GetOutput()->GetCenter());
	resliceCursor->SetThickMode(0);
	resliceCursor->SetThickness(10, 10, 10);
	resliceCursor->SetImage(reader->GetOutput());

	vtkSmartPointer<vtkResliceCursorWidget> resliceCursorWidget[3];
	vtkSmartPointer<vtkResliceCursorLineRepresentation> resliceCursorRep[3];

	double viewUp[3][3] = { { 0, 0, -1 }, { 0, 0, 1 }, { 0, 1, 0 } };
	for (int i = 0; i < 3; i++)
	{
		resliceCursorWidget[i] = vtkSmartPointer<vtkResliceCursorWidget>::New();
		resliceCursorWidget[i]->SetInteractor(iren);

		resliceCursorRep[i] = vtkSmartPointer<vtkResliceCursorLineRepresentation>::New();
		resliceCursorWidget[i]->SetRepresentation(resliceCursorRep[i]);
		resliceCursorRep[i]->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(
			resliceCursor);
		resliceCursorRep[i]->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);

		const double minVal = reader->GetOutput()->GetScalarRange()[0];
		if (vtkImageReslice* reslice = vtkImageReslice::SafeDownCast(resliceCursorRep[i]->GetReslice()))
		{
			reslice->SetBackgroundColor(minVal, minVal, minVal, minVal);
		}

		resliceCursorWidget[i]->SetDefaultRenderer(ren[i]);
		resliceCursorWidget[i]->SetEnabled(1);

		ren[i]->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		double camPos[3] = { 0, 0, 0 };
		camPos[i] = 1;
		ren[i]->GetActiveCamera()->SetPosition(camPos);

		ren[i]->GetActiveCamera()->ParallelProjectionOn();
		ren[i]->GetActiveCamera()->SetViewUp(viewUp[i][0], viewUp[i][1], viewUp[i][2]);
		ren[i]->ResetCamera();
		// ren[i]->ResetCameraClippingRange();

		// Tie the Image plane widget and the reslice cursor widget together
		cbk->IPW[i] = planeWidget[i];
		cbk->RCW[i] = resliceCursorWidget[i];
		resliceCursorWidget[i]->AddObserver(vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		// Initialize the window level to a sensible value
		double range[2];
		reader->GetOutput()->GetScalarRange(range);
		resliceCursorRep[i]->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
		planeWidget[i]->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);

		// Make them all share the same color map.
		resliceCursorRep[i]->SetLookupTable(resliceCursorRep[0]->GetLookupTable());
		planeWidget[i]->GetColorMap()->SetLookupTable(resliceCursorRep[0]->GetLookupTable());
	}

	// Add the actors
	//
	//vtkSmartPointer<vtkCamera> aCamera =
	//	vtkSmartPointer<vtkCamera>::New();
	//aCamera->SetViewUp(0, 0, -1);
	//aCamera->SetPosition(0, 1, 0);
	//aCamera->SetFocalPoint(0, 0, 0);
	//aCamera->ComputeViewPlaneNormal();
	//aCamera->Azimuth(30.0);
	//aCamera->Elevation(30.0);
	//aCamera->Dolly(1.5);
	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection(reader->GetOutputPort());
	skinExtractor->SetValue(0, 1);    //值越大，保留的部分越少。

	//做平滑处理
	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmoothPolyDataFilter::New();
	smooth->SetInputConnection(skinExtractor->GetOutputPort());
	smooth->SetNumberOfIterations(100);

	//重新计算法向量
	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();

	skinNormals->SetInputConnection(smooth->GetOutputPort());
	skinNormals->SetFeatureAngle(180);

	vtkSmartPointer<vtkStripper> skinStripper =        //create triangle strips and/or poly-lines 为了更快的显示速度
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();    //这样不会带颜色

	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);

	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0, 0, 0);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	iren->SetInteractorStyle(style);

	/////////设置截面
	vtkSmartPointer<vtkClipPolyData> cliper = vtkClipPolyData::New();
	cliper->SetInputData(skinStripper->GetOutput());

	// 此平面box可以通过右键来进行放大缩小处理（只有当鼠标控制区域只有切割体才单一有效）
	vtkSmartPointer<vtkImplicitPlaneWidget> implicitPlaneWidget = vtkImplicitPlaneWidget::New();

	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor01 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor02 = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	implicitPlaneWidget->SetInteractor(iren);
	implicitPlaneWidget->SetPlaceFactor(1.25);
	//initially position the widget
	implicitPlaneWidget->SetInputData(skinStripper->GetOutput());
	implicitPlaneWidget->PlaceWidget();

	vtkSmartPointer<vtkActor> coneSkinActor = vtkActor::New();

	vtkSmartPointer<vtkTransform> trans =
		vtkSmartPointer<vtkTransform>::New();
	trans->PostMultiply();
	trans->Translate(1, 0, 0);
	trans->RotateX(90);
	coneSkinActor->SetUserTransform(trans);

	coneSkinActor->SetMapper(skinMapper);




	vtkSmartPointer<BuildVTKWidgetCall> pCall = BuildVTKWidgetCall::New();
	pCall->setActor(coneSkinActor);
	pCall->setCliper(cliper);

	///////
	implicitPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);
	implicitPlaneWidget->On();

	
	// renWin->SetFullScreen(1);
	


	ren[0]->SetBackground(0.3, 0.1, 0.1);
	ren[1]->SetBackground(0.1, 0.3, 0.1);
	ren[2]->SetBackground(0.1, 0.1, 0.3);
	ren[3]->AddActor(outline);
	ren[3]->AddActor(skin);
	//ren[2]->AddActor(coneSkinActor);
	ren[3]->SetBackground(0.2, 0.3, 0.4);
	//ren[3]->SetActiveCamera(aCamera);
	//ren[3]->ResetCamera();
	//ren[3]->SetBackground(.2, .3, .4);
	//ren[3]->ResetCameraClippingRange();
	renWin->SetSize(600, 600);

	//ui->qvtkWidget->SetRenderWindow(renWin);		// make the link between VTK and QT
	//ui->qvtkWidget_2->SetRenderWindow(renWin);
	//ui->qvtkWidget_3->SetRenderWindow(renWin);
	//ui->qvtkWidget_4->SetRenderWindow(renWin);
	//ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren[0]);
	//ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(ren[1]);
	//ui->qvtkWidget_3->GetRenderWindow()->AddRenderer(ren[2]);
	//ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(ren[3]);
	//iren->SetRenderWindow(renWin);
	//ui->qvtkWidget_4->show();
	ren[0]->SetViewport(0, 0, 0.5, 0.5);
	ren[1]->SetViewport(0.5, 0, 1, 0.5);
	ren[2]->SetViewport(0, 0.5, 0.5, 1);
	ren[3]->SetViewport(0.5, 0.5, 1, 1);

	// Set the actors' positions
	//
	renWin->Render();

	ren[3]->GetActiveCamera()->Elevation(110);
	ren[3]->GetActiveCamera()->SetViewUp(0, 0, -1);
	ren[3]->GetActiveCamera()->Azimuth(45);
	ren[3]->GetActiveCamera()->Dolly(1.15);
	ren[3]->ResetCameraClippingRange();

	//vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	//iren->SetInteractorStyle(style);

	iren->Initialize();

	iren->Start();

}
*/
/*
#include <vtkImageShiftScale.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
void MainWindow::test()
{

	vtkSmartPointer<vtkRenderer> pRender1 = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> pRender2 = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> pWin = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> pInt = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	pWin->SetSize(600, 300);
	pWin->AddRenderer(pRender1);
	pWin->AddRenderer(pRender2);
	pRender1->SetViewport(0, 0, .5, 1);
	pRender2->SetViewport(0.5, 0, 1, 1);
	pRender2->SetBackground(.3, .3, .3);
	pInt->SetRenderWindow(pWin);
	vtkSmartPointer<vtkDICOMImageReader>pReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	pReader->SetDirectoryName("D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx");
	pReader->SetDataByteOrderToLittleEndian();
	pReader->Update();
	double range[2];
	pReader->GetOutput()->GetScalarRange(range);//得到标量范围

	vtkSmartPointer<vtkImageShiftScale> m_pShift = vtkSmartPointer<vtkImageShiftScale>::New();//图像类型转换
	m_pShift->SetShift(-1.0*range[0]);//设置偏移量
	m_pShift->SetScale(255.0 / (range[1] - range[0]));//设置放缩值
	m_pShift->SetOutputScalarTypeToUnsignedChar();
	m_pShift->SetInputConnection(pReader->GetOutputPort());
	m_pShift->ReleaseDataFlagOff();
	m_pShift->Update();

	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>pMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	pMapper->SetInputConnection(pReader->GetOutputPort()); //映射:第一步

	vtkSmartPointer<vtkColorTransferFunction> pColor = vtkSmartPointer<vtkColorTransferFunction>::New(); //第二步:color
	pColor->AddRGBSegment(0, 1, 1, 1, 255, 1, 1, 1);

	vtkSmartPointer<vtkPiecewiseFunction> pPiceFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
	pPiceFun->AddSegment(0, 0, 3000, 1);//第三步: vtkPiecwise分段
	pMapper->SetBlendModeToMaximumIntensity();

	vtkSmartPointer<vtkVolumeProperty> pProperty = vtkSmartPointer<vtkVolumeProperty>::New();//设置属性
	pProperty->SetColor(pColor);
	pProperty->SetScalarOpacity(pPiceFun);
	pProperty->SetInterpolationTypeToLinear();
	pProperty->ShadeOff();

	vtkSmartPointer<vtkVolume>pVolume = vtkSmartPointer<vtkVolume>::New();
	pVolume->SetProperty(pProperty);
	pVolume->SetMapper(pMapper); //生成体对象
	pRender1->AddVolume(pVolume); //增加体数据到渲染器中渲染
	vtkCellPicker * picker = vtkCellPicker::New();
	picker->SetTolerance(0.005);

	vtkSmartPointer<vtkImagePlaneWidget >pWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
	pWidget->SetInteractor(pInt);
	pWidget->SetPicker(picker);
	pWidget->SetInputConnection(pReader->GetOutputPort());
	//pWidget->SetResolution(10);//设置分辨率
	pWidget->RestrictPlaneToVolumeOn();
	pWidget->GetPlaneProperty()->SetColor(.9, .4, .4);
	pWidget->TextureInterpolateOn();
	pWidget->SetWindowLevel(1358, -27);
	pWidget->SetDefaultRenderer(pRender1);
	pWidget->DisplayTextOn();
	pWidget->SetResliceInterpolateToLinear();

	pWidget->SetPlaneOrientationToZAxes();
	pWidget->SetSliceIndex(14);

	pWidget->On();
	pWidget->InteractionOn();
	vtkSmartPointer<vtkPlaneSource>plane =
		vtkSmartPointer<vtkPlaneSource>::New();
	vtkSmartPointer<vtkPolyDataMapper>planeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	planeMapper=plane->GetOutput();
	vtkSmartPointer<vtkActor>planeActor =
		vtkSmartPointer<vtkActor>::New();
	planeActor->SetMapper(planeMapper);
	planeActor->SetTexture(pWidget->GetTexture());

	pRender2->AddActor(planeActor);
	pWin->Render();
	pInt->Initialize();
	pInt->Start();
	//创建一个平面
	vtkSmartPointer<vtkPlaneSource>planeSource = vtkSmartPointer<vtkPlaneSource>::New();
	planeSource->SetCenter(1, 0, 0);
	planeSource->SetNormal(1, 1, 1);
	planeSource->Update();
	//planeSource会根据上面输入数据，创建PolyData对象，并通过GetOutput输出对象指针
	vtkSmartPointer<vtkPolyData> polydataPlane = vtkSmartPointer<vtkPolyData>::New();
	polydataPlane = planeSource->GetOutput();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polydataPlane);
	//创建演员
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	////文件读取的操作
	////vtkDICOMImageReader *pReader = vtkDICOMImageReader::New();
	////pReader->SetDataByteOrderToLittleEndian();
	////pReader->SetDirectoryName( "d:\\neck" );
	////pReader->Update();
	//std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
	//vtkSmartPointer<vtkDICOMImageReader> pReader =
	//	vtkSmartPointer<vtkDICOMImageReader>::New();
	//pReader->SetDirectoryName(folder.c_str());
	//pReader->Update();

	////vtkVolume16Reader *reader = vtkVolume16Reader::New();
	////reader->SetDataDimensions(512, 512);
	////reader->SetImageRange(0, 145);
	////reader->SetDataByteOrderToLittleEndian();
	////reader->SetFilePrefix("D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx\\Untitled-");
	////reader->SetFilePattern("%s%d");
	////reader->SetDataSpacing(0.25, 0.25, 1.0);

	///*vtkVolume16Reader* pReader = vtkVolume16Reader::New();
	//pReader->SetDataDimensions(64, 64);
	//pReader->SetDataByteOrderToLittleEndian();
	//pReader->SetImageRange(0, 145);
	//pReader->SetDataSpacing(3.2, 3.2, 1.5);
	//pReader->SetFilePrefix("D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx\\Untitled-");
	//pReader->SetFilePattern("%s%d");
	//pReader->SetDataMask(0x7fff);
	//pReader->Update();*/

	//vtkOutlineFilter *DicomOutline = vtkOutlineFilter::New();
	//DicomOutline->SetInputConnection(pReader->GetOutputPort());

	//vtkPolyDataMapper *pMapper = vtkPolyDataMapper::New();
	//pMapper->SetInputConnection(DicomOutline->GetOutputPort());

	//vtkActor *DicomActor = vtkActor::New();
	//DicomActor->SetMapper(pMapper);

	//vtkRenderer *ren = vtkRenderer::New();
	//vtkRenderer *ren1 = vtkRenderer::New();
	//vtkRenderer *ren2 = vtkRenderer::New();
	//vtkRenderer *ren3 = vtkRenderer::New();

	//vtkRenderWindow *renWin = vtkRenderWindow::New();
	//vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

	//renWin->AddRenderer(ren1);
	//renWin->AddRenderer(ren2);
	//renWin->AddRenderer(ren3);
	//renWin->AddRenderer(ren);
	//iren->SetRenderWindow(renWin);

	//vtkCellPicker *picker = vtkCellPicker::New();
	//picker->SetTolerance(0.005);

	//vtkImagePlaneWidget *planeWidgetX = vtkImagePlaneWidget::New();
	//planeWidgetX->SetInteractor(iren);
	//planeWidgetX->SetKeyPressActivationValue('x');
	//planeWidgetX->SetPicker(picker);
	//planeWidgetX->RestrictPlaneToVolumeOn();
	//planeWidgetX->GetPlaneProperty()->SetColor(0.0, 0.0, 1.0);
	//planeWidgetX->DisplayTextOn();
	////planeWidgetX->TextureInterpolateOn();
	//planeWidgetX->TextureInterpolateOff();
	//planeWidgetX->SetResliceInterpolateToLinear();
	//planeWidgetX->SetInputData((vtkDataSet*)pReader->GetOutput());
	//planeWidgetX->SetPlaneOrientationToXAxes();//冠
	//planeWidgetX->SetSliceIndex(32);
	//planeWidgetX->GetTexturePlaneProperty()->SetOpacity(1);
	//planeWidgetX->On();

	//vtkImagePlaneWidget *planeWidgetY = vtkImagePlaneWidget::New();
	//planeWidgetY->SetInteractor(iren);
	//planeWidgetY->SetKeyPressActivationValue('y');
	//planeWidgetY->SetPicker(picker);
	//planeWidgetY->RestrictPlaneToVolumeOn();
	//planeWidgetY->GetPlaneProperty()->SetColor(1.0, 0.0, 0.0);
	//planeWidgetY->DisplayTextOn();
	//planeWidgetY->TextureInterpolateOn();
	//planeWidgetY->SetResliceInterpolateToLinear();
	//planeWidgetY->SetInputData((vtkDataSet*)pReader->GetOutput());
	//planeWidgetY->SetPlaneOrientationToYAxes();//矢
	//planeWidgetY->SetSliceIndex(25);
	//planeWidgetY->On();

	//vtkImagePlaneWidget *planeWidgetZ = vtkImagePlaneWidget::New();
	//planeWidgetZ->SetInteractor(iren);
	//planeWidgetZ->DisplayTextOn();
	//planeWidgetZ->RestrictPlaneToVolumeOn();
	//planeWidgetZ->SetKeyPressActivationValue('z');
	//planeWidgetZ->SetPicker(picker);
	//planeWidgetZ->GetPlaneProperty()->SetColor(1.0, 1.0, 0.0);
	//planeWidgetZ->TextureInterpolateOn();
	//planeWidgetZ->SetResliceInterpolateToLinear();
	//planeWidgetZ->SetInputData((vtkDataSet*)pReader->GetOutput());
	//planeWidgetZ->SetPlaneOrientationToZAxes(); //横断面
	//planeWidgetZ->SetSliceIndex(50);
	//planeWidgetZ->On();

	////////////////////////////////////////////////////////
	//vtkWidgetWindowLevelCallback *cbk = vtkWidgetWindowLevelCallback::New();
	//cbk->WidgetX = planeWidgetX;
	//cbk->WidgetY = planeWidgetY;
	//cbk->WidgetZ = planeWidgetZ;
	//planeWidgetX->AddObserver(vtkCommand::EndWindowLevelEvent, cbk);
	//planeWidgetY->AddObserver(vtkCommand::EndWindowLevelEvent, cbk);
	//planeWidgetZ->AddObserver(vtkCommand::EndWindowLevelEvent, cbk);
	//cbk->Delete();
	////////////////////////////////////////////////////////
	//vtkSliderRepresentation2D *sliderRep1 = vtkSliderRepresentation2D::New();
	//sliderRep1->SetMinimumValue(1);
	//sliderRep1->SetMaximumValue(50);
	//sliderRep1->SetValue(planeWidgetX->GetSliceIndex());
	//sliderRep1->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
	//sliderRep1->GetPoint1Coordinate()->SetValue(.0, .02);
	//sliderRep1->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
	//sliderRep1->GetPoint2Coordinate()->SetValue(1.0, .02);

	//vtkSliderWidget *sliderWidget1 = vtkSliderWidget::New();
	//sliderWidget1->SetInteractor(iren);
	//sliderWidget1->SetRepresentation(sliderRep1);
	//sliderWidget1->SetAnimationModeToAnimate();
	//sliderWidget1->EnabledOn();

	//vtkSliderCallback  *slidercbk = vtkSliderCallback::New();
	//slidercbk->WidgetX = planeWidgetX;
	//sliderWidget1->AddObserver(vtkCommand::InteractionEvent, slidercbk);
	////////////////////////////////////////////////////////
	//	//增加2D图像测试获取切片
	//vtkImageMapToColors *colorMap1 = vtkImageMapToColors::New();
	//colorMap1->PassAlphaToOutputOff(); //use in RGBA
	//colorMap1->SetActiveComponent(0);
	//colorMap1->SetOutputFormatToLuminance();
	//colorMap1->SetInputData((vtkDataSet*)planeWidgetX->GetResliceOutput());
	//colorMap1->SetLookupTable((vtkScalarsToColors *)planeWidgetX->GetLookupTable());
	//vtkImageActor *imageActor1 = vtkImageActor::New();
	//imageActor1->PickableOff();
	//imageActor1->SetInputData(planeWidgetY->GetResliceOutput());

	//vtkImageMapToColors *colorMap2 = vtkImageMapToColors::New();
	//colorMap2->PassAlphaToOutputOff();
	//colorMap2->SetActiveComponent(0); // for muti-component
	//colorMap2->SetOutputFormatToLuminance();
	//colorMap2->SetInputData((vtkDataSet*)planeWidgetY->GetResliceOutput());
	//colorMap2->SetLookupTable((vtkScalarsToColors *)planeWidgetX->GetLookupTable());
	//vtkImageActor *imageActor2 = vtkImageActor::New();
	//imageActor2->PickableOff();
	//imageActor2->SetInputData(colorMap2->GetOutput());

	//vtkImageMapToColors *colorMap3 = vtkImageMapToColors::New();
	//colorMap3->PassAlphaToOutputOff();
	//colorMap3->SetActiveComponent(0);
	//colorMap3->SetOutputFormatToLuminance();
	//colorMap3->SetInputData((vtkDataSet*)planeWidgetZ->GetResliceOutput());
	//colorMap3->SetLookupTable((vtkScalarsToColors *)planeWidgetX->GetLookupTable());
	//vtkImageActor *imageActor3 = vtkImageActor::New();
	//imageActor3->PickableOff();
	//imageActor3->SetInputData(colorMap3->GetOutput());
	///////////////////////////////////////////////////////////////////////
	//vtkSmartPointer<vtkTransform> transform =
	//	vtkSmartPointer<vtkTransform>::New();
	//transform->Translate(-10.0, -10.0, -10.0);

	//vtkSmartPointer<vtkAxesActor> axes =
	//	vtkSmartPointer<vtkAxesActor>::New();

	//axes->SetUserTransform(transform);

	//ren->AddActor(axes);
	////////////////////////////////////////////////////////

	//ren->AddActor(DicomActor); //outline
	//ren1->AddActor(imageActor1);
	//ren2->AddActor(imageActor2);
	//ren3->AddActor(imageActor3);

	//// OK
	//ren->SetBackground(0.1, 0.1, 0.2);
	//ren1->SetBackground(0.1, 0.2, 0.1);
	//ren2->SetBackground(0.2, 0.1, 0.2);
	//ren3->SetBackground(0.2, 0.2, 0.2);

	//renWin->SetSize(600, 400);
	////renWin->SetFullScreen(1);     

	//ren3->SetViewport(0, 0.5, 0.5, 1);
	//ren1->SetViewport(0.5, 0.5, 1, 1);
	//ren2->SetViewport(0, 0, 0.5, 0.5);
	//ren->SetViewport(0.5, 0, 1, 0.5);

	//ren->GetActiveCamera()->Dolly(1.15);
	//ren1->GetActiveCamera()->Dolly(1.15);
	//ren2->GetActiveCamera()->Dolly(1.15);
	//ren3->GetActiveCamera()->Dolly(1.15);

	//ren->ResetCamera();
	//ren1->ResetCamera();
	//ren2->ResetCamera();
	//ren3->ResetCamera();

	//iren->Initialize();
	//renWin->SetWindowName("vtkSuperViewer by winston");
	//iren->Start();
	//renWin->Render();
//}
	////对病人的皮肤产生法线
	//vtkContourFilter *skinExtractor = vtkContourFilter::New();
	//skinExtractor->SetInputConnection(v16->GetOutputPort());
	//skinExtractor->SetValue(0, 500);
	//vtkPolyDataNormals *skinNormals = vtkPolyDataNormals::New();
	//skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
	//skinNormals->SetFeatureAngle(60.0);
	//vtkPolyDataMapper *skinMapper = vtkPolyDataMapper::New();
	//skinMapper->SetInputConnection(skinNormals->GetOutputPort());
	//skinMapper->ScalarVisibilityOff();
	//vtkActor *skin = vtkActor::New();
	//skin->SetMapper(skinMapper);
	//skin->GetProperty()->SetDiffuseColor(1, 0.49, 0.25);
	//skin->GetProperty()->SetSpecular(0.3);
	//skin->GetProperty()->SetSpecularPower(20);
	//skin->GetProperty()->SetOpacity(0.5);//将皮肤设为半透明为0.5

	////抽取骨头
	//vtkContourFilter *boneExtractor = vtkContourFilter::New();
	//boneExtractor->SetInputConnection(v16->GetOutputPort());
	//boneExtractor->SetValue(0, 1150);
	//vtkPolyDataNormals *boneNormals = vtkPolyDataNormals::New();
	//boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
	//boneNormals->SetFeatureAngle(60.0);
	//vtkStripper *boneStripper = vtkStripper::New();
	//boneStripper->SetInputConnection(boneNormals->GetOutputPort());
	//vtkPolyDataMapper *boneMapper = vtkPolyDataMapper::New();
	//boneMapper->SetInputConnection(boneStripper->GetOutputPort());
	//boneMapper->ScalarVisibilityOff();
	//vtkActor *bone = vtkActor::New();
	//bone->SetMapper(boneMapper);
	//bone->GetProperty()->SetDiffuseColor(1, 1, 0.9412);

	////产生数据框架
	//vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
	//outlineData->SetInputConnection(v16->GetOutputPort());
	//vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
	//mapOutline->SetInputConnection(outlineData->GetOutputPort());
	//vtkActor *outline = vtkActor::New();
	//outline->SetMapper(mapOutline);
	//outline->GetProperty()->SetColor(0, 0, 0);

	////灰度查询表
	//vtkLookupTable *bwLut = vtkLookupTable::New();
	//bwLut->SetTableRange(0, 2000);
	//bwLut->SetSaturationRange(0, 0);
	//bwLut->SetHueRange(0, 0);
	//bwLut->SetValueRange(0, 1);
	//bwLut->Build();

	//////色调查询表
	//vtkLookupTable *hueLut = vtkLookupTable::New();
	//hueLut->SetTableRange(0, 2000);
	//hueLut->SetSaturationRange(0, 1);
	//hueLut->SetHueRange(1, 1);
	//hueLut->SetValueRange(1, 1);
	//hueLut->Build();

	//////饱和度查询表
	//vtkLookupTable *satLut = vtkLookupTable::New();
	//satLut->SetTableRange(0, 2000);
	//satLut->SetSaturationRange(0.6, 0.6);
	//satLut->SetHueRange(0, 1);
	//satLut->SetValueRange(1, 1);
	//satLut->Build();

	//////创建第一个平面
	//vtkImageMapToColors *saggitalColors = vtkImageMapToColors::New();
	//saggitalColors->SetInputConnection(v16->GetOutputPort());
	//saggitalColors->SetLookupTable(bwLut);
	//vtkImageActor *saggital = vtkImageActor::New();
	//saggital->SetInputData(saggitalColors->GetOutput());
	//saggital->SetDisplayExtent(32, 32, 0, 63, 0, 92);

	//////创建第二个平面
	//vtkImageMapToColors *axialColors = vtkImageMapToColors::New();
	//axialColors->SetInputConnection(v16->GetOutputPort());
	//axialColors->SetLookupTable(hueLut);
	//vtkImageActor *axial = vtkImageActor::New();
	//axial->SetInputData(axialColors->GetOutput());
	//axial->SetDisplayExtent(0, 63, 0, 63, 46, 46);

	//////创建第三个平面
	//vtkImageMapToColors *coronalColors = vtkImageMapToColors::New();
	//coronalColors->SetInputConnection(v16->GetOutputPort());
	//coronalColors->SetLookupTable(satLut);
	//vtkImageActor *coronal = vtkImageActor::New();
	//coronal->SetInputData(coronalColors->GetOutput());
	//coronal->SetDisplayExtent(0, 63, 0, 0, 0, 92);

	////设置相机
	//vtkCamera *aCamera = vtkCamera::New();
	//aCamera->SetViewUp(0, 0, -1);
	//aCamera->SetPosition(0, 1, 0);
	//aCamera->SetFocalPoint(0, 0, 0);
	//aCamera->ComputeViewPlaneNormal();

	//ren1->AddActor(outline);
	////ren1->AddActor(skin);
	//ren1->AddActor(bone);
	//ren1->AddActor(saggital);
	//ren1->AddActor(axial);
	//ren1->AddActor(coronal);

	////vtkRenderWindow*renWin = vtkRenderWindow::New();
	//renWin->AddRenderer(ren1);
	////renWin->AddRenderer(ren2);
	//renWin->SetSize(600, 300);

	////vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();//设定交互
	//iren->SetRenderWindow(renWin);
	//vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	//iren->SetInteractorStyle(style);//指定交互方式

	////ui->qvtkWidget_4->SetRenderWindow(renWin);		// make the link between VTK and QT
	////ui->qvtkWidget_4->GetRenderWindow()->AddRenderer(ren1);
	//// make the link between the  renderwindow and the interactor
	//iren->SetRenderWindow(renWin);
	////ui->qvtkWidget_4->show();

	//// interact with data
	//renWin->Render();

	////关闭骨头的演示
	////bone->VisibilityOff();

	//ren1->SetActiveCamera(aCamera);
	//ren1->ResetCamera();
	//aCamera->Dolly(1.5);
	//ren1->SetBackground(1, 1, 1);

	//ren1->ResetCameraClippingRange();//重设相机的剪切范围

	//iren->Initialize();
	//iren->Start();
//}
	//v16->Delete();
	//saggital->Delete();
	//bwLut->Delete();
	//saggitalColors->Delete();


	/*vtkMetaImageReader *reader = vtkMetaImageReader::New();
	reader->SetFileName(fname);
	reader->Update();*/
	/*
	vtkVolume16Reader *v16 = vtkVolume16Reader::New();
	v16->SetDataDimensions(64,64);
	v16->SetImageRange(1,93);
	v16->SetDataByteOrderToLittleEndian();
	v16->SetFilePrefix(fname);
	v16->SetDataSpacing(3.2,3.2,1.5);
	imageViewer = vtkImageViewer2::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	imageViewer->SetupInteractor(iren);
	imageViewer->SetColorLevel(1000);
	imageViewer->SetColorWindow(2000);
	imageViewer->SetSlice(idx);
	imageViewer->SetSliceOrientationToXY();

	
	this->GetRenderWindow()->AddRenderer(imageViewer->GetRenderer());

	imageViewer->Render();




	std::string folder = "D:\\projectC#\\ActivizNew\\DebugLibrary\\bin\\Debug\\ncx";
	// Setup window
	vtkSmartPointer<vtkRenderWindow> renderWindow =
	vtkSmartPointer<vtkRenderWindow>::New();
	// Setup renderer
	vtkSmartPointer<vtkRenderer> ren1 =
	vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(ren1);

	vtkSmartPointer<vtkDICOMImageReader> reader =
	vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();

	auto windowLevel = vtkSmartPointer<vtkImageViewer2>::New();
	windowLevel->SetInputConnection(reader->GetOutputPort());

	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	windowLevel->SetupInteractor(interactor);
	windowLevel->SetSize(400, 400);
	windowLevel->SetColorLevel(500);
	windowLevel->SetColorWindow(1000);
	windowLevel->SetSliceOrientationToXY(); // z-axis
	windowLevel->GetRenderer()->SetBackground(1, 1, 1);
	auto myStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
	myStyle->SetImageViewer(windowLevel);
	interactor->SetInteractorStyle(myStyle);






	vtkSmartPointer<vtkImageViewer2> windowLevel =
		vtkSmartPointer<vtkImageViewer2>::New();
	//imageViewer->SetInputConnection(reader->GetOutputPort());
	//vtkImageMapToWindowLevelColors *windowLevel = vtkImageMapToWindowLevelColors::New();
	windowLevel->SetInputConnection(reader->GetOutputPort());
	//windowLevel->SetLevel(1000);
	//windowLevel->SetWindow(2000);
	vtkImageActor *ImageActor = vtkImageActor::New();
	ren1->AddViewProp(ImageActor);
	ImageActor->SetInputData(windowLevel->GetOutput());

	int Slice = 100;
	int sliceOrientation = vtkImageViewer2::SLICE_ORIENTATION_XY;

	int *w_ext = windowLevel->GetInput()->GetWholeExtent();
	int slice_min = w_ext[sliceOrientation * 2];
	int slice_max = w_ext[sliceOrientation * 2 + 1];
	if (Slice < slice_min || Slice > slice_max)
	{
		Slice = static_cast<int>((slice_min + slice_max) * 0.5);
	}


	vtkCamera *cam = ren1->GetActiveCamera();
	int xs = 0, ys = 0;
	switch (sliceOrientation)
	{
	case vtkImageViewer2::SLICE_ORIENTATION_XY:
	default:
		xs = w_ext[1] - w_ext[0] + 1;
		ys = w_ext[3] - w_ext[2] + 1;

		cam->SetFocalPoint(0, 0, 0);
		cam->SetPosition(0, 0, 1); // -1 if medical ?
		cam->SetViewUp(0, 1, 0);

		ImageActor->SetDisplayExtent(
			w_ext[0], w_ext[1], w_ext[2], w_ext[3], Slice, Slice);
		break;

	case vtkImageViewer2::SLICE_ORIENTATION_XZ:
		xs = w_ext[1] - w_ext[0] + 1;
		ys = w_ext[5] - w_ext[4] + 1;

		cam->SetFocalPoint(0, 0, 0);
		cam->SetPosition(0, -1, 0); // 1 if medical ?
		cam->SetViewUp(0, 0, 1);

		ImageActor->SetDisplayExtent(
			w_ext[0], w_ext[1], Slice, Slice, w_ext[4], w_ext[5]);
		break;

	case vtkImageViewer2::SLICE_ORIENTATION_YZ:
		xs = w_ext[3] - w_ext[2] + 1;
		ys = w_ext[5] - w_ext[4] + 1;

		cam->SetFocalPoint(0, 0, 0);
		cam->SetPosition(1, 0, 0); // -1 if medical ?
		cam->SetViewUp(0, 0, 1);

		ImageActor->SetDisplayExtent(
			Slice, Slice, w_ext[2], w_ext[3], w_ext[4], w_ext[5]);
		break;
	}

	xs = w_ext[1] - w_ext[0] + 1;
	ys = w_ext[3] - w_ext[2] + 1;

	if (this->GetRenderWindow()->GetSize()[0] == 0)
	{
		this->GetRenderWindow()->SetSize(
			xs < 150 ? 150 : xs, ys < 100 ? 100 : ys);
	}

	if (this->ren1)
	{
		this->ren1->ResetCamera();
		this->ren1->GetActiveCamera()->SetParallelScale(
			xs < 150 ? 75 : (xs - 1) / 2.0);
	}
	this->FirstRender = 0;
	*/

//}
