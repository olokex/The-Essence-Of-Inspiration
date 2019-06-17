using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ICS.BL.Factories;
using ICS.BL.Mapper;
using ICS.BL.Models;
using Microsoft.EntityFrameworkCore;

namespace ICS.BL.Repositories
{
    public class PostRepository : IPostRepository
    {
        private readonly IDbContextFactory _dbContextFactory;

        public PostRepository(IDbContextFactory dbContextFactory)
        {
            this._dbContextFactory = dbContextFactory;
        }

        public IEnumerable<PostListModel> GetAll()
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                return dbContext.Posts.Select(e => PostMapper.MapPostEntityToListModel(e)).ToList();
            }
        }
        public IEnumerable<PostListModel> GetByBelonging(TeamDetailModel team)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                return null;
            }
        }


        public PostDetailModel GetById(Guid id)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Posts.Include(p => p.Author).Include(p=> p.Comments).ThenInclude(c => c.Author).First(t => t.Id == id);
                return PostMapper.MapPostEntityToDetailModel(entity);
            }
        }

        public PostDetailModel Create(PostDetailModel model)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = PostMapper.MapPostDetailModelToEntity(model);
                dbContext.Posts.Add(entity);
                dbContext.SaveChanges();
                return PostMapper.MapPostEntityToDetailModel(entity);
            }
        }

        public void Update(PostDetailModel model)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = PostMapper.MapPostDetailModelToEntity(model);
                dbContext.Posts.Update(entity);
                dbContext.SaveChanges();
            }
        }

        public void Delete(Guid id)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Posts.First(t => t.Id == id);
                dbContext.Posts.Remove(entity);
                dbContext.SaveChanges();
            }
        }
        public PostDetailModel AddCommentToPost(PostDetailModel post, CommentDetailModel comment)
        {
            post.Comments.Add(comment);
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                dbContext.Attach(UserMapper.MapUserDetailModelToEntity(comment.Author));
                if (comment.Author.Id != post.Author.Id)
                    dbContext.Attach(UserMapper.MapUserDetailModelToEntity(post.Author));

                var postInDb = dbContext.Posts.Include(p => p.Comments).FirstOrDefault(item => item.Id == post.Id);
                if (postInDb != null) postInDb.Comments.Add(CommentMapper.MapCommentDetailModelToEntity(comment));
                dbContext.SaveChanges();
            }
            return post;
        }
    }
}
