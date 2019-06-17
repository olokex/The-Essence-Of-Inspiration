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
using ICS.App.Exceptions;
using ICS.App.Properties;
using ICS.App.ViewModels;

namespace ICS.App.Views
{
    /// <summary>
    /// Interaction logic for SettingAddUser.xaml
    /// </summary>
    public partial class SettingAddUser : Page
    {
        private TeamDetailViewModel teamDetailVM = new TeamDetailViewModel();
        private UserDetailViewModel userDetailVM = new UserDetailViewModel();
        private bool _foundUser;

        public event SettingBase.GoBackToTheTeamChat GoBackToTheTeamChatEvent;
        public event SettingBase.SettingGoToKickUser SettingGoTuKickUserEvent;
        public event SettingBase.SettingGoToCreateNewUser SettingGoToCreateNewUserEvent;
        public event SettingBase.Logout LogoutEvent;

        public SettingAddUser()
        {
            InitializeComponent();
        }

        private void SettingsAddUserButtonFindUser_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                userDetailVM.LoadByName(TextBoxUsernameFindSettindAddUserToTeam.Text);
                //MessageBox.Show("User found");
                TextBlockAddUserFoundUserName.Text = "Username: "+userDetailVM.Model.Name;
                TextBlockAddUserFoundUserDescription.Text = "Description:\n"+userDetailVM.Model.Description;
                _foundUser = true;
            }
            catch
            {
                // ignored
            }
        }

        private void SettingsAddUserButtonAddUser_Click(object sender, RoutedEventArgs e)
        {
            if (MainWindow.CurrentTeam == null)
            {
                MessageBox.Show("Please select a team");
                return;
            }
            if (_foundUser)
            {
                try
                {
                    teamDetailVM.Load(MainWindow.CurrentTeam.Id);
                    teamDetailVM.AddUser(userDetailVM.Model);
                    _foundUser = false;
                    userDetailVM.Clear();
                    MessageBox.Show("Successfully added user to team");
                    TextBlockAddUserFoundUserName.Text = "";
                    TextBlockAddUserFoundUserDescription.Text = "";
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

        private void SettingsAddUserButtonReturn_Click(object sender, RoutedEventArgs e)
        {
            GoBackToTheTeamChatEvent();
        }

        private void SettingsAddUserButtonKickUser_Click(object sender, RoutedEventArgs e)
        {
            SettingGoTuKickUserEvent();
        }

        private void SettingsAddUserButtonCreateNewUser_Click(object sender, RoutedEventArgs e)
        {
            SettingGoToCreateNewUserEvent();
        }

        private void SettingsAddUserLogout_Click(object sender, RoutedEventArgs e)
        {
            LogoutEvent();
        }
    }
}
