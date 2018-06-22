
#include "includes.h"  // Подключение системных библиотек
#include "interface.h" // Подключение интерфейсных определений

using namespace std;

// Прототипы функций
char get_data_type (char* input_buffer);
int get_number_base (char* input_buffer);
int get_char_number_base (char);
bool input_number_verify (char* input_buffer, int number_base, bool flag_floating_point);
void get_input_number (char* input_buffer, int number_base, bool flag_floating_point);
double power (long number, long degree);
bool overflow_number_check (char* input_buffer, int number_base, char data_type);
long long convert_integer (char * buffer, int base);
long double convert_float (char * buffer, int base);
void print_binary (char data);
void swap_bit_group_1byte  (char arg);
void print_binary (void * data, int size);
void swap_bit_group_2bytes (unsigned short arg);
void swap_bit_group_4bytes (unsigned int arg);
void swap_bit_group_8bytes (unsigned long long arg);

// Непосредственно программа
int main (void) {

    // Настройка консоли для Windows
    #ifdef _WIN32
        setlocale(LC_ALL, "Russian");
        system ("chcp 1251");
    #endif

    clrscr;

    // Объявление переменных
    char data_type = '.';  // Тип данных вводимого значени
    int number_base = 0;   // Основание системы счисления вводимого значения
    char* input_buffer = new char[80]; // Строковый буфер для ввода значения
    // Переменные различного типа для ввода значения 
    float input_number_float;    // Число с плавающей запятой - 4 байта
    double input_number_double;  // Число с плавающей запятой - 8 байт
    char input_number_char;      // Целое число - 1 байт
    short input_number_short;    // Целое число - 2 байта
    int input_number_int;        // Целое число - 4 байта
    long long input_number_long; // Целое число - 8 байт
    bool flag_floating_point; // Флаг, индицирующий, что число является числом с плавающей запятой
    bool flag_overflow;       // Флаг, индицирующий, что введённое значение выходит за рамки допустимых значений

    // Используем механизм объединений для упрощения работы с числами с плавающей запятой
    // Объединение для представления float как unsigned int - 4 байта данных
    union {
        float real;
        unsigned int integer;
    } float_as_int;
    
    // Объединение для представления double как unsigned long long - 8 байт данных
    union {
        double real;
        unsigned long long integer;
    } double_as_long;

    // Повторяем алгоритм пока пользователь не пожелает выйти
    while (true) {
        // Получаем тип данных
        data_type = get_data_type (input_buffer);

        // Завершаем работу программы если пользователь ввёл quit
        if (data_type == 'q')
            return 0; 

        // Определяем, выбрал ли пользователь тип данных с плавающей запятой
        flag_floating_point = (data_type == 'f' || data_type == 'd');

        // Получаем основание системы счисления
        number_base = get_number_base (input_buffer);
        
        // Получаем непосредственно значение
        get_input_number (input_buffer, number_base, flag_floating_point);

        // Проверяем введённое значение на переполнение
        flag_overflow = overflow_number_check (input_buffer, number_base, data_type);
        if (flag_overflow) {
            cout << "Ошибка: введённое число выходит за рамки допустимых значений для данного типа!" << endl << endl;
            continue;
        }

        // Преобразование строки в указанный тип данных и вывод
        switch (data_type) {
            case 'c': // char
                input_number_char = (char)convert_integer(input_buffer, number_base); // Преобразуем строку в число
                
                cout << "Введённое значение (DEC): " << (int)input_number_char << endl;
                cout << "BIN: "; print_binary(&input_number_char, sizeof(input_number_char));

                swap_bit_group_1byte(input_number_char); // Произведём перестановку групп бит
                cout << "Orig: "; print_binary(&input_number_char, sizeof(input_number_char));
                break;
            case 's': // short
                input_number_short = (short int)convert_integer(input_buffer, number_base); // Преобразуем строку в число
                
                cout << "Введённое значение (DEC): " << input_number_short << endl;
                cout << "BIN: "; print_binary(&input_number_short, sizeof(input_number_short));

                swap_bit_group_2bytes(input_number_short); // Произведём перестановку групп бит
                cout << "Orig: "; print_binary(&input_number_short, sizeof(input_number_short));
                break;
            case 'i': // int
                input_number_int = (int)convert_integer(input_buffer, number_base); // Преобразуем строку в число
                
                cout << "Введённое значение (DEC): " << input_number_int << endl;
                cout << "BIN: "; print_binary(&input_number_int, sizeof(input_number_int));

                swap_bit_group_4bytes(input_number_int); // Произведём перестановку групп бит
                cout << "Orig: "; print_binary(&input_number_int, sizeof(input_number_int));
                break;
            case 'l': // long long
                input_number_long = (long int)convert_integer(input_buffer, number_base); // Преобразуем строку в число
                
                cout << "Введённое значение (DEC): " << input_number_long << endl;
                cout << "BIN: "; print_binary(&input_number_long, sizeof(input_number_long));

                swap_bit_group_8bytes(input_number_long); // Произведём перестановку групп бит
                cout << "Orig: "; print_binary(&input_number_long, sizeof(input_number_long));
                break;
            case 'f': // float
                input_number_float = (float)convert_float(input_buffer, number_base); // Преобразуем строку в число
                
                cout << "Введённое значение (DEC): " << input_number_float << endl;
                cout << "BIN: "; print_binary(&input_number_float, sizeof(input_number_float));

                float_as_int.real = input_number_float;
                swap_bit_group_4bytes(float_as_int.integer); // Произведём перестановку групп бит
                cout << "Orig: "; print_binary(&float_as_int.integer, sizeof(float_as_int.integer));
                break;
            case 'd': // double
                input_number_double = (double)convert_float(input_buffer, number_base); // Преобразуем строку в число
                
                cout << "Введённое значение (DEC): " << input_number_double << endl;
                cout << "BIN: "; print_binary(&input_number_double, sizeof(input_number_double));

                double_as_long.real = input_number_double;
                swap_bit_group_8bytes(double_as_long.integer); // Произведём перестановку групп бит
                cout << "Orig: "; print_binary(&double_as_long.integer, sizeof(double_as_long.integer));
                break;
        }
        cout << endl << "Для продолжения нажмите ENTER ... ";
        pause;
    }
}

