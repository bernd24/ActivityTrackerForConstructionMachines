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

    public IActionResult Index(int Id, DateTime fromDate, DateTime fromTime, DateTime tillDate, DateTime tillTime)
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

        DateTime from = new DateTime(fromDate.Year,fromDate.Month,fromDate.Day,fromTime.Hour,fromTime.Minute,fromTime.Second);
        DateTime till = new DateTime(tillDate.Year,tillDate.Month,tillDate.Day,tillTime.Hour,tillTime.Minute,tillTime.Second);
        DateTime lowestFrom = new DateTime();

        var ret = new List<NodeData>();

        foreach(var s in sensors){
            SqlKata.Query query;
            if(fromDate.Year == 1){
                query = Db.Query("Measurement")
                .Where("WS_Id",Id)
                .Where("SI_Id",(int)s.SI_Id)
                .OrderBy("TimeOfMeasure")
                .OrderBy("Nr");
            }
            else{
                query = Db.Query("Measurement")
                .Where("WS_Id",Id)
                .Where("SI_Id",(int)s.SI_Id)
                .Where("TimeOfMeasure",">",from.AddSeconds(-10))
                .Where("TimeOfMeasure","<",till)
                .OrderBy("TimeOfMeasure")
                .OrderBy("Nr");
            }
            var result = query.Get<Measurement>();

            if(result.Count() > 0 && from.Year == 1 && (lowestFrom.Year == 1 || lowestFrom > result.First().TimeOfMeasure)){
                from = result.First().TimeOfMeasure;
            }

            List<Point> datapoints = new List<Point>();
            int i = 0;
            foreach(Measurement m in result){
                if(m.Nr == 0){
                    i = 0;
                }
                DateTime NrTime = m.TimeOfMeasure.AddMilliseconds(i*100);
                if(tillDate.Year != 1 && NrTime > till){
                    break;
                }
                if(NrTime >= from){
                    datapoints.Add(new Point{x = ((DateTimeOffset)NrTime).ToUnixTimeMilliseconds(), y = m.SensorData});
                }
                if(NrTime > till){
                    till = NrTime;
                }
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
                datapoints = datapoints
            });
        }
        if(from.Year == 1){
            from = lowestFrom;
        }
        ViewBag.fromDate = from;
        ViewBag.fromTime = from;
        ViewBag.tillDate = till;
        ViewBag.tillTime = till;
        return View(ret);
    }


    public IActionResult SpecificMaschine()
    {
        return View();
    }

    public IActionResult Timeline(){
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = System.Diagnostics.Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
