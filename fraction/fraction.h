#include <iostream>
#include <cmath>

namespace xod
{
    class fraction
    {
        typedef fraction fraction_t;

    private:
        int _numerator;
        int _denominator;

    private:
        void normalize();

    public:
        fraction() : _numerator(0), _denominator(1) {}

        fraction(int value) : _numerator(value), _denominator(1) {}

        fraction(int numerator, int denominator)
        {
            _numerator = numerator;
            _denominator = denominator;
            normalize();
        }

        fraction(double value)
        {
            operator=(value);
        }

    public:
        void set(int numerator, int denominator)
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
        fraction_t operator+=(const int &value)
        {
            _numerator += value * _denominator;
            normalize();
            return *this;
        }

        fraction_t operator-=(const int &value)
        {
            _numerator -= value * _denominator;
            normalize();
            return *this;
        }

        fraction_t operator*=(const int &value)
        {
            _numerator *= value;
            normalize();
            return *this;
        }

        fraction_t operator/=(const int &value)
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
    bool operator op(const int &value) const { return ((fraction) * this).value() op value; }

        BOOL_FUNC(>)
        BOOL_FUNC(>=)
        BOOL_FUNC(<)
        BOOL_FUNC(<=)

#undef BOOL_FUNC

        // inline bool operator==(const int &value) const
        // {
        //     return _denominator == 1 && _numerator == value;
        // }

        inline bool operator==(const double &value) const
        {
            return (*this).value() == value;
        }

        // inline bool operator!=(const int &value) const
        // {
        //     return _denominator == 1 && _numerator != value;
        // }

        inline bool operator!=(const double &value) const
        {
            return (*this).value() != value;
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
    };
} // namespace xod

static std::ostream &operator<<(std::ostream &dest, const xod::fraction &frac)
{
    dest << frac.numerator();

    if (frac.denominator() != 1)
        dest << "/" << frac.denominator();

    return dest;
}