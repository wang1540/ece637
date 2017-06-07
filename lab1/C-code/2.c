#include <math.h> 
#include "tiff.h" 
#include "allocate.h" 
#include "randlib.h" 
#include "typeutil.h" 
#include "defs.h"
#define FH 9
#define FW 9

void error(char *name);

int main (int argc, char **argv)
{
    FILE *fp;
    struct TIFF_img input_img, output_img;
    int i, j;

	double *lpf[FH];
	for (i = 0; i < FH; i++) {
	    lpf[i] = (double *)malloc(FW * sizeof(double));
	}
	for (i = 0; i < FH; i++) {
	    for(j = 0; j < FW; j++) {
	        lpf[i][j] = 1.0 / 81;
	} }

	  if ( argc != 2 ) error( argv[0] );

	  /* open image file */
	  if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
	  //if ( ( fp = fopen ("E:\2016spring\ECE637\lab1_image_filter\Debug\img12.tif", "rb") ) == NULL ) {
	    fprintf ( stderr, "cannot open file %s\n", argv[1] );
	    exit ( 1 );
	  }
	/* read image */
	if (read_TIFF(fp, &input_img)) {
	    fprintf(stderr, "error reading file %s\n", argv[1]);
	    exit(1);
	}
	/* close image file */
	fclose(fp);
	/* check the type of image data */
	if (input_img.TIFF_type != 'c') {
	    fprintf(stderr, "error:  image must be 24-bit color\n");
	    exit(1);
	}
	/* set up structure for output color image */
	get_TIFF(&output_img, input_img.height, input_img.width, 'c'); /* filter the image with lpf */
	conv2d(&input_img, &output_img, FH, FW, lpf);
	/* open color image file */
	if ((fp = fopen("output-firlpf.tif", "wb")) == NULL) {
	    fprintf(stderr, "cannot open file output.tif\n");
	    exit(1);
	}
	    /* write color image */
	    if(write_TIFF(fp, &output_img)) {
	        fprintf(stderr, "error writing TIFF file %s\n", argv[2]);
	        exit(1);
	}
	    /* close color image file */
	fclose(fp);
	    /* de-allocate space which was used for the images */
	    free_TIFF(&(input_img));
	    free_TIFF(&(output_img));
	    /* free filter array */
	    for (i = 0; i < FW; i++) {
	        free(lpf[i]);
	}
	return(0); 
}


void error(char *name)
{
    printf("usage:  %s  image.tiff \n\n",name);
    printf("this program reads in a 24-bit color TIFF image.\n");
    printf("It then horizontally filters the green component, adds noise,\n");
    printf("and writes out the result as an 8-bit image\n");
    printf("with the name 'green.tiff'.\n");
    printf("It also generates an 8-bit color image,\n");
    printf("that swaps red and green components from the input image");
    exit(1);
}