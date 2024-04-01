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
#include "../Stopwatch.h"
#include "../CycleCount.h"
#include "../util.h"
#include "../config.h"
#include "../oram.h"

using namespace std;

Stopwatch timer;
CycleCount CycleCounter;
DataProvider provider = DataProvider();

int misses = 0;
int hits = 0;

void goodSuffixHeuristic_do(ENC_INT *shift, ENC_INT *bpos,
                            std::vector<char> pat, int m)
{
    int i = m;
    ENC_INT j = m + 1;
    bpos[i] = j;

    while (i > 0)
    {
        int counter1 = 0;

        ENC_CHAR patChar1 = pat[i - 1];

        ENC_CHAR patChar2;

        // oram read access
        // ENC_CHAR _access_1 = 0;
        // for (int k = 0; k < m; k++)
        // {
        //     _access_1 = CMOV(j - 1 == k, pat[k], _access_1);
        // }
        // patChar2 = _access_1;

        // direct access
        patChar2 = pat[j - 1];

        while (counter1 <= m)
        {

            ENC_BOOL _loop_0 = patChar1 != patChar2 & j <= m;

            // oram read access
            // ENC_INT _access_2 = 0;
            // for (int k = 0; k < m + 1; k++)
            // {
            //     _access_2 = CMOV(j == k, shift[k], _access_2);
            // }
            // ENC_INT temp = _access_2;

            // direct access
            ENC_INT temp = shift[j];

            ENC_BOOL _temp_0 = temp == 0;

            // oram write access
            // for (int k = 0; k < m + 1; k++)
            // {
            //     shift[k] = CMOV(j == k & _temp_0 & _loop_0,
            //                     j - i, shift[k]);
            // }

            // direct access
            shift[j] = CMOV(_temp_0 & _loop_0, j - i, shift[j]);

            // oram read access
            // ENC_INT _access_3 = 0;
            // for (int k = 0; k < m + 1; k++)
            // {
            //     _access_3 = CMOV(j == k, bpos[k], _access_3);
            // }
            // j = CMOV(_loop_0, _access_3, j);

            // direct access
            j = CMOV(_loop_0, bpos[j], j);

            counter1++;

            patChar1 = CMOV(_loop_0,
                            pat[i - 1], patChar1);

            // oram read access
            // ENC_CHAR _access_4 = 0;
            // for (int k = 0; k < m; k++)
            // {
            //     _access_4 = CMOV(j - 1 == k, pat[k], _access_4);
            // }
            // patChar2 = CMOV(_loop_0, _access_4, patChar2);

            // direct access
            patChar2 = CMOV(_loop_0, pat[j - 1], patChar2);
        }

        i = i - 1;

        j = j - 1;

        bpos[i] = j;
    }
}

void preprocessCase_do(ENC_INT *shift, ENC_INT *bpos, vector<ENC_CHAR> pat,
                       int m)
{

    ENC_INT j;
    j = bpos[0];
    for (int i = 0; i <= m; i++)
    {

        ENC_BOOL _temp_1 = shift[i] == 0;

        shift[i] =
            CMOV(_temp_1, j, shift[i]);

        ENC_BOOL _temp_2 = i == j;

        // oram read access
        // ENC_INT _access_5 = 0;
        // for (int i = 0; i < m + 1; i++)
        // {
        //     _access_5 = CMOV(j == i, bpos[i], _access_5);
        // }
        // j = CMOV(_temp_2, _access_5, j);

        // direct access
        j = CMOV(_temp_2, bpos[j], j);
    }
}

