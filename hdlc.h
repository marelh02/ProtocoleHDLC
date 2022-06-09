//// biblio qui incarne le protocole hdlc 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//Les constantes utiles dans le programme
#define n 8
#define sz (33+n)*2
#define G "10011\0"
#define adrEm "01011100\0"
#define adrRec "01010101\0"

//valeurs possibles de ns et nr
const char nums[8][4]={"000\0","001\0","010\0","011\0","100\0","101\0","110\0","111\0"};


////////////////////////////////////// fcts réalisant des ops basiques et necéssaires /////////////////////////////////////////////////////////////////////////////////////////////// 

//cette fct convertit un charactère en son code binaire et le met dans bin
void charVersBin(char c, int bin[])
{
    int j = 0;
    int i = (int) c;
    while( j < 8)
    {
        bin[j] = i%2;
        i = i/2;
        j++;
    }
}

//cette fct convertit un mot binaire en une chaîne de caractères
char binVersChar(char bin[], int oct)
{
    int k = 0;
    int i = 0;
    while( k < 8 )
    {
        if (bin[k + 8*oct] == '1')
        {
        i = i + pow(2, 7 - k);
    }
        k++;
    }
    char c = (char) i;
    return c;
}

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

//cette fct extrait une sous chaine de mot, à partir de d jusqu'à f exclue, et la met dans res
void getSubstr(char mot[], int d, int f, char res[])
{
    strcpy(res,"\0");
    char r[f-d+2];
    unsigned i=d,j=0;
    while(i<f) 
    {
        r[j]=mot[i];
        i++;
        j++;
    }
    r[j]='\0';
    strcpy(res,r);
}

//effectue le xor entre deux mots de même taille
void opxor(char a[],char b[],char res[])
{
    int i=0;
    while(i<strlen(a))
    {
        if(a[i]==b[i]) res[i]='0';
        else res[i]='1';
        i++;
    }
    res[i]='\0';
}

//cette fct enlève les zeroes inutiles d'un mot binaire, exemple: 000110 devient 110
void enleverZeroes(char mot[])
{
    unsigned i=0,enc=0;
    while(i<strlen(mot))
    {
        if(mot[i]=='1')
        {
            enc=1;
            break;
        }
        i++;
    }
    if(enc && i>0)
    {
        char res[strlen(mot)];
        enc=0;
        while(i<strlen(mot))
        {
            res[enc]=mot[i];
            i++;
            enc++;
        }
        res[enc]='\0';
        strcpy(mot,res);
    }
    else if(!enc && i==strlen(mot)) strcpy(mot,"\0");
}

//fct qui ajoute des zeroes à droite ou à gauche d'un mot binaire, selon la valeur de dir
void extensionBin(char mot[],unsigned len,char dir,char res[])
{
    char ext[len+1];
    for(int i=0;i<len;i++) ext[i]='0';
    ext[len]='\0';
    if(dir=='d')
    {
        int j=0;
        while(j<strlen(mot))
        {
            res[j]=mot[j];
            j++;
        }
        for(int k=0;k<strlen(ext);k++)
        {
            res[j]=ext[k];
            j++;
        }
        res[j]='\0';
    }
    else if(dir=='g')
    {
        int j=0;
        while(j<strlen(ext))
        {
            res[j]=ext[j];
            j++;
        }
        for(int k=0;k<strlen(mot);k++)
        {
            res[j]=mot[k];
            j++;
        }
        res[j]='\0';
    }
}

//cette fct retourne le reste de la division euclidienne de a par b dans res
void resteDivEuc(char a[],char b[],char res[])
{
    unsigned cur=0,len;
    len=strlen(b);
    char reste[strlen(b)],divd[strlen(b)],temp[strlen(b)];
    strcpy(reste,"\0");
    strcpy(divd,"\0");
    strcpy(temp,"\0");
    enleverZeroes(a);
    enleverZeroes(b);
    while(cur<strlen(a))
    {
        getSubstr(a,cur,cur+len-strlen(reste),temp);
        cur+=len-strlen(reste);
        strcat(divd,temp);
        if(strlen(divd)!=strlen(b)) break;
        else{
        opxor(divd,b,reste);
        enleverZeroes(reste);
        strcpy(divd,reste);
        }
    }
    enleverZeroes(divd);
    strcpy(res,divd);
}

