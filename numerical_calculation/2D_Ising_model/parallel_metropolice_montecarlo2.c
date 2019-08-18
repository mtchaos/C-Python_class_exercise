// J = 1,T = 2/log(sqrt(2) + 1)。トーラス状の二次元強磁性イジングモデルをメトロポリス法でシュミレーションして磁化を計算する。OpenMp M3(3)用

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int f(int x,int L){
    if(x == -1){
        return L-1;
    }else if(x == L){
        return 0;
    }else{
        return x;
    }
}

void init_xor128(unsigned int s0, unsigned int *s) {
    int i = 0;
    for (i = 0; i < 4; ++i){
        s0 = 1812433253U * ( s0 ^ ( s0 >> 30 ) ) + i;
        s[i] = s0;
    }
}

unsigned int xor128(unsigned int *s) {
    unsigned int t = ( s[0] ^ ( s[0] << 11 ) );
    s[0] = s[1];
    s[1] = s[2];
    s[2] = s[3];
    s[3] = ( s[3] ^ ( s[3] >> 19 ) ) ^ ( t ^ ( t >> 8 ) );
    return s[3];
}

double ur(unsigned int *s){
    return ((double) xor128(s))/4294967296e0;
}

int main(int argc,char const * argv[]){
    
    //変数宣言
    double start = omp_get_wtime();
    
    int L;
    if (argc == 1 || argc >= 3){ /* 引数なし or 引数3つ以上 */
        printf("引数に整数を1ついれてください。\n");
        return 1;
    }
    else{
        L = atoi(*++argv);
    }
    
    double z = sqrt(2);
    double T = 2.0/log(z + 1.0);
    double randomNum; //random number between 0 and 1
    double P;   //次の状態を選ぶための条件付き確率
    int sum_s = L*L;  //sum s_i
    double m = 0.0;   //磁化m
    double estimatedValue = 0;  //推定値
    double accuracy = 0.0;  //精度
    int spin[L][L]; //格子状のspinの状態
    unsigned int seed[4];
    int i,j,k,l,n;
    int s = 0;
    int mcs = (int)pow(10,5);
    n = omp_get_max_threads();
    double mean_m[n];
    
    //初期状態は全て１とする
    for(i = 0; i < L; i++){
        for(j = 0; j < L; j++){
            spin[i][j] = 1;
        }
    }
    
    
    //モンテカルロ計算
    #pragma omp parallel for private(l,i,j,P,randomNum,seed) firstprivate(sum_s,spin,m) num_threads(n)
    for(s = 0;s < n; s++){
        
        seed[0] = (unsigned int)(s+time(NULL));
        seed[1] = (unsigned int)(s+time(NULL)+1);
        seed[2] = (unsigned int)(s+time(NULL)+2);
        seed[3] = (unsigned int)(s+time(NULL)+3);
            
    
        for(l = 0;l < mcs;l++){
            for(i = 0;i < L;i++){
                for(j = 0;j< L;j++){
                        
                    P = exp( -2.0*spin[i][j]*(spin[f(i+1,L)][j]+spin[f(i-1,L)][j]+spin[i][f(j+1,L)]+spin[i][f(j-1,L)])/T );
                        
                    randomNum = ur(seed);
                        
                    if(randomNum <= P){
                        if(spin[i][j] == -1){
                            sum_s += 2;
                        }else{
                            sum_s -= 2;
                        }
                        spin[i][j] = -1 * spin[i][j];
                            
                    }
                        
                }
            }
            if( l >= 1000){
                m += (double)abs(sum_s)/(L*L*(mcs-1000));
            }
        }
        mean_m[s] = m;
    }
    
    //推定値と精度の計算
    for(i = 0;i < n;i++){
        estimatedValue += mean_m[i]/n;
    }
    
    for(i = 0; i< n;i++){
        accuracy += pow(mean_m[i] - estimatedValue,2)/(n*(n-1));
    }
    
    //出力
    double elapsed_time = omp_get_wtime() -start;
    printf("time = %lf\n",elapsed_time);
    printf("estimated value = %lf,accuracy = %lf",estimatedValue,accuracy);
    
    return 0;
}
