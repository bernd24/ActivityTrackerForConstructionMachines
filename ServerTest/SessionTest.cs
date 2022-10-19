namespace ServerTest;
using Server.Controllers;
using SqlKata;
using SqlKata.Execution;
using SqlKata.Compilers;
using MySqlConnector;
using Server.Models;
using System.Text;

[TestClass]
public class SessionTest
{
    QueryFactory db;
    private static readonly HttpClient client = new HttpClient();

    public SessionTest(){
        var connection = new MySqlConnection(
            "server=127.0.0.1;user id=root;password=atfcm2022;port=3306;database=atfcm;"
        );
        var compiler = new MySqlCompiler();
        db = new QueryFactory(connection, compiler);
    }

    [TestMethod]
    public async Task HandshakeTest1()
    {
        db.Query("Handshake")
        .Where("SNI_Id",1)
        .Delete();

        var json = "{\"node_ID\": 1,\"payload\": [{\"ID\":\"MPU6050\",\"elements\": 3},{\"ID\":\"HC-SR04\",\"elements\": 1}]}";
        
        var content = new StringContent(json, Encoding.UTF8, "application/json");
        
        var response = await client.PostAsync("http://localhost:7209/Session/Handshake", content);

        Assert.AreEqual(response.StatusCode,System.Net.HttpStatusCode.OK);

        var result = db.Query("Handshake")
        .Where("SNI_Id",1)
        .Get();

        Assert.AreEqual(result.Count(),4);

        foreach(var hs in result){
            switch(hs.SI_Id){
                case 1:
                    Assert.AreEqual(hs.Nr,0,"wrong order");
                    break;
                case 2:
                    Assert.AreEqual(hs.Nr,1,"wrong order");
                    break;
                case 3:
                    Assert.AreEqual(hs.Nr,2,"wrong order");
                    break;
                case 7:
                    Assert.AreEqual(hs.Nr,3,"wrong order");
                    break;
            }
        }
    }

    [TestMethod]
    public async Task HandshakeTest2()
    {
        db.Query("Handshake")
        .Where("SNI_Id",1)
        .Delete();

        var json = "{\"node_ID\": 1,\"payload\": [{\"ID\":\"HC-SR04\",\"elements\": 1},{\"ID\":\"MPU6050\",\"elements\": 6}]}";
        
        var content = new StringContent(json, Encoding.UTF8, "application/json");
        
        var response = await client.PostAsync("http://localhost:7209/Session/Handshake", content);

        Assert.AreEqual(response.StatusCode,System.Net.HttpStatusCode.OK);

        var result = db.Query("Handshake")
        .Where("SNI_Id",1)
        .Get();
        
        Assert.AreEqual(result.Count(),7);

        foreach(var hs in result){
            switch(hs.SI_Id){
                case 1:
                    Assert.AreEqual(hs.Nr,4,"wrong order");
                    break;
                case 2:
                    Assert.AreEqual(hs.Nr,5,"wrong order");
                    break;
                case 3:
                    Assert.AreEqual(hs.Nr,6,"wrong order");
                    break;
                case 4:
                    Assert.AreEqual(hs.Nr,1,"wrong order");
                    break;
                case 5:
                    Assert.AreEqual(hs.Nr,2,"wrong order");
                    break;
                case 6:
                    Assert.AreEqual(hs.Nr,3,"wrong order");
                    break;
                case 7:
                    Assert.AreEqual(hs.Nr,0,"wrong order");
                    break;
            }
        }
    }

    [TestMethod]
    public async Task DataTest1()
    {
        db.Query("Handshake")
        .Where("SNI_Id",1)
        .Delete();

        var json = "{\"node_ID\": 1,\"payload\": [{\"ID\":\"HC-SR04\",\"elements\": 1},{\"ID\":\"MPU6050\",\"elements\": 6}]}";
        
        var content = new StringContent(json, Encoding.UTF8, "application/json");
        
        var response = await client.PostAsync("http://localhost:7209/Session/Handshake", content);

        Assert.AreEqual(response.StatusCode,System.Net.HttpStatusCode.OK);

        db.Query("Measurement")
        .Delete();

        var json2 = "[{\"node_ID\": 1,\"payload\": [24.4, 53.223, 34.21, 324.2, 235.2, 324.1, 90.4,      60.122, 83.6, 78.2, 56.3, 23.5, 87.7, 566.9]}]";
        
        var content2 = new StringContent(json2, Encoding.UTF8, "application/json");
        
        var response2 = await client.PostAsync("http://localhost:7209/Session/Data", content2);

        Assert.AreEqual(response2.StatusCode,System.Net.HttpStatusCode.OK);

        var result = db.Query("Measurement")
        .Get();

        Assert.AreEqual(result.Count(),14);

        foreach(var m in result){
            switch(m.SI_Id){
                case 7:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 24.4f : 60.122f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 4:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 53.223f : 83.6f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 5:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 34.21f : 78.2f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 6:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 324.2f : 56.3f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 1:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 235.2f : 23.5f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 2:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 324.1f : 87.7f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 3:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 90.4f : 566.9f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
            }
        }
    }

    [TestMethod]
    public async Task DataTest2()
    {
        db.Query("Handshake")
        .Where("SNI_Id",1)
        .Delete();

        var json = "{\"node_ID\": 1,\"payload\": [{\"ID\":\"MPU6050\",\"elements\": 3},{\"ID\":\"HC-SR04\",\"elements\": 1}]}";
        
        var content = new StringContent(json, Encoding.UTF8, "application/json");
        
        var response = await client.PostAsync("http://localhost:7209/Session/Handshake", content);

        Assert.AreEqual(response.StatusCode,System.Net.HttpStatusCode.OK);

        db.Query("Measurement")
        .Delete();

        var json2 = "[{\"node_ID\": 1,\"payload\": [24.4, 53.223, 34.21, 324.2,      60.122, 83.6, 78.2, 56.3]}]";
        
        var content2 = new StringContent(json2, Encoding.UTF8, "application/json");
        
        var response2 = await client.PostAsync("http://localhost:7209/Session/Data", content2);

        Assert.AreEqual(response2.StatusCode,System.Net.HttpStatusCode.OK);

        var result = db.Query("Measurement")
        .Get();

        Assert.AreEqual(result.Count(),8);

        foreach(var m in result){
            switch(m.SI_Id){
                case 4:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 24.4f : 60.122f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 5:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 53.223f : 83.6f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 6:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 34.21f : 78.3f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
                case 7:
                    Assert.AreEqual(m.SensorData,(m.Nr == 0 ? 324.2f : 56.3f));
                    Assert.IsTrue(m.TimeOfMeasure < DateTime.Now.AddSeconds(2) && m.TimeOfMeasure > DateTime.Now.AddSeconds(-2),"wrong timestamp");
                    break;
            }
        }
    }
}