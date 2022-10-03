using Microsoft.AspNetCore.Mvc;
using Server.Models;
using Microsoft.AspNetCore.Authorization;
using SqlKata;
using SqlKata.Execution;
using System.Data.SqlClient;
using System.IO;

namespace Server.Controllers;

[Authorize]
public class InstallationController : Controller
{
    private readonly ILogger<MachineController> _logger;
    private readonly QueryFactory Db;

    public InstallationController(ILogger<MachineController> logger, QueryFactory db)
    {
        _logger = logger;
        Db = db;
    }

    [HttpGet]
    public IActionResult Index()
    {
        return View(new Query(Db).GetMachinesWithConfig());
    }

    [HttpGet]
    public IActionResult Create(int Id){
        //select auto_increment from information_schema.tables where table_schema = "atfcm" and table_name = "SensorConfiguration";
        ViewBag.Machine = new Query(Db).GetMachineWithConfig(Id);
        return View(Db.Query("Sensor").Get<Sensor>());
    }

    public class SNI{
        public string isMaster {get; set;}
        public string hasBattery {get; set;}
        public List<int> S_Id {get; set;}
        public List<char?> Axis {get; set;}
        public List<char> Position {get; set;}
        public List<char?> RssiPartner {get; set;}
    }

    [HttpPost]
    public IActionResult Create([FromForm]SNI sn, int Id){
        //select auto_increment from information_schema.tables where table_schema = "atfcm" and table_name = "SensorConfiguration";
        ViewBag.Machine = new Query(Db).GetMachineWithConfig(Id);
        if(ViewBag.Machine.Config == null){
            Db.Query("SensorConfiguration").Insert(new {
                Picture = "woodig1.jpg"
            });
            var result = Db.Query("SensorConfiguration")
                           .Select("Id")
                           .OrderByDesc("Id")
                           .Get();
            Db.Query("Machine")
            .Where("Id",Id)
            .Update(new {
                C_Id = result.First().Id
            });
            ViewBag.Machine = new Query(Db).GetMachineWithConfig(Id);
        }
        Db.Query("SensorNodeInstance").Insert(new {
            isMaster = sn.isMaster == "Master" ? true : false,
            hasBattery = sn.hasBattery == "Yes" ? true : false,
            C_Id = ViewBag.Machine.C_Id
        });
        var result2 = Db.Query("SensorNodeInstance")
                           .Select("Id")
                           .OrderByDesc("Id")
                           .Get();
        for(int i = 1; i < sn.S_Id.Count(); i++){
            Db.Query("SensorInstance").Insert(new {
                SN_Id = result2.First().Id,
                S_Id = sn.S_Id.ElementAt(i),
                Axis = sn.Axis.ElementAt(i) == '-' ? null : sn.Axis.ElementAt(i),
                Position = sn.Position.ElementAt(i),
                RssiPartner = sn.RssiPartner.ElementAt(i) == '-' ? null : sn.RssiPartner.ElementAt(i)
            });
        }
        ViewBag.Machine = new Query(Db).GetMachineWithConfig(Id);
        return View(Db.Query("Sensor").Get<Sensor>());
    }

    [HttpPost]
    public async Task<ActionResult> CreatePic(IFormFile ConfigPic, int Id)
    {
        var filePath = Path.GetTempFileName();
            if (ConfigPic.Length > 0)
            {

                using (var stream = System.IO.File.Create(filePath))
                {
                    await ConfigPic.CopyToAsync(stream);
                }
            }
        ViewBag.Machine = new Query(Db).GetMachineWithConfig(Id);
        if(ViewBag.Machine.Config == null){
            Db.Query("SensorConfiguration").Insert(new {
                Picture = "woodig1.jpg"
            });
            var result = Db.Query("SensorConfiguration")
                           .Select("Id")
                           .OrderByDesc("Id")
                           .Get();
            Db.Query("Machine")
            .Where("Id",Id)
            .Update(new {
                C_Id = result.First().Id
            });
            ViewBag.Machine = new Query(Db).GetMachineWithConfig(Id);
        }
        string newFileName = ViewBag.Machine.C_Id + System.IO.Path.GetExtension(ConfigPic.FileName);
        string newPath = "wwwroot/images/" + newFileName;
        if(System.IO.File.Exists(newPath)){
            System.IO.File.Delete(newPath);
        }
        System.IO.File.Move(filePath, newPath);
        SqlKata.Query query = Db.Query("SensorConfiguration")
        .Where("Id",ViewBag.Machine.C_Id);
        query.Update(new {
            Picture = newFileName
        });

        return Ok();
    }
}