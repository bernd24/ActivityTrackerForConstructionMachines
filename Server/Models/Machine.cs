using System.Data;
using System.Threading.Tasks;
using MySqlConnector;


    public class Machine
    {
        public int Id { get; set; }
        public string Manufacturer { get; set; }
        public string Model { get; set; }
        public string MachineType { get; set; }

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
            cmd.CommandText = @"INSERT INTO Machine (Manufacturer,Model,MachineType) VALUES (@manufacturer, @model, @machineType);";
            BindParams(cmd);
            await cmd.ExecuteNonQueryAsync();
            Id = (int) cmd.LastInsertedId;
        }

        public async Task UpdateAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"UPDATE Machine SET Manufacturer = @manufacturer, Model = @model, MachineType = @machineType WHERE `Id` = @id;";
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
                ParameterName = "@manufacturer",
                DbType = DbType.String,
                Value = Manufacturer,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@model",
                DbType = DbType.String,
                Value = Model,
            });
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@machineType",
                DbType = DbType.String,
                Value = MachineType,
            });
        }
    }
