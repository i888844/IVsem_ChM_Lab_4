#include <iostream>
#include <windows.h>
#include <math.h>

using namespace std;

#define START 1.0
#define END 5.0

long double factorial(int n)
{
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

long double dividedDifference(int n, int k, long double result[][12])
{
    if (k == 0) return result[n][1];
    return dividedDifference(n + 1, k - 1, result) - dividedDifference(n, k - 1, result);
}

long double interpolateFirst(int n, long double x, long double result[][12])
{
    long double value = result[0][1];
    long double term = 1;

    for (int k = 1; k <= n - 1; k++)
    {
        term *= (x - result[k - 1][0]) / (k);
        value += term * dividedDifference(0, k, result);
    }

    return value;
}

long double interpolateSecond(int n, long double x, long double result[][12])
{
    long double value = result[n - 1][1];
    long double q = (x - result[n - 1][0]) / (result[1][0] - result[0][0]);
    long double term = q;

    for (int k = 2; k <= n - 1; k++)
    {
        term *= (q - k + 1) / factorial(k);
        value += term * dividedDifference(n, k, result);
    }

    return value;
}

long double derivativeFirst(long double x, long double result[][12])
{
    long double q = (x - result[0][0]) / 0.4;
    long double value = 1 / 0.4 * (result[0][3] + (2 * q - 1) / 2 * result[0][4] + (3 * powf(q, 2) - 6 * q + 2) / 6 * result[0][5] + (4 * powf(q, 3) - 18 * powf(q, 2) - 22 * q - 6) / 24 * result[0][6] * (5 * powf(q, 4) - 40 * powf(q, 3) + 105 * powf(q, 2) - 100 * q + 24)/ 120 * result[0][7]);

    return value;
}

long double derivativeSecond(long double x, long double result[][12])
{
    long double q = (x - result[0][0]) / 0.4;
    long double value = 1 / powf(0.4, 2) * (result[0][4] + (q - 1) * result[0][5] + (6 * powf(q, 2) - 18 * q + 11) / 12 * result[0][6] + (powf(q, 3) - 6 * powf(q, 2) + 10.5 * q - 5) / 6 * result[0][7]);

    return value;
}

long double interpolationResult(long double x, long double h, long double result[][12], int n)
{
    long double value = 0.0;

    if (x < 5.0)
    {
        value = interpolateFirst(n, x, result);
    }
    else
    {
        value = interpolateSecond(n, x, result);
    }

    return value;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    long double result[11][12];
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            result[i][j] = 3990355.0;
        }
    }

    int i = 0;
    for (long double x = START; x < END + START / 10; x += 0.4)
    {
        result[i][0] = x;
        result[i][1] = atanl(1.0 / 2.0 * x);
        i++;
    }

    int j = 0;
    int m = 10;
    int n = 11;
    for (j = 2; j < 12; j++)
    {
        for (i = 0; i < m; i++)
        {
            result[i][j] = result[i + 1][j - 1] - result[i][j - 1];
        }
        m--;
    }

    printf("   x       y         dy(x)     d^2y(x)   d^3y(x)   d^4y(x)   d^5y(x)   d^6y(x)   d^7y(x)   d^8y(x)   d^9(x)   d^10y(x)\n");
    for (i = 0; i < 11; i++)
    {
        for (j = 0; j < 12; j++)
        {
            if (result[i][j] == 3990355.0)
            {
                printf("     -    ");
            }
            else
            {
                printf("%9.6lf ", result[i][j]);
            }
        }
        cout << "\n";
    }

    long double Xs[4];
    long double x0 = result[0][0];
    long double x10 = result[10][0];
    long double h = 0.4;
    Xs[0] = x0;
    Xs[1] = x0 + 0.5 * h;
    Xs[2] = x10 - 0.5 * h;
    Xs[3] = x10;

    printf("\n\n   x       f(x)     f`(x)     f`(x)н     Погрешность`   f``(x)     f``(x)н     Погрешность``\n");
    for (i = 0; i < 4; i++)
    {
        printf("%9.6lf %9.6lf %9.6lf %9.6lf %9.6lf      %9.6lf  %9.6lf   %9.6lf\n", Xs[i], atan(1.0 / 2.0 * Xs[i]), interpolationResult(Xs[i], 0.4, result, n), derivativeFirst(Xs[i], result), fabs(interpolationResult(Xs[i], 0.4, result, n) - derivativeFirst(Xs[i], result)), interpolationResult(Xs[i], 0.4, result, n), derivativeSecond(Xs[i], result), fabs(interpolationResult(Xs[i], 0.4, result, n) - derivativeSecond(Xs[i], result)));
    }
}
