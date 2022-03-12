using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using DevExpress.XtraEditors;
using Kitware.VTK;
using Entity;
using System.Diagnostics;
using System.Threading;

namespace SmartRayND
{
    public partial class Form1 : DevExpress.XtraBars.Ribbon.RibbonForm
    {
        #region 影像原片（三向）控制与窗口
        private RenderWindowControl AxialCtrl;
        private RenderWindowControl SaggitalCtrl;
        private RenderWindowControl CoronalCtrl;
        private vtkRenderWindow AxialWin;
        private vtkRenderWindow SaggitalWin;
        private vtkRenderWindow CoronalWin;
        #endregion
        private vtkDICOMImageReader reader; //dicom文件读取器

        bool rulerBegin = false;            //原版上标尺
        vtkDistanceWidget vdw;              //标尺工具

        public Form1()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 初始化原片control，原片窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_Load(object sender, EventArgs e)
        {
            AxialCtrl = new RenderWindowControl();
            SaggitalCtrl = new RenderWindowControl();
            CoronalCtrl = new RenderWindowControl();

            InitializeCtrl(AxialCtrl, AxialPanel, Color.Black);
            InitializeCtrl(SaggitalCtrl, SaggitalPanel, Color.Black);
            InitializeCtrl(CoronalCtrl, CoronalPanel, Color.Black);

            groupControl1.Visible = false;
            splitContainer1.Visible = false;
        }

        /// <summary>
        /// 初始化原片control
        ///     设置parent
        ///         设置背景色
        ///             设置dock
        /// </summary>
        /// <param name="ctrl"></param>
        /// <param name="parent"></param>
        /// <param name="ctrlBack"></param>
        private void InitializeCtrl(RenderWindowControl ctrl, Control parent, Color ctrlBack)
        {
            ctrl.Parent = parent;
            ctrl.BackColor = ctrlBack;
            ctrl.Dock = DockStyle.Fill;
        }

        /// <summary>
        /// 读取影像
        /// </summary>
        /// <param name="folder"></param>
        private void loadPacs(string folder)
        {
            clearAndInit();
            Form1_Load(null, null);
            //LoadBtn.Visible = false;
            groupControl1.Visible = true;
            splitContainer1.Visible = true;
            ribbonControl1.Minimized = true;
            //初始化reader（固定用法）
            reader = vtkDICOMImageReader.New();
            reader.SetDirectoryName(folder);        //设置影像文件夹
            reader.Update();                        //固定用法
            Load2DAxialSource(AxialCtrl, reader);   //读取显示原片，以下两行也是，三向原片
            Load2DCoronalSource(CoronalCtrl, reader);
            Load2DSaggitalSource(SaggitalCtrl, reader);

            LoadProperties();                       //预读取（设置）三维影像的属性参数，并生成影像
        }

        #region 2D
        //三向原片加载
        #region Axial

        private vtkImageViewer2 _AxialViewer;
        private vtkTextMapper _AxialSliceStatusMapper;
        private int _AxialSlice;
        private int _MinAxial;
        private int _MaxAxial;

        private void Load2DAxialSource(RenderWindowControl ctrl, vtkDICOMImageReader reader)
        {
            if (ctrl == null || reader == null)
            {
                return;
            }
            //以下是固定用法
            _AxialViewer = vtkImageViewer2.New();
            _AxialViewer.SetInputConnection(reader.GetOutputPort());
            _AxialViewer.GetSliceRange(ref _MinAxial, ref _MaxAxial);
            vtkTextProperty sliceTextProp = vtkTextProperty.New();
            sliceTextProp.SetFontFamilyToCourier();
            sliceTextProp.SetFontSize(20);
            sliceTextProp.SetVerticalJustificationToBottom();
            sliceTextProp.SetJustificationToLeft();

            AxialBar.Properties.Minimum = _MinAxial;
            AxialBar.Properties.Maximum = _MaxAxial;

            _AxialSliceStatusMapper = vtkTextMapper.New();
            _AxialSliceStatusMapper.SetTextProperty(sliceTextProp);

            vtkActor2D sliceStatusActor = vtkActor2D.New();
            sliceStatusActor.SetMapper(_AxialSliceStatusMapper);
            sliceStatusActor.SetPosition(15, 10);

            AxialWin = ctrl.RenderWindow;
            AxialWin.GetRenderers().InitTraversal();
            vtkRenderer ren;
            while ((ren = AxialWin.GetRenderers().GetNextItem()) != null)
            {
                ren.SetBackground(0.0, 0.0, 0.0);
            }

            _AxialViewer.SetRenderWindow(AxialWin);
            _AxialViewer.GetRenderer().AddActor2D(sliceStatusActor);
            int halfSlice = (_MaxAxial + _MinAxial) / 2;
            AxialBar.Value = halfSlice;
            _AxialSlice = halfSlice;
            _AxialViewer.SetSlice(halfSlice);
            _AxialSliceStatusMapper.SetInput("Slice No" + (halfSlice + 1).ToString() + "/" + (_MaxAxial + 1).ToString());

            _AxialViewer.SetupInteractor(AxialWin.GetInteractor());
            AxialWin.GetInteractor().LeftButtonReleaseEvt += InteractorStyle_LeftButtonPressEvt;
            AxialWin.GetInteractor().MouseMoveEvt += Axial_MouseMoveEvt;
            _AxialViewer.Render();
            AxialBar.ValueChanged += AxialBar_EditValueChanged;
        }

