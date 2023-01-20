#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix{
    double **matrix;
    int row;
    int column;
}Matrix;

Matrix transpose(Matrix x){
    //create result
    Matrix result;
    result.row = x.column;
    result.column = x.row;
    result.matrix = malloc((result.row)*sizeof(double*));
    for(int i = 0;i < result.row;++i){
        result.matrix[i] = malloc((result.column)*sizeof(double));
    }

    //tranpose
    for(int i = 0;i < result.row;i++){
        for(int j = 0;j < result.column;j++){
            result.matrix[i][j] = x.matrix[j][i];
        }
    }
    return result;
}

Matrix mexp(Matrix x1,Matrix x2){
    if(x1.column == x2.row){
        Matrix result;
        result.row = x1.row;
        result.column = x2.column;
        result.matrix = malloc((result.row)*sizeof(double*));
        for(int i = 0;i < result.row;++i){
            result.matrix[i] = malloc((result.column)*sizeof(double));
        }
        double sum = 0;
        for(int i = 0; i < x1.row;i++){
            for(int j = 0;j < x2.column;j++){
                for(int k = 0;k < x1.column;k++){
                    sum += x1.matrix[i][k] * x2.matrix[k][j];
                } 
                result.matrix[i][j] = sum;
                sum = 0;
            }
        }
        return result;
    }
    else{
        printf("matrices cannot be multiplied due to matrix 1 col does not equal matrix 2 row");
        exit(EXIT_FAILURE);
    }
    
}

Matrix inverse(Matrix x){
    if(x.row == x.column){
        //create identity matrix
        Matrix identity;
        identity.row = x.row;
        identity.column = x.column;
        identity.matrix = malloc((identity.row)*sizeof(double*));
        for(int i = 0;i < identity.row;++i){
            identity.matrix[i] = malloc((identity.column)*sizeof(double));
        }

        for(int i = 0;i < identity.row;i++){
            for(int j = 0;j < identity.column;j++){
                if(i == j){
                    identity.matrix[i][j] = 1;
                }
                else{
                    identity.matrix[i][j] = 0;
                }
                    
            }
        }

        //doing the inverse
        //for p ←0, 1, ··· , n −1 do
        for(int p = 0; p < x.row;p++){
            //f <--- Mp,p
            double f = x.matrix[p][p];
            //divide Mp by f
            //divide Np by f
            for(int m = 0;m < x.column;m++){
                x.matrix[p][m] = x.matrix[p][m] / f;
                identity.matrix[p][m] = identity.matrix[p][m] / f;
            }
            //for i ←p + 1, ··· , n −1 do
            for(int i  = p+1;i < x.row;i++){
                //f ←Mi,p
                f = x.matrix[i][p];
                //subtract Mp ×f from Mi
                //subtract Np ×f from Ni
                for(int m = 0;m < x.column;m++){
                    x.matrix[i][m] = x.matrix[i][m] -  (x.matrix[p][m] * f);
                    identity.matrix[i][m] = identity.matrix[i][m] - (identity.matrix[p][m] * f);
                }
            }
        }
        //for p ←n −1, ··· , 0 do
        for(int p = x.row - 1; p >= 0;p--){
            //for i ←p −1, ··· , 0 do
            for(int i = p-1; i >=0;i--){
                //f ←Mi,p
                double f = x.matrix[i][p];
                //subtract Mp ×f from Mi
                //subtract Np ×f from Ni
                for(int m = 0;m < x.column;m++){
                    x.matrix[i][m] = x.matrix[i][m] -  (x.matrix[p][m] * f);
                    identity.matrix[i][m] = identity.matrix[i][m] - (identity.matrix[p][m] * f);
                }

            }

        }
        return identity;
    }
    else{
        printf("matrix is not square");
        exit(EXIT_FAILURE);
    }

}