// Функция чтения с клавиатуры типа вводимого значения
char get_data_type (char* input_buffer) {
    char data_type = '?';
    
    while ( data_type != 'c' &&
            data_type != 's' &&
            data_type != 'i' &&
            data_type != 'l' &&
            data_type != 'f' &&
            data_type != 'd' &&
            data_type != 'q') {

        cout << "Доступные типы данных значения для выбора: char, short, int, long long, float, double." << endl
             << "Для выхода из программы введите quit." << endl
             << "Введите имя типа данных и нажмите ENTER: ";
        cin.clear(); fflush(stdin);  // Чистка буфера потока ввода
        cin.getline(input_buffer, sizeof(input_buffer));
        data_type = input_buffer[0];

       if ( data_type != 'c' &&
            data_type != 's' &&
            data_type != 'i' &&
            data_type != 'l' &&
            data_type != 'f' &&
            data_type != 'd' &&
            data_type != 'q') {
            cout << "Неверный тип данных. Повторите ввод." << endl << endl;
        }
    }

    return data_type;
}

// Функция ввода основания системы счисления
int get_number_base (char* input_buffer) {
    int number_base = 0;

    while (number_base < 2 || number_base >= (26+10)) {
        cout << "Введите основние системы счисления от 2 до 36 и нажмите ENTER: ";
        cin.clear(); fflush(stdin); // Чистка буфера потока ввода
        cin.getline(input_buffer, sizeof(input_buffer));
        number_base = atoi(input_buffer);

        if (number_base < 2 || number_base >= (26+10))
            cout << "Неверное основание системы счисления. Повторите ввод." << endl << endl;
    }

    return number_base;
}

// Функция ввода значения
void get_input_number (char* input_buffer, int number_base, bool flag_floating_point) {
        bool flag_number_acceptable = false;

        do {
            cout << "Введите число в системе счисления с основанием " << number_base << " и нажмите ENTER: ";
            cin.getline(input_buffer, sizeof(input_buffer)+1);
            flag_number_acceptable = input_number_verify(input_buffer, number_base, flag_floating_point);
        
            if (!flag_number_acceptable)
                cout << "Введённое значение некорректно. Повторите ввод." << endl << endl;
        
        } while (!flag_number_acceptable);
}

