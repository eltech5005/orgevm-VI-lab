// Вариативный список заголовков для Windows и Linux
#ifdef __linux__ 
    #include <stdlib.h>
    #include <iostream>
    #include <float.h>
#elif _WIN32
    #include "stdafx.h"
    #include <stdlib.h>
    #include <iostream>
#else
#endif