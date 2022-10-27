
const steppedLineData = {
    labels: ['Day 1', 'Day 2', 'Day 3', 'Day 4', 'Day 5', 'Day 6'],
    datasets: [
      {
        label: 'Dataset',
        // data: {count: 6, min: 0, max: 100},
        data: ['Digging', 'Ideling', 'moving'],
        borderColor: 'rgb(255, 99, 132)',
        fill: true,
        stepped: true,
      }
    ]
  };



const SteppedLineConfig = {
    type: 'line',
    data: steppedLineData,
    options: {
      responsive: true,
      interaction: {
        intersect: false,
        axis: 'x'
      },
      plugins: {
        title: {
          display: true,
          text: (ctx) => 'Step ' + ctx.chart.data.datasets[0].stepped + ' Interpolation',
        }
      }
    }
  };

  const mySteppedLineChart = new Chart(
    document.getElementById('mySteppedLineChart'),
    SteppedLineConfig 
  );