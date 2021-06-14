#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdbool.h>

#define NumOfBytesInChar 8
#define TenNumber 10
#define SevenNumber 7
#define SevenAboveTwo 128

typedef struct Bignum* Bignum;

// Creates a Bignum initialized to 0
Bignum	CreateBignum(int size);

int modolo_num(int num1,int mod);

// Creates a new copy of bn
Bignum	CreateFromBignum(const Bignum bn);

// Creates a Bignum from an integer
Bignum	CreateFromUInt(unsigned int n);

// Creates a Bignum from a string (e.g. "12345678999")
Bignum	CreateFromString(const char* num);

// Destructor for Bignums
void	DestroyBignum(Bignum bn);

// Change Bignum's size
Bignum ChangeSize(Bignum bn, int new_size);

// Left-shift by any number (<< n)
Bignum LeftShift(Bignum bn, int n);

// Right-shift by any number
Bignum RightShift(Bignum bn, int n);

// Bitwise xor. Will change bn1's size if it is smaller than bn2's
Bignum Xor(Bignum bn1, const Bignum bn2);

// Bitwise xor. Will change bn1's size if it is smaller than bn2's
Bignum And(Bignum bn1, const Bignum bn2);

// Bitwise or. Will change bn1's size if it is smaller than bn2's
Bignum Or(Bignum bn1, const Bignum bn2);

// Bitwise not
Bignum not(Bignum bn);

// Assign the value of bn2 to bn1. Change bn1's size if required
Bignum	Assign(Bignum bn1, const Bignum bn2);

// Convert a Bignum to an unsigned int. Ignore bytes that are larger than sizeof(int).
unsigned int Convert(const Bignum bn);

// Return the number of bits in the number currently stored in bn (-1 for 0).
int	NumBits(const Bignum bn);

// Compaerators: >, < and ==
bool	IsGreaterThan(const Bignum bn1, const Bignum bn2);
bool	IsSmallerThan(const Bignum bn1, const Bignum bn2);
bool	IsEqualsTo(const Bignum bn1, const Bignum bn2);

// Add bn1 and bn2, return the result
Bignum	Add(Bignum bn1, const Bignum bn2);

// Multiply bn1 by bn2, return the result's address
Bignum	Multiply(Bignum bn1, const Bignum bn2);

Bignum CheckingAndReallocating(Bignum bn1,int size,int temp);

Bignum Zeros(Bignum bn,int j,int size);

bool LsbIsOn(unsigned char cell);

void PushOneToLeft(unsigned char *left);

void PushOneToRight(unsigned char *right);

Bignum RightShiftOneByte(Bignum bn);

bool MsbIsOn(unsigned char cell);

Bignum LeftShiftOneByte(Bignum bn);



#endif
