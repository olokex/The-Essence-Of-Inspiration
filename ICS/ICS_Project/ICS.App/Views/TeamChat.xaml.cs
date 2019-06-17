using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ICS.App.ViewModels;
using ICS.BL.Mapper;
using ICS.BL.Models;

namespace ICS.App.Views
{
    /// <summary>
    /// Interaction logic for TeamChat.xaml
    /// </summary>
    public partial class TeamChat : Page
    {
        

        public event SettingBase.TeamChatCreateNewTeam TeamChatCreateNewTeamEvent;

        public event SettingBase.SettingGoToAddUser SettingGoToAddUserEvent;
        public event SettingBase.TeamChatUserInfo TeamChatUserInfoEvent;

        public TeamListViewModel teamsVM = new TeamListViewModel();
        public TeamDetailViewModel teamDetailVM = new TeamDetailViewModel();
        public PostListViewModel listVM = new PostListViewModel();
        public PostDetailViewModel PostDetailVM = new PostDetailViewModel();

        private PostDetailModel selectedPost;
        public static UserListModel selectedUser;

        public TeamChat()
        {
            InitializeComponent();
            
        }


        private void CreateNewTeamButton_OnClick(object sender, RoutedEventArgs e)
        {
            TeamChatCreateNewTeamEvent();
        }

        private void RefreshButton_OnClick(object sender, RoutedEventArgs e)
        {
            refresh();
        }

        private void SettingsButton_Click(object sender, RoutedEventArgs e)
        {
            SettingGoToAddUserEvent();
        }


        private void TeamList_OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count > 0)
            {

                TeamListModel selected = (TeamListModel) e.AddedItems[0];
                teamDetailVM.Load(selected.Id);
                MainWindow.CurrentTeam = teamDetailVM.Model;
                selectedPost = null;

                TextBlockTeamNameTeamChat.Text = MainWindow.CurrentTeam.TeamName;
                TextBlockTeamDescriptionTeamChat.Text = MainWindow.CurrentTeam.Description;

                ListViewMembers.ItemsSource = MainWindow.CurrentTeam.Users;

                listVM.Load(MainWindow.CurrentTeam);
                listVM.Posts.Sort((x, y) => DateTime.Compare(x.Timestamp, y.Timestamp));
                listVM.Posts.Reverse();
                ListViewPosts.ItemsSource = listVM.Posts;

                List<CommentListModel> commentToShow = new List<CommentListModel>();
                if (selectedPost != null && selectedPost.Comments != null)
                    foreach (var item in selectedPost.Comments)
                    {
                        commentToShow.Add(CommentMapper.MapCommentDetailModelToListModel(item));
                    }
                commentToShow.Sort((x, y) => DateTime.Compare(x.Timestamp, y.Timestamp));
                commentToShow.Reverse();
                ListViewCommentsOfPost.ItemsSource = commentToShow;
            }
        }

        public void refresh()
        {
            teamsVM.Load(MainWindow.CurrentUser);
            TeamList.ItemsSource = teamsVM.Teams;
            if (MainWindow.CurrentTeam != null)
            {
                TextBlockTeamNameTeamChat.Text = MainWindow.CurrentTeam.TeamName;
                TextBlockTeamDescriptionTeamChat.Text = MainWindow.CurrentTeam.Description;

                teamDetailVM.Load(MainWindow.CurrentTeam.Id);
                MainWindow.CurrentTeam = teamDetailVM.Model;
                ListViewMembers.ItemsSource = MainWindow.CurrentTeam.Users;

                listVM.Load(MainWindow.CurrentTeam);
                listVM.Posts.Sort((x, y) => DateTime.Compare(x.Timestamp, y.Timestamp));
                listVM.Posts.Reverse();
                ListViewPosts.ItemsSource = listVM.Posts;

                List<CommentListModel> commentToShow = new List<CommentListModel>();
                if (selectedPost != null && selectedPost.Comments != null)
                    foreach (var item in selectedPost.Comments)
                    {
                        commentToShow.Add(CommentMapper.MapCommentDetailModelToListModel(item));
                    }
                commentToShow.Sort((x, y) => DateTime.Compare(x.Timestamp, y.Timestamp));
                commentToShow.Reverse();
                ListViewCommentsOfPost.ItemsSource = commentToShow;
            }
            else
            {
                ListViewMembers.ItemsSource = null;
                ListViewPosts.ItemsSource = null;
                ListViewCommentsOfPost.ItemsSource = null;
            }

        }

        private void ButtonSendNewPost_Click(object sender, RoutedEventArgs e)
        {
            if(TextBoxNewPostContent.Text=="")
            {
                MessageBox.Show("Text cannot be empty");
                return;
            }
            if(TextBoxTitleNewPost.Text=="")
            {
                MessageBox.Show("Title cannot be empty");
                return;
            }
            if(MainWindow.CurrentTeam==null)
            {
                MessageBox.Show("Please select team");
                return;
            }

            PostDetailModel newPost = new PostDetailModel
            {
                Content = TextBoxNewPostContent.Text,
                Author = MainWindow.CurrentUser,
                Comments = new List<CommentDetailModel>(),
                Likes = 0,
                Timestamp = DateTime.Now,
                Title = TextBoxTitleNewPost.Text
            };
            teamDetailVM.Model = MainWindow.CurrentTeam;
            teamDetailVM.AddPost(newPost);
            refresh();
        }

        private void ButtonSendNewCommentToPost_Click(object sender, RoutedEventArgs e)
        {
            if(TextBoxNewCommentContent.Text=="")
            {
                MessageBox.Show("Text cannot be empty");
                return;
            }

            if (selectedPost == null)
            {
                MessageBox.Show("Please select a post");
                return;
            }
            CommentDetailModel newComment = new CommentDetailModel
            {
                Content = TextBoxNewCommentContent.Text,
                Author = MainWindow.CurrentUser,
                Likes = 0,
                Timestamp = DateTime.Now
            };
            PostDetailVM.Model = selectedPost;
            PostDetailVM.addComment(newComment);
            refresh();
        }

        private void ListViewPosts_OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count > 0)
            {
                PostListModel selected = (PostListModel) e.AddedItems[0];
                PostDetailVM.Load(selected.Id);
                selectedPost = PostDetailVM.Model;

                List<CommentListModel> commentToShow = new List<CommentListModel>();
                if (selectedPost != null && selectedPost.Comments != null)
                    foreach (var item in selectedPost.Comments)
                    {
                        commentToShow.Add(CommentMapper.MapCommentDetailModelToListModel(item));
                    }
                commentToShow.Sort((x, y) => DateTime.Compare(x.Timestamp, y.Timestamp));
                commentToShow.Reverse();
                ListViewCommentsOfPost.ItemsSource = commentToShow;
            }

        }

        private void ListViewMembers_OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count > 0)
            {
                selectedUser = (UserListModel) e.AddedItems[0];
                TeamChatUserInfoEvent();
            }
        }

        private void ButtonSearchTeamChat_OnClick(object sender, RoutedEventArgs e)
        {
            string stringToFind = TextBoxSearchTeamChat.Text;
            if (listVM.Posts == null)
            {
                MessageBox.Show("No posts");
                return;
            }
            List<PostListModel> foundPosts = new List<PostListModel>();
            foreach (var item in listVM.Posts)
            {
                if (item.Title.IndexOf(stringToFind, StringComparison.OrdinalIgnoreCase) >= 0)
                    foundPosts.Add(item);
            }

            ListViewPosts.ItemsSource = null;
            ListViewPosts.ItemsSource = foundPosts;
            ListViewCommentsOfPost.ItemsSource = null;
        }
    }
}
