#pragma once
/**
 * \file persistance.h
 * \brief header pour les fonctions liées à la persistance des données
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * \struct Score
 * \brief Structure du score.
 * Contient Les infos relatives au score stocké dans un fichier
 */
typedef struct{
    char pseudo[6]; /*!< Pseudo de 6 caractères */
    long score; /*!< Score qui est au maximum de 999 999 */
}Score;


/**
 * \fn void enregistreScore(Score *scores[10])
 * \brief Enregistre une liste de 10 scores dans le fichier binaire de sauvegarde
 * \param scores Liste des 10 pointeurs sur scores à enregistrer
 * 
 * La liste contiendra des pointeurs sur Score aux emplacements remplis et des NULL aux emplacements vides
 * Si le fichier n'existe pas, il faut le créer
 */
void enregistreScore(Score *scores[10]);

/**
 * \fn void lisScore(Score *scores[10])
 * \brief Lis les scores dans le fichier binaire de sauvegarde
 * \param scores Liste de pointeur sur Score de longueur 10 dans laquelle on veux recevoir les scores
 * 
 * La liste contiendra des pointeurs sur Score aux emplacements remplis et des NULL aux emplacements vides
 * Si le fichier n'existe pas, il faut le créer et renvoyer une liste de NULL
 */
void lisScore(Score *scores[10]);

/**
 * \fn bool estDansLaListe(Score *scores[10], long scoreTest);
 * \brief Indique si le score de test mérite d'être dans la liste
 * \param scores Liste des pointeurs sur scores actellement enregistré
 * \param scoreTest score qu'il faut tester
 * \return Vrai si le score est cencé être dans la liste, Faux si le score est plus petit que le dernier de la liste
 */
bool estDansLaListe(Score *scores[10], long scoreTest);

/**
 * \fn void insereDansListe(Score *scores[10], Score score)
 * \brief Permet d'insérer dans la liste le score et garder la liste trié par odre décroissant
 * \param scores Liste des pointeurs sur scores actuellement enregistré
 * \param score Le Score à insérer
 */
void insereDansListe(Score *scores[10], Score *score);


/**
 * \fn void libereScores(Score *scores[10])
 * \brief Permet de libérer chaque pointeur sur score de la liste
 * \param scores Liste de pointeur sur Score à libéré
 * 
 * Libère les pointeurs et affecte la valeur NULL à chaque membre de la liste
 */
void libereScores(Score *scores[10]);