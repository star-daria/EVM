#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int proizv1(double **matrix, int n, double *x, double *mres);
int proizv2(double **matrix, int n, double *x, double *mres);
int proizv3(double *matrix, int n, double *x, double *mres);
int proizv4(double *matrix, int n, double *x, double *mres);

double Norm1(double *vector, int n);
double Norm2(double *vector, int n);
double Norm(double *vector, int n);

void Gauss1(double **a, double *y, int n);
void Gauss2(double **a, double *y, int n);
void Gauss3(double *a, double *y, int n);
void Gauss4(double *a, double *y, int n);

void a1MGilb2(double **matrix, int n, int m);
void a2MGilb2(double **matrix, int n, int m);
void a3MGilb1(double *matrix, int n, int m);
void a4MGilb1(double *matrix, int n, int m);

void printM(double **matrix, int n);

void dert(double *x, int n);

int main(void)
{
    int  n = 1000, m = 400, i;
    double **a, *b, *x, *mas, *zn, *err, *val, *ost;
    a = (double**)malloc(n*sizeof(double*));
    b = (double*)malloc(n*sizeof(double));

    for (i = 0; i < n; i++) a[i] = (double*)malloc(n*sizeof(double));
    mas = (double*)malloc(n*n*sizeof(double));
    x = (double*)malloc(n*sizeof(double));

    zn = (double*)malloc(n*sizeof(double));
    err = (double*)malloc(n*sizeof(double));
    val = (double*)malloc(n*sizeof(double)); 
    ost = (double*)malloc(n*sizeof(double));
    dert(x, n);

    // тест для хранения в двумерном массиве по строчкам
    a1MGilb2(a, n, m); // матрица Гильберта 
    proizv1(a, n, x, b); // значение b 
    double t;
    t = clock();
    Gauss1(a, b, n); // решение
    t = clock() - t;
    printf("test = %le\n", t/CLOCKS_PER_SEC);
    for (i = 0; i < n; i++) zn[i] = b[i];
    for (i = 0; i < n; i++) err[i] = zn[i] - x[i]; // вектор погрешности
    printf("%le\t%le\t%le\t\n", Norm1(err, n), Norm2(err, n), Norm(err, n)); // нормы
    a1MGilb2(a, n, m); // восстановление 
    proizv1(a, n, x, b);
    proizv1(a, n, zn, val);
    for (i = 0; i < n; i++) ost[i] = val[i] - b[i]; // невязка
    printf("%le\t%le\t%le\t\n", Norm1(ost, n), Norm2(ost, n), Norm(ost, n));

    // тест для хранения матрицы в двумерном по столбцам
    a2MGilb2(a, n, m);
    proizv2(a, n, x, b);
    t = clock();
    Gauss2(a, b, n); 
    t = clock() - t;
    printf("test = %le\n", t/CLOCKS_PER_SEC);
    for (i = 0; i < n; i++) zn[i] = b[i];
    for (i = 0; i < n; i++) err[i] = zn[i] - x[i];
    printf("%le\t%le\t%le\t\n", Norm1(err, n), Norm2(err, n), Norm(err, n));
    a2MGilb2(a, n, m);
    proizv2(a, n, x, b);
    proizv2(a, n, zn, val);
    for (i = 0; i < n; i++) ost[i] = val[i] - b[i];
    printf("%le\t%le\t%le\t\n", Norm1(ost, n), Norm2(ost, n), Norm(ost, n));

    // тест для хранения матрицы в одномерном для строчек
    a3MGilb1(mas, n, m);
    proizv3(mas, n, x, b);
    t = clock();
    Gauss3(mas, b, n);
    t = clock() - t;
    printf("test = %le\n", t/CLOCKS_PER_SEC);
    for (i = 0; i < n; i++) zn[i] = b[i];
    for (i = 0; i < n; i++) err[i] = zn[i] - x[i];
    printf("%le\t%le\t%le\t\n", Norm1(err, n), Norm2(err, n), Norm(err, n));
    a3MGilb1(mas, n, m);
    proizv3(mas, n, x, b);
    proizv3(mas, n, zn, val);
    for (i = 0; i < n; i++) ost[i] = val[i] - b[i];
    printf("%le\t%le\t%le\t\n", Norm1(ost, n), Norm2(ost, n), Norm(ost, n));

    // тест для хранения матрицы в одномерном для столбцов
    a4MGilb1(mas, n, m);
    proizv4(mas, n, x, b);
    t = clock();
    Gauss4(mas, b, n);
    t = clock() - t;
    printf("test = %le\n", t/CLOCKS_PER_SEC);
    for (i = 0; i < n; i++) zn[i] = b[i];
    for (i = 0; i < n; i++) err[i] = zn[i] - x[i];
    printf("%le\t%le\t%le\t\n", Norm1(err, n), Norm2(err, n), Norm(err, n));
    a4MGilb1(mas, n, m);
    proizv4(mas, n, x, b);
    proizv4(mas, n, zn, val);
    for (i = 0; i < n; i++) ost[i] = val[i] - b[i];
    printf("%le\t%le\t%le\t", Norm1(ost, n), Norm2(ost, n), Norm(ost, n));
}


