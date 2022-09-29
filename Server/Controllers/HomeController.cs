using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;
using Microsoft.AspNetCore.Authorization;
using Server.JWT;

namespace Server.Controllers;

[Authorize]
public class HomeController : Controller
{
    private readonly ILogger<HomeController> _logger;

    public HomeController(ILogger<HomeController> logger)
    {
        _logger = logger;
    }
    
    [AllowAnonymous]
    public async Task<IActionResult> Index()
    {
        return View();
    }

    public async Task<IActionResult> Home()
    {
        TempData["lang"] = "sv";
        return View();
    }

    [AllowAnonymous]
    [HttpPost]
    public string TestESP([FromBody]List<int> list){
        return "success";
    }

    public async Task<IActionResult> ShowChart()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = System.Diagnostics.Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
