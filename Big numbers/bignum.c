#include "bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*This exercise implements a module that allows to save non-negative integers of any size and perform operations on it*/
/******* struct *********/

struct Bignum {
  unsigned char* 	num;
  int		size;
};

/**************** AllocateMemory function ****************/

void* AllocateMemory(unsigned int size, const char* file, int line_num)
{
  void*   ptr = malloc(size);
  
  // If we failed the user will know exactly where the error happened
  if(ptr == NULL) {
    fprintf(stderr, "Fatal error in %s (%d): failed to allocate memory of size %u\n\n", file, line_num, size);
    exit(-1);
  }
  
  // Set all memory to 0
  memset(ptr, 0, size);
  
  return ptr;
}

/**************** ReallocateMemory function ****************/

void* ReallocateMemory(void* mem, unsigned int size, const char* file, int line_num)
{
  void*   ptr = realloc(mem, size);
  
  // If we failed the user will know exactly where the error happened
  if(ptr == NULL) {
    fprintf(stderr, "Fatal error in %s (%d): failed to allocate memory of size %u\n\n", file, line_num, size);
    exit(-1);
  }
  
  return ptr;
}

/**************** FreeMemory function ****************/
void FreeMemory(void* mem)
{
  free(mem);
}

/**************** CreateBignum function ****************/

Bignum	CreateBignum(int size){
  
  Bignum p_big=(Bignum)AllocateMemory(sizeof(Bignum),__FILE__, __LINE__);
  
  p_big->num=(unsigned char*)AllocateMemory(size*sizeof(unsigned char),__FILE__, __LINE__);
  p_big->size = size;
  
  return p_big;
}

/**************** CreateFromBignum function ****************/

Bignum	CreateFromBignum(const Bignum bn){
  
  Bignum new_bn = CreateBignum(bn->size);
  Assign(new_bn,bn);/*This function is implemented later. It copies the contents of the right argument to the left.*/
  
  return new_bn;
  
}

/**************** CreateFromUInt function ****************/

Bignum	CreateFromUInt(unsigned int n){
  
  int i;
  Bignum unsig_bn = CreateBignum(sizeof(unsigned int));
  unsig_bn->size = sizeof(unsigned int);
  
  for(i=unsig_bn->size-1;i>=0;i--){
    unsig_bn->num[i]|=n;/*each iteration copy the 8 lsb of n into one cell in num array*/
    
    n>>=NumOfBytesInChar;/* the right shift opeation is "updating" the 8 lsb of n so that during  the loop 
	the whole  unsigned number n will be copied into num array.*/
  }
  
  return unsig_bn;
}


/**************** DestroyBignum function ****************/

void DestroyBignum(Bignum bn){
  
  FreeMemory(bn->num);
  FreeMemory(bn);
}

/**************** Convert function ****************/
/* This function accepts Bignum and returns an unsigned int containing the same number that Bignum contains*/
unsigned int Convert(const Bignum bn){
  
  int i = 0;
  unsigned int ret_number = 0,size=sizeof(unsigned int);
  
  if(bn->size<size){ /*If the bignum is too long then the returned value will contain only the sizeof (unsigned int) 
					of the least significant bytes.*/
    size=bn->size;
  }
  
  else{
    i=bn->size-size;
    size+=i;
  }
  for(;i<size;i++){
    ret_number<<=NumOfBytesInChar;
    ret_number|=bn->num[i];
    
  }
  
  return ret_number;
  
  
}
/**************** LeftShift function ****************/

/*This function gets Bignum and number n and shift the number that saved in Bignum n shifts left.*/
Bignum LeftShift(Bignum bn, int n){
  
  while(n){
    bn=LeftShiftOneByte(bn);
    n--;
  }
  return bn;
}

/**************** LeftShiftOneByte function ****************/

