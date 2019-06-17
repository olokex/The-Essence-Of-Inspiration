using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ICS.DAL
{
    public class PostEntity : BaseEntity
    {
        public DateTime Timestamp { get; set; }
        public UserEntity Author { get; set; }
        public string Content { get; set; }
        public virtual ICollection<CommentEntity> Comments { get; set; }
        public int Likes { get; set; }
        public string Title { get; set; }

        private sealed class PostEqualityComparer : IEqualityComparer<PostEntity>
        {
            public bool Equals(PostEntity x, PostEntity y)
            {
                if (ReferenceEquals(x, y))
                {
                    return true;
                }

                if (ReferenceEquals(x, null))
                {
                    return false;
                }

                if (ReferenceEquals(y, null))
                {
                    return false;
                }

                if (x.GetType() != y.GetType())
                {
                    return false;
                }

                return x.Id.Equals(y.Id) &&
                       x.Timestamp == y.Timestamp &&
                       UserEntity.UserEntityComparer.Equals(x.Author, y.Author) &&
                       string.Equals(x.Content, y.Content) &&
                       string.Equals(x.Title, y.Title) &&
                       //x.Comments.OrderBy(comment => comment.Id).SequenceEqual(y.Comments.OrderBy(comment => comment.Id), CommentEntity.CommentEntityComparer) &&
                       x.Likes == y.Likes;
            }

            public int GetHashCode(PostEntity obj)
            {
                unchecked
                {
                    var hashCode = obj.Id.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Timestamp.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Author.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Content.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Comments.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Likes.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Title.GetHashCode();
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<PostEntity> PostEntityComparer { get; } = new PostEqualityComparer();

    }
}
