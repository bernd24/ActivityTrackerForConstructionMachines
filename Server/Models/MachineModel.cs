using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class MachineModel
    {
        public int Id { get; set; }
        public string Manufacturer { get; set; }
        public string ModelName { get; set; }
        public string MachineType { get; set; }
        public int ModelYear { get; set; }
    }
}