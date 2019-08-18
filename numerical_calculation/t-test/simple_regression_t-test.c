//データ(Xi; Yi) (i = 1,...,n) が与えられたとき, 単回帰Yi = bXi + aの
//回帰係数とを求めるプログラムを作成し, 回帰係数についてt 検定を行うためのp 値を求める.
//このプログラムが正しい結果を導いていることを,乱数を用いたデータ生成モデルを用いて確認する.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double mu = 3.0;
double sigma = 0.5;

void Box_Muller(double* n1,double* n2,double u1,double u2){
    *n1 = mu + sigma * pow(-2.0 * log(u1),0.5) * sin(2.0 * M_PI * u2);
    *n2 = mu + sigma * pow(-2.0 * log(u1),0.5) * cos(2.0 * M_PI * u2);
}

double f(double t,double a,double b){
    return pow(t,a-1)*pow(1-t,b-1);
}

int main() {
    int n = 100;
    int N = 10000;  //シンプソン則の刻む数
    double xSample[100] = {0};
    double ySample[100] = {0};
    long int m = pow(2,32); //乗算法のparameter
    int A = 69621;
    long int i = 1;    //乗算法の初期値を1とする
    double rand1,rand2;    //Box-muller法で使う(0,1]の独立な一様乱数
    double sum = 0;
    double x_SampleMean = 0;
    double y_SampleMean = 0;
    double SD_x = 0;    //xの標準偏差
    double SE_regression = 0;   //回帰の標準誤差
    double alpha = 0;   //y = alpha + beta*x
    double beta = 0;    //y = alpha + beta*x
    double beta_numerator = 0;  //yの分子
    double beta_denominator = 0;    //yの分母
    double a = (n-2)/2.0;
    double b = (n-2)/2.0;
    double IBF_numerator = 0;   //不完全ベータ関数の分子
    double IBF_denominator = 0;     //不完全ベータ関数の分母
    double h;
    double t = 0;   //t値
    double p;   //p値
    double SignigicenseLevel = 0.05;   //有意水準
    printf("有意水準を%.2lfとする\n",SignigicenseLevel);
    
    
    //グラフ出力用
    FILE *gp;
    gp = popen("gnuplot -persist","w");
    fprintf(gp, "set xlabel \"x\"\n");
    fprintf(gp, "set ylabel \"y\"\n");
    fprintf(gp, "set xrange [-5:10]\n");
    fprintf(gp, "set yrange [-5:10]\n");
    
    //box muller法で乱数発生
    srand((unsigned int)time(NULL)); //乱数のタネの初期化
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
        Box_Muller(xSample + j,ySample + j,rand1,rand2);
        x_SampleMean += xSample[j]/n;
        y_SampleMean += ySample[j]/n;
        
    }
    
    //betaとxの標準偏差計算
    for(int j = 0; j < n;j++){
        beta_numerator += (ySample[j] - y_SampleMean)*(xSample[j] - x_SampleMean);
        beta_denominator += pow(xSample[j] - x_SampleMean,2);
        SD_x = pow(xSample[j] - x_SampleMean,2);
    }
    beta = beta_numerator/beta_denominator;
    alpha = y_SampleMean - beta*x_SampleMean;
    SD_x = sqrt(SD_x);
    
    //回帰の標準誤差計算
    for(int j = 0; j < n;j++){
        SE_regression += pow(ySample[j] - (alpha + beta*xSample[j]),2)/(n-2);
    }
    
    
    //t計算
    t = beta/(SE_regression/SD_x);
    double x = (t + sqrt(t*t + n -2))/(2*sqrt(t*t + n - 2));

    
    //シンプソン公式で不完全ベータ関数の分母(=ベータ関数)を計算
    h = 1.0/(2*N);
    IBF_denominator = h*(f(0,a,b)+ f(1,a,b) + 4*f(1-h,a,b))/3.0;//f(0),f(1),f(2N-1)を最初に足しこむ
    
    for(int j = 1;j <= N-1;j++){
        IBF_denominator += (2*f(h,a,b) + f(h + 1.0/(2*N),a,b) )/(3.0*N);
        h += 1.0/N;
    }
    
    //シンプソン公式で不完全ベータ関数の分子を計算
    h = (x - 0.0)/(2*N);
    IBF_numerator = h*(f(0,a,b)+ f(x,a,b) + 4*f(x-h,a,b))/3.0;
    
    for(int k = 1;k <= N-1;k++){
        IBF_numerator += (x - 0.0)*(2*f(h,a,b) + f(h + (x - 0.0)/(2*N),a,b) )/(3.0*N);
        h += (x - 0.0)/N;
    }
    
    p = 1 - IBF_numerator/IBF_denominator;
    
    printf("t値 = %lfで、p値 = %lf\n",t,p);
    
    if(p <= SignigicenseLevel){
        printf("p = %lf <= %lf より回帰係数beta <= 0という帰無仮説を棄却する\n",p,SignigicenseLevel);
        printf("つまり、回帰係数beta > 0という説を採用\n");
    }else{
        printf("p = %lf > %lf より回帰係数beta <= 0という帰無仮説を棄却できない\n",p,SignigicenseLevel);
        printf("また、スチューデントのt分布の対称性より、回帰係数beta >= 0という帰無仮説も棄却できないことになる\n");
    }
    
    //グラフ出力
    fprintf(gp,"plot '-'\n");
    for(int j = 0; j < n; j++){
        fprintf(gp,"%lf %lf\n", xSample[j], ySample[j]);
    }
    fprintf(gp,"e\n");
    fprintf(gp, "exit\n"); // gnuplotの終了
    pclose(gp);

    
    return 0;
}

