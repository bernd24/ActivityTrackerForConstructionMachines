using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class SensorInstance
    {
        public int SN_Id { get; set; }
        public int S_Id { get; set; }
        public int RssiPartner { get; set; }
        public int A_Id { get; set; }

        public int Id { get; set; }
        public SensorNodeInstance SensorNode { get; set; }
        public Sensor Sensor { get; set; }
        public SensorInstance Partner { get; set; }
        public Activity Activity { get; set; }
    }
}