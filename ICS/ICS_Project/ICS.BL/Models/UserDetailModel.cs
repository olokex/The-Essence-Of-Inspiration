using System;
using System.Collections.Generic;
using System.Text;

namespace ICS.BL.Models
{
    public class UserDetailModel : BaseModel
    {
        public string Name { get; set; }
        public string HashedPassword { get; set; }
        public string Description { get; set; }
        public virtual List<TeamListModel> Teams { get; set; }
    }
}
