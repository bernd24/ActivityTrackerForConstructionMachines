using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.Threading.Tasks;
using MySqlConnector;


    public class Query
    {

        internal AppDb Db { get; set; }

        public Query()
        {
        }

        internal Query(AppDb db)
        {
            Db = db;
        }

        public async Task<Machine> FindOneAsync(int id)
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM Machine WHERE `Id` = @id";
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@id",
                DbType = DbType.Int32,
                Value = id,
            });
            var result = await ReadAllAsync(await cmd.ExecuteReaderAsync());
            return result.Count > 0 ? result[0] : null;
        }

        public async Task<List<Machine>> LatestPostsAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM Machine ORDER BY `Id` DESC LIMIT 10;";
            return await ReadAllAsync(await cmd.ExecuteReaderAsync());
        }

        public async Task DeleteAllAsync()
        {
            using var txn = await Db.Connection.BeginTransactionAsync();
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"DELETE FROM Machine";
            await cmd.ExecuteNonQueryAsync();
            await txn.CommitAsync();
        }

        private async Task<List<Machine>> ReadAllAsync(DbDataReader reader)
        {
            var machines = new List<Machine>();
            using (reader)
            {
                while (await reader.ReadAsync())
                {
                    var machine = new Machine(Db)
                    {
                        Id = reader.GetInt32(0),
                        Manufacturer = reader.GetString(1),
                        Model = reader.GetString(2),
                        MachineType = reader.GetString(3)
                    };
                    machines.Add(machine);
                }
            }
            return machines;
        }
    }