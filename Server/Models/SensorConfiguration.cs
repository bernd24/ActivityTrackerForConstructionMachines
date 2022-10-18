using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class SensorConfiguration
    {
        public List<SensorNodeInstance> SensorNodes { get; set; }
        public int Id { get; set; }
        public string Picture { get; set; }
        public string Notes { get; set; }

        public bool hasBinary {get; set;}
    }
}