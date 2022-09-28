using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class SensorNodeInstance
    {
        public int C_Id { get; set; }
        public int SN_Id { get; set; }

        public int Id { get; set; }
        public SensorConfiguration Config { get; set; }
        public SensorNode SensorNode { get; set; }
        public bool hasBattery { get; set; }
        public bool isMaster { get; set; }
        public string Notes { get; set; }
    }
}