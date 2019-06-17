using System.Collections.Generic;

namespace ICS.BL.Models
{
    public class TeamDetailModel : BaseModel
    {
        public string TeamName { get; set; }
        public string Description { get; set; }
        public List<UserListModel> Users { get; set; }
        public List<UserListModel> Mods { get; set; }
        public List<PostDetailModel> Posts { get; set; }
    }
}