Bignum LeftShiftOneByte(Bignum bn){
  
  int size=bn->size;
  bool add_one_next=false;
  bool add_one_now=false;
  int i;
  
  
  for(i=size-1;i>=0;i--){
    add_one_now=add_one_next;
    if(MsbIsOn(bn->num[i]))
      add_one_next=true;
    else
      add_one_next=false;
    
    PushOneToLeft(&(bn->num[i]));
    
    if(add_one_now){
      (bn->num[i])|=1;/*turn on the lsb of the current cell.*/
      add_one_now=false;
    }
  }
  return bn;
}

/**************** MsbIsOn function ****************/

bool MsbIsOn(unsigned char cell){
  
  unsigned char msb = (1<<SevenNumber);
  
  if(msb&cell)
    return true;
  
  return false;
}

/**************** RightShift function ****************/
/*This function gets Bignum and number n and shift the number that saved in Bignum n shifts right.*/
Bignum RightShift(Bignum bn, int n){
  
  while(n){
    bn=RightShiftOneByte(bn);
    n--;
  }
  return bn;
}

/**************** RightShiftOneByte function ****************/

Bignum RightShiftOneByte(Bignum bn){
  
  int size=bn->size;
  bool add_one=false;
  int i;
  
  for(i=size-1;i>0;i--){
    
    if(LsbIsOn(bn->num[i-1]))/*if the Lsb of next cell from the left is one.*/
      add_one=true;
    
    PushOneToRight(&(bn->num[i]));/* make one right shift to the current cell.*/
    
    if(add_one)
      (bn->num[i])|=SevenAboveTwo;/*turn on the msb of the current cell.*/
    
    add_one=false;
    
  }
  PushOneToRight(&(bn->num[i]));/* make one right shift to the last cell from the right (first cell of the array).*/
  
  return bn;
}


/**************** Push_one_to_right function ****************/

void PushOneToRight(unsigned char *right){
  *right=*right>>1;
}

/**************** Push_one_to_left function ****************/

void PushOneToLeft(unsigned char *left){
  *left=*left<<1;
}

/**************** LsbIsOn function ****************/

bool LsbIsOn(unsigned char cell){
  
  unsigned char lsb = 1;
  
  if(lsb&cell)
    return true;
  return false;
}

/**************** NumBits function ****************/
/*This function returns the length of the number stored in Bignum in bytes*/
int	NumBits(const Bignum bn){
  
  int i,j;
  Bignum help = CreateFromBignum(bn);
  
  /* The princilpe of this algorithem is to find the cell in num array, where the MSB is.
  Then, to find his distance in it cell(j) and add the num of bytes from the right * sizeof(char).*/
  for(i=0;i<bn->size;i++){
    
    if(help->num[i]>0){
      
      for(j=1;j<=NumOfBytesInChar;j++){
	
	if(help->num[i]==1){
	  
	  DestroyBignum(help);
	  return (j+(NumOfBytesInChar*((bn->size-i-1))));
	}
	
	help->num[i]=help->num[i]>>1;
	
      }
    }
  }
  DestroyBignum(help);
  return 0;
}



/**************** CreateFromString function ****************/
/*This function accepts a string containing a number and creates the same bignum as the transferred number.
 The function uses a familiar algorithm that multiplies the content of the number generated at 10 
 and add the current number in each iteration*/
Bignum	CreateFromString(const char* num){
  
  const char *p = num;
  Bignum bn_with_ten = CreateFromUInt(TenNumber);
  Bignum temp_number = CreateFromUInt(*p-'0');
  Bignum new_number;
  
  for(;*(p+1)!='\0';p++){
    
    Multiply(temp_number,bn_with_ten);
    
    new_number = CreateFromUInt(*(p+1)-'0');
    Add(temp_number,new_number);
    
  }
  
  DestroyBignum(bn_with_ten);
  DestroyBignum(new_number);
  
  return temp_number;
}

/**************** Xor function ****************/

