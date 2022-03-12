using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Kitware.VTK;

namespace SmartRayND
{
    public class MouseEvents
    {
        #region Control

        private bool leftPressed = false;
        private bool rightPressed = false;
        private bool middlePressed = false;

        private double[] oriPosCam1;
        private double[] oriFocCam1;
        private double[] oriPosCam2;
        private double[] oriFocCam2;

        private vtkRenderWindow ModelWin;
        private vtkRenderer leftRen;
        private vtkRenderer rightRen;
        private vtkRenderer MainRen;
        private vtkVolume volume;
        private vtkInteractorStyleTrackballCamera interactorStyle;

        public MouseEvents(vtkRenderer MainRen, vtkRenderWindow ModelWin, vtkVolume volume)
        {
            this.MainRen = MainRen;
            this.ModelWin = ModelWin;
            this.volume = volume;
            c = volume.GetCenter();
            Rotate();
        }

        public MouseEvents(vtkRenderer MainRen, vtkRenderWindow ModelWin, vtkVolume volume, vtkInteractorStyleTrackballCamera interactorStyle)
        {
            this.MainRen = MainRen;
            this.ModelWin = ModelWin;
            this.volume = volume;
            this.interactorStyle = interactorStyle;
            c = volume.GetCenter();
            Rotate();
        }

        private vtkRenderWindow lrWin;
        private vtkVolume lrVolume;
        public void SetLeftRight(vtkRenderer leftRen, vtkRenderer rightRen, vtkRenderWindow lrWin, vtkVolume lrVolume)
        {
            this.leftRen = leftRen;
            this.rightRen = rightRen;
            this.lrWin = lrWin;
            this.lrVolume = lrVolume;
        }

        public void InteractorStyle_MiddleButtonReleaseEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            interactorStyle.OnMiddleButtonUp();
            middlePressed = false;
            //ep = ModelWin.GetInteractor().GetEventPosition();
        }

