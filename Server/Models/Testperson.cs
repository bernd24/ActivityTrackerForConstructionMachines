using System.Data;
using System.Threading.Tasks;
using MySqlConnector;
using System.ComponentModel.DataAnnotations;

namespace UIAutomate.Models{
    
    public class Testperson
    {
        [Key]
        public int Id { get; set; }

        public string anonymName {get; set; }

        public List<SUSTest> SUSTests {get; set;} = new List<SUSTest>();
        public List<UEQTest> UEQTests {get; set;} = new List<UEQTest>();
        public List<VisAWITest> VisAWITests {get; set;} = new List<VisAWITest>();

    }
}