using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using ICS.App.Commands;
using ICS.App.Exceptions;
using ICS.BL.Factories;
using ICS.BL.Models;
using ICS.BL.Repositories;
using Microsoft.Win32;

namespace ICS.App.ViewModels
{
    class LoginViewModel : ViewModelBase
    {
        private readonly IUserRepository repository = new UserRepository(new DbContextFactory());

        public UserDetailModel currentUser;

        public void login(string UserName, string Password)
        {
            var md5 = new MD5CryptoServiceProvider();
            var md5data = md5.ComputeHash(Encoding.ASCII.GetBytes(Password));
            var HashedPassword = Encoding.ASCII.GetString(md5data);
            UserListModel userToFind;
            try
            {
                 userToFind = repository.GetAll().First(user => user.Name == UserName);        
            }
            catch (Exception)
            {
                MessageBox.Show("User not found");
                throw new ApplicationException();
            }

            UserDetailModel UserDetail = repository.GetById(userToFind.Id);
            if (UserDetail.HashedPassword != HashedPassword)
            {
                throw new WrongPasswordException();
            }

            currentUser = UserDetail;
        }

        public void CreateAccount(string name, string password, string password2, string description)
        {
            if (!string.Equals(password, password2, comparisonType: StringComparison.Ordinal))
            {
                throw new WrongPasswordException();
            }
            try
            {
                repository.GetAll().First(user => user.Name == name);
                throw new UserNameAlreadyUsedException();
            }
            catch (UserNameAlreadyUsedException)
            {
                throw;
            }
            catch
            {
                // ignored
            }

            var md5 = new MD5CryptoServiceProvider();
            var md5Data = md5.ComputeHash(Encoding.ASCII.GetBytes(password));
            var hashedPassword = Encoding.ASCII.GetString(md5Data);

            UserDetailModel userNew = new UserDetailModel
            {
                Description = description,
                Name = name,
                HashedPassword = hashedPassword,
                Teams = new List<TeamListModel>()
            };

            repository.Create(userNew);
            currentUser = userNew;
        }
    }
}
