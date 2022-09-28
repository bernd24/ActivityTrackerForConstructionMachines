using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace UIAutomate.Models{
    
    public class SUSTest
    {
        public int Id { get; set; }

        public string Question {get; set; }
        public bool StronglyDisagree {get; set;}
        public bool Disagree {get; set;}
        public bool Neutral {get; set;}
        public bool Agree {get; set;}
        public bool StronglyAgree {get; set;}

    }
}