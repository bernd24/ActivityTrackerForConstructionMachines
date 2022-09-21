using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;
using UIAutomate.Models;

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
    public async Task<IActionResult> SUSTest()
    {
        return View();
    }

    public async Task<IActionResult> Index()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
