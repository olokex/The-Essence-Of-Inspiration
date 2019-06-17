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
using ICS.App.ViewModels;
using ICS.BL.Models;

namespace ICS.App.Views
{
    /// <summary>
    /// Interaction logic for LoginScreen2.xaml
    /// </summary>
    public partial class LoginScreen : Page
    {
        public delegate void LoginScreenCreateNewUser();
        public event LoginScreenCreateNewUser LoginScreenCreateNewUserEvent;

        public delegate void LoginScreenAfter();
        public event LoginScreenAfter LoginScreenAfterEvent;
        private LoginViewModel vm = new LoginViewModel();
        public LoginScreen()
        {
            InitializeComponent();
        }

        private void ButtonCreateAccount_OnClick(object sender, RoutedEventArgs e)
        {      
            LoginScreenCreateNewUserEvent();
        }

        private void ButtonLogIn_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                vm.login(TextBoxLogin.Text,TextBoxPassword.Password);
                MainWindow.CurrentUser = vm.currentUser;
                LoginScreenAfterEvent();
            }
            catch
            {
                // ignored
            }   
        }
    }
}
