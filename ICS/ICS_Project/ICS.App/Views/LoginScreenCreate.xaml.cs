using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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

namespace ICS.App.Views
{
    /// <summary>
    /// Interaction logic for LoginScreenCreate.xaml
    /// </summary>
    public partial class LoginScreenCreate : Page
    {

        public delegate void LoginScreenCreateNewUserAfter();
        public event LoginScreenCreateNewUserAfter LoginScreenCreateNewUserAfterEvent;
        private LoginViewModel vm = new LoginViewModel();
        public LoginScreenCreate()
        {
            InitializeComponent();
        }


        private void ButtonCreateAccountCreate_OnClick(object sender, RoutedEventArgs e)
        {
            if (TextBoxPasswordCreate.Password == "")
            {
                MessageBox.Show("Password cannot be empty");
                return;
            }
            if(!Regex.IsMatch(TextBoxCreate.Text, @"^([\w\.\-]+)@([\w\-]+)((\.(\w){2,3})+)$"))
            {
                MessageBox.Show("Name must be valid email address");
                return;
            }
            try
            {
                vm.CreateAccount(TextBoxCreate.Text, TextBoxPasswordCreate.Password,
                    TextBoxPasswordCreateRepeat.Password, LoginScreenCreateTextBoxUserDescription.Text);
                LoginScreenCreateNewUserAfterEvent();
                MainWindow.CurrentUser = vm.currentUser;
            }
            catch
            {
                // ignored
            }
        }
    }
}
