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
    /// Interakční logika pro EditRecord.xaml
    /// </summary>
    public partial class EditRecord : Page
    {
        public EditRecord()
        {
            InitializeComponent();


        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }

        public delegate void RecButtAdd();
        public event RecButtAdd EditRecClickAdd;
        private void EditRecordButtonClickAdd(object sender, RoutedEventArgs e)
        {
            EditRecClickAdd();
        }

        public delegate void RecButtOverview();
        public event RecButtAdd EditRecClickOverview;
        private void EditRecordButtonClickOverview(object sender, RoutedEventArgs e)
        {
            EditRecClickOverview();
        }

        public void neco()
        {
            for (int i = 0; i < LoadExampleData.DataTest.Count(); i++)
            {
                if (LoadExampleData.DataTest[i].WasSelected)
                {
                    EditRecordComputer.Visibility = Visibility.Hidden;
                    EditRecordNotebook.Visibility = Visibility.Hidden;
                    EditRecordMonitor.Visibility = Visibility.Hidden;
                    EditRecordTelephone.Visibility = Visibility.Hidden;
                    switch (LoadExampleData.DataTest[i].TypeOfDevice)
                    {
                        case "computer":
                            EditRecordComputer.Visibility = Visibility.Visible;
                            EditRecordComputer.DataContext = LoadExampleData.DataTest[i];
                            break;
                        case "monitor":
                            EditRecordMonitor.Visibility = Visibility.Visible;
                            EditRecordMonitor.DataContext = LoadExampleData.DataTest[i];
                            break;
                        case "telefon":
                            EditRecordTelephone.Visibility = Visibility.Visible;
                            EditRecordTelephone.DataContext = LoadExampleData.DataTest[i];
                            break;
                        case "notebook":
                            EditRecordNotebook.Visibility = Visibility.Visible;
                            EditRecordNotebook.DataContext = LoadExampleData.DataTest[i];
                            break;
                    }
                    LoadExampleData.DataTest[i].WasSelected = false;
                }
            }
        }

        public delegate void RecButtonClickEdit();
        public event RecButtonClickEdit RecButtonClickEditEvent;
        private void EditRecordButtonEditComputer(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < LoadExampleData.DataTest.Count(); i++)
            {
                if (LoadExampleData.DataTest[i].WasSelected)
                {
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Date = DateTime.Now;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Author = "Pavelka";
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Name = ECName.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Manufacturer = ECMan.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).ManufactureNumber = ECManNum.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Condition = ECCon.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Price = ECPrice.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).CPU = ECCpu.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).RAM = ECRam.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).GPU = ECGpu.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Memory = ECMem.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Dimensions = ECDim.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).OS = ECOS.Text;
                    ((RenameLaterClasses.Computer)LoadExampleData.DataTest[i]).Ports = ECPorts.Text;
                }
                LoadExampleData.DataTest[i].WasSelected = false;
            }
            RecButtonClickEditEvent();
        }

        private void EditRecordButtonEditNotebook(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < LoadExampleData.DataTest.Count(); i++)
            {
                if (LoadExampleData.DataTest[i].WasSelected)
                {
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Date = DateTime.Now;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Author = "Pavelka";
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Name = ENNam.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Manufacturer = ENMan.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).ManufactureNumber = ENManNum.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Condition = ENCond.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).FingerPrintReader = ENFing.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Price = ENPrice.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).CPU = ENCpu.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).RAM = ENRam.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).GPU = ENGpu.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Memory = ENMem.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Dimensions = ENDim.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Numpad = ENNumpad.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).OS = ENOS.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).BatteryCappacity = ENBat.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Weight = ENWeight.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Camera = ENCam.Text;
                    ((RenameLaterClasses.Notebook)LoadExampleData.DataTest[i]).Ports = ENPorts.Text;
                }
                LoadExampleData.DataTest[i].WasSelected = false;
            }
            RecButtonClickEditEvent();
        }

        private void EditRecordButtonEditMonitor(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < LoadExampleData.DataTest.Count(); i++)
            {
                if (LoadExampleData.DataTest[i].WasSelected)
                {
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Author = "Pavelka";
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Date = DateTime.Now;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Name = EMNam.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Manufacturer = EMMan.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).ManufactureNumber = EMManNum.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Condition = EMCon.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Price = EMPrice.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Dimensions = EMDim.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).PanelType = EMPanelType.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Resolution = EMRes.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Frequency = EMFreq.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).ResponseTime = EMResp.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Connectors = EMConnectors.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Vesa = EMVesa.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).Speakers = EMSpeak.Text;
                    ((RenameLaterClasses.Monitor)LoadExampleData.DataTest[i]).CablesIncluded = EMCables.Text;
                }
                LoadExampleData.DataTest[i].WasSelected = false;
            }
            RecButtonClickEditEvent();
        }

        private void EditRecordButtonEditTelephone(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < LoadExampleData.DataTest.Count(); i++)
            {
                if (LoadExampleData.DataTest[i].WasSelected)
                {
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Author = "Pavelka";
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Date = DateTime.Now;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Name = ETNam.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Manufacturer = ETMan.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).ManufactureNumber = ETManNum.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Condition = ETCon.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Price = ETPrice.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).CPU = ETCpu.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).RAM = ETRam.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Display = ETDisplay.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Weight = ETWeight.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Size = ETSize.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).DualSim = ETDual.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).SdCard = ETSdCard.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Camera = ETCam.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).ChargingTime = ETChar.Text;
                    ((RenameLaterClasses.Telephone)LoadExampleData.DataTest[i]).Colour = ETCol.Text;
                }
                LoadExampleData.DataTest[i].WasSelected = false;
            }
            RecButtonClickEditEvent();
        }
    }
}
