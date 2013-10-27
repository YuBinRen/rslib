---
layout: default
---

<h1>rslib project page</h1>

Table of contents:

* TOC.
{:toc}

## Overview ##

This is a C++ library which implements polynomials, simple and extended Galois Fields and
Reed-Solomon encoder and decoder.

Did you find any mistake or bug? Please share it on [GitHub issues](https://github.com/sycy600/rslib/issues)
or do a pull request with fix.

This is a documentation for version 0.1.0.

## How to build ##

Clone repository:

    git clone https://github.com/sycy600/rslib.git

Create build directory:

    mkdir build
    cd build

Run **cmake**:

    cmake ../rslib

Build project:

    make

Install project - that will install static library and headers on your system:

    sudo make install

Include headers in your program and link against **librslib**. To compile the project
use C++11 standard.

## Polynomials ##

### Addition ###

If there are two polynomials:

$$ a(x) = a_mx^m + a_{m-1}x^{m-1} + \ldots + a_1x + a_0 $$

$$ b(x) = b_mx^m + b_{m-1}x^{m-1} + \ldots + b_1x + b_0 $$

then sum of these polynomials is:

$$ a(x) + b(x) = (a_m + b_m)x^m + (a_{m-1} + b_{m-1})x^{m-1} + \ldots + (a_1 + b_1)x + a_0 + b_0 $$

Example:

$$ a(x) = 2x^2 + 3x + 4 $$

$$ b(x) = 4x + 1 $$

$$ a(x) + b(x) = 2x^2 + (3 + 4)x + 4 + 1 = 2x^2 + 7x + 5 $$

Using **rslib**:

{% highlight cpp %}
#include <rslib/polynomial.h>
         
rslib::Polynomial<int> a({4, 3, 2});
rslib::Polynomial<int> b({1, 4});
rslib::Polynomial<int> result = a + b;
std::cout << result; // [5,7,2,]
{% endhighlight %}

### Subtraction ###

If there are two polynomials:

$$ a(x) = a_mx^m + a_{m-1}x^{m-1} + \ldots + a_1x + a_0 $$

$$ b(x) = b_mx^m + b_{m-1}x^{m-1} + \ldots + b_1x + b_0 $$

then difference of these polynomials is:

$$ a(x) - b(x) = (a_m - b_m)x^m + (a_{m-1} - b_{m-1})x^{m-1} + \ldots + (a_1 - b_1)x + a_0 - b_0 $$

Example:

$$ a(x) = 3x + 2 $$

$$ b(x) = 5x^2 + 4x + 1 $$

$$ a(x) - b(x) = (0 - 5)x^2 + (3 - 4)x + 2 - 1 = -5x^2 - x + 1 $$

Using **rslib**:

{% highlight cpp %}
#include <rslib/polynomial.h>
         
rslib::Polynomial<int> a({2, 3});
rslib::Polynomial<int> b({1, 4, 5});
rslib::Polynomial<int> result = a - b;
std::cout << result; // [1,-1,-5,]
{% endhighlight %}

### Multiplication ###

If there are two polynomials:

$$ a(x) = a_mx^m + a_{m-1}x^{m-1} + \ldots + a_1x + a_0 $$

$$ b(x) = b_nx^n + b_{n-1}x^{n-1} + \ldots + b_1x + b_0 $$

then product of these polynomials is:

$$ \begin{array}{rcl} a(x) \times b(x) & = & (a_m \times b_n)x^{m+n} + (a_m \times b_{n-1} + a_{m-1} \times b_n)x^{m+n-1} + \ldots + (a_1 \times b_0 + a_0 \times b_1)x + a_0 \times b_0 \\\\
& = & (\sum\limits_{i+j=m+n} a_i \times b_j )x^{m+n} + (\sum\limits_{i+j=m+n-1} a_i \times b_j)x^{m+n-1} + \ldots + (\sum\limits_{i+j=1} a_i \times b_j)x + (\sum\limits_{i+j=0} a_i \times b_j) \end{array} $$

Example:

$$ a(x) = 3x + 2 $$

$$ b(x) = 5x^2 + 4x + 1 $$

$$ a(x) \times b(x) = (5 \times 3)x^3 + (3 \times 4 + 5 \times 2)x^2 + (3 \times 1 + 4 \times 2)x + 2 \times 1 = 15x^3 + 22x^2 + 11x + 2 $$

Using **rslib**:

{% highlight cpp %}
#include <rslib/polynomial.h>
         
rslib::Polynomial<int> a({2, 3});
rslib::Polynomial<int> b({1, 4, 5});
rslib::Polynomial<int> result = a * b;
std::cout << result; // [2,11,22,15,]
{% endhighlight %}

### Division and modulo operation ###

If there are two polynomials:

$$ a(x) = a_mx^m + a_{m-1}x^{m-1} + \ldots + a_1x + a_0 $$

$$ b(x) = b_nx^n + b_{n-1}x^{n-1} + \ldots + b_1x + b_0 $$

then there has to be found such polynomial \\( q(x) \\) so that

$$ a(x) \div b(x) = q(x) \times b(x) + r(x) $$

where \\( r(x) \\) is a rest of division


#### Algorithm of polynomial long division \\( a(x) \div b(x) \\): ####

<div class="algorithm">
<div class="algorithm_first_indent">
<p>Initialise:</p>
<p>\( q(x) := 0 \) and \( r(x) := a(x) \)</p>
</div>
<p>While \( r(x) \neq 0 \) and \( \deg(r(x)) \ge \deg(b(x)) \):</p>
<div class="algorithm_second_indent">
<p>Divide leading coefficient of \(r(x)\) by leading coefficient of \(b(x)\)</p>
<p>\( result := lead(r(x)) \div lead(b(x)) \)</p>
<p>\( q_{\deg(r(x)) - \deg(b(x))}(x) := result \)</p>
<p>Calculate a new rest</p>
<p>\( r(x) := r(x) - result \times x^{\deg(r(x)) - \deg(b(x))} \times b(x) \)</p>
</div>
</div>

Example:

$$ a(x) = 2x^2+3x+2 $$

$$ b(x) = x+3 $$

$$ \begin{array}{l}
\hspace{17 mm} 2x - 3 \newline
\hspace{5 mm} \overline{2x^2+3x+2}\div x+3 \newline
-\underline{(2x^2+6x)} \newline
\hspace{12 mm} -3x + 2 \newline
\hspace{6 mm} -\underline{(-3x - 9)} \newline
\hspace{25 mm} 11\end{array}$$

$$ q(x) = 2x - 3 $$

$$ r(x) = 11 $$

$$ (2x^2 + 3x + 2) \div (x + 3) = (2x - 3)(x + 3) + 11 $$

Using **rslib**:

{% highlight cpp %}
#include<rslib/polynomial.h>
         
rslib::Polynomial<int> a({2, 3, 2});
rslib::Polynomial<int> b({3, 1});
rslib::Polynomial<int> quotient = a / b;
std::cout << quotient; // [-3,2,]
rslib::Polynomial<int> rest = a % b;
std::cout << rest; // [11,]
{% endhighlight %}

### Evaluation ###

To evaluate a polynomial Horner's method is used. If there is given a polynomial:

$$ a(x) = a_mx^m + a_{m-1}x^{m-1} + \ldots + a_1x + a_0 $$

then it can be rewritten as:

$$ a(x) = x(x(x \ldots (a_{m}x + a_{m-1}) + a_{m-2}) \ldots ) + a_1) + a_0 $$