//cette fct calcule le crc de info, et le met dans res
void crc(char info[],char res[])
{
    char infoDup[strlen(info)+strlen(G)-1],resu[strlen(G)];
    extensionBin(info,strlen(G)-1,'d',infoDup);
    resteDivEuc(infoDup,G,resu);
    enleverZeroes(resu);
    extensionBin(resu,strlen(G)-1-strlen(resu),'g',res);
    
}

//cette fct vérifie la validité de la trame à partir du crc dans le champs fcs
void verifiercrc(char t[])
{
    char crc[strlen(G)],v[strlen(t)-15];
    getSubstr(t,strlen(t)-strlen(G)+1,strlen(t),crc);
    getSubstr(t,0,strlen(t)-16,v);
    char v2[strlen(crc)+strlen(v)+1];
    strcpy(v2,"\0");
    strcat(v2,v);
    strcat(v2,crc);
    strcpy(crc,"\0");
    resteDivEuc(v2,G,crc);
    enleverZeroes(crc);
    puts("\nVérification de la validité de la trame à partir du crc: ");
    !strlen(crc)?printf("la trame est correcte\n"):printf("la trame n'est pas correcte\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////// fcts d'affichage /////////////////////////////////////////////////////////////////////////////////////////

void asciiArt1()
{
puts("                                    ,-.");
puts("                                   / \\  `.  __..-,O");
puts("                                  :   \\ --''_..-'.'");
puts("                                  |    . .-' `. '.");
puts("                                  :     .     .`.'");
puts("                                   \\     `.  /  ..");
puts("                                    \\      `.   ' .");
puts("                                     `,       `.   \\");
puts("                                    ,|,`.        `-.\\");
puts("                                   '.||  ``-...__..-`");
puts("                                    |  |");
puts("                                    |__|");
puts("                                    /||\\");
puts("                                   //||\\\\");
puts("                                  // || \\\\");
puts("                               __//__||__\\\\__");
puts("                              '--------------' ");
}

void asciiArt2()
{
puts(" ____               __                          ___             ");
puts("/\\  _`\\            /\\ \\__                      /\\_ \\            ");
puts("\\ \\ \\L\\ \\_ __   ___\\ \\ ,_\\   ___     ___    ___\\//\\ \\      __   ");
puts(" \\ \\ ,__/\\`'__\\/ __`\\ \\ \\/  / __`\\  /'___\\ / __`\\\\ \\ \\   /'__`\\ ");
puts("  \\ \\ \\/\\ \\ \\//\\ \\L\\ \\ \\ \\_/\\ \\L\\ \\/\\ \\__//\\ \\L\\ \\\\_\\ \\_/\\  __/ ");
puts("   \\ \\_\\ \\ \\_\\\\ \\____/\\ \\__\\ \\____/\\ \\____\\ \\____//\\____\\ \\____\\");
puts("    \\/_/  \\/_/ \\/___/  \\/__/\\/___/  \\/____/\\/___/ \\/____/\\/____/\n\n");                                                            
puts(" __  __  ____    __       ____                         ");         
puts("/\\ \\/\\ \\/\\  _`\\ /\\ \\     /\\  _`\\     ");                           
puts("\\ \\ \\_\\ \\ \\ \\/\\ \\ \\ \\    \\ \\ \\/\\_\\      ");                        
puts(" \\ \\  _  \\ \\ \\ \\ \\ \\ \\  __\\ \\ \\/_/_        ");                     
puts("  \\ \\ \\ \\ \\ \\ \\_\\ \\ \\ \\L\\ \\\\ \\ \\L\\ \\          ");                  
puts("   \\ \\_\\ \\_\\ \\____/\\ \\____/ \\ \\____/             ");               
puts("    \\/_/\\/_/\\/___/  \\/___/   \\/___/  ");
}

void outro()
{
    system("clear");
     printf("\033[0;33m");
     asciiArt2();
     sleep(1);
    system("clear");
     printf("\033[0;31m");
     asciiArt2();
     sleep(1);
     system("clear");
     printf("\033[0;32m");
     asciiArt2();
     sleep(1);
     system("clear");
     printf("\033[0;35m");
     asciiArt2();
     sleep(1);
     system("clear");
     printf("\033[0;34m");
     asciiArt2();
     sleep(1);
     system("clear");
     printf("\033[0;36m");
     asciiArt2();
     sleep(1);
}

void intro()
{
    system("clear");
    asciiArt1();
    printf("\n\nBienvenue à la simulation du protocole HDLC.\nCréation de: Benhlima Douae et Elhilali Marouane de la première année du génie informatique\n");
    sleep(3);
    system("clear");
}

//scénarios I

void sciSNW()
{
    puts("\nVous venez de recevoir une trame I, que faire?\n1) Envoyer un message et acquitter? (piggybacking)\n2) Acquittement simple\n3) Rejet de la dernière trame (simuler qu'une trame est érronée)\nVeuillez saisir le numéro de votre choix\n************************************************************************\n");
}

//scénarios S

void scrr()
{
    puts("\nVotre recepteur vient d'accuser la reception de votre dernière trame I, que faire?\n1) Envoyer une autre trame I?\n2) Demande de déconnexion? (fin des échanges)\nVeuillez saisir le numéro de votre choix\n************************************************************************");
}

//scénarios S
void scsabm(){
    puts("\nVous venez de recevoir une demande de connexion pour échanger des informations, que faire?\n1) Accepter?\n2) Refuser?\nVeuillez saisir le numéro de votre choix\n************************************************************************\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////// ces fcts appliquent les principes du protocole hdlc ///////////////////////////////////////////////////////////////////////////////////////////////

//cette structure représente les trames u et s
typedef struct{
    char adresse[9];
    char controle[9];
    char fcs[17];
}tus;

//cette structure représente la trame i
typedef struct{
    char adresse[9];
    char controle[9];
    char info[n+1];
    char fcs[17];
}ti;

void formulerTrameUS(tus t,char s[])
{
    strcpy(s,"\0");
    strcat(s,t.adresse);
    strcat(s,t.controle);
    strcat(s,t.fcs);
}

tus deformulerTrameUS(char s[])
{
    tus t;
    getSubstr(s,0,8,t.adresse);
    getSubstr(s,8,16,t.controle);
    getSubstr(s,16,32,t.fcs);
    return t;
}

void formulerTrameI(ti t,char s[])
{
    char resultat[33+n];
    strcpy(resultat,"\0");
    strcat(resultat,t.adresse);
    strcat(resultat,t.controle);
    strcat(resultat,t.info);
    strcat(resultat,t.fcs);
    strcpy(s,resultat);
}

ti deformulerTrameI(char s[])
{
    ti t;
    getSubstr(s,0,8,t.adresse);
    getSubstr(s,8,16,t.controle);
    getSubstr(s,16,16+n,t.info);
    getSubstr(s,16+n,32+n,t.fcs);
    return t;
}

void transcodage(char str[],char res[])
{
    unsigned flag=0;
    unsigned neolen=0;
    for(int i=0;i<strlen(str);i++)
    {
        if(flag==5)
        {
            res[neolen]='0';
            flag=0;
            neolen++;
        }
        if(str[i]=='0') 
        {
            res[neolen]=str[i];
            flag=0;
            neolen++;
        }
        else
        {
            res[neolen]=str[i];
            flag++;
            neolen++;
        }
    }
    res[neolen]='\0';
}

void rencodage(char str[],char res[])
{
    unsigned flag=0;
    unsigned neolen=0;
    for(int i=0;i<strlen(str);i++)
    {
        if(flag==5)
        {
            flag=0;
            i++;
        }
        if(str[i]=='0') 
        {
            res[neolen]=str[i];
            flag=0;
            neolen++;
        }
        else
        {
            res[neolen]=str[i];
            flag++;
            neolen++;
        }
    } 
    res[neolen]='\0';
}

//à modifier pour amélioration
void transcodageAjouterFanions(char str[],char res[])
{
    char strTrans[strlen(str)*2];
    strcpy(res,"\0");
    transcodage(str,strTrans);
    strcat(res,"01111110\0");
    strcat(res,strTrans);
    strcat(res,"01111110\0");
}

//à modifier pour amélioration
void rencodageEnleverFanions(char str[],char res[])
{
    unsigned i=8,neolen=0;
    strcpy(res,"\0");
    while(i<strlen(str)-8)
    {
        res[neolen]=str[i];
        neolen++;
        i++;
    }
    res[neolen]='\0';
    char temp[strlen(res)+1];
    strcpy(temp,res);
    rencodage(temp,res);
}

//cette fct examine une trame reçue ete determine sa nature
void decortiquerTrame(char t[],ti *tinfo,tus *tuuss,char *nature)
{
    char res[strlen(t)];
    rencodageEnleverFanions(t,res);
    puts("\n************************************************************************\n");
    verifiercrc(res);
    if(strlen(res)>32)
    {
        ti tri;
        tri=deformulerTrameI(res);
        puts("La trame reçue est une trame d'informations.");
        (*nature)='i';
        printf("Adresse: %s\nControle: %s\nInformation: %s\nFCS: %s",tri.adresse,tri.controle,tri.info,tri.fcs);
        (*tinfo)=tri;
    }
    else
    {
        tus trus;
        trus=deformulerTrameUS(res);
        if(trus.controle[0]=='1' && trus.controle[1]=='1'){
            puts("La trame reçue est une trame non-numérotée.");
            (*nature)='u';
        }
        else if(trus.controle[0]=='1' && trus.controle[1]=='0'){
            puts("La trame reçue est une trame de supervision.");
            (*nature)='s';
        }
        printf("Adresse: %s\nControle: %s\nFCS: %s",trus.adresse,trus.controle,trus.fcs);
        (*tuuss)=trus;
    }
}

//cette fct construit une trame I
void composerTrameI(char adr[],char nr[],char p_f[],char ns[],char resultat[])
{
    char data[9],res[(33+n)*2];
    viderBuffer();
    printf("Donnez un code binaire sur %d bits qui représente votre information: ",n);
    gets(data);
    strcpy(res,"\0");
    strcat(res,adr);
    strcat(res,"0\0");
    strcat(res,ns);
    strcat(res,p_f);
    strcat(res,nr);
    strcat(res,data);
    char rde[5],rd[17];
    crc(res,rde);
    extensionBin(rde,17-strlen(G),'g',rd);
    strcat(res,rd);
    puts("La trame I à envoyer est");
    puts(res);
    transcodageAjouterFanions(res,resultat);
}

//cette fct construit une trame S
void composerTrameS(char adr[],char nr[],char p_f[],char ss[],char resultat[])
{
    char res[33];
    strcpy(res,"\0");
    strcat(res,adr);
    strcat(res,"10\0");
    strcat(res,ss);
    strcat(res,p_f);
    strcat(res,nr);
    char rde[5],rd[17];
    crc(res,rde);
    extensionBin(rde,17-strlen(G),'g',rd);
    strcat(res,rd);
    puts("La trame à envoyer est");
    puts(res);
    transcodageAjouterFanions(res,resultat);
}

//cette fct construit une trame U
void composerTrameU(char adr[],char mm[],char p_f[],char mmm[],char resultat[])
{
    char res[33];
    strcpy(res,"\0");
    strcat(res,adr);
    strcat(res,"11\0");
    strcat(res,mm);
    strcat(res,p_f);
    strcat(res,mmm);
    char rde[5],rd[17];
    crc(res,rde);
    extensionBin(rde,17-strlen(G),'g',rd);
    strcat(res,rd);
    puts("La trame à envoyer est");
    puts(res);
    transcodageAjouterFanions(res,resultat);
}

//cette fct traite les scénarios possibles selon la trame reçue
void scenarioSNW(unsigned *ns,unsigned *nr,unsigned *finp,unsigned *fina,char te[],char tr[],char myAdr[],char adr[])
{
    ti ttinfo;
    tus ttus;
    char nature;
    
    decortiquerTrame(tr,&ttinfo,&ttus,&nature);
    
    if(nature=='i'){
        
        sciSNW();
        unsigned ch;
        
        do{
            scanf("%u",&ch);
            if(ch>4 || ch==0) puts("\nChoix invalide!\nDonnez un choix valide");
        }while(ch>4 || ch==0);
        
        if(ch==1){
            
            (*nr)++;
            //fflush (stdin);
            //composerTrameI(adr,nums[(*nr)%8],"1\0",nums[(*ns)%8],te);
            composerTrameI(adr,nums[(*nr)%8],"0\0",nums[(*ns)%8],te);
            
        }else if(ch==2){
            
            (*nr)++;
            composerTrameS(adr,nums[(*nr)%8],"0\0","00\0",te);
            
        }else{
            
           composerTrameS(adr,nums[(*nr)%8],"0\0","01\0",te);
           
        }
        
    }else if(nature=='u'){
        
        char m[6],m2[4];
        getSubstr(ttus.controle,2,4,m);
        getSubstr(ttus.controle,5,8,m2);
        strcat(m,m2);

        if(!strcmp(m,"11100\0")){
            //SABM
            
            unsigned ch;
            scsabm();
            
            do{
                scanf("%u",&ch);
                if(ch!=1 && ch!=2) puts("\nChoix invalide!\nDonnez un choix valide");
            }while(ch!=1 && ch!=2);
            
            if(ch==1) composerTrameU(adrEm,"00\0","0\0","110\0",te);
            
            else{
                composerTrameU(adrEm,"11\0","0\0","000\0",te);
                (*finp)=(*fina)=1;
            }
            
        }else if(!strcmp(m,"00110\0")){
            
            //UA
            if(*finp)
            {
                
                (*fina)=1;
                puts("\nFin de l'échange");
                
            }else{
                
                puts("\nDemande de connexion accéptée, debut de l'échange");
                composerTrameI(adr,nums[(*nr)%8],"1\0",nums[(*ns)%8],te);
                
            }

        }else if(!strcmp(m,"00010\0")){
            
            //DISC
            puts("\nDemande de déconnexion accéptée");
            composerTrameU(adrEm,"00\0","0\0","110\0",te);
            (*finp)=(*fina)=1;
            
        }else if(!strcmp(m,"11000\0")){
            
            //DM
            puts("\nDemande de connexion refusée, fin de l'échange");
            (*finp)=(*fina)=1;
            
        }else puts("\nChamps de contrôle invalide");

    }else if(nature=='s'){
        
        char ss[3];
        getSubstr(ttus.controle,2,4,ss);
        
        if(!strcmp(ss,"00\0")){
            
            //RR
            (*ns)++;
            scrr();
            unsigned ch;
            
            do{
                scanf("%u",&ch);
                if(ch>3 || ch==0) puts("\nChoix invalide!\nDonnez un choix valide");
            }while(ch>3 || ch==0);
            
            if(ch==1) composerTrameI(adr,nums[(*nr)%8],"0\0",nums[(*ns)%8],te);
            
            else{
                composerTrameU(adrEm,"00\0","1\0","010\0",te);
                (*finp)=1;
            }
            
        }else if(!strcmp(ss,"01\0")){
            
            //REJ
            puts("\nLa dernière trame n'était pas envoyée correctement, retransmission de la trame...");
        
        }else puts("\nChamps de contrôle invalide");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