// Функция возвращает численное основание системы счисления для введённого символа
int get_char_number_base (char buffer_char) {
    char number_base_alphabet[] = "0123456789ABCDEFGHIJKLMNPOPQRSTUVWXYZ"; // Символы, допустимые в системах счисления с основаниями 2-36 включительно
    int number_base = 0;  

    // Высчитываем номер данного символа в алфавите систем счисления
    for (; number_base_alphabet[number_base] != 0 && number_base_alphabet[number_base] != buffer_char; number_base++);

    return number_base;
}

// Функция проверки введённого значения на допустимость символов для указанной системы счисления
// Функция также переводит введённые буквы в верхний регистр
bool input_number_verify (char* input_buffer, int number_base, bool flag_floating_point) {
    int point_counter = 0; // Счетчик количества точек (отделяющих дробную часть)

    // Оперируем строкой с введённым значением посимвольно
    for (int char_counter = 0; input_buffer[char_counter] != 0; char_counter++) {
        // Копируем текущий символ в буферную переменную на случай необходимости преобразования
        char buffer_char = input_buffer[char_counter];

        // Первый символ строки может быть знаком введённого числа
        if (char_counter == 0 && (buffer_char == '-' || buffer_char == '+'))
            continue;
        // Если символ - буква в нижнем регистре - переводим её в верхний регистр
        if (buffer_char >= 'a' && buffer_char <= 'z') {
            buffer_char += 'A' - 'a';
            input_buffer[char_counter] = buffer_char;
        }
        // Если символ - запятая - заменяем её на точку для корректности представления
        if (buffer_char == ',') {
            buffer_char = '.';
            input_buffer[char_counter] = buffer_char;
        }
        // Если символ - точка - считаем её и переходим к следующему символу
        if (buffer_char == '.') {
            point_counter++;
            continue;
        }
        // Если символ допустим для данной системы счисления - переходим к следующему символу
        if (get_char_number_base(buffer_char) < number_base)
            continue;

        return false; // Во всех иных случаях символ недопустим
    }

    // Если точек больше одной - введённое значение недопустимо
    if (point_counter > 1)
        return false;
    // Если точка присутствует, но её не должно быть - введённое значение недопустимо
    if (point_counter > 0 && !flag_floating_point)
        return false;

    return true; // В иных случаях значение проходит проверку корректности
}

// Функция возведения целого числа в положительную или отрицательную степень
double power (long number, long degree) {
    double result = 1;

    if (degree < 0) {
        degree = -degree;
        for (long i = 0; i < degree; i++)
            result *= number;

        return 1 / result;
    } else {
        for (long i = 0; i < degree; i++)
            result *= number;

        return result;
    }
}

// Функция проверки значения на переполнение типа
bool overflow_number_check (char* input_buffer, int number_base, char data_type) {
    long double value = convert_float(input_buffer, number_base); // Конвертируем строку в буфферную переменную большого размера
    bool flag_overflow = false;

    if (value < 0) // Если значение отрицательное - принудительно сделаем его положительным
        value = -value;

    // Проверка на попадание полученного значения в рамки допустимых для указанного типа данных
    switch (data_type) {
        case 'f': flag_overflow = value > FLT_MAX;   break;
        case 'd': flag_overflow = value > DBL_MAX;   break;
        case 'l': flag_overflow = value > INT64_MAX; break;
        case 'i': flag_overflow = value > INT32_MAX; break;
        case 'c': flag_overflow = value > 255;       break;
        case 's': flag_overflow = value > INT16_MAX; break;
    }
    
    return flag_overflow;
}

// Функция преобразования строки в целое число. Максимальный размер - 64 бита.
long long convert_integer (char * input_buffer, int number_base) {
    long long result = 0;  // Результат преобразования
    int char_counter = 0;  // Индекс символа в строке
    bool flag_negative = false; // Флаг, показывающий, что число является отрицательным

    // Анализируем первый символ в строке
    switch (input_buffer[0]) {
        case 0: return 0;          // Если строка пуста - значение будет 0
        case '-' : flag_negative = true; // Если первый символ - минус - поднимаем флаг
        case '+' : char_counter++;       // Если первый символ - плюс - пропускаем символ
    }

    // Переводим число в десятичную систему счисления умножением, проходя строку посимвольно
    for (; input_buffer[char_counter] != 0; char_counter++)
        result = result * number_base + get_char_number_base(input_buffer[char_counter]);

    if (flag_negative) // Если число отрицательное - делаем его отрицательным
        result = -result;

    return result;
}