Horner's method allows to evaluate polynomial with less number of multiplications than
in a regular way.

Example:

$$ a(x) = 2x^2+3x+2 = x(2x + 3) + 2 $$

$$ a(2) = 2(2 \times 2 + 3) + 2 = 16 $$

Using **rslib**:

{% highlight cpp %}
#include <rslib/polynomial.h>

rslib::Polynomial<int> a({2, 3, 2});
int result = a.evaluate(2);
std::cout << result; // 16
{% endhighlight %}

## Simple finite field ##

A simple finite field consists of \\(p\\) elements where \\(p\\) is a prime number. This 
element \\(p\\) is called characteristic. Simple field is denoted as
\\(GF(p)\\) (Galois Field) and elements of such field are \\(0, 1, 2, \ldots, p-1\\).

Example:

\\(GF(5)\\) consists of elements \\(0, 1, 2, 3, 4\\).

Using **rslib**:

{% highlight cpp %}
#include <rslib/simplefield.h>

rslib::SimpleField GF(5);
{% endhighlight %}

### Arithmetic ###

In simple field works addition, subtraction, multiplication and division.

**Addition** of elements \\(a\\) and \\(b\\)

$$ (a + b) \mod p $$

**Subtraction** of elements \\(a\\) and \\(b\\)

$$ (a - b) \mod p = (a + (-b)) \mod p $$

