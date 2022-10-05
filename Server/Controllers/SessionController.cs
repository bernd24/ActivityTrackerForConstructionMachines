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
    private readonly ILogger<SessionController> _logger;
    private readonly QueryFactory Db;

    public SessionController(ILogger<SessionController> logger, QueryFactory db)
    {
        _logger = logger;
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
            .Where("Sensor.SensorName",hs.ID)
            .Where("SensorInstance.SN_Id",handshakes.node_ID)
            .Join("Sensor","Sensor.Id","SensorInstance.S_Id")
            .Get();
            foreach(var si in si_list){
                switch(hs.ID){
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
                                            Nr = i+3,
                                            SI_Id = si.Id
                                        });
                                        break;
                                    case "Y":
                                        Db.Query("Handshake")
                                        .Insert(new {
                                            SNI_Id = handshakes.node_ID,
                                            Nr = i+4,
                                            SI_Id = si.Id
                                        });                            
                                        break;
                                    case "Z":
                                        Db.Query("Handshake")
                                        .Insert(new {
                                            SNI_Id = handshakes.node_ID,
                                            Nr = i+5,
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
            }
            i += hs.elements;
        }
        return "success";
    }

    [AllowAnonymous]
    [HttpPost]
    public string Data([FromBody]List<SensorData<float>> data){
        foreach(SensorData<float> d in data){
            IEnumerable<dynamic> handshake = Db.Query("Handshake")
            .Where("SNI_Id",d.node_ID)
            .OrderBy("Nr")
            .Get();

            int sensorNr = 0;
            int packetNr = 0;
            foreach(float f in d.payload){
                Db.Query("Measurement").Insert(new {
                    WS_Id = 1,
                    SI_Id = handshake.ElementAt(sensorNr).SI_Id,
                    Nr = packetNr,
                    TimeOfMeasure = DateTime.Now,
                    SensorData = f
                });
                sensorNr++;
                if(sensorNr == handshake.Count()){
                    sensorNr = 0;
                    packetNr++;
                }
            }
        }
        return "success";
    }
}