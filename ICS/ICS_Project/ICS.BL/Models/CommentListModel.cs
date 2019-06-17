using System;
using System.Collections.Generic;
using System.Text;

namespace ICS.BL.Models
{
    public class CommentListModel : BaseModel
    {
        public string AuthorName { get; set; }
        public string Content { get; set; }
        public DateTime Timestamp { get; set; }
    }
}
