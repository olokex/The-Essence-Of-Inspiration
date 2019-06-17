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
    class CommentListViewModel : ViewModelBase
    {
        private readonly ICommentRepository repository = new CommentRepository(new DbContextFactory());

        public List<CommentListModel> Comments { get; set; }

        public void Load()
        {
            Comments = repository.GetAll().ToList();
        }
    }
}
