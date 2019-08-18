//M3(2)
// J = 1,T = 2/log(sqrt(2) + 1)。
//マルコフ連鎖モンテカルロ法のサンプル列内の相関を見る為に、初期状態の影響が消える様子を見る。
//トーラス状の二次元強磁性イジングモデルを熱浴法でシュミレーションして、磁化を計算する。

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    int L = 64;
    double T = 2.0/log(sqrt(2)+1);
    double randomNum; //random number between 0 and 1
    double P;   //次の状態を選ぶための条件付き確率
    int sum_s = L*L;
    double m = 1.0;   //磁化m
    double tmp;
    int spin[L][L]; //格子状のspinの状態
    unsigned int seed[4];
    int i,j,l;
    int mcs = 200;
    
    FILE *gp;
    
    gp = popen("gnuplot -persist","w");
    fprintf(gp, "set xlabel \"mcs\"\n");    // ラベル表示
    fprintf(gp, "set ylabel \"m\"\n");
    fprintf(gp, "plot '-' with points pointtype 1 title \"Gibbs Sampling Ising Model L = %d\"\n",L);
    
    for(i = 0; i < L; i++){
        for(j = 0; j < L; j++){
            spin[i][j] = 1;
        }
    }
    fprintf(gp,"%d\t%f\n", 0, m);
    
    //モンテカルロ計算
    
    seed[0] = (unsigned int)(time(NULL));
    seed[1] = (unsigned int)(time(NULL)+1);
    seed[2] = (unsigned int)(time(NULL)+33);
    seed[3] = (unsigned int)(time(NULL)+2);
            
    for(l = 0;l < mcs;l++){
        for(i = 0;i < L;i++){
            for(j = 0;j< L;j++){
                    
                tmp = (spin[f(i+1,L)][j]+spin[f(i-1,L)][j]+spin[i][f(j+1,L)]+spin[i][f(j-1,L)])/T;
                P = (1 + tanh(tmp))/2.0;
                
                randomNum = ur(seed);
                        
                if(randomNum < P){
                    if(spin[i][j] == -1){
                        sum_s += 2;
                        spin[i][j] = 1;
                    }
                }else{
                    if(spin[i][j] == 1){
                        sum_s -= 2;
                        spin[i][j] = -1;
                    }
                }
                        
            }
        }
        m = (double)abs(sum_s)/(L*L);
        fprintf(gp,"%d\t%f\n", l+1, m);
        
    }
    
    //出力
    fprintf(gp,"e\n");
    fprintf(gp, "exit\n"); // gnuplotの終了
    pclose(gp);
    
    return 0;
}

