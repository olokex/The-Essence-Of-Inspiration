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
    public class CommentRepository : ICommentRepository
    {
        private readonly IDbContextFactory _dbContextFactory;

        public CommentRepository(IDbContextFactory dbContextFactory)
        {
            this._dbContextFactory = dbContextFactory;
        }

        public IEnumerable<CommentListModel> GetAll()
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                return dbContext.Comments.Select(e => CommentMapper.MapCommentEntityToListModel(e)).ToList();
            }
        }

        public CommentDetailModel GetById(Guid id)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Comments.Include(c => c.Author).First(t => t.Id == id);
                return CommentMapper.MapCommentEntityToDetailModel(entity);
            }
        }

        public CommentDetailModel Create(CommentDetailModel model)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = CommentMapper.MapCommentDetailModelToEntity(model);
                dbContext.Comments.Add(entity);
                dbContext.SaveChanges();
                return CommentMapper.MapCommentEntityToDetailModel(entity);
            }
        }

        public void Update(CommentDetailModel model)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = CommentMapper.MapCommentDetailModelToEntity(model);
                dbContext.Comments.Update(entity);
                dbContext.SaveChanges();
            }
        }

        public void Delete(Guid id)
        {
            using (var dbContext = _dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Comments.First(t => t.Id == id);
                dbContext.Comments.Remove(entity);
                dbContext.SaveChanges();
            }
        }

        
    }
}
