using System;
using System.Collections.Generic;
using ICS.BL.Models;
using Xunit;

namespace ICS.BL.Tests
{
    public class UserRepositoryTests : IClassFixture<UserRepositoryTestsFixture>
    {
        private readonly UserRepositoryTestsFixture fixture;

        public UserRepositoryTests(UserRepositoryTestsFixture fixture)
        {
            this.fixture = fixture;
        }

        [Fact]
        public void CreateUserDetailModel()
        {
            var model = new UserDetailModel
            {
                Name = "Hanry@email.com",
                Description = "Pro gamer",
                HashedPassword = "nani",
                Teams = new List<TeamListModel>()
            };

            var returnedModel = fixture.Repository.Create(model);

            Assert.NotNull(returnedModel);

            fixture.Repository.Delete(returnedModel.Id);
        }
    }
}
