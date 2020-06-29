using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ITUmain.RenameLaterClasses
{
    class Computer : DeviceType
    {
        public string ManufactureNumber { get; set; }
        public string Manufacturer { get; set; }
        public string Name { get; set; }
        public string Condition { get; set; }
        public string Price { get; set; }
        public string Place { get; set; } // location?
        public string Description { get; set; }
        public string Author { get; set; }
        public DateTime Date { get; set; }
        // end
        public string CPU { get; set; }
        public string RAM { get; set; }
        public string GPU { get; set; }
        public string Memory { get; set; }
        public string Dimensions { get; set; }
        public string OS { get; set; }
        public string Ports { get; set; }
    }
}
