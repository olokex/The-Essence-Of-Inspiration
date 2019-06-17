using System;
using System.Collections.Generic;
using System.Text;
using ICS.BL.Repositories;

namespace ICS.BL.Tests
{
    public class CommentRepositoryTestsFixture
    {
        private readonly ICommentRepository repository;

        public CommentRepositoryTestsFixture()
        {
            repository = new CommentRepository(new InMemoryDbContextFactory());
        }

        public ICommentRepository Repository => repository;

    }
}