
#include <stdlib.h>
#include <limits.h>
#include <iostream>

// Вариативный список заголовков для Windows и Linux
#ifdef __linux__ 
    #include <float.h>
#elif _WIN32
    #include "stdafx.h"
#else
#endif
