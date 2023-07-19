#include "persistance.h"


int main(){
    Score j1;
    long scorej1 = 100000;
    strcpy(j1.pseudo,"jr1");
    j1.score = scorej1;
    Score j2;
    long scorej2 = 150000;
    strcpy(j2.pseudo,"jr2");
    j2.score = scorej2;

    Score *scores[10];
    for (int i = 0; i < sizeof(scores)/sizeof(Score*); i++)
    {
        scores[i] = NULL;
    }

    lisScore(scores);
    if(estDansLaListe(scores,scorej1)){
        insereDansListe(scores,j1);
    }
    if(estDansLaListe(scores,scorej2)){
        insereDansListe(scores,j2);
    }
    printf("Debut test 1 :\n");
    for (int i = 0; i < sizeof(scores)/sizeof(Score*); i++)
    {
        if(scores[i]){
            printf("%d] %s : %d\n",i,scores[i]->pseudo,scores[i]->score);
        }
    }
    printf("Fin test 1\n");
    enregistreScore(scores);
    libereScores(scores);
    lisScore(scores);
    printf("Debut test 2 :\n");
    for (int i = 0; i < sizeof(scores)/sizeof(Score*); i++)
    {
        if(scores[i]){
            printf("%d] %s : %d\n",i,scores[i]->pseudo,scores[i]->score);
        }
    }
    printf("Fin test 2\n");
    for(int i=0;i<10;i++){
        if(estDansLaListe(scores,scorej2)){
            insereDansListe(scores,j2);
        }
    }
    printf("Debut test 3 :\n");
    for (int i = 0; i < sizeof(scores)/sizeof(Score*); i++)
    {
        if(scores[i]){
            printf("%d] %s : %d\n",i,scores[i]->pseudo,scores[i]->score);
        }
    }
    printf("Fin test 3\n");

    lisScore(scores);

    printf("Fin des tests\n");

    return 0;
}