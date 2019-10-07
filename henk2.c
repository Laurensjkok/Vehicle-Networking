#include <stdio.h>
#define IdLength 11
#define MaxDataLength 64
#define POLYNOMIAL 0xC599  /* 11011 followed by 0's */

bool frame[135];
bool framestart[15];
bool bindata[MaxDataLength];
bool idbin[IdLength];
bool checksum[15];
bool DLCbin[4];
bool polynomial[16] = {1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,1};
int iddec = 3;
int EndOfData;
int numbytes;
int stuffedlength;
unsigned long data = 72057594037927935;


void stuffing()
{
	int insertedbits = 0; //set counter to zero
	int c, ins;//kak
	int n = 122;
	int val;
	int i = 0;
	while(i<122){
		val = frame[i];// bit stuffing is only necessary for consequtive ones, not zeros
		if ((val == frame[i+1]) && (val == frame[i+2]) && (val == frame[i+3]) && (val == frame[i+4])){
		ins = 1 - frame[i];
		insertedbits++;
		for (c = n - 1; c >= (i+5) - 1; c--) {  
			frame[c+1] = frame[c];
		}
		frame[i+5] = ins;
		}
		if (i >= (insertedbits + EndOfData + 13)){
			break;
		}
		i++;
	}
	stuffedlength = EndOfData + 15 + insertedbits; //Indicates index of CRC delimiter bit

	
}

void CRC()
{	//Very inefficient CRC implementation! Can no doubt be improved
	bool checkdata[79];
	int k;
	for (int j = 0; j < 64; j++){
		checkdata[j] = bindata[j];
	}
	for (int i = 64; i < 79; i++){
		checkdata[i] = 0;
		
	}
	while (k<64){
		if (checkdata[k]==0){
			k++;
		}
		else{
			for (int l=0; l<16; l++)
			{
				checkdata[k+l] = checkdata[k+l] ^ polynomial[l];
			}
	
		}
	}
	for (int m = 0; m<15; m++){
		checksum[m] = checkdata[64+m];
	}
}
	

int RoundUp(double input)
{
	int whole;
	double rem;
	int output;
	whole = (int)input;
	rem = input - (double)whole;
	if (rem == 0){
		output = whole;
	}
	else{
		output = whole + 1;
	}
	return output;
}

void iddec2bin()//can we use one dec2bin function to call inside the code?
{
	int n = iddec;
	int i = 0;
	while (n > 0)
	{
		idbin[IdLength-i -1 ] = n % 2;
		n = n/2;
		i++;
	}
}

void DLCdec2bin(int n)
{
	int i = 0;
	while (n > 0)
	{
		DLCbin[3-i] = n % 2;
		n = n/2;
		i++;
	}
}


int datadec2bin()
{
	unsigned long n = data;
	int i = 0;
	int k = 0;
	int LengthOfDataField = 0;
	double dnumbytes = 0;
	while (n > 0)
	{
		bindata[MaxDataLength-i -1 ] = n % 2;
		n = n/2;
		i++;
	}
	i = 0;
	while (k == 0)
	{
		if (i == 63 && bindata[i] == 0){
			LengthOfDataField = 0;
			break;
		}
		else{
		k = bindata[i];
		LengthOfDataField = MaxDataLength - i;
		i++;
		}
	}
	
	dnumbytes = (double)LengthOfDataField / 8;
	LengthOfDataField = RoundUp(dnumbytes);
//	int BitsOfDataField = LengthOfDataField * 8;
//	bool DataOut[BitsOfDataField];
//	for (i = 0; i < LengthOfDataField; ++i)
//	{
//		DataOut[BitsOfDataField -i -1] = bindata[MaxDataLength -i -1];
//	}
//	dnumbytes2 = (dnumbytes + 0.5);
//	printf("%f \n", dnumbytes2);
//	inumbytes = dnumbytes2;
//	printf("%d \n", inumbytes);
//	printf("%d \n", LengthOfDataField);
	return LengthOfDataField;
	
}

void make_frame()
{
	
	iddec2bin();

	numbytes = datadec2bin();
	DLCdec2bin(numbytes);
//	printf("%d", numbytes);
	CRC();
	
	EndOfData = (19+(8*numbytes)); // Not really end of data. Indicates first bit of CRC field
	frame[0] = 0;
	for (int i = 1; i < (IdLength + 1); ++i)
	{
		frame[i] = idbin[i-1];
	}
	for (int i = 12; i < 15; ++i)
	{
		frame[i] = 0;
	}
	for (int i = 15; i < 19; ++i)
	{
		frame[i] = DLCbin[i-15];
	}
	for (int i = 19; i < EndOfData; ++i)
	{
		frame[i] = bindata[MaxDataLength - (8*numbytes) + i - 19];
	}
	for (int i = EndOfData; i < (EndOfData+15);++i){
		frame[i] = checksum[i-EndOfData];
	}
	stuffing();
	for (int i = stuffedlength; i<(stuffedlength+13); ++i){
		frame[i] = 1;
	}
}



int main()
{
	make_frame();
	printf("\n");
	for (int j = 0; j < (stuffedlength + 13); ++j)
	{
		printf("%d", frame[j]);
	}	
	
	
}


