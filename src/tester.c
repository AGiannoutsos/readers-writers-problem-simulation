#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){ 

    char buf1[50];
    char buf2[50];
    for (int i = 10; i < 100; i+=5){
        for (int j = 100; j > 0; j-=5){
            snprintf(buf1, 16, "./mainexe %d 50", j);
            snprintf(buf2, 15, " %d ", i);
            printf("%s\n",strcat(buf1,buf2));
            system(strcat(buf1,buf2));
        }
        
    }
      
   // 
}