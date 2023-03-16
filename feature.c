/*
 * feature.c
 * an implementation of feature matrix
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "feature.h"
#define BUFFSIZE 65536
#define WORDSIZE 50
static void get_rows(FILE *fp, table *data);
static void get_cols(FILE *fp, table *data);
static void get_data(FILE *fp, table *data);

table feature_matrix(char *filename){
    table data;
    FILE *fp;
    char line[BUFFSIZE];
    
    if((fp = fopen(filename,"r")) == NULL){
        fprintf(stderr,"feature:: cannot open data matrix.\n");
        exit(1);
    }
    
    get_rows(fp, &data);
    get_cols(fp, &data);
    get_data(fp, &data);
    
    fclose(fp);
    return data;
}

static void get_rows(FILE *fp, table *data){
    int n = 0;
    char buff[BUFFSIZE];
    bool firstline = true;
    
    while(NULL != fgets(buff,sizeof(buff),fp)){
        if(firstline){
            firstline = false;
            continue;
        }
        n++;
    }
    rewind(fp);

    (*data).n_rows = n;
    
    int i;
    (*data).row_headers = (char **)calloc((*data).n_rows,sizeof(char *));
    for(i = 0;i < (*data).n_rows;i++){
        (*data).row_headers[i] = (char *)calloc(WORDSIZE,sizeof(char));
    }
    
    firstline = true;
    i = 0;
    while(NULL != fgets(buff,sizeof(buff),fp)){
        if(firstline){
            firstline = false;
            continue;
        }
        char *sp, *lp = buff;
        while(*lp){
            if((sp = strchr(lp,'\t')) == NULL)
                break;
            *sp = '\0';
            strcpy((*data).row_headers[i],lp);
            i++;
            break;
        }
    }
    
    rewind(fp);
}

static void get_cols(FILE *fp, table *data){
    int m = 0;
    char buff[BUFFSIZE];
    bool firstline = true;
    bool firstcolumn = true;
    
    while(NULL != fgets(buff,sizeof(buff),fp)){
        if(firstline){
            char *sp, *lp = buff;
            while(*lp){
                if((sp = strpbrk(lp,"\t\n")) == NULL)
                    break;
                *sp = '\0';
                if(firstcolumn){
                    firstcolumn = false;
                    lp = sp + 1;
                    continue;
                }
                lp = sp + 1;
                m++;
            }
            break;
        }
    } 
    rewind(fp);
    
    (*data).n_cols = m;
    
    int i = 0;
    (*data).col_headers = (char **)calloc((*data).n_cols,sizeof(char *));
    for(i = 0;i < (*data).n_cols;i++){
        (*data).col_headers[i] = (char *)calloc(WORDSIZE,sizeof(char *));
    }
    
    firstline = true;
    firstcolumn = true;
    i = 0;
    while(NULL != fgets(buff,sizeof(buff),fp)){
        if(firstline){
            char *sp, *lp = buff;
            while(*lp){
                if((sp = strpbrk(lp,"\t\n")) == NULL)
                    break;
                *sp = '\0';
                if(firstcolumn){
                    firstcolumn = false;
                    lp = sp + 1;
                    continue;
                }
                strcpy((*data).col_headers[i],lp);
                lp = sp + 1;
                i++;
            }
            break;
        }
    }
    rewind(fp);
}

static void get_data(FILE *fp, table *data){
    int i,j;
    (*data).matrix = (double **)calloc((*data).n_rows,sizeof(double *));
    for(i = 0;i < (*data).n_rows;i++){
        (*data).matrix[i] = (double *)calloc((*data).n_cols,sizeof(double));
    }
    
    bool firstline = true;
    char buff[BUFFSIZE];
    i = 0;
    while(NULL != fgets(buff,sizeof(buff),fp)){
        if(firstline){
            firstline = false;
            continue;
        }
        char *sp, *lp = buff;
        bool firstcolumn = true;
        j = 0;
        while(*lp){
            if((sp = strpbrk(lp,"\t\n")) == NULL)
                break;
            *sp = '\0';
            if(firstcolumn){
                firstcolumn = false;
                lp = sp + 1;
                continue;
            }
            (*data).matrix[i][j] = atof(lp);
            lp = sp + 1;
            j++;
        }
        i++;
    }
}

table invert_matrix(table data) {
    //make rows and row headers columns and column headers
    table matrix;
    matrix.n_rows = data.n_cols;
    matrix.n_cols = data.n_rows;
    matrix.row_headers = (char **)calloc(matrix.n_rows,sizeof(char *));
    matrix.col_headers = (char **)calloc(matrix.n_cols,sizeof(char *));
    matrix.matrix = (double **)calloc(matrix.n_rows,sizeof(double *));
    int i,j;
    for(i = 0;i < matrix.n_rows;i++){
        matrix.row_headers[i] = (char *)calloc(WORDSIZE,sizeof(char));
        strcpy(matrix.row_headers[i],data.col_headers[i]);
        matrix.matrix[i] = (double *)calloc(matrix.n_cols,sizeof(double));
    }
    for(i = 0;i < matrix.n_cols;i++){
        matrix.col_headers[i] = (char *)calloc(WORDSIZE,sizeof(char));
        strcpy(matrix.col_headers[i],data.row_headers[i]);
    }
    for(i = 0;i < matrix.n_rows;i++){
        for(j = 0;j < matrix.n_cols;j++){
            matrix.matrix[i][j] = data.matrix[j][i];
        }
    }

    return matrix;
}