where

$$-b \mod p = (p -b) \mod p$$

**Multiplication** of elements \\(a\\) and \\(b\\)

$$ (a \times b) \mod p $$

**Division** of elements \\(a\\) and \\(b\\)

$$ (a \div b) \mod p = (a \times b^{-1}) \mod p $$

where

$$ b^{-1} = b^{p-2} $$

because Fermat's little theorem says that

$$ b^p \equiv b \mod p $$

and in finite field it is so that

$$ b^p = b $$

so if we divide both sides by \\(b\\)

$$ b^{p-1} = 1 $$

and then again

$$ b^{p-2} = \frac{1}{b} = b^{-1} $$

Example:

In the \\(GF(5)\\):

$$ 3 + 4 = 2 $$

$$ 3 - 4 = 3 + 1 = 4 $$

$$ 3 \times 4 = 2 $$

$$ 3 \div 4 = 3 \times 4 = 2 $$

$$ -4 = 1 $$

$$ 4^{-1} = 4 $$

Using **rslib**:

{% highlight cpp %}
#include <rslib/simplefield.h>
#include <rslib/simplefieldelement.h>

rslib::SimpleField GF(5);
rslib::SimpleFieldElement first(3, GF);
rslib::SimpleFieldElement second(4, GF);
std::cout << first + second; // 2
std::cout << first - second; // 4
std::cout << first * second; // 2
std::cout << first / second; // 2
std::cout << -second; // 1
std::cout << second.multiplicativeInverse(); // 4
{% endhighlight %}

## Extended finite field ##

Extended finite field denoted as \\(GF(q)\\) where \\(q=p^m\\) is the number of elements
in the field, \\(p\\) is a prime number and \\(m\\) is an extension order.

To generate extended finite field there can be provided generator polynomial.
In **rslib** this generator element is a polynomial whose coefficients reside in \\(GF(p)\\).
Degree of this generator polynomial is equal to \\(m\\). This polynomial is used to generate field elements
\\(1, \alpha, \alpha^2, \alpha^3, \ldots, \alpha^{q-2}\\). Element \\(\alpha^{q-1}\\) equals to \\(1\\).
In the \\(GF(q)\\) there is also included element \\(0\\). There is a set of these polynomial generators
so we need to know exactly which polynomials can generate fields.

Elements are generated as follows using generator polynomial \\(p(x)\\):

* \\(\alpha^0 = 1 = 1 \mod p(x)\\)
* \\(\alpha = x \mod p(x)\\)
* \\(\alpha^2 = x^2 \mod p(x)\\)
* \\(\alpha^3 = x^3 \mod p(x)\\)

and so on.

Example:

\\(GF(8)\\) which is \\(GF(2^3)\\) consists of elements \\(0, 1, \alpha, \alpha^2, \ldots, \alpha^6\\).
The generator polynomial for this field is \\(p(x) = x^3 + x + 1\\). You can notice that coefficients of
this polynomial come from field \\(GF(2)\\) - it is \\(0\\) or \\(1\\).

* \\(0 = 0\\)
* \\(\alpha^0 = 1 = 1 \mod x^3 + x + 1\\)
* \\(\alpha = x \mod x^3 + x + 1 = x\\)
* \\(\alpha^2 = x^2 \mod x^3 + x + 1 = x^2\\)
* \\(\alpha^3 = x^3 \mod x^3 + x + 1 = x + 1\\)
* \\(\alpha^4 = x^4 \mod x^3 + x + 1 = x^2 + x\\)
* \\(\alpha^5 = x^5 \mod x^3 + x + 1 = x^2 + x + 1\\)
* \\(\alpha^6 = x^6 \mod x^3 + x + 1 = x^2 + 1\\)

Using **rslib**:

{% highlight cpp %}
#include <rslib/simplefield.h>
#include <rslib/simplefieldelement.h>
#include <rslib/extendedfield.h>
#include <rslib/extendedfieldelement.h>

