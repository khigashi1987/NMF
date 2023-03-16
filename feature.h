/*
 * feature.h
 * a header file for feature matrix
 */
#ifndef FEATURE_H
#define FEATURE_H
#include <stdio.h>

typedef struct{
    char **row_headers;
    char **col_headers;
    int n_rows;
    int n_cols;
    double **matrix;
}table;

extern table feature_matrix(char *filename);
extern table invert_matrix(table data);
extern void free_feature_matrix(double **matrix);
#endif
