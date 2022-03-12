using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Kitware.VTK;

namespace Entity
{
    /// <summary>
    /// vtkColorTransferFunction
    /// </summary>
    public class ColorTransferFunc
    {
        private List<double[]> colorlist;
        private ColorType cType = ColorType.RGB;

        public List<double[]> Colorlist
        {
            get
            {
                return colorlist;
            }

            //set
            //{
            //    colorlist = value;
            //}
        }

        public ColorType CType
        {
            get
            {
                return cType;
            }

            //set
            //{
            //    cType = value;
            //}
        }

        public ColorTransferFunc()
        {
            colorlist = new List<double[]>();
        }

        public ColorTransferFunc(ColorType CType)
        {
            colorlist = new List<double[]>();
            SetType(CType);
        }

        public void SetType(ColorType CType)
        {
            this.cType = CType;
        }

        public void Initialize()
        {
            colorlist.Clear();
        }

        public void AddPoint(double x, double h, double s, double v)
        {
            double[] arr = { x, h, s, v };
            colorlist.Add(arr);
        }
    }
}
