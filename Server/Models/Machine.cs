using System.Data;
using System.Threading.Tasks;
using MySqlConnector;
using SqlKata.Execution;

namespace Server.Models{
    public class machine{
        public int Id { get; set; }
        public int M_Id { get; set; }
        public int? C_Id { get; set; }
        public string InternalId { get; set; }
        public bool inUse { get; set; }
    }

    public class Machine
    {
        public int Id { get; set; }
        public MachineModel Model { get; set; }
        public SensorConfiguration Config { get; set; }
        public string InternalId { get; set; }
        public bool inUse { get; set; }
    }
}