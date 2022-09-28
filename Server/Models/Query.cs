using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.Threading.Tasks;
using MySqlConnector;
using Server.Models;
using SqlKata;
using SqlKata.Execution;

    public class Query
    {
        private readonly QueryFactory Db;

        internal Query(QueryFactory db)
        {
            Db = db;
        }

        public List<Machine> GetMachines(){
            var machines = new List<Machine>();
            IEnumerable<dynamic> result = Db.Query("Machine")
            .Select("Machine.Id","Machine.InternalId","Machine.inUse",
            "MachineModel.Id as M_Id","MachineModel.Manufacturer","MachineModel.ModelName",
            "MachineModel.MachineType","MachineModel.ModelYear")
            .Join("MachineModel","Machine.M_Id","MachineModel.Id")
            .Get();
            foreach(var m in result){
                var machine = new Machine(){
                    Id = m.Id,
                    InternalId = m.InternalId,
                    inUse = m.inUse,
                    Model = new MachineModel(){
                        Id = m.M_Id,
                        Manufacturer = m.Manufacturer,
                        ModelName = m.ModelName,
                        MachineType = m.MachineType,
                        ModelYear = m.ModelYear
                    },
                    Config = null
                };
                machines.Add(machine);
            }
            return machines;
        }
    }