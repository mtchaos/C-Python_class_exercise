/*要素が全て自然数a以下の自然数列b1,b2,...,bnの部分和の中で合計が最大のものを動的計画法を用いて解く。
 以下の漸化式でx(j,k)を決定。x(j,0) = 1,x(0,k)=1であるとする。
x(j,k) = 1 if x(j-1,k) = 1,
         1 if k-bj ≧ 0 かつ x(j-1,k-bj) = 1,
         0 otherwise
部分和問題の最適値はx(n,k) = 1 となる最大のkである。
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

double gettimeofday_sec(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

int main(int argc, char *argv[]) {
    
    int i,n,a,b_size,s,t,l,seed;
    if (argc != 4){
        printf("Usage ./a.out (a) (n) (seed)\n");
        return 0;
    }
    a = atoi(argv[1]);   //自然数列bの上限。bi <= a
    n = atoi(argv[2]);    //自然数列bのサイズ
    seed = atoi(argv[3]);   //乱数の種
    
    //自然数列bをrandomに生成
    int b[n+1];
    srand(seed);
    for(i = 0; i < n; i++){
        b[i+1] = rand()%(a+1);
    }
    /* 時間計測開始*/
    double start = gettimeofday_sec();
    /* 初期設定 */
    int c = 0;
    int x[n+1][a+1]; //x(j,k)
    int j,k;
    for(j = 0;j <= n;j++){
        x[j][0] = 1;
    }
    for(k = 1;k <= a;k++){
        x[0][k] = 0;
    }
    j = 1;
    while (j <= n){
        k = 1;
        while (k <= a){
            if( x[j-1][k] == 1 ){
                x[j][k] = 1;
            }else if( k - b[j] >= 0 && x[j-1][k-b[j]] == 1){
                x[j][k] = 1;
            }else{
                x[j][k] = 0;
            }
            k++;
        }
        j++;
    }
    k = 0;
    c = 0;
    while( k <= a){
        if( x[n][k] == 1 ){
            c = k;
        }
        k++;
    }
    printf("optimal value = %d\n",c);
    //最適解求める
    int p[c + 1][2];
    p[0][0] = 0;
    for(i = 1;i < c+1;i++){
        p[i][0] = -1;
    }
    for(i = 0;i < c+1;i++){
        p[i][1] = 0;
    }
    for(i = 1;i < n+1;i++){
        s = b[i];
        for(l = c;l >= 0;l--){
            if(p[l][0] != -1){
                t = l;
                if(t + s <= c && p[s + t][0] == -1 ){
                    p[s+t][0] = s;
                    p[s+t][1] = i;
                    if(p[c][0] != -1){
                        goto loopend;
                    }
                }
            }
        }
    }
    loopend:
    printf("optimal solution = ( ");
    i = c;
    while( p[i][0] != 0){
        printf("b[%d] ",p[i][1]);
        i = i - p[i][0];
    }
    printf(")\n");
    /* 時間計測終了*/
    double end = gettimeofday_sec();
    printf("execution time = %lf\n",(end-start)*1000);
    return 0;
}
