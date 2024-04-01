#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <random>

using namespace std;

class DataProvider
{
public:
    string dataset;
    vector<char> getText();
    vector<vector<char>> generatePatterns(vector<char> text, int patSize);
    vector<vector<char>> generatePatterns(vector<char> text, int minPatSize, int maxPatSize);
    vector<char> generateRandomCharacters(int size);
    vector<vector<char>> patternFromText(vector<char> text);
    vector<vector<char>> generateRandPattern(int numberOfPatterns, int patternSize);
    vector<vector<char>> generateRandPattern(int numberOfPatterns, int minPatternSize, int maxPatternSize);
};