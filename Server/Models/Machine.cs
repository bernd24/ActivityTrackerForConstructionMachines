using System.Data;
using System.Threading.Tasks;
using MySqlConnector;

namespace Server.Models{
    public class Machine
    {
        public int Id { get; set; }
        public MachineModel Model { get; set; }
        public SensorConfiguration Config { get; set; }
        public string InternalId { get; set; }
        public bool inUse { get; set; }

        internal AppDb Db { get; set; }

        public Machine()
        {
        }

        internal Machine(AppDb db)
        {
            Db = db;
        }

        public async Task InsertAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"INSERT INTO Machine (M_Id,C_Id,InternalId,inUse) VALUES (@modelId, @configId, @internalId, @inUse);";
            BindParams(cmd);
            await cmd.ExecuteNonQueryAsync();
            Id = (int) cmd.LastInsertedId;
        }

        public async Task UpdateAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"UPDATE Machine SET M_Id = @modelId, C_Id = @configId, InternalId = @internalId, inUse = @inUse WHERE `Id` = @id;";
            BindParams(cmd);
            BindId(cmd);
            await cmd.ExecuteNonQueryAsync();
        }

        public async Task DeleteAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"DELETE FROM Machine WHERE `Id` = @id;";
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
                ParameterName = "@modelId",
                DbType = DbType.String,
                Value = Model.Id,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@configId",
                DbType = DbType.String,
                Value = Config.Id,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@internalId",
                DbType = DbType.String,
                Value = InternalId,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@inUse",
                DbType = DbType.String,
                Value = inUse,
            });
        }
    }
}