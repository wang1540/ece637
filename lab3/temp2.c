

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

struct pixel {
	int m;
	int n;
};

struct pixelList {
	struct pixel pixel;
	struct pixelList *next;
};


void ConnectedNeighbors(
	struct pixel s,
	double T,
	unsigned char **img,
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
	unsigned int **seg,
	int *NumConPixels);

void ConnectedNeighbors(
	struct pixel s,
	double T,
	unsigned char **img,
	int width,
	int height,
	int *M,
	struct pixel c[4]) {
	*M = 0;

	if ((s.n-1) >= 0 && abs(img[s.m][s.n] - img[s.m][s.n-1]) <= T) {
		c[*M].m = s.m;
		c[*M].n = s.n - 1;
		(*M)++;
	}
	if ((s.n+1) < width && abs(img[s.m][s.n] - img[s.m][s.n+1]) <= T) {
		c[*M].m = s.m;
		c[*M].n = s.n + 1;
		(*M)++;
	}
	if ((s.m-1) >= 0 && abs(img[s.m][s.n] - img[s.m-1][s.n]) <= T) {
		c[*M].m = s.m - 1;
		c[*M].n = s.n;
		(*M)++;
	}
	if ((s.m+1) < height && abs(img[s.m][s.n] - img[s.m+1][s.n]) <= T) {
		c[*M].m = s.m + 1;
		c[*M].n = s.n;
		(*M)++;
	}
}

void ConnectedSet(
	struct pixel s,
	double T,
	unsigned char **img,
	int width,
	int height,
	int ClassLabel,
	unsigned int **seg,
	int *NumConPixels) {
	struct pixelList *head, *tail, *tmp;
	struct pixel c[4];
	int M, i;

	(*NumConPixels) = 0;
	head = (struct pixelList *)malloc(sizeof(struct pixelList));
	head->pixel.m = s.m;
	head->pixel.n = s.n;
	head->next = NULL;
	tail = head;

	/****************************************************
     * start with the chosen pixel
     * build a linked list of the pixel that is not yet
     * labeled, the next starting pixel will be the head
     * of the built linked list
     ***************************************************/

	while (head != NULL) {
		if (seg[head->pixel.m][head->pixel.n] != ClassLabel) {
			seg[head->pixel.m][head->pixel.n] = ClassLabel;
			(*NumConPixels)++;
            // find the connected neighbors (not-yet labled)
			ConnectedNeighbors(head->pixel, T, img, width, height, &M, c);

            // go through the neighbors, build the linked list
			for (i = 0; i < M; i++) {
				if (seg[c[i].m][c[i].n] != ClassLabel) {
					tmp = (struct pixelList *)malloc(sizeof(struct pixelList));
					tmp->pixel.m = c[i].m;
					tmp->pixel.n = c[i].n;
					tmp->next = NULL;

					tail->next = tmp;
					tail = tmp;
				}
			}
		}

        // new pixel will the linked list head
		tmp = head->next;
		free(head);
		head = tmp;
	}
}



int main (int argc, char **argv) {
	FILE *fp;
	struct TIFF_img input_img;
	struct pixel s;
	int i, j, numcon, segLabel;
	double T;

	T=3.0;

	
	/* we have 1 segment at the beginning */
	segLabel = 1;

	/* open image file */
	if ((fp = fopen("img22gd2.tif", "rb")) == NULL) {
		fprintf(stderr, "cannot open file %s\n", argv[1]);
		exit(1);
	}

	/* read image */
	if (read_TIFF(fp, &input_img)) {
		fprintf(stderr, "error reading file %s\n", argv[1]);
		exit(1);
	}

	/* close image file */
	fclose(fp);

	/* check the type of image data */
	if (input_img.TIFF_type != 'g') {
		fprintf(stderr, "error:  image must be grayscale\n");
		exit(1);
	}

	/* get the other parameters */

	unsigned int **seg = (unsigned int **)get_img(input_img.width,
                                                  input_img.height,
                                                  sizeof(unsigned int));

    /* make sure all elements are 0 */
    for (i = 0; i < input_img.height; i++) {
		for (j = 0; j < input_img.width; j++) {
			seg[i][j] = 0;
		}
	}
	
    /* go through the image */
	for (i = 0; i < input_img.height; i++) {
		for (j = 0; j < input_img.width; j++) {
			if (seg[i][j] == 0) {
				s.m = i;
				s.n = j;
				ConnectedSet(s, T, input_img.mono, input_img.width,
                             input_img.height, segLabel, seg, &numcon);
				if (numcon > 100) {
                    /* new label created after 100 connected sets */
					segLabel++;
				} else {
                    /* otherwise, back to 0 */
					ConnectedSet(s, T, input_img.mono, input_img.width,
                                 input_img.height, 0, seg, &numcon);
				}
			}
		}
	}
	for (i = 0; i < input_img.height; i++) {
		for (j = 0; j < input_img.width; j++) {
			input_img.mono[i][j] = seg[i][j];
		}
	}

	free_img((void *)seg);

	/* open output image file */
	if ((fp = fopen("segmentation.tif", "wb")) == NULL) {
		fprintf(stderr, "cannot open file output.tif\n");
		exit(1);
	}

	/* write output image */
	if(write_TIFF(fp, &input_img)) {
		fprintf(stderr, "error writing TIFF file %s\n", argv[3]);
		exit(1);
	}

	/* close color image file */
	fclose(fp);

	/* de-allocate space which was used for the images */
	free_TIFF(&(input_img));

	return(0);
}