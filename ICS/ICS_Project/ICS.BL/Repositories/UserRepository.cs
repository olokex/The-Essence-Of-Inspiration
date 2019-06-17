using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ICS.BL.Factories;
using ICS.BL.Mapper;
using ICS.BL.Models;
using ICS.DAL;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;

namespace ICS.BL.Repositories
{
    public class UserRepository : IUserRepository
    {
        private readonly IDbContextFactory _dbContextFactory;

        public UserRepository(IDbContextFactory dbContextFactory)
        {
            this._dbContextFactory = dbContextFactory;
        }

        public IEnumerable<UserListModel> GetAll()
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                return dbContext.Users.Select(e => UserMapper.MapUserEntityToListModel(e)).ToList();
            }
        }

        public UserDetailModel GetById(Guid id)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Users.Include(user => user.TeamList).ThenInclude(teamUser => teamUser.Team).First(t => t.Id == id);
                return UserMapper.MapUserEntityToDetailModel(entity);
            }
        }

        public UserDetailModel Create(UserDetailModel model)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = UserMapper.MapUserDetailModelToEntity(model);
                dbContext.Users.Add(entity);
                dbContext.SaveChanges();
                return UserMapper.MapUserEntityToDetailModel(entity);
            }
        }

        public void Update(UserDetailModel model)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = UserMapper.MapUserDetailModelToEntity(model);
                dbContext.Users.Update(entity);
                dbContext.SaveChanges();
            }
        }

        public void Delete(Guid id)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Users.First(t => t.Id == id);
                dbContext.Users.Remove(entity);
                dbContext.SaveChanges();
            }
        }

        public UserDetailModel GetByName(string name)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Users.Include(user => user.TeamList).ThenInclude(teamUser => teamUser.Team).First(t => t.Name == name);
                return UserMapper.MapUserEntityToDetailModel(entity);
            }
        }
    }
}
