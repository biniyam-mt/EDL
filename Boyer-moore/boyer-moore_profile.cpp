#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include "DataProvider.h"
#include "../util.h"

using namespace std;

vector<int> tripCountFreq;

void init_freq(int n, int m)
{
    cout << "Initializing frequency table...\n";
    for (int i = 0; i < n - m + 1; i++)
    {
        tripCountFreq.push_back(0);
    }
}

void goodSuffixHeuristic(int *shift, int *bpos,
                         vector<char> pat, int m)
{
    int i = m;
    int j = m + 1;
    bpos[i] = j;

    while (i > 0)
    {
        // oram-read
        // FIXED_TRIP_COUNT_HERE ---------------------------------
        int counter = 0;

        char patChar1 = pat[i - 1];
        char patChar2 = pat[j - 1];
        while (counter < m && j <= m && patChar1 != patChar2)
        {
            // oram-read
            int temp = shift[j];
            if (temp == 0)
            {
                // oram-write
                shift[j] = j - i;
            }
            // oram-read
            j = bpos[j];
            counter = counter + 1;
            patChar1 = pat[i - 1];
            patChar2 = pat[j - 1];
        }
        i = i - 1;
        j = j - 1;
        bpos[i] = j;
    }
}

void preprocessCase2(int *shift, int *bpos, vector<char> pat,
                     int m)
{
    int j;
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

void search(vector<char> text, vector<char> pat, bool *ret)
{
    int s = 0;
    int j;

    int m = pat.size();
    int n = text.size();

    int bpos[m + 1];
    int shift[m + 1];

    for (int i = 0; i < m + 1; i++)
    {
        shift[i] = 0;
    }

    goodSuffixHeuristic(shift, bpos, pat, m);
    preprocessCase2(shift, bpos, pat, m);

    int counter = 0;
    while (counter < n - m + 1 && s <= n - m)
    {

        j = m - 1;
        int counter2 = m;
        // oram-read
        while (counter2 > 0 && j >= 0 & pat[j] == text[s + j])
        {
            j = j - 1;
            counter2 = counter2 - 1;
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
            int temp = shift[j + 1];
            s = s + temp;
        }
        counter = counter + 1;
    }
    tripCountFreq[counter]++;
}

int main(int argc, char *argv[])
{
    DataProvider provider = DataProvider();

    // With DataSets
    provider.dataset = argv[1];
    vector<char> text = provider.getText();

    // Random chars
    // provider.dataset = "random";
    // vector<char> text = provider.generateRandomCharacters(100000);

    int textSize = text.size();
    cout << "\nSearching " << provider.dataset << " dataset" << endl;
    cout << textSize << " characters\n";

    int fixedPatternSize = 5;
    int minPatternSize = 2;
    int maxPatternSize = 25;
    // vector<vector<char>> patternVector = provider.patternFromText(text);
    // vector<vector<char>> patternVector = provider.generateRandPattern(10000, minPatternSize, maxPatternSize);
    vector<vector<char>> patternVector = provider.generatePatterns(text, minPatternSize, maxPatternSize);

    printVector(text);
    printVector(patternVector);

    init_freq(textSize, minPatternSize);

    for (int p = 0; p < patternVector.size(); p++)
    {
        vector<char> pattern = patternVector[p];
        std::random_device rd;
        std::mt19937 g(rd());
        shuffle(text.begin(), text.end(), g);
        bool ret[textSize - 1];
        for (int i = 0; i < textSize - 1; i++)
            ret[i] = false;

        search(text, pattern, ret);

        // for (int i = 0; i < textSize - 1; i++)
        // {
        //     if (re
        //     {
        //         cout << "Found at index " << i;
        //         bool valid = true;
        //         for (int j = 0; j < patternSize; j++)
        //         {
        //             if (!pattern[j] == text[i + j])
        //             {
        //                 valid = false;
        //             }
        //         }
        //         if (valid)
        //             cout << greenBold << " -------VALID!" << resetColor;
        //         else
        //             cout << redBold << "  -------INVALID!" << resetColor;
        //         cout << endl;
        //     }
        // }
        float progress = static_cast<float>(p + 1) / patternVector.size();
        cout << p << "/" << patternVector.size();
        printProgressBar(progress, " searching: ");
    }
    // for (int i = 0; i < tripCountFreq.size(); i++)
    // {
    //     if (tripCountFreq[i] != 0)
    //         cout << i << " " << tripCountFreq[i] << endl;
    // }
    writeVectorToCSV(tripCountFreq, "./output/freq_" + provider.dataset + "_EPRS.csv");

    return 0; // Exit successfully
}
