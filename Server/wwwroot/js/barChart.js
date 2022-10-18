const DATA_COUNT = 7;
const NUMBER_CFG = {count: DATA_COUNT, min: -100, max: 100};

const labels = [1,2,3,4,5,6,7];
const barData = {
  labels: labels,
  datasets: [
    {
      label: 'Dataset 1',
      data:5,
      borderColor: 'rgb(255, 99, 80)',
      backgroundColor: 'rgb(100, 99, 132)',
    },
    {
      label: 'Dataset 2',
      data: 7,
      borderColor: 'rgb(255, 99, 132)',
      backgroundColor: 'rgb(5, 99, 132)',
    }
  ]
};

const barConfig = {
    type: 'bar',
    data: barData,
    options: {
      indexAxis: 'y',
      // Elements options apply to all of the options unless overridden in a dataset
      // In this case, we are setting the border of each horizontal bar to be 2px wide
      elements: {
        bar: {
          borderWidth: 2,
        }
      },
      responsive: true,
      plugins: {
        legend: {
          position: 'right',
        },
        title: {
          display: true,
          text: 'Chart.js Horizontal Bar Chart'
        }
      }
    },
  };

  const myBarChart = new Chart(
    document.getElementById('myBarChart'),
    barConfig 
  );