#include "engine.h"

void initGrille(char grille[][GRILLE_LIGNES])
{
    for (int i = 0; i < GRILLE_COLONNES; i++)
    {
        for (int j = 0; j < GRILLE_LIGNES; j++)
        {
            grille[i][j] = -1;
        }
    }
}

void initMsPerRow()
{
    int _msPerRow[21] = {
        887,
        820,
        753,
        686,
        619,
        552,
        469,
        368,
        285,
        184,
        167,
        151,
        134,
        117,
        100,
        100,
        84,
        84,
        67,
        67,
        50};
    for (int i = 0; i < 21; i++)
    {
        msPerRow[i] = _msPerRow[i];
    }
}

long long getMsTimeStamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + (long long)(tv.tv_usec / 1000.f);
}

void initPiece(Piece *p, char id)
{
    p->id = id;
    p->state = 0;
    PieceEnum pid = (PieceEnum)id;
    switch (pid)
    {
    case Piece_S:
        p->tiles[0] = (Coord_Piece){4, 16};
        p->tiles[1] = (Coord_Piece){4, 15};
        p->tiles[2] = (Coord_Piece){5, 16};
        p->tiles[3] = (Coord_Piece){3, 15};
        break;
    case Piece_Z:
        p->tiles[0] = (Coord_Piece){3, 16};
        p->tiles[1] = (Coord_Piece){4, 16};
        p->tiles[2] = (Coord_Piece){4, 15};
        p->tiles[3] = (Coord_Piece){5, 15};
        break;
    case Piece_T:
        p->tiles[0] = (Coord_Piece){4, 16};
        p->tiles[1] = (Coord_Piece){3, 16};
        p->tiles[2] = (Coord_Piece){5, 16};
        p->tiles[3] = (Coord_Piece){4, 15};
        break;
    case Piece_L:
        p->tiles[0] = (Coord_Piece){4, 16};
        p->tiles[1] = (Coord_Piece){3, 16};
        p->tiles[2] = (Coord_Piece){5, 16};
        p->tiles[3] = (Coord_Piece){3, 15};
        break;
    case Piece_J:
        p->tiles[0] = (Coord_Piece){4, 16};
        p->tiles[1] = (Coord_Piece){3, 16};
        p->tiles[2] = (Coord_Piece){5, 16};
        p->tiles[3] = (Coord_Piece){5, 15};
        break;
    case Piece_O:
        p->tiles[0] = (Coord_Piece){4, 16};
        p->tiles[1] = (Coord_Piece){5, 16};
        p->tiles[2] = (Coord_Piece){4, 15};
        p->tiles[3] = (Coord_Piece){5, 15};
        break;
    case Piece_I:
        p->tiles[0] = (Coord_Piece){4, 16};
        p->tiles[1] = (Coord_Piece){3, 16};
        p->tiles[2] = (Coord_Piece){5, 16};
        p->tiles[3] = (Coord_Piece){6, 16};
        break;

    default:
        break;
    }
}

Coord_Piece initCoordPiece(int x, int y)
{
    Coord_Piece cp = {x, y};
    return cp;
}

void initInfoPartieSolo(System *system, int niveauDepart)
{
    initInfoPartie(&(system->infoPartieJ1), niveauDepart, -1, -1);
}

void initInfoPartieMulti(System *system, int niveauDepart)
{
    char randp = rand() % 7;
    char randf = rand() % 7;
    initInfoPartie(&(system->infoPartieJ1), niveauDepart, randp, randf);
    initInfoPartie(&(system->infoPartieJ2), niveauDepart, randp, randf);
    supprimeFile(&(system->fileMulti.FileJ1));
    supprimeFile(&(system->fileMulti.FileJ2));
}

void initInfoPartie(InfoPartie *ip, int niveauDepart, char randp, char randf)
{
    ip->niveau = niveauDepart;
    ip->niveauDepart = niveauDepart;
    ip->totalLigne = 0;
    ip->score = 0;

    ip->msOfLastDown = 0;
    ip->msOfLastPause = 0;

    initGrille(ip->grille);
    initPiece(&(ip->piece), (randp != -1) ? randp : rand() % 7);
    ip->idPieceFutur = (randf != -1) ? randf : rand() % 7;
}

