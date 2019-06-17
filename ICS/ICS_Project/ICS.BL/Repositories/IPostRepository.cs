using System;
using System.Collections.Generic;
using System.Text;
using ICS.BL.Models;

namespace ICS.BL.Repositories
{
    public interface IPostRepository
    {
        IEnumerable<PostListModel> GetAll();
        PostDetailModel GetById(Guid id);
        IEnumerable<PostListModel> GetByBelonging(TeamDetailModel team);
        PostDetailModel Create(PostDetailModel model);
        void Update(PostDetailModel model);
        void Delete(Guid id);
        PostDetailModel AddCommentToPost(PostDetailModel post, CommentDetailModel comment);
    }
}
