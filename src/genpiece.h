#pragma once

/**
 * \file genpiece.h
 * \brief Gestion de le génération aléatoire des pièces
 */

#ifndef GENPIECE_H
#define GENPIECE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "general_struct.h"

/**
 * \brief Fonction qui génère l'id de la future pièce en effectuant une opération OU logique.
 * 
 * \param locking Pièce actuelle qui est en jeu dans la grille
 * \param preview Prochaine pièce généré aléatoirement
 * \return unsigned char Retourne l'id de la prochaine pièce après plusieurs vérifications avec nos paramètres
* */
unsigned char generationPieces(unsigned char locking, unsigned char preview);

/**
 * \brief Fonction permettant créer un maillon avec une certaine valeur passé en paramètre
 * 
 * \param value Valeur avec laquelle notre maillon sera créé
 * \return MaillonChar* Retourne un pointeur sur le maillon
* */
MaillonChar *creerMaillon(char value);

/**
 * \brief Fonction permettant d'enfiler une valeur sur un maillon
 * 
 * \param maillon Pointeur sur un pointeur d'un maillon
 * \param value Valeur avec laquelle on va enfiler notre maillon
* */
void enfilePiece(MaillonChar **maillon, char value);

/**
 * \brief Fonction permettant de défiler sur un maillon
 * 
 * \param premier Pointeur sur un pointeur de la première valeur d'un maillon
 * \return char Retourne la valeur sur laquelle on pointe
* */
char defilePiece(MaillonChar **premier);

/**
 * \brief Fonction permettant de supprimer une valeur dans la file
 * s
 * \param premier Pointeur sur un pointeur de la première valeur d'un maillon
* */
void supprimeFile(MaillonChar **premier);

/**
 * \brief Fonction permettant de gérer la génération de nouvelles pièces pour le mode multijoueur
 * 
 * \param sys Pointeur sur la structure de variable global
 * \param joueur Joueur actuel
 * \return char Retourne la nouvelle pièce associé au joueur
* */
char nouvellePiecesMulti(System *sys, char joueur);

#endif