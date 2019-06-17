using System;
using System.Collections.Generic;
using System.Text;

namespace ICS.DAL
{
    public class UserTeam : BaseEntity
    {
        public Guid UserId { get; set; }
        public UserEntity User { get; set; }

        public Guid TeamId { get; set; }
        public TeamEntity Team { get; set; }

        private sealed class UserTeamEqualityComparer : IEqualityComparer<UserTeam>
        {
            public bool Equals(UserTeam x, UserTeam y)
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
                return x.UserId.Equals(y.UserId) &&
                       x.TeamId.Equals(y.TeamId) &&
                       UserEntity.UserEntityComparer.Equals(x.User, y.User) &&
                       TeamEntity.TeamEntityComparer.Equals(x.Team, y.Team);
            }

            public int GetHashCode(UserTeam obj)
            {
                unchecked
                {
                    var hashCode = obj.UserId.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.TeamId.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.User.GetHashCode();
                    hashCode = (hashCode * 397) ^ obj.Team.GetHashCode();
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<UserTeam> TeamEntityComparer { get; } = new UserTeamEqualityComparer();

    }
}
