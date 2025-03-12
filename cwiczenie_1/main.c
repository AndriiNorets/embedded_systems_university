#include <stdio.h>
#include <stdlib.h>
int dodawanie(int a, int b)
{
    return a + b;
}

void czy_dodatnia(int liczba)
{
    if (liczba > 0)
    {
        printf("Liczba jest dodatnia");
    }
    else
    {
        printf("Liczba nie jest dodatnia");
    }
}

int sumuj_tab(int tab[], int size)
{
    int suma = 0;
    for (int i = 0; i < size; i++)
    {
        suma += tab[i];
    }
    return suma;
}

int zad1_1(int n)
{
    int result = 1;
    for (int i = 1; i < n + 1; i++)
    {
        result *= i;
    }
    return result;
}
int zad1_2(int start, int end)
{
    int result = 0;
    for (int i = start; i < end + 1; i++)
    {
        if (i % 2 == 0)
        {
            result += i;
        }
    }

    return result;
}

void zad1_3(int n)
{
    for (int i = 0; i <= n; i++)
    {
        int result = 1 * i;
        printf("%d * %d = %d \n", 1, i, result);
    }
}
int zad2_1(int liczba)
{
    int result = 0;
    while (liczba > 0)
    {
        result += liczba % 10;
        liczba /= 10;
    }
    return result;
}
int zad2_2(int liczba)
{
    int result = 0;
    while (liczba > 0)
    {
        result = result * 10 + (liczba % 10);

        liczba /= 10;
    }
    return result;
}
void zad3_1()
{
    int liczba = 0;
    do
    {
        printf("Wprowadz liczbe większą od 0:\n");
        scanf("%d", &liczba);
    } while (liczba < 1);

    printf("Wprowadzona liczba: %d\n", liczba);
}
int main()
{
    printf("Hello, Systemy Wbudowane!\n");

    int a = 10;
    double b = 3.14;
    char c = 'A';
    int d = 5;

    printf("int: %d, double: %.2f, char: %c", a, b, c);

    double suma1 = a + b;

    printf("\n suma a i b: %f", suma1);

    int suma2 = dodawanie(a, d);
    printf("%d", suma2);

    czy_dodatnia(a);

    // ZADANIE 1
    // 1.1
    printf("\n %d", zad1_1(5));

    // 1.2
    printf("\n %d \n", zad1_2(7, 10));

    // 1.3
    zad1_3(4);

    // ZADANIE 2
    // 2.1
    printf("suma cyfr liczby: %d \n", zad2_1(1234));

    // 2.2
    printf("Odwrócona liczba: %d \n", zad2_2(1234));

    // ZADANIE 3
    zad3_1();

    return 0;
}
