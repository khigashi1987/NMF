/*
 * learn.h
 */
#ifndef LEARN_H
#define LEARN_H
#include <stdlib.h>
#include "feature.h"

double get_time();
extern void nmf_learn(double **data, int n_rows, int n_cols, int n_class, double **W, double **H, int maxiter);
#endif
