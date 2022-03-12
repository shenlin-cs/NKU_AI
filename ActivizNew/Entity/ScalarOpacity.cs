using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Kitware.VTK;

namespace Entity
{
    public class ScalarOpacity
    {
        private Dictionary<double, double> scalarDictionary;

        public Dictionary<double, double> ScalarDictionary
        {
            get
            {
                return scalarDictionary;
            }

            //set
            //{
            //    scalarDictionary = value;
            //}
        }

        public ScalarOpacity()
        {
            scalarDictionary = new Dictionary<double, double>();
        }

        public void Initialize()
        {
            scalarDictionary.Clear();
        }

        public void AddPoint(double x, double val)
        {
            scalarDictionary.Add(x, val);
        }
    }
}
