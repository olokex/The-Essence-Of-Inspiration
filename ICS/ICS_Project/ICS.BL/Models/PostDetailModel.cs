using System;
using System.Collections.Generic;

namespace ICS.BL.Models
{
    public class PostDetailModel : BaseModel
    {
        public DateTime Timestamp { get; set; }
        public string Content { get; set; }
        public List<CommentDetailModel> Comments { get; set; }
        public int Likes { get; set; }
        public UserDetailModel Author { get; set; }
        public string Title { get; set; }
    }
}
