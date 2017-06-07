#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"


int main(int argc, char const *argv[])
{
	/* code */
	FILE *fp;
	struct TIFF_img org_img, crp_img, output;
	struct pixel s;

	/* open original image file */
	if ( ( fp = fopen ( "img14g.tif", "rb" ) ) == NULL ) {
	  fprintf ( stderr, "cannot open file %s\n", argv[1] );
	  exit ( 1 );
	}

	/* read image */
	if ( read_TIFF ( fp, &org_img ) ) {
	  fprintf ( stderr, "error reading file %s\n", argv[1] );
	  exit ( 1 );
	}

	/* close image file */
	fclose ( fp );

	/* open corrupted image file */
	if ( ( fp = fopen ( "img14sp.tif", "rb" ) ) == NULL ) {
	  fprintf ( stderr, "cannot open file %s\n", argv[1] );
	  exit ( 1 );
	}

	/* read image */
	if ( read_TIFF ( fp, &crp_img ) ) {
	  fprintf ( stderr, "error reading file %s\n", argv[1] );
	  exit ( 1 );
	}

	/* close image file */
	fclose ( fp );

	get_TIFF ( &output, org_img.height, org_img.width, 'g' );


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