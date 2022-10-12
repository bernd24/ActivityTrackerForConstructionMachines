
    const labels = [0,1,2,3,4,5,6,7,8,9,10,11,12];

    const datapoints = [0, 20, 20, 60, 60, 120, NaN, 180, 120, 125, 105, 110, 170];
    const data = {
    labels: labels,
    datasets: [
        {
        label: 'Cubic interpolation (monotone)',
        data: datapoints,
        borderColor: 'rgb(100, 99, 132)',
        //  @* borderColor: Utils.CHART_COLORS.blue,     this shit is not working  Utils file is not there https://github.com/chartjs/Chart.js/blob/master/docs/scripts/utils.js*@  
        //  @* but i dont think we need that *@
        fill: false,
        cubicInterpolationMode: 'monotone',
        tension: 0.4
        }, {
        label: 'Cubic interpolation',
        data: datapoints,
        borderColor: 'rgb(255, 99, 132)',
        fill: false,
        tension: 0.4
        }, {
        label: 'Linear interpolation (default)',
        data: datapoints,
        borderColor: 'rgb(200, 99, 50)',
        fill: false
        }
    ]
    };

  const config = {
    type: 'line',
    data: data,
    options: {}
  };

    const myChart = new Chart(
    document.getElementById('myLineChart'),
    config 
  );


