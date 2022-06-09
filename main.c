#include "hdlc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#define adrA "01011100\0"
#define adrB "01010101\0"
#define n 8
#define sz (33+n)*2

int p1[2];
int p2[2];

void main()
{
    intro();
    int pid;
    pipe(p1);
    pipe(p2);

    if(pid=fork())
    {
        unsigned ns=0,nr=0,finp=0,fina=0,deb=1;
        char tpe[sz];
        char tpr[sz];
        do{

            //kill(pid,SIGSTOP);
			puts("Je suis le père (émetteur)");

            //Envoie d'une trame au fils

            if(deb){
                puts("Une demande d'échange est envoyée au recepteur");
                composerTrameU(adrRec,"11\0","1\0","100\0",tpe);
                deb=0;
            }else scenarioSNW(&ns,&nr,&finp,&fina,tpe,tpr,adrEm,adrRec);

            close(p1[0]);
            write(p1[1],tpe,sz);
            //kill(pid,SIGCONT);

            //Reception et traitement d'une trame du fils
            close(p2[1]);
            read(p2[0],tpr,sz);

        }while(!finp && !fina);
		waitpid(pid);
        printf("\nFin de la tâche de l'émetteur, envoie de %u trames, et reception de %u trames\n",ns,nr);

    }
    else{
        unsigned ns=0,nr=0,finp=0,fina=0;
        char tfe[sz];
        char tfr[sz];
        do{

            //Reception d'une trame du père

            close(p1[1]);
            read(p1[0],tfr,sz);
			//kill(getppid(),SIGSTOP);
            puts("Je suis le fils (recepteur)");
            scenarioSNW(&ns,&nr,&finp,&fina,tfe,tfr,adrRec,adrEm);

            //Envoie d'une trame au père

            close(p2[0]);
            write(p2[1],tfe,sz);
            //kill(getppid(),SIGCONT);

        }while(!finp && !fina);
        printf("\nFin de la tâche du recepteur, envoie de %u trames, et reception de %u trames\n",ns,nr);
        _exit(NULL);
    }
    sleep(3);
    outro();
}
