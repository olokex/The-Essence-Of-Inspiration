using System;
using ICS.DAL.Interface;

namespace ICS.DAL
{
    public abstract class BaseEntity : IEntity
    {
        public Guid Id { get; set; }
    }
}
