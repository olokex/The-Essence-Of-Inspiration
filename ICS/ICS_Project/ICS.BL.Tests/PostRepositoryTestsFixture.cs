using System;
using System.Collections.Generic;
using System.Text;
using ICS.BL.Repositories;

namespace ICS.BL.Tests
{
    public class PostRepositoryTestsFixture
    {
        private readonly IPostRepository repository;

        public PostRepositoryTestsFixture()
        {
            repository = new PostRepository(new InMemoryDbContextFactory());
        }

        public IPostRepository Repository => repository;

    }
}
