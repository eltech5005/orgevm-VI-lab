
// Вариативная очистка экрана для Windows и Linux
#ifndef clrscr
    #ifdef _WIN32
        #define clrscr system("cls")
    #else
        #define clrscr system("clear")
    #endif
#endif

// Вариативная пауза ввода для Windows и Linux
#ifndef pause
    #ifdef _WIN32
        #define pause system("pause")
    #else
        #define pause system("read pause")
    #endif
#endif
