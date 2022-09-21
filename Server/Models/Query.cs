using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.Threading.Tasks;
using MySqlConnector;
using UIAutomate.Models;

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

        public async Task<SUSTest> FindOneAsync(int id)
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM SUSTest WHERE `Id` = @id";
            cmd.Parameters.Add(new MySqlParameter
            {
                ParameterName = "@id",
                DbType = DbType.Int32,
                Value = id,
            });
            var result = await ReadAllAsync(await cmd.ExecuteReaderAsync());
            return result.Count > 0 ? result[0] : null;
        }

        public async Task<List<SUSTest>> LatestPostsAsync()
        {
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"SELECT * FROM SUSTest ORDER BY `Id` DESC LIMIT 10;";
            return await ReadAllAsync(await cmd.ExecuteReaderAsync());
        }

        public async Task DeleteAllAsync()
        {
            using var txn = await Db.Connection.BeginTransactionAsync();
            using var cmd = Db.Connection.CreateCommand();
            cmd.CommandText = @"DELETE FROM SUSTest";
            await cmd.ExecuteNonQueryAsync();
            await txn.CommitAsync();
        }

        private async Task<List<SUSTest>> ReadAllAsync(DbDataReader reader)
        {
            var SUSTests = new List<SUSTest>();
            using (reader)
            {
                while (await reader.ReadAsync())
                {
                    var SUSTest = new SUSTest(Db)
                    {
                        Id = reader.GetInt32(0),
                        Question = reader.GetString(1),
                        StronglyDisagree = reader.GetBoolean(2),
                        Disagree = reader.GetBoolean(3),
                        Neutral = reader.GetBoolean(4),
                        Agree = reader.GetBoolean(5),
                        StronglyAgree = reader.GetBoolean(6)
                    };
                    SUSTests.Add(SUSTest);
                }
            }
            return SUSTests;
        }
    }