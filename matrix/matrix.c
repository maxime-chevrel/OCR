#include "matrix.h"

#define MGET matrix_get_elem
#define MSET matrix_set_elem


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
	if(row>=mat.row||col>=mat.col)
		errx(1,"matrix_get_elem	:	Bad Dimension");
	return *(mat.mat+row*mat.col+col);
}
void matrix_set_elem(Matrix mat, size_t row, size_t col, double d)
{
	if(row>=mat.row||col>=mat.col)
		errx(1,"matrix_set_elem	:	Bad Dimension");

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


Matrix matrix_mult(Matrix mat1, Matrix mat2, char dispose1, char dispose2)
{
	if(mat1.col!=mat2.row)
		errx(1,"matrix_mult :	Bad Dimension");
	Matrix res = matrix_new(mat1.row,mat2.col);
	for(size_t i=0;i<res.row;i++)
	{
		for(size_t j=0;j<res.col;j++)
		{
			double result=0;
			for(size_t k=0; k<mat1.col;k++)
			{
				result+=MGET(mat1,i,k)*MGET(mat2,k,j);
			}
			MSET(res,i,j,result);
		}
	}
	if(dispose1)
		matrix_dispose(mat1);
	if(dispose2)
		matrix_dispose(mat2);
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


