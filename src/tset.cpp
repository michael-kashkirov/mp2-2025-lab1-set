#include "tset.h"
#include <algorithm>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField() const
{
    return BitField;
}

int TSet::GetMaxPower(void) const { return MaxPower; }

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower) throw std::out_of_range("Element is out of range");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower) throw std::out_of_range("Element is out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower) throw std::out_of_range("Element is out of range");
    BitField.ClrBit(Elem);
}

TSet& TSet::operator=(const TSet& s)
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const
{
    return MaxPower == s.MaxPower && BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const { return !(*this == s); }

TSet TSet::operator+(const TSet& s)
{
    TSet result(std::max(MaxPower, s.MaxPower));
    result.BitField = BitField | s.BitField;
    return result;
}

TSet TSet::operator+(const int Elem)
{
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem)
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet& s)
{
    TSet result(std::max(MaxPower, s.MaxPower));
    result.BitField = BitField & s.BitField;
    return result;
}

TSet TSet::operator~(void)
{
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

std::istream &operator>>(std::istream &istr, TSet &s)
{
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s)
{
    ostr << "{";
    bool first = true;
    for (int i = 0; i < s.MaxPower; ++i) {
        if (s.BitField.GetBit(i)) {
            if (!first) ostr << ", ";
            ostr << i;
            first = false;
        }
    }
    ostr << "}";
    return ostr;
}
