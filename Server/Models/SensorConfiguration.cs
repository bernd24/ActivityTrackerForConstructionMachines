using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class SensorConfiguration
    {
        public int Id { get; set; }
        public string Picture { get; set; }
        public string Notes { get; set; }

        internal AppDb Db { get; set; }

        public SensorConfiguration()
        {
        }

        internal SensorConfiguration(AppDb db)
        {
            Db = db;
        }

        public async Task InsertAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"INSERT INTO SensorConfiguration (Picture,Notes) VALUES (@picture, @notes);";
            BindParams(cmd);
            await cmd.ExecuteNonQueryAsync();
            Id = (int) cmd.LastInsertedId;
        }

        public async Task UpdateAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"UPDATE SensorConfiguration SET Picture = @picture, Notes = @notes WHERE `Id` = @id;";
            BindParams(cmd);
            BindId(cmd);
            await cmd.ExecuteNonQueryAsync();
        }

        public async Task DeleteAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"DELETE FROM SensorConfiguration WHERE `Id` = @id;";
            BindId(cmd);
            await cmd.ExecuteNonQueryAsync();
        }

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
                ParameterName = "@picture",
                DbType = DbType.String,
                Value = Picture,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@notes",
                DbType = DbType.String,
                Value = Notes,
            });
        }
    }
}