using System.Collections.Generic;

namespace ICS.DAL
{
    public class TeamEntity : BaseEntity
    {
        public string TeamName { get; set; }
        public string Description { get; set; }
        public virtual ICollection<UserTeam> UsersList { get; set; }
        public virtual ICollection<UserEntity> ModsList { get; set; }
        public virtual ICollection<PostEntity> PostList { get; set; }

        private sealed class TeamEntityEqualityComparer : IEqualityComparer<TeamEntity>
        {
            public bool Equals(TeamEntity x, TeamEntity y)
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
                       string.Equals(x.TeamName, y.TeamName) &&
                       string.Equals(x.Description, y.Description); //&&
                       //x.UsersList.OrderBy(user => user.UserId).SequenceEqual(y.UsersList.OrderBy(user => user.UserId), UserTeam.TeamEntityComparer);
            }

            public int GetHashCode(TeamEntity obj)
            {
                unchecked
                {
                    var hashCode = obj.TeamName.GetHashCode();
                    hashCode = (hashCode * 397) ^ (obj.Description != null ? obj.Description.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.ModsList != null ? obj.ModsList.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.PostList != null ? obj.PostList.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ obj.UsersList.GetHashCode();
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<TeamEntity> TeamEntityComparer { get; } = new TeamEntityEqualityComparer();

    }
}