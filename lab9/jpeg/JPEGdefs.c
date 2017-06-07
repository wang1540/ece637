#include "JPEGdefs.h"
#include "Htables.h"

int BitSize(int value)
{
	int bitsize=0;
	value = abs(value);

	while (value>0)
	{
		bitsize++;
		value = value/2;
	}

	return bitsize;
}

void VLI_encode(int bitsize, int value, char *block_code)
{
	int i;
	char VLI[20] = "";
	bitsize = BitSize(value);
	value = (value < 0)? (value-1) : value;

	for (i = bitsize - 1; i >= 0; i--)
	{
		VLI[i] = (value & 1)? '1':'0';
		value >>= 1;
	}

	strcat(block_code, VLI);
	return;
}

void ZigZag(int ** img, int y, int x, int *zigline)
{
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			zigline[Zig[i][j]] = img[y+i][x+j];
		}
	}
	return;
}

void DC_encode(int dc_value, int prev_value, char *block_code)
{
	int diff = dc_value - prev_value;
	int bitsize = BitSize(diff);

	strcat(block_code, dcHuffman.code[bitsize]);
	VLI_encode(bitsize, diff, block_code);
	return;
}

void AC_encode(int *zigzag, char *block_code)
{
	/* Init variables */
	int idx = 1;
	int zerocnt = 0;
	int bitsize;

	while(idx < 64)
	{
		if (zigzag[idx]==0)	zerocnt++;
		else{
			for (; zerocnt > 15; zerocnt -= 16) {
				strcat(block_code, acHuffman.code[15][0]);
			}
			bitsize = BitSize(zigzag[idx]);
			strcat(block_code, acHuffman.code[zerocnt][bitsize]);
			VLI_encode(bitsize, zigzag[idx], block_code);
			zerocnt = 0;
		}
		idx++;
	}

	/* EOB coding */
	if(zerocnt)	strcat(block_code, acHuffman.code[0][0]);
	return;
}

void Block_encode(int prev_dc, int *zigzag, char *block_code)
{
	DC_encode(zigzag[0], prev_dc, block_code);
	AC_encode(zigzag, block_code);
	return;
}

unsigned char bin2dec(char *bin)
{
	int dec = 0;
	for(int i=0; i<8; i++)
	{	
		dec = dec + ((int)bin[i]-48)*pow(2, 7-i);
	}
	return (unsigned char) dec;
}

int Convert_encode(char *block_code, unsigned char *byte_code)
{
	int length = strlen(block_code) / 8;
	int len = length;
	int i,tmp,k;
	char temp[9]="";

	k = 0;
	for (i = 0; i < len; i++)
	{
		tmp = strxfrm (temp, block_code, 9);
		memmove(block_code, block_code+8, strlen(block_code));
		byte_code[k] = bin2dec(temp);

		if (byte_code[k] == 0xff) 
		{
			k++;
			byte_code[k] = 0x00;
			length++;
		}
		k++;
	}
	return length;
}

unsigned char Zero_pad(char *block_code)
{
	int byte_value;
	int length = strlen(block_code);

	for(int i=0; i<length; i++)
	{	
		byte_value = byte_value + ((int)block_code[i]-48)*pow(2, 7-i);
	}
	return (unsigned char) byte_value;
}

/*
int main(int argc, char const *argv[])
{
	int bitsize = BitSize(-6);
	char block_code[100]="0111111111111001111111110011111111100111111111010010011010";

// "01111111 11111001 11";


	// VLI_encode(bitsize, -6, block_code);
	printf("%s\n", block_code);
	// char temp[9]="";
	// int i = strxfrm (temp, block_code, 9);
	// memmove(block_code, block_code+8, strlen(block_code));
	// unsigned char kk = bin2dec(temp);
	// printf("%u\n", kk);
	// printf("%s\n", temp);
	// printf("%s\n", block_code);


	// i = strxfrm (temp, block_code, 9);
	// memmove(block_code, block_code+8, strlen(block_code));
	// kk = bin2dec(temp);
	// printf("%u\n", kk);
	// printf("%s\n", temp);
	// printf("%s\n", block_code);

	unsigned char byte_code[100];
	int l = Convert_encode(block_code, byte_code);
	for (int i = 0; i<l; i++)
	{
		printf("%u\n", byte_code[i]);
	}
	printf("%s\n", block_code);

	// char temp[8] = "01111111";
	// unsigned char kk = stringToBinary(temp);
	// printf("%u\n", kk);
	char wawa[100]="000111";
	unsigned char tt = Zero_pad(wawa);
	printf("%u\n", tt);

	tt = xZero_pad(wawa);
	printf("%u\n", tt);


	return 0;
}*/