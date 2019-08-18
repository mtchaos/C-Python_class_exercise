#include<stdio.h>
#include<math.h>
//(5) K = 0.1,B = 12 ルンゲクッタ法
//
//    d(y1,y2) = (f1(t,y1,y2),f2(t,y1,y2))
//    dt
//
//    f1(t,y1,y2) = y2
//    f2(t,y1,y2) = -K-y1^3+Bcos(t)

double K = 0.1;
double B = 12;
double cube(double); //3乗
double f1(double,double); //f1(t,y1,y2)
double f2(double,double,double); //f2(t,y1,y2)

int main(){
    
    FILE *gp;
    
    gp = popen("gnuplot -persist","w");
    fprintf(gp, "set xlabel \"x\"\n");    // ラベル表示
    fprintf(gp, "set ylabel \"dx/dt\"\n");
    fprintf(gp, "plot '-' with points pt 1 title \"Runge-Kutta method\"\n");
    
    double pi = M_PI;
    double y1,y2,old_y1,old_y2,t;
    double g1_y1,g2_y1,g3_y1,g4_y1,g1_y2,g2_y2,g3_y2,g4_y2;
    double DT = 2*pi/1000;
    int count = 0;
    
    t = 0;
    y1 = 3;
    y2 = 0;
    
    while(t < 10000){
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
            count += 1;
            if(count % 1000 == 0){
                fprintf(gp,"%f\t%f\n", y1, y2);
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


