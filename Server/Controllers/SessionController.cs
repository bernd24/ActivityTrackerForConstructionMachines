using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Server.Models;
using Microsoft.AspNetCore.Authorization;
using Server.JWT;
using Microsoft.AspNetCore.Authorization;
using SqlKata;
using SqlKata.Execution;
using System.Data.SqlClient;

namespace Server.Controllers;

[Authorize]
public class SessionController : Controller
{
    private readonly QueryFactory Db;

    public SessionController(QueryFactory db)
    {
        Db = db;
    }

    [AllowAnonymous]
    [HttpPost]
    public string Handshake([FromBody]SensorData<Handshake> handshakes){
        Db.Query("Handshake")
        .Where("SNI_Id",handshakes.node_ID)
        .Delete();

        int i = 0;
        foreach(Handshake hs in handshakes.payload){
            IEnumerable<dynamic> si_list = Db.Query("SensorInstance")
            .Select("SensorInstance.Id","Sensor.SensorType","SensorInstance.Axis")
            .Where("Sensor.SensorName",hs.id)
            .Where("SensorInstance.SN_Id",handshakes.node_ID)
            .Join("Sensor","Sensor.Id","SensorInstance.S_Id")
            .OrderBy("SensorInstance.Id")
            .Get();
            int assignedSensors = 0;
            foreach(var si in si_list){
                switch(hs.id){
                    case "MPU6050": 
                        switch(si.SensorType){
                            case "Accelerometer":
                                switch(si.Axis){
                                    case "X":
                                        Db.Query("Handshake")
                                        .Insert(new {
                                            SNI_Id = handshakes.node_ID,
                                            Nr = i,
                                            SI_Id = si.Id
                                        });
                                        break;
                                    case "Y":
                                        Db.Query("Handshake")
                                        .Insert(new {
                                            SNI_Id = handshakes.node_ID,
                                            Nr = i+1,
                                            SI_Id = si.Id
                                        });                            
                                        break;
                                    case "Z":
                                        Db.Query("Handshake")
                                        .Insert(new {
                                            SNI_Id = handshakes.node_ID,
                                            Nr = i+2,
                                            SI_Id = si.Id
                                        });                            
                                        break;
                                }
                                break;
                            case "Gyrometer":
                                switch(si.Axis){
                                    case "X":
                                        Db.Query("Handshake")
                                        .Insert(new {
                                            SNI_Id = handshakes.node_ID,
                                            Nr = hs.elements == 6 ? i+3 : i,
                                            SI_Id = si.Id
                                        });
                                        break;
                                    case "Y":
                                        Db.Query("Handshake")
                                        .Insert(new {
                                            SNI_Id = handshakes.node_ID,
                                            Nr = hs.elements == 6 ? i+4 : i+1,
                                            SI_Id = si.Id
                                        });                            
                                        break;
                                    case "Z":
                                        Db.Query("Handshake")
                                        .Insert(new {
                                            SNI_Id = handshakes.node_ID,
                                            Nr = hs.elements == 6 ? i+5 : i+2,
                                            SI_Id = si.Id
                                        });                            
                                        break;
                                }
                                break;                                
                        }
                        break;
                    default:
                        Db.Query("Handshake")
                        .Insert(new {
                            SNI_Id = handshakes.node_ID,
                            Nr = i,
                            SI_Id = si.Id
                        }); 
                        break;
                }
                assignedSensors++;
                if(assignedSensors >= hs.elements){
                    break;
                }
            }
            i += hs.elements;
        }
        Db.Query("SensorNodeInstance")
        .Where("Id",handshakes.node_ID)
        .Update(new {
            elementCount = i
        }); 
        return "success";
    }

