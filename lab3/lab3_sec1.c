#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

/*gcc -ansi -Wall -std=c99 lab3_sec1.c tiff.c allocate.c randlib.c*/

struct pixel	{
	int m,n;
};

void ConnectedNeighbors(
	struct pixel s,
	double T,
	unsigned char ** img, 
	int width,
	int height,
	int *M,
	struct pixel c[4]);

void ConnectedSet(
	struct pixel s,
	double T,
	unsigned char **img,
	int width,
	int height,
	int ClassLabel,
	unsigned char **seg,
	int *NumConPixels);

int main(int argc, char const *argv[])
{
	/* code */
	FILE *fp;
	struct TIFF_img input_img, output;
	struct pixel s;
	double T = 1;
	int ClassLabel = 0;
	int NumConPixels = 0;
	//unsigned int **seg;

	s.m = 0; 
	s.n = 273; 
	T = 1.0;

	/* open image file */
	if ( ( fp = fopen ( "img22gd2.tif", "rb" ) ) == NULL ) {
	  fprintf ( stderr, "cannot open file %s\n", argv[1] );
	  exit ( 1 );
	}

	/* read image */
	if ( read_TIFF ( fp, &input_img ) ) {
	  fprintf ( stderr, "error reading file %s\n", argv[1] );
	  exit ( 1 );
	}

	/* close image file */
	fclose ( fp );

	get_TIFF ( &output, input_img.height, input_img.width, 'g' );

	
	int i, j;
	for(i=0; i<input_img.height; i++)
	{
		for(j=0; j<input_img.width; j++)
		{
			output.mono[i][j] = 255;
		}
	}
	output.mono[s.m][s.n] = ClassLabel;

	// int M = 0;
	// struct pixel c[4];
	// ConnectedNeighbors(s, T, input_img.mono, input_img.width, input_img.height, &M, c);
	// printf("%d\n", M);
	// printf("%d\n", input_img.mono[0][16]);


	ConnectedSet(s, T, input_img.mono, input_img.width, input_img.height, ClassLabel, output.mono, &NumConPixels);

	// printf("%d\n", NumConPixels);

  	/* open output image file */
  	if ( ( fp = fopen ( "output.tif", "wb" ) ) == NULL ) {
    	fprintf ( stderr, "cannot open file output.tif\n");
    	exit ( 1 );
  	}

  	/* write output image */
  	if ( write_TIFF ( fp, &output ) ) {
    	fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
    	exit ( 1 );
  	}

  	/* close output image file */
  	fclose ( fp );

	free_TIFF ( &(input_img) );
	free_TIFF ( &(output) );
	return 0;
}

void ConnectedNeighbors(struct pixel s, double T, unsigned char ** img, int width, int height, int *M, struct pixel c[4])
{

	if(((s.m-1)>=0)&&(abs(img[s.m][s.n]-img[s.m-1][s.n])<=T))
	{
		c[*M].m = s.m-1;
		c[(*M)++].n = s.n;
	}

	if(((s.m+1)<height)&&(abs(img[s.m][s.n]-img[s.m+1][s.n])<=T))
	{
		c[*M].m = s.m+1;
		c[(*M)++].n = s.n;
	}

	if(((s.n-1)>=0)&&(abs(img[s.m][s.n]-img[s.m][s.n-1])<=T))
	{
		c[*M].m = s.m;
		c[(*M)++].n = s.n-1;
	}

	if(((s.n+1)<width)&&(abs(img[s.m][s.n]-img[s.m][s.n+1])<=T))
	{
		c[*M].m = s.m;
		c[(*M)++].n = s.n+1;
	}

	return;
}

void ConnectedSet(struct pixel s, double T, unsigned char **img, int width, int height, int ClassLabel, unsigned char **seg, int *NumConPixels)
{
	int M = 0;
	struct pixel c[4];

	ConnectedNeighbors(s, T, img, width, height, &M, c);

	while(M>0)
	{
		if (seg[c[M-1].m][c[M-1].n] != ClassLabel)
		{
			seg[c[M-1].m][c[M-1].n] = ClassLabel;
			(*NumConPixels)++;
			printf("%d %d\n", c[M-1].m, c[M-1].n);
			ConnectedSet(c[M-1], T, img, width, height, ClassLabel, seg, NumConPixels);
		}
		M--;
	}
	return;
}