int proizv1(double **matrix, int n, double *x, double *mres)
{
    for (int i = 0; i < n; i++)
	{
        mres[i] = 0.0;
        for (int j = 0; j < n; j++) 
		mres[i] += matrix[i][j] * x[j];
    }
    return 0;
}


int proizv2(double **matrix, int n, double *x, double *mres)
{
    for (int i = 0; i < n; i++)
	{
        mres[i] = 0.0;
        for (int j = 0; j < n; j++)
        mres[i] += matrix[j][i] * x[j];
    }
    return 0;
}


int proizv3(double *matrix, int n, double *x, double *mres)
{
    for (int i = 0; i < n; i++)
	{
        mres[i] = 0.0;
        for (int j = 0; j < n; j++)
        mres[i] += matrix[i*n + j] * x[j];
    }
    return 0;
}


int proizv4(double *matrix, int n, double *x, double *mres)
{
    for (int i = 0; i < n; i++)
	{
        mres[i] = 0.0;
        for (int j = 0; j < n; j++)
        mres[i] += matrix[i + j*n] * x[j];
    }
    return 0;
}


double Norm1(double *vector, int n)
{
    double res = 0.0;
    for (int i = 0; i < n; i++) res += fabs(vector[i]);
    return res;
}

double Norm2(double *vector, int n)
{
    double res = 0.0;
    for (int i = 0; i < n; i++) res += vector[i] * vector[i];
    return sqrt(res);
}

double Norm(double *vector, int n)
{
    double res = 0.0;
    for (int i = 0; i < n; i++) if(fabs(vector[i]) > fabs(res)) res = vector[i];
    return res;
}

void Gauss1(double **a, double *y, int n)
{
    int c, i, j, k;
    double maxa, t;
    for (i = 0; i < n; i++)
    {
        maxa = fabs(a[i][i]);
        c = i;
        for (j = i + 1; j < n; j++)
		{
            if (fabs(a[j][i]) > maxa)
			{
                maxa = fabs(a[j][i]);
                c = j;
			}
        }
        for (k = 0; k < n; k++)
		{
            t = a[i][k];
            a[i][k] = a[c][k];
            a[c][k] = t;
        }
        t = y[i];
        y[i] = y[c];
        y[c] = t;
        y[i] /= a[i][i];
        for (j = n - 1; j >= i; j--) a[i][j] /= a[i][i];
        for (j = i + 1; j < n; j++)
		{
            y[j] -= y[i] * a[j][i];
            for (k = n - 1; k > i; k--) a[j][k] -= a[i][k] * a[j][i];
            a[j][i] = 0;
        }
    }
    for (i = n - 1; i > 0; i--) 
	 for (j = 0; j < i; j++) y[j] -= a[j][i] * y[i];
}

void Gauss2(double **a, double *y, int n)
{
    int c, i, j, k;
    double maxa, t;
    for (i = 0; i < n; i++)
	{
        maxa = fabs(a[i][i]);
        c = i;
        for (j = i + 1; j < n; j++)
		{
            if (fabs(a[i][j]) > maxa)
			{
                maxa = fabs(a[i][j]);
                c = j;
            }
        }
        for (k = 0; k < n; k++)
		{
            t = a[k][i];
            a[k][i] = a[k][c];
            a[k][c] = t;
        }
        t = y[i];
        y[i] = y[c];
        y[c] = t;
        y[i] /= a[i][i];
        for (j = n - 1; j >= i; j--) a[j][i] /= a[i][i];
        for (j = i + 1; j < n; j++)
		{
            y[j] -= y[i] * a[i][j];
            for (k = n - 1; k > i; k--) a[k][j] -= a[k][i] * a[i][j];
            a[i][j] = 0;
        }
    }
    for (i = n - 1; i > 0; i--) 
	 for (j = 0; j < i; j++) y[j] -= a[i][j] * y[i];
}

