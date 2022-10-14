using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;
using Microsoft.AspNetCore.Authorization;
using SqlKata;
using SqlKata.Execution;
using System.Data.SqlClient;
using System.Globalization;


namespace Server.Controllers;

[Authorize]
public class ChartController : Controller
{
    private readonly ILogger<ChartController> _logger;
    private readonly QueryFactory Db;

    public ChartController(ILogger<ChartController> logger, QueryFactory db)
    {
        _logger = logger;
        Db = db;
    }
    [AllowAnonymous]
    public IActionResult Index(int Id)
    {
        System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
        customCulture.NumberFormat.NumberDecimalSeparator = ".";

        System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;
        var sensors = Db.Query("Measurement")
        .Select("Sensor.SensorName","Sensor.SensorType","SensorInstance.Axis","Measurement.SI_Id","SensorInstance.SN_Id")
        .Where("WS_Id",Id)
        .GroupBy("SI_Id")
        .Join("SensorInstance","SensorInstance.Id","Measurement.SI_Id")
        .Join("Sensor","Sensor.Id","SensorInstance.S_Id")
        .OrderBy("SensorInstance.SN_Id")
        .Get();


        var ret = new List<NodeData>();

        foreach(var s in sensors){
            var result = Db.Query("Measurement")
            .Where("WS_Id",Id)
            .Where("SI_Id",(int)s.SI_Id)
            .OrderBy("TimeOfMeasure")
            .OrderBy("Nr")
            .Get<Measurement>();
            List<float> datapoints = new List<float>();
            List<String> timestamps = new List<String>();
            int i = 0;
            foreach(Measurement m in result){
                if(m.Nr == 0){
                    i = 0;
                }
                datapoints.Add(m.SensorData);
                timestamps.Add(m.TimeOfMeasure.AddMilliseconds(i*100).ToString());
                i++;
            }
            if(ret.Count == 0 || ret.Last().SN_Id != s.SN_Id){
                ret.Add(new NodeData{
                    SN_Id = s.SN_Id,
                    data = new List<MeasureData>()
                });
            }
            ret.Last().data.Add(new MeasureData{
                sensorName = s.SensorName + " " + s.SensorType + (s.Axis == null ? "" : " " + s.Axis + "-Axis"),
                datapoints = datapoints,
                timestamps = timestamps
            });
        }
        return View(ret);
    }

    [AllowAnonymous]
    public IActionResult SpecificMaschine()
    {
        return View();
    }


    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = System.Diagnostics.Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
