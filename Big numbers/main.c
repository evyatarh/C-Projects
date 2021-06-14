#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bignum.h"

/************************************************************************************************************************************************
 * This program will test the bignum module using random numbers. You can run the program multiple times in order to try different numbers
 ***********************************************************************************************************************************************/
int main()
{
	srand(time(NULL));

	Bignum bn = CreateBignum(4);
	printf("CreateBignum, Convert\t%s\t(Bignum=%u, Expected=0)\n", ((Convert(bn)==0)? "ok" : "fail"), Convert(bn));
	DestroyBignum(bn);

	unsigned int x1 = rand();
	unsigned int x2 = rand();
	Bignum bn1 = CreateFromUInt(x1);
	Bignum bn2 = CreateFromUInt(x2);

	// Creation succeeded
	printf("CreateFromUInt  \t%s\t(Bignum=%u, Expected=%u)\n", ((x1==Convert(bn1))? "ok" : "fail"), Convert(bn1), x1);

	bn = CreateFromBignum(bn1);
	printf("CreateFromBignum\t%s\t(Bignum=%u, Expected=%u)\n", ((Convert(bn)==Convert(bn1))? "ok" : "fail"), Convert(bn), Convert(bn1));
	DestroyBignum(bn);

	char str[128];
	sprintf(str, "%u", rand());
	bn = CreateFromString(str);
	printf("CreateFromString\t%s\t(Bignum=%u, Expected=%s)\n", ((Convert(bn)==atoi(str))? "ok" : "fail"), Convert(bn), str);
	DestroyBignum(bn);

	unsigned int y = rand();
	bn = CreateFromUInt(y);
	ChangeSize(bn, 1);
	printf("ChangeSize      \t%s\t(Bignum=%u, expected=%u)\n", ((Convert(bn)==(unsigned char)y)? "ok" : "fail"), Convert(bn), (unsigned char)y);
	DestroyBignum(bn);

	x2 <<=5;
	LeftShift(bn2, 5);
	printf("LeftShift       \t%s\t(Bignum=%u, Expected=%u)\n", ((x2==Convert(bn2))? "ok" : "fail"), Convert(bn2), x2);

	x1 >>= 7;
	RightShift(bn1, 7);
	y = Convert(bn1);
	printf("RightShift      \t%s\t(Bignum=%u, Expected=%u)\n", ((x1==Convert(bn1))? "ok" : "fail"), Convert(bn1), x1);

	x1 ^= x2;
	Xor(bn1, bn2);
	printf("Xor             \t%s\t(Bignum=%u, Expected=%u)\n", ((x1==Convert(bn1))? "ok" : "fail"), Convert(bn1), x1);

	x2=rand();
	DestroyBignum(bn2);
	bn2=CreateFromUInt(x2);
	x1 &= x2;
	And(bn1, bn2);
	printf("And             \t%s\t(Bignum=%u, Expected=%u)\n", ((x1==Convert(bn1))? "ok" : "fail"), Convert(bn1), x1);

	x2=rand();
	DestroyBignum(bn2);
	bn2=CreateFromUInt(x2);
	x1 |= x2;
	Or(bn1, bn2);
	printf("Or              \t%s\t(Bignum=%u, Expected=%u)\n", ((x1==Convert(bn1))? "ok" : "fail"), Convert(bn1), x1);

	x1 = ~x1;
	not(bn1);
	printf("Not             \t%s\t(Bignum=%u, Expected=%u)\n", ((x1==Convert(bn1))? "ok" : "fail"), Convert(bn1), x1);

	Bignum bn3 = CreateBignum(3);
	Assign(bn3, bn1);
	printf("Assign          \t%s\t(Bignum=%u, Expected=%u)\n", ((Convert(bn1)==Convert(bn3))? "ok" : "fail"), Convert(bn3), Convert(bn1));

	unsigned int n=0;
	y = x1;
	while(y > 0) {
		n++;
		y >>= 1;
	}
	printf("NumBits         \t%s\t(Bignum=%u, Expected=%u)\n", ((NumBits(bn1)==n)? "ok" : "fail"), NumBits(bn1), n);

	printf("IsGreaterThan   \t%s\t(Bignum=%s, Expected=%s)\n", ((IsGreaterThan(bn1, bn2)==(x1>x2))? "ok" : "fail"), (IsGreaterThan(bn1, bn2)? "True" : "False"), ((x1>x2)? "True" : "False"));
	printf("IsSmallerThan   \t%s\t(Bignum=%s, Expected=%s)\n", ((IsSmallerThan(bn1, bn2)==(x1<x2))? "ok" : "fail"), (IsSmallerThan(bn1, bn2)? "True" : "False"), ((x1<x2)? "True" : "False"));
	printf("IsEqualTo       \t%s\t(Bignum=%s, Expected=%s)\n", ((IsEqualsTo(bn1, bn2)==(x1==x2))? "ok" : "fail"), (IsEqualsTo(bn1, bn2)? "True" : "False"), ((x1==x2)? "True" : "False"));

	DestroyBignum(bn1);
	DestroyBignum(bn2);
	x1=rand()%1000;
	x2=rand()%1000;
	bn1=CreateFromUInt(x1);
	bn2=CreateFromUInt(x2);
	
	Multiply(bn1, bn2);
	x1 *= x2;
	printf("Multiply        \t%s\t(Bignum=%u, Expected=%u)\n", ((Convert(bn1)==x1)? "ok" : "fail"), Convert(bn1), x1);
	
	Add(bn1, bn2);
	x1 += x2;
	printf("Add             \t%s\t(Bignum=%u, Expected=%u)\n", ((Convert(bn1)==x1)? "ok" : "fail"), Convert(bn1), x1);

	DestroyBignum(bn1);
	DestroyBignum(bn2);

	return 0;
}
