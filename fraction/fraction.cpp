#include "fraction.h"

void xod::fraction::normalize()
{
    if (_denominator == 0)
    {
        _denominator = 1;
        return;
    }

    // use euclidean algorithm, gcd(a, b) to get the common divisor, b
    int a = _numerator > _denominator ? _numerator : _denominator; // biggest
    int b = _numerator + _denominator - a;                         // smallest, min

    for (;;)
    {
        int tmp = a % b;

        if (tmp == 0)
        {
            break;
        }

        a = b;
        b = tmp;
    }

    _numerator /= b;
    _denominator /= b;
}

// does decimal to fraction conversion using richard's fast continued
// fraction iteration algorithm
xod::fraction &xod::fraction::operator=(long double value)
{
    if (value == 0)
    {
        set(0, 1);
        return *this;
    }

    // using Farey process fast method
    const double precision = 0.00001;
    const int max_iter = 25;

    int a = 0, b = 1;
    int c = 1, d = 0;
    double y = value;
    int a_s, b_s;

    for (int i = 0; i <= max_iter; ++i)
    {
        const int s = std::abs((int)y);
        a_s = a + s * c;
        b_s = b + s * d;

        const double x_y = a_s / (double)b_s;
        const double diff = x_y - value;

        if (std::abs(diff) >= 0 && std::abs(diff) <= precision)
        {
            // std::cout << "iter = " << i << std::endl;
            break;
        }

        a = c;
        b = d;
        c = a_s;
        d = b_s;

        y = 1 / (double)(y - s);
    }

    set(a_s, b_s);

    return *this;
}



// does decimal to fraction conversion using Farey's slow continued
// fraction algorithm
/*
xod::fraction &xod::fraction::operator=(long double value)
{
    if (value == 0)
    {
        set(0, 1);
        return *this;
    }

    // using Farey process slow method
    double precision = 0.00001;

    int s = (int)value;
    double α = value - s;

    int a = 0, b = 1;
    int c = 1, d = 1;
    int mid_n, mid_d;
    int max_iter = 25;

    for (int i = 0; i <= max_iter; ++i)
    {
        mid_n = a + c;
        mid_d = b + d;

        double mid = mid_n / (double)mid_d;
        double diff = mid - α;

        // std::cout << "run" << std::endl;

        if (std::abs(diff) >= 0 && std::abs(diff) <= precision)
        {
            // std::cout << "iter = " << i << std::endl;
            break;
        }
        else if (mid < α)
        {
            a = mid_n;
            b = mid_d;
        }
        else
        {
            c = mid_n;
            d = mid_d;
        }
    }

    mid_n += (mid_d * s); // convert to proper fraction

    set(mid_n, mid_d);

    return *this;
} */