void search_do(vector<ENC_CHAR> text, vector<ENC_CHAR> pat, ENC_BOOL *ret)
{
    ENC_INT s = 0;
    ENC_INT j;

    int m = pat.size();
    int n = text.size();

    int finds = 0;
    ENC_INT bpos[m + 1];
    ENC_INT shift[m + 1];

    for (int i = 0; i < m + 1; i++)
    {
        shift[i] = 0;
    }

    goodSuffixHeuristic_do(shift, bpos, pat, m);
    preprocessCase_do(shift, bpos, pat, m);

    int counter3 = 0;
    int MAX_ITERATION = (n - m) * 1;
    while (counter3 <= MAX_ITERATION)
    {

        ENC_BOOL _loop_1 = s <= n - m;

        j = CMOV(_loop_1, m - 1, j);

        // oram read access
        // ENC_CHAR _access_6 = 0;
        // for (int i = 0; i < m; i++)
        // {
        //     _access_6 = CMOV(j == i, pat[i], _access_6);
        // }
        // ENC_CHAR currPatChar = _access_6;

        // direct access
        ENC_CHAR currPatChar = pat[j];

        // oram read access
        // ENC_CHAR _access_7 = 0;
        // for (int i = 0; i < n; i++)
        // {
        //     _access_7 = CMOV(s + j == i, text[i], _access_7);
        // }
        // ENC_CHAR currTextChar = _access_7;

        // direct access
        ENC_CHAR currTextChar = text[s + j];

        int counter2 = 0;
        while (counter2 < m)
        {
            ENC_BOOL _loop_2 = j >= 0 & currPatChar == currTextChar;

            j = CMOV(_loop_1 & _loop_2, j - 1, j);

            // oram read access
            // ENC_CHAR _access_8 = 0;
            // for (int i = 0; i < m; i++)
            // {
            //     _access_8 = CMOV(j == i, pat[i], _access_8);
            // }
            // currPatChar = CMOV(_loop_1 & _loop_2, _access_8, currPatChar);

            // direct access
            currPatChar = CMOV(_loop_1 & _loop_2, pat[j], currPatChar);

            // oram read access
            // ENC_CHAR _access_9 = 0;
            // for (int i = 0; i < n; i++)
            // {
            //     _access_9 = CMOV(s + j == i, text[i], _access_9);
            // }
            // currTextChar = CMOV(_loop_1 & _loop_2, _access_9, currTextChar);

            // direct access
            currTextChar = CMOV(_loop_1 & _loop_2, text[s + j], currTextChar);

            counter2++;
        }

        ENC_BOOL _temp_3 = j < 0;

        // oram write access
        // for (int i = 0; i < n; i++)
        // {
        //     ret[i] = CMOV(s == i & _temp_3 & _loop_1, true, ret[i]);
        // }

        // direct access
        ret[s] = CMOV(_temp_3 & _loop_1, true, ret[s]);
        finds = CMOV(_temp_3 & _loop_1, finds + 1, finds);

        s = CMOV(_temp_3 & _loop_1,
                 s + shift[0], s);

        ENC_INT temp;

        // oram read access
        // ENC_INT _access_10 = 0;
        // for (int i = 0; i < m + 1; i++)
        // {
        //     _access_10 = CMOV(j + 1 == i, shift[i], _access_10);
        // }
        // temp = CMOV(!_temp_3 & _loop_1, _access_10, temp);

        // direct access
        temp = CMOV(!_temp_3 & _loop_1, shift[j + 1], temp);

        s = CMOV(!_temp_3 & _loop_1,
                 s + temp, s);

        counter3++;

        if (counter3 > MAX_ITERATION)
        {
            if (s <= n - m)
                misses++;

            else
                hits++;
        }
    }
}

// Naive implementation

