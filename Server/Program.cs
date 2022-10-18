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
using Server.Models;
using System.Diagnostics;

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

var callback = new Callback();
var timer = new Timer(callback.SessionEndCallback,null,0,10000);

app.Run();

public class Callback{
    QueryFactory db;

    public Callback(){
        var connection = new MySqlConnection(
            "server=127.0.0.1;user id=root;password=atfcm2022;port=3306;database=atfcm;"
        );
        var compiler = new MySqlCompiler();
        db = new QueryFactory(connection, compiler);

        System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
        customCulture.NumberFormat.NumberDecimalSeparator = ".";

        System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;
    }

    public void SessionEndCallback(Object stateInfo){
        IEnumerable<WorkSession> result = db.Query("WorkSession")
        .Select("WorkSession.Id","WorkSession.C_Id")
        .Where("WorkSession.isProcessed",false)
        .Get<WorkSession>();
        foreach(WorkSession ws in result){
            var lastMeasurementQuery = db.Query("Measurement")
            .Where("WS_Id",ws.Id)
            .OrderByDesc("TimeOfMeasure")
            .Limit(1)
            .Get();

            if(lastMeasurementQuery.Count() != 0 && (lastMeasurementQuery.First().TimeOfMeasure.AddMinutes(2) < DateTime.Now)){
                db.Query("WorkSession")
                .Where("Id",ws.Id)
                .Update(new{
                    isProcessed = true
                });

                Console.WriteLine("processing session " + ws.Id);

                var sensors = db.Query("Measurement")
                .Select("Sensor.SensorName","Sensor.SensorType","SensorInstance.Axis","Measurement.SI_Id","SensorInstance.SN_Id","SensorInstance.A_Id")
                .Where("WS_Id",ws.Id)
                .GroupBy("SI_Id")
                .Join("SensorInstance","SensorInstance.Id","Measurement.SI_Id")
                .Join("Sensor","Sensor.Id","SensorInstance.S_Id")
                .OrderBy("SensorInstance.SN_Id")
                .Get();

                foreach(var s in sensors){
                    var data = db.Query("Measurement")
                    .Where("WS_Id",ws.Id)
                    .Where("SI_Id",(int)s.SI_Id)
                    .OrderBy("TimeOfMeasure")
                    .OrderBy("Nr")
                    .Get<Measurement>();

                    string text = "";
                    foreach(Measurement m in data){
                        text += m.SensorData + " ";
                    }
                    string filePath = ws.Id + "/" + s.SensorName + "_" + s.SensorType + "_" + (s.Axis == null ? "0" : s.Axis) + "_" + s.SI_Id + ".txt";
                    (new FileInfo(filePath)).Directory.Create();
                    System.IO.File.WriteAllBytes(filePath,Encoding.ASCII.GetBytes(text));
                }

                Process p = new Process();
                p.StartInfo.FileName = "atb.bin";
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.Arguments = "" + ws.Id;
                p.Start();
                p.WaitForExit();

                db.Query("WorkSessionActivity")
                .Where("WS_Id",ws.Id)
                .Delete();
                foreach(var s in sensors){
                    string filePath = ws.Id + "/" + s.SensorName + "_" + s.SensorType + "_" + (s.Axis == null ? "0" : s.Axis) + "_" + s.SI_Id + "_timeline.txt";
                    if(s.A_Id != null && System.IO.File.Exists(filePath)){
                        string text = System.IO.File.ReadAllText(filePath);
                        string[] values = text.Split(' '); 
                        DateTime start = db.Query("Measurement")
                        .Where("WS_Id",ws.Id)
                        .OrderByDesc("TimeOfMeasure")
                        .Limit(1)
                        .Get<Measurement>()
                        .First()
                        .TimeOfMeasure;
                        DateTime end = start;
                        DateTime current = start;
                        bool period = false;

                        foreach(string val in values){
                            if(val == "0"){
                                if(period){
                                    db.Query("WorkSessionActivity")
                                    .Insert(new{
                                        A_Id = s.A_Id,
                                        WS_Id = ws.Id,
                                        StartTime = start,
                                        EndTime = end
                                    });
                                }
                                period = false;
                            }   
                            else{
                                if(!period){
                                    period = true;
                                    start = current;
                                }
                                else{
                                    end = current;
                                }
                            }
                            current = current.AddMilliseconds(100);
                        }
                        if(period){
                            db.Query("WorkSessionActivity")
                            .Insert(new{
                                A_Id = s.A_Id,
                                WS_Id = ws.Id,
                                StartTime = start,
                                EndTime = end
                            });
                        }
                    }
                }

                new DirectoryInfo("" + ws.Id).Delete(true);

                Console.WriteLine("finished processing");
            }
        }
    }
}