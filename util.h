#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <functional>
#include <map>

using namespace std;

const char *resetColor = "\033[0m";
string getColorCode(char colorChar)
{
    std::string colorCode;
    switch (colorChar)
    {
    case 'c':
        colorCode = "\033[1;36m"; // Cyan
        break;
    case 'g':
        colorCode = "\033[1;32m"; // Green
        break;
    case 'y':
        colorCode = "\033[1;33m"; // Yellow
        break;
    case 'r':
        colorCode = "\033[1;31m"; // Red
        break;
    case 'p':
        colorCode = "\033[1;35m"; // Purple
        break;
    default:
        colorCode = "\033[0m"; // Black
        break;
    }
    return colorCode;
}
void writeVectorToCSV(const vector<int> &myVector, const string &fileName)
{
    // Open the file for writing
    ofstream outputFile(fileName);

    // Check if the file is successfully opened
    if (!outputFile.is_open())
    {
        cerr << "Error opening the file: " << fileName << endl;
        return;
    }

    // Write header to the CSV file
    outputFile << "TripCount,Frequency" << endl;

    // Write index and corresponding value to the CSV file
    for (size_t i = 0; i < myVector.size(); ++i)
    {
        outputFile << i << "," << myVector[i] << endl;
    }

    // Close the file
    outputFile.close();

    cout << "CSV file has been created: " << fileName << endl;
}

void printProgressBar(float progress, string title, int barWidth = 50)
{
    const int totalSteps = 100;

    cout << title << "|";

    int pos = static_cast<int>(barWidth * progress);
    for (int j = 0; j < barWidth; ++j)
    {
        if (j < pos)
            cout << "█";
        else
            cout << "-";
    }

    cout << "| " << static_cast<int>(progress * 100.0) << "%\r";
    cout.flush();

    // If progress is 100%, move to the next line
    if (progress >= 1.0)
    {
        cout << endl;
    }
}

void print(char color, bool lineBreak, initializer_list<string> content)
{
    const char *colorCode = getColorCode(color).c_str();

    for (const std::string &str : content)
    {
        cout << colorCode << str << resetColor << " ";
    }
    if (lineBreak)
    {
        cout << endl;
    }
}
void ln()
{
    cout << "\n";
}

#include <random>

int randNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

void printVector(const std::vector<std::vector<char>> &vec)
{
    for (const auto &innerVec : vec)
    {
        for (char ch : innerVec)
        {
            std::cout << ch;
        }
        std::cout << "\n";
    }
}

void printVector(const std::vector<char> &vec)
{
    for (char ch : vec)
    {
        std::cout << ch;
    }
    std::cout << std::endl;
}