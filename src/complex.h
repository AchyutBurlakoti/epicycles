#include <iostream>

/* As we are applying complex fourier transform on image data so we need Complex object */

class Complex
{
    public:
    double re;
    double im;
    double freq;
    double amp;
    double phase;

    Complex(double a, double b, double fr = 0, double am = 0, double ph = 0)
     : re(a), im(b), freq(fr), amp(am), phase(ph) { }

    Complex()
     : re(0), im(0), freq(0), amp(0), phase(0) { }

    void add(Complex _acomp)
    {
        re += _acomp.re;
        im += _acomp.im;
    }
    Complex mult(Complex _mcomp)
    {
        Complex temp;
        temp.re = re * _mcomp.re - im * _mcomp.im;
        temp.im = re * _mcomp.im + im * _mcomp.re;
        return temp;
    }
    void copy(Complex& c)
    {
        re = c.re;
        im = c.im;
        amp = c.amp;
        freq = c.freq;
        phase = c.phase;
    }
};