// Copyright 2013 sycy600

#include <rslib/extendedfield.h>
#include <algorithm>
#include <cmath>
#include <limits>

namespace rslib {

ExtendedField::ExtendedField(const Polynomial<SimpleFieldElement>& generator)
  : generator_(generator),
    characteristic_(generator.getValue(0).getField().getCharacteristic()),
    extensionOrder_(generator.degree()),
    size_(std::pow(characteristic_, extensionOrder_)),
    simpleField_(generator.getValue(0).getField()) {
  createPolynomialRepresentation();
  createAdditionTable();
}

void ExtendedField::createPolynomialRepresentation() {
  // Create p(x) = 0 and it will be representation for 0 element.
  Polynomial<SimpleFieldElement> zeroPolynomial({SimpleFieldElement(
                                                 0,
                                                 simpleField_)});
  polynomialRepresentation_.push_back(zeroPolynomial);

  // Create r(x) = 1
  Polynomial<SimpleFieldElement> polynomialToBeDivided({SimpleFieldElement(
                                                        1,
                                                        simpleField_)});
  // Create w(x) = x
  const Polynomial<SimpleFieldElement> xPolynomial({SimpleFieldElement(
                                                    0,
                                                    simpleField_),
                                                    SimpleFieldElement(
                                                    1,
                                                    simpleField_)});
  // 1 = r(x) % generator
  // A = r(x) * w(x) % generator = x % generator
  // A^2 = r(x) * w(x) * w(x) % generator = x^2 % generator
  // ...
  for (unsigned int i = 1; i < size_; ++i) {
    polynomialRepresentation_.push_back(polynomialToBeDivided % generator_);
    polynomialToBeDivided *= xPolynomial;
  }
}

void ExtendedField::createAdditionTable() {
  // This algorithm is described in "A Method for
  // Computing Addition Tables in GF(p^n)" by Kyoki Imamura.

  // Create polynomials N which coefficients are placed in integer set
  // basing on coefficients in GF(p).
  std::vector<unsigned int> NX;
  std::vector<Polynomial<unsigned int>> elementsInIntegerSet;
  for (const Polynomial<SimpleFieldElement>& elementInIntegerSet
      : polynomialRepresentation_) {
    std::vector<unsigned int> coefficients;
    for (const SimpleFieldElement& coefficient
        : elementInIntegerSet) {
      coefficients.push_back(coefficient.getValue());
    }
    elementsInIntegerSet.push_back(
          Polynomial<unsigned int>(coefficients));
  }

  // For each polynomial in integer set compute N(x)
  for (const Polynomial<unsigned int>& p : elementsInIntegerSet) {
    NX.push_back(p.evaluate(characteristic_));
  }

  // For each polynomial i in integer set compute N(Z(x)) so that
  // N(Z(x)) = N(x) - p + 1 if N(x) - p + 1 mod p = 0
  // else N(Z(x)) = N(x) + 1
  std::vector<unsigned int> NZX;
  for (unsigned int i = 0; i < NX.size(); ++i) {
    unsigned int checkCongruent = NX[i] + 1u - characteristic_;
    if (checkCongruent % characteristic_) {
      NZX.push_back(NX[i] + 1u);
    } else {
      NZX.push_back(checkCongruent);
    }
  }

  // If there are known values N(Z(x)) and N(x) then find
  // where N(Z(x)) = N(x)
  for (unsigned int i = 0; i < size_; ++i) {
    unsigned int found = std::find(NX.begin(),
                                   NX.end(),
                                   NZX[i]) - NX.begin() - 1;
    zechLogarithms_.push_back(ZechLogarithm(found));
  }
}

ExtendedFieldElement ExtendedField::add(
    const ExtendedFieldElement& first,
    const ExtendedFieldElement& second) const {
  if (first.getValue() == 0) {
    return second;
  } else if (second.getValue() == 0) {
    return first;
  } else if (additiveInverse(first) == second) {
    return ExtendedFieldElement(0, *this);
  } else if (first.getValue() > second.getValue()) {
    unsigned int result = (
        second.getValue() +
        zechLogarithms_[first.getValue() - second.getValue() + 1u].getValue()
        - 1u) % (size_ - 1u) + 1u;
    return ExtendedFieldElement(result, *this);
  } else {
    unsigned int result = (
        first.getValue() +
        zechLogarithms_[second.getValue() - first.getValue() + 1u].getValue()
        - 1u) % (size_ - 1u) + 1u;
    return ExtendedFieldElement(result, *this);
  }
}

ExtendedFieldElement ExtendedField::additiveInverse(
    const ExtendedFieldElement& element) const {
  if (element.getValue() == 0) {
    return element;
  } else if (characteristic_ == 2) {
    return element;
  } else {
    unsigned int result =
        (element.getValue() + (size_ - 1u) / 2u - 1u) % (size_ - 1u) + 1u;
    return ExtendedFieldElement(result, *this);
  }
}

ExtendedFieldElement ExtendedField::multiply(
    const ExtendedFieldElement& first,
    const ExtendedFieldElement& second) const {
  if (first.getValue() == 0) {
    return first;
  } else if (second.getValue() == 0) {
    return second;
  } else {
    unsigned int result =
        1 + (first.getValue() + second.getValue() - 2u) % (size_ - 1u);
    return ExtendedFieldElement(result, *this);
  }
}

ExtendedFieldElement ExtendedField::multiplicativeInverse(
    const ExtendedFieldElement& element) const {
  if (element.getValue() == 1) {
    return element;
  } else {
    unsigned int result = size_ + 1 - element.getValue();
    return ExtendedFieldElement(result, *this);
  }
}

}  // namespace rslib