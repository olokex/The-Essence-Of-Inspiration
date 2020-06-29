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
    /// Interakční logika pro LoginScreen.xaml
    /// </summary>
    public partial class LoginScreen : Page
    {
        public delegate void ViewLoginScreen();
        public event ViewLoginScreen ViewLoginScreenEvent;
        public LoginScreen()
        {
            InitializeComponent();
        }

        private void ButtonLogIn_Click(object sender, RoutedEventArgs e)
        {
            ViewLoginScreenEvent();
        }
    }
}
