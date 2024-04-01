#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include "DNASequence.h"
#include "util.h"
#include "Stopwatch.h"
#include "CycleCount.h"

using namespace std;
typedef int private_int;
typedef bool private_bool;
typedef char private_char;

void goodSuffixHeuristic(private_int *shift, private_int *bpos,
                         vector<private_char> pat, int m)
{
    int i = m;
    private_int j = m + 1;
    bpos[i] = j;

    while (i > 0)
    {
        private_char patChar1 = pat[i - 1];
        private_char patChar2 = pat[j - 1];
        while (j <= m && patChar1 != patChar2)
        {
            // oram-read
            private_int temp = shift[j];
            if (temp == 0)
            {
                // oram-write
                shift[j] = j - i;
            }
            // oram-read
            j = bpos[j];

            patChar1 = pat[i - 1];
            patChar2 = pat[j - 1];
        }
        i = i - 1;
        j = j - 1;
        bpos[i] = j;
    }
}

void preprocessCase2(private_int *shift, private_int *bpos, vector<private_char> pat,
                     int m)
{
    private_int j;
    j = bpos[0];
    for (int i = 0; i <= m; i++)
    {
        if (shift[i] == 0)
        {
            shift[i] = j;
        }

        if (i == j)
        {
            // oram-read
            j = bpos[j];
        }
    }
}

void search(vector<private_char> text, vector<private_char> pat, private_bool *ret)
{
    private_int s = 0;
    private_int j;

    int m = pat.size();
    int n = text.size();

    private_int bpos[m + 1];
    private_int shift[m + 1];

    for (int i = 0; i < m + 1; i++)
    {
        shift[i] = 0;
    }

    goodSuffixHeuristic(shift, bpos, pat, m);
    preprocessCase2(shift, bpos, pat, m);

    while (s <= n - m)
    {

        j = m - 1;

        // oram-read
        while (j >= 0 & pat[j] == text[s + j])
        {
            j = j - 1;
        }

        if (j < 0)
        {
            // oram-write
            ret[s] = true;
            s = s + shift[0];
        }
        else
        {
            // oram-read
            private_int temp = shift[j + 1];
            s = s + temp;
        }
    }
}

int main()
{
    Stopwatch timer;
    CycleCount CycleCounter;
    chrono::microseconds totalTimeMicS = chrono::microseconds(0);
    uint64_t totalcycles = 0;

    // fetch data
    string organism = "covid";
    DNASequence dna = DNASequence(organism);
    vector<char> sequence = dna.readSequence();
    int patternSize = 6;
    int textSize = sequence.size();

    vector<vector<char>> patternVector = dna.generatePatterns(sequence, patternSize);
    vector<char> text = sequence;
    int patternVectorSize = patternVector.size();

    cout << "Matching " << organism << " Genome" << endl;
    cout << textSize << " nucleotides\n";

    for (int p = 0; p < patternVectorSize; p++)
    {
        vector<char> pattern = patternVector[p];
        bool ret[textSize - 1];
        for (int i = 0; i < textSize - 1; i++)
            ret[i] = false;

        // measure elapsed time and cycle count
        timer.reset();
        CycleCounter.reset();
        timer.start();
        CycleCounter.start();

        search(text, pattern, ret);

        timer.stop();
        CycleCounter.stop();

        // cout << "time was: " << totalTimeMicS.count() << endl;
        // cout << "time: " << timer.duration().count() << endl;
        totalTimeMicS += timer.duration();
        totalcycles += CycleCounter.cycles();
        // cout << "now time is: " << totalTimeMicS.count() << endl;

        for (int i = 0; i < textSize - 1; i++)
        {
            if (ret[i])
            {
                bool valid = true;
                for (int j = 0; j < patternSize; j++)
                {
                    if (!pattern[j] == sequence[i + j])
                    {
                        valid = false;
                    }
                }
                if (!valid)
                {
                    print("INVALID!", "red");
                    ln();
                }
            }
        }
        float progress = static_cast<float>(p + 1) / patternVectorSize;
        cout << p << "/" << patternVectorSize;
        printProgressBar(progress, " searching: ");
    }
    auto totalTimeMilS = chrono::duration_cast<std::chrono::milliseconds>(totalTimeMicS);
    auto averageTimeMicS = totalTimeMicS.count() / patternVectorSize;
    auto averageTimeMilS = totalTimeMilS.count() / patternVectorSize;

    auto averageCyclecount = totalcycles / patternVectorSize;
    ln();
    print("Total ", "green");
    cout << "(for " << patternVectorSize << " executions): ";
    ln();
    cout << "time: " << totalTimeMicS.count() << "  μs" << endl;
    cout << "cycle: " << totalcycles << endl;
    ln();
    print("Average:", "green");
    ln();
    cout << "time:    " << averageTimeMicS << "  μs" << endl;
    cout << "cycle:    " << averageCyclecount << endl;

    return 0; // Exit successfully
}
