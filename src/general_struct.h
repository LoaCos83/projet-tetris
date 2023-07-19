#pragma once
/**
 * \file general_struct.h
 * \brief Structure pour l'application
 * \version 1
 * 
 * 
 **/
#define GRILLE_COLONNES 10
#define GRILLE_LIGNES 18

#include "../gfxlib/BmpLib.h"
#include "inputs.h"

#include "../fmod/fmod.h"
#include "persistance.h"

//genpiece
/**
 * \brief Permet le bon défilement des pièces (que quand une pièce se fige, une nouvelle apparaisse)
 * 
 **/
typedef struct _MaillonChar MaillonChar;
struct _MaillonChar
{
    char value;
    MaillonChar *suivant;
};

/**
 * \brief génère les files des pièces en jeu solo et multi
 * 
 **/
typedef struct
{
    MaillonChar *FileJ1;
    MaillonChar *FileJ2;
} FilePiece;

//engine
/**
 * \brief Vecteur de déplacement, pour localiser une tile
 * 
 **/
typedef struct
{
    int x;
    int y;
} Coord_Piece;

/**
 * \brief Toutes variables concernant la pièce (forme du tétramino, état de rotation)
 * 
 **/
typedef struct
{
    char id;
    char state;
    Coord_Piece tiles[4];
} Piece;

/**
 * \brief Toutes les variables permettant le jeu, le score, le niveau, le nombre de lignes validé, la pièce futur, la vitesse de chute des pièces
 * 
 **/
typedef struct
{
    bool fin;
    int niveau;
    int niveauDepart;
    int totalLigne;
    long score;
    char grille[GRILLE_COLONNES][GRILLE_LIGNES];
    Piece piece;
    char idPieceFutur;
    long long msOfLastDown;
    long long msOfLastPause;
    bool idle;
} InfoPartie;

//interface
/**
 * \brief Toutes les fenetres du jeu
 * 
 **/
typedef enum
{
    Accueil,
    Solo,
    Multi,
    Scoreboard,
    JeuSolo,
    JeuMulti,
    Option,
} Page;

/**
 * \brief Indique quand on est en jeu ou pas,en fin de partie et si on est rentré dans le scoreboard
 * 
 **/
typedef enum{
    PasEnJeu,
    JeuEncours,
    AffichageScores,
    DemandePseudo
} EtatJeu;

/**
 * \brief Permet le déplacement, le retour, la validation dans la page de la sélection du niveau en solo et multi
 * 
 **/
typedef struct
{
    Page page;
    int Vselection;
    int Hselection;
    EtatJeu etaJeu;
} InfoMenu;

/**
 * \brief Permet de déplacement vertical dans la page option
 * 
 **/
typedef struct
{
    int Hchoix;
    int Hmusique;
    int Hbruitage;
} InfoMusique;

/**
 * \brief Structure gérant les sons
 * 
 **/
typedef struct{
    bool musiqueChange;
    FMOD_SYSTEM *systemSon;
    FMOD_SOUND *musique[2];
    FMOD_SOUND *bruitage[20];
    FMOD_CHANNELGROUP *channelGroupeMusique;
    FMOD_CHANNELGROUP *channelGroupeBruitage;
}DonneesSon;

/**
 * \brief structure pour les variables du pseudo
 * 
 **/
typedef struct{
    char pseudo[6];
    char ptrPseudo;
}Pseudo;

/**
 * \brief Structure de variable globales
 *
 **/
typedef struct
{
    InfoMenu infoMenu;
    InfoMusique infoMusique;
    Controleur controleur;
    Controleur controleur2;
    DonneesImageRGB *imageFond;
    DonneesSon donneesSon;

    Score *scores[10];

    Pseudo pseudo;
    InfoPartie infoPartieJ1;
    InfoPartie infoPartieJ2;
    bool isGamePaused;
    FilePiece fileMulti;
} System;
