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

namespace ITUmain
{
    /// <summary>
    /// Interakční logika pro MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        View.LoginScreen loginScr = new View.LoginScreen();
        View.AddDevice adddevice = new View.AddDevice();
        View.Overview overw = new View.Overview();
        View.EditRecord rec = new View.EditRecord();
        public MainWindow()
        {
            InitializeComponent();
            current_frame.NavigationUIVisibility = NavigationUIVisibility.Hidden;
            loginScr.ViewLoginScreenEvent += LoginScreenSignin;
            adddevice.ReturnBackEvent += AddDeviceReturnBack;
            rec.EditRecClickAdd += EditRecordAddDevice;
            rec.EditRecClickOverview += EditRecordOverview;
            overw.OverviewAddDeviceEvent += OverviewAddDevice;
            overw.OverviewButtonRemoveEvent += OverviewRemoveItems;
            overw.OverviewSelectedItemIdx += OverviewSelectItem;
            rec.RecButtonClickEditEvent += RecordEditEdited;

            current_frame.Content = loginScr;
        }
       
        private void LoginScreenSignin()
        {
            var signedIn = overw;
            current_frame.Content = signedIn;
            
        }

        private void AddDeviceReturnBack()
        {
            var retback = overw;
            current_frame.Content = retback;
        }

        private void EditRecordAddDevice()
        {
            var AddDevice = adddevice;
            current_frame.Content = AddDevice;
        }

        private void EditRecordOverview()
        {
            var overview = overw;
            current_frame.Content = overview;
        }

        private void OverviewAddDevice()
        {
            var AddDevice = adddevice;
            current_frame.Content = AddDevice;
        }

        private void OverviewRemoveItems()
        {
            current_frame.Content = overw;
            current_frame.Refresh();
        }

        private void OverviewSelectItem()
        {
            current_frame.Content = rec;
            rec.neco();
        }

        private void RecordEditEdited()
        {
            current_frame.Content = overw;
        }
    }
}
