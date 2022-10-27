using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class Measurement
    {
        public int WS_Id { get; set; }
        public int SI_Id { get; set; }

        public int Id { get; set; }
        public WorkSession Session { get; set; }
        public SensorInstance Sensor { get; set; }
        public int Nr { get; set; }
        public DateTime TimeOfMeasure { get; set; }
        public float SensorData { get; set; }
    }
}