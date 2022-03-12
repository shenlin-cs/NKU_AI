using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Entity
{
    /// <summary>
    /// 三维影像色彩方案类
    /// </summary>
    public class PropertyOfVolume
    {
        public string name;
        public List<double[]> GradientOpacity;
        public double SpecularPower;
        public List<double[]> ScalarOpacity;
        public double specular;
        public double shade;
        public double ambient;
        public List<double[]> ColorTransfer;
        public double diffuse;
        public double interpolation;
        public double effectiveRange_Min;
        public double effectiveRange_Max;
        public double baiSu = 1;

        public PropertyOfVolume()
        {
            GradientOpacity = new List<double[]>();
            ScalarOpacity = new List<double[]>();
            ColorTransfer = new List<double[]>();
        }

        private void AddGradient(double x, double y)
        {
            GradientOpacity.Add(new double[]{ x, y});
        }

        private void AddScalar(double x, double y)
        {
            ScalarOpacity.Add(new double[] { x, y });
        }

        private void AddColor(double x, double r, double g, double b)
        {
            ColorTransfer.Add(new double[] { x, r, g, b });
        }

        public void AddColor(string str)
        {
            string[] arr = str.Split(' ');
            if ((arr.Length - 1) % 4 == 0)
            {
                for (int i = 1; i < arr.Length; i++)
                {
                    double x, r, g, b;
                    bool tryx = double.TryParse(arr[i], out x);
                    bool tryr = double.TryParse(arr[i + 1], out r);
                    bool tryg = double.TryParse(arr[i + 2], out g);
                    bool tryb = double.TryParse(arr[i + 3], out b);
                    AddColor(x, r, g, b);
                    i = i + 3;
                }
            }
        }

        public void AddProperty(string str, PropertyType type)
        {
            string[] arr = str.Split(' ');
            if (arr.Length % 2 != 0)
            {
                for (int i = 1; i < arr.Length; i++)
                {
                    double x = 0;
                    double y = 0;
                    bool tryX = double.TryParse(arr[i], out x);
                    bool tryY = double.TryParse(arr[i + 1], out y);
                    switch (type)
                    {
                        case PropertyType.Gradient:
                            AddGradient(x, y);
                            break;
                        case PropertyType.Scalar:
                            AddScalar(x, y);
                            break;
                        default:
                            break;
                    }
                    i++;
                }
            }
        }
    }
}
