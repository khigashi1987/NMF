# define TESTMATRICIES_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include "testMatricies.h"
#include "feature.h"

void compareMatricies(char* originalFile, char* WFile, char* HFile, float threshold){
    char c;
    int i,j,k;
    table originalData;
    table wData;
    table hData;
    table invertedHData;

    // read files into tables
    originalData = feature_matrix(originalFile);
    hData = feature_matrix(HFile);
    wData = feature_matrix(WFile);
    
    int rowFirst = wData.n_rows;
    int columnFirst = wData.n_cols;
    // Looks like columns and rows are fliped in h, so invert them
    invertedHData = invert_matrix(hData);
    int rowSecond = invertedHData.n_rows;
    int columnSecond = invertedHData.n_cols;

    // create dynamic storage for matrix of wData.n_rows x hData.n_cols
    double **WH;
    WH = (double **)calloc(wData.n_rows,sizeof(double *));
    for(i = 0;i < wData.n_rows;i++){
        WH[i] = (double *)calloc(invertedHData.n_cols,sizeof(double));
    }

    // Multiply W and H to get WH as matrix multiplication
	// Initializing elements of matrix WH to 0.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			WH[i][j] = 0;
		}
	}

	// Multiplying matrix wData and invertedHData and storing in array WH.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			for(k=0; k<columnFirst; ++k)
			{
				WH[i][j] += wData.matrix[i][k] * invertedHData.matrix[k][j];
			}
		}
	}

    // write WH to file called WH.dat
    FILE *fp;
    fp = fopen("WH.dat","w");
    for(i = 0;i < wData.n_rows;i++){
        for(j = 0;j < invertedHData.n_cols;j++){
            fprintf(fp,"%f ",WH[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

    // get rows and columns count of WH
    int n_rows = wData.n_rows;
    int n_cols = invertedHData.n_cols;

    // First check if WH and originalData are the same size - this should be the same!
    if(n_rows != originalData.n_rows || n_cols != originalData.n_cols){
        fprintf(stderr,"Error: WH and originalData are not the same size");

    // Now check if the two matricies are within a certain threshold of each other
    }else{
        int same = 1;
        for(i = 0;i < n_rows;i++){
            for(j = 0;j < n_cols;j++){
                // if the absolute difference between the two values is greater than 0.05, then they are not the same
                if(fabs(WH[i][j] - originalData.matrix[i][j]) > threshold){    
                    //print row and column that is different
                    printf("WH[%d][%d] = %f, originalData[%d][%d] = %f \n",i,j,WH[i][j],i,j,originalData.matrix[i][j]);
                    same = 0;
                }
                // if(WH[i][j] != originalData.matrix[i][j]){
                //     same = 0;
                // }
            }
        }
        if(same){
            printf("WH and originalData are the same");
        }else{
            printf("WH and originalData are not the same");
        }
    }
}