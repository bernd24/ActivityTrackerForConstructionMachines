using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;
using UIAutomate.Models;
using System;  
using System.Collections.Generic;  
using System.Linq;  
using System.Web;  
using System.Web.Mvc; 

namespace Server.Controllers;

public class SUSTestController : Controller
{
    private readonly ILogger<SUSTestController> _logger;
    public SUSTestDBContext Db { get; set;}

    // public SUSTestDBContext context = new SUSTestDBContext();
    public SUSTestController(ILogger<SUSTestController> logger, SUSTestDBContext db)
    {
        _logger = logger;
        Db = db;
    }

        // [HttpGet]  
    // public ActionResult Formular()  

    [HttpGet]
    public async Task<IActionResult> Formular()
    {
        // Db = new SUSTestDBContext("server=localhost;userid=test;password=IbLN+h1UIP;port=3306;database=UITestDatabase;");
        await Db.Connection.OpenAsync();
        var query = new Query(Db);
        var result = await query.LatestPostsAsync();
        // var listOfData = Db.SUSTests.ToList();
        return View(result);
    }

    [HttpPost]  
    public async Task<IActionResult> Create([FromBody]SUSTest m)
    {              
        // var sustest = new SUSTest();
        // sustest.Question = "new question";
        // sustest.StronglyDisagree = true;
        // sustest.Disagree = true;
        // sustest.Neutral = true;
        // sustest.Agree = true;
        // sustest.StronglyAgree = true;

        await Db.Connection.OpenAsync();
        var query = new Query(Db);
        m.Db = Db;
        await m.UpdateAsync();
        return View();



        // Db.SUSTests.Add(sustest);  
        // Db.SaveChanges();  
        // ViewBag.Message = "Data Insert Successfully";  
        // return View();  
    }  

    [HttpGet]
    public async Task<IActionResult> Introduction()
    {
        await Db.Connection.OpenAsync();
        var query = new Query(Db);
        var result = await query.LatestPostsAsync();
        return View(result);
    }

    // [HttpPost]
    // public async Task<IActionResult> Create([FromBody]SUSTest m)
    // {
    //     await Db.Connection.OpenAsync();
    //     m.Db = Db;
    //     await m.InsertAsync();
    //     return View();
    // }   

    [HttpPost]
    async Task<IActionResult> Update([FromBody]SUSTest m)
    {
        await Db.Connection.OpenAsync();
        m.Db = Db;
        await m.UpdateAsync();
        try{
            Db.SUSTests.Add(m);
            Db.SaveChanges();
            return RedirectToAction("Introduction");
         }catch{
            return View(m);
         }
    }

    [HttpPost]
     public ActionResult Formular([FromBody]SUSTest m)
    {
        ViewBag.Message = string.Format("Hello {0}.\\nCurrent Date and Time: {1}", DateTime.Now.ToString());
        // return View();

        // await Db.Connection.OpenAsync();
        // m.Db = Db;
        // await m.UpdateAsync(m);
        var sustest = new SUSTest();
        sustest.Question = "new question";
        sustest.StronglyDisagree = true;
        sustest.Disagree = true;
        sustest.Neutral = true;
        sustest.Agree = true;
        sustest.StronglyAgree = true;
        // await Db.Connection.OpenAsync();



        // Db.SUSTests.Add(sustest);
        // Db.SUSTests.FirstOrDefault().UpdateAsync(sustest);
        // Db.SaveChanges();
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
