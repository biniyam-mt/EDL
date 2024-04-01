#include <vector>
#include <iostream>
#include <random>
#include "util.h"

using namespace std;
typedef int private_int;
typedef bool private_bool;
typedef char private_char;
typedef ulong private_ulong;
typedef uint private_uint;

vector<int> tripCountFreq;
void init_freq(int n)
{
    cout << "Initializing frequency table...\n";
    for (int i = 0; i < n; i++)
    {
        tripCountFreq.push_back(0);
    }
}
private_ulong powm(private_ulong b, private_ulong e, private_uint m)
{
    private_ulong result = 1;
    int count = 0;
    while (e != 0 && count < 64)
    {
        if ((e & 1) == 1)
        {
            result = (result * b) % m;
        }
        b = (b * b) % m;
        e = e / 2;
        count++;
    }
    tripCountFreq[count]++;

    return result;
}

/** Main function */
int main(void)
{
    srand(time(0));
    int executionCount = 10000000;

    init_freq(64);

    int b, e, m;

    for (int i = 0; i < executionCount; i++)
    {

        // TODO: better range for these numbers, maybe bytes
        b = randNumber(1, 1000);
        e = randNumber(0, 1000);
        m = randNumber(2, 100);

        // fprintf(stdout, "INFO: a[%d] = { ", dataSize);
        // for (unsigned i = 0; i < dataSize; i++)
        //     fprintf(stdout, "%d, ", a[i]);
        // fprintf(stdout, " }\n");

        private_ulong result = powm(b, e, m);

        float progress = static_cast<float>(i + 1) / executionCount;
        cout << i + 1 << "/" << executionCount;
        printProgressBar(progress, " computing: ");
    }
    writeVectorToCSV(tripCountFreq, "./output/freq_pow.csv");

    return 0;
}