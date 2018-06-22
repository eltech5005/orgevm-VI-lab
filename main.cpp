
#include "includes.h"  // Подключение системных библиотек
#include "interface.h" // Подключение интерфейсных определений

using namespace std;

// Прототипы функций
int Pos(char c);
bool verify(char * s, int base, bool flag_floating_point);
long long ConvertInt(char * buffer, int base);
double power(long num, long deg);
long double ConvertFloat(char * buffer, int base);
void Bits(char data);
void Bits(void * data, int size);
void Invert(unsigned int arg);
void Invert8(unsigned long long arg);

// Непосредственно программа
int main (void) {

    // Настройка консоли для Windows
    #ifdef _WIN32
        setlocale(LC_ALL, "Russian");
        system ("chcp 1251");
    #endif

    clrscr;

    // Объявление переменных
    char data_type;        // Тип данных вводимого значени
    int number_base;       // Основание системы счисления вводимого значения
    char input_buffer[80]; // Строковый буфер для ввода значения
    // Переменные различного типа для ввода значения 
    float input_number_float;    // Число с плавающей запятой - 4 байта
    double input_number_double;  // Число с плавающей запятой - 8 байт
    char input_number_char;      // Целое число - 1 байт
    short input_number_short;    // Целое число - 2 байта
    int input_number_int;        // Целое число - 4 байта
    long long input_number_long; // Целое число - 8 байт
    bool flag_floating_point; // Флаг, индицирующий, что число является числом с плавающей запятой


    union {
        float num; //4 байта
        unsigned int byte4; //4 байта
    }
    uni2;
    
    union {
        double num; //8 байт
        unsigned long long byte8; //8 байт
    }
    uni3;

    while (true)
    {
        //Выясняем формат данных
        data_type = '?';
        while (
            data_type != 'f' &&
            data_type != 'd' &&
            data_type != 's' &&
            data_type != 'i' &&
            data_type != 'l' &&
            data_type != 'c'
            )
        {
            cout << "Формат данных: float, double, short, int, long long, char или quit" << endl;
            cin.getline(input_buffer, sizeof(input_buffer));
            data_type = input_buffer[0];
            if (data_type == 'q') return 0; //Завершаем работу программы если quit
            if (data_type == 0) return 0; //или просто enter
        }
        //Если float или double, значит подразумевается запятая
        flag_floating_point = data_type == 'f' || data_type == 'd';
        //Выясняем систему счисления
        number_base = 0;
        while (number_base<2 || number_base >= (26+10))
        {
            cout << "Выберите систему счисления 2-36: ";
            cin.getline(input_buffer, sizeof(input_buffer));
            number_base = atoi(input_buffer);
        }
        //Читаем строку
        do { //Настойчиво требуем ввода 
            cout << "Введите значение: ";
            cin.getline(input_buffer, sizeof(input_buffer));
        } while (!verify(input_buffer, number_base, flag_floating_point)); //Пока не введет верное значение

        long double value = ConvertFloat(input_buffer, number_base);
        if (value < 0) value = -value;
        bool overflow = false;
        //Проверка на переполнение
        switch (data_type)
        {
        case 'f':overflow = value > FLT_MAX; break;
        case 'd':overflow = value > DBL_MAX; break;
        case 'l':overflow = value > INT64_MAX; break;
        case 'i':overflow = value > INT32_MAX; break;
        case 'c':overflow = value > 255; break;
        case 's':overflow = value > INT16_MAX; break;
        }
        //Обработка переполнения
        if (overflow)
        {
            cout << "Переполнение" << endl;
            continue;
        }

        //Преобразование из строки в нужный тип данных
        switch (data_type)
        {
        case 'f':
            input_number_float = (float)ConvertFloat(input_buffer, number_base);			
            //Выводим двоичное (и десятичное для контроля правильности ввода) значения
            cout << "Десятичное " << input_number_float << endl;
            Bits(&input_number_float, sizeof(input_number_float));			
            uni2.num = input_number_float;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        case 'd':
            input_number_double = (double)ConvertFloat(input_buffer, number_base);
            cout << "Десятичное " << input_number_double << endl;
            Bits(&input_number_double, sizeof(input_number_double));
            uni3.num = input_number_double;
            Invert8(uni3.byte8);
            Bits(&uni3.byte8, sizeof(uni3.byte8));
            break;
        case 'i':
            input_number_int = (int)ConvertInt(input_buffer, number_base);
            cout << "Десятичное " << input_number_int << endl;
            Bits(&input_number_int, sizeof(input_number_int));
            uni2.num = input_number_int;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        case 'l':
            input_number_long = (long int)ConvertInt(input_buffer, number_base);
            cout << "Десятичное " << input_number_long << endl;
            Bits(&input_number_long, sizeof(input_number_long));
            uni2.num = input_number_long;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        case 'c':
            input_number_char = (char)ConvertInt(input_buffer, number_base);
            cout << "Десятичное " << (int)input_number_char << endl;
            Bits(&input_number_char, sizeof(input_number_char));
            uni2.num = input_number_char;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        case 's':
            input_number_short = (short int)ConvertInt(input_buffer, number_base);
            cout << "Десятичное " << input_number_short << endl;
            Bits(&input_number_short, sizeof(input_number_short));
            uni2.num = input_number_short;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        }
        pause;
    }
}

