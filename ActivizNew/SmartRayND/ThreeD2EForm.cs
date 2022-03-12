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

namespace SmartRayND
{
    public partial class ThreeD2EForm : DevExpress.XtraEditors.XtraForm
    {
        private RenderWindowControl ModelCtrl;
        private vtkDICOMImageReader reader;
        private vtkVolume volume;
        private vtkVolumeProperty volumeProperty;
        private vtkSmartVolumeMapper volumeMapper;
        private vtkRenderer leftRen;
        private vtkRenderer rightRen;
        private vtkRenderWindow ModelWin;

        public delegate void VolumeLoadedDel();
        public VolumeLoadedDel VolumeLoaded;

        private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams myCp = base.CreateParams;
                myCp.ClassStyle = myCp.ClassStyle | CP_NOCLOSE_BUTTON;
                return myCp;
            }
        }

        public ThreeD2EForm()
        {
            InitializeComponent();
        }
        public ThreeD2EForm(vtkDICOMImageReader reader)
        {
            this.reader = reader;
            InitializeComponent();
        }

        private MouseEvents me;
        public ThreeD2EForm(vtkDICOMImageReader reader, MouseEvents me)
        {
            this.reader = reader;
            this.me = me;
            InitializeComponent();
        }

        private void ThreeD2EForm_Load(object sender, EventArgs e)
        {
            ModelCtrl = new RenderWindowControl();
            ModelCtrl.Parent = MainPanel;
            ModelCtrl.BackColor = Color.Black;
            ModelCtrl.Dock = DockStyle.Fill;
            ShowVolume();
        }

        private void ShowVolume()
        {
            volumeProperty = vtkVolumeProperty.New();
            volumeMapper = vtkSmartVolumeMapper.New();
            volume = vtkVolume.New();

            ModelWin = ModelCtrl.RenderWindow;
            vtkImageData imageData = vtkImageData.New();
            leftRen = vtkRenderer.New();
            rightRen = vtkRenderer.New();
            leftRen.SetViewport(0.0, 0.0, 0.5, 1.0);
            rightRen.SetViewport(0.5, 0.0, 1.0, 1.0);
            vtkInteractorStyleTrackballCamera interactorStyle = vtkInteractorStyleTrackballCamera.New();
            vtkRenderWindowInteractor renderWindowInteractor = vtkRenderWindowInteractor.New();
            renderWindowInteractor.SetInteractorStyle(interactorStyle);
            renderWindowInteractor.SetRenderWindow(ModelWin);

            imageData.ShallowCopy(reader.GetOutput());
            //leftRen.SetBackground(0.2, 0.3, 0.4);
            //rightRen.SetBackground(0.2, 0.3, 0.4);
            leftRen.SetBackground(0, 0, 0);
            rightRen.SetBackground(0, 0, 0);

            ModelWin.AddRenderer(leftRen);
            ModelWin.AddRenderer(rightRen);

            volumeMapper.SetMaxMemoryFraction(0.1f);
            volumeMapper.SetBlendModeToComposite();
            volumeMapper.SetRequestedRenderMode((int)vtkSmartVolumeMapper.DefaultRenderMode_WrapperEnum.GPURenderMode);
            volumeMapper.SetInput(imageData);

            volumeProperty.ShadeOn();
            volumeProperty.SetInterpolationTypeToLinear();

            setCropPlanes();
            volumeMapper.CroppingOff();
            volume.SetMapper(volumeMapper);

            volume.SetProperty(volumeProperty);
            leftRen.AddVolume(volume);
            leftRen.ResetCamera();

            rightRen.AddVolume(volume);
            rightRen.ResetCamera();

            c = volume.GetCenter();
            Rotate();

            oriPosCam1 = leftRen.GetActiveCamera().GetPosition();
            oriFocCam1 = leftRen.GetActiveCamera().GetFocalPoint();
            oriPosCam2 = rightRen.GetActiveCamera().GetPosition();
            oriFocCam2 = rightRen.GetActiveCamera().GetFocalPoint();

            vtkTransform vt = vtkTransform.New();
            vtkMatrix4x4 mat1 = vtkMatrix4x4.New();
            mat1.SetElement(0, 0, 0.5);
            mat1.SetElement(1, 1, 1);
            mat1.SetElement(2, 2, 1);
            mat1.SetElement(3, 3, 1);
            vt.SetMatrix(mat1);
            leftRen.GetActiveCamera().SetUserTransform(vt);
            rightRen.GetActiveCamera().SetUserTransform(vt);
            //mat1.SetElement(0, 0, 1);
            //vt.SetMatrix(mat1);


            interactorStyle.LeftButtonPressEvt += InteractorStyle_LeftButtonPressEvt;
            interactorStyle.RightButtonPressEvt += InteractorStyle_RightButtonPressEvt;
            interactorStyle.MiddleButtonPressEvt += InteractorStyle_MiddleButtonPressEvt;
            interactorStyle.LeftButtonReleaseEvt += InteractorStyle_LeftButtonReleaseEvt;
            interactorStyle.RightButtonReleaseEvt += InteractorStyle_RightButtonReleaseEvt;
            interactorStyle.MiddleButtonReleaseEvt += InteractorStyle_MiddleButtonReleaseEvt;
            interactorStyle.MouseWheelForwardEvt += InteractorStyle_MouseWheelForwardEvt;
            interactorStyle.MouseWheelBackwardEvt += InteractorStyle_MouseWheelBackwardEvt;
            interactorStyle.MouseMoveEvt += InteractorStyle_MouseMoveEvt;

            ModelWin.Render();
            me.SetLeftRight(leftRen, rightRen, ModelWin, volume);
            //VolumeLoaded();

            if (LoadComplete != null)
            {
                LoadComplete();
            }
            this.Show();
            renderWindowInteractor.Start();
        }

        public delegate void LoadCompleteDel();
        public LoadCompleteDel LoadComplete;

        private double _MinCropX = -500.0;
        private double _MaxCropX = 500.0;
        private double _MinCropY = -500.0;
        private double _MaxCropY = 500.0;
        private double _MinCropZ = -500.0;
        private double _MaxCropZ = 500.0;

        private void setCropPlanes()
        {
            volumeMapper.SetCroppingRegionPlanes(_MinCropX, _MaxCropX, _MinCropY, _MaxCropY, _MinCropZ, _MaxCropZ);
        }

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

        public void CropOnOff(bool isOn)
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

        public void CropPlanesChange(int trackbarVal, int index)
        {
            double[] arr = volumeMapper.GetCroppingRegionPlanes();
            arr[index] = trackbarVal / 2.0;
            volumeMapper.SetCroppingRegionPlanes(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
            ModelWin.Render();
        }

        #region Control

        private bool leftPressed = false;
        private bool rightPressed = false;
        private bool middlePressed = false;

        private double[] oriPosCam1;
        private double[] oriFocCam1;
        private double[] oriPosCam2;
        private double[] oriFocCam2;

        /// <summary>
        /// 以下交互方法设为空，即不让此窗口中响应鼠标事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void InteractorStyle_MiddleButtonReleaseEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //middlePressed = false;
            //ep = ModelWin.GetInteractor().GetEventPosition();
        }

        private void InteractorStyle_RightButtonReleaseEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //rightPressed = false;
        }

        private void InteractorStyle_LeftButtonReleaseEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //leftPressed = false;
            //ep = ModelWin.GetInteractor().GetEventPosition();
        }

        private void InteractorStyle_MiddleButtonPressEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //middlePressed = true;
            //ep = ModelWin.GetInteractor().GetEventPosition();
        }

        private void InteractorStyle_RightButtonPressEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //rightPressed = true;
        }

        int[] ep;
        private void InteractorStyle_LeftButtonPressEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //leftPressed = true;
            //ep = ModelWin.GetInteractor().GetEventPosition();

        }

        private double deltaDegree = 5;
        private void InteractorStyle_MouseMoveEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //if (leftPressed)
            //{
            //    //int[] tmp = ModelWin.GetInteractor().GetEventPosition();
            //    int[] tmp = ModelWin.GetInteractor().GetLastEventPosition();
            //    if (tmp[0] < ep[0])
            //    {
            //        rotateAngle_Z += deltaDegree;
            //    }
            //    else if (tmp[0] > ep[0])
            //    {
            //        rotateAngle_Z -= deltaDegree;
            //    }
            //    if (tmp[1] < ep[1])
            //    {
            //        rotateAngle_X += deltaDegree;
            //    }
            //    else if (tmp[1] > ep[1])
            //    {
            //        rotateAngle_X -= deltaDegree;
            //    }
            //    Transform();
            //    ep = tmp;
            //}
            //if (middlePressed)
            //{
            //    int[] tmp = ModelWin.GetInteractor().GetEventPosition();
            //    double dmx = 0;
            //    double dmy = 0;
            //    if (tmp[0] < ep[0])
            //    {
            //        //move_X -= deltaMove;
            //        dmx = deltaMove;
            //    }
            //    else if (tmp[0] > ep[0])
            //    {
            //        //move_X += deltaMove;
            //        dmx = -deltaMove;
            //    }
            //    if (tmp[1] < ep[1])
            //    {
            //        //move_Z += deltaMove;
            //        dmy = deltaMove;
            //    }
            //    else if (tmp[1] > ep[1])
            //    {
            //        //move_Z -= deltaMove;
            //        dmy = -deltaMove;
            //    }
            //    //Transform();
            //    var pos1 = leftRen.GetActiveCamera().GetPosition();
            //    var foc1 = leftRen.GetActiveCamera().GetFocalPoint();
            //    var pos2 = rightRen.GetActiveCamera().GetPosition();
            //    var foc2 = rightRen.GetActiveCamera().GetFocalPoint();
            //    leftRen.GetActiveCamera().SetPosition(pos1[0] + dmx, pos1[1] + dmy, pos1[2]);
            //    leftRen.GetActiveCamera().SetFocalPoint(foc1[0] + dmx, foc1[1] + dmy, foc1[2]);
            //    rightRen.GetActiveCamera().SetPosition(pos2[0] + dmx, pos2[1] + dmy, pos2[2]);
            //    rightRen.GetActiveCamera().SetFocalPoint(foc2[0] + dmx, foc2[1] + dmy, foc2[2]);
            //    ep = tmp;
            //    ModelWin.Render();
            //}
        }

        private double scale = 1;
        private double deltaScale = 0.2;

        private double[] c;

        private double rotateAngle_X = 90;
        private double rotateAngle_Y = 0;
        private double rotateAngle_Z = 0;

        private double deltaMove = 15;

        private void Transform()
        {
            vtkTransform t = vtkTransform.New();
            t.Translate(c[0], c[1], c[2]);
            t.RotateX(rotateAngle_X);
            t.RotateY(rotateAngle_Y);
            t.RotateZ(rotateAngle_Z);
            t.Scale(scale, scale, scale);
            t.Translate(-c[0], -c[1], -c[2]);
            volume.SetUserTransform(t);
            leftRen.ResetCameraClippingRange();
            rightRen.ResetCameraClippingRange();
            ModelWin.Render();
        }

        private void Rotate()
        {
            vtkTransform t = vtkTransform.New();
            t.Translate(c[0], c[1], c[2]);
            t.RotateX(rotateAngle_X);
            t.RotateY(rotateAngle_Y);
            t.RotateZ(rotateAngle_Z);
            t.Translate(-c[0], -c[1], -c[2]);
            volume.SetUserTransform(t);
            ModelWin.Render();
        }

        private void InteractorStyle_MouseWheelBackwardEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //scale -= deltaScale;
            //if (scale < deltaScale)
            //{
            //    scale = deltaScale;
            //}
            //Transform();
        }

        private void InteractorStyle_MouseWheelForwardEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //scale += deltaScale;
            //Transform();
        }

        #endregion

        #region Eyes Distance

        double dist = 0;

        public void EyesDistanceAdjust(int value)
        {
            vtkCamera camLeft = leftRen.GetActiveCamera();
            vtkCamera camRight = rightRen.GetActiveCamera();
            double x = dist + value / 100.0;

            camRight.SetPosition(camLeft.GetPosition()[0] + x, camLeft.GetPosition()[1], camLeft.GetPosition()[2]);
            camRight.SetRoll(camLeft.GetRoll());
            leftRen.ResetCameraClippingRange();
            rightRen.ResetCameraClippingRange();
            ModelWin.Render();
        }

        #endregion
    }
}