using System.Collections.Generic;
using ICS.BL.Models;
using ICS.DAL;

namespace ICS.BL.Mapper
{
    internal static class UserMapper
    {
        public static UserListModel MapUserEntityToListModel(UserEntity user)
        {
            return new UserListModel
            {
                Id = user.Id,
                Name = user.Name,
                Description = user.Description
            };
        }
        public static UserDetailModel MapUserEntityToDetailModel(UserEntity user)
        {
            List<TeamListModel> teams = new List<TeamListModel>();
            if (user.TeamList != null)
                foreach (var item in user.TeamList)
                {
                    teams.Add(TeamMapper.MapTeamEntityToListModel(item.Team));
                }

            return new UserDetailModel
            {
                Id = user.Id,
                Name = user.Name,
                HashedPassword = user.HashedPassword,
                Description = user.Description,
                Teams = teams
            };
        }
        public static UserEntity MapUserDetailModelToEntity(UserDetailModel user)
        {
            return new UserEntity
            {
                Id = user.Id,
                Name = user.Name,
                HashedPassword = user.HashedPassword,
                Description = user.Description
            };
        }

        public static UserListModel MapDetailModelToListModel(UserDetailModel user)
        {
            return new UserListModel
            {
                Id = user.Id,
                Name = user.Name,
                Description = user.Description
            };
        }

        public static UserEntity MapUserListModelToEntity(UserListModel user)
        {
            return new UserEntity
            {
                Id = user.Id,
                Name = user.Name,
                Description = user.Description
            };
        }
    }
}
