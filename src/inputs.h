#pragma once
/**
 * \file inputs.h
 * \brief Header pour le contrôleur
 */

/**
 * \enum DpadInput
 * \brief Etats du Dpad
 */
typedef enum{
    dpad_none, /*!< Rien appuyé */
    dpad_haut, /*!< haut appuyé */
    dpad_bas, /*!< bas appuyé */
    dpad_droite, /*!< droite appuyé */
    dpad_gauche /*!< gauche appuyé */
}DpadInput;

/**
 * \enum ActionInput
 * \brief Etats des boutons action
 */
typedef enum{
    action_none, /*!< Rien appuyé */
    action_A, /*!< A appuyé */
    action_B /*!< B appuyé */
}ActionInput;

/**
 * \struct Controleur
 * \brief Structure indiquand l'état des boutons d'un contrôleur
 */
typedef struct{
    DpadInput dpad; /*!< Etat du Dpad */
    ActionInput action; /*!< Etat des boutons action */
}Controleur;

/**
 * \fn void resetControleur(Controleur*ctr)
 * \brief Remet à zero l'état contrôleur
 * \param ctr Controleur à remettre à zero
 */
void resetControleur(Controleur*ctr);

/**
 * \fn void appuisDpad(Controleur*ctr)
 * \brief Appuis sur un bouton du Dpad
 * \param ctr Controleur sur lequel on appuis
 * \param in Etat de l'appuis
 */
void appuisDpad(Controleur*ctr, DpadInput in);

/**
 * \fn void appuisAction(Controleur*ctr)
 * \brief Appuis sur un bouton action (A ou B)
 * \param ctr Controleur sur lequel on appuis
 * \param in Etat de l'appuis
 */
void appuisAction(Controleur*ctr, ActionInput in);


Controleur initControleur();