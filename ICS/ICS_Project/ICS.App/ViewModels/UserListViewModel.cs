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
    class UserListViewModel : ViewModelBase
    {
        private readonly IUserRepository repository = new UserRepository(new DbContextFactory());

        public List<UserListModel> Users { get; set; }

        public void Load()
        {
            Users = repository.GetAll().ToList();
        }
    }
}