void nouvellePieces(System *system, InfoPartie *ip, char joueur, char nbjoueur)
{
    unsigned char locking = ip->piece.id;
    initPiece(&(ip->piece), ip->idPieceFutur);
    if (nbjoueur == 1)
    {
        ip->idPieceFutur = generationPieces(locking, ip->idPieceFutur);
    }
    else
    {
        ip->idPieceFutur = nouvellePiecesMulti(system, joueur);
    }
}

bool valideFuturPosition(InfoPartie *ip, Piece futurPiece)
{
    for (int i = 0; i < 4; i++)
    {
        Coord_Piece cp = futurPiece.tiles[i];
        if (cp.x >= 0 && cp.x < GRILLE_COLONNES && cp.y >= 0 && cp.y < GRILLE_LIGNES)
        {
            if (ip->grille[cp.x][cp.y] != -1)
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

void freezePiece(InfoPartie *ip)
{
    for (int i = 0; i < 4; i++)
    {
        Coord_Piece cp = ip->piece.tiles[i];
        ip->grille[cp.x][cp.y] = ip->piece.id;
    }
}

bool deplacePiece(InfoPartie *ip, Coord_Piece deplacement)
{
    Piece futurPiece = ip->piece;
    for (int i = 0; i < 4; i++)
    {
        futurPiece.tiles[i].x += deplacement.x;
        futurPiece.tiles[i].y += deplacement.y;
    }
    if (valideFuturPosition(ip, futurPiece))
    {
        ip->piece = futurPiece;
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}

bool tournePiece(InfoPartie *ip, char sens)
{
    Piece piece = ip->piece;
    piece.state += sens;
    char cas = 0;
    switch ((PieceEnum)piece.id)
    {
    case Piece_S:
    case Piece_I:
    case Piece_Z:
        if (piece.state < 0)
        {
            piece.state = 1;
            sens *= -1;
        }
        if (piece.state > 1)
        {
            piece.state = 0;
            sens *= -1;
        }
        break;
    case Piece_L:
    case Piece_J:
    case Piece_T:

        if (sens > 0)
        {
            cas = piece.state - 1;
        }
        else
        {
            cas = piece.state;
        }

        if (piece.state < 0)
        {
            piece.state = 3;
            cas = 3;
        }
        if (piece.state > 3)
        {
            piece.state = 0;
            cas = 3;
        }
        break;
    default:
        break;
    }
    switch ((PieceEnum)piece.id)
    {
    case Piece_S:
        deplaceTile(piece.tiles + 2, -2 * sens, sens);
        deplaceTile(piece.tiles + 3, 0, sens);
        break;
    case Piece_Z:
        deplaceTile(piece.tiles + 2, -1 * sens, 0);
        deplaceTile(piece.tiles + 3, -1 * sens, 2 * sens);
        break;
    case Piece_I:
        deplaceTile(piece.tiles + 1, sens, -1 * sens);
        deplaceTile(piece.tiles + 2, -1 * sens, sens);
        deplaceTile(piece.tiles + 3, -2 * sens, 2 * sens);
        break;
    case Piece_L:
        switch (cas)
        {
        case 0:
            deplaceTile(piece.tiles + 1, sens, sens);
            deplaceTile(piece.tiles + 2, -sens, -sens);
            deplaceTile(piece.tiles + 3, 0, 2 * sens);
            break;
        case 1:
            deplaceTile(piece.tiles + 1, sens, -sens);
            deplaceTile(piece.tiles + 2, -sens, sens);
            deplaceTile(piece.tiles + 3, 2 * sens, 0);
            break;
        case 2:
            deplaceTile(piece.tiles + 1, -sens, -sens);
            deplaceTile(piece.tiles + 2, sens, sens);
            deplaceTile(piece.tiles + 3, 0, -2 * sens);
            break;
        case 3:
            deplaceTile(piece.tiles + 1, -sens, sens);
            deplaceTile(piece.tiles + 2, sens, -sens);
            deplaceTile(piece.tiles + 3, -2 * sens, 0);
            break;
        }
        break;
    case Piece_J:
        switch (cas)
        {
        case 0:
            deplaceTile(piece.tiles + 1, sens, sens);
            deplaceTile(piece.tiles + 2, -sens, -sens);
            deplaceTile(piece.tiles + 3, -2 * sens, 0);
            break;
        case 1:
            deplaceTile(piece.tiles + 1, sens, -sens);
            deplaceTile(piece.tiles + 2, -sens, sens);
            deplaceTile(piece.tiles + 3, 0, 2 * sens);
            break;
        case 2:
            deplaceTile(piece.tiles + 1, -sens, -sens);
            deplaceTile(piece.tiles + 2, sens, sens);
            deplaceTile(piece.tiles + 3, 2 * sens, 0);
            break;
        case 3:
            deplaceTile(piece.tiles + 1, -sens, sens);
            deplaceTile(piece.tiles + 2, sens, -sens);
            deplaceTile(piece.tiles + 3, 0, -2 * sens);
            break;
        }
        break;
    case Piece_T:
        switch (cas)
        {
        case 0:
            deplaceTile(piece.tiles + 1, sens, sens);
            deplaceTile(piece.tiles + 2, -sens, -sens);
            deplaceTile(piece.tiles + 3, -sens, sens);
            break;
        case 1:
            deplaceTile(piece.tiles + 1, sens, -sens);
            deplaceTile(piece.tiles + 2, -sens, sens);
            deplaceTile(piece.tiles + 3, sens, sens);
            break;
        case 2:
            deplaceTile(piece.tiles + 1, -sens, -sens);
            deplaceTile(piece.tiles + 2, sens, sens);
            deplaceTile(piece.tiles + 3, sens, -sens);
            break;
        case 3:
            deplaceTile(piece.tiles + 1, -sens, sens);
            deplaceTile(piece.tiles + 2, sens, -sens);
            deplaceTile(piece.tiles + 3, -sens, -sens);
            break;
        }
        break;
    default:
        break;
    }

    if (valideFuturPosition(ip, piece))
    {
        ip->piece = piece;
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}

void deplaceTile(Coord_Piece *co, char x, char y)
{
    co->x += x;
    co->y += y;
}

void checkLine(InfoPartie *ip, int *ligne, int *nblignes)
{
    *ligne = -1;
    *nblignes = 0;
    for (int j = 0; j < GRILLE_LIGNES; j++)
    {
        bool ligneComplete = 1; //considère la ligne complète
        for (int i = 0; i < GRILLE_COLONNES; i++)
        {
            if (ip->grille[i][j] == -1)
            {
                ligneComplete = 0; // trous => ligne pas complète
                i = GRILLE_COLONNES;
            }
        }
        // vérification si ligne complète
        if (ligneComplete)
        {
            if (*ligne == -1)
            {
                *ligne = j;
            }
            // de part la nature du jeu, il est impossible d'avoit plus de 4 lignes
            (*nblignes)++;
        }
    }
}

void deplaceToutAuDessus(InfoPartie *ip, int ligne, int nblignes)
{
    for (int i = 0; i < GRILLE_COLONNES; i++)
    {
        for (int j = ligne; j < GRILLE_LIGNES - nblignes; j++)
        {
            ip->grille[i][j] = ip->grille[i][j + nblignes];
            ip->grille[i][j + nblignes] = -1;
        }
    }
}

void addScore(InfoPartie *ip, int nblignes)
{
    int scorelevel[4] = {40, 100, 300, 1200};
    ip->score += scorelevel[nblignes - 1] * (ip->niveau + 1);
}

void calculNiveau(InfoPartie *ip)
{
    if (ip->totalLigne / 10 > ip->niveauDepart)
    {
        ip->niveau = ip->totalLigne / 10;
    }
}

void gestionControle(System *system)
{

    if (system->controleur.dpad == dpad_haut || system->controleur2.dpad == dpad_haut)
    {
        gerePause(system);
    }

    if (!system->isGamePaused){

        if(system->controleur.dpad == dpad_droite || system->controleur.dpad == dpad_gauche){
            playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[2], 0, system->donneesSon.channelGroupeBruitage);
	    } else if(system->controleur.action == action_A || system->controleur.action == action_B){
			playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[3], 0, system->donneesSon.channelGroupeBruitage);
        }

        //joueur 1
        gestionControleIP(&(system->infoPartieJ1), &(system->controleur));
        //joueur 2
        gestionControleIP(&(system->infoPartieJ2), &(system->controleur2));
    }
}

void gestionControleIP(InfoPartie *ip, Controleur *controleur)
{
    if (!ip->idle && !ip->fin)
    {
        if (controleur->dpad == dpad_droite)
        {
            deplacePiece(ip, DEP_DROITE);
        }
        else if (controleur->dpad == dpad_gauche)
        {
            deplacePiece(ip, DEP_GAUCHE);
        }
        else if (controleur->dpad == dpad_bas)
        {
            deplacePiece(ip, DEP_BAS);
        }
        else if (controleur->action == action_A)
        {
            //tourner dans le sens positif
            tournePiece(ip, 1);
        }
        else if (controleur->action == action_B)
        {
            //tourner dans le sens négatif
            tournePiece(ip, -1);
        }
    }
    resetControleur(controleur);
}

void gerePause(System *system)
{
    long long ts = getMsTimeStamp();
    if (system->isGamePaused)
    {
        system->isGamePaused = false;
        system->infoPartieJ1.msOfLastDown += ts - system->infoPartieJ1.msOfLastPause;
        system->infoPartieJ2.msOfLastDown += ts - system->infoPartieJ2.msOfLastPause;
        FMOD_ChannelGroup_SetPaused(system->donneesSon.channelGroupeMusique,false);
    }
    else
    {
        playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[6], 0, system->donneesSon.channelGroupeBruitage);
        FMOD_ChannelGroup_SetPaused(system->donneesSon.channelGroupeMusique,true);
        system->isGamePaused = true;
        system->infoPartieJ1.msOfLastPause = ts;
        system->infoPartieJ2.msOfLastPause = ts;
    }
}

void timeStep(System *system, char joueur, char nbjoueur)
{
    InfoPartie *infoPartie = &(system->infoPartieJ1);
    if (joueur == 2)
    {
        infoPartie = &(system->infoPartieJ2);
    }
    int niveau = (infoPartie->niveau > 20) ? 20 : infoPartie->niveau; //si le niveau est > à 21 on prend 20 pour ne pas sortir du tableau msPerRow
    if (getMsTimeStamp() - infoPartie->msOfLastDown >= msPerRow[niveau])
    {
        infoPartie->idle = false;
        long long tempsAdditionel = 0;
        if (!deplacePiece(infoPartie, DEP_BAS))
        {
            tempsAdditionel = 33; //2 frames de pause après qu'une pièce soit freeze
            infoPartie->idle = true;
            freezePiece(infoPartie); //on met la piece sur la grille
            int ligne = 0;
            int nbligne = 0;
            int totalligne=0;
            checkLine(infoPartie, &ligne, &nbligne); //on regarde si une ou des ligne sont pleines
            while (nbligne > 0)                         //si il y a des ligne
            {
                if(nbligne==4){
                    playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[5], 0, system->donneesSon.channelGroupeBruitage);
                }else{
                    playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[4], 0, system->donneesSon.channelGroupeBruitage);
                }
                deplaceToutAuDessus(infoPartie, ligne, nbligne); //on déplace les tiles de la grille pour enlever les lignes pleine
                totalligne+=nbligne;
                checkLine(infoPartie, &ligne, &nbligne);// il y a une possibilité qu'il y ait plusieur groupe de lignes
            }
            if(totalligne>0){
                addScore(infoPartie, totalligne);                   //on ajoute le score en fonction du nombre de ligne
                infoPartie->totalLigne += totalligne;               //on ajoute les lignes au compteur de ligne
                calculNiveau(infoPartie);                        //on recalcul le niveau en fonction des lignes
                tempsAdditionel = 1557;                          //93 frame de pause si une ligne est faite
                infoPartie->idle = true;
            }

            nouvellePieces(system, infoPartie, joueur, nbjoueur); //on fait un cycle de piece

            if (!valideFuturPosition(infoPartie, infoPartie->piece))
            {
                //gerePause(system);
                infoPartie->fin = true;
                playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[8], 0, system->donneesSon.channelGroupeBruitage);
                if (system->infoMenu.page == JeuSolo)
                {
                    if(infoPartie->score > 0 && estDansLaListe(system->scores,infoPartie->score)){
                        system->infoMenu.etaJeu = DemandePseudo;
                    }else{
                        system->infoMenu.etaJeu = AffichageScores;
                    }
                }
                if (system->infoPartieJ1.fin && system->infoPartieJ2.fin)
                {
                    system->infoMenu.etaJeu = AffichageScores;
                }
            }
        }
        infoPartie->msOfLastDown = getMsTimeStamp() + tempsAdditionel; //on reset le timestamp de la dernière descante
    }
}
