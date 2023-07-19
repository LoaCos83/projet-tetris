#include "interface.h"
#include "sound.h"

//gcc -c sound/sound.c
//gcc -Wall sound.o -o sound libfmod.so libfmodL.so
/*typedef struct{
    FMOD_SYSTEM *systemSon;
    FMOD_SOUND *musique;
    FMOD_SOUND *bruitage[10];
    FMOD_CHANNELGROUP *channelGroupeMusique;
    FMOD_CHANNELGROUP *channelGroupeBruitage;
}DonneesSon;


    FMOD_ChannelGroup_SetVolume(channelGroupeMusique, slt/100.f);

    FMOD_ChannelGroup_SetVolume(channelGroupeBruitage, slt/100.f);*/

void initDonneesSon(DonneesSon *donnees){


    FMOD_System_Create(&(donnees->systemSon));
    FMOD_System_Init(donnees->systemSon, SON_SIMULTANES, FMOD_INIT_NORMAL, NULL);
    //FMOD_System_SetDSPBufferSize(donnees->systemSon,512,4);

    FMOD_System_CreateChannelGroup(donnees->systemSon, ChannelMusique, &(donnees->channelGroupeMusique));
    FMOD_System_CreateChannelGroup(donnees->systemSon, ChannelBruitage, &(donnees->channelGroupeBruitage));

    donnees->musiqueChange=true;

    initSound(donnees->systemSon, &(donnees->musique[0]), "sound/tetris_sound.mp3");
    FMOD_Sound_SetLoopCount(donnees->musique[0], -1);
    initSound(donnees->systemSon, &(donnees->musique[1]), "sound/tetrisB.mp3");
    FMOD_Sound_SetLoopCount(donnees->musique[1], -1);

    initSound(donnees->systemSon, &(donnees->bruitage[0]), "sound/validate_sound.wav");   //Son du bouton A dans le menu
    
    initSound(donnees->systemSon, &(donnees->bruitage[1]), "sound/cancel_sound.wav");   //Son du bouton B dans le menu

    initSound(donnees->systemSon, &(donnees->bruitage[2]), "sound/moving_sound.wav");   //Son du déplacement dans le menu (ZQSD) et jeu

    initSound(donnees->systemSon, &(donnees->bruitage[3]), "sound/rotate_sound.wav");   //Son de la rotation
    initSound(donnees->systemSon, &(donnees->bruitage[4]), "sound/linecomplete_sound.wav");   //Son de 1/2/3 lignes
    initSound(donnees->systemSon, &(donnees->bruitage[5]), "sound/4linescomplete_sound.wav");   //Son de 4 lignes
    initSound(donnees->systemSon, &(donnees->bruitage[6]), "sound/pausetetris.mp3");   //Son de la pause
    initSound(donnees->systemSon, &(donnees->bruitage[7]), "sound/changelvl_sound.wav");   //Son du changement de lvl
    initSound(donnees->systemSon, &(donnees->bruitage[8]), "sound/death_sound.wav");    //Son de la mort
    for(int i=0;i<9;i++){
        FMOD_Sound_SetLoopCount(donnees->bruitage[i], 0);
    }

}

void initSound(FMOD_SYSTEM *systemSon, FMOD_SOUND **sonJoue, char* mp3Sound){                             

    FMOD_RESULT resultat;

    resultat = FMOD_System_CreateSound(systemSon, mp3Sound, FMOD_2D | FMOD_LOOP_NORMAL, 0, sonJoue);
    if (resultat != FMOD_OK){
        fprintf(stderr, "Lecture impossible de la musique A!\n");
        exit(EXIT_FAILURE);
    }
}
void deleteSystemSon(DonneesSon *donnees){

    FMOD_Sound_Release(donnees->musique[0]);
    FMOD_Sound_Release(donnees->musique[1]);
    for(int  i=0; i<10; i++){
        FMOD_Sound_Release(donnees->bruitage[i]);
    }
    FMOD_ChannelGroup_Release(donnees->channelGroupeMusique); 
    FMOD_ChannelGroup_Release(donnees->channelGroupeBruitage);
    
    FMOD_System_Close(donnees->systemSon);
    FMOD_System_Release(donnees->systemSon);
}

void playSound(FMOD_SYSTEM *systemSon, FMOD_SOUND *sonJoue, FMOD_CHANNEL **channel, FMOD_CHANNELGROUP *channelgroup){

    FMOD_System_PlaySound(systemSon, sonJoue, channelgroup, 0, 0);
}