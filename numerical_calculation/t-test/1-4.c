/*Box-Muller 法を用いて平均0 = 2、分散2 = ２に従う乱数をn( = 100)
個作るプログラムを作り、その乱数列が平均値２を持つことをt 検定によっ
て検定する。
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double mean = 2.0;
double variation = 2.0;


void Box_Muller(double* n1,double u1,double u2){
    *n1 = mean + sqrt(variation) * sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

double f(double t,double a,double b){
    return pow(t,a-1)*pow(1-t,b-1);
}

int main() {
    long int m = pow(2,32); //乗算法のparameter
    int A = 69621;
    
    long int i = 1;    //乗算法の初期値を1とする
    double rand1,rand2;    //Box-muller法で使う独立な一様乱数
    int n = 100;    //発生させる正規乱数の個数
    int N = 10000;   //シンプソン則の分割数
    double N1[100] = {0};   //発生させた正規乱数を格納
    
    double sum = 0;
    double sample_mean = 0; //発生させた正規乱数の標本平均
    double sample_standard_deviation = 0;  //発生させた正規乱数の標本標準偏差
    double a = (n-1)/2.0;
    double b = (n-1)/2.0;
    double numerator = 0;   //正規化不完全ベーた関数の分子
    double denominator = 0; //正規化不完全ベーた関数の分母
    double h;
    double t;   //t値
    double p;   //p値
    double alpha = 0.05;   //有意水準alpha
    printf("有意水準を%.2lfとする\n",alpha);
    printf("平均%lf、分散%lfの乱数を作る\n",mean,variation);
    
    //box muller法で正規乱数発生してグラフ出力
    for(int j = 0; j < n;j++){
        
        //乗算法で1つ目の一様乱数生成
        i = (A*i);
        i = i % m;
        rand1 = (double)i / (m-1);
        
        //乗算法で２つめの一様乱数生成
        i = (A*i);
        i = i % m;
        rand2 = (double)i / (m-1);
        
        //boxmuller法で正規乱数発生
        Box_Muller(N1 + j,rand1,rand2);
        
        //標本平均を計算
        sample_mean += N1[j]/n;
        
    }
    
    //標本標準偏差を計算
    /*
    for(int i = 0;i < n; i++){
        sample_standard_deviation += pow(N1[i]-sample_mean,2)/(n-1);
    }
    sample_standard_deviation = sqrt(sample_standard_deviation);
    */
    
    
    t = (sqrt(n)*(sample_mean - mean))/(sqrt(variation));
    double x = (t + sqrt(t*t + n -1))/(2*sqrt(t*t + n - 1));
    
    //シンプソン公式で不完全ベータ関数の分母(=ベータ関数)を計算
    h = 1.0/(2*N);
    denominator = h*(f(0,a,b)+ f(1,a,b) + 4*f(1-h,a,b))/3.0;//f(0),f(1),f(2N-1)を最初に足しこむ
    
    for(int j = 1;j <= N-1;j++){
        denominator += (2*f(h,a,b) + f(h + 1.0/(2*N),a,b) )/(3.0*N);
        h += 1.0/N;
    }
    
    //帰無仮説  標本平均 <= 真の値(=平均値 mean) 検証
    //シンプソン公式で不完全ベータ関数の分子を計算
    h = (x - 0.0)/(2*N);
    numerator = h*(f(0,a,b)+ f(x,a,b) + 4*f(x-h,a,b))/3.0;
    
    for(int k = 1;k <= N-1;k++){
        numerator += (x - 0.0)*(2*f(h,a,b) + f(h + (x - 0.0)/(2*N),a,b) )/(3.0*N);
        h += (x - 0.0)/N;
    }
    
    //p値計算
    p = 1 - numerator/denominator;
    
    printf("t値 = %.15eで、p値 = %e\n",t,p);
    
    if(p <= alpha){
        printf("p = %lf <= %lf より標本平均 <= 真の値という帰無仮説を棄却する\n",p,alpha);
        printf("つまり、標本平均 > 真の値という説を採用\n");
    }else{
        printf("p = %lf > %lf より標本平均 <= 真の値という帰無仮説を棄却できない\n",p,alpha);
        printf("また、スチューデントのt分布の対称性より、標本平均 >= 真の値真の値という帰無仮説を棄却できないことになる\n");
    }
    return 0;
}
