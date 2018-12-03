#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#define key 12345

/* Structure de la voiture */
typedef struct{
    int num;
    int temps1;
    int temps2;
    int temps3;
    int tt;//temps total
    int pit;//pit stop
    int out;//arrêt
    pid_t pid;
} voiture;

voiture *voituresCourse;
voiture copieAffichage[20];


void courseTour() {
    int numVoiture[] = {44,77,5,7,3,33,11,31,18,35,27,55,10,28,8,20,2,14,9,16};
    for(int i = 0; i <= 19; i++){
        int f = fork();
        
        if( f == -1){
            perror("Impossible de créer un fils: Erreur fork.\n");
            exit(-1);
        }
        
        /* Création d'une voiture */
        if(f == 0){
            srand(time(0) + getpid());
            int shmid2 = shmget(key, sizeof(voiture), IPC_CREAT | 0666);
            
            if(shmid2 == -1){
                perror("Création de segment impossible: Erreur shmget du fils.\n");
                exit(-1);
            }
            
            /* Attachement à la structure en mémoire partager */
            voituresCourse = (voiture *)shmat(shmid2, 0, 0);
            if(voituresCourse == (voiture*)-1){
                perror("Attachement impossible: Erreur shmat du fils.\n");
                exit(-1);
            }
            
            voituresCourse[i].num = numVoiture[i];
            voituresCourse[i].pid = getpid();
            
            for(int i = 0 ; i < 15; i++){
                sleep(5);
                int a = rand()%(55 - 35) + 35;
                int b = rand()%(55 - 35) + 35;
                int c = rand()%(55 - 35) + 35;
                int d = a + b + c;
                if(voituresCourse[i].tt == 0){
                    voituresCourse[i].temps1 = a;
                    voituresCourse[i].temps2 = b;
                    voituresCourse[i].temps3 = c;
                    voituresCourse[i].tt = d;
                }
                else{
                    if(d < voituresCourse[i].tt){
                        voituresCourse[i].temps1 = a;
                        voituresCourse[i].temps2 = b;
                        voituresCourse[i].temps3 = c;
                        voituresCourse[i].tt = d;
                    }
                }
            }
            
            //il est temps pour le fils de mourrir
            //mais avant il doit se détacher de la memoire partagee
            if(shmdt(voituresCourse) == -1){
                perror("détachement impossible: Erreur shmdt du fils.\n");
                exit(-1);
            }
            sleep(1);
            //le fils se suicide
            exit(1);
        }
    }
}


/* Initialise la course entière en repertoriant les numeros respectifs des voitures */
void initCourse(){
    int numVoiture[] = {44,77,5,7,3,33,11,31,18,35,27,55,10,28,8,20,2,14,9,16};
    for(int i = 0; i <= 19; i++){
        voituresCourse[i].num = numVoiture[i];
        voituresCourse[i].temps1 = 0;
        voituresCourse[i].temps2 = 0;
        voituresCourse[i].temps3 = 0;
        voituresCourse[i].tt = 0;
        voituresCourse[i].pid = 0;
    }
}

void trierTab(voiture voitureCopie[20]){
    for(int i = 0; i <= 19; i++){
        for(int j = 0; j <=18; j++){
            if(voitureCopie[j].tt > voitureCopie[j+1].tt){
                voiture voit = voitureCopie[j];
                voitureCopie[j]= voitureCopie[j+1];
                voitureCopie[j+1] = voit;
            }
        }
    }
}

void affichage(){
    voiture voitureCopie[20];
    memcpy(&voitureCopie, voituresCourse, 20*sizeof(voiture));
    
    system("clear");
    printf("COURSE P1 : première scéance d'essais !!!\n\n\n");
    printf("\t|\tS1\t|\tS2\t|\tS3\t|\tTT\t|\tPIT\t|\tOUT\t\n\n");
    sleep(1);
    
    trierTab(voitureCopie);
    
    for(int i = 0; i <= 19; i++){
        sleep(1);
        int div = voitureCopie[i].tt / 60;
        int res = voitureCopie[i].tt % 60;
        
        // affichage
        printf("%d\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:%d m\t|\t(P)\t|\tIN\t\n", voitureCopie[i].num, voitureCopie[i].temps1, voitureCopie[i].temps2, voitureCopie[i].temps3, div, res);
    }
}

void courseSession(int trs){
    for(int i = 0; i <= trs; i++){
        courseTour();
        affichage();
    }
}

int main(int argc, char *argv[]){
    system("clear");
    void initCourse();
    
    /* Création du segment en mémoire partagée */
    int shmid = shmget(key, sizeof(voiture), IPC_CREAT | 0666);
    if(shmid == -1){
        perror("Création de segment impossible: Erreur shmget du père.\n");
        exit(-1);
    }
    
    voituresCourse = (voiture *)shmat(shmid, 0, 0);
    if(voituresCourse == (voiture*)-1){
        perror("Attachement impossible: Erreur shmat du pére.\n");
        exit(-1);
    }
    sleep(1);
    courseSession(3);
    if(shmdt(voituresCourse) == -1){
        perror("détachement impossible: Erreur shmdt du père.\n");
        exit(-1);
    }
    
    if(shmctl(shmid, IPC_RMID,  (struct shmid_ds *) NULL) == -1){
        perror("destruction de la memoire partagee n'est pas faite: Erreur shmctl.\n");
        exit(-1);
    }
    exit(1);
    //le père attend la mort du fils
    wait(0);
    
    return 1;
}
