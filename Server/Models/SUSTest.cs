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

        internal AppDb Db { get; set; }

        public SUSTest()
        {
        }

        internal SUSTest(AppDb db)
        {
            Db = db;
        }

        public async Task InsertAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            // cmd.CommandText = @"INSERT INTO SUSTest (Manufacturer,Model,SUSTestType) VALUES (@manufacturer, @model, @SUSTestType);";
            cmd.CommandText = @"INSERT INTO SUSTest (Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree) VALUES (@Question,@StronglyDisagree, @Disagree, @Neutral, @Agree, @StronglyAgree);";           
            BindParams(cmd);
            await cmd.ExecuteNonQueryAsync();
            Id = (int) cmd.LastInsertedId;
        }

        // public async Task UpdateAsync()
        // {
        //     using var cmd = Db.Connection.CreateCommand();
        //     cmd.CommandText = @"UPDATE SUSTest SET Manufacturer = @manufacturer, Model = @model, SUSTestType = @SUSTestType WHERE `Id` = @id;";
        //     BindParams(cmd);
        //     BindId(cmd);
        //     await cmd.ExecuteNonQueryAsync();
        // }

        // public async Task DeleteAsync()
        // {
        //     using var cmd = Db.Connection.CreateCommand();
        //     cmd.CommandText = @"DELETE FROM SUSTest WHERE `Id` = @id;";
        //     BindId(cmd);
        //     await cmd.ExecuteNonQueryAsync();
        // }

        private void BindId(MySqlCommand cmd)
        {
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@id",
                DbType = DbType.Int32,
                Value = Id,
            });
        }

        private void BindParams(MySqlCommand cmd)
        {
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@Question",
                DbType = DbType.String,
                Value = Question,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@StronglyDisagree",
                DbType = DbType.Boolean,
                Value = StronglyDisagree,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@Disagree",
                DbType = DbType.Boolean,
                Value = Disagree,
            });
                        cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@Neutral",
                DbType = DbType.Boolean,
                Value = Neutral,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@Agree",
                DbType = DbType.Boolean,
                Value = Agree,
            });
                        cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@StronglyAgree",
                DbType = DbType.Boolean,
                Value = StronglyAgree,
            });
        }
    }
}