using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.Threading.Tasks;
using MySqlConnector;
using Server.Models;

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

        public async Task<MachineModel> FindOneAsync(int id)
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM MachineModel WHERE `Id` = @id";
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@id",
                DbType = DbType.Int32,
                Value = id,
            });
            var result = await ReadAllAsync(await cmd.ExecuteReaderAsync());
            return result.Count > 0 ? result[0] : null;
        }

        public async Task<List<MachineModel>> LatestPostsAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM MachineModel ORDER BY `Id` DESC LIMIT 10;";
            return await ReadAllAsync(await cmd.ExecuteReaderAsync());
        }

        public async Task DeleteAllAsync()
        {
            using var txn = await Db.Connection.BeginTransactionAsync();
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"DELETE FROM MachineModel";
            await cmd.ExecuteNonQueryAsync();
            await txn.CommitAsync();
        }

        private async Task<List<MachineModel>> ReadAllAsync(DbDataReader reader)
        {
            var machines = new List<MachineModel>();
            using (reader)
            {
                while (await reader.ReadAsync())
                {
                    var machine = new MachineModel(Db)
                    {
                        Id = reader.GetInt32(0),
                        Manufacturer = reader.GetString(1),
                        ModelName = reader.GetString(2),
                        MachineType = reader.GetString(3)
                    };
                    machines.Add(machine);
                }
            }
            return machines;
        }
    }