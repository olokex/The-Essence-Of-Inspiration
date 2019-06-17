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

namespace ICS.App.Views
{
    /// <summary>
    /// Interaction logic for SettingKickUser.xaml
    /// </summary>
    public partial class SettingKickUser : Page
    {
        private TeamDetailViewModel teamDetailVM = new TeamDetailViewModel();
        private UserDetailViewModel userDetailVM = new UserDetailViewModel();
        private bool _foundUser;

        public event SettingBase.GoBackToTheTeamChat GoBackToTheTeamChatEvent;
        public event SettingBase.SettingGoToAddUser SettingGoToAddUserEvent;
        public event SettingBase.SettingGoToCreateNewUser SettingGoToCreateNewUserEvent;
        public event SettingBase.Logout LogoutEvent;



        public SettingKickUser()
        {
            InitializeComponent();
        }

        private void SettingsAddUserButtonSettings_Click(object sender, RoutedEventArgs e)
        {
            SettingGoToAddUserEvent();
        }

        private void SettingsAddUserButtonCreateNewUser_Click(object sender, RoutedEventArgs e)
        {
            SettingGoToCreateNewUserEvent();
        }

        private void SettingsAddUserButtonReturn_Click(object sender, RoutedEventArgs e)
        {
            GoBackToTheTeamChatEvent();
        }

        private void SettingsKickUserButtonFindUser_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                userDetailVM.LoadByName(TextBoxKickUserSettingFind.Text);
                TextBlockKickUserFoundUserName.Text = "Username: " + userDetailVM.Model.Name;
                TextBlockKickUserFoundUserDescription.Text = "Description:\n" + userDetailVM.Model.Description;
                _foundUser = true;
            }
            catch
            {
                // ignored
            }
        }

        private void SettingsKickUserrButtonKickUser_Click(object sender, RoutedEventArgs e)
        {
            if (MainWindow.CurrentTeam == null)
            {
                MessageBox.Show("Please select a team");
                return;
            }

            if (_foundUser)
            {
                if (MainWindow.CurrentUser.Name == userDetailVM.Model.Name)
                {
                    MessageBox.Show("Cannot kick yourself from team");
                    return;
                }

                try
                {
                    teamDetailVM.Load(MainWindow.CurrentTeam.Id);
                    teamDetailVM.KickUser(userDetailVM.Model);
                    _foundUser = false;
                    userDetailVM.Clear();
                    MessageBox.Show("Successfully kicked user from team");
                    TextBlockKickUserFoundUserName.Text = "";
                    TextBlockKickUserFoundUserDescription.Text = "";
                }
                catch
                {
                    // ignored
                }
            }
            else
            {
                MessageBox.Show("User not found");
            }
        }

        private void SettingsKickUserLogout_Click(object sender, RoutedEventArgs e)
        {
            LogoutEvent();
        }
    }
}
