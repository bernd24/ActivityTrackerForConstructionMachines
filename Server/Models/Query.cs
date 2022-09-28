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

        public async Task<Machine> FindOneAsyncMachine(int id)
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM Machine WHERE `Id` = @id";
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@id",
                DbType = DbType.Int32,
                Value = id,
            });
            var result = await ReadAllAsyncMachine(await cmd.ExecuteReaderAsync());
            return result.Count > 0 ? result[0] : null;
        }

        public async Task<SensorConfiguration> FindOneAsyncSensorConfiguration(int id)
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM SensorConfiguration WHERE `Id` = @id";
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@id",
                DbType = DbType.Int32,
                Value = id,
            });
            var result = await ReadAllAsyncSensorConfiguration(await cmd.ExecuteReaderAsync());
            return result.Count > 0 ? result[0] : null;
        }

        public async Task<MachineModel> FindOneAsyncMachineModel(int id)
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM MachineModel WHERE `Id` = @id";
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@id",
                DbType = DbType.Int32,
                Value = id,
            });
            var result = await ReadAllAsyncMachineModel(await cmd.ExecuteReaderAsync());
            return result.Count > 0 ? result[0] : null;
        }

        public async Task<List<Machine>> LatestPostsAsyncMachine()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT Machine.Id,Machine.InternalId,Machine.inUse,MachineModel.Id,MachineModel.Manufacturer,MachineModel.ModelName,MachineModel.MachineType,MachineModel.ModelYear FROM Machine,MachineModel WHERE Machine.M_Id = MachineModel.Id ORDER BY Machine.Id;";
            DbDataReader reader = await cmd.ExecuteReaderAsync();
            return await ReadAllAsyncMachine(reader);
        }

        public async Task<List<MachineModel>> LatestPostsAsyncMachineModel()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM MachineModel ORDER BY `Id` DESC LIMIT 10;";
            return await ReadAllAsyncMachineModel(await cmd.ExecuteReaderAsync());
        }

        public async Task DeleteAllAsyncMachineModel()
        {
            using var txn = await Db.Connection.BeginTransactionAsync();
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"DELETE FROM MachineModel";
            await cmd.ExecuteNonQueryAsync();
            await txn.CommitAsync();
        }

        private async Task<List<Machine>> ReadAllAsyncMachine(DbDataReader reader)
        {
            var machines = new List<Machine>();
            using (reader)
            {
                while (await reader.ReadAsync())
                {
                    var machine = new Machine(Db)
                    {
                        Id = reader.GetInt32(0),
                        InternalId = reader.IsDBNull(1) ? "" : reader.GetString(1),
                        inUse = reader.GetBoolean(2),
                        Model = new MachineModel(){
                            Id = reader.GetInt32(3),
                            Manufacturer = reader.GetString(4),
                            ModelName = reader.GetString(5),
                            MachineType = reader.GetString(6),
                            ModelYear = reader.GetInt32(7)
                        },
                        Config = null
                    };
                    machines.Add(machine);
                }
            }
            return machines;
        }

        private async Task<List<MachineModel>> ReadAllAsyncMachineModel(DbDataReader reader)
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
                        MachineType = reader.GetString(3),
                        ModelYear = reader.GetInt32(4)
                    };
                    machines.Add(machine);
                }
            }
            return machines;
        }

        private async Task<List<SensorConfiguration>> ReadAllAsyncSensorConfiguration(DbDataReader reader)
        {
            var configs = new List<SensorConfiguration>();
            using (reader)
            {
                while (await reader.ReadAsync())
                {
                    var config = new SensorConfiguration(Db)
                    {
                        Id = reader.GetInt32(0),
                        Picture = reader.GetString(1),
                        Notes = reader.GetString(2),
                    };
                    configs.Add(config);
                }
            }
            return configs;
        }
    }