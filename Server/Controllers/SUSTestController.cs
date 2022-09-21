using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;
using UIAutomate.Models;

namespace Server.Controllers;

public class SUSTestController : Controller
{
    private readonly ILogger<SUSTestController> _logger;
    public AppDb Db { get; }

    public SUSTestController(ILogger<SUSTestController> logger, AppDb db)
    {
        _logger = logger;
        Db = db;
    }

    [HttpGet]
    public async Task<IActionResult> Index()
    {
        await Db.Connection.OpenAsync();
        var query = new Query(Db);
        var result = await query.LatestPostsAsync();
        return View(result);
    }

    [HttpPost]
    public async Task<IActionResult> Create([FromBody]SUSTest m)
    {
        await Db.Connection.OpenAsync();
        m.Db = Db;
        await m.InsertAsync();
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
