#include<stdio.h>
#include<math.h>
//(3) K = 0.1,B = 1 ルンゲクッタ法
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
    
    double pi = M_PI;
    double y1,y2,old_y1,old_y2,DT,t;
    double g1_y1,g2_y1,g3_y1,g4_y1,g1_y2,g2_y2,g3_y2,g4_y2;
    double dt[4] = {2*pi/50,2*pi/100,2*pi/150,2*pi/200};
    
    t = 0;
    DT = dt[3];
    y1 = 3;
    y2 = 0;
        
    while(t < 40){
            
        t += DT;
        old_y1 = y1;
        old_y2 = y2;
            
        g1_y1 = f1(old_y1,old_y2);
        g1_y2 = f2(t-DT,old_y1,old_y2);
            
        g2_y1 = f1(old_y1+(DT/2)*g1_y1,old_y2+(DT/2)*g1_y2);
        g2_y2 = f2(t-DT+DT/2,old_y1+(DT/2)*g1_y1,old_y2+(DT/2)*g1_y2);
            
        g3_y1 = f1(old_y1+(DT/2)*g2_y1,old_y2+(DT/2)*g2_y2);
        g3_y2 = f2(t-DT+DT/2,old_y1+(DT/2)*g2_y1,old_y2+(DT/2)*g2_y2);
            
        g4_y1 = f1(old_y1+DT*g3_y1,old_y2+DT*g3_y2);
        g4_y2 = f2(t-DT+DT,old_y1+DT*g3_y1,old_y2+DT*g3_y2);
            
        y1 = old_y1 + (DT/6.0)*(g1_y1+2*g2_y1+2*g3_y1+g4_y1);
        y2 = old_y2 + (DT/6.0)*(g1_y2+2*g2_y2+2*g3_y2+g4_y2);
        fprintf(gp, "plot '-' with points pointtype 1 title \"Runge-Kutta Method dt = %lf\"\n",DT);
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

