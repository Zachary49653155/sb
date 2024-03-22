#include "duoji.h"


extern uint16_t quare[19];
void control_duoji(uint8_t a)
{
    if(quare[a] > quare[a+6])
    {
        for(quare[a+12]=quare[a+6]; quare[a+12]<quare[a]; quare[a+12]++)HAL_Delay(30);
        quare[a+6]=quare[a];
    }
    else if(quare[a] < quare[a+6])
    {
        for(quare[a+12]=quare[a+6]; quare[a+12]>quare[a]; quare[a+12]--)HAL_Delay(30);
        quare[a+6]=quare[a];
    }
}
