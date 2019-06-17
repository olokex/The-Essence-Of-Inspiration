using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ICS.App.Exceptions
{
    class UserNameNotFoundException : Exception
    {
        public UserNameNotFoundException()
        {
            MessageBox.Show("UserName not found");
        }
    }
}
