using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;

namespace Server.Controllers;

public class HomeController : Controller
{
    private readonly ILogger<HomeController> _logger;
    public AppDb Db { get; }

    public HomeController(ILogger<HomeController> logger, AppDb db)
    {
        _logger = logger;
        Db = db;
    }

    public async Task<IActionResult> Index()
    {
        await Db.Connection.OpenAsync();
        var query = new DbQuery(Db);
        var result = await query.FindOneAsync(1);
        TempData["test"] = result.Content;
        return View();
    }

    public IActionResult Machines()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
