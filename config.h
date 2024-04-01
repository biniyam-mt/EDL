#include <iostream>

#define ENC_CHAR char
#define ENC_INT int
#define ENC_UINT unsigned int
#define ENC_UINT64 uint64_t
#define ENC_LONG long
#define ENC_ULONG unsigned long
#define ENC_BOOL bool
#define ENC_FLOAT float
#define ENC_DOUBLE double

#define CMOV(P, A, B) conditionalMove((P), (A), (B))

template <typename T>
T conditionalMove(bool condition, const T &ifValue, const T &elseValue)
{
    T result = T();
    // std::cout << "\ncondition: " << condition << "\n";
    // std::cout << "ifVal: " << ifValue << "\n";
    // std::cout << "elseVal: " << elseValue << "\n";

    __asm__("test   %w1, %w1;"
            "cmovz  %w3, %w0;" // zero flag cleared?
            "cmovnz %w2, %w0;" // zero flag not cleared?
            : "=r"(result)
            : "r"(condition), "r"(ifValue), "r"(elseValue)
            : "cc");

    // std::cout << "returned: " << result << "\n\n";
    if (result == 65535)
    {
        return -1;
    }
    return result;
}