    [AllowAnonymous]
    [HttpPost]
    public IActionResult Data([FromBody]List<SensorData<float>> data){
        DateTime timestamp = DateTime.Now;
        foreach(SensorData<float> d in data){
            IEnumerable<dynamic> handshake = Db.Query("Handshake")
            .Where("SNI_Id",d.node_ID)
            .Get();

            if(handshake.Count() == 0){
                return Ok("no handshake");
            }

            SensorNodeInstance sni = Db.Query("SensorNodeInstance")
            .Where("Id",d.node_ID)
            .Get<SensorNodeInstance>().First();

            IEnumerable<dynamic> lastSessionQuery = Db.Query("WorkSession")
            .Select("Measurement.TimeOfMeasure","WorkSession.Id")
            .Where("WorkSession.C_Id",sni.C_Id)
            .OrderByDesc("Measurement.TimeOfMeasure")
            .LeftJoin("Measurement","WorkSession.Id","Measurement.WS_Id")
            .Limit(1)
            .Get();

            Machine m;
            IEnumerable<Machine> mQuery = Db.Query("Machine")
            .Where("C_Id",sni.C_Id)
            .Get<Machine>();
            if(mQuery.Count() == 0){
                continue;
            }
            m = mQuery.First();

            int WS_Id;
            if(lastSessionQuery.Count() > 0 && lastSessionQuery.First().TimeOfMeasure != null){
                dynamic lastSession = lastSessionQuery.First();
                
                if(lastSession.TimeOfMeasure.AddMinutes(2) < DateTime.Now){
                    WS_Id = createSession(sni.C_Id,m.Id);
                }
                else{
                    WS_Id = lastSession.Id;
                }
            }
            else{
                WS_Id = createSession(sni.C_Id,m.Id);
            }

            for(int packetNr = 0; (packetNr+1)*sni.elementCount <= d.payload.Count; packetNr++){
                foreach(var hs in handshake){
                    Db.Query("Measurement").Insert(new {
                        WS_Id = WS_Id,
                        SI_Id = hs.SI_Id,
                        Nr = packetNr,
                        TimeOfMeasure = timestamp,
                        SensorData = d.payload[hs.Nr+packetNr*sni.elementCount]
                    });
                }
            }
        }

        return Ok();
    }

    [NonAction]
    public int createSession(int C_Id, int M_Id){
        Db.Query("WorkSession").Insert(new{
            C_Id = C_Id,
            M_Id = M_Id
        });
        return Db.Query("WorkSession")
        .OrderByDesc("WorkSession.Id")
        .Limit(1)
        .Get<WorkSession>().First().Id;
    }

    [HttpPost]
    public async Task<ActionResult> LogData(IFormFile log, int SI_Id, int WS_Id, DateTime time, DateTime date){
        var filePath = Path.GetTempFileName();
        if(log.Length > 0){
            using(var stream = System.IO.File.Create(filePath)){
                await log.CopyToAsync(stream);
            }
        }
        string text = System.IO.File.ReadAllText(filePath);
        string[] values = text.Split(' '); 
        int i = 0;
        DateTime timestamp = new DateTime(date.Year,date.Month,date.Day,time.Hour,time.Minute,time.Second);
        foreach(string val in values){
            if(i == 100){
                i = 0;
                timestamp = timestamp.AddSeconds(10);
            }
            Db.Query("Measurement")
            .Insert(new{
                WS_Id = WS_Id,
                SI_Id = SI_Id,
                Nr = i,
                TimeOfMeasure = timestamp,
                SensorData = float.Parse(val, System.Globalization.CultureInfo.InvariantCulture.NumberFormat)
            });
            i++;
        }
        return Ok();
    }

    [HttpGet]
    public IActionResult LogData(){
        return View();
    }

    [HttpGet("Session/MachineSessions/{M_Id}")]
    public IActionResult MachineSessions(int M_Id){
        return Json(Db.Query("WorkSession").Where("M_Id",M_Id).Get());
    }

    [HttpGet("Session/Data/{WS_Id}")]
    public IActionResult Data(int WS_Id){
        return Json(Db.Query("Measurement").Where("WS_Id",WS_Id).Get());
    }
}