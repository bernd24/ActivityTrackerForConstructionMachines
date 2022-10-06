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
        Db.Query("SensorNodeInstance")
        .Where("Id",handshakes.node_ID)
        .Update(new {
            elementCount = i
        }); 
        return "success";
    }

    [AllowAnonymous]
    [HttpPost]
    public string Data([FromBody]List<SensorData<float>> data){
        DateTime timestamp = DateTime.Now;
        foreach(SensorData<float> d in data){
            IEnumerable<dynamic> handshake = Db.Query("Handshake")
            .Where("SNI_Id",d.node_ID)
            .Get();

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

            for(int packetNr = 0; (packetNr+1)*sni.elementCount < d.payload.Count; packetNr++){
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
        return "success";
    }

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
}