Bignum Xor(Bignum bn1, const Bignum bn2){
  
  unsigned char diff = 0;
  int i;
  
  if(bn1->size<bn2->size)
  bn1 =	CheckingAndReallocating(bn1,bn2->size,bn1->size);
  
  if(bn1->size>bn2->size)
    diff = (bn1->size - bn2->size)-1;
  
  else
    diff = bn1->size-1;
  
  for(i=bn1->size-1;(i>=0)&&(diff>=0);i--,diff--)
    bn1->num[i]^=bn2->num[diff];
  
  return bn1;
  
}

/**************** Zeros function ****************/
/*This function zero the bignum from a given index*/
Bignum Zeros(Bignum bn,int j,int size){
  
  int i;
  for(i=j;i<size;i++){
    bn->num[i]=0;
  }
  
  return bn;
}

/**************** not function ****************/

Bignum not(Bignum bn){
  
  int i=0;
  for(i=0;i<bn->size;i++){
    (bn->num[i])=~(bn->num[i]);
  }
  
  return bn;
}

/**************** ChangeSize function ****************/

Bignum ChangeSize(Bignum bn, int new_size){
  
  unsigned char diff = 0;
  
  if(bn->size<new_size){
    diff=new_size-bn->size;
    
    bn->num=(unsigned char*)ReallocateMemory(bn->num,new_size*sizeof(unsigned char),__FILE__,__LINE__);
    bn->size=new_size;
    
    RightShift(bn,diff*NumOfBytesInChar);
    return bn;
  }
  bn = Zeros(bn,0,bn->size-1);
  
  return bn;
  
}

/**************** And function ****************/

Bignum And(Bignum bn1, const Bignum bn2){
  
  unsigned char diff = 0;
  int i;
  
  /*If the bn1 memory size is smaller than that of bn2, the function will increase the memory of bn1 
  to the size of bn2 and fill the new part with zeros.*/
  if(bn1->size<bn2->size)
    bn1 = CheckingAndReallocating(bn1,bn2->size,bn1->size);
  
  
  if(bn1->size>bn2->size)
    diff = (bn1->size - bn2->size)-1;
  
  else
    diff = bn1->size-1;
  
  for(i=bn1->size-1;(i>=0)&&(diff>=0);i--,diff--)
    bn1->num[i]&=bn2->num[diff];
  
  
  return bn1;
  
}
/**************** Checking_and_reallocating function ****************/

Bignum CheckingAndReallocating(Bignum bn1,int size,int temp){
  
  bn1=ChangeSize(bn1,size);
  bn1=Zeros(bn1,temp,bn1->size);
  
  return bn1;
}

/**************** Or function ****************/

Bignum Or(Bignum bn1, const Bignum bn2){
  
  unsigned char diff = 0;
  int i;
  
  if(bn1->size<bn2->size)
    bn1 =CheckingAndReallocating(bn1,bn2->size,bn1->size);
  
  
  if(bn1->size>bn2->size)
    diff = (bn1->size - bn2->size)-1;
  
  else
    diff = bn1->size-1;
  
  for(i=bn1->size-1;(i>=0)&&(diff>=0);i--,diff--)
    bn1->num[i]|=bn2->num[diff];
  
  return bn1;
}

/**************** Assign function ****************/

Bignum	Assign(Bignum bn1, const Bignum bn2){
  
  Bignum zeros_bignum = CreateBignum(bn1->size);
  
  And(bn1,zeros_bignum);/*zero bn1*/
  Or(bn1,bn2);/*copy bn2 to bn1*/
  
  DestroyBignum(zeros_bignum);
  
  return bn1;
  
}

