#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct{
    int num;
    int temps1;
    int temps2;
    int temps3;
    int tt;//temps total
    int pit;//pit stop
    int out;//arrêt
} voiture;

int main(int argc, char *argv[]){
    
    int segid; // id du segment de memoire
    voiture * voituresCourse;
    int nbvoitures = 20; // nombre de voitures
    key_t key = 1245; // clé
    
    // Mémoire partagée
    segid = shmget(key,20*sizeof(voiture),0666);
    
    if(segid == -1){
        perror("shmget");
        return (EXIT_FAILURE);
    }
    
    printf("Shared memory segment ID is %d\n", segid);
    srand(time(NULL));
    
    //for(int p = 0; p <= 100; p++ ){
        //system("clear");
        //printf("COURSE P1 : première scéance d'essais !!!\n");
        //printf("\t|\tS1\t|\tS2\t|\tS3\t|\tTT\t|\tPIT\t|\tOUT\t\n\n");

        //for(int i= 1; i < nbvoitures ; i++){
            
            int pid = fork();
            
            if(pid == -1){
                exit(-1);
            }
    
            if(pid == 0){
                
                int shmfils = shmget(key,0,0666);

                voituresCourse = (voiture *)shmat(segid,(void *)0,0);
                
                /* ecriture */
                
                printf("Dans le fork !!!\n");
                voituresCourse->temps1 = rand()%(55 - 35) + 35;
                voituresCourse->temps2 = rand()%(55 - 35) + 35;
                voituresCourse->temps3 = rand()%(55 - 35) + 35;
                voituresCourse->tt = voituresCourse->temps1 + voituresCourse->temps2 + voituresCourse->temps3;
                
                shmdt((void *) voituresCourse);
            }
            else{
                printf("Affichage par le père\n");
                voituresCourse = shmat(segid,(void *)0,0);
                
                sleep(1);
                
                 /* affichage */
                
                int div = voituresCourse->tt / 60;
                int res = voituresCourse->tt % 60;
                 printf("\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:%d m\t|\t(P)\t|\tIN\t\n", voituresCourse->temps1, voituresCourse->temps2, voituresCourse->temps3, div, res);
                exit(1);
            }
        //}
}