        public void InteractorStyle_RightButtonReleaseEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            rightPressed = false;
        }

        public void InteractorStyle_LeftButtonReleaseEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            interactorStyle.OnLeftButtonUp();
            leftPressed = false;
            //ep = ModelWin.GetInteractor().GetEventPosition();
        }

        public void InteractorStyle_MiddleButtonPressEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            interactorStyle.OnMiddleButtonDown();
            middlePressed = true;
            //ep = ModelWin.GetInteractor().GetEventPosition();
        }

        public void InteractorStyle_RightButtonPressEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            rightPressed = true;
        }

        int[] ep;
        public void InteractorStyle_LeftButtonPressEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            interactorStyle.OnLeftButtonDown();
            leftPressed = true;
            //ep = ModelWin.GetInteractor().GetEventPosition();

        }

        private double deltaDegree = 5;

        //private int dist = 100;
        public void EyeDistAdjust(int dist)
        {
            this.dist = dist;
            syncronizePos();
        }
        private int dist;

        void syncronizePos()
        {
            if (leftRen == null || rightRen == null)
            {
                return;
            }
            var cam = MainRen.GetActiveCamera();
            var lcam = leftRen.GetActiveCamera();
            var rcam = rightRen.GetActiveCamera();
            var p = cam.GetPosition();
            var f = cam.GetFocalPoint();
            var v = cam.GetViewUp();

            lcam.SetViewUp(v[0], v[1], v[2]);
            lcam.SetPosition(p[0], p[1], p[2]);
            lcam.SetFocalPoint(f[0], f[1], f[2]);

            rcam.SetViewUp(v[0], v[1], v[2]);
            rcam.SetFocalPoint(f[0], f[1], f[2]);
            rcam.SetPosition(p[0], p[1], p[2]);

            //以下为计算右眼相机移动距离
            var n = cam.GetViewPlaneNormal();
            double[] l = { v[1] * n[2] - v[2] * n[1], v[2] * n[0] - v[0] * n[2], v[0] * n[1] - v[1] * n[0] };
            var na = Math.Sqrt(l[0] * l[0] + l[1] * l[1] + l[2] * l[2]);
            l[0] /= (na / (dist / 100.0));
            l[1] /= (na / (dist / 100.0));
            l[2] /= (na / (dist / 100.0));

            rcam.SetPosition(p[0] + l[0], p[1] + l[1], p[2] + l[2]);

            MainRen.ResetCameraClippingRange();
            leftRen.ResetCameraClippingRange();
            rightRen.ResetCameraClippingRange();
            ModelWin.Render();
            lrWin.Render();
        }

        public void InteractorStyle_MouseMoveEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            #region leftpressed
            //if (leftPressed)
            //{
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
            //        dmx = deltaMove;
            //    }
            //    else if (tmp[0] > ep[0])
            //    {
            //        dmx = -deltaMove;
            //    }
            //    if (tmp[1] < ep[1])
            //    {
            //        dmy = deltaMove;
            //    }
            //    else if (tmp[1] > ep[1])
            //    {
            //        dmy = -deltaMove;
            //    }
            //    var posM = MainRen.GetActiveCamera().GetPosition();
            //    var focM = MainRen.GetActiveCamera().GetFocalPoint();
            //    MainRen.GetActiveCamera().SetPosition(posM[0] + dmx, posM[1] + dmy, posM[2]);
            //    MainRen.GetActiveCamera().SetFocalPoint(focM[0] + dmx, focM[1] + dmy, focM[2]);
            //    if (leftRen != null)
            //    {
            //        var pos1 = leftRen.GetActiveCamera().GetPosition();
            //        var foc1 = leftRen.GetActiveCamera().GetFocalPoint();
            //        leftRen.GetActiveCamera().SetPosition(pos1[0] + dmx, pos1[1] + dmy, pos1[2]);
            //        leftRen.GetActiveCamera().SetFocalPoint(foc1[0] + dmx, foc1[1] + dmy, foc1[2]);
            //    }
            //    if (rightRen != null)
            //    {
            //        var pos2 = rightRen.GetActiveCamera().GetPosition();
            //        var foc2 = rightRen.GetActiveCamera().GetFocalPoint();
            //        rightRen.GetActiveCamera().SetPosition(pos2[0] + dmx, pos2[1] + dmy, pos2[2]);
            //        rightRen.GetActiveCamera().SetFocalPoint(foc2[0] + dmx, foc2[1] + dmy, foc2[2]);
            //    }
            //    ep = tmp;
            //    ModelWin.Render();
            //    if (lrWin != null)
            //        lrWin.Render();
            //}
            #endregion
            if(leftPressed || middlePressed)
            {
                interactorStyle.OnMouseMove();
                syncronizePos();
            }
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
            t.RotateWXYZ(rotateAngle_Y, 0, 1, 0);
            t.RotateWXYZ(rotateAngle_X, 1, 0, 0);
            t.RotateWXYZ(rotateAngle_Z, 0, 0, 1);
            //t.RotateY(rotateAngle_Y);
            //t.RotateX(rotateAngle_X);
            //t.RotateZ(rotateAngle_Z);
            //var cam = MainRen.GetActiveCamera();
            //double[] viewUp = cam.GetViewUp();
            //double[] axis = { -cam.GetViewTransformMatrix().GetElement(0,0)
            //,-cam.GetViewTransformMatrix().GetElement(0,1)
            //,-cam.GetViewTransformMatrix().GetElement(0,2)};
            //var p = cam.GetViewPlaneNormal();
            //t.RotateWXYZ(-rotateAngle_X, axis[0], axis[1], axis[2]);
            //t.RotateWXYZ(-rotateAngle_Z, viewUp[0], viewUp[1], viewUp[2]);
            //t.RotateWXYZ(rotateAngle_Z, p[0], p[1], p[2]);
            t.Scale(scale, scale, scale);
            t.Translate(-c[0], -c[1], -c[2]);
            volume.SetUserTransform(t);
            if (lrVolume != null)
                lrVolume.SetUserTransform(t);
            MainRen.ResetCameraClippingRange();
            if (leftRen != null)
                leftRen.ResetCameraClippingRange();
            if (rightRen != null)
                rightRen.ResetCameraClippingRange();
            ModelWin.Render();
            if (lrWin != null)
                lrWin.Render();
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
            if (lrVolume != null)
                lrVolume.SetUserTransform(t);
            ModelWin.Render();
            if (lrWin != null)
                lrWin.Render();
        }

        public void InteractorStyle_MouseWheelBackwardEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //scale -= deltaScale;
            //if (scale < deltaScale)
            //{
            //    scale = deltaScale;
            //}
            //Transform();
            interactorStyle.OnMouseWheelBackward();
            syncronizePos();
        }

        public void InteractorStyle_MouseWheelForwardEvt(vtkObject sender, vtkObjectEventArgs e)
        {
            //scale += deltaScale;
            //Transform();
            interactorStyle.OnMouseWheelForward();
            syncronizePos();
        }

        #endregion
    }
}