int Pos(char c) {
    int result = 0;
    //Объявляем алфавит
    char Alphabet[] = "0123456789ABCDEFGHIJKLMNPOPQRSTUVWXYZ";
    for (; Alphabet[result] != 0 && Alphabet[result] != c; result++);
    return result;
}

//Проверка на допустимость символов, заодно перевести на верхний регистр
bool verify(char * s, int base, bool flag_floating_point) {
    int points = 0; //Счетчик точек (отделяющих дробную часть)
    for (int k = 0; s[k] != 0; k++)
    {
        char c = s[k];
        if (k == 0 && (c == '-' || c == '+')) continue; //В начале может быть знак
        if (c >= 'a' && c <= 'z') { c += 'A' - 'a'; s[k] = c; } //Верхний регистр
        if (c == ',') { c = '.'; s[k] = c; }
        if (c == '.') { points++;  continue; } 
        if (Pos(c) < base) continue; //Проверка на законный символ
        return false; //Если незаконный символ
    }
    if (points > 1) return false; //Если слишком много точек
    if (points > 0 && !flag_floating_point) return false; //Если точка есть, но ее не должно быть
    return true; //Все символы допустимые
}

//Функция преобразования целого числа
//Самое длинное машинное представление - 64 бита
long long ConvertInt(char * buffer, int base) {
    long long result = 0; //Результат
    int k = 0; //Индекс в строке
    bool negative = false; //Признак отрицательного числа
    switch (buffer[0])
    {
    case 0: return 0; //В том случае если пустая строка - то и результат нулевой
    case '-' : negative = true; //Случился первый символ минус	
    case '+' : k++; //Или плюс
    };

    for (; buffer[k] != 0; k++) //Проходим до конца строки
        result = result * base + Pos(buffer[k]);
    if (negative) result = -result; //Не зря же минус определяли
    return result;
}

//Функция озведения в степень(полож. и отриц.)
double power(long num, long deg) {
    double result = 1;

    if (deg < 0) {
        deg = -deg;
        for (long i = 0; i < deg; i++) {
            result *= num;
        }

        return 1 / result;
    }
    else {
        for (long i = 0; i < deg; i++) {
            result *= num;
        }

        return result;
    }
}

//Функция преобразования числа с плавающей точкой
//Самое точное машинное представление - long double
long double ConvertFloat(char * buffer, int base) {
    long double result = 0;
    bool negative = false;
    int k = 0;
    long double m = 1.0; //На что умножать число
    long double power = base;
    long double multiplier = 1.0;
    switch (buffer[0])
    {
    case 0: return 0;
    case '-':negative = true;
    case '+':k++;
    };

    for (; buffer[k] != 0; k++) //до конца строки
    {
        if (buffer[k] == '.') {
            multiplier = 1.0 / base;//Встретилась точка
            power = 1.0;
            continue;
        }
        m *= multiplier;//Теперь множитель стал таким
        result = power * result + m * Pos(buffer[k]);
    }
    if (negative) result = -result;
    return result;
}

//Функция вывода (побитно один байт)
void Bits(char data) {
    unsigned char mask = 0x80; //Начиная с такой маски
    for (int k = 0; k < 8; k++) //ровно 8 раз (столько бит в байте)
    {
        cout << ((data & mask) ? "1" : "0");
        mask >>= 1;
        if (k == 3) cout << " "; //Пробел между тетрадами
        
    }
    cout << " "; //пробел между байтами
}

//Вывести побитно данные размером size байт
void Bits(void * data, int size) {
    char * Data = (char*)data;
    for (int k = size - 1; k >= 0; k--)
        Bits(Data[k]);
    cout << endl;
}

void Invert(unsigned int arg) {

    int group_size; // Размер группы для замены
    int first_bit;  // Старший бит первой группы
    int second_bit; // Старший бит второй группы

    unsigned int mask_1 = 2; // Битовая маска 1 (00...10)
    unsigned int mask_2 = 0; // Битовая маска 2 (00...00)
    unsigned int mask_3 = 0xFFFFFFFFu; // Битовая маска (11...11) // u - указание того, что значение беззнаковое

    // Вввод значений
    cout << "Введите кол-во бит в группе:" << endl;
    cin >> group_size;
    group_size--; // Уменьшаем размер группы на 1, чтобы соответствовать нумерации бит с 0
    cout << "Введите номер старшего бита в 1 группе:" << endl;
    cin >> first_bit;
    cout << "Введите номер старшего бита в 2 группе:" << endl;
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
    cout << "Результат: " << endl;
    Bits(&arg, sizeof(arg));

}

void Invert8(unsigned long long arg) {

    int group_size; // Размер группы для замены
    int first_bit;  // Старший бит первой группы
    int second_bit; // Старший бит второй группы

    unsigned long long mask_1 = 2; // Битовая маска 1 (00...10)
    unsigned long long mask_2 = 0; // Битовая маска 2 (00...00)
    unsigned long long mask_3 = 0xFFFFFFFFFFFFFFFFu; // Битовая маска (11...11) // u - указание того, что значение беззнаковое

    // Вввод значений
    cout << "Введите кол-во бит в группе:" << endl;
    cin >> group_size;
    group_size--; // Уменьшаем размер группы на 1, чтобы соответствовать нумерации бит с 0
    cout << "Введите номер старшего бита в 1 группе:" << endl;
    cin >> first_bit;
    cout << "Введите номер старшего бита в 2 группе:" << endl;
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
    cout << "Результат: " << endl;
    Bits(&arg, sizeof(arg));

}
