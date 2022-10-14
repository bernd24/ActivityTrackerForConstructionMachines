namespace Server.Models{

public class NodeData{
    public int SN_Id {get; set;}
    public List<MeasureData> data{get; set;}
}

public class Point{
    public Int64 x{get; set;}
    public float y{get; set;}
}

public class MeasureData {
            public string sensorName {get; set;}
            public List<Point> datapoints {get; set;}
        }
}