        private void Axial_MouseMoveEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            if (rulerBegin)
            {
                if (vdw.GetWidgetState() == 0)
                {
                    vdw.Off();
                    vdw.SetInteractor(AxialWin.GetInteractor());
                    vdw.On();
                }
            }
            if (vdw != null)
                Console.WriteLine(vdw.GetWidgetState());
        }

        private void Saggital_MouseMoveEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            if (rulerBegin)
            {
                if (vdw.GetWidgetState() == 0)
                {
                    vdw.Off();
                    vdw.SetInteractor(SaggitalWin.GetInteractor());
                    vdw.On();
                }
            }
            if (vdw != null)
                Console.WriteLine(vdw.GetWidgetState());
        }

        private void Coronal_MouseMoveEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            if (rulerBegin)
            {
                if (vdw.GetWidgetState() == 0)
                {
                    vdw.Off();
                    vdw.SetInteractor(CoronalWin.GetInteractor());
                    vdw.On();
                }
            }
            if (vdw != null)
                Console.WriteLine(vdw.GetWidgetState());
        }

        private void InteractorStyle_LeftButtonPressEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            int[] p = AxialWin.GetInteractor().GetLastEventPosition();
            Console.WriteLine(p[0] + ":" + p[1]);
            if (rulerBegin)
            {
                rulerBegin = false;
            }
            Console.WriteLine(rulerBegin.ToString());
        }

        #endregion

        #region Saggital

        private vtkImageViewer2 _SaggitalViewer;
        private vtkTextMapper _SaggitalSliceStatusMapper;
        private int _SaggitalSlice;
        private int _MinSaggital;
        private int _MaxSaggital;
        private void Load2DSaggitalSource(RenderWindowControl ctrl, vtkDICOMImageReader reader)
        {
            SaggitalBar.Visible = true;

            _SaggitalViewer = vtkImageViewer2.New();

            var bounds = reader.GetOutput().GetBounds();
            double[] center = { (bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, (bounds[5] + bounds[4]) / 2.0 };

            vtkTransform trans = vtkTransform.New();
            trans.Translate(center[0], center[1], center[2]);
            trans.RotateX(180);
            trans.Translate(-center[0], -center[1], -center[2]);

            vtkImageReslice vir = vtkImageReslice.New();
            vir.SetInputConnection(reader.GetOutputPort());
            vir.SetResliceTransform(trans);
            vir.SetInterpolationModeToCubic();
            vir.SetOutputSpacing(reader.GetOutput().GetSpacing()[0],
                reader.GetOutput().GetSpacing()[1],
                reader.GetOutput().GetSpacing()[2]);
            vir.SetOutputOrigin(reader.GetOutput().GetOrigin()[0],
                reader.GetOutput().GetOrigin()[1],
                reader.GetOutput().GetOrigin()[2]);
            var a = reader.GetOutput().GetExtent();
            vir.SetOutputExtent(a[0], a[1], a[2], a[3], a[4], a[5]);

            _SaggitalViewer.SetInputConnection(vir.GetOutputPort());
            _SaggitalViewer.SetSliceOrientationToYZ();

            _SaggitalViewer.GetSliceRange(ref _MinSaggital, ref _MaxSaggital);
            SaggitalBar.Properties.Minimum = _MinSaggital;
            SaggitalBar.Properties.Maximum = _MaxSaggital;
            int halfSlice = (_MinSaggital + _MaxSaggital) / 2;
            SaggitalBar.Value = halfSlice;
            _SaggitalSlice = halfSlice;
            _SaggitalViewer.SetSlice(halfSlice);

            vtkTextProperty sliceTextProp = vtkTextProperty.New();
            sliceTextProp.SetFontFamilyToCourier();
            sliceTextProp.SetFontSize(20);
            sliceTextProp.SetVerticalJustificationToBottom();
            sliceTextProp.SetJustificationToLeft();
            _SaggitalSliceStatusMapper = vtkTextMapper.New();
            _SaggitalSliceStatusMapper.SetInput("Slice No" + (halfSlice + 1).ToString() + "/" + (_MaxSaggital + 1).ToString());
            _SaggitalSliceStatusMapper.SetTextProperty(sliceTextProp);
            vtkActor2D sliceStatusActor = vtkActor2D.New();
            sliceStatusActor.SetMapper(_SaggitalSliceStatusMapper);
            sliceStatusActor.SetPosition(15, 10);
            _SaggitalViewer.GetRenderer().AddActor2D(sliceStatusActor);

            SaggitalWin = ctrl.RenderWindow;
            SaggitalWin.GetRenderers().InitTraversal();
            vtkRenderer ren;
            while ((ren = SaggitalWin.GetRenderers().GetNextItem()) != null)
            {
                ren.SetBackground(0.0, 0.0, 0.0);
            }
            _SaggitalViewer.SetRenderWindow(SaggitalWin);

            _SaggitalViewer.SetupInteractor(SaggitalWin.GetInteractor());

            SaggitalWin.GetInteractor().LeftButtonReleaseEvt += InteractorStyle_LeftButtonPressEvt;
            SaggitalWin.GetInteractor().MouseMoveEvt += Saggital_MouseMoveEvt;
            _SaggitalViewer.Render();
            SaggitalBar.ValueChanged += SaggitalBar_EditValueChanged;
        }

        #endregion

        #region Coronal
        private vtkImageViewer2 _CoronalViewer;
        private vtkTextMapper _CoronalSliceStatusMapper;
        private int _CoronalSlice;
        private int _MinCoronal;
        private int _MaxCoronal;

        private void Load2DCoronalSource(RenderWindowControl ctrl, vtkDICOMImageReader reader)
        {
            CoronalBar.Visible = true;
            _CoronalViewer = vtkImageViewer2.New();

            var bounds = reader.GetOutput().GetBounds();
            double[] center = { (bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, (bounds[5] + bounds[4]) / 2.0 };

            vtkTransform trans = vtkTransform.New();
            trans.Translate(center[0], center[1], center[2]);
            trans.RotateY(180);
            trans.Translate(-center[0], -center[1], -center[2]);

            vtkImageReslice vir = vtkImageReslice.New();
            vir.SetInputConnection(reader.GetOutputPort());
            vir.SetResliceTransform(trans);
            vir.SetInterpolationModeToCubic();
            vir.SetOutputSpacing(reader.GetOutput().GetSpacing()[0],
                reader.GetOutput().GetSpacing()[1],
                reader.GetOutput().GetSpacing()[2]);
            vir.SetOutputOrigin(reader.GetOutput().GetOrigin()[0],
                reader.GetOutput().GetOrigin()[1],
                reader.GetOutput().GetOrigin()[2]);
            var a = reader.GetOutput().GetExtent();
            vir.SetOutputExtent(a[0], a[1], a[2], a[3], a[4], a[5]);

            _CoronalViewer.SetInputConnection(vir.GetOutputPort());
            _CoronalViewer.SetSliceOrientationToXZ();
            _CoronalViewer.GetSliceRange(ref _MinCoronal, ref _MaxCoronal);

            CoronalBar.Properties.Minimum = _MinCoronal;
            CoronalBar.Properties.Maximum = _MaxCoronal;
            int halfSlice = (_MinCoronal + _MaxCoronal) / 2;
            CoronalBar.Value = halfSlice;
            _CoronalSlice = halfSlice;
            _CoronalViewer.SetSlice(halfSlice);

            vtkTextProperty sliceTextProp = vtkTextProperty.New();
            sliceTextProp.SetFontFamilyToCourier();
            sliceTextProp.SetFontSize(20);
            sliceTextProp.SetVerticalJustificationToBottom();
            sliceTextProp.SetJustificationToLeft();
            _CoronalSliceStatusMapper = vtkTextMapper.New();
            _CoronalSliceStatusMapper.SetInput("Slice No" + (halfSlice + 1).ToString() + "/" + (_MaxCoronal + 1).ToString());
            _CoronalSliceStatusMapper.SetTextProperty(sliceTextProp);
            vtkActor2D sliceStatusActor = vtkActor2D.New();
            sliceStatusActor.SetMapper(_CoronalSliceStatusMapper);
            sliceStatusActor.SetPosition(15, 10);
            _CoronalViewer.GetRenderer().AddActor2D(sliceStatusActor);

            CoronalWin = ctrl.RenderWindow;
            CoronalWin.GetRenderers().InitTraversal();

            vtkRenderer ren;
            while ((ren = CoronalWin.GetRenderers().GetNextItem()) != null)
            {
                ren.SetBackground(0.0, 0.0, 0.0);
            }

            _CoronalViewer.SetRenderWindow(CoronalWin);
            _CoronalViewer.Render();
            _CoronalViewer.GetRenderer().ResetCamera();

            _CoronalViewer.SetupInteractor(CoronalWin.GetInteractor());

            CoronalWin.GetInteractor().LeftButtonReleaseEvt += InteractorStyle_LeftButtonPressEvt;
            CoronalWin.GetInteractor().MouseMoveEvt += Coronal_MouseMoveEvt;
            _CoronalViewer.Render();
            CoronalBar.ValueChanged += CoronalBar_EditValueChanged;
        }
        #endregion

        private void AxialBar_EditValueChanged(object sender, EventArgs e)
        {
            if (AxialBar.Value >= _MinAxial && AxialBar.Value <= _MaxAxial)
            {
                _AxialSlice = AxialBar.Value;
                _AxialViewer.SetSlice(_AxialSlice);
                _AxialSliceStatusMapper.SetInput("Slice No" + (_AxialSlice + 1).ToString() + "/" + (_MaxAxial + 1).ToString());
                _AxialViewer.Render();
            }
        }

        private void SaggitalBar_EditValueChanged(object sender, EventArgs e)
        {
            if (SaggitalBar.Value >= _MinSaggital && SaggitalBar.Value <= _MaxSaggital)
            {
                _SaggitalSlice = SaggitalBar.Value;
                _SaggitalViewer.SetSlice(_SaggitalSlice);
                _SaggitalSliceStatusMapper.SetInput("Slice No" + (_SaggitalSlice + 1).ToString() + "/" + (_MaxSaggital + 1).ToString());
                _SaggitalViewer.Render();
            }
        }

        private void CoronalBar_EditValueChanged(object sender, EventArgs e)
        {
            if (CoronalBar.Value >= _MinCoronal && CoronalBar.Value <= _MaxCoronal)
            {
                _CoronalSlice = CoronalBar.Value;
                _CoronalViewer.SetSlice(_CoronalSlice);
                _CoronalSliceStatusMapper.SetInput("Slice No" + (_CoronalSlice + 1).ToString() + "/" + (_MaxCoronal + 1).ToString());
                _CoronalViewer.Render();
            }
        }

        #endregion

        #region 3D

        private PropertyDB pdb;                 //三维影像参数

        /// <summary>
        /// 预读取（设置）三维影像的属性参数，及生成影像
        /// </summary>
        private void LoadProperties()
        {
            pdb = new PropertyDB();
            foreach (var item in pdb.PropDict.Keys)
            {
                ComboPresets.Items.Add(item);
            }

            load3DModel();      //生成影像
        }

        private RenderWindowControl ModelCtrl;      //三维影像control
        private vtkVolume volume;                   //三维影像volume
        private vtkVolumeProperty volumeProperty;   //三维影像属性
        private vtkSmartVolumeMapper volumeMapper;  //三维影像mapper
        private vtkRenderer leftRen;                //三维影像renderer，渲染器
        private vtkRenderWindow ModelWin;           //三维影像窗口

        MouseEvents me;                             //鼠标事件

        /// <summary>
        /// 读取三维影像
        /// </summary>
        private void load3DModel()
        {
            //以下基本是固定用法
            ModelCtrl = new RenderWindowControl();      //初始化control
            ModelCtrl.Parent = splitContainer1.Panel1;  //设置parent
            ModelCtrl.BackColor = Color.Black;          //设置背景色
            ModelCtrl.Dock = DockStyle.Fill;            //设置dock

            volumeProperty = vtkVolumeProperty.New();   //初始化property
            volumeMapper = vtkSmartVolumeMapper.New();  //初始化mapper
            volume = vtkVolume.New();                   //初始化volume

            ModelWin = ModelCtrl.RenderWindow;          //固定用法，获取ModelWin
            vtkImageData imageData = vtkImageData.New();//初始化vtkImageData
            leftRen = vtkRenderer.New();                //初始化renderer
            //以下两句初始化交互事件（鼠标、相机）
            vtkInteractorStyleTrackballCamera interactorStyle = vtkInteractorStyleTrackballCamera.New();
            vtkRenderWindowInteractor renderWindowInteractor = vtkRenderWindowInteractor.New();

            imageData.ShallowCopy(reader.GetOutput());      //从reader中获取imageData
            //leftRen.SetBackground(0.2, 0.3, 0.4);
            //leftRen.SetBackground(0, 0, 1);
            leftRen.SetBackground(0, 0, 0);                 //设置背景颜色RGB（这里是黑色）

            ModelWin.AddRenderer(leftRen);                  //设置leftRen为ModelWin的渲染器
            renderWindowInteractor.SetInteractorStyle(interactorStyle); //鼠标交互
            renderWindowInteractor.SetRenderWindow(ModelWin);           //交互器的对象是ModelWin（即在ModelWin生效）

            //以下四行固定用法
            volumeMapper.SetMaxMemoryFraction(0.1f);    //设置内存，不设置用默认的话容易内存不足
            volumeMapper.SetBlendModeToComposite();
            volumeMapper.SetRequestedRenderMode((int)vtkSmartVolumeMapper.DefaultRenderMode_WrapperEnum.GPURenderMode);
            volumeMapper.SetInput(imageData);           //设置mapper的输入项为imageData
            //以下两行固定用法
            volumeProperty.ShadeOn();
            volumeProperty.SetInterpolationTypeToLinear();

            setCropPlanes();
            volumeMapper.CroppingOff();         //让切面不生效，即默认显示全的三维模型
            volume.SetMapper(volumeMapper);     //将volumeMapper赋给volume

            volume.SetProperty(volumeProperty); //设置volume的显示属性
            leftRen.AddVolume(volume);          //将volume赋给渲染器
            leftRen.ResetCamera();              //重置相机
            //volume.SetOrientation(-90, 90, 0);
            //var or = volume.GetOrientation();
            //Console.WriteLine(or[0] + ":" + or[1] + ":" + or[2]);
            c = volume.GetCenter(); //暂时无用
            //Rotate();
            
            //var iop = reader.GetImageOrientationPatient();
            //if (iop[0] == 1 && iop[1] == 0)
            //{
            //    Rotate();
            //}
            //else
            //{
            //    volume.SetOrientation(-90, 90, 0);
            //}
            //for (int i = 0; i < iop.Length; i++)
            //{
            //    Console.WriteLine(iop[i]);
            //}

            double[] d = leftRen.GetActiveCamera().GetOrientationWXYZ();    //暂时无用
            Console.WriteLine(d[0] + ":" + d[1] + ":" + d[2] + ":" + d[3]); //暂时无用

            //以下数行为鼠标事件的绑定
            me = new MouseEvents(leftRen, ModelWin, volume, interactorStyle);
            interactorStyle.LeftButtonPressEvt += me.InteractorStyle_LeftButtonPressEvt;
            interactorStyle.LeftButtonReleaseEvt += me.InteractorStyle_LeftButtonReleaseEvt;
            interactorStyle.MiddleButtonPressEvt += me.InteractorStyle_MiddleButtonPressEvt;
            interactorStyle.MiddleButtonReleaseEvt += me.InteractorStyle_MiddleButtonReleaseEvt;
            interactorStyle.MouseWheelBackwardEvt += me.InteractorStyle_MouseWheelBackwardEvt;
            interactorStyle.MouseWheelForwardEvt += me.InteractorStyle_MouseWheelForwardEvt;
            interactorStyle.MouseMoveEvt += me.InteractorStyle_MouseMoveEvt;

            ModelWin.Render();  //开始渲染ModelWin

            //tdef为左右两个影像的三维显示窗口，3D电视用
            tdef = new ThreeD2EForm(reader, me);
            tdef.VolumeLoaded = VolumeLoaded;
            tdef.LoadComplete = TDEFLoadComplete;
            try
            {
                tdef.Show();        //显示此窗口
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
        private ThreeD2EForm tdef;          //tdef为左右两个影像的三维显示窗口，3D电视用
        /// <summary>
        /// 当3D显示窗口加载结束执行，让预设图像色彩方案设置为第一个方案
        /// </summary>
        private void TDEFLoadComplete()
        {
            ComboPresets.SelectedIndex = 0;
        }

        /// <summary>
        /// volume加载结束执行，让预设图像色彩方案设置为第一个方案
        /// </summary>
        private void VolumeLoaded()
        {
            if (ComboPresets.Items.Count > 0)
                ComboPresets.SelectedItem = ComboPresets.Items[0];
        }

        #region 旧的代码，无用
        private double[] c;
        private double rotateAngle_X = 90;
        private double rotateAngle_Y = 0;
        private double rotateAngle_Z = 0;
        private void Rotate()
        {
            vtkTransform t = vtkTransform.New();
            t.Translate(c[0], c[1], c[2]);

            //t.RotateWXYZ(rotateAngle_X, 1, 0, 0);
            //t.RotateWXYZ(rotateAngle_Y, 0, 1, 0);
            //t.RotateWXYZ(rotateAngle_Z, 0, 0, 1);
            t.RotateX(rotateAngle_X);
            t.RotateY(rotateAngle_Y);
            t.RotateZ(rotateAngle_Z);
            t.Translate(-c[0], -c[1], -c[2]);
            volume.SetUserTransform(t);
            ModelWin.Render();
        }
        #endregion

        //以下六个变量为模型三个切向的最小最大值
        private double _MinCropX = -500.0;
        private double _MaxCropX = 500.0;
        private double _MinCropY = -500.0;
        private double _MaxCropY = 500.0;
        private double _MinCropZ = -500.0;
        private double _MaxCropZ = 500.0;

        /// <summary>
        /// 设置三维模型三向切面的初始阀值
        /// </summary>
        private void setCropPlanes()
        {
            if (volumeMapper != null)
                volumeMapper.SetCroppingRegionPlanes(_MinCropX, _MaxCropX, _MinCropY, _MaxCropY, _MinCropZ, _MaxCropZ);
        }

        /// <summary>
        /// 设置三维图像显示属性
        /// </summary>
        /// <param name="pov"></param>
        public void SetProperty(PropertyOfVolume pov)
        {
            volumeProperty.SetAmbient(pov.ambient);
            volumeProperty.SetDiffuse(pov.diffuse);
            volumeProperty.SetSpecular(pov.specular);
            volumeProperty.SetSpecularPower(pov.SpecularPower);
            volumeProperty.SetShade((int)pov.shade);
            volumeProperty.SetInterpolationType((int)pov.interpolation);

            vtkPiecewiseFunction gradientOpacity = vtkPiecewiseFunction.New();

            foreach (double[] gradient in pov.GradientOpacity)
            {
                gradientOpacity.AddPoint(gradient[0], gradient[1]);
            }
            volumeProperty.SetGradientOpacity(gradientOpacity);

            vtkPiecewiseFunction scalarOpacity = vtkPiecewiseFunction.New();
            foreach (double[] scalar in pov.ScalarOpacity)
            {
                scalarOpacity.AddPoint(scalar[0], scalar[1]);
            }
            volumeProperty.SetScalarOpacity(scalarOpacity);

            vtkColorTransferFunction colorTransfer = vtkColorTransferFunction.New();
            foreach (double[] color in pov.ColorTransfer)
            {
                colorTransfer.AddRGBPoint(color[0], color[1], color[2], color[3]);
            }
            volumeProperty.SetColor(colorTransfer);

            ModelWin.Render();
        }

        public void SetScalar(vtkPiecewiseFunction scalar)
        {
            volumeProperty.SetScalarOpacity(scalar);
            ModelWin.Render();
        }

        public void SetColor(vtkColorTransferFunction vctf)
        {
            volumeProperty.SetColor(vctf);
            ModelWin.Render();
        }

        #endregion

        #region Events
        /// <summary>
        /// 调整双眼距离（3D电视用）
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void trackBarControl1_EditValueChanged(object sender, EventArgs e)
        {
            //Console.WriteLine(EyeDistBar.Value);
            //tdef.EyesDistanceAdjust(EyeDistBar.Value);
            me.EyeDistAdjust(EyeDistBar.Value);
        }

        /// <summary>
        /// 三向切面调整trackbar，以下YTrackBar与ZTrackBar相同
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void XTrackBar_EditValueChanged(object sender, EventArgs e)
        {
            CropPlanesChange(XTrackBar.Value.Minimum, 0);
            CropPlanesChange(XTrackBar.Value.Maximum, 1);
            tdef.CropPlanesChange(XTrackBar.Value.Minimum, 0);
            tdef.CropPlanesChange(XTrackBar.Value.Maximum, 1);
        }
        #endregion

        /// <summary>
        /// 从后向前渐显过程（录视频用）
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void proBtn_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 500; i++)
            {
                CropPlanesChange(i, 3);
                //break;
                Thread.Sleep(10);
            }
        }
        /// <summary>
        /// 从下向上渐显过程（录视频用）
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void proBtn2_Click(object sender, EventArgs e)
        {
            //for (int i = -120; i < 200; i++)
            //{
            //    CropPlanesChange(i, 4);
            //    Thread.Sleep(50);
            //}
            for (int i = 500; i >= -110; i--)
            {
                CropPlanesChange(i, 4);
                Thread.Sleep(10);
            }
        }

        /// <summary>
        /// 三维图你切面变化
        /// </summary>
        /// <param name="trackbarVal"></param>
        /// <param name="index"></param>
        public void CropPlanesChange(int trackbarVal, int index)
        {
            double[] arr = volumeMapper.GetCroppingRegionPlanes();
            arr[index] = trackbarVal / 2.0;
            volumeMapper.SetCroppingRegionPlanes(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
            ModelWin.Render();
        }

        private PropertyOfVolume currPov;
        private List<double[]> currScalar;
        /// <summary>
        /// 下拉框选择色彩方案
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ComboPresets_SelectedIndexChanged(object sender, EventArgs e)
        {
            string name = ComboPresets.SelectedItem.ToString();
            Console.WriteLine(name);
            if (pdb.PropDict.ContainsKey(name))
            {
                if (tdef != null)
                    tdef.SetProperty(pdb.PropDict[name]);
                SetProperty(pdb.PropDict[name]);
                currPov = pdb.PropDict[name];
                currScalar = currPov.ScalarOpacity;
                double range = currPov.effectiveRange_Max - currPov.effectiveRange_Min;
                int emin = -(int)(range * 100 / 2 / currPov.baiSu);
                int emax = (int)(range * 100 / 2 / currPov.baiSu);
                ThresholdBar.Properties.Maximum = emax;
                ThresholdBar.Properties.Minimum = emin;
                ThresholdBar.Value = 0;
            }
        }
        /// <summary>
        /// 调整ThresholdBar（三维图像显示）
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ThresholdBar_EditValueChanged(object sender, EventArgs e)
        {
            vtkPiecewiseFunction scalar = vtkPiecewiseFunction.New();
            foreach (var item in currScalar)
            {
                scalar.AddPoint(item[0] + ThresholdBar.Value / 10.0, item[1]);
            }
            SetScalar(scalar);
            vtkColorTransferFunction vctf = vtkColorTransferFunction.New();
            foreach (var item in currPov.ColorTransfer)
            {
                vctf.AddRGBPoint(item[0] + ThresholdBar.Value / 10.0, item[1], item[2], item[3]);
            }
            SetColor(vctf);
            try
            {
                tdef.SetScalar(scalar);
                tdef.SetColor(vctf);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        private bool isCropOn = false;
        /// <summary>
        /// 点击打开/关闭Crop(三维模型三向切)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CropBtn_Click(object sender, EventArgs e)
        {
            isCropOn = !isCropOn;
            CropBtn.Text = isCropOn ? "Crop OFF" : "Crop ON";
            xLabel.Visible = isCropOn;
            yLabel.Visible = isCropOn;
            zLabel.Visible = isCropOn;
            XTrackBar.Visible = isCropOn;
            YTrackbar.Visible = isCropOn;
            ZTrackbar.Visible = isCropOn;
            CropOnOff(isCropOn);
            if (tdef != null)
                tdef.CropOnOff(isCropOn);
        }

        public void CropOnOff(bool isOn)
        {
            if (volumeMapper != null && ModelWin != null)
            {
                if (isOn)
                {
                    volumeMapper.CroppingOn();
                }
                else
                {
                    volumeMapper.CroppingOff();
                }
                ModelWin.Render();
            }
        }

        private void YTrackbar_EditValueChanged(object sender, EventArgs e)
        {
            CropPlanesChange(YTrackbar.Value.Minimum, 2);
            CropPlanesChange(YTrackbar.Value.Maximum, 3);
            tdef.CropPlanesChange(YTrackbar.Value.Minimum, 2);
            tdef.CropPlanesChange(YTrackbar.Value.Maximum, 3);
        }

        private void ZTrackbar_EditValueChanged(object sender, EventArgs e)
        {
            CropPlanesChange(ZTrackbar.Value.Minimum, 4);
            CropPlanesChange(ZTrackbar.Value.Maximum, 5);
            tdef.CropPlanesChange(ZTrackbar.Value.Minimum, 4);
            tdef.CropPlanesChange(ZTrackbar.Value.Maximum, 5);
        }

        private bool tdefShow = true;
        /// <summary>
        /// 显隐3D图像窗口（左右3D成像）
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ShowHide3DBtn_Click(object sender, EventArgs e)
        {
            if (tdef != null)
            {
                tdefShow = !tdefShow;
                tdef.Visible = tdefShow;
                ShowHide3DBtn.Text = tdefShow ? "Hide 3D" : "Show 3D";
            }
        }

        /// <summary>
        /// 初始化
        /// </summary>
        private void clearAndInit()
        {
            groupControl1.Visible = false;
            splitContainer1.Visible = false;
            ribbonControl1.Minimized = false;

            XTrackBar.Value = new DevExpress.XtraEditors.Repository.TrackBarRange(XTrackBar.Properties.Minimum, XTrackBar.Properties.Maximum);
            YTrackbar.Value = new DevExpress.XtraEditors.Repository.TrackBarRange(YTrackbar.Properties.Minimum, YTrackbar.Properties.Maximum);
            ZTrackbar.Value = new DevExpress.XtraEditors.Repository.TrackBarRange(ZTrackbar.Properties.Minimum, ZTrackbar.Properties.Maximum);

            EyeDistBar.Value = 0;
            ThresholdBar.Value = 0;

            isCropOn = true;
            CropBtn_Click(null, null);
            if (tdef != null)
            {
                tdefShow = false;
                ShowHide3DBtn_Click(null, null);
                tdef.Dispose();
                tdef = null;
            }

            if (AxialCtrl != null)
            {
                AxialCtrl.Dispose();
                AxialCtrl = null;
            }
            if (SaggitalCtrl != null)
            {
                SaggitalCtrl.Dispose();
                SaggitalCtrl = null;
            }
            if (CoronalCtrl != null)
            {
                CoronalCtrl.Dispose();
                CoronalCtrl = null;
            }
            if (ModelCtrl != null)
            {
                ModelCtrl.Dispose();
                ModelCtrl = null;
            }
            ComboPresets.Items.Clear();
        }

        private void InitBtn_Click(object sender, EventArgs e)
        {
            clearAndInit();
        }

        /// <summary>
        /// 读取“脑”CT
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void BrainBtn_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            loadPacs(@"ncx");
            //loadPacs("PAN_YUE_YING-MR0372461");
            //loadPacs("CHEN_BI_ZHAN");
            //loadPacs(@"301_12_19\20191219\09001353\4");
            //loadPacs("CBCT");
            //loadPacs("ZHANG_XIU_MEI-CT0785537");
        }

        /// <summary>
        /// 窗口关闭时，关进程
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            //Application.Exit();
            Process.GetCurrentProcess().Kill();
            //DialogResult result = MessageBox.Show("你确定要关闭吗！", "提示信息", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            //if (result == DialogResult.OK)
            //{
            //    e.Cancel = false;  //点击OK
            //}
            //else
            //{
            //    e.Cancel = true;
            //}
        }

        /// <summary>
        /// 读取“肺”CT
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void LungBtn_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            loadPacs("dyx");
        }

        /// <summary>
        /// 读取胸腹CT
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AbdBtn_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            loadPacs("abd");
        }

        private void KosetsuBtn_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            loadPacs("Kos2");
        }

        private void AxialBar_EditValueChanged_1(object sender, EventArgs e)
        {

        }

        private void SaggitalBar_EditValueChanged_1(object sender, EventArgs e)
        {

        }
    }
}