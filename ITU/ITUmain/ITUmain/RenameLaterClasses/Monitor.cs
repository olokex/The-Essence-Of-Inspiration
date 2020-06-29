using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ITUmain.RenameLaterClasses
{
    class Monitor : DeviceType
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
        public string Dimensions { get; set; }
        public string PanelType { get; set; }
        public string Resolution { get; set; }
        public string Frequency { get; set; }
        public string ResponseTime { get; set; }
        public string Connectors { get; set; }
        public string Vesa  { get; set; }
        public string Speakers  { get; set; }
        public string CablesIncluded { get; set; }
    }
}
