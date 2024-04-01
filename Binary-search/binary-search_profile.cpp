#include <iostream>
#include <vector>
#include <cmath>
#include "../util.h"
#include <random>

using namespace std;
typedef int private_int;
typedef bool private_bool;
typedef char private_char;

vector<int> tripCountFreq;
void init_freq(int n)
{
    cout << "Initializing frequency table...\n";
    for (int i = 0; i < n; i++)
    {
        tripCountFreq.push_back(0);
    }
}

int binarySearch(vector<private_int> arr, private_int target)
{
    int n = arr.size();
    private_int low = 0;
    private_int high = n - 1;

    int ret = -1;
    int counter = 0;
    int MAX_ITERATION = ceil(log2(n));

    while (low <= high)
    {
        private_int mid = low + (high - low) / 2;

        if (arr[mid] == target)
        {
            ret = mid; // Target found
            break;
        }
        else if (arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
        counter++;
    }
    tripCountFreq[counter]++;

    return ret; // Target not found
}
void insertionSort(vector<int> &arr)
{
    int n = arr.size();

    for (int i = 1; i < n; ++i)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }
}
int main(void)
{
    srand(time(0));
    int executionCount = 10000;
    int dataSize = 10000;
    int maxNum = 10000;
    int minNum = -10000;

    init_freq(ceil(log2(dataSize)));

    for (int i = 0; i < executionCount; i++)
    {
        vector<private_int> data;
        for (unsigned i = 0; i < dataSize; i++)
            data.push_back(randNumber(minNum, maxNum));

        insertionSort(data);
        int targetIdx = randNumber(0, dataSize - 1);
        int target = data[targetIdx];

        // fprintf(stdout, "INFO: data[%d] = { ", dataSize);
        // for (unsigned i = 0; i < dataSize; i++)
        //     fprintf(stdout, "%d, ", data[i]);
        // fprintf(stdout, " }\n");

        private_int result;

        result = binarySearch(data, target);

        if (data[result] != target)
        {
            print('r', false, {"INVALID!"});
            return 1;
        }

        float progress = static_cast<float>(i + 1) / executionCount;
        cout << i << "/" << executionCount;
        printProgressBar(progress, " computing: ");
    }
    writeVectorToCSV(tripCountFreq, "./output/freq_binary.csv");

    return 0;
}