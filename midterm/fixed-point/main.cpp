/*
 * Write a fixed-point number class that always maintains 2 digits to the right
 * of the decimal point.
 
 * The suggested internal representation is simply as an long integer.  Then:
 *     1234 means 12.34
 *       12 means   .12
 *       20 means   .20
 *      122 means  1.22
 * etc.
 *
 * You can output integers with leading 0's with:
 *
 *     #include <iostream>
 *     #include <iomanip>
 *     std::cout << std::setw(4) << std::setfill('0') << 30 << std::endl;
 *
 * Note that width (set by std::setw()) will be reset with every output
 * operation, so you will need to reset it.
 */        

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <assert.h>

// You can modifiy the class internally as needed, except that you can't
// implement any assignment operators.
class Fixed
{
    /*template <template T>
            operator T() const {}*/

    public:
    // ZACH START

        Fixed(int i)
        {
            // i = 2
            digits = std::floor(i);
            offset = 0;
        }

        Fixed(double d)
        {
            /*
             * The suggested internal representation is simply as an long integer.  Then:
             *     1234 means 12.34
             *       12 means   .12
             *       20 means   .20
             *      122 means  1.22
             */

            // d = 2.55
            digits = std::floor(d);
            offset = std::round((d - digits)*100);
        }

        Fixed(long d, long o)
        {
            digits = d;
            offset = o;
        }

        Fixed(const Fixed &old)
        {
            digits = old.digits;
            offset = old.offset;
        }

        long total()
        {
            return 0;
            //return digits + offset*100;
        }

        long getTop()
        {
            return digits;
        }

        long getDec()
        {
            return offset;
        }

        operator int()
        {
            return digits;
        }

        operator double()
        {
            return (getTop() * 100 + getDec()) / 100.;
        }

    // ZACH END
    private:
        //Fixed &operator=(int);  // Must not implement.
        //Fixed &operator=(double);  // Must not implement.
    private:
        // Suggested implementation representation, but not required.
        long digits;
        long offset;
};


std::ostream &operator<<(std::ostream &os, Fixed m)
{
    return os << m.getTop() << "." << std::setw(2) << std::setfill('0') << (m.getDec());
}

Fixed operator+(Fixed input, double new_d)
{
    Fixed tmp(new_d);

    long new_digits = input.getTop() + tmp.getTop();
    long new_offset = input.getDec() + tmp.getDec();

    if (new_offset > 100)
    {
        new_digits++;
        new_offset -= 100;
    }

    Fixed t(new_digits, new_offset);
    return t;
}

Fixed operator+(double new_d, Fixed input)
{
    return input + new_d;
}

Fixed operator+(Fixed input, int new_i)
{
    Fixed t(input.getTop()+new_i, input.getDec());
    return t;
}

Fixed operator+(int new_i, Fixed input)
{
    return input + new_i;
}

Fixed operator+(Fixed input1, Fixed input2)
{
    long tmp_top = input1.getTop() + input2.getTop();
    long tmp_btm = input1.getDec() + input2.getDec();

    if (tmp_btm > 100)
    {
        tmp_top++;
        tmp_btm -= 100;
    }

    Fixed t(tmp_top, tmp_btm);
    return t;
}

Fixed operator/(Fixed input1, int i)
{
    double save = (input1.getTop() * 100 + input1.getDec()) / 100.;
    Fixed t(save/i);
    return t;
}

Fixed operator*(Fixed input1, Fixed input2)
{
    double n1 = (input1.getTop()*100+input1.getDec())/100.;
    double n2 = (input2.getTop()*100+input2.getDec())/100.;
    double n3 = n1*n2;

    Fixed t(n3);
    return t;
}

#define CHECK(f, out) check(f, out, __LINE__)

void
check(const Fixed &f, const std::string &out, const int ln) {
    std::stringstream ss;
    ss << f;
    if (ss.str() != out) {
        std::cerr << "At line " << ln << ": ";
        std::cerr << "Should output " << out << ", but instead output " << ss.str() << "." << std::endl;
    }
}

int
main()
{
    Fixed f1(2); // Becomes 2.00.
    f1 = 1.27;

    // Prints out 1.27.
    std::cout << f1 << std::endl;
    CHECK(f1, "1.27");

    // Prints out 3.27.
    std::cout << f1 + 2 << std::endl;
    CHECK(f1 + 2, "3.27");

    // Prints out 3.28.
    std::cout << f1 + 2.01 << std::endl;
    CHECK(f1 + 2.01, "3.28");

    // Prints out 3.28.
    std::cout << f1 + 2.011 << std::endl;
    CHECK(f1 + 2.011, "3.28");

    // Prints out 3.28.
    std::cout << 2.011 + f1 << std::endl;
    CHECK(2.011 + f1, "3.28");

    auto f2 = f1;

    // Prints out 2.54.
    std::cout << f1 + f2 << std::endl;
    CHECK(f1 + f2, "2.54");

    // Prints out 1.61.
    std::cout << f1 * f2 << std::endl;
    CHECK(f1 * f2, "1.61");

    // Convert from int.
    f1 = 3;

    // Prints out 3.00.
    std::cout << f1 << std::endl;
    CHECK(f1, "3.00");
    f1 = 2.45;

    // Convert to int, just truncate.
    // Prints 2.
    std::cout << int(f1) << std::endl;
    assert(int(f1) == 2);

    // Convert to double.
    double x = f1;
    // Prints 2.45.
    std::cout << x << std::endl;
    assert(fabs(x - 2.45) < .00001);

    Fixed f3{1.223};

    // Prints out 1.22.
    std::cout << f3 << std::endl;
    CHECK(f3, "1.22");

    // Division.
    Fixed f4(2);
    f4 = 2.01;

    // Prints out 1.00.
    std::cout << f4/2 << std::endl;
    CHECK(f4/2, "1.00");
}