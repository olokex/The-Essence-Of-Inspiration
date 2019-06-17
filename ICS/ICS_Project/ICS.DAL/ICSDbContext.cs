using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.EntityFrameworkCore;

namespace ICS.DAL
{
    public class ICSDbContext : DbContext
    {
        public ICSDbContext()
        {
        }

        public ICSDbContext(DbContextOptions<ICSDbContext> contextOptions) : base(contextOptions)
        {
        }

        public DbSet<CommentEntity> Comments { get; set; }
        public DbSet<PostEntity> Posts { get; set; }
        public DbSet<TeamEntity> Teams { get; set; }
        public DbSet<UserEntity> Users { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<UserTeam>()
               .HasKey(ut => new { ut.UserId, ut.TeamId });

            modelBuilder.Entity<UserTeam>()
                .HasOne(ut => ut.User)
                .WithMany(u => u.TeamList)
                .HasForeignKey(ut => ut.UserId)
                .OnDelete(DeleteBehavior.Cascade);

            modelBuilder.Entity<UserTeam>()
                .HasOne(ut => ut.Team)
                .WithMany(t => t.UsersList)
                .HasForeignKey(ut => ut.TeamId)
                .OnDelete(DeleteBehavior.Cascade);
        }
    }
}
