#include "genpiece.h"

unsigned char generationPieces(unsigned char locking, unsigned char preview)
{

    //Je génère entre 0 et 6 chosen
    unsigned char chosen;
    chosen = rand() % 7;

    //Je fais un bitwise OR des 3 variables
    unsigned char result;
    result = locking | preview | chosen;

    //Si le résultat == locking , je régènere entre 0 et 6 chosen et je refais un bitwise OR des 3 variables
    if (result == locking)
    {
        chosen = rand() % 7;
        result = locking | preview | chosen;

        //Si le résultat == locking , je regenere entre 0 et 6 chosen qui sera la piece final à return
        if (result == locking)
        {
            chosen = rand() % 7;
        }
    }

    return chosen;
}

MaillonChar *creerMaillon(char value)
{
    MaillonChar *newvalue = (MaillonChar*)malloc(sizeof(MaillonChar));

    newvalue->value = value;
    newvalue->suivant = NULL;

    return newvalue;
}

void enfilePiece(MaillonChar **premier, char value)
{
    MaillonChar *maillon = creerMaillon(value);
    if (*premier)
    {
        MaillonChar *temp = *premier;
        while (temp->suivant)
        {
            temp = temp->suivant;
        }
        temp->suivant = maillon;
    }
    else
    {
        *premier = maillon;
    }
}

char defilePiece(MaillonChar **premier)
{
    if (*premier)
    {
        MaillonChar *temp = *premier;
        *premier = (*premier)->suivant;

        char ret = temp->value;
        free(temp);

        return ret;
    }
    return -1;
}

void supprimeFile(MaillonChar **premier)
{
    while (*premier)
    {
        defilePiece(premier);
    }
}

char nouvellePiecesMulti(System *sys, char joueur)
{
    InfoPartie *infoPartie = &(sys->infoPartieJ1);
    MaillonChar **premierAct = &(sys->fileMulti.FileJ1);
    MaillonChar **premierAdv = &(sys->fileMulti.FileJ2);

    if (joueur == 2)
    {
        infoPartie = &(sys->infoPartieJ2);
        premierAct = &(sys->fileMulti.FileJ2);
        premierAdv = &(sys->fileMulti.FileJ1);
    }
    if (*premierAct)
    {
        char nouvellePieces = defilePiece(premierAct);
        return nouvellePieces;
    }
    else
    {
        char nouvellePieces = generationPieces(infoPartie->piece.id, infoPartie->idPieceFutur);
        enfilePiece(premierAdv, nouvellePieces);
        return nouvellePieces;
    }
}
