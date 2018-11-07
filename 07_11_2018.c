#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct{
    //int num;
    int temps1;
    int temps2;
    int temps3;
    int tt;//temps total
    int pit;//pit stop
    int out;//arrêt
    pid_t pid;
} voiture;

int main(int argc, char *argv[]){
    system("clear");
    printf("COURSE P1 : première scéance d'essais !!!\n");
    printf("\t|\tS1\t|\tS2\t|\tS3\t|\tTT\t|\tPIT\t|\tOUT\t\n\n");

    //***************************************chaque passage de la voiture
    voiture *voituresCourse;
    int shmid;
    key_t key = 12345;
    int f;

    //************************création du segment de memoire partagee
    shmid = shmget(key, sizeof(voiture), IPC_CREAT | 0666);
    if(shmid == -1){
      perror("Création de segment impossible: Erreur shmget du père.");
      exit(-1);
    }

    srand(time(NULL));
    //*****************mutation
    for(int i = 0; i <= 19; i++){
      int a = rand()%(55 - 35) + 35;
      int b = rand()%(55 - 35) + 35;
      int c = rand()%(55 - 35) + 35;
      f = fork();

      if( f == -1){
        perror("Impossible de créer un fils");
        exit(-1);
      }

      if(f == 0){
        int shmid2 = shmget(key, sizeof(voiture), IPC_CREAT | 0666);

        if(shmid2 == -1){
          perror("Création de segment impossible: Erreur shmget du fils\n");
          exit(-1);
        }

        voituresCourse = (voiture *)shmat(shmid2, 0, 0);
        if(voituresCourse == (voiture*)-1){
          perror("Attachement impossible: Erreur shmat du fils\n");
          exit(-1);
        }

        voituresCourse[i].temps1 = rand()%(55 - 35) + 35;
        voituresCourse[i].temps2 = rand()%(55 - 35) + 35;
        voituresCourse[i].temps3 = rand()%(55 - 35) + 35;
        voituresCourse[i].tt = voituresCourse[i].temps1 + voituresCourse[i].temps2 + voituresCourse[i].temps3;
        voituresCourse[i].pid = getpid();

        //il est temps pour le fils de mourrir
        //mais avant il doit se détacher de la memoire partagee
        if(shmdt(voituresCourse) == -1){
          perror("détachement impossible: Erreur shmdt du fils\n");
          exit(-1);
        }
        sleep(1);
        //le fils se suicide
        exit(1);
      }
    }

    for(int i = 0; i <= 19; i++){
      voituresCourse = (voiture *)shmat(shmid, 0, 0);

      if(voituresCourse == (voiture*)-1){
        perror("Attachement impossible: Erreur shmat du pére\n");
        exit(-1);
      }

      sleep(1);

      int div = voituresCourse[i].tt / 60;
      int res = voituresCourse[i].tt % 60;

      // affichage
      printf("%d\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:0 s\t|\t%d:%d m\t|\t(P)\t|\tIN\t\n", voituresCourse[i].pid, voituresCourse[i].temps1, voituresCourse[i].temps2, voituresCourse[i].temps3, div, res);

    }
    printf("\n\nPremiere course terminee\n\n\n");

      if(shmdt(voituresCourse) == -1){
        perror("détachement impossible: Erreur shmdt du père.");
        exit(-1);
      }

      if(shmctl(shmid, IPC_RMID,  (struct shmid_ds *) NULL) == -1){
        perror("destruction de la memoire partagee n'est pas faite: Erreur shmctl.");
        exit(-1);
      }
      exit(1);


    //le père attend la mort du fils
    wait(0);

    return 1;
}
