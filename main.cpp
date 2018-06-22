
#include "includes.h"
#include "interface.h"

using namespace std;

int Pos(char c);
bool verify(char * s, int base, bool havepoint);
long long ConvertInt(char * buffer, int base);
double power(long num, long deg);
long double ConvertFloat(char * buffer, int base);
void Bits(char data);
void Bits(void * data, int size);
void Invert(unsigned int arg);
void Invert8(unsigned long long arg);

int main (void) {

    // Настройка консоли для Windows
    #ifdef _WIN32
        setlocale(LC_ALL, "Russian");
        system ("chcp 1251");
    #endif

    clrscr;

    //Объявляем переменные
    char format;//Переменная для выяснения типа данных
    int base;//Система сччисления
    char buffer[80];//Объявление строки
    //Список возможных типов данных
    float Float;//Тип с плавающей запятой наименьшего размера.
    double Double;//Тип с плавающей запятой, размер которого больше или равен размеру типа float
    int Int;//Целочисленный тип
    long long Long;//Целочисленный тип размер которого больше или равен размеру типа int.
    short Short;//Целочисленный тип, размер которого меньше или равен размеру типа int
    char Char; //Целочисленный тип, обычно содержащий члены основной кодировки выполнения

    bool havepoint; //Переменная для проверки точки

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
        format = '?';
        while (
            format != 'f' &&
            format != 'd' &&
            format != 's' &&
            format != 'i' &&
            format != 'l' &&
            format != 'c'
            )
        {
            cout << "Формат данных: float, double, short, int, long long, char или quit" << endl;
            cin.getline(buffer, sizeof(buffer));
            format = buffer[0];
            if (format == 'q') return 0; //Завершаем работу программы если quit
            if (format == 0) return 0; //или просто enter
        }
        //Если float или double, значит подразумевается запятая
        havepoint = format == 'f' || format == 'd';
        //Выясняем систему счисления
        base = 0;
        while (base<2 || base >= (26+10))
        {
            cout << "Выберите систему счисления 2-36: ";
            cin.getline(buffer, sizeof(buffer));
            base = atoi(buffer);
        }
        //Читаем строку
        do { //Настойчиво требуем ввода 
            cout << "Введите значение: ";
            cin.getline(buffer, sizeof(buffer));
        } while (!verify(buffer, base, havepoint)); //Пока не введет верное значение

        long double value = ConvertFloat(buffer, base);
        if (value < 0) value = -value;
        bool overflow = false;
        //Проверка на переполнение
        switch (format)
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
        switch (format)
        {
        case 'f':
            Float = (float)ConvertFloat(buffer, base);			
            //Выводим двоичное (и десятичное для контроля правильности ввода) значения
            cout << "Десятичное " << Float << endl;
            Bits(&Float, sizeof(Float));			
            uni2.num = Float;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        case 'd':
            Double = (double)ConvertFloat(buffer, base);
            cout << "Десятичное " << Double << endl;
            Bits(&Double, sizeof(Double));
            uni3.num = Double;
            Invert8(uni3.byte8);
            Bits(&uni3.byte8, sizeof(uni3.byte8));
            break;
        case 'i':
            Int = (int)ConvertInt(buffer, base);
            cout << "Десятичное " << Int << endl;
            Bits(&Int, sizeof(Int));
            uni2.num = Int;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        case 'l':
            Long = (long int)ConvertInt(buffer, base);
            cout << "Десятичное " << Long << endl;
            Bits(&Long, sizeof(Long));
            uni2.num = Long;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        case 'c':
            Char = (char)ConvertInt(buffer, base);
            cout << "Десятичное " << (int)Char << endl;
            Bits(&Char, sizeof(Char));
            uni2.num = Char;
            Invert(uni2.byte4);
            Bits(&uni2.byte4, sizeof(uni2.byte4));
            break;
        case 's':
            Short = (short int)ConvertInt(buffer, base);
            cout << "Десятичное " << Short << endl;
            Bits(&Short, sizeof(Short));
            uni2.num = Short;
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
bool verify(char * s, int base, bool havepoint) {
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
    if (points > 0 && !havepoint) return false; //Если точка есть, но ее не должно быть
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
    int k = 0, s = 0, res = 1;
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
    mask_1 = mask_1 >> first_bit - second_bit;
    mask_2 = mask_2 << first_bit - second_bit;

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
    mask_1 = mask_1 >> first_bit - second_bit;
    mask_2 = mask_2 << first_bit - second_bit;

    // Вставляем исходные группы в данное число (уже перемещённые)
    arg = arg | mask_1;
    arg = arg | mask_2;

    // Выводим результат
    cout << "Результат: " << endl;
    Bits(&arg, sizeof(arg));

}