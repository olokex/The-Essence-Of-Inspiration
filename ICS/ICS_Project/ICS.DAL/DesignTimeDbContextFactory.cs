using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;

namespace ICS.DAL
{
    class DesignTimeDbContextFactory
    {
        class DesignTimeICSDbContextFactory : IDesignTimeDbContextFactory<ICSDbContext>
        {
            public ICSDbContext CreateDbContext(string[] args)
            {
                var optionsBuilder = new DbContextOptionsBuilder<ICSDbContext>();
                optionsBuilder.UseSqlServer(@"Data Source=(LocalDB)\MSSQLLocalDB;Initial Catalog = ICSDB;MultipleActiveResultSets = True;Integrated Security = True; ");
                return new ICSDbContext(optionsBuilder.Options);
            }
        }
    }
}
