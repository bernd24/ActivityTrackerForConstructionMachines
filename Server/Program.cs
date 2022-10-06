using Microsoft.EntityFrameworkCore;

// using System.Data.Entity;
using MySql.Data.MySqlClient;
using Microsoft.Extensions.DependencyInjection;
var builder = WebApplication.CreateBuilder(args);
builder.Services.AddDbContext<TestpersonContext>(options =>
    options.UseSqlite(builder.Configuration.GetConnectionString("TestpersonContext") ?? throw new InvalidOperationException("Connection string 'UITestContext' not found.")));

// var serverVersion = new MySqlServerVersion(new Version(8, 0, 27));
// builder.Services.AddDbContext<TestpersonContext>(options => 
//         options.UseMySql("server=127.0.0.1;user id=root;password=atfcm2022;port=3306;database=atfcmUser;",serverVersion));

// Add services to the container.
builder.Services.AddControllersWithViews();

var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Home/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

// var builder = WebApplication.CreateBuilder(args);



app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

app.UseAuthorization();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Index}/{id?}");

app.Run();
