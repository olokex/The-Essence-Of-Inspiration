using System;
using System.Collections.Generic;
using System.Text;
using ICS.BL.Models;

namespace ICS.BL.Repositories
{
    public interface ICommentRepository
    {
        IEnumerable<CommentListModel> GetAll();
        CommentDetailModel GetById(Guid id);
        CommentDetailModel Create(CommentDetailModel model);
        void Update(CommentDetailModel model);
        void Delete(Guid id);
        
    }
}
