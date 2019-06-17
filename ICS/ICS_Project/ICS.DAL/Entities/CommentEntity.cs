using System;
using System.Collections.Generic;

namespace ICS.DAL
{
    public class CommentEntity : BaseEntity
    {
        public DateTime Timestamp { get; set; }
        public UserEntity Author { get; set; }
        public string Content { get; set; }
        public int Likes { get; set; }

        private sealed class CommentEqualityComparer : IEqualityComparer<CommentEntity>
        {
            public bool Equals(CommentEntity x, CommentEntity y)
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
                       x.Likes == y.Likes;
            }

            public int GetHashCode(CommentEntity obj)
            {
                unchecked
                {
                    var hashCode = obj.Id.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Timestamp.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Author.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Content.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Likes.GetHashCode();
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<CommentEntity> CommentEntityComparer { get; } = new CommentEqualityComparer();

    }
}