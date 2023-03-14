/*
 * nmf.c
 * Nonnegative matrix factorization, main driver.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "nmf.h"
#include "feature.h"
#include "learn.h"
#include "timer.h"

int main(int argc, char **argv){
    table data;
    char c;
    int nclass = CLASS_DEFAULT;
    int maxiter = MAXITER_DEFAULT;
    int i,j;
    double time;
    
    while((c = getopt(argc, argv, "N:I:h")) != -1){
        switch(c){
            case 'N': nclass = atoi(optarg); break;
            case 'I': maxiter = atoi(optarg); break;
            case 'h': usage(); break;
            default: usage(); break;
        }
    }
    if(!(argc - optind == 1)){
        usage();
    }
    
    // open data
    data = feature_matrix(argv[optind]);
    
    fprintf(stdout,"Number of words      %d\n",data.n_rows);
    fprintf(stdout,"Number of samples    %d\n",data.n_cols);
    fprintf(stdout,"Number of classes    %d\n",nclass);
    
    // allocate parameters
    double **W;
    W = (double **)calloc(data.n_rows,sizeof(double *));
    for(i = 0;i < data.n_rows;i++){
        W[i] = (double *)calloc(nclass,sizeof(double));
    }
    double **H;
    H = (double **)calloc(nclass,sizeof(double *));
    for(i = 0;i < nclass;i++){
        H[i] = (double *)calloc(data.n_cols,sizeof(double));
    }
    
    // run NMF
    initialize_timer ();
    start_timer();

    nmf_learn(data.matrix, data.n_rows, data.n_cols, nclass, W, H, maxiter);
    
   /* stop timer */
   stop_timer();
   time=elapsed_time ();

   printf("elapsed time = %lf (sec)\n", time);
       
    // output results
    FILE *wfp, *hfp;
    if((wfp = fopen("W.dat","w")) == NULL){
        fprintf(stderr,"nmf_main:: cannot open output file.\n");
        exit(1);
    }
    if((hfp = fopen("H.dat","w")) == NULL){
        fprintf(stderr,"nmf_main:: cannot open output file.\n");
        exit(1);
    }
    // write W
    fprintf(wfp,"words");
    for(i = 0;i < nclass;i++){
        fprintf(wfp,"\tclass%d",i);
    }
    fprintf(wfp,"\n");
    for(i = 0;i < data.n_rows;i++){
        fprintf(wfp,"%s",data.row_headers[i]);
        for(j = 0;j < nclass;j++){
            fprintf(wfp,"\t%.8f",W[i][j]);
        }
        fprintf(wfp,"\n");
    }
    // write H
    fprintf(hfp,"samples");
    for(i = 0;i < nclass;i++){
        fprintf(hfp,"\tclass%d",i);
    }
    fprintf(hfp,"\n");
    for(i = 0;i < data.n_cols;i++){
        fprintf(hfp,"%s",data.col_headers[i]);
        for(j = 0;j < nclass;j++){
            fprintf(hfp,"\t%.8f",H[j][i]);
        }
        fprintf(hfp,"\n");
    }

    exit(0);
}

void usage(void){
    printf("usage: %s -N classes [-I maxiter] infile\n", "nmf");
    exit(0);
}
