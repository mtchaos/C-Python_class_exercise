問題
今, ある自然数k に対して[-k,k] に座標を持つような, (2k + 1) × (2k + 1) の正方格子の原点(0,0) に居るとする.
この格子の一区画を歩くのに1単位時間かかる. ある順番でn 個の座標ペア(xi,yi), -k < xi, yi < k, i = 1,2,...,n が
与えられ, 全ての送り火を与えられた順番で見たい。送り火を全て見るために歩くのにかかる最短時間はいくらだろうか。そしてプログラムの実行時間はいくらかかるだろうか。

入力形式:
input.txtに次のように保存されている.
n
k
x1 y1
x2 y2
...
xn yn


プログラムの使い方
まず、make_write_data.cを実行して、input.txtに書き込む。そして、再帰で解くならreculsion.cを、動的計画法で解くならDP.cで解く。

実行例
gcc make_write_data.c
./a.out 10 50 22
gcc DP.c