rslib::SimpleField GF2 = rslib::SimpleField(2);
rslib::Polynomial<rslib::SimpleFieldElement>
      generator = rslib::Polynomial<rslib::SimpleFieldElement>({
                  rslib::SimpleFieldElement(1, GF2),
                  rslib::SimpleFieldElement(1, GF2),
                  rslib::SimpleFieldElement(0, GF2),
                  rslib::SimpleFieldElement(1, GF2)});

rslib::ExtendedField GF8 = rslib::ExtendedField(generator);

std::cout << rslib::ExtendedFieldElement(0, GF8).getPolynomialRepresentation(); // [0,]
std::cout << rslib::ExtendedFieldElement(1, GF8).getPolynomialRepresentation(); // [1,]
std::cout << rslib::ExtendedFieldElement(2, GF8).getPolynomialRepresentation(); // [0,1,]
std::cout << rslib::ExtendedFieldElement(3, GF8).getPolynomialRepresentation(); // [0,0,1,]
std::cout << rslib::ExtendedFieldElement(4, GF8).getPolynomialRepresentation(); // [1,1,]
std::cout << rslib::ExtendedFieldElement(5, GF8).getPolynomialRepresentation(); // [0,1,1,]
std::cout << rslib::ExtendedFieldElement(6, GF8).getPolynomialRepresentation(); // [1,1,1,]
std::cout << rslib::ExtendedFieldElement(7, GF8).getPolynomialRepresentation(); // [1,0,1,]
{% endhighlight %}

### Arithmetic ###

In extended field works addition, subtraction, multiplication and division.

Example:

In the \\(GF(8)\\):

$$ \alpha + \alpha^2 = \alpha^4 $$

$$ \alpha - \alpha^2 = \alpha^4 $$

$$ \alpha \times \alpha^2 = \alpha^3 $$

$$ \alpha \div \alpha^2 = \alpha^6 $$

$$ -\alpha = \alpha $$

$$ \alpha^{-1} = \alpha^6 $$

Using **rslib**:

{% highlight cpp %}
#include <rslib/simplefield.h>
#include <rslib/simplefieldelement.h>
#include <rslib/extendedfield.h>
#include <rslib/extendedfieldelement.h>

rslib::SimpleField GF2 = rslib::SimpleField(2);
rslib::Polynomial<rslib::SimpleFieldElement>
      generator = rslib::Polynomial<rslib::SimpleFieldElement>({
                  rslib::SimpleFieldElement(1, GF2),
                  rslib::SimpleFieldElement(1, GF2),
                  rslib::SimpleFieldElement(0, GF2),
                  rslib::SimpleFieldElement(1, GF2)});

rslib::ExtendedField GF8 = rslib::ExtendedField(generator);
rslib::ExtendedFieldElement first(2, GF8);
rslib::ExtendedFieldElement second(3, GF8);
std::cout << first + second; // A^4
std::cout << first - second; // A^4
std::cout << first * second; // A^3
std::cout << first / second; // A^6
std::cout << -first; // A
std::cout << first.multiplicativeInverse(); // A^6
{% endhighlight %}

## Reed-Solomon code ##

Reed-Solomon code is an error-correction code. It means that you have some data, you encode this
data using Reed-Solomon encoding algorithm producing codeword. Then this data can be for example
somewhere transmitted over some channel and some bytes can be corrupted (for example some bits change
from 0 to 1 or vice versa). Then while decoding received word by Reed-Solomon code decoder these
errors can be repaired to some point. One of the characteristic of Reed-Solomon code is
error correction capability which means how many errors code can correct.

For practical usage it is convenient to use \\(GF(256)\\) to work with Reed-Solomon code.
Each element of \\(GF(256)\\) can be represented as polynomial of degree \\(7\\) and coefficients
of these polynomial come from \\(GF(2)\\) which can be \\(0\\) or \\(1\\). So elements can be encoded
as binary \\(8\\)-elements vector which is a byte.

Codeword of Reed-Solomon code consists of data elements and redundant elements. Data elements are the data
we want to encode. Redundant elements are added by encoder and used while decoding.

Here are some characteristic for any Reed-Solomon code:

* for field \\(GF(q)\\) number of elements in codeword \\(n\\) equals to \\(q-1\\)
* each +1 error correction capability requires 2 redundant elements

Example:

