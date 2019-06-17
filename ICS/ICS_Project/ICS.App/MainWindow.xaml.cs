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
using ICS.App.Views;
using ICS.BL.Models;

namespace ICS.App
{
    /// <summary>
    /// Interakční logika pro MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        TeamChat window = new TeamChat();

        UserInfo userInfo = new UserInfo();

        SettingAddUser settingsAddUser = new SettingAddUser();

        SettingCreateNewUser settingCreateNewUser = new SettingCreateNewUser();

        SettingKickUser settingKickUser = new SettingKickUser();

        LoginScreen loginScreen = new LoginScreen();
        public static UserDetailModel CurrentUser { get; set; }
        public static TeamDetailModel CurrentTeam { get; set; }
        public MainWindow()
        {
            InitializeComponent();
            load_frame.NavigationUIVisibility = NavigationUIVisibility.Hidden;

            loginScreen.LoginScreenCreateNewUserEvent += LoginScreenCreateNewUserEvent;
            loginScreen.LoginScreenAfterEvent += LoginScreenAfterEvent;
            
            userInfo.GoBackToTheTeamChatEvent += GoBackToTheTeamChatEvent;
            userInfo.TeamChatCreateNewTeamEvent += TeamChatCreateNewTeamEvent;
            userInfo.SettingGoToAddUserEvent += SettingGoToAddUserEvent;


            load_frame.Content = loginScreen;
        }

        private void LogoutEvent()
        {
            CurrentTeam = null;
            CurrentUser = null;
            loginScreen = new LoginScreen();
            loginScreen.LoginScreenCreateNewUserEvent += LoginScreenCreateNewUserEvent;
            loginScreen.LoginScreenAfterEvent += LoginScreenAfterEvent;
            load_frame.Content = loginScreen;
        }

        private void TeamChatUserInfoEvent()
        {
            load_frame.Content = userInfo;
            userInfo.refresh();
        }

        private void SettingGoToAddUserEvent()
        {
            settingsAddUser = new SettingAddUser();
            settingsAddUser.GoBackToTheTeamChatEvent += GoBackToTheTeamChatEvent;
            settingsAddUser.SettingGoTuKickUserEvent += SettingGoTuKickUserEvent;
            settingsAddUser.SettingGoToCreateNewUserEvent += SettingGoToCreateNewUserEvent;
            settingsAddUser.LogoutEvent += LogoutEvent;
            load_frame.Content = settingsAddUser;
        }

        private void SettingGoToCreateNewUserEvent()
        {
            settingCreateNewUser = new SettingCreateNewUser();
            settingCreateNewUser.GoBackToTheTeamChatEvent += GoBackToTheTeamChatEvent;
            settingCreateNewUser.SettingGoTuKickUserEvent += SettingGoTuKickUserEvent;
            settingCreateNewUser.SettingGoToAddUserEvent += SettingGoToAddUserEvent;
            settingCreateNewUser.LogoutEvent += LogoutEvent;
            load_frame.Content = settingCreateNewUser;
        }

        private void SettingGoTuKickUserEvent()
        {
            settingKickUser = new SettingKickUser();
            settingKickUser.GoBackToTheTeamChatEvent += GoBackToTheTeamChatEvent;
            settingKickUser.SettingGoToAddUserEvent += SettingGoToAddUserEvent;
            settingKickUser.SettingGoToCreateNewUserEvent += SettingGoToCreateNewUserEvent;
            settingKickUser.LogoutEvent += LogoutEvent;
            load_frame.Content = settingKickUser;
        }

        private void GoBackToTheTeamChatEvent()
        {

            load_frame.Content = window;
            window.refresh();
        }

        private void LoginScreenAfterEvent()
        {
            window = new TeamChat();
            window.TeamChatCreateNewTeamEvent += TeamChatCreateNewTeamEvent;
            window.SettingGoToAddUserEvent += SettingGoToAddUserEvent;
            window.TeamChatUserInfoEvent += TeamChatUserInfoEvent;
            load_frame.Content = window;
            window.refresh();
        }


        private void LoginScreenCreateNewUserEvent()
        {
            var loginCreate = new LoginScreenCreate();
            load_frame.Content = loginCreate;
            loginCreate.LoginScreenCreateNewUserAfterEvent += LoginScreenCreateNewUserAfterEvent;
        }

        private void LoginScreenCreateNewUserAfterEvent()
        {
            window = new TeamChat();
            window.TeamChatCreateNewTeamEvent += TeamChatCreateNewTeamEvent;
            window.SettingGoToAddUserEvent += SettingGoToAddUserEvent;
            window.TeamChatUserInfoEvent += TeamChatUserInfoEvent;
            load_frame.Content = window;
        }

        private void TeamChatCreateNewTeamEvent()
        {
            var teamCreate = new CreateNewTeam();
            load_frame.Content = teamCreate;
            teamCreate.CreateNewTeamAfterEvent += CreateNewTeamAfterEvent;
        }

        private void CreateNewTeamAfterEvent()
        {
            load_frame.Content = window;
            window.refresh();
        }
    }
}
