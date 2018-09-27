#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    int num;
    double temps;
} voiture;

double generate_time(char temp[20]){
    int x;
    srand(time(NULL));
    x = rand();
    return x;
}

void affichage(){
    printf("%d\n", v1.num);
    printf("%lf\n", v1.temps);
    printf("%s\n", "fin");
}

int main(){
    voiture v1;
    v1.num = 44;
    v1.temps = generate_time("P1");
    
    affichage();
}