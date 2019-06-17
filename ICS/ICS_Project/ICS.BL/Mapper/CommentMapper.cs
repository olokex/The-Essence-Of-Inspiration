using ICS.BL.Models;
using ICS.DAL;

namespace ICS.BL.Mapper
{
    public static class CommentMapper
    {
        public static CommentListModel MapCommentEntityToListModel(CommentEntity comment)
        {
            return new CommentListModel
            {
                Id = comment.Id,
                Timestamp = comment.Timestamp,
                Content = comment.Content,
                AuthorName = comment.Author.Name
            };
        }
        public static CommentDetailModel MapCommentEntityToDetailModel(CommentEntity comment)
        {
            return new CommentDetailModel
            {
                Id = comment.Id,
                Timestamp = comment.Timestamp,
                Content = comment.Content,
                Author = UserMapper.MapUserEntityToDetailModel(comment.Author),
                Likes = comment.Likes
            };
        }
        public static CommentEntity MapCommentDetailModelToEntity(CommentDetailModel comment)
        {
            return new CommentEntity
            {
                Id = comment.Id,
                Timestamp = comment.Timestamp,
                Content = comment.Content,
                Likes = comment.Likes,
                Author = UserMapper.MapUserDetailModelToEntity(comment.Author)
            };
        }
        public static CommentListModel MapCommentDetailModelToListModel(CommentDetailModel comment)
        {
            return new CommentListModel
            {
                Id = comment.Id,
                Timestamp = comment.Timestamp,
                Content = comment.Content,
                AuthorName = comment.Author.Name
            };
        }
    }
}
