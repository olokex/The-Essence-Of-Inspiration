using System;
using System.Collections.Generic;
using System.Text;

namespace ICS.BL.Models
{
    public class CommentDetailModel : BaseModel
    {
        public DateTime Timestamp { get; set; }
        public UserDetailModel Author { get; set; }
        public string Content { get; set; }
        public int Likes { get; set; }
    }
}
