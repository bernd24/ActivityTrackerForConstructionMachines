using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class WorkSessionActivity
    {
        public int WS_Id { get; set; }
        public int A_Id { get; set; }

        public int Id { get; set; }
        public DateTime StartTime { get; set; }
        public DateTime EndTime { get; set; }
        public WorkSession Session { get; set; }
        public Activity Activity { get; set; }
    }
}