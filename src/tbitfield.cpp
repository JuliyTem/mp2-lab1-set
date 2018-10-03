// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 1) throw -1;
	BitLen = len;
	MemLen = ((BitLen + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8));
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[BitLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{ 
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n > BitLen)) throw - 3;
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n > (sizeof(TELEM) * 8))) throw - 3;
	return (TELEM)0 | ((TELEM)1 << n);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n > BitLen)) throw - 4;
	pMem[GetMemIndex(n)] |= GetMemMask(n % (sizeof(TELEM) * 8));
	return;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n > BitLen)) throw - 5;
	pMem[GetMemIndex(n)] &= (~GetMemMask(n % (sizeof(TELEM) * 8)));
	return;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n > BitLen || n < 0) throw - 3;
	TELEM mask = GetMemMask(n % (sizeof(TELEM) * 8));
	return ((pMem[GetMemIndex(n)] & (mask)) > 0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) return *this;
	delete[]pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	this->pMem = new TELEM[BitLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (&bf == this)
		return 1;
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i])
				return 0;
		return 1;
	}
	return 0;
	
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (*this == bf) return 0;
	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (BitLen > bf.BitLen)
	{
		TBitField temp(*this);
		for (int i = 0; i < bf.MemLen; i++)
		{
			temp.pMem[i] = pMem[i] | bf.pMem[i];
		}
		return temp;
	}
	else 
	{
		TBitField temp(bf);
		for (int i = 0; i < MemLen; i++)
		{
			temp.pMem[i] = pMem[i] | bf.pMem[i];
		}
		return temp;
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen > bf.BitLen)
	{
		TBitField temp(BitLen);
		for (int i = 0; i < bf.MemLen; i++)
		{
			temp.pMem[i] = pMem[i] & bf.pMem[i];
		}
		return temp;
	}
	else
	{
		TBitField temp(bf.BitLen);
		for (int i = 0; i < MemLen; i++)
		{
			temp.pMem[i] = pMem[i] & bf.pMem[i];
		}
		return temp;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField out(BitLen);
	for (int i = 0; i < (MemLen - 1); i++)
	{
		out.pMem[i] = ~(pMem[i]);
	}
	for (int i = ((MemLen - 1) * sizeof(TELEM)); i < BitLen; i++)
	{
		if (GetBit(i))
			out.ClrBit(i);
		else
			out.SetBit(i);
	}
	return out;
}

// ввод/вывод

istream &operator >> (istream &istr, TBitField &bf) // ввод
{
	int input = 0;
	cin >> input;
	bf.SetBit(input);
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
	{
		if (bf.GetBit(i)) ostr << i << " ";
	}
	ostr << "\n";
	return ostr;
}