// Функция преобразования строки в число с плавающей точкой. Максимальный размер - от 96 до 128 бит.
long double convert_float (char * input_buffer, int number_base) {
    long double result = 0;     // Результат преобразования
    bool flag_negative = false; // Флаг, показывающий, что число является отрицательным
    int char_counter = 0;       // Счётчик символов
    long double number_base_multiplier = 1.0; // Мультипликатор, основанный на основании системы счисления
    long double conversion_multiplier = 1.0;  // Мультипликатор преобразания, основынный на основании системы счисления и разряде
    long double number_power = number_base;   // Мультипликатор степени
    
    // Анализируем первый символ в строке
    switch (input_buffer[0]) {
        case 0: return 0;          // Если строка пуста - значение будет 0
        case '-' : flag_negative = true; // Если первый символ - минус - поднимаем флаг
        case '+' : char_counter++;       // Если первый символ - плюс - пропускаем символ
    }

    // Переводим число в десятичную систему счисления умножением, проходя строку посимвольно
    for (; input_buffer[char_counter] != 0; char_counter++) {
        
        if (input_buffer[char_counter] == '.') {
            number_base_multiplier = 1.0 / number_base; // Если мы перешли к дробной части - меняем её пультипликатор
            number_power = 1.0; // Устанавливаем мультипликатор целой части в нейтральное относительно подсчёта значение
            continue;           // Переходим к следующему символу
        }
        conversion_multiplier *= number_base_multiplier; // Обновляем мультиплиактор дробной части

        // Высчитываем результат
        result = number_power * result + conversion_multiplier * get_char_number_base(input_buffer[char_counter]);
    }

    if (flag_negative) // Если число отрицательное - делаем его отрицательным
        result = -result;

    return result;
}

// Функция вывода байта в двоичном виде
void print_binary (char data) {
    unsigned char mask = 0x80; // Исходная маска

    // Для всех 8 бит байта
    for (int bit = 0; bit < 8; bit++) { 
        cout << ((data & mask) ? "1" : "0"); // Выводим бит 
        mask >>= 1;                          // Сдвигаем маску
        
        if (bit == 3)
            cout << " "; // Вставляем пробел между тетрадами

    }
    cout << " "; // Вставляем пробел между байтами
}

// Функция вывода числа в двоичном виде
void print_binary (void * data, int size) {
    char* data_string = (char*)data; // Разбиваем число на байты

    // Выводим число побайтово
    for (int byte = size - 1; byte >= 0; byte--)
        print_binary(data_string[byte]);

    cout << endl;
}

// Функция перестановки группы бит для числа размером 1 байт
void swap_bit_group_1byte (char arg) {

    int group_size; // Размер группы для замены
    int first_bit;  // Старший бит первой группы
    int second_bit; // Старший бит второй группы

    char mask_1 = 2; // Битовая маска 1 (00...10)
    char mask_2 = 0; // Битовая маска 2 (00...00)
    char mask_3 = 0xFF; // Битовая маска (11...11) // u - указание того, что значение беззнаковое

    // Вввод значений
    cout << endl
         << "Перестановка групп бит в двоичном представлении." << endl << "Внимание: нумерация бит начинается с 0 и идёт справа налево." << endl;
    cout << "Введите количество бит в группе для перестановки: ";
    cin >> group_size;
    group_size--; // Уменьшаем размер группы на 1, чтобы соответствовать нумерации бит с 0
    cout << "Введите номер старшего бита в 1 группе: ";
    cin >> first_bit;
    cout << "Введите номер старшего бита в 2 группе: ";
    cin >> second_bit;

    // Если бит первой группы правее, чем второй - делаем swap для номеров этих битов, обеспечивая таким образом инвариантность алгоритма
    if (first_bit < second_bit) {
        int temp = first_bit;
        first_bit = second_bit;
        second_bit = temp;
    }

    // Получаем маску для секции размера группы +1, которую мы переставляем
    mask_1 = mask_1 << group_size;
    mask_1--;
    mask_2 = mask_1; // Создаём копию этой маски

    mask_1 = mask_1 << (first_bit - group_size);  // Покрываем первую группу маской
    mask_2 = mask_2 << (second_bit - group_size); // Покрываем вторую группу маской

    mask_1 = mask_1 & arg; // Вычленяем из данного числа секцию 1 при помощи битовой маски
    mask_2 = mask_2 & arg; // Вычленяем из данного числа секцию 2 при помощи битовой маски

    // Получаем маску с 0 на местах 1 в целевых группах исходного числа
    mask_3 = mask_3 ^ mask_1;
    mask_3 = mask_3 ^ mask_2;

    // Обнуляем искомые группы в исходном числе
    arg = arg & mask_3;

    // При помощи сдвига меняем местами отображения искомых групп в масках
    mask_1 = mask_1 >> (first_bit - second_bit);
    mask_2 = mask_2 << (first_bit - second_bit);

    // Вставляем исходные группы в данное число (уже перемещённые)
    arg = arg | mask_1;
    arg = arg | mask_2;

    // Выводим результат
    cout << endl << "Результат перестановки:" << endl << "Swap: ";
    print_binary(&arg, sizeof(arg));

}

