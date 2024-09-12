#include "matrix.h"

int main(void)
{
	Matrix mat1=matrix_new(2,2);
	matrix_set_elem(mat1,0,0,3.14);
	matrix_set_elem(mat1,1,1,3.14);
	matrix_print(mat1);

	Matrix mat2=matrix_new(2,2);
	matrix_set_elem(mat2,0,0,3.14);
	matrix_set_elem(mat2,1,1,3.14);
	matrix_print(mat2);
	

	Matrix add=matrix_add(mat1,mat2,1,1);
	matrix_print(add);
	matrix_dispose(add);
	return 0;
}
