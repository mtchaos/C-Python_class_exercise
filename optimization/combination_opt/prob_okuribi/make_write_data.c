/*
 問題
 今, ある自然数k に対して[􀀀k; k] に座標を持つ
 ような, (2k + 1)*(2k + 1) の正方格子の原点
 (0,0) に居るとする. この格子の一区画を歩くの
 に1 単位時間かかる. ある順番でn 個の座標ペア
 (xi,yi), -k<xi,yi<k, i = 1,2,...n が
 与えられ, 全ての送り火を与えられた順番で見
 たい.
 
 このプログラムでは、
 コマンドライン引数でn,kを受け取り、-k<xi,yi<k, i = 1,2,...nをランダムに生成。
 input.txtへ以下の形式でn,k,xi,yi (i = 1,2,...n)を書き込む。
 n
 k
 x1 y1
 x2 y2
 ...
 xn yn
 */

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    
    if(argc != 4){
        printf("Usage: inst (n) (k) (seed)\n");
        return(1);
    }
    
    
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    
    srand(atoi(argv[3]));
    
    int i;
    FILE *file;
    file = fopen("input.txt","w");
    
    fprintf(file,"%d\n", n);
    fprintf(file,"%d\n", k);
    
    int a,b;
    for(i = 0; i < n; i++){
        a = rand()%(2*k+1)-k;
        b = rand()%(2*k+1)-k;
        fprintf(file,"%d %d\n", a, b);
        
    }
    fclose(file);

    
}
