#include "Common.h"

namespace ircs
{
    namespace common
    {
        size_t GetTimestampMS()
        {
            size_t ticks = std::clock();
            float t=ticks;
            t=(t/CLOCKS_PER_SEC) * 1000;
            size_t timestamp =t;
            return timestamp;
        }
    }
    
}