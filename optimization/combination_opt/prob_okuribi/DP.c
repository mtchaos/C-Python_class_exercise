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

struct node {
    double value;// このノードが保持している値
    int a;
    int b;
    struct  node *left_ptr;   // 左枝
    struct  node *right_ptr;  // 右枝
};
// ツリーの頂点 root
static struct node *root = NULL;

int x[100000];   //送り火の位置のx座標
int y[100000];   //送り火の位置のy座標
int size_n = 0;  // 送り火の数(n)
double count = 0; //データ入力のための変数
double start;   //時間計測のための変数
double end;
double time_exe = 0;

void make_path_tree(struct node *,int ,int*,int*,double*,double*);
void make_tree(struct node **, int );

int main(){
    
    //
    //input.txtからデータ入力
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
    
    int a_n = 0;  //(x[n],y[n])が見える最短路の最後の立ち位置
    int b_n = 0;
    double length = 0; //一つの路の組み合わせでの最短路の長さ
    double min_length = 2*(size_n-1)*k + k;  //最短路の長さ
                                             //(初期値は取りうる一番長い路の長さ)
    
    
    //路と立ち位置を記録するための深さn-1の完全二分木を作る
    for(int v = 0; v < size_n;v++){
        make_tree(&root,v);
    }
    
    
    
    start = gettimeofday_sec();
    make_path_tree(root,1,&a_n,&b_n,&length,&min_length);
    
    end = gettimeofday_sec();
    time_exe += (end-start)*1000;
    
 
    
    printf("実行時間は　%lf(ms)\n",time_exe);
    printf("課題１　最短路の長さは %lf で,\n",min_length);
    printf("最短時間は %lf 分\n",5*min_length);
    printf("枝刈りの頻度は　%lf\n",count);
    //結果をファイルに書き込む
    FILE *file_output;
    file_output = fopen("data_kadai2.txt","a");
    if(file_output == NULL) {
        printf("the file can`t be opened\n");
        return -1;
    }
    fprintf(file_output,"%d %lf\n",size_n,count);
    fclose(file_output);
    
    return 0;
}
        

void make_tree(struct node **node, int value)
{
    int  result;    // 数値の大小比較結果
    
    // ノードが存在しない場合
    if ((*node) == NULL) {
        
        // 新しい領域を割り当てノードを作成する
        (*node) = malloc(sizeof(struct node));
        if ((*node) == NULL){
            fprintf(stderr, "NULL");
            exit (8);
        }
        
        // メンバを初期化
        (*node)->left_ptr  = NULL;
        (*node)->right_ptr = NULL;
        (*node)->value     = value;
        (*node)->a         = 0;
        (*node)->b         = 0;
        // make_tree関数を終了
        return;
    }
    
    // テキストから取り出した数値をノードの数値と比較
    // 現在のノードより大きかったら正。小さかったら負。等しかったら0
    if ((*node)->value < value) {
        result = 1;
    } else if ((*node)->value > value) {
        result = -1;
    } else if ((*node)->value == value) {
        result = 0;
    }
    
    // 現在の数値が既にあれば、新たなノードは作成せずmake_tree関数を終了
    if (result == 0)
        return;
    
    // 大きかったら両枝に移動
    if (0 < result) {
        make_tree(&(*node)->right_ptr, value);
        make_tree(&(*node)->left_ptr, value);
    }
}


void make_path_tree(struct node *now,int n,int*a,int *b,double *l,double *min){
    if(n > size_n){
        return;
    }
    
    //左枝で使うように、一個前の値を記憶
    int pre_a = *a;
    int pre_b = *b;
    double pre_l = *l;
    
    //最短路を計算
    *l += abs(x[n] - *a);
    *a = x[n];
    make_path_tree(now->right_ptr,n+1,a,b,l,min);
    
    //ノードに記憶
    now->a = pre_a;
    now->b = pre_b;
    now->value = pre_l;
    
    //左枝で使うように、一個前の値を代入
    *a = pre_a;
    *b = pre_b;
    *l = pre_l;
    
    //見つかってる最短路より大きければ計算しない
    if(now->value >= *min){
        count++;
        return;
    }
    
    //(x[n],y[n])までの最短路を計算
    if(n == size_n){
        if(abs(x[n] - *a) <= abs(y[n]- *b)){
            if(*min > *l + (double)abs(x[n] - *a)){
                *min = *l + (double)abs(x[n] - *a);
            }
        }else{
            if(*min > *l + (double)abs(y[n] - *b)){
                *min = *l + (double)abs(y[n] - *b);
            }
        }
    }
    
    //最短路を計算
    *l += abs(y[n] - *b);
    *b = y[n];
    make_path_tree(now->left_ptr,n+1,a,b,l,min);
    
}
