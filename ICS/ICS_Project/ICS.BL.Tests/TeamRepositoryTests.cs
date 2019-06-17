using System;
using System.Collections.Generic;
using ICS.BL.Models;
using Xunit;

namespace ICS.BL.Tests
{
    public class TeamRepositoryTests : IClassFixture<TeamRepositoryTestsFixture>
    {
        private readonly TeamRepositoryTestsFixture fixture;

        public TeamRepositoryTests(TeamRepositoryTestsFixture fixture)
        {
            this.fixture = fixture;
        }

        [Fact]
        public void CreateTeamDetailModel()
        {
            var model = new TeamDetailModel
            {
                TeamName = "users",
                Description = "OP team",
                Users = new List<UserListModel>(),
                Mods = new List<UserListModel>(),
                Posts = new List<PostDetailModel>()

            };

            var user = new UserDetailModel
            {
                Description = "mam rad ics",
                Name = "Patrick",
                Teams = new List<TeamListModel>()
            };

            var returnedModel = fixture.Repository.Create(model, user);

            Assert.NotNull(returnedModel);
            fixture.Repository.Delete(returnedModel.Id);
        }

        [Fact]
        public void AddUserToExistingTeam()
        {
            var model = new TeamDetailModel
            {
                TeamName = "users",
                Description = "OP team",
                Users = new List<UserListModel>(),
                Mods = new List<UserListModel>(),
                Posts = new List<PostDetailModel>()

            };

            var user = new UserDetailModel
            {
                Description = "mam rad ics",
                Name = "Patrick",
                Teams = new List<TeamListModel>()
            };

            var returnedModel = fixture.Repository.Create(model,user);

            Assert.NotNull(returnedModel);

            

            var user2 = new UserDetailModel
            {
                Description = "popisek",
                Name = "Spongebob",
                Teams = new List<TeamListModel>()
            };
            var returned = fixture.Repository.AddUserToTeam(model, user2);
            Assert.NotNull(returned);
            fixture.Repository.Delete(returned.Id);
        }
    }
}
