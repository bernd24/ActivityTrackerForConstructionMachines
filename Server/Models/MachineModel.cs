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

        internal AppDb Db { get; set; }

        public MachineModel()
        {
        }

        internal MachineModel(AppDb db)
        {
            Db = db;
        }

        public async Task InsertAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"INSERT INTO MachineModel (Manufacturer,ModelName,MachineType) VALUES (@manufacturer, @modelName, @machineType);";
            BindParams(cmd);
            await cmd.ExecuteNonQueryAsync();
            Id = (int) cmd.LastInsertedId;
        }

        public async Task UpdateAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"UPDATE MachineModel SET Manufacturer = @manufacturer, ModelName = @modelName, MachineType = @machineType WHERE `Id` = @id;";
            BindParams(cmd);
            BindId(cmd);
            await cmd.ExecuteNonQueryAsync();
        }

        public async Task DeleteAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"DELETE FROM MachineModel WHERE `Id` = @id;";
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
                ParameterName = "@manufacturer",
                DbType = DbType.String,
                Value = Manufacturer,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@modelName",
                DbType = DbType.String,
                Value = ModelName,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@machineType",
                DbType = DbType.String,
                Value = MachineType,
            });
        }
    }
}