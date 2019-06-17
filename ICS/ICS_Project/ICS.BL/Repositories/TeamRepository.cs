using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ICS.BL.Factories;
using ICS.BL.Mapper;
using ICS.BL.Models;
using ICS.DAL;
using Microsoft.EntityFrameworkCore;

namespace ICS.BL.Repositories
{
    public class TeamRepository : ITeamRepository
    {
        private readonly IDbContextFactory _dbContextFactory;

        public TeamRepository(IDbContextFactory dbContextFactory)
        {
            this._dbContextFactory = dbContextFactory;
        }

        public IEnumerable<TeamListModel> GetAll()
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                return dbContext.Teams.Select(e => TeamMapper.MapTeamEntityToListModel(e)).ToList();
            }
        }

        public List<TeamListModel> GetByBelonging(UserDetailModel currentUser)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var outputTeams = new List<TeamListModel>();
                var allTeams = dbContext.Teams.Select(e => TeamMapper.MapTeamEntityToListModel(e)).ToList();
                foreach (var team in allTeams)
                {
                    foreach (var userTeam in currentUser.Teams)
                    {
                        if (userTeam.Id==team.Id)
                            outputTeams.Add(team);
                    }
                }

                return outputTeams;
            }
        }

        public TeamDetailModel GetById(Guid id)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Teams.Include(team => team.PostList).ThenInclude( p => p.Author).Include(team => team.UsersList).ThenInclude(teamUser => teamUser.User).First(t => t.Id == id);
                return TeamMapper.MapTeamEntityToDetailModel(entity);
            }
        }

        public TeamDetailModel Create(TeamDetailModel model, UserDetailModel user)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = TeamMapper.MapTeamDetailModelToEntity(model);
                dbContext.Teams.Add(entity);
                dbContext.SaveChanges();
                model.Users = new List<UserListModel>();
                
                AddUserToTeam(model, user);
                return model;
            }
        }

        public void Update(TeamDetailModel model)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = TeamMapper.MapTeamDetailModelToEntity(model);
                dbContext.Teams.Update(entity);
                dbContext.SaveChanges();
            }
        }

        public void Delete(Guid id)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Teams.First(t => t.Id == id);
                if (entity != null) dbContext.Teams.Remove(entity);
                dbContext.SaveChanges();
            }
        }

        public TeamDetailModel AddUserToTeam(TeamDetailModel team, UserDetailModel user)
        {
            team.Users.Add(UserMapper.MapDetailModelToListModel(user));
            user.Teams.Add(TeamMapper.MapTeamDetailModelToListModel(team));
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                dbContext.Set<UserTeam>().Add(new UserTeam
                {
                    TeamId = team.Id,
                    UserId = user.Id
                });
                dbContext.SaveChanges();
            }
            
            return team;
        }

        public TeamDetailModel RemoveUserFromTeam(TeamDetailModel team, UserDetailModel user)
        {
            team.Users.Remove(UserMapper.MapDetailModelToListModel(user));
            user.Teams.Remove(TeamMapper.MapTeamDetailModelToListModel(team));
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                dbContext.Set<UserTeam>().Remove(new UserTeam
                {
                    TeamId = team.Id,
                    UserId = user.Id
                });
                dbContext.SaveChanges();
            }

            return team;
        }

        public TeamDetailModel AddPostToTeam(TeamDetailModel team, PostDetailModel post)
        {
            team.Posts.Add(post);
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var teamInDb = dbContext.Teams.Include(ad => ad.PostList).FirstOrDefault(i => i.Id == team.Id);
                if (teamInDb != null) teamInDb.PostList.Add(PostMapper.MapPostDetailModelToEntity(post));
                dbContext.Attach(UserMapper.MapUserDetailModelToEntity(post.Author));
                dbContext.SaveChanges();
            }

            return team;
        }
    }
}
