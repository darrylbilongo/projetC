#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

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
    system("clear");
    printf("COURSE P1 : première scéance d'essais !!!");
    printf("\t|\tS1\t|\tS2\t|\tS3\t|\tTT\t|\tPIT\t|\tOUT\t\n\n");

    //***************************************chaque passage de la voiture
    voiture *voituresCourse;
    int shmid;
    key_t key = 12345;
    int pid;
    srand(time(NULL));

    //************************création du segment de memoire partagee
    shmid = shmget(key, sizeof(voiture), IPC_CREAT | 0666);
    if(shmid == -1){
      perror("Création de segment impossible: Erreur shmget");
      exit(-1);
    }

    //*****************mutation
    pid = fork();

    if(pid == -1){
      perror("Impossible de créer un fils");
      exit(-1);
    }

    else if(pid == 0){
      int shmid2 = shmget(key, sizeof(voiture), IPC_CREAT | 0666);

      if(shmid2 == -1){
        perror("Création de segment impossible: Erreur shmget");
        exit(-1);
      }

      voituresCourse = (voiture *)shmat(shmid2, 0, 0);
      if(voituresCourse == (voiture*)-1){
        perror("Attachement impossible: Erreur shmat");
        exit(-1);
      }

      voituresCourse->temps1 = rand()%(55 - 35) + 35;
      voituresCourse->temps2 = rand()%(55 - 35) + 35;
      voituresCourse->temps3 = rand()%(55 - 35) + 35;
      voituresCourse->tt = voituresCourse->temps1 + voituresCourse->temps2 + voituresCourse->temps3;

      //il est temps pour le fils de mourrir
      //mais avant il doit se détacher de la memoire partagee
      if(shmdt(voituresCourse) == -1){
        perror("détachement impossible: Erreur shmdt");
        exit(-1);
      }
      //le fils se suicide
      exit(1);
    }
    else{
      voituresCourse = (voiture *)shmat(shmid, 0, 0);

      if(voituresCourse == (voiture*)-1){
        perror("Attachement impossible: Erreur shmat");
        exit(-1);
      }

      sleep(1);

      int div = voituresCourse->tt / 60;
      int res = voituresCourse->tt % 60;

      // affichage
      printf("\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:%d m\t|\t(P)\t|\tIN\t\n", voituresCourse->temps1, voituresCourse->temps2, voituresCourse->temps3, div, res);

      if(shmdt(voituresCourse) == -1){
        perror("détachement impossible: Erreur shmdt");
        exit(-1);
      }
      exit(1);
    }

    //le père attend la mort du fils
    wait(0);

    return 1;
}
