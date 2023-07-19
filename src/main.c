#include <stdio.h>
#include"interface.h"


int main(int argc, char **argv){

	srand(time(NULL));
	initialiseGfx(argc, argv);

	prepareFenetreGraphique("Tetri'ISEN", LargeurFenetre, HauteurFenetre);

	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();
    return 0;
}
//Test push//