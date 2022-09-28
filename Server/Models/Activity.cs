using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class Activity
    {
        public int Id { get; set; }
        public string ActivityName { get; set; }
        public float FuelEstimate { get; set; }
    }
}