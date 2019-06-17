using System;
using System.Collections.Generic;
using System.Text;
using ICS.BL.Models;

namespace ICS.BL.Repositories
{
    public interface IUserRepository
    {
        IEnumerable<UserListModel> GetAll();
        UserDetailModel GetById(Guid id);
        UserDetailModel Create(UserDetailModel model);
        void Update(UserDetailModel model);
        void Delete(Guid id);
        UserDetailModel GetByName(string name);
    }
}
