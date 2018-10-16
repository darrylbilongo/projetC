#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct{
    //int num;
    int temps1;
    int temps2;
    int temps3;
    int tt;//temps total
    int pit;//pit stop
    int out;//arrêt
} voiture;

int main(int argc, char *argv[]){
    printf("COURSE P1 : première scéance d'essais !!!");
    printf("\t|\tS1\t|\tS2\t|\tS3\t|\tTT\t|\tPIT\t|\tOUT\t\n\n");
    
    // chaque passage de la voiture
    int temps_passage ;
    srand(time(NULL));
    for(int p = 0; p <= 100; p++ ){
        voiture voituresCourse[20];
        for(int i= 0; i <= 19; i++){
            voituresCourse[i].temps1 = rand()%(55 - 35) + 35;
            voituresCourse[i].temps2 = rand()%(55 - 35) + 35;
            voituresCourse[i].temps3 = rand()%(55 - 35) + 35;
            
            voituresCourse[i].tt = voituresCourse[i].temps1 + voituresCourse[i].temps2 + voituresCourse[i].temps3;
            int div = voituresCourse[i].tt / 60;
            int res = voituresCourse[i].tt % 60;
            
            // affichage
            printf("\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:%d m\t|\t(P)\t|\tIN\t\n", voituresCourse[i].temps1, voituresCourse[i].temps2, voituresCourse[i].temps3, div, res);
        }
        system("clear");
    }
}
