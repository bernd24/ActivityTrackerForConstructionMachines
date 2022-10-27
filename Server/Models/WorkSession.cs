using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class WorkSession
    {
        public int C_Id { get; set; }
        public int M_Id { get; set; }

        public int Id { get; set; }
        public string Worker { get; set; }
        public string Notes { get; set; }
        public SensorConfiguration Config { get; set; }
        public Machine Machine { get; set; }
        public bool isProcessed {get; set;}
    }
}