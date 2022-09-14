﻿using System.Diagnostics;
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

    [HttpGet]
    public async Task<IActionResult> Index()
    {
        await Db.Connection.OpenAsync();
        var query = new Query(Db);
        var result = await query.LatestPostsAsync();
        return View(result);
    }

    [HttpPost]
    public async Task<IActionResult> Create()
    {
        await Db.Connection.OpenAsync();
        var newMachine = new Machine(Db);
        newMachine.Manufacturer = "Volvo";
        newMachine.Model = "EC850";
        newMachine.MachineType = "Excavator";
        await newMachine.InsertAsync();
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
