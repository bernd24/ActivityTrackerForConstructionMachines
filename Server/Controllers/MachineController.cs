using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;
using Microsoft.AspNetCore.Authorization;
using SqlKata;
using SqlKata.Execution;
using System.Data.SqlClient;

namespace Server.Controllers;

[Authorize]
public class MachineController : Controller
{
    private readonly ILogger<MachineController> _logger;
    private readonly QueryFactory Db;

    public MachineController(ILogger<MachineController> logger, QueryFactory db)
    {
        _logger = logger;
        Db = db;
    }

    [HttpGet]
    public IActionResult Index()
    {
        var query = new Query(Db);
        var result = query.GetMachines();
        return View(result);
    }

    [HttpPost]
    public IActionResult Create([FromForm]machine form)
    {
        Db.Query("Machine").Insert(new {
            M_Id = form.M_Id,
            C_Id = form.C_Id,
            InternalId = form.InternalId,
            inUse = form.inUse
        });
        return RedirectToAction("Index", "Machine");
    }

    [HttpGet]
    public IActionResult Create()
    {
        return View(Db.Query("MachineModel").Get<MachineModel>());
    }

    [HttpPost]
    public IActionResult Update([FromForm]machine form)
    {
        Db.Query("Machine").Where("Id",form.Id).Update(new {
            InternalId = form.InternalId,
            inUse = form.inUse
        });
        return RedirectToAction("Index", "Machine");
    }

    [HttpGet("Machine/Update/{Id}")]
    public IActionResult Update(int Id)
    {
        var result = Db.Query("Machine").Where("Id",Id).Get<machine>();
        ViewBag.Machine = result.First();
        return View();
    }

    [HttpPost("Machine/Delete/{Id}")]
    public IActionResult Delete(int Id)
    {
        Db.Query("Machine").Where("Id",Id).Delete();
        return RedirectToAction("Index", "Machine");
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
