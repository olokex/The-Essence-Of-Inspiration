using System;
using System.Collections.Generic;
using ICS.BL.Models;
using Xunit;

namespace ICS.BL.Tests
{
    public class PostRepositoryTests : IClassFixture<PostRepositoryTestsFixture>
    {
        private readonly PostRepositoryTestsFixture fixture;

        public PostRepositoryTests(PostRepositoryTestsFixture fixture)
        {
            this.fixture = fixture;
        }

        [Fact]
        public void CreatePostDetailModel()
        {
            var model = new PostDetailModel
            {
                Timestamp = new DateTime(2019, 1, 12, 03, 30, 15),
                Content = "Hi C#",
                Likes = 5,
                Author = new UserDetailModel
                {
                    Name = "John",
                    Description = "Programmer"
                },
                Title = "asdf",
                Comments = new List<CommentDetailModel>()
            };

            var returnedModel = fixture.Repository.Create(model);

            Assert.NotNull(returnedModel);

            fixture.Repository.Delete(returnedModel.Id);
        }
    }
}
