// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw "Error: negative length";
    BitLen = len;
    MemLen = (BitLen + 31) / 32; 
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw "Error: index out of range";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw "Error: index out of range";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw "Error: index out of range";
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)
        return 1;
    else
        return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.GetLength())
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return ~(*this == bf); 
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxLen = BitLen;
    if (bf.GetLength() > maxLen)
        maxLen = bf.GetLength(); 
    TBitField temp(maxLen);

    for (int i = 0; i < MemLen; i++)
        temp.pMem[i] = pMem[i];

   int  bfMemLen = (bf.GetLength() + 31) / 32;
    for (int i = 0; i < bfMemLen; i++)
        temp.pMem[i] |= bf.pMem[i];

    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = BitLen;
    if (bf.GetLength() > maxLen)
        maxLen = bf.GetLength();
    TBitField temp(maxLen);
    int bfMemLen = (bf.GetLength() + 31) / 32;
    int minMemLen = MemLen;
    if (bfMemLen < minMemLen)
        minMemLen = bfMemLen;
    for (int i = 0; i < minMemLen; i++)
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    return temp;
}
    
TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < MemLen - 1; i++) {
        temp.pMem[i] = ~pMem[i];
    }
        int lastBits = BitLen % 32;
    TELEM mask = (lastBits == 0) ? ~0 : ((1 << lastBits) - 1);
    if (MemLen > 0) {
        temp.pMem[MemLen - 1] = ~pMem[MemLen - 1] & mask;
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char ch;
    int i = 0;
    while (istr >> ch) {
        if (ch == '0') bf.ClrBit(i++);
        else if (ch == '1')
            bf.SetBit(i++);
        else 
            break; 
        if (i >= bf.GetLength()) break;
    } 
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        if (bf.GetBit(i))
            ostr << '1'; 
        else
            ostr << '0';
    } 
    return ostr;
}
