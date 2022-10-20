const LineData = {
    labels: [
        'Red',
        'Blue',
        'Yellow'
      ],
    datasets: [
      {
        label: 'Dataset 1',
        data: [300, 50, 100],
        borderColor:  'rgb(205, 99, 100)',
        backgroundColor:  'rgb(200, 99, 132)',
        fill: true
      },
      {
        label: 'Dataset 2',
        data: [40, 100, 10],
        borderColor:  'rgb(255, 99, 132)',
        backgroundColor: 'rgb(255, 99, 132)',
        // Utils.transparentize( 'rgb(255, 99, 132)'),
        fill: true
      }
    ]
  };

  const lineConfig = {
    type: 'line',
    data: LineData,
    options: {
      scales: {
        myAreaChart: {
          type: 'logarithmic',
          position: 'right', // `axis` is determined by the position as `'y'`
        },
      },
      plugins: {
        legend: {
          position: 'bottom',
        },
        filler: {
          propagate: false,
        },
        title: {
          display: false,
          text: 'Emisions'
        }
      },
      pointBackgroundColor: '#fff',
      radius: 10,
      interaction: {
        intersect: false,
      }
    },
  };

  const myLineChart = new Chart(
    document.getElementById('myLineChart'),
    lineConfig 
  );