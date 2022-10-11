const AreaData = {
    labels: [
        'Red',
        'Blue',
        'Yellow'
      ],
    datasets: [
      {
        label: 'Dataset 1',
        data: [300, 50, 100],
        borderColor:  'rgb(255, 99, 132)',
        backgroundColor:  'rgb(255, 99, 132)',
        fill: true
      },
      {
        label: 'Dataset 2',
        data: [300, 50, 100],
        borderColor:  'rgb(255, 99, 132)',
        backgroundColor: 'rgb(255, 99, 132)',
        // Utils.transparentize( 'rgb(255, 99, 132)'),
        fill: true
      }
    ]
  };

const areaConfig = {
    type: 'line',
    data: AreaData,
    options: {
      plugins: {
        filler: {
          propagate: false,
        },
        title: {
          display: true,
          text: (ctx) => 'drawTime: ' + ctx.chart.options.plugins.filler.drawTime
        }
      },
      pointBackgroundColor: '#fff',
      radius: 10,
      interaction: {
        intersect: false,
      }
    },
  };

const myAreaChart = new Chart(
    document.getElementById('myAreaChart'),
    areaConfig 
  );