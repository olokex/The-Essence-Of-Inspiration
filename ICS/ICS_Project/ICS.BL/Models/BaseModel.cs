using System;

namespace ICS.BL.Models
{
    public abstract class BaseModel
    {
        public Guid Id { get; set; } = Guid.NewGuid();
    }
}