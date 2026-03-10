#include <stdio.h>
#include <math.h>

#define ENABLE_RESULT_LOGS 0

#if ENABLE_RESULT_LOGS
#define log_result(...) printf(__VA_ARGS__)
#else
#define log_result(...)
#endif