int main(int argc, char **argv){
    //train
    FILE *train;
    int kt,nt;

    //open train file
    train = fopen(argv[1],"r");

    //remove string "train"
    fscanf(train,"%*s");

    //get k(columns of x)(number of attributes)(cols)
    fscanf(train, "%d",&kt);


    //get n(num of houses)(rows)
    fscanf(train, "%d", &nt);

    //create x
    Matrix x;
    x.row = nt;
    x.column = kt+ 1;
    x.matrix = malloc((x.row)* sizeof(double*));
    for(int i = 0;i < x.row;++i){
        x.matrix[i] = malloc((x.column)*sizeof(double));
    }

    //create y
    Matrix y;
    y.row = nt;
    y.column = 1;
    y.matrix = malloc((y.row)*sizeof(double*));
    for(int i = 0;i < y.row;++i){
        y.matrix[i] = malloc(y.column*sizeof(double));
    }

    //fill x and y
    //rows(n-1)
    for(int i = 0; i < nt;i++){
        //col 0
        x.matrix[i][0] = 1;
        //col 1 -> kt
        for(int j = 1;j <= kt;j++){
            fscanf(train, "%lf",&x.matrix[i][j]);
        }
        //col kt+1
        fscanf(train, "%lf",&y.matrix[i][0]);
    }

    //find W
    //transpose of x
    Matrix xt = transpose(x);

    //multiply transpose of xt * x
    Matrix resxtx = mexp(xt,x);

    //inverse of (xt and x)
    Matrix invxtx = inverse(resxtx);

    //multiply inverse of (xt and x) by transpose of x
    Matrix invxtxxt = mexp(invxtx,xt);

    //multiply by y
    Matrix w = mexp(invxtxxt,y);

    //W will be used to get Y from x of data
    //get data
    FILE *data;
    int kd,m;

    data = fopen(argv[2],"r");

    fscanf(data,"%*s");

    //get k(columns of x)(number of attributes)
    fscanf(data, "%d",&kd);

    //get n(num of houses)(rows)
    fscanf(data, "%d", &m);

    //create x
    Matrix xp;
    xp.matrix = malloc(m*sizeof(double*));
    xp.row = m;
    xp.column = kd+1;
    for(int i = 0;i < xp.row;++i){
        xp.matrix[i] = malloc((xp.column)*sizeof(double));
    }

    //store x
    for(int i = 0;i < xp.row;i++){
        xp.matrix[i][0] = 1;
        for(int j = 1; j < xp.column;j++){
            fscanf(data,"%lf",&xp.matrix[i][j]);
        }
    }

    //multiply W with X
    Matrix fin = mexp(xp,w);

    //print out y
    for(int i = 0;i < fin.row;i++){
        for(int j = 0; j < fin.column;j++){
            printf("%.0f\n", fin.matrix[i][j]);
        }
    }

    //freeing 
    for(int i  = 0;i < x.row; ++i){
        free(x.matrix[i]);
    }
    free(x.matrix);
    for(int i  = 0; i < y.row;++i){
        free(y.matrix[i]);
    }
    free(y.matrix);

    for(int i = 0;i < xt.row;++i){
        free(xt.matrix[i]);
    }
    free(xt.matrix);

    for(int i = 0; i < resxtx.row;++i){
        free(resxtx.matrix[i]);
    }
    free(resxtx.matrix);

    for(int i =  0;i < invxtx.row;++i){
        free(invxtx.matrix[i]);
    }
    free(invxtx.matrix);

    for(int i =  0;i < invxtxxt.row;++i){
        free(invxtxxt.matrix[i]);
    }
    free(invxtxxt.matrix);

    for(int i =  0;i < w.row;++i){
        free(w.matrix[i]);
    }
    free(w.matrix);

    for(int i =  0;i < xp.row;++i){
        free(xp.matrix[i]);
    }
    free(xp.matrix);

    for(int i =  0;i < fin.row;++i){
        free(fin.matrix[i]);
    }
    free(fin.matrix);

    //close files
    fclose(train);
    fclose(data);
    
    return EXIT_SUCCESS;
}