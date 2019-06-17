using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ICS.App.Exceptions
{
    class UserAlreadyInTeamException : Exception
    {
        public UserAlreadyInTeamException()
        {
            MessageBox.Show("User is already in selected team");
        }
    }
}
