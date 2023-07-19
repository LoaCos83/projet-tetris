#include "persistance.h"

void enregistreScore(Score *scores[10])
{
    int compt = 0;
    for (int i = 0; i < 10; i++)
    {
        if (scores[i])
        {
            compt++;
        }
    }

    Score trans[compt];

    //printf("Liste etant en train de s'enregistrer : \n");
    for (int i = 0; i < compt; i++)
    {
        trans[i] = *scores[i];
        //printf("%d] Name : %s \t Score = %ld \n", i, trans[i].pseudo, trans[i].score);
    }
    FILE *fichier;
    fichier = NULL;
    fichier = fopen("scores.bin", "wb+");
    if (fichier != NULL && compt != 0)
    {
        for (int i = 0; i < compt; i++)
        {
            fwrite(&trans[i], sizeof(Score), 1, fichier);
        }
        //printf("Writing done\n");
    }
    else
    {
        //printf("Probleme d'ouverture avec le fichier. \n");
    }
    fclose(fichier);
}

void lisScore(Score *scores[10])
{
    Score *trans = malloc(10 * sizeof(Score));
    FILE *fichier;
    fichier = NULL;
    fichier = fopen("scores.bin", "rb");

    if (fichier != NULL)
    {
        //printf("Liste recuperee a partir de la sauvegarde : \n");
        for (int i = 0; i < 10; i++)
        {
            if (fread(&trans[i], sizeof(Score), 1, fichier) == 1)
            {
                //printf("%d] Name : %s \t Score = %ld \n", i, trans[i].pseudo, trans[i].score);
                Score * score = malloc(sizeof(Score));
                *score = trans[i];
                scores[i] = score;
            }
            else
            {
                scores[i] = NULL;
            }
        }
    }
    else
    {
        //printf("Pas de fichier trouve. \n");
        fichier = fopen("scores.bin", "wb");
        fclose(fichier);
        for (int i = 0; i < 10; i++)
        {
            scores[i] = NULL;
        }
    }
    free(trans);
}

bool estDansLaListe(Score *scores[10], long scoreTest)
{
    if (scores[9])
    {
        if (scores[9]->score < scoreTest)
            return true;
        else
            return false;
    }
    else
    {
        return true;
    }
}

void insereDansListe(Score *scores[10], Score *ptscore)
{
    Score* score = (Score*)malloc(sizeof(Score));
    *score = *ptscore;
    int mode = 0;
    Score *decalage;
    Score *decalageTemp;
    for (int i = 0; i < 10; i++)
    {

        if (scores[i])
        {
            if (scores[i]->score < score->score && mode == 0)
            {

                mode = 1;
            }
            /*else if(scores[i]->score==score.score)        //à définir si les scores "gaux sont implementé dans la liste
            {
                break;          
            }*/

            if (mode == 1)
            {
                decalage = scores[i];
                scores[i] = score;
                mode = 2;
            }
            else if (mode == 2)
            {
                decalageTemp = scores[i];
                scores[i] = decalage;
                decalage = decalageTemp;
            }
        }
        else if (!scores[i] && i == 0)
        {

            free(scores[i]);
            scores[i] = score;

            break;
        }
        else
        {

            if (mode == 0)
            {

                if (scores[i - 1]->score < score->score)
                {
                    decalage = scores[i - 1];
                    scores[i - 1] = score;
                    scores[i] = decalage;
                }
                else
                {

                    scores[i] = score;
                }
            }

            else
            {

                scores[i] = decalage;
            }
            break;
        }
    }
}

void libereScores(Score *scores[10])
{
    for(int i=0; i<10;i++)
    {
        //printf("free %d : %p\n",i,scores[i]);
        if(scores[i]){
            //printf("free score %s %ld\n", scores[i]->pseudo,scores[i]->score);
            free(scores[i]);
            scores[i]=NULL;
            //printf("done !\n");
        }
    }
}
