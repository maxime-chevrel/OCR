#include "matrix.h"

#define MGET matrix_get_elem


Matrix matrix_new(size_t row, size_t col)
{
	Matrix mat;
	mat.row=row;
	mat.col=col;
	mat.mat=calloc(row*col,sizeof(double));
	return mat;
}

void matrix_dispose(Matrix mat)
{
	free(mat.mat);
}



double matrix_get_elem(Matrix mat, size_t row, size_t col)
{
	return *(mat.mat+row*mat.col+col);
}
void matrix_set_elem(Matrix mat, size_t row, size_t col, double d)
{
	 *(mat.mat+row*mat.col+col)=d;
}


Matrix matrix_add(Matrix mat1, Matrix mat2, char dispose1, char dispose2)
{
	if(mat1.row!=mat2.row||mat1.col!=mat2.col)
		errx(1,"matrix_add :	Bad Dimension");
	Matrix res=matrix_new(mat1.row,mat2.col);
	for(size_t i=0;i<mat1.row;i++)
	{
		for(size_t j=0;j<mat1.col;j++)
		{
			matrix_set_elem(res,i,j,MGET(mat1,i,j)+MGET(mat2,i,j));
		}
	}
	if(dispose1)
	{
		matrix_dispose(mat1);
	}
	if(dispose2)
	{
		matrix_dispose(mat2);
	}
	return res;
}





void matrix_print(Matrix mat)
{
	for(size_t i=0;i<mat.row;i++)
	{
		for(size_t j=0;j<mat.col;j++)
		{
			printf("%f	",matrix_get_elem(mat,i,j));
		}
		printf("\n");
	}
	printf("\n");
}


