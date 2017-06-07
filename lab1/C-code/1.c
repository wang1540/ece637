// #include <math.h>
// #include "tiff.h"
// #include "allocate.h"
// #include "randlib.h"
// #include "typeutil.h"

// void error(char *name);

// int main (int argc, char **argv) 
// {
//   FILE *fp;
//   struct TIFF_img input_img, color_img;
//   double **img_red,**img_green,**img_blue;
//   int32_t i,j,k,l;
//   double r_value,g_value,b_value;
	
//   if ( argc != 2 ) error( argv[0] );

//   /* open image file */
//   if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
//   //if ( ( fp = fopen ("E:\2016spring\ECE637\lab1_image_filter\Debug\img12.tif", "rb") ) == NULL ) {
//     fprintf ( stderr, "cannot open file %s\n", argv[1] );
//     exit ( 1 );
//   }

//   /* read image */
//   if ( read_TIFF ( fp, &input_img ) ) {
//     fprintf ( stderr, "error reading file %s\n", argv[1] );
//     exit ( 1 );
//   }

//   /* close image file */
//   fclose ( fp );

//   /* check the type of image data */
//   if ( input_img.TIFF_type != 'c' ) {
//     fprintf ( stderr, "error:  image must be 24-bit color\n" );
//     exit ( 1 );
//   }

//   /* Allocate image of double precision floats */
//   img_red = (double **)get_img(input_img.width+8,input_img.height+8,sizeof(double));//imgsize + filtersize
//   //img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
//   img_green = (double **)get_img(input_img.width+8,input_img.height+8,sizeof(double));
//   img_blue = (double **)get_img(input_img.width+8,input_img.height+8,sizeof(double));

    
//   /* set up structure for output color image */
//   /* Note that the type is 'c' rather than 'g' */
//   get_TIFF ( &color_img, input_img.height, input_img.width, 'c' );//the output img

//   for (i = 0;i<input_img.height+8;i++){
//     for(j = 0;j<input_img.width+8;j++){
//       img_red[i][j] = 0;
//       img_green[i][j] = 0;
//       img_blue[i][j] = 0;  
//     }
//   }
  
//   for(i = 4;i<input_img.height+4;i++){
//     for(j = 4;j<input_img.width+4;j++){
//       img_red[i][j] = input_img.color[0][i-4][j-4];
//       img_green[i][j] = input_img.color[1][i-4][j-4];
//       img_blue[i][j] = input_img.color[2][i-4][j-4];     //create new imgs with boundary filled with 0
//     }
//   }

//   /* Illustration: constructing a sample color image -- interchanging the red and green components from the input color image */
//   for ( i = 0; i < input_img.height; i++ ){
//       for ( j = 0; j < input_img.width; j++ ) {
// 	  r_value = 0;
// 	  g_value = 0;
// 	  b_value = 0;
// 	  for (k = -4; k <=4; k++ ){
//      		 for ( l = -4; l <= 4; l++ ){
// 				r_value = r_value + (img_red[i+4-k][j+4-l])/81.0;   //i+4,j+4 are the index should start to read
// 				g_value = g_value + (img_green[i+4-k][j+4-l])/81.0;	
// 				b_value = b_value + (img_blue[i+4-k][j+4-l])/81.0;	
// 			  }	
// 			}	
// 		  color_img.color[0][i][j] = (int)r_value;
// 		  color_img.color[1][i][j] = (int)g_value;
// 		  color_img.color[2][i][j] = (int)b_value;
//       }
// }
  
    
    
//   /* open color image file */
//   if ( ( fp = fopen ( "result_Q3.tif", "wb" ) ) == NULL ) {
//       fprintf ( stderr, "cannot open file color.tif\n");
//       exit ( 1 );
//   }
    
//   /* write color image */
//   if ( write_TIFF ( fp, &color_img ) ) {
//       fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
//       exit ( 1 );
//   }
    
//   /* close color image file */
//   fclose ( fp );

//   /* de-allocate space which was used for the images */
//   free_TIFF ( &(input_img) );
//   free_TIFF ( &(color_img) );
//   free_img( (void**)img_red );
//   free_img( (void**)img_blue );
//   free_img( (void**)img_green );  

//   //system£¨"PAUSE"£©;
//   return(0);
// }

// void error(char *name)
// {
//     printf("usage:  %s  image.tiff \n\n",name);
//     printf("this program reads in a 24-bit color TIFF image.\n");
//     printf("It then horizontally filters the green component, adds noise,\n");
//     printf("and writes out the result as an 8-bit image\n");
//     printf("with the name 'green.tiff'.\n");
//     printf("It also generates an 8-bit color image,\n");
//     printf("that swaps red and green components from the input image");
//     exit(1);
// }








#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

double clipping(double pixel);
void error(char *name);

int main (int argc, char **argv) 
{
  FILE *fp;
  struct TIFF_img input_img, color_img;
  double **img_red,**img_green,**img_blue;

  float lamda = 1.5;

  int32_t i,j,k,l;
  double r,g,b;
  
  if ( argc != 2 ) error( argv[0] );

  /* open image file */
  if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
  //if ( ( fp = fopen ("E:\2016spring\ECE637\lab1_image_filter\Debug\img12.tif", "rb") ) == NULL ) {
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
  img_red = (double **)get_img(input_img.width+4,input_img.height+4,sizeof(double));//imgsize + filtersize
  //img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  img_green = (double **)get_img(input_img.width+4,input_img.height+4,sizeof(double));
  img_blue = (double **)get_img(input_img.width+4,input_img.height+4,sizeof(double));

    
  /* set up structure for output color image */
  /* Note that the type is 'c' rather than 'g' */
  get_TIFF ( &color_img, input_img.height, input_img.width, 'c' );//the output img

  for (i = 0;i<input_img.height+4;i++){
    for(j = 0;j<input_img.width+4;j++){
      img_red[i][j] = 0;
      img_green[i][j] = 0;
      img_blue[i][j] = 0;  
    }
  }
  
  for(i = 2;i<input_img.height+2;i++){
    for(j = 2;j<input_img.width+2;j++){
      img_red[i][j] = input_img.color[0][i-2][j-2];
      img_green[i][j] = input_img.color[1][i-2][j-2];
      img_blue[i][j] = input_img.color[2][i-2][j-2];     //create new imgs with boundary filled with 0
    }
  }

  double h = 1.0/25;
  /* Illustration: constructing a sample color image -- interchanging the red and green components from the input color image */
  for ( i = 0; i < input_img.height; i++ ){
      for ( j = 0; j < input_img.width; j++ ) {
    r = 0;
    g = 0;
    b = 0;
    for (k = -2; k < 3; k++ )
    for (l = -2; l < 3; l++ ){
      //take the sum
      r = r + h*img1[i-k+2][j-l+2];
      g = g + h*img2[i-k+2][j-l+2]; 
      b = b + h*img3[i-k+2][j-l+2];  
      } 

    r = ((lamda+1)*img1[i+2][j+2] - lamda*r);
    g = ((lamda+1)*img1[i+2][j+2] - lamda*g);
    b = ((lamda+1)*img1[i+2][j+2] - lamda*b);

      color_img.color[0][i][j] = (int)(r);
      color_img.color[1][i][j] = (int)(g);
      color_img.color[2][i][j] = (int)(b);

      }
}
  
  /* open color image file */
  if ( ( fp = fopen ( "Revised_result_Q4_lamda=1.5.tif", "wb" ) ) == NULL ) {
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
  free_img( (void**)img_red );
  free_img( (void**)img_blue );
  free_img( (void**)img_green );  

  //system£¨"PAUSE"£©;
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
