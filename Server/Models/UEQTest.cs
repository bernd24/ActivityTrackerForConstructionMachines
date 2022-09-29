using System.Data;
using System.Threading.Tasks;
using MySqlConnector;
using System.ComponentModel.DataAnnotations;

namespace UIAutomate.Models{
    
    public class UEQTest
    {
        [Key]
        public int Id { get; set; }
        public string LeftWord {get; set; }
        public string RightWord {get; set;}
        public bool one {get; set;}
        public bool two {get; set;}
        public bool three {get; set;}
        public bool four {get; set;}
        public bool five {get; set;}
        public bool six {get; set;}
        public bool seven {get; set;}

        // public int TestpersonId {get; set;}
        public Testperson Testperson {get; set; }
    }
}