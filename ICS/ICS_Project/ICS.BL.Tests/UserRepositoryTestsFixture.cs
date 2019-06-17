using System;
using System.Collections.Generic;
using System.Text;
using ICS.BL.Repositories;

namespace ICS.BL.Tests
{
    public class UserRepositoryTestsFixture
    {
        private readonly IUserRepository repository;

        public UserRepositoryTestsFixture()
        {
            repository = new UserRepository(new InMemoryDbContextFactory());
        }

        public IUserRepository Repository => repository;

    }
}
