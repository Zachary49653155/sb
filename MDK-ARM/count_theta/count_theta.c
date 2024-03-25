#include "count_theta.h"
#include "printf.h"
#include "math.h"

extern double x0;
extern double y0;
extern double m;
#define PI 3.14159265
extern double a1,a2,a3,angle1,angle2,angle3;
extern double theta1,theta2,theta3,theta4;
void thetacount(void)
{
	  //计算出x0和y0
    if(USART1_RxBuffer[1] == '0')
    {
        x0 = (USART1_RxBuffer[2]-48)*100+(USART1_RxBuffer[3]-48)*10+(USART1_RxBuffer[4]-48)+(USART1_RxBuffer[5]-48)*0.1;
    }
    else if(USART1_RxBuffer[1] == '1')
    {
        x0 = -((USART1_RxBuffer[2]-48)*100+(USART1_RxBuffer[3]-48)*10+(USART1_RxBuffer[4]-48)+(USART1_RxBuffer[5]-48)*0.1);
    }
    if(USART1_RxBuffer[6] == '0')
    {
        y0 = (USART1_RxBuffer[7]-48)*100+(USART1_RxBuffer[8]-48)*10+(USART1_RxBuffer[9]-48)+(USART1_RxBuffer[10]-48)*0.1;
    }
    else if(USART1_RxBuffer[6] == '1')
    {
        y0 = -((USART1_RxBuffer[7]-48)*100+(USART1_RxBuffer[8]-48)*10+(USART1_RxBuffer[9]-48)+(USART1_RxBuffer[10]-48)*0.1);
    }
    printf("\nx0 = %f\n",x0);
    printf("y0 = %f\n",y0);
    m = sqrt(x0*x0+y0*y0)-170;//求出机械臂点C与原点距离m
    printf("m = %f\n",m);
    a1=(103.5*103.5 + m*m + 16*16 - 88*88) / (2 * 103.5 * sqrt(m*m + 16*16));
    printf("a1 = %f\n",a1);
    angle1 = acos(a1);
    printf("angle1 = %f\n",angle1);
    angle2 = atan(16 / m);
    printf("angle2 = %f\n",angle2);
    a3=(103.5*103.5 - m*m - 16*16 + 88*88) / (2 * 103.5 * 88);
    printf("a3 = %f\n",a3);
    angle3 = acos(a3);
    printf("angle3 = %f\n",angle3);
    theta1 = -(90-(angle2+angle1)/ (2 * PI)* 360);
    printf("theta1 = %f\n",theta1);
    theta2 = 180 - angle3/ (2 * PI)* 360;
    printf("theta2 = %f\n",theta2);
    theta3 = 180 - ((angle1 + angle3 + angle2) / (2 * PI)* 360);
    printf("theta3 = %f\n",theta3);
    theta4 = atan(y0/x0)/ (2 * PI)* 360;
    printf("theta4 = %f\n",theta4);

}
