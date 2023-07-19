#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * \file interface.h
 * \brief Collection de fonctions pour la boucle d'événement ainsi que l'affichage
 * \version 1
 * \date 2021-05-03
 * 
 * 
 **/

#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h>    // Pour pouvoir utiliser printf()
#include <string.h>
#include <time.h>

#include <math.h>    // Pour pouvoir utiliser sin() et cos()
#include "../gfxlib/GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "../gfxlib/BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "../gfxlib/ESLib.h"    // Pour utiliser valeurAleatoire()
// Largeur et hauteur par defaut d'une image correspondant a nos criteres

#define LargeurFenetre 1600
#define HauteurFenetre 900

#define MENU_LIGNES 5
#define OPTION_LIGNES 2

#define NB_LIGNES 20
#define NB_COLONNES 14

#define HAUTEUR_CASE 30
#define LONGUEUR_CASE 30

#define LARGEUR_BORDURE_AVANT_GRILLE 200

#define ID_VIDE X
#define ID_FORME F
#define ID_MUR M

#include "inputs.h"


/**
 *
 */


#include "general_struct.h"

#include "engine.h"
#include "genpiece.h"

#include "sound.h"

#include "persistance.h"

/**
 * \brief Initialisation de toutes les variables utiles au bon fonctionnement de l'application
 * 
 * \param s Pointeur sur la structure de variable globales
 **/
void initSystem(System *s);

/**
 * \brief Initialise la structure InfoMenu
 * 
 * \return InfoMenu Structure initialisé
 **/
InfoMenu initInfoMenu();

/**
 * \brief Initialise la structure InfoMusique
 * 
 * \return InfoMusique Structure initialisé
 **/
InfoMusique initInfoMusique();

/**
 * \brief Initialise la structure Pseudo
 * 
 * \return Pseudo Structure initialisé
 **/
Pseudo initPseudo();

/**
 * \brief Effectue les test de la navigation suivant l'input de l'utilisateur
 * 
 * \param system Pointeur sur la structure de variable globales
 **/
void navigueMenu(System *system);

/**
 * \brief Change la page actuelle vers la page passée en paramètre
 * 
 * \param menu Pointeur sur la structure de variable globales
 * \param p page sur laquelle on veut aller parmis l'enumeration Page
 **/
void changePage(System *menu, Page p);


/**
 * \brief Libérer la mémoire de tout ce qui était alloué dans la mémoire
 *
 * \param system  pointeur vers les structures de variables globales
 **/
void freeSystem(System *system);


/**
 * \brief Ferme la fenetre du jeu
 *
 * \param system pointeur vers les structures de variables globales
 **/
void quitterGestionEvenement(System *system);

/**
 * \brief La fonction de gestion des evenements, appelee automatiquement par le systeme
 *  des qu'une evenement survient
 *
 * \param evenement Contient les fonctions graphique
 **/
void gestionEvenement(EvenementGfx evenement);

/**
 * \brief Génère la page d'accueil du jeu
 *
 * \param system pointeur vers les structures de variables globales
 **/
void PageDemarrage1( System *system);

/**
 * \brief Génère la page des scores du jeu
 *
 * \param system pointeur vers les structures de variables globales
 **/
void PageScore(System *system);

/**
 * \brief Génère la page de sélection du niveau de difficulté du jeu
 *
 * \param system pointeur vers les structures de variables globales
 **/
void PageDepart( System *system);

/**
 * \brief Génère la page jouable du jeu en solo
 *
 * \param system pointeur vers les structures de variables globales
 **/
void PageJeu(System *system);

/**
 * \brief Génère la page jouable du jeu en multijoueur
 *
 * \param system pointeur vers les structures de variables globales
 **/
void PageJeu2(System *system);

/**
 * \brief Génère la page qui permet de modifier les options du jeu
 *
 * \param system pointeur vers les structures de variables globales
 **/
void PageOption(System *system);

/**
 * \brief Génère la grille du jeu en solo
 *
 * \param system pointeur vers les structures de variables globales
 * \param joueur Permet de positionner correctement les grilles en fonctions du nombre de joueur
 * \param mode Indique si on joue en solo ou en multi
 **/
void creerGrille(System *system, char joueur, char mode);

/**
 * \brief Génère la grille du jeu en multi
 *
 * \param system pointeur vers les structures de variables globales
 * \param joueur Permet de positionner correctement les grilles en fonctions du nombre de joueur
 * \param mode Indique si on joue en solo ou en multi
 **/
void creerGrille2(System *system, char joueur, char mode);

/**
 * \brief Affiche la pièce suivante
 *
 * \param idPieceFutur Indique quel tetramino qui sera la pièce future
 * \return Piece
 **/
Piece creerFuturPieces(int idPieceFutur);

/**
 * \brief Colorise les tétrominos de couleur différente
 *
 * \param id Indique quel tetramino aura sa couleur associée
 **/
void changeCouleurId(int id);


#endif
