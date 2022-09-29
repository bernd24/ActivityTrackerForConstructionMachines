using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using UIAutomate.Models;

public class TestpersonContext : DbContext{
    public DbSet<Testperson> Testpersons {get; set;}
    public DbSet<SUSTest> SUSTests {get; set;}
    public DbSet<UEQTest> UEQTests {get; set;}
    public DbSet<VisAWITest> VisAWITests {get; set;}

    public TestpersonContext()
    {
        
    }
    // public TestpersonContext(DbContextOptions<TestpersonContext> options): base(options) {}  
    protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    {
        optionsBuilder.UseSqlServer(@"Server=(localdb)\mssqllocaldb;Database=Test");
    }

}