// Функция перестановки группы бит для числа размером 2 байта
void swap_bit_group_2bytes (unsigned short arg) {

    int group_size; // Размер группы для замены
    int first_bit;  // Старший бит первой группы
    int second_bit; // Старший бит второй группы

    unsigned short mask_1 = 2; // Битовая маска 1 (00...10)
    unsigned short mask_2 = 0; // Битовая маска 2 (00...00)
    unsigned short mask_3 = 0xFFFFu; // Битовая маска (11...11) // u - указание того, что значение беззнаковое

    // Вввод значений
    cout << endl
         << "Перестановка групп бит в двоичном представлении." << endl << "Внимание: нумерация бит начинается с 0 и идёт справа налево." << endl;
    cout << "Введите количество бит в группе для перестановки: ";
    cin >> group_size;
    group_size--; // Уменьшаем размер группы на 1, чтобы соответствовать нумерации бит с 0
    cout << "Введите номер старшего бита в 1 группе: ";
    cin >> first_bit;
    cout << "Введите номер старшего бита в 2 группе: ";
    cin >> second_bit;

    // Если бит первой группы правее, чем второй - делаем swap для номеров этих битов, обеспечивая таким образом инвариантность алгоритма
    if (first_bit < second_bit) {
        int temp = first_bit;
        first_bit = second_bit;
        second_bit = temp;
    }

    // Получаем маску для секции размера группы +1, которую мы переставляем
    mask_1 = mask_1 << group_size;
    mask_1--;
    mask_2 = mask_1; // Создаём копию этой маски

    mask_1 = mask_1 << (first_bit - group_size);  // Покрываем первую группу маской
    mask_2 = mask_2 << (second_bit - group_size); // Покрываем вторую группу маской

    mask_1 = mask_1 & arg; // Вычленяем из данного числа секцию 1 при помощи битовой маски
    mask_2 = mask_2 & arg; // Вычленяем из данного числа секцию 2 при помощи битовой маски

    // Получаем маску с 0 на местах 1 в целевых группах исходного числа
    mask_3 = mask_3 ^ mask_1;
    mask_3 = mask_3 ^ mask_2;

    // Обнуляем искомые группы в исходном числе
    arg = arg & mask_3;

    // При помощи сдвига меняем местами отображения искомых групп в масках
    mask_1 = mask_1 >> (first_bit - second_bit);
    mask_2 = mask_2 << (first_bit - second_bit);

    // Вставляем исходные группы в данное число (уже перемещённые)
    arg = arg | mask_1;
    arg = arg | mask_2;

    // Выводим результат
    cout << endl << "Результат перестановки:" << endl << "Swap: ";
    print_binary(&arg, sizeof(arg));

}

