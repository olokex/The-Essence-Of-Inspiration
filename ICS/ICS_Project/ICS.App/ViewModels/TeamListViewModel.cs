using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICS.BL.Factories;
using ICS.BL.Models;
using ICS.BL.Repositories;

namespace ICS.App.ViewModels
{
    public class TeamListViewModel : ViewModelBase
    {
        private readonly ITeamRepository repository = new TeamRepository(new DbContextFactory());

        public List<TeamListModel> Teams { get; set; }

        public void Load(UserDetailModel userBelongingToTeam)
        {
            Teams = repository.GetByBelonging(userBelongingToTeam);
        }
    }
}
