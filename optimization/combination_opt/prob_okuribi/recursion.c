#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>

double gettimeofday_sec(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec+ (double)tv.tv_usec*1e-6;
}

int x[100000];   //送り火の位置のx座標
int y[100000];   //送り火の位置のy座標
int size_n = 0;  // 送り火の数(n)
void decimal_to_binary(int,char []);  //10進数を2進数にする関数
void find_shortest_path(int,int*,int*,double*,char c[]); //最短路を求める再帰関数


int main(){
    
    //
    //input.txtからデータの入力
    //
    int input[100000];
    int ret;
    int in_a,in_b;
    
    FILE *file_input;
    file_input = fopen("input.txt","r");
    if(file_input == NULL) {
        printf("the file can`t be opened\n");
        return -1;
    }
    
    int m = 0;
    while(( ret = fscanf( file_input , "%d %d" , &in_a , &in_b )) != EOF ) {
        input[m] = in_a;
        m++;
        input[m] = in_b;
        m++;
    }
    fclose(file_input);
    // end input
    
    size_n = input[0];
    int k = input[1];// (2k+1)x(2k+1)の格子のサイズ
    
    //送り火の位置の入力
    for(int v = 2; v < m; v++){
        if(v % 2 == 0){
            x[v / 2] = input[v];
        }else{
            y[ (v-1)/2 ] = input[v];
        }
    }
    
    //
    //データの入力終了
    //
    
    int a_n = 0;  //(x[n],y[n])が見える最短路の最後の立ち位置、アドレスを渡すのでとりあえず０で初期化
    int b_n = 0;
    double length = 0; //ある時点での最短路の長さ、アドレスを渡すのでとりあえず0で初期化
    double min_length = 2*(size_n-1)*k + k;  //最短路の長さ(初期値は取りうる一番長い路の長さ)
    
    
    char path_order[size_n - 1];    //総当たり法の組み合わせを二進数で表現。path_order = 101ならば、y移動→x移動→y移動
    
    //path_order を初期化
    for(int v = 0; v < size_n - 1; v++){
        path_order[v] = '0';
    }
    
    double start;   //時間計測のための変数
    double end;
    double time_exe = 0;
    
    //総当たり法を実行する、ただし最後の立ち位置から一個前(n-1)の時は、x移動、y移動の二つの選択肢から最短路が短くなる方を選ぶ。
    //だからn-1回しか回さない。
    for(int v = 0; v < pow(2,size_n - 1); v++){
        
        decimal_to_binary(v,path_order);
        
        
        start = gettimeofday_sec();
        
        find_shortest_path(size_n,&a_n,&b_n,&length,path_order);
        
        
        if(min_length > length){
            min_length = length;
        }
        
        
        end = gettimeofday_sec();
        time_exe += (end-start)*1000;
        
    }
    
    printf("実行時間は　%lf(ms)\n",time_exe);
    printf("課題１　最短路の長さは %lf で、最短時間は %lf 分\n",min_length,5*min_length);
    
    //結果をファイルに書き込む
    FILE *file_output;
    file_output = fopen("data_kadai1-1.txt","a");
    if(file_output == NULL) {
        printf("the file can`t be opened\n");
        return -1;
    }
    fprintf(file_output,"%d %lf\n",size_n,time_exe);
    fclose(file_output);
    
    return 0;
}


//*l は(x[n],y[n])が見える最短路の長さ
//（*a,*b）は(x[n],y[n])が見える最短の位置
//
void find_shortest_path(int n ,int *a,int *b,double *l,char path_order[]){
    
    if(n == size_n){
        find_shortest_path(n-1,a,b,l,path_order);
    
        //一個前の値を記憶
        int pre_a = *a;
        int pre_b = *b;
        double pre_l = *l;
        
        //(x[n],y[n])までの最短路を計算
        if(abs(x[n] - pre_a) <= abs(y[n]- pre_b)){
            *a = x[n];
            *b = pre_b;
            *l += (double)abs(x[n] - pre_a);
        }else{
            *a = pre_a;
            *b = y[n];
            *l += (double)abs(y[n] - pre_b);
        }
    }else if(n != 0){
        find_shortest_path(n-1,a,b,l,path_order);
        
        //一個前の値を記憶
        int pre_a = *a;
        int pre_b = *b;
        double pre_l = *l;
        end:
        //最短路を計算
        if(path_order[n-1] == '0'){
            *a = x[n];
            *b = pre_b;
            *l += (double)abs(x[n] - pre_a);
        }else if(path_order[n-1] == '1'){
            *a = pre_a;
            *b = y[n];
            *l += (double)abs(y[n] - pre_b);
        }
    }else{
        //初期点
        *a = 0;
        *b = 0;
        *l = 0.0;
    }
}

void decimal_to_binary(int decimal,char a[]){
    double binary = 0;
    double base = 1;
    int count = 0;
    while(decimal>0){
        a[count] = '0' + decimal % 2;
        binary = binary + ( decimal % 2 ) * base;
        decimal = decimal / 2;
        base = base * 10;
        count++;
    }
    
}
