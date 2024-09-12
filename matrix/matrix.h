#ifndef __MATRIX__H__
#define __MATRIX__H__


#include <stdio.h>
#include <stdlib.h>
#include <err.h>

typedef struct {
	size_t row;
	size_t col;
	double *mat;
}Matrix;


/*	return a new matrix	*/
Matrix matrix_new(size_t row, size_t col);

/*	free memory allocated for a matrix	*/
void matrix_dispose(Matrix mat);

/* get the element at the position (row,col)*/
double matrix_get_elem(Matrix mat, size_t row, size_t col);


/* set the element at the position (row,col)*/
void matrix_set_elem(Matrix mat, size_t row, size_t col, double d);

/*	display the matrix	*/
void matrix_print(Matrix mat);

/* Additions of two matrix	*/
//Matrix matrix_add(Matrix mat1, Matrix mat2);
Matrix matrix_add(Matrix mat1, Matrix mat2, char dispose1, char dispose2);

#endif
