namespace Server.Models{

public class NodeData{
    public int SN_Id {get; set;}
    public List<MeasureData> data{get; set;}
}

public class MeasureData {
            public string sensorName {get; set;}
            public List<float> datapoints {get; set;}
            public List<string> timestamps {get; set;}
        }
}