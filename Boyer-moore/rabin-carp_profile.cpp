#include <vector>
#include "../config.h"
#include <climits>

using namespace std;

void rabinCarp_do(vector<ENC_CHAR> &txt, int n, vector<ENC_CHAR> &pat, int m, ENC_BOOL *ret)
{
    int q = INT_MAX;
    int D = 256;

    // value of h would be "pow(d, M-1)%q"
    int h = 1;
    for (int i = 0; i < m - 1; i++)
        h = (h * D) % q;

    ENC_INT p = 0; // hash value for pattern
    ENC_INT t = 0; // hash value for txt

    // calculate the hash value of pattern and first window of text
    for (int i = 0; i < m; i++)
    {
        p = (D * p + pat[i]) % q;
        t = (D * t + txt[i]) % q;
    }

    // slide the pattern over text one by one
    for (int i = 0; i <= (n - m); i++)
    {

        // check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters one by one
        ret[i] = (p == t);

        // calculate hash value for next window of text: Remove leading digit, add trailing digit
        ENC_BOOL _pred = (i < (int)txt.size() - (int)pat.size());
        t = CMOV(_pred, ((D * (t - ((ENC_INT)txt[i]) * h) + txt[i + pat.size()]) % q), t);

        ENC_BOOL _pred1 = (t < 0);
        t = CMOV(_pred & _pred1, t + q, t);
    }
}

void rabinCarp_na(vector<ENC_CHAR> &txt, int n, vector<ENC_CHAR> &pat, int m, ENC_BOOL *ret)
{
    int q = INT_MAX;
    int D = 256;

    // value of h would be "pow(d, M-1)%q"
    int h = 1;
    for (int i = 0; i < (int)pat.size() - 1; i++)
        h = (h * D) % q;

    ENC_INT p = 0; // hash value for pattern
    ENC_INT t = 0; // hash value for txt

    // calculate the hash value of pattern and first window of text
    for (int i = 0; i < (int)pat.size(); i++)
    {
        p = (D * p + pat[i]) % q;
        t = (D * t + txt[i]) % q;
    }

    // slide the pattern over text one by one
    for (int i = 0; i <= (int)txt.size() - (int)pat.size(); i++)
    {

        // check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters one by one
        ret[i] = (p == t);

        // calculate hash value for next window of text: Remove leading digit, add trailing digit
        if (i < (int)txt.size() - (int)pat.size())
        {
            t = (D * (t - ((ENC_INT)txt[i]) * h) + txt[i + pat.size()]) % q;

            // we might get negative value of t, converting it to positive
            if (t < 0)
                t = (t + q);
        }
    }
}