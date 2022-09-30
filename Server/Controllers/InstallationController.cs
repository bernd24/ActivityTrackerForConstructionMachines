using Microsoft.AspNetCore.Mvc;
using Server.Models;
using Microsoft.AspNetCore.Authorization;
using SqlKata;
using SqlKata.Execution;
using System.Data.SqlClient;

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
    public IActionResult Create(){
        //select auto_increment from information_schema.tables where table_schema = "atfcm" and table_name = "SensorConfiguration";
        return View(Db.Query("Sensor").Get<Sensor>());
    }
}