using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace ICS.DAL
{
    public class UserEntity : BaseEntity
    {
        public string Name { get; set; }
        public string HashedPassword { get; set; }
        public string Description { get; set; }
        public virtual ICollection<UserTeam> TeamList { get; set; }

        private sealed class UserEntityEqualityComparer : IEqualityComparer<UserEntity>
        {
            public bool Equals(UserEntity x, UserEntity y)
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

                return x.Id.Equals(y.Id) && string.Equals(x.Name, y.Name) && string.Equals(x.Description, y.Description);
            }

            public int GetHashCode(UserEntity obj)
            {
                unchecked
                {
                    var hashCode = (obj.Name != null ? obj.Name.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.Description != null ? obj.Description.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.TeamList != null ? obj.TeamList.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ obj.Id.GetHashCode();
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<UserEntity> UserEntityComparer { get; } = new UserEntityEqualityComparer();

    }
}
