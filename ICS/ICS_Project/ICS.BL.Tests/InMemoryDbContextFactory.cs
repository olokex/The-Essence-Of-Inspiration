using ICS.BL.Factories;
using ICS.DAL;
using Microsoft.EntityFrameworkCore;

namespace ICS.BL.Tests
{
    class InMemoryDbContextFactory : IDbContextFactory
    {
        public ICSDbContext CreateDbContext()
        {
            var optionsBuilder = new DbContextOptionsBuilder<ICSDbContext>();
            optionsBuilder.UseInMemoryDatabase("ICSDB");
            return new ICSDbContext(optionsBuilder.Options);
        }
    }
}