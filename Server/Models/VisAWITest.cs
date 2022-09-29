using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace UIAutomate.Models{
    
    public class VisAWITest
    {
        public int Id { get; set; }

        public string Question {get; set; }
        public bool DisagreeEtAll {get; set;}
        public bool Disagree {get; set;}
        public bool DisagreeRather {get; set;}
        public bool Neutral {get; set;}
        public bool AgreeRather {get; set;}
        public bool Agree {get; set;}
        public bool AgreeEtAll {get; set;}

    }
}