using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICS.BL.Factories;
using ICS.BL.Models;
using ICS.BL.Repositories;
using ICS.BL.Mapper;
using Microsoft.EntityFrameworkCore.Storage;

namespace ICS.App.ViewModels
{
    public class PostListViewModel : ViewModelBase
    {
        private readonly IPostRepository repository = new PostRepository(new DbContextFactory());

        public List<PostListModel> Posts { get; set; }

        public void Load(TeamDetailModel team)
        {
            //Posts = repository.GetAll().ToList();
            //Posts = repository.GetByBelonging().ToList();
            Posts = new List<PostListModel>();
            foreach (var post in team.Posts)
            {
                Posts.Add(PostMapper.MapPostDetailModelToListModel(post));
            }
        }
    }
}
