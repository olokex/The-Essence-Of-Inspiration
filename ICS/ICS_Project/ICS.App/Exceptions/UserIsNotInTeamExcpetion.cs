using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ICS.App.Exceptions
{
    class UserIsNotInTeamExcpetion : Exception
    {
        public UserIsNotInTeamExcpetion()
        {
            MessageBox.Show("User is not in selected team");
        }
    }
}
