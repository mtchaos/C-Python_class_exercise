#include<stdio.h>
#include<math.h>
//(3) K = 0.1,B = 1 修正オイラー法
//
//    d(y1,y2) = (f1(t,y1,y2),f2(t,y1,y2))
//    dt
//
//    f1(t,y1,y2) = y2
//    f2(t,y1,y2) = -K-y1^3+Bcos(t)

double K = 0.1;
double B = 1;
double cube(double); //3乗
double f1(double,double); //f1(t,y1,y2)
double f2(double,double,double); //f2(t,y1,y2)

int main(){
    
    FILE *gp;
    
    gp = popen("gnuplot -persist","w");
    fprintf(gp, "set xlabel \"t\"\n");    // ラベル表示
    fprintf(gp, "set ylabel \"x\"\n");
    fprintf(gp, "set xrange [20:40]\n"); //各軸の範囲指定
    
    //修正オイラー法
    double pi = M_PI;
    double y1,y2,old_y1,old_y2,DT,t;
    double dt[4] = {2*pi/50,2*pi/100,2*pi/150,2*pi/200};
    
    t = 0;
    DT = dt[3]; //刻み幅dtを設定
    y1 = 3;
    y2 = 0;
    fprintf(gp, "plot '-' with points pointtype 1 title \"Modified Euler Method dt = %lf\"\n",DT);
    
    while(t < 40){
            
        t += DT;
        old_y1 = y1;
        old_y2 = y2;
        y1 = old_y1 + (DT/2.0)*( f1(old_y1,old_y2)+ f1(old_y1+DT*f1(old_y1,old_y2),old_y2+DT*f2(t-DT,old_y1,old_y2)));
        y2 = old_y2 + (DT/2.0)*( f2(t-DT,old_y1,old_y2)+ f2(t,old_y1+DT*f1(old_y1,old_y2),old_y2+DT*f2(t-DT,old_y1,old_y2)));
        
        
        if(20 <= t){
            fprintf(gp,"%f\t%f\n", t, y1);
        }
    }
    
    fprintf(gp,"e\n");
    fprintf(gp, "exit\n"); // gnuplotの終了
    pclose(gp);
    
    return 0;
}

double cube(double x){
    return x*x*x;
}

double f1(double y1,double y2){
    return y2;
}

double f2(double t,double y1,double y2){
    return -K*y2-cube(y1)+B*cos(t);
}
