using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ICS.App.Views
{
    public abstract class SettingBase 
    {
        public delegate void GoBackToTheTeamChat();
        public delegate void SettingGoToKickUser();
        public delegate void SettingGoToCreateNewUser();
        public delegate void SettingGoToAddUser();
        public delegate void TeamChatCreateNewTeam();
        public delegate void TeamChatUserInfo();
        public delegate void Logout();
    }
}
