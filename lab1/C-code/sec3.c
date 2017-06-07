
#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

void error(char *name);
double clipping(double pixel);


int main (int argc, char **argv) 
{
  FILE *fp;
  struct TIFF_img input_img, green_img, color_img;
  double **img1,**img2,**img3; // red, green, blue
  int32_t i,j,k,l;

  if ( argc != 2 ) error( argv[0] );

  /* open image file */
  if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
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

  /* check the type of image data */
  if ( input_img.TIFF_type != 'c' ) {
    fprintf ( stderr, "error:  image must be 24-bit color\n" );
    exit ( 1 );
  }

  /* Allocate image of double precision floats */
  img1 = (double **)get_img(input_img.width+8,input_img.height+8,sizeof(double));//red
  img2 = (double **)get_img(input_img.width+8,input_img.height+8,sizeof(double));//green
  img3 = (double **)get_img(input_img.width+8,input_img.height+8,sizeof(double));//blue

  /* copy green component to double array */  
  for(i = 0;i<input_img.height+8;i++)
  for(j = 0;j<input_img.width+8;j++){
      img1[i][j] = (((i>3)&&(i<input_img.height+4))&&((j>3)&&(j<input_img.width+4)))? input_img.color[0][i-4][j-4]:0;
      img2[i][j] = (((i>3)&&(i<input_img.height+4))&&((j>3)&&(j<input_img.width+4)))? input_img.color[1][i-4][j-4]:0;
      img3[i][j] = (((i>3)&&(i<input_img.height+4))&&((j>3)&&(j<input_img.width+4)))? input_img.color[2][i-4][j-4]:0;
  }
  

  /* set up structure for output color image */
  /* Note that the type is 'c' rather than 'g' */
  get_TIFF ( &color_img, input_img.height, input_img.width, 'c' );

  double r, g, b;
  double h = 1.0/81; //for -4 to 4, m&n
      
  for ( i = 0; i < input_img.height; i++ )
  for ( j = 0; j < input_img.width; j++ ) {
    r = 0.0;
    g = 0.0;
    b = 0.0;

    //-4 to 4
    for (k = -4; k <5; k++ )
    for ( l = -4; l < 5; l++ ){
      //take the sum
      //h=1/81
      r = r + h*img1[i-k+4][j-l+4];
      g = g + h*img2[i-k+4][j-l+4]; 
      b = b + h*img3[i-k+4][j-l+4];  
      } 

    //y(m,n)
    color_img.color[0][i][j] = clipping(r);
    color_img.color[1][i][j] = clipping(g);
    color_img.color[2][i][j] = clipping(b);
    }
  
   
  /* open color image file */
  if ( ( fp = fopen ( "color.tif", "wb" ) ) == NULL ) {
      fprintf ( stderr, "cannot open file color.tif\n");
      exit ( 1 );
  }
    
  /* write color image */
  if ( write_TIFF ( fp, &color_img ) ) {
      fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
      exit ( 1 );
  }
    
  /* close color image file */
  fclose ( fp );

  /* de-allocate space which was used for the images */
  free_TIFF ( &(input_img) );
  free_TIFF ( &(color_img) );
  
  free_img( (void**)img1 );
  free_img( (void**)img2 );  
  free_img( (void**)img3 ); 

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


double clipping(double pixel)
{
  if (pixel > 255)  return 255;
  if (pixel < 0)  return 0;
  return pixel;
}
