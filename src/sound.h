#pragma once
/**
 * \file sound.h
 * \brief function sound
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../fmod/fmod.h"

#define SON_SIMULTANES 3
#define ChannelMusique "channelMusique"
#define ChannelBruitage "channelBruitage"


/**
 * @brief   fonction permetant d'initialiser les données de la structure Son
 * 
 * @param donnees   structure Son a initialisé 
 */
void initDonneesSon(DonneesSon *donnees);

/**
 * @brief   fonction permettant d'attribuer un fichier audio à la variable placée en paramètre
 * 
 * @param system    system sur lequel on veut jouer le son
 * @param sonJoue   variable recupérant un fichier audio
 * @param mp3Sound  fichier audio attribuer à une variable
 */
void initSound(FMOD_SYSTEM *system, FMOD_SOUND **sonJoue, char* mp3Sound);

/**
 * @brief   fonction qui libère la mémoire des données utilisées pour les sons
 * 
 * @param donnees   structure est libérée 
 */
void deleteSystemSon(DonneesSon *donnees);

/**
 * @brief  fonction qui lance un son
 * 
 * @param system    system utilisé pour les sons
 * @param sonJoue   sons à jouer
 * @param channel   channel sur lequel jouer le son
 * @param channelgroup  channel sur lequel jouer le son
 */
void playSound(FMOD_SYSTEM *system, FMOD_SOUND *sonJoue, FMOD_CHANNEL **channel, FMOD_CHANNELGROUP *channelgroup);