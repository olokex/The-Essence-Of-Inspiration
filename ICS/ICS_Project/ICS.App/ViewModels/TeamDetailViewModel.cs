using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICS.App.Exceptions;
using ICS.BL.Factories;
using ICS.BL.Models;
using ICS.BL.Repositories;
using ICS.BL.Mapper;

namespace ICS.App.ViewModels
{
    public class TeamDetailViewModel : ViewModelBase
    {
        private readonly ITeamRepository repository = new TeamRepository(new DbContextFactory());

        public TeamDetailModel Model { get; set; } = new TeamDetailModel();


        public void Load(Guid id)
        {
            Model = repository.GetById(id);
        }

        public void Create(UserDetailModel user)
        {
            Model = repository.Create(Model, user);
            
        }

        public void Edit()
        {
            repository.Update(Model);
        }

        public void Delete()
        {
            repository.Delete(Model.Id);
        }

        public void AddUser(UserDetailModel user)
        {
            if (Model.Users == null)
                Model.Users = new List<UserListModel>();
            foreach (var item in Model.Users)
            {
                if(item.Name == user.Name)
                    throw new UserAlreadyInTeamException();
            }
            repository.AddUserToTeam(Model, user);   
        }

        public void AddPost(PostDetailModel post)
        {
            if(Model.Posts==null)
                Model.Posts= new List<PostDetailModel>();
            repository.AddPostToTeam(Model, post);
        }

        public void KickUser(UserDetailModel user)
        {
            if (user.Id == Guid.Empty)
                return;
            if (Model.Users != null)
            {
                foreach (var item in Model.Users)
                {
                    if (item.Name == user.Name)
                    {
                        repository.RemoveUserFromTeam(Model, user);
                        return;
                    }
                }
    
            }
            throw new UserIsNotInTeamExcpetion();
            
        }

        public void RemoveMod(UserDetailModel user)
        {//Příprava na rozšíření o adminy
            if (user.Id == Guid.Empty)
                return;
            UserListModel ModToKick = new UserListModel
            {
                Description = user.Description,
                Id = user.Id,
                Name = user.Name
            };
            if (Model.Mods != null && Model.Mods.Contains(ModToKick))
                Model.Mods.Remove(ModToKick);
            repository.Update(Model);
        }

        public void AddMod(UserDetailModel user)
        {//Příprava na rozšíření o adminy
            if (user.Id == Guid.Empty)
                return;
            UserListModel ModToAdd = new UserListModel
            {
                Description = user.Description,
                Id = user.Id,
                Name = user.Name
            };
            if (Model.Mods == null)
                Model.Mods = new List<UserListModel>();
            if (!Model.Mods.Contains(ModToAdd))
                Model.Mods.Add(ModToAdd);
            repository.Update(Model);
        }
    }
}
