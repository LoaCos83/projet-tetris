

#include "inputs.h"

void resetControleur(Controleur*ctr){
    ctr->dpad = dpad_none;
    ctr->action = action_none;
}

void appuisDpad(Controleur*ctr, DpadInput in){
    ctr->dpad = in;
}

void appuisAction(Controleur*ctr, ActionInput in){
    ctr->action = in;
}

Controleur initControleur(){
    Controleur manette = {dpad_none, action_none};
    return manette;
}