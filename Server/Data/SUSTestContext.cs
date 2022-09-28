using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using UIAutomate.Models;

    public class SUSTestContext : DbContext
    {
        public SUSTestContext (DbContextOptions<SUSTestContext> options)
            : base(options)
        {
        }

        public DbSet<UIAutomate.Models.SUSTest> SUSTest { get; set; } = default!;
    }
