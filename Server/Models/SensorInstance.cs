using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class SensorInstance
    {
        public int SN_Id { get; set; }
        public int S_Id { get; set; }
        public int? A_Id { get; set; }

        public int Id { get; set; }
        public SensorNodeInstance SensorNode { get; set; }
        public Sensor Sensor { get; set; }
        public string RssiPartner { get; set; }
        public Activity Activity { get; set; }
        public string Axis { get; set; }
        public string Position { get; set; }
    }
}