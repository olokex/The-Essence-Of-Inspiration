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
    /// Interaction logic for SettingCreateNewUser.xaml
    /// </summary>
    public partial class SettingCreateNewUser : Page
    {
        public event SettingBase.GoBackToTheTeamChat GoBackToTheTeamChatEvent;
        public event SettingBase.SettingGoToKickUser SettingGoTuKickUserEvent;
        public event SettingBase.SettingGoToAddUser SettingGoToAddUserEvent;
        public event SettingBase.Logout LogoutEvent;
        private LoginViewModel vm = new LoginViewModel();

        public SettingCreateNewUser()
        {
            InitializeComponent();
        }

        private void SettingsAddUserButtonSettings_Click(object sender, RoutedEventArgs e)
        {
            SettingGoToAddUserEvent();
        }

        private void SettingsAddUserButtonKickUser_Click(object sender, RoutedEventArgs e)
        {
            SettingGoTuKickUserEvent();
        }

        private void SettingsAddUserButtonGoBack_Click(object sender, RoutedEventArgs e)
        {
            GoBackToTheTeamChatEvent();
        }

        private void SettingsCreateNewUserButtonCreateUser_Click(object sender, RoutedEventArgs e)
        {
            if (SettingsCreateNewUserPasswordBoxPassword.Password == "")
            {
                MessageBox.Show("Password cannot be empty");
                return;
            }
            if (!Regex.IsMatch(SettingsCreateNewUserTextBoxUserName.Text, @"^([\w\.\-]+)@([\w\-]+)((\.(\w){2,3})+)$"))
            {
                MessageBox.Show("Name must be valid email address");
                return;
            }
            try
            {
                vm.CreateAccount(SettingsCreateNewUserTextBoxUserName.Text, SettingsCreateNewUserPasswordBoxPassword.Password,
                    SettingsCreateNewUserPasswordBoxPasswordRepeat.Password, SettingsCreateNewUserDescription.Text);
                MessageBox.Show("User successfully created");
            }
            catch
            {
                // ignored
            }
        }

        private void SettingsCreateUserLogout_Click(object sender, RoutedEventArgs e)
        {
            LogoutEvent();
        }
    }
}
