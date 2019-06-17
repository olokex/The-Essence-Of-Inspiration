using System;
using ICS.BL.Models;
using Xunit;

namespace ICS.BL.Tests
{
    public class CommentRepositoryTests : IClassFixture<CommentRepositoryTestsFixture>
    {
        private readonly CommentRepositoryTestsFixture fixture;

        public CommentRepositoryTests(CommentRepositoryTestsFixture fixture)
        {
            this.fixture = fixture;
        }

        [Fact]
        public void CreateWithNonExistingItemDoesNotThrow()
        {
            var model = new CommentDetailModel
            {
                Content = "Konichiva",
                Likes = 5,
                Author =  new UserDetailModel
                {
                    Description = "I wanna 30b",
                    Name = "Carl"
                },
                Timestamp = new DateTime(2019, 1, 12, 03, 30, 15),
            };

            var returnedModel = fixture.Repository.Create(model);

            Assert.NotNull(returnedModel);

            fixture.Repository.Delete(returnedModel.Id);
        }

        [Fact]
        public void UpdateExistingItemDoesNotThrow()
        {
            var model = new CommentDetailModel
            {
                Content = "Konichiva",
                Likes = 5,
                Author = new UserDetailModel
                {
                    Description = "I wanna 30b",
                    Name = "Carl"
                },
                Timestamp = new DateTime(2019, 1, 12, 03, 30, 15),
            };

            var returnedModel = fixture.Repository.Create(model);

            fixture.Repository.Update(returnedModel);

            Assert.NotNull(returnedModel);

            fixture.Repository.Delete(returnedModel.Id);
        }

    }
}
