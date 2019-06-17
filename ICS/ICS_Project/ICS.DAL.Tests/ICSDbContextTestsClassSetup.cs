using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.EntityFrameworkCore;

namespace ICS.DAL.Tests
{
    public class ICSDbContextTestsClassSetup
    {
        public ICSDbContext ICSDbContextSUT { get; set; }

        public ICSDbContextTestsClassSetup()
        {
            this.ICSDbContextSUT = CreateICSDbContext();
        }

        public ICSDbContext CreateICSDbContext()
        {
            return new ICSDbContext(CreateDbContextOptions());
        }

        private DbContextOptions<ICSDbContext> CreateDbContextOptions()
        {
            var options = new DbContextOptionsBuilder<ICSDbContext>()
                .UseInMemoryDatabase(databaseName: "ICSTestDB")
                .Options;
            return options;
        }

        public void Dispose()
        {
            ICSDbContextSUT?.Dispose();
        }
    }
}
