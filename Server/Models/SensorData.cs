namespace Server.Models{
    public class Handshake{
        public string id {get; set;}
        public int elements {get; set;}
    }

    public class SensorData<T>{
        public int node_ID {get; set;}
        public List<T> payload {get; set;}
    }
}