#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;
typedef int private_int;
typedef bool private_bool;
typedef char private_char;

#define NO_OF_CHARS 256
#define N 512
#define M 8
#define M_1 9 // M + 1
#define P 505 // N-M+1

int main()
{

    srand(14);
    private_char text[N];
    for (unsigned i = 0; i < N; i++)
    {
        int rand_char = (rand() % 58) + 65;
        if (rand_char > 90 && rand_char < 97)
        {
            i--;
        }
        else
        {
            text[i] = rand_char;
        }
    }
    private_char pat[] = {'c', 'y', 'p', 'z', 'A', 'B', 'A', 'x'};
    // Insert pattern
    for (int i = 0; i < 3; i++)
    {
        int shift = rand() % (N - M);
        for (unsigned j = 0; j < M; j++)
        {
            text[shift + j] = pat[j];
        }
    }

    private_bool ret[N];
    for (int i = 0; i < N; i++)
    {
        ret[i] = false;
    }

    private_int s = 0;
    private_int j;

    private_int bpos[M_1];
    private_int shift[M_1];

    for (int i = 0; i < M_1; i++)
    {
        shift[i] = 0;
    }

    int p = 0;
    private_int q = M_1;
    bpos[M + p] = q;

    while (p < M)
    {
        int counter1 = 0;

        private_char patChar1 = pat[M + p - 1];
        // oram-read 6 x 6
        private_char patChar2;
        patChar2 = pat[q - 1];
        while (counter1 < M && q <= M && patChar1 != patChar2)
        {
            // oram-read 36 x 7
            private_int temp = shift[q];
            if (temp == 0)
            {
                // oram-write 36 x 7
                shift[q] = q - M + p;
            }
            // oram-read 36 x 7
            q = bpos[q];
            counter1 = counter1 + 1;
            patChar1 = pat[M + p - 1];
            // oram-read 36 x 7
            patChar2 = pat[q - 1];
        }
        p = p + 1;
        q = q - 1;
        bpos[M + p] = q;
    }

    private_int l;
    l = bpos[0];
    for (int i = 0; i < M_1; i++)
    {
        if (shift[i] == 0)
        {
            shift[i] = l;
        }

        if (i == l)
        {
            // oram-read 7 x 7
            l = bpos[l];
        }
    }

    int counter3 = 0;
    while (counter3 < P && s <= N - M)
    {

        j = M - 1;
        int counter2 = 0;

        // oram-read 12 x 6
        private_char currPatChar = pat[j];

        private_int idx_s = s + j;
        // oram-read 12 x 17
        private_char currTextChar = text[s + j];
        while (counter2 < M && j >= 0 & currPatChar == currTextChar)
        {
            j = j - 1;
            counter2 = counter2 + 1;

            // oram-read 72 x 6
            currPatChar = pat[j];
            idx_s = s + j;

            // oram-read 72 x 17
            currTextChar = text[s + j];
        }

        if (j < 0)
        {
            // oram-write 12 x 17
            ret[s] = true;
            s = s + shift[0];
        }
        else
        {
            private_int temp;
            // oram-read 12 x 7
            temp = shift[j + 1];
            s = s + temp;
        }
        counter3 = counter3 + 1;
    }

    for (int i = 0; i < N; i++)
    {
        if (ret[i])
        {
            cout << "Found at index " << i << endl;
        }
    }
    return 0;
}