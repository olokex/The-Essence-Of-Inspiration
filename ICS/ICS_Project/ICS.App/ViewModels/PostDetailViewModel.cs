using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICS.BL.Factories;
using ICS.BL.Models;
using ICS.BL.Repositories;

namespace ICS.App.ViewModels
{
    public class PostDetailViewModel : ViewModelBase
    {
        private readonly IPostRepository repository = new PostRepository(new DbContextFactory());

        public PostDetailModel Model { get; set; } = new PostDetailModel();

        public void Load(Guid id)
        {
            Model = repository.GetById(id);
        }
        public void Save()
        {
            if (Model.Id == Guid.Empty)
            {
                Model = repository.Create(Model);
            }
            else
            {
                repository.Update(Model);
            }
        }

        public void Delete()
        {
            repository.Delete(Model.Id);
        }

        public void Like()
        {//Příprava na rozšíření o liky na posty a commenty
            Model.Likes++;
            repository.Update(Model);
        }

        public void DisLike()
        {//Příprava na rozšíření o liky na posty a commenty
            Model.Likes--;
            repository.Update(Model);
        }

        public void addComment(CommentDetailModel comment)
        {
            if(Model.Comments==null)
                Model.Comments=new List<CommentDetailModel>();
            Model = repository.AddCommentToPost(Model, comment);
        }
    }
}
