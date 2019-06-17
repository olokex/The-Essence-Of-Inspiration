using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.EntityFrameworkCore;
using Xunit;
using System.Linq;

namespace ICS.DAL.Tests
{
    public class ICSDbContextTests : IClassFixture<ICSDbContextTestsClassSetup>
    {
        public ICSDbContextTests(ICSDbContextTestsClassSetup TestContext)
        {
            _testContext = TestContext;
        }

        private readonly ICSDbContextTestsClassSetup _testContext;
        
        [Fact]
        public void AddUserTest()
        {
            var user = new UserEntity
            {
                Name = "Deuce",
                Description = "Student"
            };

            _testContext.ICSDbContextSUT.Users.Add(user);
            _testContext.ICSDbContextSUT.SaveChanges();

            using (var dbx = _testContext.CreateICSDbContext())
            {
                var RetrievedUser = dbx.Users.First(entity => entity.Id == user.Id);
                Assert.Equal(user, RetrievedUser, UserEntity.UserEntityComparer);
            }
        }

        [Fact]
        public void AddTeamTest()
        {
            var team = new TeamEntity
            {
                TeamName = "SquadNumberOne",
                Description = "VUT students"
            };

            _testContext.ICSDbContextSUT.Teams.Add(team);
            _testContext.ICSDbContextSUT.SaveChanges();

            using (var dbx = _testContext.CreateICSDbContext())
            {
                var RetrievedTeam = dbx.Teams.First(entity => entity.Id == team.Id);
                Assert.Equal(team, RetrievedTeam, TeamEntity.TeamEntityComparer);
            }
        }

        [Fact]
        public void AddPostTest()
        {
            var post = new PostEntity
            {
                Title = "Title first test",
                Timestamp = new DateTime(2019, 1, 24, 14, 30, 15),
                Author = new UserEntity
                {
                    Name = "Danny",
                    Description = "Student"
                },
                Content = "Doesn’t expecting the unexpected make the unexpected expected?",
                Likes = 125
            };

            _testContext.ICSDbContextSUT.Posts.Add(post);
            _testContext.ICSDbContextSUT.SaveChanges();

            using (var dbx = _testContext.CreateICSDbContext())
            {
                var RetrievedPost = dbx.Posts
                    .Include(entity => entity.Author)
                    .First(entity => entity.Id == post.Id);
                Assert.Equal(post, RetrievedPost, PostEntity.PostEntityComparer);
            }
        }

        [Fact]
        public void AddCommentTest()
        {
            var comment = new CommentEntity
            {
                Timestamp = new DateTime(2019, 1, 12, 14, 30, 15),
                Author = new UserEntity
                {
                    Name = "Anna",
                    Description = "Student"
                },
                Content = "C# je hrozně super.",
                Likes = 100
            };

            _testContext.ICSDbContextSUT.Comments.Add(comment);
            _testContext.ICSDbContextSUT.SaveChanges();

            using (var dbx = _testContext.CreateICSDbContext())
            {
                var RetrievedComment = dbx.Comments
                    .Include(entity => entity.Author)
                    .First(entity => entity.Id == comment.Id);
                Assert.Equal(comment, RetrievedComment, CommentEntity.CommentEntityComparer);
            }
        }
    }
}
