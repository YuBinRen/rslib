---
layout: default
---

# rslib project page #

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


### Algorithm of polynomial long division \\( a(x) \div b(x) \\): ###

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

$$ \begin{array}{} \;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;2x - 3 \newline
\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\overline{2x^2+3x+2}\div x+3 \newline
-\underline{(2x^2+6x)} \newline
\;\;\;\;\;\;\;\;\;\;\;\;\;\;\,-3x + 2 \newline
\;\;\;\;\;\;\;\;\;\;-\underline{(-3x - 9)} \newline
\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;\;11\end{array}$$

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

## Sources ##

* [Polynomial long division](http://en.wikipedia.org/wiki/Polynomial_long_division)
* [Horner's method](http://en.wikipedia.org/wiki/Horner%27s_method)
* Władysław Mochnacki, Kody korekcyjne i kryptografia
* [Finite field arithmetic](http://en.wikipedia.org/wiki/Finite_field_arithmetic)
