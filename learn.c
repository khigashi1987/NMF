/*
 * learn.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "MT.h"
#include "learn.h"
#include "feature.h"

void nmf_learn(double **data, int n_rows, int n_cols, int n_class, double **W, double **H, int maxiter){
    double **X_hat;
    int i,j;
    
    X_hat = (double **)calloc(n_rows,sizeof(double *));
    for(i = 0;i < n_rows;i++){
        X_hat[i] = (double *)calloc(n_cols,sizeof(double));
    }
    
    // initialize W, H
    init_genrand(time(NULL));
    int rand_max = 100;
    // W(n_rows, n_class)
    for(i = 0;i < n_rows;i++){
        for(j = 0;j < n_class;j++){
            W[i][j] = (double)(genrand_int32() % rand_max) + 1;
        }
    }
    // H(n_class, n_cols)
    for(i = 0;i < n_class;i++){
        for(j = 0;j < n_cols;j++){
            H[i][j] = (double)(genrand_int32() % rand_max) + 1;
        }
    }
    
    // X_hat = W x H
    int k;
    for(i = 0;i < n_rows;i++){
        for(j = 0;j < n_cols;j++){
            X_hat[i][j] = 0.0;
            for(k = 0;k < n_class;k++){
                X_hat[i][j] += W[i][k] * H[k][j];
            }
        }
    }


    FILE *ofp;
    if((ofp = fopen("ISD.txt","w")) == NULL){
        fprintf(stderr,"nmf_learn:: cannnot open output file.\n");
        exit(1);
    }
    fprintf(ofp,"STEP\tISD\n");

    
    // iteration
    int it;
    double numerator;
    double denominator;
    double isd;
    double wh;
    double prev_isd;
    double converge_threshold = 1.0e-12;
    double epsilon = 1.0e-12;
    for(it = 0;it < maxiter;it++){
        printf("iteration %2d / %3d..\n",it+1,maxiter);
        fflush(stdout);
        // update rules for minimizing IS divergence
        // update W
        for(i = 0;i < n_rows;i++){
            for(k = 0;k < n_class;k++){
                if(W[i][k] != 0.0){
                    numerator = 0.0;
                    denominator = 0.0;
                    for(j = 0;j < n_cols;j++){
                        if(X_hat[i][j] == 0.0){
                            numerator += (data[i][j] * H[k][j] / epsilon * epsilon);
                            denominator += H[k][j] / epsilon;
                        }else{
                            numerator += (data[i][j] * H[k][j]) / (X_hat[i][j] * X_hat[i][j]);
                            denominator += H[k][j] / X_hat[i][j];
                        }
                    }
                    if(denominator != 0.0){
                        W[i][k] = W[i][k] * sqrt(numerator / denominator);
                    }else{
                        W[i][k] = W[i][k] * sqrt(numerator / epsilon);
                    }
                    if(W[i][k] < epsilon) W[i][k] = 0.0;
                }
            }
        }
        // update H
        for(k = 0;k < n_class;k++){
            for(j = 0;j < n_cols;j++){
                if(H[k][j] != 0.0){
                    numerator = 0.0;
                    denominator = 0.0;
                    for(i = 0;i < n_rows;i++){
                        if(X_hat[i][j] == 0.0){
                            numerator += (data[i][j] * W[i][k]) / (epsilon * epsilon);
                            denominator += W[i][k] / epsilon;
                        }else{
                            numerator += (data[i][j] * W[i][k]) / (X_hat[i][j] * X_hat[i][j]);
                            denominator += W[i][k] / X_hat[i][j];
                        }
                    }
                    if(denominator != 0.0){
                        H[k][j] = H[k][j] * sqrt(numerator / denominator);
                    }else{
                        H[k][j] = H[k][j] * sqrt(numerator / epsilon);
                    }
                    if(H[k][j] < epsilon) H[k][j] = 0.0;
                }
            }
        }
        // update X_hat
        for(i = 0;i < n_rows;i++){
            for(j = 0;j < n_cols;j++){
                X_hat[i][j] = 0.0;
                for(k = 0;k < n_class;k++){
                    X_hat[i][j] += W[i][k] * H[k][j];
                }
            }
        }
        // compute IS divergence
        isd = 0.0;
        for(i = 0;i < n_rows;i++){
            for(j = 0;j < n_cols;j++){
                wh = 0.0;
                for(k = 0;k < n_class;k++){
                    wh += W[i][k] * H[k][j];
                }
                if((wh < epsilon) && (data[i][j] < epsilon)){
                    isd += 0.0;
                }else if(wh < epsilon){
                    isd += (data[i][j] / epsilon) - log(data[i][j] / epsilon) - 1.0;
                }else if(data[i][j] < epsilon){
                    isd += (epsilon / wh) - log(epsilon / wh) - 1.0;
                }else{
                    isd += (data[i][j] / wh) - log(data[i][j] / wh) - 1.0;
                }
            }
        }
        fprintf(stdout,"\nIS Divergence = %.8f\n",isd);
        fprintf(ofp,"%d\t%.8f\n",it,isd);
        if((it != 0) && (prev_isd < isd)) break;
        if((it != 0) && (fabs(prev_isd - isd) < converge_threshold)){
            printf("converged.\n");
            break;
        }
        prev_isd = isd;
    }
    fclose(ofp);
}
