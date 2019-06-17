using System;
using System.Collections.Generic;
using System.Text;
using ICS.BL.Repositories;

namespace ICS.BL.Tests
{
    public class TeamRepositoryTestsFixture
    {
        private readonly ITeamRepository repository;

        public TeamRepositoryTestsFixture()
        {
            repository = new TeamRepository(new InMemoryDbContextFactory());
        }

        public ITeamRepository Repository => repository;

    }
}
