#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <windows.h>

struct wavfile
{
    char        id[4];          	// doit contenir "RIFF"
    int         totallength;        // taille totale du fichier moins 8 bytes
    char        wavefmt[8];     	// doit etre "WAVEfmt "
    int         format;             // 16 pour le format PCM
    short       pcm;              	// 1 for PCM format
    short       channels;         	// nombre de channels
    int         frequency;          // frequence d echantillonage
    int         bytes_per_second;   // nombre de bytes par secondes
    short       bytes_by_capture;
    short       bits_per_sample;  	// nombre de bytes par echantillon
    char        data[4];        	// doit contenir "data"
    int         bytes_in_data;      // nombre de bytes de la partie data
};

long charger_wav (char *path);
long charger_wav (char *path)
{
   int i=0, nbech=0;
   long tmp=0;
   FILE *wav = fopen (path, "rb");
   struct wavfile header;

    if ( wav == NULL )
    {
        printf("\nne peut pas ouvrir le fichier demande, verifier le nom\n");
        printf("ne pas oublier l'extension .wav\n");
        /*exit(1);*/
    }

    /*---------------------lecture de l entete et enregistrement dans header--------*/
    //initialise le header par l'entete du fichier wave
    //verifie que le fichier posséde un entete compatible
    if ( fread(&header,sizeof(header),1,wav) < 1 )
    {
        printf("\nne peut pas lire le header\n");
        /*exit(1);*/
    }
    if (    header.id[0] != 'R'|| header.id[1] != 'I'|| header.id[2] != 'F'|| header.id[3] != 'F' )
    {
        printf("\nerreur le fichier n'est pas un format wave valide\n");
        /*exit(1);*/
    }
    if (header.channels!=1)
    {
        printf("\nerreur : le fichier n'est pas mono\n");
        /*exit(1);*/
    }
	printf("Lecture du fichier OK\nRecherche en cours...\n");
    /*-------------------------determiner la taille des tableaux---------------------*/
    nbech=(header.bytes_in_data/header.bytes_by_capture);

    /*---------------------creation des tableaux dynamiques--------------------------*/
    long *data=NULL; //tableau de l'onde temporelle
    data=malloc( nbech * sizeof(long));
    if (data == NULL)
    {
        /*exit(0);*/
    }

    /*---------------------initialisation des tableaux dynamiques--------------------*/
    for(i=0; i<(nbech); i++)
    {
        data[i]=0;
    }
    /*printf("initialisation tableau OK\n");*/
	
	
    /*---------------------remplissage du tableau tab avec les echantillons----------*/
    i=0;
    long value;
    while( fread(&value,3,1,wav) )
    {
        //lecture des echantillons et enregistrement dans le tableau
       /* if (value & 0x7FFFFF)
            value = 0x000000FF + value;*/
        value=value<<8;
        data[i]=value;
        /*printf("%ld\n",data[i]);*/
        i++;
    }
    /*printf("\nnombre d'echantillons lus : %d\n",i);*/
    i=0;
    /*for(i=0;i<nbech;i++)
    {
        if(data[i+2]>=128)
            data[i+2]-=256;
        data[i]=data[i]+256*data[i+1]+65536*data[i+2];
    }*/
    for(i=0;i<nbech;i++)
    {
        if(abs(data[i])>=tmp)
            tmp=abs(data[i]);
        /*if(i>440990)*/
            /*printf("%ld\n",data[i]);*/
    }
    /*tmp=tmp/8388608;*/
    /*printf("crete : %ld\n",tmp);*/
    /*printf("Appuyer sur un chiffre pour continuer\n");
    scanf("%d",&i);*/
    free(data);
    fclose(wav);
    /*printf("fonction OK\n");*/
   return tmp;
}

int main()
{

    int i=0, pf, df;
    long a=0, b=0, choix=1;
    float dB=0;
    char nom[10];

    /*---------------------selection des fichiers audio-------------------------------*/
    while(choix)
    {
    printf ("Entrer 'x' le numero du premier fichier audio 'SRx.wav' :\n");
    scanf("%d", &pf);
    printf ("Entrer 'y' le numero du dernier fichier audio 'SRy.wav' :\n");
    scanf("%d", &df);
    /*printf("Noms OK\n");*/
    for(i=0;i<=(df-pf);i++)
    {
            sprintf(nom,"SR%d.wav",pf+i);
            b=charger_wav(nom);
            if(a<b)
            a=b;
    }
    /*printf("crete des fichiers : %ld\n",a);*/
    /*dB=(float)(2^30)/a;*/

    /*printf("dB : %f\n",dB);*/
    dB=20*log10(2147483647./a);
    printf("\n______________________________________\n\n");
    printf("La marge de gain est de %.1f dB\n",dB);
    printf("______________________________________\n\n");
    printf("1 : relancer le programme\n0 : fermer le programme\n");
    scanf("%ld",&choix);
    printf("\n\n");
    /*printf("nbech : %d\n",a);*/
    /*Sleep(5000);*/
    }
    return 0;
}
