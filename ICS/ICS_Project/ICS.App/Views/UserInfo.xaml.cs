using System;
using System.Collections.Generic;
using System.ComponentModel;
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
    /// Interaction logic for UserInfo.xaml
    /// </summary>
    public partial class UserInfo : Page
    {
        public event SettingBase.TeamChatCreateNewTeam TeamChatCreateNewTeamEvent;
        public event SettingBase.SettingGoToAddUser SettingGoToAddUserEvent;
        public event SettingBase.GoBackToTheTeamChat GoBackToTheTeamChatEvent;

        public TeamListViewModel teamsVM = new TeamListViewModel();
        public TeamDetailViewModel teamDetailVM = new TeamDetailViewModel();
        public UserInfo()
        {
            InitializeComponent();
        }

        private void ButtonCreateNewTeam_OnClick(object sender, RoutedEventArgs e)
        {
            TeamChatCreateNewTeamEvent();
        }

        private void ButtonUserInfoGoToAddNewUserToTeam_OnClick(object sender, RoutedEventArgs e)
        {
            SettingGoToAddUserEvent();
        }

        private void ButtonUserCancel_OnClick(object sender, RoutedEventArgs e)
        {
            GoBackToTheTeamChatEvent();
        }


        public void refresh()
        {
            TextBlockTeamNameUserInfo.Text = MainWindow.CurrentTeam.TeamName;
            TextBlockTeamDescriptionUserInfo.Text = MainWindow.CurrentTeam.Description;

            teamsVM.Load(MainWindow.CurrentUser);
            ListViewUserInfoTeams.ItemsSource = teamsVM.Teams;
            ListViewUserInfoMembers.ItemsSource = MainWindow.CurrentTeam.Users;

            TextBlockUserInfoUserName.Text = TeamChat.selectedUser.Name;
            TextBlockUserInfoDescription.Text = TeamChat.selectedUser.Description;
            UserDetailViewModel selectedUserDetailVm = new UserDetailViewModel();
            selectedUserDetailVm.LoadByName(TeamChat.selectedUser.Name);
            ListViewUsersTeams.ItemsSource = selectedUserDetailVm.Model.Teams;

            TextBlockActivity.Text = MainWindow.CurrentUser.Name == TeamChat.selectedUser.Name ? "Online" : "Offline";
        }

        private void ListViewUserInfoTeams_OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count > 0)
            {
                TeamListModel selected = (TeamListModel)e.AddedItems[0];
                teamDetailVM.Load(selected.Id);
                MainWindow.CurrentTeam = teamDetailVM.Model;
                GoBackToTheTeamChatEvent();
            }

            refresh();
        }

        private void ListViewUserInfoMembers_OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count > 0)
            {
                TeamChat.selectedUser = (UserListModel)e.AddedItems[0];
                refresh();
            }
        }
    }
}