void goodSuffixHeuristic_na(ENC_INT *shift, ENC_INT *bpos,
                            vector<ENC_CHAR> pat, int m)
{
    int i = m;
    ENC_INT j = m + 1;
    bpos[i] = j;

    while (i > 0)
    {
        ENC_CHAR patChar1 = pat[i - 1];
        ENC_CHAR patChar2 = pat[j - 1];
        while (j <= m && patChar1 != patChar2)
        {
            // oram-read
            ENC_INT temp = shift[j];
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

void preprocessCase_na(ENC_INT *shift, ENC_INT *bpos, vector<ENC_CHAR> pat,
                       int m)
{
    ENC_INT j;
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

void search_na(vector<ENC_CHAR> text, vector<ENC_CHAR> pat, ENC_BOOL *ret)
{
    ENC_INT s = 0;
    ENC_INT j;
    int m = pat.size();
    int n = text.size();

    ENC_INT bpos[m + 1];
    ENC_INT shift[m + 1];
    int match = 0;

    for (int i = 0; i < m + 1; i++)
    {
        shift[i] = 0;
    }

    goodSuffixHeuristic_na(shift, bpos, pat, m);
    preprocessCase_na(shift, bpos, pat, m);

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
            match++;
            s = s + shift[0];
        }
        else
        {
            // oram-read
            ENC_INT temp = shift[j + 1];
            s = s + temp;
        }
    }
}

void search_n2(vector<ENC_CHAR> text, vector<ENC_CHAR> pat, ENC_BOOL *ret)
{
    int m = pat.size();
    int n = text.size();
    int finds = 0;

    for (int s = 0; s <= (n - m); s++) // loop over input text
    {

        ENC_BOOL match = true;
        for (int i = 0; i < m; i++) // loop over pattern pat
        {
            match = match & (pat[i] == text[s + i]);
        }
        ret[s] = match;
        if (match)
            finds++;
    }
}

void rabinKarp_na(vector<ENC_CHAR> text, vector<ENC_CHAR> pat, ENC_BOOL *ret)
{
    int q = INT_MAX;
    int D = 256;
    int m = pat.size();
    int n = text.size();
    int finds = 0;

    // value of h would be "pow(d, M-1)%q"
    int h = 1;
    for (int i = 0; i < (int)pat.size() - 1; i++)
        h = (h * D) % q;

    ENC_INT p = 0; // hash value for pattern
    ENC_INT t = 0; // hash value for text

    // calculate the hash value of pattern and first window of text
    for (int i = 0; i < (int)pat.size(); i++)
    {
        p = (D * p + pat[i]) % q;
        t = (D * t + text[i]) % q;
    }

    // slide the pattern over text one by one
    for (int i = 0; i <= (int)text.size() - (int)pat.size(); i++)
    {

        // check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters one by one
        ret[i] = (p == t);
        if (p == t)
            finds++;

        // calculate hash value for next window of text: Remove leading digit, add trailing digit
        if (i < (int)text.size() - (int)pat.size())
        {
            t = (D * (t - ((ENC_INT)text[i]) * h) + text[i + pat.size()]) % q;

            // we might get negative value of t, converting it to positive
            if (t < 0)
                t = (t + q);
        }
    }
}

void rabinKarp_do(vector<ENC_CHAR> text, vector<ENC_CHAR> pat, ENC_BOOL *ret)
{
    int q = INT_MAX;
    int D = 256;
    int m = pat.size();
    int n = text.size();
    int finds = 0;

    // value of h would be "pow(d, M-1)%q"
    int h = 1;
    for (int i = 0; i < m - 1; i++)
        h = (h * D) % q;

    ENC_INT p = 0; // hash value for pattern
    ENC_INT t = 0; // hash value for text

    // calculate the hash value of pattern and first window of text
    for (int i = 0; i < m; i++)
    {
        p = (D * p + pat[i]) % q;
        t = (D * t + text[i]) % q;
    }

    // slide the pattern over text one by one
    for (int i = 0; i <= (n - m); i++)
    {

        // check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters one by one
        ret[i] = (p == t);
        if (ret[i])
            finds++;

        // calculate hash value for next window of text: Remove leading digit, add trailing digit
        ENC_BOOL _pred = (i < (int)text.size() - (int)pat.size());
        t = CMOV(_pred, ((D * (t - ((ENC_INT)text[i]) * h) + text[i + pat.size()]) % q), t);

        ENC_BOOL _pred1 = (t < 0);
        t = CMOV(_pred & _pred1, t + q, t);
    }
}

int run(function<void(vector<ENC_CHAR>, vector<ENC_CHAR>, ENC_BOOL *)> search, vector<ENC_CHAR> &text, vector<vector<char>> &patternVector)
{
    chrono::microseconds totalTimeMicS = chrono::microseconds(0);
    uint64_t totalcycles = 0;
    // With DataSets
    int textSize = text.size();
    int patternVectorSize = patternVector.size();

    for (int p = 0; p < patternVectorSize; p++)
    {
        // std::random_device rd;
        // std::mt19937 g(rd());
        // shuffle(text.begin(), text.end(), g);

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
                for (int j = 0; j < pattern.size(); j++)
                {
                    if (!pattern[j] == text[i + j])
                    {
                        valid = false;
                    }
                }
                if (!valid)
                {
                    print('r', false, {"INVALID!"});
                    return 1;
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
    print('g', false, {"Total"});
    cout << "(for " << patternVectorSize << " executions): ";
    ln();
    cout << "time: " << totalTimeMicS.count() << "  μs" << endl;
    cout << "cycle: " << totalcycles << endl;
    ln();
    print('g', false, {"Average:"});
    ln();
    cout << "time:    " << averageTimeMicS << "  μs" << endl;
    cout << "cycle:    " << averageCyclecount << endl;
    // in milli seconds
    //  cout << "total duration for " << patternVectorSize << " executions: " << totalTimeMilS.count() << " milli seconds" << endl;
    //  cout << "average execution time: " << averageTimeMilS << " milli seconds" << endl;
    cout << "Hits: " << hits << endl;
    cout << "Misses: " << misses << endl;
    cout << "Accuracy: " << (float)hits / (hits + misses) << endl;
    return 0; // Exit successfully
}
int main(int argc, char *argv[])
{
    provider.dataset = argv[1];
    vector<char> text = provider.getText();

    int textSize = text.size();
    cout << "\nSearching " << provider.dataset << " dataset" << endl;
    cout << textSize << " characters\n";

    int fixedPatternSize = 5;
    int minPatternSize = 2;
    int maxPatternSize = 25;

    vector<vector<char>> patternVector = provider.generatePatterns(text, minPatternSize, maxPatternSize);

    cout << patternVector[0][0] << " pattern start\n";
    cout << patternVector[11][0] << " pattern end\n";

    cout << "Native: \n";
    run(search_na, text, patternVector);
    cout << "DO: \n";
    run(search_do, text, patternVector);
    cout << "N2: \n";
    run(search_n2, text, patternVector);
    // cout << "RabinKarp DO: \n";
    // run(rabinKarp_do, text, patternVector);
    // cout << "RabinKarp: \n";
    // run(rabinKarp_na, text, patternVector);
}