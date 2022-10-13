namespace Server.Models{

public class MeasureData {
            public string sensorName {get; set;}
            public List<float> datapoints {get; set;}
            public List<string> timestamps {get; set;}
        }
}