// Функция перестановки группы бит для числа размером 4 байта
void swap_bit_group_4bytes (unsigned int arg) {

    int group_size; // Размер группы для замены
    int first_bit;  // Старший бит первой группы
    int second_bit; // Старший бит второй группы

    unsigned int mask_1 = 2; // Битовая маска 1 (00...10)
    unsigned int mask_2 = 0; // Битовая маска 2 (00...00)
    unsigned int mask_3 = 0xFFFFFFFFu; // Битовая маска (11...11) // u - указание того, что значение беззнаковое

    // Вввод значений
    cout << endl
         << "Перестановка групп бит в двоичном представлении." << endl << "Внимание: нумерация бит начинается с 0 и идёт справа налево." << endl;
    cout << "Введите количество бит в группе для перестановки: ";
    cin >> group_size;
    group_size--; // Уменьшаем размер группы на 1, чтобы соответствовать нумерации бит с 0
    cout << "Введите номер старшего бита в 1 группе: ";
    cin >> first_bit;
    cout << "Введите номер старшего бита в 2 группе: ";
    cin >> second_bit;

    // Если бит первой группы правее, чем второй - делаем swap для номеров этих битов, обеспечивая таким образом инвариантность алгоритма
    if (first_bit < second_bit) {
        int temp = first_bit;
        first_bit = second_bit;
        second_bit = temp;
    }

    // Получаем маску для секции размера группы +1, которую мы переставляем
    mask_1 = mask_1 << group_size;
    mask_1--;
    mask_2 = mask_1; // Создаём копию этой маски

    mask_1 = mask_1 << (first_bit - group_size);  // Покрываем первую группу маской
    mask_2 = mask_2 << (second_bit - group_size); // Покрываем вторую группу маской

    mask_1 = mask_1 & arg; // Вычленяем из данного числа секцию 1 при помощи битовой маски
    mask_2 = mask_2 & arg; // Вычленяем из данного числа секцию 2 при помощи битовой маски

    // Получаем маску с 0 на местах 1 в целевых группах исходного числа
    mask_3 = mask_3 ^ mask_1;
    mask_3 = mask_3 ^ mask_2;

    // Обнуляем искомые группы в исходном числе
    arg = arg & mask_3;

    // При помощи сдвига меняем местами отображения искомых групп в масках
    mask_1 = mask_1 >> (first_bit - second_bit);
    mask_2 = mask_2 << (first_bit - second_bit);

    // Вставляем исходные группы в данное число (уже перемещённые)
    arg = arg | mask_1;
    arg = arg | mask_2;

    // Выводим результат
    cout << endl << "Результат перестановки:" << endl << "Swap: ";
    print_binary(&arg, sizeof(arg));

}

// Функция перестановки группы бит для числа размером 8 байт
void swap_bit_group_8bytes (unsigned long long arg) {

    int group_size; // Размер группы для замены
    int first_bit;  // Старший бит первой группы
    int second_bit; // Старший бит второй группы

    unsigned long long mask_1 = 2; // Битовая маска 1 (00...10)
    unsigned long long mask_2 = 0; // Битовая маска 2 (00...00)
    unsigned long long mask_3 = 0xFFFFFFFFFFFFFFFFu; // Битовая маска (11...11) // u - указание того, что значение беззнаковое

    // Вввод значений
    cout << endl
         << "Перестановка групп бит в двоичном представлении." << endl << "Внимание: нумерация бит начинается с 0 и идёт справа налево." << endl;
    cout << "Введите количество бит в группе для перестановки: ";
    cin >> group_size;
    group_size--; // Уменьшаем размер группы на 1, чтобы соответствовать нумерации бит с 0
    cout << "Введите номер старшего бита в 1 группе: ";
    cin >> first_bit;
    cout << "Введите номер старшего бита в 2 группе: ";
    cin >> second_bit;

    // Если бит первой группы правее, чем второй - делаем swap для номеров этих битов, обеспечивая таким образом инвариантность алгоритма
    if (first_bit < second_bit) {
        int temp = first_bit;
        first_bit = second_bit;
        second_bit = temp;
    }

    // Получаем маску для секции размера группы +1, которую мы переставляем
    mask_1 = mask_1 << group_size;
    mask_1--;
    mask_2 = mask_1; // Создаём копию этой маски

    mask_1 = mask_1 << (first_bit - group_size);  // Покрываем первую группу маской
    mask_2 = mask_2 << (second_bit - group_size); // Покрываем вторую группу маской

    mask_1 = mask_1 & arg; // Вычленяем из данного числа секцию 1 при помощи битовой маски
    mask_2 = mask_2 & arg; // Вычленяем из данного числа секцию 2 при помощи битовой маски

    // Получаем маску с 0 на местах 1 в целевых группах исходного числа
    mask_3 = mask_3 ^ mask_1;
    mask_3 = mask_3 ^ mask_2;

    // Обнуляем искомые группы в исходном числе
    arg = arg & mask_3;

    // При помощи сдвига меняем местами отображения искомых групп в масках
    mask_1 = mask_1 >> (first_bit - second_bit);
    mask_2 = mask_2 << (first_bit - second_bit);

    // Вставляем исходные группы в данное число (уже перемещённые)
    arg = arg | mask_1;
    arg = arg | mask_2;

    // Выводим результат
    cout << endl << "Результат перестановки:" << endl << "Swap: ";
    print_binary(&arg, sizeof(arg));

}