For the field \\(GF(16)\\) we want to have a code which can correct four errors. The length of codeword
equals to \\(n=16-1=15\\). The number of redundant elements equals to \\(r=2*4=8\\). The number
of data elements equals to \\(k=n-r=15-8=7\\). Such code is denoted as \\(RS(15, 7)\\).

### Encoding ###

For encoding purpose there is used code generator polynomial. For error correction capability \\(t\\)
this code generator polynomial is created as follows:

$$ g(x) = (x-\alpha)(x-\alpha^2)\cdots(x-\alpha^{2t}) $$

Then we have some data which are represented by information polynomial \\(m(x)\\). Constructing codeword
\\(c(x)\\) is done as follows:

$$ c(x) = m(x)x^{2t} + (m(x)x^{2t} \mod g(x)) $$

Example:

For code \\(RS(15, 7)\\) the code generator polynomial is as follows:

$$ g(x) = (x-\alpha)(x-\alpha^2)(x-\alpha^3)(x-\alpha^4)(x-\alpha^5)(x-\alpha^6)(x-\alpha^7)(x-\alpha^8)$$

$$ g(x) = x^8 + \alpha^{14}x^7 + \alpha^2x^6 + \alpha^4x^5 + \alpha^2x^4 + \alpha^{13}x^3 + \alpha^5x^2 + \alpha^{11}x + \alpha^6$$

Now let's encode some data. Let the information polynomial be:

$$ m(x) = \alpha^3x^6 + \alpha^{11}x^5 + \alpha^{11}x^4 + \alpha^4x^3 + \alpha^5x^2 + x + \alpha^8 $$

Then after computation:

$$ c(x) = \alpha^3x^{14} + \alpha^{11}x^{13} + \alpha^{11}x^{12} + \alpha^4x^{11} + \alpha^5x^{10} + x^9 + \alpha^8x^8 + \alpha^{13}x^7 + \alpha^5x^6 + \alpha^2x^5 + \alpha^2x^4 + x^3 + {\alpha}x^2 + \alpha^{10}x + \alpha $$

Using **rslib**:

{% highlight cpp %}
#include <rslib/simplefield.h>
#include <rslib/simplefieldelement.h>
#include <rslib/extendedfield.h>
#include <rslib/extendedfieldelement.h>
#include <rslib/encoder.h>

rslib::SimpleField GF2 = rslib::SimpleField(2);

rslib::Polynomial<rslib::SimpleFieldElement>
      generator = rslib::Polynomial<rslib::SimpleFieldElement>({
                  rslib::SimpleFieldElement(1, GF2),
                  rslib::SimpleFieldElement(1, GF2),
                  rslib::SimpleFieldElement(0, GF2),
                  rslib::SimpleFieldElement(0, GF2),
                  rslib::SimpleFieldElement(1, GF2)});

rslib::ExtendedField GF16 = rslib::ExtendedField(generator);

rslib::Encoder encoder(4, GF16);

rslib::Polynomial<rslib::ExtendedFieldElement> information =
      rslib::Polynomial<rslib::ExtendedFieldElement>({
                  rslib::ExtendedFieldElement(9, GF16),
                  rslib::ExtendedFieldElement(1, GF16),
                  rslib::ExtendedFieldElement(6, GF16),
                  rslib::ExtendedFieldElement(5, GF16),
                  rslib::ExtendedFieldElement(12, GF16),
                  rslib::ExtendedFieldElement(12, GF16),
                  rslib::ExtendedFieldElement(4, GF16)});
rslib::Polynomial<rslib::ExtendedFieldElement> codeword
      = encoder.encode(information);

// [A,A^10,A,1,A^2,A^2,A^5,A^13,A^8,1,A^5,A^4,A^11,A^11,A^3,]
std::cout << codeword << std::endl;
{% endhighlight %}

### Decoding ###

Decoding process can repair up to \\(t\\) errors in received word. There are several steps to decode
Reed-Solomon received word:

1. computation of syndromes - syndromes are not zeros then we have corrupted data
2. computation of error-locator polynomial - this polynomial is used to find locations where error occurred (Berlekamp-Massey algorithm)
3. computation of error locators - reciprocals of roots of error-locator polynomial
4. computation of error values for given locations (Forney algorithm)
5. correction of received word by subtracting error values from given locations

Example:

Going back to codeword

