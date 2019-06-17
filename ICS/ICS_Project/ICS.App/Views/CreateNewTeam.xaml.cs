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
    /// Interaction logic for CreateNewTeam2.xaml
    /// </summary>
    public partial class CreateNewTeam : Page
    {
        public delegate void CreateNewTeamAfter();
        public event CreateNewTeamAfter CreateNewTeamAfterEvent;
        private TeamDetailViewModel vm = new TeamDetailViewModel();
        public CreateNewTeam()
        {
            InitializeComponent();
        }


        private void ButtonCreateTeam_OnClick(object sender, RoutedEventArgs e)
        {
            vm.Model.TeamName = TextBoxTeamName.Text;
            vm.Model.Description = TextBoxTeamDescription.Text;
            vm.Create(MainWindow.CurrentUser);
            
            CreateNewTeamAfterEvent();
        }
    }
}
