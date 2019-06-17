using System;
using System.Collections.Generic;
using System.Text;
using ICS.BL.Models;

namespace ICS.BL.Repositories
{
    public interface ITeamRepository
    {
        IEnumerable<TeamListModel> GetAll();
        List<TeamListModel> GetByBelonging(UserDetailModel currentUser);

        TeamDetailModel GetById(Guid id);
        TeamDetailModel Create(TeamDetailModel model, UserDetailModel user);
        void Update(TeamDetailModel model);
        void Delete(Guid id);
        TeamDetailModel AddUserToTeam(TeamDetailModel team, UserDetailModel user);
        TeamDetailModel RemoveUserFromTeam(TeamDetailModel team, UserDetailModel user);
        TeamDetailModel AddPostToTeam(TeamDetailModel team, PostDetailModel post);
    }
}
