#include <vector>
#include <iostream>
#include <random>
#include "../util.h"

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

private_int gcdList(vector<private_int> a, int n)
{
    private_int gcd = a[0];

    int outerCount = 0;
    for (int j = 1; j < n; ++j)
    {

        private_int temp = a[j];
        int innerCount = 0;
        while (temp % gcd != 0 && outerCount < 32)
        {
            private_int old_gcd = gcd;
            gcd = temp % gcd;
            temp = old_gcd;
            innerCount++;
        }
        if (innerCount > outerCount)
        {
            outerCount = innerCount;
        }
    }
    tripCountFreq[outerCount]++;

    return gcd;
}

/** Main function */
int main(void)
{
    srand(time(0));
    int executionCount = 1000000;
    int minDataSize = 10;
    int maxDataSize = 1000;
    int maxMultiple = 1000000;
    int minMultiple = 1;
    int minFactor = 10;
    int maxFactor = 50;

    init_freq(32);

    for (int i = 0; i < executionCount; i++)
    {
        int factor = randNumber(minFactor, maxFactor);

        vector<private_int> data;
        int dataSize = randNumber(minDataSize, maxDataSize);
        for (unsigned i = 0; i < dataSize; i++)
            data.push_back((randNumber(minMultiple, maxMultiple)) * factor);

        // fprintf(stdout, "INFO: a[%d] = { ", dataSize);
        // for (unsigned i = 0; i < dataSize; i++)
        //     fprintf(stdout, "%d, ", a[i]);
        // fprintf(stdout, " }\n");

        private_int gcd;

        gcd = gcdList(data, dataSize);

        float progress = static_cast<float>(i + 1) / executionCount;
        cout << i << "/" << executionCount;
        printProgressBar(progress, " computing: ");
    }
    writeVectorToCSV(tripCountFreq, "./output/freq_gcd_RNRS.csv");

    return 0;
}