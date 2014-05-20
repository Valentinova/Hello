#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <unistd.h>
#include "../mytimer.h"

MyTimer* beater;

int main(){
	int tmpchar = 0;
	
	beater = InitHeartBeat(1000);
		
	while(1){
		
		tmpchar = getchar();
		
		if('r' == tmpchar){
			printf("get char r-reset \n");
			ResetHeartBeat(beater);
		}	
		if('s' == tmpchar){
			printf("get char s-stop \n");
			StopHeartBeat(beater);
		}
		if('d' == tmpchar){
			printf("get char d-delete \n");
			DestroyHeartBeat(beater);
		}
	}

	DestroyHeartBeat(beater);

	return 0;
	
}
