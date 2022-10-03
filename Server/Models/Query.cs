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

        public List<Machine> GetMachinesWithConfig(){
            var machines = new List<Machine>();
            IEnumerable<dynamic> result = Db.Query("Machine")
            .Select("Machine.Id","Machine.InternalId","Machine.inUse",
            "MachineModel.Id as M_Id","MachineModel.Manufacturer","MachineModel.ModelName",
            "MachineModel.MachineType","MachineModel.ModelYear",
            "SensorConfiguration.Id as C_Id","SensorConfiguration.Picture","SensorConfiguration.Notes")
            .Join("MachineModel","Machine.M_Id","MachineModel.Id")
            .LeftJoin("SensorConfiguration","Machine.C_Id","SensorConfiguration.Id")
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
                    Config = m.C_Id == null ? null : new SensorConfiguration(){
                        Id = m.C_Id,
                        Picture = m.Picture,
                        Notes = m.Notes
                    },
                    C_Id = m.C_Id == null ? null : m.C_Id
                };
                if(machine.Config != null){
                    machine.Config.SensorNodes = GetSensorNodeInstances(machine.Config.Id);
                }
                machines.Add(machine);
            }
            
            return machines;
        }

        public Machine GetMachineWithConfig(int Id){
            var machines = new List<Machine>();
            IEnumerable<dynamic> result = Db.Query("Machine")
            .Select("Machine.Id","Machine.InternalId","Machine.inUse",
            "MachineModel.Id as M_Id","MachineModel.Manufacturer","MachineModel.ModelName",
            "MachineModel.MachineType","MachineModel.ModelYear",
            "SensorConfiguration.Id as C_Id","SensorConfiguration.Picture","SensorConfiguration.Notes")
            .Where("Machine.Id",Id)
            .Join("MachineModel","Machine.M_Id","MachineModel.Id")
            .LeftJoin("SensorConfiguration","Machine.C_Id","SensorConfiguration.Id")
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
                    Config = m.C_Id == null ? null : new SensorConfiguration(){
                        Id = m.C_Id,
                        Picture = m.Picture,
                        Notes = m.Notes
                    },
                    C_Id = m.C_Id == null ? null : m.C_Id
                };
                if(machine.Config != null){
                    machine.Config.SensorNodes = GetSensorNodeInstances(machine.Config.Id);
                }
                return machine;
            }
            return null;
        }

        public List<SensorNodeInstance> GetSensorNodeInstances(int C_Id){
            var sensorNodeInstances = new List<SensorNodeInstance>();
            IEnumerable<dynamic> result = Db.Query("SensorNodeInstance")
            .Select("SensorNodeInstance.Id as SNI_Id","SensorNodeInstance.hasBattery","SensorNodeInstance.isMaster",
            "SensorNode.Id as SN_Id","SensorNode.MAC","SensorNode.BatteryStatus","SensorNode.Color")
            .Where("C_Id",C_Id)
            .LeftJoin("SensorNode","SensorNodeInstance.SN_Id","SensorNode.Id")
            .Get();
            foreach(var sn in result){
                var sensorNodeInstance = new SensorNodeInstance(){
                    Id = sn.SNI_Id,
                    C_Id = C_Id,
                    SN_Id = sn.SN_Id,
                    hasBattery = sn.hasBattery,
                    isMaster = sn.isMaster,
                    SensorNode = sn.SN_Id == null ? null : new SensorNode(){
                        Id = sn.SN_Id,
                        MAC = sn.MAC,
                        BatteryStatus = sn.BatteryStatus,
                        Color = sn.Color
                    }
                };
                IEnumerable<dynamic> result2 = Db.Query("SensorInstance")
                .Select("SensorInstance.Id as SI_Id","SensorInstance.Axis","SensorInstance.Position","SensorInstance.RssiPartner",
                "Sensor.Id as S_Id","Sensor.SensorName","Sensor.SensorType","Sensor.Manufacturer")
                .Where("SensorInstance.SN_Id",(int)(sn.SNI_Id))
                .Join("Sensor","SensorInstance.S_Id","Sensor.Id")
                .Get();
                sensorNodeInstance.Sensors = new List<SensorInstance>();
                foreach(var s in result2){
                    var sensor = new SensorInstance(){
                        Id = s.SI_Id,
                        SN_Id = sn.SNI_Id,
                        Axis = s.Axis,
                        Position = s.Position,
                        RssiPartner = s.RssiPartner,
                        Sensor = new Sensor(){
                            Id = s.S_Id,
                            SensorName = s.SensorName,
                            SensorType = s.SensorType,
                            Manufacturer = s.Manufacturer
                        }
                    };
                    sensorNodeInstance.Sensors.Add(sensor);
                }
                sensorNodeInstances.Add(sensorNodeInstance);
            }

            

            return sensorNodeInstances;
        }
    }