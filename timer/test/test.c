#include <stdlib.h>
#include <stdio.h>
#include <../mytimer.h>

MyTimer* beater;

int main(){
	beater = malloc(beater,sizeof(MyTimer));
	
	InitHeartBeat(beater, 1000);
		

}
