﻿// <auto-generated />
using System;
using ICS.DAL;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;

namespace ICS.DAL.Migrations
{
    [DbContext(typeof(ICSDbContext))]
    partial class ICSDbContextModelSnapshot : ModelSnapshot
    {
        protected override void BuildModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("ProductVersion", "2.2.3-servicing-35854")
                .HasAnnotation("Relational:MaxIdentifierLength", 128)
                .HasAnnotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn);

            modelBuilder.Entity("ICS.DAL.CommentEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd();

                    b.Property<Guid?>("AuthorId");

                    b.Property<string>("Content");

                    b.Property<int>("Likes");

                    b.Property<Guid?>("PostEntityId");

                    b.Property<DateTime>("Timestamp");

                    b.HasKey("Id");

                    b.HasIndex("AuthorId");

                    b.HasIndex("PostEntityId");

                    b.ToTable("Comments");
                });

            modelBuilder.Entity("ICS.DAL.PostEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd();

                    b.Property<Guid?>("AuthorId");

                    b.Property<string>("Content");

                    b.Property<int>("Likes");

                    b.Property<Guid?>("TeamEntityId");

                    b.Property<DateTime>("Timestamp");

                    b.Property<string>("Title");

                    b.HasKey("Id");

                    b.HasIndex("AuthorId");

                    b.HasIndex("TeamEntityId");

                    b.ToTable("Posts");
                });

            modelBuilder.Entity("ICS.DAL.TeamEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd();

                    b.Property<string>("Description");

                    b.Property<string>("TeamName");

                    b.HasKey("Id");

                    b.ToTable("Teams");
                });

            modelBuilder.Entity("ICS.DAL.UserEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd();

                    b.Property<string>("Description");

                    b.Property<string>("HashedPassword");

                    b.Property<string>("Name");

                    b.Property<Guid?>("TeamEntityId");

                    b.HasKey("Id");

                    b.HasIndex("TeamEntityId");

                    b.ToTable("Users");
                });

            modelBuilder.Entity("ICS.DAL.UserTeam", b =>
                {
                    b.Property<Guid>("UserId");

                    b.Property<Guid>("TeamId");

                    b.Property<Guid>("Id");

                    b.HasKey("UserId", "TeamId");

                    b.HasIndex("TeamId");

                    b.ToTable("UserTeam");
                });

            modelBuilder.Entity("ICS.DAL.CommentEntity", b =>
                {
                    b.HasOne("ICS.DAL.UserEntity", "Author")
                        .WithMany()
                        .HasForeignKey("AuthorId");

                    b.HasOne("ICS.DAL.PostEntity")
                        .WithMany("Comments")
                        .HasForeignKey("PostEntityId");
                });

            modelBuilder.Entity("ICS.DAL.PostEntity", b =>
                {
                    b.HasOne("ICS.DAL.UserEntity", "Author")
                        .WithMany()
                        .HasForeignKey("AuthorId");

                    b.HasOne("ICS.DAL.TeamEntity")
                        .WithMany("PostList")
                        .HasForeignKey("TeamEntityId");
                });

            modelBuilder.Entity("ICS.DAL.UserEntity", b =>
                {
                    b.HasOne("ICS.DAL.TeamEntity")
                        .WithMany("ModsList")
                        .HasForeignKey("TeamEntityId");
                });

            modelBuilder.Entity("ICS.DAL.UserTeam", b =>
                {
                    b.HasOne("ICS.DAL.TeamEntity", "Team")
                        .WithMany("UsersList")
                        .HasForeignKey("TeamId")
                        .OnDelete(DeleteBehavior.Cascade);

                    b.HasOne("ICS.DAL.UserEntity", "User")
                        .WithMany("TeamList")
                        .HasForeignKey("UserId")
                        .OnDelete(DeleteBehavior.Cascade);
                });
#pragma warning restore 612, 618
        }
    }
}
