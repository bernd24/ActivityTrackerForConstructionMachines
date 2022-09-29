using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using UIAutomate.Models;

    public class UITestContext : DbContext
    {
        public UITestContext (DbContextOptions<UITestContext> options)
            : base(options)
        {
        }

        public DbSet<UIAutomate.Models.VisAWITest> VisAWITest { get; set; } = default!;

        public DbSet<UIAutomate.Models.SUSTest> SUSTest { get; set; }
    }