/**************** Add function ****************/
/*This function add two Bignums in assist of the attached algorithem 
{
    while (y != 0)
    {
        int carry = x & y;
        x = x ^ y; 
        y = carry << 1;
    }
    return x;
}*/
Bignum	Add(Bignum bn1, const Bignum bn2){
  
  Bignum bn_copy_of_bn1 = CreateFromBignum(bn1);
  Bignum bn_copy_of_bn2 = CreateFromBignum(bn2);
  Bignum bn_zeros = CreateBignum(bn_copy_of_bn2->size);
  
  Bignum bn_carry = CreateFromBignum(bn1);
  
  while(!IsEqualsTo(bn_copy_of_bn2,bn_zeros)){
    
    
    Assign(bn_carry,And(bn_copy_of_bn1,bn_copy_of_bn2));
    Assign(bn_copy_of_bn1,Xor(bn1,bn_copy_of_bn2));
    Assign(bn_copy_of_bn2,LeftShiftOneByte(bn_carry));
    
  }
  
  Assign(bn1,bn_copy_of_bn1);
  DestroyBignum(bn_copy_of_bn2);
  DestroyBignum(bn_copy_of_bn1);
  DestroyBignum(bn_zeros);
  DestroyBignum(bn_carry);
  
  return bn1;
}


/**************** Multiply function ****************/
/*This function multiply two Bignums in assist of the attached algorithem
{
    int res = 0; 
 
  
    while (b > 0)// While second number doesn't become 1
    {
         // If second number becomes odd, add the first number to result
         if (b & 1)
             res = res + a;
 
         // Double the first number and halve the second number
         a = a << 1;
         b = b >> 1;
     }
     return res;
}*/
Bignum	Multiply(Bignum bn1, const Bignum bn2){
  
  Bignum copy_of_bn1 = CreateFromBignum(bn1);
  Bignum copy_of_bn2 = CreateFromBignum(bn2);
  Bignum bn_one = CreateBignum(bn2->size);
  Bignum bn_zeros = CreateBignum(bn2->size);
  Bignum bn_result = CreateBignum(bn2->size);
  Bignum help;
  
  bn_one->num[bn_one->size-1]|=1;  
  
  while(IsGreaterThan(copy_of_bn2,bn_zeros)){
    
    help = CreateFromBignum(copy_of_bn2);
    
    And(help,bn_one);
    
    if(IsEqualsTo(help,bn_one))
      Add(bn_result,copy_of_bn1);
    
    
    DestroyBignum(help);
    LeftShiftOneByte(copy_of_bn1);
    RightShiftOneByte(copy_of_bn2);
  }  
  
  Assign(bn1,bn_result);
  
  DestroyBignum(bn_result);
  DestroyBignum(bn_zeros);
  DestroyBignum(bn_one);
  DestroyBignum(copy_of_bn2);
  DestroyBignum(copy_of_bn1);
  
  return bn1;
}


/**************** IsGreaterThan function ****************/

bool	IsGreaterThan(const Bignum bn1, const Bignum bn2){
	int num_bits1 = NumBits(bn1);
	int num_bits2 = NumBits(bn2);
  
  
	int i;
  
	if(num_bits1<num_bits2)
		return false;
  
	 if(num_bits1>num_bits2)
		return true;
	/*in case that the num of the bits are equal we will scan the Bignums from the left.
	if we wiil find one byte (cell in num array) that greater or smaller than rhe other 
	it means the whole bignum is greater or smaller respectively.*/
	
    
    for(i=0;i<bn1->size-1;i++){
      if(bn1->num[i]>bn2->num[i])
        return true;
      if(bn1->num[i]<bn2->num[i])
	      return false;
      
    }
  
  return false;
}
/**************** IsSmallerThan function ****************/
bool	IsSmallerThan(const Bignum bn1, const Bignum bn2){
  
  return IsGreaterThan(bn2,bn1);
}

/**************** IsEqualsTo function ****************/
bool	IsEqualsTo(const Bignum bn1, const Bignum bn2){
  
  if(!IsGreaterThan(bn1,bn2))
    if(!IsSmallerThan(bn1,bn2))
      return true;
    
    return false;
}
