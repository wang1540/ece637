#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

struct data
{
	int weight;
	double value;
};

double wmfilter(double **img, int i, int j);

int main(int argc, char const *argv[])
{
	/* code */
	FILE *fp;
	struct TIFF_img input_img, output;

	/* open image file */
	if ( ( fp = fopen ( "img14gn.tif", "rb" ) ) == NULL ) {
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

	double **img;
	img = (double **)get_img(input_img.width+4,input_img.height+4,sizeof(double));
	int i, j;

	for(i=0; i<input_img.height+4; i++)
	{
		for(j=0; j<input_img.width+4; j++)
		{
			if((i>1)&&(j>1))
			{
				img[i][j] = input_img.mono[i-2][j-2];
			}
			else
			{
				img[i][j] = 0;
			}
		}
	}

	get_TIFF ( &output, input_img.height, input_img.width, 'g' );

	for(i=2; i<input_img.height+2; i++)
	{
		for(j=2; j<input_img.width+2; j++)
		{
			output.mono[i-2][j-2] = wmfilter(img, i, j);
		}
	}

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
	free_img( (void**)img );
	free_TIFF ( &(input_img) );
	free_TIFF ( &(output) );
	return 0;
}



double wmfilter(double **img, int i, int j)
{
	int k,l,count;
	struct data temp[25];
	struct data t;
	count = 0;

	// get the pixel value and its weight
	for(k=0; k<5; k++)
	{
		for(l=0; l<5; l++)
		{
			if ((k!=0)&&(k!=4)&&(l!=0)&&(l!=4))
			{
				temp[count].weight = 2;
			}
			else
			{
				temp[count].weight = 1;
			}
			temp[count++].value = img[i+k-2][j+l-2];
			// count++;
		}
	}


	// sort
	for(count=0; count<25; count++)
	{
		for(k=count+1; k<25; k++)
		{
			if(temp[count].value < temp[k].value)
			{
				t = temp[count];
				temp[count] = temp[k];
				temp[k] = t;
			}
		}
	}
	// printf("hehe\n");
	// find the weighted median
	int sum_h = 0;
	int sum_t = 0;
	for(count=0; count<25; count++)
	{
		sum_h += temp[count].weight;
		sum_t = 34 - sum_h;
		if(sum_h >= sum_t)
		{
			return temp[count].value;
		}
	}
	return 0.0;
}