$$ c(x) = \alpha^3x^{14} + \alpha^{11}x^{13} + \alpha^{11}x^{12} + \alpha^4x^{11} + \alpha^5x^{10} + x^9 + \alpha^8x^8 + \alpha^{13}x^7 + \alpha^5x^6 + \alpha^2x^5 + \alpha^2x^4 + x^3 + {\alpha}x^2 + \alpha^{10}x + \alpha $$

Let's say that there occurred three errors during transmission and we have following received word (notice coefficients by \\(x^{14}, x^{6}, x\\)):

$$ r(x) = \alpha^{11}x^{14} + \alpha^{11}x^{13} + \alpha^{11}x^{12} + \alpha^4x^{11} + \alpha^5x^{10} + x^9 + \alpha^8x^8 + \alpha^{13}x^7 + \alpha^5x^6 + \alpha^4x^5 + \alpha^2x^4 + x^3 + {\alpha}x^2 + \alpha^{13}x + \alpha $$

The output of decoding is information polynomial:

$$ m(x) = \alpha^3x^6 + \alpha^{11}x^5 + \alpha^{11}x^4 + \alpha^4x^3 + \alpha^5x^2 + x + \alpha^8 $$

Using **rslib**:

{% highlight cpp %}
#include <rslib/simplefield.h>
#include <rslib/simplefieldelement.h>
#include <rslib/extendedfield.h>
#include <rslib/extendedfieldelement.h>
#include <rslib/encoder.h>
#include <rslib/bmadecoder.h>

rslib::SimpleField GF2 = rslib::SimpleField(2);

rslib::Polynomial<rslib::SimpleFieldElement>
      generator = rslib::Polynomial<rslib::SimpleFieldElement>({
                  rslib::SimpleFieldElement(1, GF2),
                  rslib::SimpleFieldElement(1, GF2),
                  rslib::SimpleFieldElement(0, GF2),
                  rslib::SimpleFieldElement(0, GF2),
                  rslib::SimpleFieldElement(1, GF2)});

rslib::ExtendedField GF16 = rslib::ExtendedField(generator);

rslib::Encoder encoder(4, GF16);

rslib::Polynomial<rslib::ExtendedFieldElement> information =
      rslib::Polynomial<rslib::ExtendedFieldElement>({
                  rslib::ExtendedFieldElement(9, GF16),
                  rslib::ExtendedFieldElement(1, GF16),
                  rslib::ExtendedFieldElement(6, GF16),
                  rslib::ExtendedFieldElement(5, GF16),
                  rslib::ExtendedFieldElement(12, GF16),
                  rslib::ExtendedFieldElement(12, GF16),
                  rslib::ExtendedFieldElement(4, GF16)});

rslib::Polynomial<rslib::ExtendedFieldElement> codeword
      = encoder.encode(information);

// [A,A^10,A,1,A^2,A^2,A^5,A^13,A^8,1,A^5,A^4,A^11,A^11,A^3,]
std::cout << codeword << std::endl;

rslib::Polynomial<rslib::ExtendedFieldElement>
                                 receivedWord(codeword);
receivedWord.setValue(1, rslib::ExtendedFieldElement(14, GF16));
receivedWord.setValue(5, rslib::ExtendedFieldElement(5, GF16));
receivedWord.setValue(14, rslib::ExtendedFieldElement(12, GF16));
// [A,A^13,A,1,A^2,A^4,A^5,A^13,A^8,1,A^5,A^4,A^11,A^11,A^11,]
std::cout << receivedWord << std::endl;

rslib::BMADecoder decoder(4, GF16);

// [A^8,1,A^5,A^4,A^11,A^11,A^3,]
std::cout << decoder.decode(receivedWord) << std::endl;
{% endhighlight %}

## Sources ##

* [Polynomial long division](http://en.wikipedia.org/wiki/Polynomial_long_division)
* [Horner's method](http://en.wikipedia.org/wiki/Horner%27s_method)
* Władysław Mochnacki, Kody korekcyjne i kryptografia
* Kody korekcyjne, kody Reeda-Solomon, lecture material
* [Finite field arithmetic](http://en.wikipedia.org/wiki/Finite_field_arithmetic)
* [Forney algorithm](https://en.wikipedia.org/wiki/Forney_algorithm)
* [Reed-Solomon error correction](https://en.wikipedia.org/wiki/Reed%E2%80%93Solomon_error_correction)
