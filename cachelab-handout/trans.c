/* 
Carson King | SID:109920861
Cameron Johnson | SID: 109667312

 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

//Prototypes
int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void transposeNormal(int M, int N, int A[N][M], int B[M][N]);
void transposeOther(int M, int N, int A[N][M], int B[M][N]);
/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
  
  //Checks the values to determine what kind of transpose should be run 
  switch(M)
    {
    case 32 || 64:
      transposeNormal(M,N,A,B);
      break;
    default:
      transposeOther(M,N,A,B);
    }

}//End of program

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */

//If the m and n values are irregular (not 32x32 or 64x64, it runs this)
char trans_other_desc[] = "Other transpose";
void transposeOther(int M, int N, int A[N][M], int B[M][N]) {
  int block = 16;
  int diagCheck = 0;
  int i,j,k,l,dindex,temp;

  //tranposes the matrix
  for(i = 0; i < N; i += block) {
      for(j = 0; j < M; j += block) {
	    for(k = i; (k < i+block)&&(k<N); k++) {
	      for(l = j; (l < j + block)&&(l<M); l++) {
		    if(k!=l) B[l][k] = A[k][l];
		    else {
		      temp = A[k][l];
		      dindex = k;
		      diagCheck = 1;
		    }

		    }//End of for loop
	    //a check if a diagonal is in temp
	      if(diagCheck == 1) {
		    B[dindex][dindex] = temp;
		    diagCheck = 0;
		  }//End of if
	    }//end of for loop
	  }//End of for loop
    }//end of for loop
}//End of program

//Transposes the matrix normally (32x32, 64x64)
char trans_norm_desc[] = "Normal transpose";
void transposeNormal(int M, int N, int A[N][M], int B[M][N]) {
  int block,i,j,k,l,dindex,temp;
  int diagCheck = 0;

  //If its 32
  if(M == 32) {
      block = 8;
  }//End of if

  else {
    block = 4;
  }//End of if

  for(i = 0; i < N; i += block) {
      for(j = 0; j < M; j += block) {
	    for(k = i; k < i + block; k++) {
	      for(l = j; l < j + block; l++) {
		  //same as the first function checks if the value is diagonal
		    if(k!=l) B[l][k] = A[k][l];
		    else {
		      temp = A[k][l];
		      dindex = k;
		      diagCheck= 1;
		    }//End of if

		  }//End of for loop
	      //checks if the diagonal is in temp
	      if(diagCheck == 1) {
		    B[dindex][dindex] = temp;
		    diagCheck = 0;
		  }//end of if
	    }//End of for loop
	  }//End of for loop
    }//End of for loop

}//End square transpose



char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