void Gauss3(double *a, double *y, int n)
{
    int c, i, j, k;
    double maxa, t;
    for (i = 0; i < n; i++)
	{
        maxa = fabs(a[i*n + i]);
        c = i;
        for (j = i + 1; j < n; j++)
		{
            if (fabs(a[j*n + i]) > maxa)
			{
                maxa = fabs(a[j*n + i]);
                c = j;
            }
        }
        for (k = 0; k < n; k++)
		{
            t = a[i*n + k];
            a[i*n + k] = a[c*n + k];
            a[c*n + k] = t;
        }
        t = y[i];
        y[i] = y[c];
        y[c] = t;
        y[i] /= a[i*n + i];
        for (j = n - 1; j >= i; j--) a[i*n + j] /= a[i*n + i];
        for (j = i + 1; j < n; j++)
		{
            y[j] -= y[i] * a[j*n + i];
            for(k = n - 1 ; k > i; k--) a[j*n + k] -= a[i*n + k] * a[j*n+ i];
            a[j*n + k] = 0;
        }
    }
    for (i = n - 1; i > 0; i--)
	 for (j = 0; j < i; j++) y[j] -= a[j*n + i] * y[i];
}


void Gauss4(double *a, double *y, int n)
{
    int c, i, j, k;
    double maxa, t;
    for (i = 0; i < n; i++)
	{
        maxa = fabs(a[i + i * n]);
        c = i;
        for (j = i + 1; j < n; j++)
		{
            if (fabs(a[j + i * n]) > maxa)
			{
                maxa = fabs(a[j + i * n]);
                c = j;
            }
        }
        for (k = 0; k < n; k++)
		{
            t = a[i + k * n];
            a[i + k * n] = a[c + k * n];
            a[c + k * n] = t;
        }
        t = y[i];
        y[i] = y[c];
        y[c] = t;
        y[i] /= a[i + n * i];
        for (j = n - 1 ; j >= i; j--) a[i + n * j] /= a[i + i * n];
        for (j = i + 1; j < n; j++)
		{
            y[j] -= y[i] * a[j + n * i];
            for (int k = n - 1; k > i; k--) a[j + k * n] -= a[i + k * n] * a[j + i * n];
            a[j + i * n] = 0;
        }
    }
    for (i = n - 1; i > 0; i--)
	 for (j = 0; j < i; j++) y[j] -= a[j + n * i] * y[i];
}



void a1MGilb2(double **matrix, int n, int m)
{
    for (int i = 0; i < n; i++) 
		for (int j = 0; j < n; j++)
		{ matrix[i][j] = (i < j + m) ? 1.0 / (i + j + 1.0) : 0.0;}
}


void a2MGilb2(double **matrix, int n, int m )
{
    for (int i = 0; i < n; i++) 
		for (int j = 0; j < n; j++)
		{ matrix[j][i] = (i < j + m) ? 1.0 / (i + j + 1.0) : 0.0;}
}


void a3MGilb1(double *matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{ matrix[i*n + j] = ( i < j + m) ? 1.0 / (i + j + 1.0) : 0.0;}
}


void a4MGilb1(double *matrix, int n, int m)
{
    for (int i = 0; i < n; i++) 
		for (int j = 0; j < n; j++)
		{ matrix[i + j * n] = ( i < j + m) ? 1.0 / (i + j + 1.0) : 0.0;}
}


void printM(double **matrix, int n)
{
    for(int i = 0; i < n; i++)
	{
        for(int j = 0; j < n; j++) printf("%lf ", matrix[i][j]);
        printf("\n");
    }
}

void dert(double *x, int n)
{
    for (int i = 0; i < n; i++)
	{
        if (i % 2 == 0) x[i] = 1;
        else x[i] = -1;
    }
}


