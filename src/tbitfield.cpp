#include "tbitfield.h"
#include <string>
#include <cstring>

static const int cBITS = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::invalid_argument("Negative length");
    BitLen = len;
    MemLen = (BitLen + cBITS - 1) / cBITS;
    if (MemLen == 0) MemLen = 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen == 0) MemLen = 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const
{
    return n / cBITS;
}

TELEM TBitField::GetMemMask(const int n) const
{
    TELEM mask = 1u;
    mask <<= (n % cBITS);
    return mask;
}

int TBitField::GetLength(void) const { return BitLen; }

void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit position out of range");
    int index = GetMemIndex(n);
    pMem[index] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit position out of range");
    int index = GetMemIndex(n);
    pMem[index] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit position out of range");
    int index = GetMemIndex(n);
    return (pMem[index] & GetMemMask(n)) ? 1 : 0;
}

TBitField& TBitField::operator=(const TBitField &bf)
{
    if (this == &bf) return *this;
    if (MemLen != bf.MemLen) {
        delete [] pMem;
        MemLen = bf.MemLen;
        if (MemLen == 0) MemLen = 1;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; ++i) pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; ++i) if (pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const { return !(*this == bf); }

TBitField TBitField::operator|(const TBitField &bf)
{
    int resultLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(resultLen);
    int resultMemLen = result.MemLen;
    for (int i = 0; i < resultMemLen; ++i) {
        TELEM a = (i < MemLen) ? pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        result.pMem[i] = a | b;
    }
    return result;
}

TBitField TBitField::operator&(const TBitField &bf)
{
    int resultLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(resultLen);
    int resultMemLen = result.MemLen;
    for (int i = 0; i < resultMemLen; ++i) {
        TELEM a = (i < MemLen) ? pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        result.pMem[i] = a & b;
    }
    return result;
}

TBitField TBitField::operator~(void)
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; ++i) result.pMem[i] = ~pMem[i];
    int extraBits = (MemLen * cBITS) - BitLen;
    if (extraBits > 0) {
        TELEM mask = static_cast<TELEM>(~static_cast<TELEM>(0));
        mask >>= extraBits;
        result.pMem[MemLen - 1] &= mask;
    }
    return result;
}

std::istream &operator>>(std::istream &istr, TBitField &bf)
{
    std::string input;
    if (!(istr >> input)) return istr;
    for (char c : input) {
        if (c != '0' && c != '1') { istr.setstate(std::ios::failbit); return istr; }
    }
    for (int i = 0; i < bf.GetLength(); ++i) bf.ClrBit(i);
    int strLen = static_cast<int>(input.length());
    int bitLen = bf.GetLength();
    for (int i = 0; i < strLen && i < bitLen; ++i) {
        if (input[i] == '1') bf.SetBit(bitLen - 1 - i);
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf)
{
    for (int i = bf.GetLength() - 1; i >= 0; --i) ostr << (bf.GetBit(i) ? '1' : '0');
    return ostr;
}
