using ICS.DAL;

namespace ICS.BL.Factories
{
    public interface IDbContextFactory
    {
        ICSDbContext CreateDbContext();
    }
}