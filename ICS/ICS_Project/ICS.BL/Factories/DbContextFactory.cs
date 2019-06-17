using ICS.DAL;
using Microsoft.EntityFrameworkCore;

namespace ICS.BL.Factories
{
    public class DbContextFactory : IDbContextFactory
    {
        public ICSDbContext CreateDbContext()
        {
            var optionsBuilder = new DbContextOptionsBuilder<ICSDbContext>();
            optionsBuilder.UseSqlServer(@"Data Source=(LocalDB)\MSSQLLocalDB;Initial Catalog = ICSDB;MultipleActiveResultSets = True;Integrated Security = True; ");
            return new ICSDbContext(optionsBuilder.Options);
        }
    }
}