using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class Sensor
    {
        public int Id { get; set; }
        public string SensorName { get; set; }
        public string SensorType { get; set; }
        public string Manufacturer { get; set; }
    }
}