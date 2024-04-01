#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include "DataProvider.h"

using namespace std;

vector<char> DataProvider::getText()
{
    vector<char> charVector;

    string fileName = "data/" + dataset + ".txt";
    ifstream inputFile(fileName);

    if (!inputFile.is_open())
    {
        cerr << "Error opening the file." << endl;
        return charVector;
    }

    string line;

    while (getline(inputFile, line))
    {
        for (char c : line)
        {
            charVector.push_back(c);
        }
    }
    inputFile.close();

    return charVector;
}

vector<vector<char>> DataProvider::generatePatterns(vector<char> text, int patSize)
{
    cout << "Generating patterns...\n";
    vector<vector<char>> patternVector;
    int size = text.size();

    for (int i = 0; i < size; i++)
    {
        // cout << "size: " << size << endl;
        vector<char> pattern;
        int j = 0;
        while (j < patSize && i + j < size)
        {
            pattern.push_back(text[i + j]);
            j++;
        }
        if (pattern.size() == patSize)
        {

            patternVector.push_back(pattern);
        }
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(patternVector.begin(), patternVector.end(), g);
    cout << "Generated " << patternVector.size() << " patterns\n";
    return patternVector;
}

vector<vector<char>> DataProvider::generatePatterns(vector<char> text, int minPatSize, int maxPatSize)
{
    cout << "Generating patterns...\n";
    vector<vector<char>> patternVector;
    int size = text.size();

    for (int i = 0; i < size; i++)
    {
        // cout << "size: " << size << endl;
        vector<char> pattern;
        int j = 0;
        int patSize = minPatSize + rand() % (maxPatSize - minPatSize + 1);
        while (j < patSize && i + j < size)
        {
            pattern.push_back(text[i + j]);
            j++;
        }
        if (pattern.size() == patSize)
        {

            patternVector.push_back(pattern);
        }
    }
    // random_device rd;
    mt19937 g(321);
    shuffle(patternVector.begin(), patternVector.end(), g);
    cout << "Generated " << patternVector.size() << " patterns\n";
    return patternVector;
}

vector<char> DataProvider::generateRandomCharacters(int size)
{
    vector<char> result(size);

    // Seed the random number generator
    random_device rd;
    mt19937 gen(rd());

    // Define the character set
    const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{}|;:,.<>? ";

    // Fill the vector with random characters
    for (int i = 0; i < size; ++i)
    {
        // Generate a random index within the character set range
        uniform_int_distribution<int> distribution(0, charset.size() - 1);
        int index = distribution(gen);
        result[i] = charset[index];
    }

    return result;
}
vector<vector<char>> DataProvider::patternFromText(vector<char> text)
{
    vector<vector<char>> patterns;
    vector<char> pattern;

    for (char ch : text)
    {
        if (!isspace(ch))
        { // If character is not whitespace
            pattern.push_back(ch);
        }
        else if (!pattern.empty())
        { // If pattern is not empty
            patterns.push_back(pattern);
            pattern.clear();
        }
    }

    // If there's still a pattern remaining
    if (!pattern.empty())
    {
        patterns.push_back(pattern);
    }

    return patterns;
}

vector<vector<char>> DataProvider::generateRandPattern(int numberOfPatterns, int patternSize)
{
    vector<vector<char>> patterns;

    for (int i = 0; i < numberOfPatterns; i++)
    {

        vector<char> pattern = generateRandomCharacters(patternSize);

        patterns.push_back(pattern);
    }
    return patterns;
}

vector<vector<char>> DataProvider::generateRandPattern(int numberOfPatterns, int minPatternSize, int maxPatternSize)
{
    vector<vector<char>> patterns;

    for (int i = 0; i < numberOfPatterns; i++)
    {
        int size = minPatternSize + rand() % (maxPatternSize - minPatternSize + 1);
        patterns.push_back(generateRandomCharacters(size));
    }
    return patterns;
}
