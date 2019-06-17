using System.Collections.Generic;
using System.Collections.ObjectModel;
using ICS.BL.Models;
using ICS.DAL;

namespace ICS.BL.Mapper
{
    public static class PostMapper
    {
        public static PostListModel MapPostEntityToListModel(PostEntity post)
        {
            return new PostListModel
            {
                Id = post.Id,
                Timestamp = post.Timestamp,
                Content = post.Content,
                AuthorName = post.Author.Name,
                Title = post.Title
            };                        
        }                             
        public static PostDetailModel MapPostEntityToDetailModel(PostEntity post)
        {
            List<CommentDetailModel> comments = new List<CommentDetailModel>();
            if (post.Comments != null)
                foreach (var item in post.Comments)
                {
                    comments.Add(CommentMapper.MapCommentEntityToDetailModel(item));
                }

            return new PostDetailModel
            {
                Id = post.Id,
                Timestamp = post.Timestamp,
                Content = post.Content,
                Author = UserMapper.MapUserEntityToDetailModel(post.Author),
                Likes = post.Likes,
                Title = post.Title,
                Comments = comments
            };                        
        }                             
        public static PostEntity MapPostDetailModelToEntity(PostDetailModel post)
        {
            List<CommentEntity> comments = new List<CommentEntity>();
            foreach (var item in post.Comments)
            {
                comments.Add(CommentMapper.MapCommentDetailModelToEntity(item));
            }

            return new PostEntity     
            {               
                Id = post.Id,
                Timestamp = post.Timestamp,
                Content = post.Content,
                Title = post.Title,
                Likes = post.Likes,
                Comments = new Collection<CommentEntity>(comments),
                Author = UserMapper.MapUserDetailModelToEntity(post.Author)
            };
        }

        public static PostListModel MapPostDetailModelToListModel(PostDetailModel post)
        {
            return new PostListModel
            {
                Id = post.Id,
                Timestamp = post.Timestamp,
                Content = post.Content,
                AuthorName = post.Author.Name,
                Title = post.Title
            };
        }
    }
}
