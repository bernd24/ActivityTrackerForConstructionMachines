using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;

namespace Server.Controllers;

public class MachineController : Controller
{
    private readonly ILogger<MachineController> _logger;
    public AppDb Db { get; }

    public MachineController(ILogger<MachineController> logger, AppDb db)
    {
        _logger = logger;
        Db = db;
    }

    public async Task<IActionResult> Index()
    {
        await Db.Connection.OpenAsync();
        var query = new Query(Db);
        var result = await query.FindOneAsync(1);
        TempData["ma"] = result.Manufacturer;
        TempData["mo"] = result.Model;
        TempData["mt"] = result.MachineType;
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
