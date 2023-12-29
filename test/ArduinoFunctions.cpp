#include <chrono>


uint32_t millis( void )
// Return microsecond count in place of Arduino function millis. (Run tests at 1000x speed.)
{
    return std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now().time_since_epoch()).count();
}


uint32_t micros( void )
// Return microsecond count in place of Arduino function micros. ( used as random seed only ).
{
    return std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
