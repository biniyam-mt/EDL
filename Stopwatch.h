// Stopwatch.h

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <iostream>

class Stopwatch
{
public:
    // Start the stopwatch
    void start()
    {
        startTime = std::chrono::high_resolution_clock::now();
        running = true;
    }

    // Stop the stopwatch
    void stop()
    {
        if (running)
        {
            stopTime = std::chrono::high_resolution_clock::now();
            running = false;
        }
        else
        {
            std::cerr << "Stopwatch is not running." << std::endl;
        }
    }
    void reset()
    {
        startTime = std::chrono::high_resolution_clock::now();
        stopTime = startTime;
        running = false;
    }
    // Show the elapsed time in microseconds
    void show() const
    {
        if (running)
        {
            std::cerr << "Stopwatch is still running. Call stop() before calling show()." << std::endl;
        }
        else
        {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
            std::cout << "Elapsed time: " << duration.count() << " microseconds" << std::endl;
        }
    }

    // Get the elapsed time as a duration
    std::chrono::microseconds duration() const
    {
        if (running)
        {
            std::cerr << "Stopwatch is still running. Call stop() before calling duration()." << std::endl;
            return std::chrono::microseconds(0);
        }
        else
        {
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
            return elapsedTime;
        }
    }

private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point stopTime;
    bool running = false;
};

#endif // STOPWATCH_H
