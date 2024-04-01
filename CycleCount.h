// CycleCount.h
#include <iostream>

#ifndef CYCLE_COUNT_H
#define CYCLE_COUNT_H

#include <cstdint>
using namespace std;

class CycleCount
{
public:
    // Start the cycle count
    void start()
    {
        startCycle = rdtsc();
        running = true;
    }

    // Stop the cycle count
    void stop()
    {
        if (running)
        {
            stopCycle = rdtsc();
            running = false;
        }
        else
        {
            cerr << "Trying to stop when not running" << endl;
        }
    }

    // Reset the cycle count
    void reset()
    {
        startCycle = 0;
        stopCycle = 0;
        running = false;
    }

    // Return the cycle count
    uint64_t cycles() const
    {
        if (running)
        {
            cerr << "Trying to get cycles while still running" << endl;
            return 0;
        }
        else
        {
            return stopCycle - startCycle;
        }
    }

private:
    // Function to read the Time Stamp Counter (TSC)
    inline uint64_t rdtsc() const
    {
        uint32_t lo, hi;
        asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
        return ((uint64_t)hi << 32) | lo;
    }

    uint64_t startCycle = 0;
    uint64_t stopCycle = 0;
    bool running = false;
};

#endif // CYCLE_COUNT_H
