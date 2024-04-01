#include <vector>

using namespace std;

template <typename T>
class Oram
{
public:
    int size;
    vector<T> *data;

    Oram(vector<T> &dataVector)
    {
        size = dataVector.size();
        data = dataVector;
    }

    T naiveAccess(bool isWrite, int index, T replacementVal)
    {
        if (index >= size)
        {
            return 0;
        }

        T result = T();
        for (int k = 0; k < size; k++)
        {
            if (isWrite)
            {
                data[k] = CMOV(index == k, replacementVal, data[k]);
                result = 1;
            }
            else
            {
                result = CMOV(index == k, data[k], result);
            }
        }

        return result;
    }
};