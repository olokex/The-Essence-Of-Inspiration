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
    /// Interakční logika pro Overview.xaml
    /// </summary>
    public partial class Overview : Page
    {
        public delegate void ViewAddDevice();
        public event ViewAddDevice OverviewAddDeviceEvent;
        public Overview()
        {
            InitializeComponent();
            OverviewShowItems.ItemsSource = LoadExampleData.DataTest;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }

        private void OverviewButtonClickAdd(object sender, RoutedEventArgs e)
        {
            OverviewAddDeviceEvent();
        }

        public delegate void OverviewButtonRemove();
        public event OverviewButtonRemove OverviewButtonRemoveEvent;
        private void OverviewButtonDeleteClick(object sender, RoutedEventArgs e)
        {
            //MessageBox.Show(OverviewShowItems.SelectedIndex.ToString());
            List<int> indices = new List<int>();
            if (OverviewShowItems.Items.Count > 0)
            {
                for (int i = 0; i < OverviewShowItems.Items.Count; i++)
                {
                    if (((RenameLaterClasses.DeviceType)OverviewShowItems.Items[i]).Checked)
                    {
                        indices.Add(i);
                    }
                }
                for (int i = indices.Count - 1; i >= 0; i--)
                {
                    LoadExampleData.DataTest.RemoveAt(indices[i]);
                }
            }
            OverviewButtonRemoveEvent();
        }

        //public delegate void TestPassArguments(int x);
        public delegate void TestPassArguments();
        public event TestPassArguments OverviewSelectedItemIdx;
        private void OverviewListviewSelectedItem(object sender, SelectionChangedEventArgs e)
        {
            int x = OverviewShowItems.SelectedIndex;
            ((RenameLaterClasses.DeviceType)LoadExampleData.DataTest[x]).IndexSelected = x;
            ((RenameLaterClasses.DeviceType)LoadExampleData.DataTest[x]).WasSelected = true;
            Control.ListViewHasChanged = true;
            OverviewSelectedItemIdx();
        }
    }
}
