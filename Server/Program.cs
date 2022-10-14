using Server.JWT;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using System.Text;
using MySqlConnector;
using SqlKata;
using SqlKata.Execution;
using SqlKata.Compilers;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddControllersWithViews();
builder.Services.AddTransient<QueryFactory>((e) =>
{

    var connection = new MySqlConnection(
        "server=127.0.0.1;user id=root;password=atfcm2022;port=3306;database=atfcm;"
    );

    var compiler = new MySqlCompiler();

    return new QueryFactory(connection, compiler);

});


// For Entity Framework
var serverVersion = new MySqlServerVersion(new Version(8, 0, 27));
builder.Services.AddDbContext<ApplicationDbContext>(options => options.UseMySql("server=127.0.0.1;user id=root;password=atfcm2022;port=3306;database=atfcmUser;",serverVersion));
// For Identity
builder.Services.AddIdentity<IdentityUser, IdentityRole>()
    .AddEntityFrameworkStores<ApplicationDbContext>()
    .AddDefaultTokenProviders();
// Adding Authentication
builder.Services.AddAuthentication(options =>
{
    options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
    options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
    options.DefaultScheme = JwtBearerDefaults.AuthenticationScheme;
})
// Adding Jwt Bearer
.AddJwtBearer(options =>
{
    options.SaveToken = true;
    options.RequireHttpsMetadata = false;
    options.TokenValidationParameters = new TokenValidationParameters()
    {
        ValidateIssuer = true,
        ValidateAudience = true,
        ValidAudience = "http://localhost:7209",
        ValidIssuer = "http://localhost:7209",
        IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes("JWTAuthenticationHIGHsecuredPasswordVVVp1OH7Xzyr"))
    };
    options.Events = new JwtBearerEvents
    {
      OnMessageReceived = context =>
      {
        context.Token = context.Request.Cookies["token"];
        return Task.CompletedTask;
      }
    };
});

var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Home/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

app.UseAuthentication();
app.UseAuthorization();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Index}/{id?}");

app.Run();
