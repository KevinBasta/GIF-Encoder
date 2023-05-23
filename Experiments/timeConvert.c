#include <stdlib.h>
#include <stdio.h>

int realTimeToMediaTime(unsigned int time, unsigned int otherSystem) { 
    double normalizedTime = time / 60.0;
    double final = (normalizedTime * otherSystem);
    int finalInt = final;
    printf("%d\n", finalInt);
}


int main() { 
    realTimeToMediaTime(143, 1000);
}