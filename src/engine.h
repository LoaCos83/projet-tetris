#pragma once
/**
 * \file engine.h
 * \brief Fonctions pour le moteur du jeu
**/

#include <stdbool.h>

#include <sys/time.h> // Pour pouvoir utiliser gettimeofday()

#include "general_struct.h"
#include "genpiece.h"
#include "sound.h"

#define DEP_BAS initCoordPiece(0, -1)
#define DEP_DROITE initCoordPiece(1, 0)
#define DEP_GAUCHE initCoordPiece(-1, 0)

/**
 * \brief Liste des pièces avec une valeur associé utilisé dans le générateur de pièce
**/
typedef enum
{
    Piece_S = 5,
    Piece_Z = 4,
    Piece_T = 6,
    Piece_L = 0,
    Piece_J = 1,
    Piece_O = 3,
    Piece_I = 2
} PieceEnum;

/**
 * \brief Nombre de ms entre chaque descente automatique de pièce suivant le niveau
 * 
 **/
int msPerRow[21];

/**
 * \param grille La grille qu'il faut initialiser
 * \brief Initialise une grille avec des -1 avec GRILLE_COLONNES colones et GRILLE_LIGNES lignes
**/
void initGrille(char grille[][GRILLE_LIGNES]);

/**
 * \brief Initialise la variable global msPerRow
**/
void initMsPerRow();

/**
 * \return Le timestamp en millisecondes
 * \brief Permet d'obtenir le timestamp en millisecondes à partir de gettimeofday()
**/
long long getMsTimeStamp();

/**
 * \param p Pointeur sur la pièce à initialiser
 * \param id Id de la piece à initialiser
 * \brief Initialise une pièce avec l'id spécifié
**/
void initPiece(Piece *p, char id);

/**
 * \param x Valeur initiale de x
 * \param y Valeur initiale de y
 * \return Renvoie une structure Coord_Piece initialisé avec les coordonnées spécifiées
**/
Coord_Piece initCoordPiece(int x, int y);

/**
 * \brief Raccourci pour initialiser l'InfoPartie de J1
 * 
 * \param system Pointeur vers la structure de variables globales
 * \param niveauDepart Niveau de départ
 **/
void initInfoPartieSolo(System *system, int niveauDepart);

/**
 * \brief Raccourci pour initialiser les deux InfoPartie avec synchronisation des pièces de départ
 * 
 * \param system Pointeur vers la structure de variables globales
 * \param niveauDepart Niveau de départ
 **/
void initInfoPartieMulti(System *system, int niveauDepart);

/**
 * \param ip Pointeur sur InfoPartie qui doit être initialisé
 * \param niveauDepart Niveau de départ
 * \brief Initialise l'InfoPartie avec le niveau de départ
**/
void initInfoPartie(InfoPartie *ip, int niveauDepart, char randp, char randf);

/**
 * \param ip Pointeur sur InfoPartie qui doit changer de pièce
 * \brief Change la pièce active avec la pièce en réserve et utilise le générateur de pièce pour générer la suivante
**/
void nouvellePieces(System *system, InfoPartie *ip, char joueur, char nbjoueur);

/**
 * \brief Indique si la position de la pièce passé en paramètre est valide dans la grille de l'InfoPartie
 * 
 * \param ip InfoPartie qui contient la grille sur laquelle on veut tester
 * \param futurPiece Piece que l'on veut tester
 * \return true : Position valide
 * \return false : Position invalide
 **/
bool valideFuturPosition(InfoPartie *ip, Piece futurPiece);

/**
 * \brief Fixe la pièce active dans la grille
 * 
 * \param ip InfoPartie à freezer
 **/
void freezePiece(InfoPartie *ip);

/**
 * \brief Déplace la pièce active suivant un vecteur décrit par le deplacement passé en paramètre
 * 
 * \param ip InfoPartie où le déplacement à lieu
 * \param deplacement Vecteur indiquant la direction
 * \return true : La position obtenue est valide
 * \return false : La position obtenue est invalide
 **/
bool deplacePiece(InfoPartie *ip, Coord_Piece deplacement);

/**
 * \brief Tourne la pièce active suivant le sens passé en paramètre
 * 
 * \param ip InfoPartie où la rotation à lieu
 * \param sens Sens de rotation : 1 ou -1
 * \return true : La position obtenue est valide
 * \return false : La position obtenue est invalide
 **/
bool tournePiece(InfoPartie *ip, char sens);

/**
 * \brief Assigne les coordonnées passées en paramètre à la position de la tile passé en paramètre
 * 
 * \param co Coordonnées à changer
 * \param x Nouveau x
 * \param y Nouveau y
 **/
void deplaceTile(Coord_Piece *co, char x, char y);

/**
 * \brief Regarde si il y a des lignes complètes dans la grille
 * 
 * \param ip InfoPartie qui contient la grille à vérifier
 * \param ligne Pointeur sur la première ligne complète
 * \param nblignes Pointeur sur le nombre de ligne complètes
 **/
void checkLine(InfoPartie *ip, int *ligne, int *nblignes);

/**
 * \brief Supprime nblignes au dessus de la ligne passé en paramètre et fait tout descendre
 * 
 * \param ip InfoPartie concerné
 * \param ligne ligne de départ
 * \param nblignes nombre de ligne(s) à supprimer
 **/
void deplaceToutAuDessus(InfoPartie *ip, int ligne, int nblignes);

/**
 * \brief Ajoute le score suivant le nombre de lignes passer en paramètre et suivant le niveau actuel
 * 
 * \param ip InfoPartie du joueur
 * \param nblignes nombre de lignes
 **/
void addScore(InfoPartie *ip, int nblignes);

/**
 * \brief calcul le niveau en fonction du nombre de lignes
 * 
 * \param ip InfoPartie du joueur
 **/
void calculNiveau(InfoPartie *ip);

/**
 * \brief Gestion des actions sur la partie en fonction des inputs des joueurs
 * 
 * \param system Pointeur sur la structure de variables globals
 **/
void gestionControle(System *system);

/**
 * \brief Active ou désactive la pause
 * 
 * \param system Pointeur sur la structure de variables globals
 **/
void gerePause(System *system);

/**
 * \brief Appel vers toutes les fonction pour faire les actions sur une frame
 * 
 * \param system Pointeur sur la structure de variables globals
 * \param joueur Indique quel joueur est concerné : 1 ou 2
 **/
void timeStep(System *system, char joueur, char players);

/**
 * \brief effectue la rotation ou le déplacement d'une pièce en fonction des inputs du joueur concerné
 * 
 * \param ip Pointeur sur l'InfoPartie du joueur concerné
 * \param controleur Controleur du joueur concerné
 **/
void gestionControleIP(InfoPartie *ip, Controleur *controleur);