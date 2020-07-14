#include <iostream>
#include <cmath>

namespace xod
{
    template <typename int_t = int>
    class fraction
    {
        typedef fraction<int_t> fraction_t;

    private:
        int _numerator;
        int _denominator;

    private:
        void normalize();

    public:
        fraction() : _numerator(0), _denominator(1) {}

        fraction(int_t value) : _numerator(value), _denominator(1) {}

        fraction(int_t numerator, int_t denominator)
        {
            _numerator = numerator;
            _denominator = denominator;
            normalize();
        }

        template <typename float_t>
        explicit fraction(float_t value)
        {
            operator=(value);
        }

    public:
        void set(int_t numerator, int_t denominator)
        {
            _numerator = numerator;
            _denominator = denominator;
            normalize();
        }

        inline double value() const
        {
            return _numerator / (double)_denominator;
        }

        const int &numerator() const
        {
            return _numerator;
        }

        const int &denominator() const
        {
            return _denominator;
        }

        inline bool is_negative() const
        {
            return (_numerator < 0) ^ (_denominator < 0);
        }

    public:
        fraction_t operator+=(const int_t &value)
        {
            _numerator += value * _denominator;
            normalize();
            return *this;
        }

        fraction_t operator-=(const int_t &value)
        {
            _numerator -= value * _denominator;
            normalize();
            return *this;
        }

        fraction_t operator*=(const int_t &value)
        {
            _numerator *= value;
            normalize();
            return *this;
        }

        fraction_t operator/=(const int_t &value)
        {
            _denominator *= value;
            normalize();
            return *this;
        }

    public:
        fraction_t operator+=(const fraction_t &value)
        {
            _numerator = _numerator * value.denominator() + _denominator * value.numerator();
            _denominator = _denominator * value.denominator();
            normalize();
            return *this;
        }

        fraction_t operator-=(const fraction_t &value)
        {
            _numerator = _numerator * value.denominator() - _denominator * value.numerator();
            _denominator = _denominator * value.denominator();
            normalize();
            return *this;
        }

        fraction_t operator*=(const fraction_t &value)
        {
            _numerator *= value.numerator();
            _denominator *= value.denominator();
            normalize();
            return *this;
        }

        fraction_t operator/=(const fraction_t &value)
        {
            _numerator *= value.denominator();
            _denominator *= value.numerator();
            normalize();
            return *this;
        }

    public:
#define OP_FUNC(op, op_to_exec)                    \
    fraction_t operator op(const fraction_t &frac) \
    {                                              \
        fraction_t tmp(*this);                     \
        tmp op_to_exec frac;                       \
        return tmp;                                \
    }

        OP_FUNC(+, +=)
        OP_FUNC(-, -=)
        OP_FUNC(*, *=)
        OP_FUNC(/, /=)
#undef OP_FUNC

        //comparision
#define BOOL_FUNC(op)                                                                                      \
    bool operator op(const fraction_t &frac) const { return ((fraction) * this).value() op frac.value(); } \
    bool operator op(const int_t &value) const { return ((fraction) * this).value() op value; }

        BOOL_FUNC(>)
        BOOL_FUNC(>=)
        BOOL_FUNC(<)
        BOOL_FUNC(<=)

#undef BOOL_FUNC

        inline bool operator==(const int_t &value) const
        {
            return _denominator == 1 && _numerator == value;
        }

        inline bool operator!=(const int_t &value) const
        {
            return _denominator != 1 && _numerator != value;
        }

        inline bool operator==(const fraction_t &value) const
        {
            return _denominator == value.denominator() && _numerator == value.numerator();
        }

        inline bool operator!=(const fraction_t &value) const
        {
            return _denominator != value.denominator() && _numerator != value.numerator();
        }

        operator bool const()
        {
            return ((fraction) * this).value() != 0;
        }

        fraction_t &operator=(double value)
        {
            return *this = (long double)value;
        }

        fraction_t &operator=(long double value);
    }; // namespace xod
} // namespace xod

template <typename int_t>
void xod::fraction<int_t>::normalize()
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

// does decimal to fraction conversion using Farey's slow continued
// fraction algorithm
/* template <typename int_t>
xod::fraction<int_t> &xod::fraction<int_t>::operator=(long double value)
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

// does decimal to fraction conversion using richard's fast continued
// fraction iteration algorithm
template <typename int_t>
xod::fraction<int_t> &xod::fraction<int_t>::operator=(long double value)
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

template <typename int_t>
static std::ostream &operator<<(std::ostream &dest, const xod::fraction<int_t> &frac)
{
    dest << frac.numerator() << "/" << frac.denominator();
    return dest;
}