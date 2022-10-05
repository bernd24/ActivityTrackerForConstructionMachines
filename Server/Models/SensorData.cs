namespace Server.Models{
    public class Handshake{
        public string ID {get; set;}
        public int elements {get; set;}
    }

    public class SensorData<T>{
        public int node_ID {get; set;}
        public int type {get; set;}
        public List<T> payload {get; set;}

        public int packets {get; set;}
    }
}