using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class SensorNode
    {
        public int Id { get; set; }
        public string MAC { get; set; }
        public int BatteryStatus { get; set; }
    }
}