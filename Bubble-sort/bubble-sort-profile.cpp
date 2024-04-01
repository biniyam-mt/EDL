#include "../util.h"
#include "../config.h"
#include <vector>
#include <iostream>

using namespace std;
vector<int> tripCountFreq;
void init_freq(int n)
{
  cout << "Initializing frequency table...\n";
  for (int i = 0; i < n; i++)
  {
    tripCountFreq.push_back(0);
  }
}
void bubblesort(ENC_INT *data, unsigned size)
{
  int counter = 0;
  for (unsigned i = size; i > 1; i--)
  {
    counter++;

    bool swapped = false;
    for (unsigned j = 0; j < i - 1; j++)
    {

      if (data[j] > data[j + 1])
      {
        ENC_INT tmp = data[j];
        data[j] = data[j + 1];
        data[j + 1] = tmp;
        swapped = true;
      }
    }

    if (!swapped)
      break;
  }
  int tripPercentage = (counter * 100) / size;
  tripCountFreq[tripPercentage]++;
  // cout << "Counter 2: " << counter << "/" << size << "\npercentage " << tripPercentage << endl;
}

void print_data(ENC_INT *data, unsigned size)
{
  fprintf(stdout, "DATA DUMP:\n");
  for (unsigned i = 0; i < size; i++)
    fprintf(stdout, "  data[%u] = %d\n", i, data[i]);
}
int main(void)
{

  init_freq(100);
  int executionCount = 10000;
  string inputData = "RNRS";
  for (int i = 0; i < executionCount; i++)
  {
    int DATASET_SIZE = randNumber(10, 1000);
    ENC_INT data[DATASET_SIZE];
    // initialize the array to sort
    for (unsigned i = 0; i < DATASET_SIZE; i++)
      data[i] = randNumber(0, 1000);
    bubblesort(data, DATASET_SIZE);

    // // check the array
    // for (unsigned i = 0; i < DATASET_SIZE - 1; i++)
    // {
    //   if (data[i] > data[i + 1])
    //   {
    //     fprintf(stdout, "ERROR: data is not properly sorted.\n");
    //     return -1;
    //   }
    // }
    // fprintf(stdout, "INFO: data is properly sorted.\n");
    float progress = static_cast<float>(i + 1) / executionCount;
    cout << i << "/" << executionCount;
    printProgressBar(progress, " computing: ");
  }
  string path = "./output/bubble_sort_" + inputData + ".csv";
  writeVectorToCSV(tripCountFreq, path);
  return 0;
}
