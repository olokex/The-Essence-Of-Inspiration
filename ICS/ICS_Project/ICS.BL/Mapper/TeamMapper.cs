using System.Collections.Generic;
using ICS.BL.Models;
using ICS.DAL;

namespace ICS.BL.Mapper
{
    internal static class TeamMapper
    {
        public static TeamListModel MapTeamEntityToListModel(TeamEntity team)
        {
            return new TeamListModel
            {
                Id = team.Id,
                TeamName = team.TeamName,
                Description = team.Description
            };
        }
        public static TeamDetailModel MapTeamEntityToDetailModel(TeamEntity team)
        {
            List<PostDetailModel> posts = new List<PostDetailModel>();
            if (team.PostList != null)
                foreach (var item in team.PostList)
                {
                    posts.Add(PostMapper.MapPostEntityToDetailModel(item));
                }

            List<UserListModel> users = new List<UserListModel>();
            if (team.UsersList != null)
                foreach (var item in team.UsersList)
                {
                    users.Add(UserMapper.MapUserEntityToListModel(item.User));
                }

            List<UserListModel> mods = new List<UserListModel>();
            if (team.ModsList != null)
                foreach (var item in team.ModsList)
                {//Příprava na rozšíření o mody
                    mods.Add(UserMapper.MapUserEntityToListModel(item));
                }

            return new TeamDetailModel
            {
                Id = team.Id,
                TeamName = team.TeamName,
                Description = team.Description,
                Mods = mods,
                Users = users,
                Posts = posts

            };
        }
        public static TeamEntity MapTeamDetailModelToEntity(TeamDetailModel team)
        {

            List<PostEntity> posts = new List<PostEntity>();
            if (team.Posts != null)
                foreach (var item in team.Posts)
                {
                    posts.Add(PostMapper.MapPostDetailModelToEntity(item));
                }

            List<UserEntity> mods = new List<UserEntity>();
            if (team.Mods != null)
                foreach (var item in team.Mods)
                {//Příprava na rozšíření o mody
                    mods.Add(UserMapper.MapUserListModelToEntity(item));
                }

            return new TeamEntity
            {
                Id = team.Id,
                TeamName = team.TeamName,
                Description = team.Description,
                PostList = posts,
                ModsList = mods
            
            };
        }

        public static TeamListModel MapTeamDetailModelToListModel(TeamDetailModel team)
        {
            return new TeamListModel
            {
                Id = team.Id,
                TeamName = team.TeamName,
                Description = team.Description
            };
        }
    }
}
