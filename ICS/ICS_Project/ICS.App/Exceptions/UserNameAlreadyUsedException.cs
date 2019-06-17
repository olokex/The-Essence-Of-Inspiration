using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ICS.App.Exceptions
{
    class UserNameAlreadyUsedException : Exception
    {
        public UserNameAlreadyUsedException()
        {
            MessageBox.Show("Username already used");
        }
    }
}
