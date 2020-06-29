using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ITUmain.View
{
    /// <summary>
    /// Interakční logika pro AddDevice.xaml
    /// </summary>
    public partial class AddDevice : Page
    {
        public delegate void AddDeviceButtonClick();
        public event AddDeviceButtonClick ReturnBackEvent;
        public AddDevice()
        {
            InitializeComponent();
        }

        private void ItemAddCombobox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (this.IsInitialized)
            {
                telephone.Visibility = Visibility.Hidden;
                monitor.Visibility = Visibility.Hidden;
                computer.Visibility = Visibility.Hidden;
                notebook.Visibility = Visibility.Hidden;

                switch (ItemAddCombobox.SelectedItem.ToString())
                {
                    case "System.Windows.Controls.ComboBoxItem: Telefon":
                        telephone.Visibility = Visibility.Visible;
                        break;
                    case "System.Windows.Controls.ComboBoxItem: Notebook":
                        notebook.Visibility = Visibility.Visible;
                        break;
                    case "System.Windows.Controls.ComboBoxItem: PC":
                        computer.Visibility = Visibility.Visible;
                        break;
                    case "System.Windows.Controls.ComboBoxItem: Monitor":
                        monitor.Visibility = Visibility.Visible;
                        break;
                }

            }
        }

        private void AddDeviceButtonOnClick(object sender, RoutedEventArgs e)
        {
            ReturnBackEvent();
        }

        private void AddDeviceButtonAddClick(object sender, RoutedEventArgs e)
        {
            switch (ItemAddCombobox.Text)
            {
                case "Telefon":
                    //string stringValue = TNam.SelectedValue.ToString();

                    RenameLaterClasses.Telephone tmp = new RenameLaterClasses.Telephone()
                    {
                        TypeOfDevice = "telefon",
                        Author = "Pavelka",
                        Date = DateTime.Now,
                        ManufactureNumber = TManNum.Text,
                        Manufacturer = TMan.Text,
                        Name = TNam.Text,
                        CPU = TCpu.Text,
                        RAM = TRam.Text,
                        Display = TDisp.Text,
                        Size = TDim.Text,
                        Camera = TCam.Text,
                        SdCard = TSd.Text,
                        DualSim = TDual.Text,
                        Weight = TWeight.Text,
                        Colour = TCol.Text,
                        ChargingTime = TBattery.Text,
                        Condition = TCon.Text,
                        Price = (string)TPrice.Text
                    };
                    LoadExampleData.DataTest.Add(tmp);
                   
                    break;
                case "Notebook":
                    RenameLaterClasses.Notebook tmp3 = new RenameLaterClasses.Notebook()
                    {
                        TypeOfDevice = "notebook",
                        Author = "Pavelka",
                        Date = DateTime.Now,
                        ManufactureNumber = NManNum.Text,
                        Name = NName.Text,
                        Manufacturer = NMan.Text,
                        CPU = NCpu.Text,
                        RAM = NRam.Text,
                        Memory = NMem.Text,
                        GPU = NGpu.Text,
                        Dimensions = NDim.Text,
                        OS = NOS.Text,
                        Ports = NPorts.Text,
                        Weight = NWeight.Text,
                        BatteryCappacity = NBat.Text,
                        Display = NDis.Text,
                        Camera = NCam.Text,
                        FingerPrintReader = NRead.Text,
                        Numpad = NNumpad.Text,
                        Condition = NCon.Text,
                        Price = NPrice.Text
                    };
                    LoadExampleData.DataTest.Add(tmp3);
                    break;
                case "PC":
                    RenameLaterClasses.Computer tmp2 = new RenameLaterClasses.Computer()
                    {
                        TypeOfDevice = "computer",
                        Author = "Lorenz",
                        Date = DateTime.Now,
                        Name = CNam.Text,
                        ManufactureNumber = CManNum.Text,
                        Manufacturer = CMan.Text,
                        CPU = CCpu.Text,
                        RAM = CRam.Text,
                        Memory = CMem.Text,
                        GPU = CGpu.Text,
                        Dimensions = CDim.Text,
                        OS = COS.Text,
                        Ports = CPort.Text,
                        Condition = CCondition.Text,
                        Price = CPrice.Text
                    };
                    LoadExampleData.DataTest.Add(tmp2);
                    break;
                case "Monitor":
                    RenameLaterClasses.Monitor tmp1 = new RenameLaterClasses.Monitor()
                    {
                        TypeOfDevice = "monitor",
                        Author = "Lorenz",
                        Date = DateTime.Now,
                        ManufactureNumber = MManNum.Text,
                        Dimensions = MDim.Text,
                        PanelType = MPan.Text,
                        Resolution = MRes.Text,
                        Frequency = MFreq.Text,
                        ResponseTime = MResp.Text,
                        Connectors = MCon.Text,
                        Vesa = MVesa.Text,
                        Speakers = MRep.Text,
                        CablesIncluded = MCabel.Text,
                        Condition = MCondition.Text,
                        Price = MPrice.Text
                    };
                    LoadExampleData.DataTest.Add(tmp1);
                    break;
            }
        }
    }
}
