using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICS.App.Exceptions;
using ICS.BL.Factories;
using ICS.BL.Models;
using ICS.BL.Repositories;

namespace ICS.App.ViewModels
{
    class UserDetailViewModel : ViewModelBase
    {
        private readonly IUserRepository repository = new UserRepository(new DbContextFactory());

        public UserDetailModel Model { get; set; } = new UserDetailModel();

        public void Load(Guid id)
        {
            Model = repository.GetById(id);
        }

        public void LoadByName(String name)
        {
            try
            {
                Model = repository.GetByName(name);
            }
            catch (Exception)
            {
                throw new UserNameNotFoundException();
            }
            
        }

        public void Create()
        {
            if (Model.Id == Guid.Empty)
            {
                Model = repository.Create(Model);
            }
        }

        public void Edit()
        {
            repository.Update(Model);
        }
        public void Delete()
        {
            repository.Delete(Model.Id);
        }
        public void Clear()
        {
            Model = new UserDetailModel();
        }
    }
}
