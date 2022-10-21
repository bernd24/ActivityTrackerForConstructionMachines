using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;
using Microsoft.AspNetCore.Authorization;
using Server.JWT;

namespace Server.Controllers;

[Authorize]
public class HomeController : Controller
{
    

    public HomeController()
    {

    }
    
    [AllowAnonymous]
    public IActionResult Index()
    {
        return View();
    }

    public IActionResult Home()
    {
        return View();
    }

    public IActionResult Overview()
    {
        return View();
    }

    [AllowAnonymous]
    [HttpPost]
    public string TestESP([FromBody]List<int> list){
        return "success";
    }

    public IActionResult ShowChart()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = System.Diagnostics.Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
