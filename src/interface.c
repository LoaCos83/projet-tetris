#include "interface.h"

void initSystem(System *s)
{
	s->infoMenu = initInfoMenu(); //Page,Vertical,Horizontal
	s->infoMusique = initInfoMusique();
	s->pseudo = initPseudo();

	s->controleur = initControleur();
	s->controleur2 = initControleur();

	lisScore(s->scores);

	s->imageFond = NULL;
	initDonneesSon(&(s->donneesSon));
	FMOD_ChannelGroup_SetVolume(s->donneesSon.channelGroupeMusique, s->infoMusique.Hmusique / 10.f);
	FMOD_ChannelGroup_SetVolume(s->donneesSon.channelGroupeBruitage, s->infoMusique.Hbruitage / 10.f);

	playSound(s->donneesSon.systemSon, s->donneesSon.musique[0], 0, s->donneesSon.channelGroupeMusique);

	initInfoPartieMulti(s, 0);
	s->isGamePaused = false;

	supprimeFile(&(s->fileMulti.FileJ1));
	supprimeFile(&(s->fileMulti.FileJ2));
}

void freeSystem(System *system)
{
	libereDonneesImageRGB(&(system->imageFond));
	supprimeFile(&(system->fileMulti.FileJ1));
	supprimeFile(&(system->fileMulti.FileJ2));

	deleteSystemSon(&(system->donneesSon));

	enregistreScore(system->scores);
	libereScores(system->scores);
}

void quitterGestionEvenement(System *system)
{
	freeSystem(system);
	termineBoucleEvenements();
}

InfoMenu initInfoMenu()
{
	InfoMenu i = {0, 0, 0, 0};
	return i;
}

InfoMusique initInfoMusique()
{
	InfoMusique musique = {0, 5, 5};
	return musique;
}

Pseudo initPseudo()
{
	Pseudo pseudo;
	strcpy(pseudo.pseudo, "");
	pseudo.ptrPseudo = 0;
	return pseudo;
}

void gestionEvenement(EvenementGfx evenement)
{

	static System system;

	switch (evenement)
	{

	case Initialisation:

		initMsPerRow();
		initSystem(&system);
		system.imageFond = lisBMPRGB("image/background.bmp");

		rafraichisFenetre();
		demandeTemporisation(30);
		break;

	case Temporisation:
		if (system.donneesSon.musiqueChange)
		{
			FMOD_ChannelGroup_Stop(system.donneesSon.channelGroupeMusique);
			FMOD_ChannelGroup_Stop(system.donneesSon.channelGroupeBruitage);
			if (system.infoMusique.Hchoix < 2)
			{
				playSound(system.donneesSon.systemSon, system.donneesSon.musique[system.infoMusique.Hchoix], 0, system.donneesSon.channelGroupeMusique);
			}

			system.donneesSon.musiqueChange = false;
		}
		if (!system.isGamePaused)
		{
			if (system.infoMenu.page == JeuSolo)
			{
				if (!system.infoPartieJ1.fin)
				{
					timeStep(&system, 1, 1);
				}
			}
			else if (system.infoMenu.page == JeuMulti)
			{
				if (!system.infoPartieJ1.fin)
				{
					timeStep(&system, 1, 2);
				}
				if (!system.infoPartieJ2.fin)
				{
					timeStep(&system, 2, 2);
				}
			}
		}

		rafraichisFenetre();

		break;

	case Affichage:

		ecrisImage(0, 0, system.imageFond->largeurImage, system.imageFond->hauteurImage, system.imageFond->donneesRGB);
		switch (system.infoMenu.page)
		{
		case Accueil:
			PageDemarrage1(&system);
			break;
		case Solo:
			PageDepart(&system);
			break;
		case Multi:
			PageDepart(&system);
			break;
		case Scoreboard:
			PageScore(&system);
			break;
		case Option:
			PageOption(&system);
			break;
		case JeuSolo:
			PageJeu(&system);
			break;
		case JeuMulti:
			PageJeu2(&system);
			break;
		}
		

		break;

	case Clavier:

		switch (caractereClavier())
		{
		case 'n':
			FMOD_ChannelGroup_Stop(system.donneesSon.channelGroupeMusique);
			FMOD_ChannelGroup_Stop(system.donneesSon.channelGroupeBruitage);
			break;
		case 'z':
		case 'Z':
			system.controleur.dpad = dpad_haut;
			break;

		case 's':
		case 'S':
			system.controleur.dpad = dpad_bas;
			break;

		case 'q':
		case 'Q':
			system.controleur.dpad = dpad_gauche;
			break;

		case 'd':
		case 'D':
			system.controleur.dpad = dpad_droite;
			break;

		case 'v':
		case 'V':
			system.controleur.action = action_A;
			break;

		case 'b':
		case 'B':
			system.controleur.action = action_B;
			break;

		case 'l':
			system.controleur2.action = action_A;
			break;
		case 'm':
			system.controleur2.action = action_B;
			break;
		}
		switch (system.infoMenu.etaJeu)
		{
		case JeuEncours:
			gestionControle(&system);
			break;
		case DemandePseudo:
		{
			//remplissage du pseudo

			if (caractereClavier() > 32 && caractereClavier() < 127) //caractère ascii
			{
				int ptr = system.pseudo.ptrPseudo < 6 ? system.pseudo.ptrPseudo : 5;
				system.pseudo.pseudo[ptr] = caractereClavier();
				if (system.pseudo.ptrPseudo < 6)
				{
					system.pseudo.ptrPseudo++;
				}
			}
			else if (caractereClavier() == 8) //touche effacer
			{
				if (system.pseudo.ptrPseudo == 6) //si overflow, on supprime le dernier caractère et on pointe sur le dernier caractere
				{
					system.pseudo.ptrPseudo--;
					system.pseudo.pseudo[(int)system.pseudo.ptrPseudo] = '\0';
				}
				else if (system.pseudo.ptrPseudo < 6 && system.pseudo.ptrPseudo > 0)
				//si on pointe au milieux de la chaine, on supprime le caratère d'avant et on recule le pointeur
				{
					system.pseudo.pseudo[(int)system.pseudo.ptrPseudo] = '\0';
					system.pseudo.ptrPseudo--;
					system.pseudo.pseudo[(int)system.pseudo.ptrPseudo] = '\0';
				}
				else if (system.pseudo.ptrPseudo == 0) //si on pointe sur le premier caratère, on supprime le premier caractère
				{
					system.pseudo.pseudo[(int)system.pseudo.ptrPseudo] = '\0';
				}
			}
			else if (caractereClavier() == 13)
			{ //touche entrer
				if (system.pseudo.ptrPseudo > 0)
				{
					//creation du score
					Score score;
					score.score = system.infoPartieJ1.score;
					strcpy(score.pseudo, system.pseudo.pseudo);
					//enregistrement du score
					insereDansListe(system.scores, &score);
					enregistreScore(system.scores);
					//on enleve la pop-up du nouveau score
					system.infoMenu.etaJeu = AffichageScores;
					system.pseudo = initPseudo();
				}
			}
			break;
		}
		case AffichageScores:
			if (system.controleur.action != action_none)
			{
				system.infoMenu.etaJeu = PasEnJeu;
				if (system.infoMenu.page == JeuSolo)
				{
					system.infoMenu.page = Solo;
					system.infoPartieJ1.fin = 0;
				}
				if (system.infoMenu.page == JeuMulti)
				{
					system.infoMenu.page = Multi;
					system.infoPartieJ1.fin = 0;
					system.infoPartieJ2.fin = 0;
				}
			}

			break;
		case PasEnJeu:
			navigueMenu(&system);
			break;
		}
		resetControleur(&(system.controleur));
		resetControleur(&(system.controleur2));
		rafraichisFenetre();
		break;

	case ClavierSpecial:
		switch (toucheClavier())
		{
		case ToucheFlecheGauche:
			system.controleur2.dpad = dpad_gauche;
			break;
		case ToucheFlecheDroite:
			system.controleur2.dpad = dpad_droite;
			break;
		case ToucheFlecheBas:
			system.controleur2.dpad = dpad_bas;
			break;
		case ToucheFlecheHaut:
			system.controleur2.dpad = dpad_haut;
			break;
		default:
			break;
		}

		switch (system.infoMenu.etaJeu)
		{
		case JeuEncours:
			gestionControle(&system);
			break;
		case DemandePseudo:
		{
			break;
		}
		break;
		case AffichageScores:
			break;
		case PasEnJeu:
			navigueMenu(&system);
			break;
		}
		resetControleur(&(system.controleur));
		resetControleur(&(system.controleur2));
		rafraichisFenetre();

		break;

	case BoutonSouris:
		//permet de quitter le programe proprement même si coincé
		/*
		if (etatBoutonSouris() == GaucheAppuye)
		{
			
			if ((largeurFenetre() - 125 <= abscisseSouris()) && (abscisseSouris() <= largeurFenetre()) && (0 <= ordonneeSouris()) && (ordonneeSouris() <= hauteurFenetre() / 9))
				quitterGestionEvenement(&system);
		}*/
		break;

	case Souris:

		//rafraichisFenetre();
		break;

	case Inactivite:
		break;

	case Redimensionnement:

		break;
	}
}
void navigueMenu(System *system)
{
	if (system->controleur.action == action_A)
	{
		if (system->infoMenu.page == Accueil || system->infoMenu.page == Solo || system->infoMenu.page == Multi)
		{
			playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[0], 0, system->donneesSon.channelGroupeBruitage);
		}
	}
	else if (system->controleur.action == action_B)
	{
		if (system->infoMenu.page != Accueil)
		{
			playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[1], 0, system->donneesSon.channelGroupeBruitage);
		}
	}
	else if (system->controleur.dpad == dpad_bas || system->controleur.dpad == dpad_haut)
	{
		if (system->infoMenu.page != Scoreboard)
		{
			playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[2], 0, system->donneesSon.channelGroupeBruitage);
		}
	}
	else if (system->controleur.dpad == dpad_droite || system->controleur.dpad == dpad_gauche)
	{
		if (system->infoMenu.page != Scoreboard && system->infoMenu.page != Accueil)
		{
			playSound(system->donneesSon.systemSon, system->donneesSon.bruitage[2], 0, system->donneesSon.channelGroupeBruitage);
		}
	}

	if (system->controleur.action == action_A)
	{
		switch (system->infoMenu.page)
		{
		case Accueil:

			switch (system->infoMenu.Vselection)
			{
			case 0:
				changePage(system, Solo);
				break;
			case 1:
				changePage(system, Multi);
				break;
			case 2:
				changePage(system, Scoreboard);
				break;
			case 3:
				changePage(system, Option);
				break;
			case 4:
				quitterGestionEvenement(system);
				break;
			}
			break;

		case Solo:
			if (system->infoMenu.Vselection == 0)
			{
				//debut d'une partie solo
				system->infoMenu.page = JeuSolo;
				initInfoPartieSolo(system, system->infoMenu.Hselection);
				system->infoMenu.etaJeu = JeuEncours;
			}

			else if (system->infoMenu.Vselection == 1)
			{
				system->infoMenu.page = Accueil;
				system->infoMenu.Vselection = 0;
			}
			break;

		case Multi:
			if (system->infoMenu.Vselection == 0)
			{
				//debut d'une partie multi
				system->infoMenu.page = JeuMulti;
				initInfoPartieMulti(system, system->infoMenu.Hselection);
				system->infoMenu.etaJeu = JeuEncours;
			}
			else if (system->infoMenu.Vselection == 1)
			{
				system->infoMenu.page = Accueil;
				system->infoMenu.Vselection = 1;
			}
			break;
		default:
			break;
		}
	}

	else if (system->controleur.action == action_B)
	{
		switch (system->infoMenu.page)
		{
		case Solo:
			system->infoMenu.page = Accueil;
			system->infoMenu.Vselection = 0;
			break;

		case Multi:
			system->infoMenu.page = Accueil;
			system->infoMenu.Vselection = 1;
			break;

		case Scoreboard:
			system->infoMenu.page = Accueil;
			system->infoMenu.Vselection = 2;
			break;

		case Option:
			system->infoMenu.page = Accueil;
			system->infoMenu.Vselection = 3;
			break;

		case JeuSolo:
			system->infoMenu.page = Solo;
			system->infoMenu.Vselection = 0;
			break;

		case JeuMulti:
			system->infoMenu.page = Multi;
			system->infoMenu.Vselection = 0;
			break;
		default:
			break;
		}
	}
	else if (system->controleur.dpad == dpad_bas)
	{
		switch (system->infoMenu.page)
		{
		case Accueil:
			system->infoMenu.Vselection++;
			if (system->infoMenu.Vselection >= MENU_LIGNES)
			{
				system->infoMenu.Vselection = 0;
			}
			break;

		case Solo:
			system->infoMenu.Vselection = !(system->infoMenu.Vselection);
			break;

		case Multi:
			system->infoMenu.Vselection = !(system->infoMenu.Vselection);
			break;

		case Option:
			system->infoMenu.Vselection++;
			if (system->infoMenu.Vselection > OPTION_LIGNES)
			{
				system->infoMenu.Vselection = 0;
			}
			break;
		default:
			break;
		}
	}

	else if (system->controleur.dpad == dpad_haut)
	{
		switch (system->infoMenu.page)
		{
		case Accueil:
			system->infoMenu.Vselection--;
			if (system->infoMenu.Vselection < 0)
			{
				system->infoMenu.Vselection = MENU_LIGNES - 1;
			}
			break;

		case Solo:
			system->infoMenu.Vselection = !(system->infoMenu.Vselection);
			break;

		case Multi:
			system->infoMenu.Vselection = !(system->infoMenu.Vselection);
			break;

		case Option:
			system->infoMenu.Vselection--;
			if (system->infoMenu.Vselection < 0)
			{
				system->infoMenu.Vselection = OPTION_LIGNES;
			}
			break;

		default:
			break;
		}
	}

	else if (system->controleur.dpad == dpad_gauche)
	{
		if (system->infoMenu.page == Solo || system->infoMenu.page == Multi)
		{
			if (system->infoMenu.Vselection == 0)
			{
				system->infoMenu.Hselection--;
				if (system->infoMenu.Hselection < 0)
				{
					system->infoMenu.Hselection = 9;
				}
			}
		}

		if (system->infoMenu.page == Option)
		{
			if (system->infoMenu.Vselection == 0)
			{
				system->infoMusique.Hchoix--;
				if (system->infoMusique.Hchoix < 0)
				{
					system->infoMusique.Hchoix = 2;
				}
				system->donneesSon.musiqueChange = true; //on indique que la musique a changé
			}
			if (system->infoMenu.Vselection == 1)
			{
				system->infoMusique.Hmusique--;
				if (system->infoMusique.Hmusique < 0)
				{
					system->infoMusique.Hmusique = 0;
				}
				FMOD_ChannelGroup_SetVolume(system->donneesSon.channelGroupeMusique, system->infoMusique.Hmusique / 10.f);
			}

			if (system->infoMenu.Vselection == 2)
			{
				system->infoMusique.Hbruitage--;
				if (system->infoMusique.Hbruitage < 0)
				{
					system->infoMusique.Hbruitage = 0;
				}
				FMOD_ChannelGroup_SetVolume(system->donneesSon.channelGroupeBruitage, system->infoMusique.Hbruitage / 10.f);
			}
		}
	}
	else if (system->controleur.dpad == dpad_droite)
	{
		if (system->infoMenu.page == Solo || system->infoMenu.page == Multi)
		{
			if (system->infoMenu.Vselection == 0)
			{
				system->infoMenu.Hselection++;
				if (system->infoMenu.Hselection > 9)
				{
					system->infoMenu.Hselection = 0;
				}
			}
		}
		if (system->infoMenu.page == Option)
		{
			if (system->infoMenu.Vselection == 0)
			{
				system->infoMusique.Hchoix++;
				if (system->infoMusique.Hchoix > 2)
				{
					system->infoMusique.Hchoix = 0;
				}
				system->donneesSon.musiqueChange = true; //on indique que la musique a changé
			}
			if (system->infoMenu.Vselection == 1)
			{
				system->infoMusique.Hmusique++;
				if (system->infoMusique.Hmusique > 10)
				{
					system->infoMusique.Hmusique = 10;
				}
				FMOD_ChannelGroup_SetVolume(system->donneesSon.channelGroupeMusique, system->infoMusique.Hmusique / 10.f);
			}

			if (system->infoMenu.Vselection == 2)
			{
				system->infoMusique.Hbruitage++;
				if (system->infoMusique.Hbruitage > 10)
				{
					system->infoMusique.Hbruitage = 10;
				}
				FMOD_ChannelGroup_SetVolume(system->donneesSon.channelGroupeBruitage, system->infoMusique.Hbruitage / 10.f);
			}
		}
	}
	resetControleur(&(system->controleur));
}

void changePage(System *system, Page p)
{
	system->infoMenu.page = p;
	system->infoMenu.Vselection = 0;
	system->infoMenu.Hselection = 0;
}

void PageDemarrage1(System *system)
{
	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 147.69);
	afficheChaine(" S O L O", largeurFenetre() / 38.4, (largeurFenetre() / 2) * 0.8, hauteurFenetre() / 1.2);
	afficheChaine("M U L T I J O U E U R", largeurFenetre() / 38.4, largeurFenetre() / 3.4, hauteurFenetre() / 1.5);
	afficheChaine("S C O R E B O A R D", largeurFenetre() / 38.4, largeurFenetre() / 3.2, hauteurFenetre() / 2);
	afficheChaine("O P T I O N S", largeurFenetre() / 38.4, largeurFenetre() / 2.6, hauteurFenetre() / 3);
	afficheChaine("Q U I T T E R", largeurFenetre() / 38.4, largeurFenetre() / 2.6, hauteurFenetre() / 7);

	if (system->infoMenu.Vselection == 0)
		// SELECTION "SOLO"
		triangle(largeurFenetre() / 4, hauteurFenetre() / 1.1, largeurFenetre() / 4, hauteurFenetre() / 1.28, largeurFenetre() / 3, hauteurFenetre() / 1.18);
	if (system->infoMenu.Vselection == 1)
		// SELECTION "M U L T I J O U E U R"
		triangle(largeurFenetre() / 6, hauteurFenetre() / 1.6, largeurFenetre() / 6, hauteurFenetre() / 1.31, largeurFenetre() / 4, hauteurFenetre() / 1.45);
	if (system->infoMenu.Vselection == 2)
		// SELECTION "S C O R E B O A R D"
		triangle(largeurFenetre() / 6, hauteurFenetre() / 2.2, largeurFenetre() / 6, hauteurFenetre() / 1.7, largeurFenetre() / 4, hauteurFenetre() / 1.9);
	if (system->infoMenu.Vselection == 3)
		// SELECTION "O P T I O N S"
		triangle(largeurFenetre() / 4, hauteurFenetre() / 3.3, largeurFenetre() / 4, hauteurFenetre() / 2.5, largeurFenetre() / 3, hauteurFenetre() / 2.8);
	if (system->infoMenu.Vselection == 4)
		// SELECTION "Q U I T T E R"
		triangle(largeurFenetre() / 4, hauteurFenetre() / 9, largeurFenetre() / 4, hauteurFenetre() / 4.5, largeurFenetre() / 3, hauteurFenetre() / 6);
}

void PageDepart(System *system)
{

	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 147.69);
	afficheChaine("N I V E A U  D E  D E P A R T", largeurFenetre() / 38.4, largeurFenetre() / 5, hauteurFenetre() / 1.3);
	couleurCourante(47, 54, 64);
	// NIVEAU 0
	rectangle(largeurFenetre() / 15, hauteurFenetre() / 2, 1.7 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 1
	rectangle(2.4 * largeurFenetre() / 15, hauteurFenetre() / 2, 3.1 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 2
	rectangle(3.8 * largeurFenetre() / 15, hauteurFenetre() / 2, 4.5 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 3
	rectangle(5.2 * largeurFenetre() / 15, hauteurFenetre() / 2, 5.9 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 4
	rectangle(6.6 * largeurFenetre() / 15, hauteurFenetre() / 2, 7.3 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 5
	rectangle(8 * largeurFenetre() / 15, hauteurFenetre() / 2, 8.7 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 6
	rectangle(9.4 * largeurFenetre() / 15, hauteurFenetre() / 2, 10.1 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 7
	rectangle(10.8 * largeurFenetre() / 15, hauteurFenetre() / 2, 11.5 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 8
	rectangle(12.2 * largeurFenetre() / 15, hauteurFenetre() / 2, 12.9 * largeurFenetre() / 15, hauteurFenetre() / 1.7);
	// NIVEAU 9
	rectangle(13.6 * largeurFenetre() / 15, hauteurFenetre() / 2, 14.3 * largeurFenetre() / 15, hauteurFenetre() / 1.7);

	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 240);
	afficheChaine("0", largeurFenetre() / 38.4, largeurFenetre() / 15 * 1.25, hauteurFenetre() / 1.92);
	afficheChaine("1", largeurFenetre() / 38.4, 2.6 * largeurFenetre() / 15, hauteurFenetre() / 1.92);
	afficheChaine("2", largeurFenetre() / 38.4, 4 * largeurFenetre() / 15, hauteurFenetre() / 1.92);
	afficheChaine("3", largeurFenetre() / 38.4, 5.4 * largeurFenetre() / 15, hauteurFenetre() / 1.92);
	afficheChaine("4", largeurFenetre() / 38.4, 6.8 * largeurFenetre() / 15, hauteurFenetre() / 1.92);
	afficheChaine("5", largeurFenetre() / 38.4, 8.2 * largeurFenetre() / 15, hauteurFenetre() / 1.92);
	afficheChaine("6", largeurFenetre() / 38.4, 9.6 * largeurFenetre() / 15, hauteurFenetre() / 1.92);
	afficheChaine("7", largeurFenetre() / 38.4, 11 * largeurFenetre() / 15, hauteurFenetre() / 1.92);
	afficheChaine("8", largeurFenetre() / 38.4, 12.4 * largeurFenetre() / 15, hauteurFenetre() / 1.92);
	afficheChaine("9", largeurFenetre() / 38.4, 13.8 * largeurFenetre() / 15, hauteurFenetre() / 1.92);

	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 147.69);
	afficheChaine("R E T O U R", largeurFenetre() / 38.4, largeurFenetre() / 2.5, hauteurFenetre() / 4);

	couleurCourante(251, 197, 49);
	if (system->infoMenu.Hselection == 0 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 0
		triangle(largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 1.35, hauteurFenetre() / 1.6, 1.7 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 1 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 1
		triangle(2.4 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 2.75, hauteurFenetre() / 1.6, 3.1 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 2 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 2
		triangle(3.8 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 4.15, hauteurFenetre() / 1.6, 4.5 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 3 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 3
		triangle(5.2 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 5.55, hauteurFenetre() / 1.6, 5.9 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 4 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 4
		triangle(6.6 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 6.95, hauteurFenetre() / 1.6, 7.3 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 5 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 5
		triangle(8 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 8.35, hauteurFenetre() / 1.6, 8.7 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 6 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 6
		triangle(9.4 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 9.75, hauteurFenetre() / 1.6, 10.1 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 7 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 7
		triangle(10.8 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 11.15, hauteurFenetre() / 1.6, 11.5 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 8 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 8
		triangle(12.2 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 12.55, hauteurFenetre() / 1.6, 12.9 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Hselection == 9 && system->infoMenu.Vselection == 0)
		// SELECTION NIVEAU 9
		triangle(13.6 * largeurFenetre() / 15, hauteurFenetre() / 1.4, largeurFenetre() / 15 * 13.95, hauteurFenetre() / 1.6, 14.3 * largeurFenetre() / 15, hauteurFenetre() / 1.4);
	if (system->infoMenu.Vselection == 1)
	{
		// SELECTION RETOUR

		rectangle(largeurFenetre() / 2.6, hauteurFenetre() / 5, 1.9 * largeurFenetre() / 3, hauteurFenetre() / 2.9);
		couleurCourante(47, 54, 64);
		epaisseurDeTrait(largeurFenetre() / 147.69);
		afficheChaine("R E T O U R", largeurFenetre() / 38.4, largeurFenetre() / 2.5, hauteurFenetre() / 4);
	}
}

void PageScore(System *system)
{
	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 147.69);
	afficheChaine("S C O R E B O A R D", largeurFenetre() / 38.4, largeurFenetre() / 3.3, hauteurFenetre() / 1.2);
	float taille = largeurFenetre() / 38.4;
	float ybase = hauteurFenetre() / 1.4;
	float yincrement = hauteurFenetre() / 8;
	float x1 = largeurFenetre() / 8;
	float x2 = largeurFenetre() / 1.8;
	float xdecalpseudo = largeurFenetre() / 20;
	float xdecalscore = largeurFenetre() / 6;

	epaisseurDeTrait(4);
	char temp[7] = "";
	for (int i = 0; i < 10; i++)
	{
		sprintf(temp, "%d.", i + 1);
		afficheChaine(temp, taille, (i < 5) ? x1 : x2, ybase - (i % 5) * yincrement);
		if (system->scores[i])
		{
			sprintf(temp, "%s", system->scores[i]->pseudo);
			afficheChaine(temp, taille, ((i < 5) ? x1 : x2) + xdecalpseudo, ybase - (i % 5) * yincrement);
			sprintf(temp, "%ld", system->scores[i]->score);
			afficheChaine(temp, taille, ((i < 5) ? x1 : x2) + xdecalscore, ybase - (i % 5) * yincrement);
		}
		else
		{
			afficheChaine("------", taille, ((i < 5) ? x1 : x2) + xdecalpseudo, ybase - (i % 5) * yincrement);
			afficheChaine("------", taille, ((i < 5) ? x1 : x2) + xdecalscore, ybase - (i % 5) * yincrement);
		}
	}
}

void PageJeu(System *system)
{

	char texttemp[10];

	couleurCourante(47, 54, 64);
	//FOND NOIR
	rectangle(5 * largeurFenetre() / 20, 0, 15 * largeurFenetre() / 20, hauteurFenetre());
	//Apercu prochaine pièce
	couleurCourante(113, 128, 147);
	rectangle(12.3 * largeurFenetre() / 20, hauteurFenetre() / 10, 14.7 * largeurFenetre() / 20, 4 * hauteurFenetre() / 10);

	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 640);
	afficheChaine("S C O R E", largeurFenetre() / 106.67, 12.8 * largeurFenetre() / 20, 9 * hauteurFenetre() / 10);
	sprintf(texttemp, "%li", system->infoPartieJ1.score);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 12.8 * largeurFenetre() / 20, 8.5 * hauteurFenetre() / 10);

	afficheChaine("L E V E L", largeurFenetre() / 106.67, 12.8 * largeurFenetre() / 20, 7.5 * hauteurFenetre() / 10);
	sprintf(texttemp, "%d", system->infoPartieJ1.niveau);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 12.8 * largeurFenetre() / 20, 7 * hauteurFenetre() / 10);

	afficheChaine("L I G N E", largeurFenetre() / 106.67, 12.8 * largeurFenetre() / 20, 6 * hauteurFenetre() / 10);
	sprintf(texttemp, "%d", system->infoPartieJ1.totalLigne);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 12.8 * largeurFenetre() / 20, 5.5 * hauteurFenetre() / 10);

	// IF PAUSE

	if (system->infoMenu.etaJeu == AffichageScores || system->infoMenu.etaJeu == DemandePseudo)
	{

		couleurCourante(232, 65, 24);
		epaisseurDeTrait(largeurFenetre() / 200);
		afficheChaine("P E R D U !", largeurFenetre() / 30, 2 * largeurFenetre() / 7, 5 * hauteurFenetre() / 6);

		float taille = largeurFenetre() / 50;
		float ybase = 5 * hauteurFenetre() / 7;
		float yincrement = 0.5f * hauteurFenetre() / 7;
		float x = 2 * largeurFenetre() / 7;
		float xdecalpseudo = 0.3f * largeurFenetre() / 7;
		float xdecalscore = 1.4f * largeurFenetre() / 7;

		epaisseurDeTrait(4);
		char temp[7] = "";
		for (int i = 0; i < 10; i++)
		{
			sprintf(temp, "%d.", i + 1);
			afficheChaine(temp, taille, x, ybase - i * yincrement);
			if (system->scores[i])
			{
				sprintf(temp, "%s", system->scores[i]->pseudo);
				afficheChaine(temp, taille, x + xdecalpseudo, ybase - i * yincrement);
				sprintf(temp, "%ld", system->scores[i]->score);
				afficheChaine(temp, taille, x + xdecalscore, ybase - i * yincrement);
			}
			else
			{
				afficheChaine("------", taille, x + xdecalpseudo, ybase - i * yincrement);
				afficheChaine("------", taille, x + xdecalscore, ybase - i * yincrement);
			}
		}

		if (system->infoMenu.etaJeu == DemandePseudo)
		{
			//BRAVO
			//if(system->infoPartieJ1.score dans le top 10)

			couleurCourante(127, 143, 166);
			rectangle(1.9 * largeurFenetre() / 7, hauteurFenetre() / 3, 5 * largeurFenetre() / 7, 2.1 * hauteurFenetre() / 3);

			couleurCourante(39, 60, 117);
			epaisseurDeTrait(6);
			afficheChaine("BRAVO !", largeurFenetre() / 30, 2.8 * largeurFenetre() / 7, 1.8 * hauteurFenetre() / 3);
			epaisseurDeTrait(largeurFenetre() / 250);
			afficheChaine("TU ES DANS LE CLASSEMENT", largeurFenetre() / 50, 2.3 * largeurFenetre() / 7, 1.5 * hauteurFenetre() / 3);
			afficheChaine("PSEUDO :", largeurFenetre() / 50, 2.5 * largeurFenetre() / 7, 1.2 * hauteurFenetre() / 3);
			afficheChaine(system->pseudo.pseudo, largeurFenetre() / 50, 3.5 * largeurFenetre() / 7, 1.2 * hauteurFenetre() / 3);
		}
	}
	else if (system->isGamePaused && !system->infoPartieJ1.fin)
	{
		//Dissimulation de la grille
		couleurCourante(47, 54, 64);
		rectangle(5 * largeurFenetre() / 20, 0, 15 * largeurFenetre() / 20, hauteurFenetre());
		couleurCourante(220, 221, 225);
		epaisseurDeTrait(largeurFenetre() / 200);
		afficheChaine("P A U S E", largeurFenetre() / 30, 1.15 * largeurFenetre() / 3, 1.9 * hauteurFenetre() / 3);
	}
	else
	{
		creerGrille(system, 1, 1);
		creerGrille2(system, 1, 1);
	}
}

void creerGrille(System *system, char joueur, char mode)
{

	int nbLigne = GRILLE_LIGNES;
	int nbCol = GRILLE_COLONNES;

	float valeurBaseCol = 0;
	float valeurLongueurCol = 0;

	InfoPartie infoPartie;
	if (mode == 1)
	{
		valeurBaseCol = 5 * largeurFenetre() / 20;
		valeurLongueurCol = 7 * largeurFenetre() / 20;

		infoPartie = system->infoPartieJ1;
	}
	else if (mode == 2)
	{
		if (joueur == 1)
		{
			valeurBaseCol = largeurFenetre() / 25;
			valeurLongueurCol = 7 * largeurFenetre() / 25;
			infoPartie = system->infoPartieJ1;
		}
		else if (joueur == 2)
		{
			valeurBaseCol = 14 * largeurFenetre() / 25;
			valeurLongueurCol = 7 * largeurFenetre() / 25;
			infoPartie = system->infoPartieJ2;
		}
	}

	for (int i = 0; i < nbCol; i++)
	{
		for (int j = 0; j < nbLigne; j++)
		{
			if (infoPartie.grille[i][j] != -1)
			{
				changeCouleurId(infoPartie.grille[i][j]);
			}
			else
			{
				for (int k = 0; k < 4; k++)
				{
					//test de si la case comporte une partie de la piece
					if (infoPartie.piece.tiles[k].x == i && infoPartie.piece.tiles[k].y == j)
					{
						changeCouleurId(infoPartie.piece.id);
						break;
					}
					else
					{
						couleurCourante(113, 128, 147);
					}
				}
			}

			rectangle(1 + valeurBaseCol + i * (valeurLongueurCol / (nbCol)),
					  1 + j * hauteurFenetre() / (nbLigne),
					  -1 + valeurBaseCol + (i + 1) * (valeurLongueurCol / (nbCol)),
					  -1 + (j + 1) * hauteurFenetre() / (nbLigne));
		}
	}
}

void creerGrille2(System *system, char joueur, char mode)
{
	int nbLigne = 2;
	int nbCol = 5;

	float valeurBaseCol = 0;
	float valeurBaseLigne = 0;
	float valeurLongueurCol = 0;
	float valeurLongueurLigne = 0;

	InfoPartie infoPartie;
	if (mode == 1)
	{
		valeurBaseCol = 12.3 * largeurFenetre() / 20;
		valeurLongueurCol = 2.4 * largeurFenetre() / 20;
		valeurBaseLigne = 2 * hauteurFenetre() / 10;
		valeurLongueurLigne = hauteurFenetre() / 10;

		infoPartie = system->infoPartieJ1;
	}
	else if (mode == 2)
	{
		if (joueur == 1)
		{

			valeurBaseCol = 8.3 * largeurFenetre() / 25;
			valeurLongueurCol = 2.4 * largeurFenetre() / 25;
			valeurBaseLigne = 2 * hauteurFenetre() / 10;
			valeurLongueurLigne = hauteurFenetre() / 10;

			infoPartie = system->infoPartieJ1;
		}
		else if (joueur == 2)
		{

			valeurBaseCol = 21.3 * largeurFenetre() / 25;
			valeurLongueurCol = 2.4 * largeurFenetre() / 25;
			valeurBaseLigne = 2 * hauteurFenetre() / 10;
			valeurLongueurLigne = hauteurFenetre() / 10;

			infoPartie = system->infoPartieJ2;
		}
	}

	Piece piece = creerFuturPieces(infoPartie.idPieceFutur);

	for (int i = 0; i < nbCol; i++)
	{
		for (int j = 0; j < nbLigne; j++)
		{

			for (int k = 0; k < 4; k++)
			{
				//test de si la case comporte une partie de la piece
				if (piece.tiles[k].x == i && piece.tiles[k].y - 1 == j)
				{
					changeCouleurId(piece.id);

					rectangle(1 + valeurBaseCol + i * (valeurLongueurCol / (nbCol)),
							  1 + valeurBaseLigne + j * valeurLongueurLigne / (nbLigne),
							  -1 + valeurBaseCol + (i + 1) * (valeurLongueurCol / (nbCol)),
							  -1 + valeurBaseLigne + (j + 1) * valeurLongueurLigne / (nbLigne));
					break;
				}
				else
				{
					//	couleurCourante(255, 255, 255);
				}
			}
		}
	}
}

Piece creerFuturPieces(int idPieceFutur)
{
	Piece piece;

	Piece *p = &piece;

	p->id = idPieceFutur;
	p->state = 0;
	PieceEnum pid = (PieceEnum)idPieceFutur;
	switch (pid)
	{
	case Piece_S:
		p->tiles[0] = (Coord_Piece){2, 2};
		p->tiles[1] = (Coord_Piece){2, 1};
		p->tiles[2] = (Coord_Piece){3, 2};
		p->tiles[3] = (Coord_Piece){1, 1};
		break;
	case Piece_Z:
		p->tiles[0] = (Coord_Piece){1, 2};
		p->tiles[1] = (Coord_Piece){2, 2};
		p->tiles[2] = (Coord_Piece){2, 1};
		p->tiles[3] = (Coord_Piece){3, 1};
		break;
	case Piece_T:
		p->tiles[0] = (Coord_Piece){2, 2};
		p->tiles[1] = (Coord_Piece){1, 2};
		p->tiles[2] = (Coord_Piece){3, 2};
		p->tiles[3] = (Coord_Piece){2, 1};
		break;
	case Piece_L:
		p->tiles[0] = (Coord_Piece){2, 2};
		p->tiles[1] = (Coord_Piece){1, 2};
		p->tiles[2] = (Coord_Piece){3, 2};
		p->tiles[3] = (Coord_Piece){1, 1};
		break;
	case Piece_J:
		p->tiles[0] = (Coord_Piece){2, 2};
		p->tiles[1] = (Coord_Piece){1, 2};
		p->tiles[2] = (Coord_Piece){3, 2};
		p->tiles[3] = (Coord_Piece){3, 1};
		break;
	case Piece_O:
		p->tiles[0] = (Coord_Piece){2, 2};
		p->tiles[1] = (Coord_Piece){3, 2};
		p->tiles[2] = (Coord_Piece){2, 1};
		p->tiles[3] = (Coord_Piece){3, 1};
		break;
	case Piece_I:
		p->tiles[0] = (Coord_Piece){2, 2};
		p->tiles[1] = (Coord_Piece){1, 2};
		p->tiles[2] = (Coord_Piece){3, 2};
		p->tiles[3] = (Coord_Piece){4, 2};
		break;

	default:
		break;
	}

	return piece;
}

void changeCouleurId(int id)
{
	switch ((PieceEnum)id)
	{
	case Piece_S:
		couleurCourante(134, 74, 169);
		break;
	case Piece_I:
		couleurCourante(201, 174, 38);
		break;
	case Piece_Z:
		couleurCourante(230, 148, 206);
		break;
	case Piece_O:
		couleurCourante(28, 224, 233);
		break;
	case Piece_J:
		couleurCourante(166, 36, 46);
		break;
	case Piece_L:
		couleurCourante(191, 105, 37);
		break;
	case Piece_T:
		couleurCourante(44, 65, 176);
		break;
	}
}

void PageJeu2(System *system)
{
	char texttemp[10];

	couleurCourante(47, 54, 64);
	//FOND GRILLE GAUCHE
	rectangle(largeurFenetre() / 25, 0, 11 * largeurFenetre() / 25, hauteurFenetre());
	//Apercu prochaine pièce
	couleurCourante(113, 128, 147);
	rectangle(8.3 * largeurFenetre() / 25, hauteurFenetre() / 10, 10.7 * largeurFenetre() / 25, 4 * hauteurFenetre() / 10);

	couleurCourante(47, 54, 64);
	//FOND GRILLE DROITE
	rectangle(14 * largeurFenetre() / 25, 0, 24 * largeurFenetre() / 25, hauteurFenetre());
	//Apercu prochaine pièce
	couleurCourante(113, 128, 147);
	rectangle(21.3 * largeurFenetre() / 25, hauteurFenetre() / 10, 23.7 * largeurFenetre() / 25, 4 * hauteurFenetre() / 10);

	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 640);

	//INFOS J1
	afficheChaine("S C O R E", largeurFenetre() / 106.67, 8.8 * largeurFenetre() / 25, 9 * hauteurFenetre() / 10);
	sprintf(texttemp, "%li", system->infoPartieJ1.score);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 8.8 * largeurFenetre() / 25, 8.5 * hauteurFenetre() / 10);

	afficheChaine("L E V E L", largeurFenetre() / 106.67, 8.8 * largeurFenetre() / 25, 7.5 * hauteurFenetre() / 10);
	sprintf(texttemp, "%d", system->infoPartieJ1.niveau);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 8.8 * largeurFenetre() / 25, 7 * hauteurFenetre() / 10);

	afficheChaine("L I G N E", largeurFenetre() / 106.67, 8.8 * largeurFenetre() / 25, 6 * hauteurFenetre() / 10);
	sprintf(texttemp, "%d", system->infoPartieJ1.totalLigne);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 8.8 * largeurFenetre() / 25, 5.5 * hauteurFenetre() / 10);

	//INFOS J2
	afficheChaine("S C O R E", largeurFenetre() / 106.67, 21.8 * largeurFenetre() / 25, 9 * hauteurFenetre() / 10);
	sprintf(texttemp, "%li", system->infoPartieJ2.score);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 21.8 * largeurFenetre() / 25, 8.5 * hauteurFenetre() / 10);

	afficheChaine("L E V E L", largeurFenetre() / 106.67, 21.8 * largeurFenetre() / 25, 7.5 * hauteurFenetre() / 10);
	sprintf(texttemp, "%d", system->infoPartieJ2.niveau);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 21.8 * largeurFenetre() / 25, 7 * hauteurFenetre() / 10);

	afficheChaine("L I G N E", largeurFenetre() / 106.67, 21.8 * largeurFenetre() / 25, 6 * hauteurFenetre() / 10);
	sprintf(texttemp, "%d", system->infoPartieJ2.totalLigne);
	afficheChaine(texttemp, largeurFenetre() / 106.67, 21.8 * largeurFenetre() / 25, 5.5 * hauteurFenetre() / 10);

	creerGrille(system, 1, 2);
	creerGrille(system, 2, 2);
	creerGrille2(system, 1, 2);
	creerGrille2(system, 2, 2);

	if (system->isGamePaused)

	{
		//Dissimulation des grilles
		couleurCourante(47, 54, 64);
		rectangle(largeurFenetre() / 25, 0, 11 * largeurFenetre() / 25, hauteurFenetre());
		rectangle(14 * largeurFenetre() / 25, 0, 24 * largeurFenetre() / 25, hauteurFenetre());

		//Fenetre Pause
		couleurCourante(113, 128, 147);
		rectangle(largeurFenetre() / 4, 3.5 * hauteurFenetre() / 8, 3 * largeurFenetre() / 4, 5 * hauteurFenetre() / 8);
		couleurCourante(220, 221, 225);
		epaisseurDeTrait(largeurFenetre() / 200);
		afficheChaine("P A U S E", largeurFenetre() / 30, 1.15 * largeurFenetre() / 3, 4 * hauteurFenetre() / 8);
	}

	else if (system->infoPartieJ1.fin)
	{
		if (!system->infoPartieJ2.fin)
		{
			//EN ATTENTE DU J2
			couleurCourante(47, 54, 64);
			rectangle(1.2 * largeurFenetre() / 25, hauteurFenetre() / 3, 7.7 * largeurFenetre() / 25, 2.1 * hauteurFenetre() / 3);
			couleurCourante(251, 197, 49);
			epaisseurDeTrait(largeurFenetre() / 250);
			afficheChaine("E N  A T T E N T E", largeurFenetre() / 60, 1.6 * largeurFenetre() / 25, 1.55 * hauteurFenetre() / 3);
		}
	}

	else if (system->infoPartieJ2.fin)
	{
		if (!system->infoPartieJ1.fin)
		{
			//EN ATTENTE DU J1
			couleurCourante(47, 54, 64);
			rectangle(14.2 * largeurFenetre() / 25, hauteurFenetre() / 3, 20.7 * largeurFenetre() / 25, 2.1 * hauteurFenetre() / 3);
			couleurCourante(251, 197, 49);
			epaisseurDeTrait(largeurFenetre() / 250);
			afficheChaine("E N  A T T E N T E", largeurFenetre() / 60, 14.6 * largeurFenetre() / 25, 1.55 * hauteurFenetre() / 3);
		}
	}
	//PAGE FIN

	if (system->infoPartieJ2.fin && system->infoPartieJ1.fin)
	{
		if (system->infoPartieJ1.score > system->infoPartieJ2.score)
		{

			couleurCourante(0, 168, 255);
			epaisseurDeTrait(largeurFenetre() / 200);
			afficheChaine("G A G N E !", largeurFenetre() / 30, 1.2 * largeurFenetre() / 25, 5 * hauteurFenetre() / 6);

			couleurCourante(232, 65, 24);
			epaisseurDeTrait(largeurFenetre() / 200);
			afficheChaine("P E R D U !", largeurFenetre() / 30, 14.2 * largeurFenetre() / 25, 5 * hauteurFenetre() / 6);
		}

		else if (system->infoPartieJ1.score < system->infoPartieJ2.score)
		{

			couleurCourante(232, 65, 24);
			epaisseurDeTrait(largeurFenetre() / 200);
			afficheChaine("P E R D U !", largeurFenetre() / 30, 1.2 * largeurFenetre() / 25, 5 * hauteurFenetre() / 6);

			couleurCourante(0, 168, 255);
			epaisseurDeTrait(largeurFenetre() / 200);
			afficheChaine("G A G N E !", largeurFenetre() / 30, 14.2 * largeurFenetre() / 25, 5 * hauteurFenetre() / 6);
		}

		else
		{

			couleurCourante(251, 197, 49);
			epaisseurDeTrait(largeurFenetre() / 200);
			afficheChaine("E G A L I T E", largeurFenetre() / 35, 1.2 * largeurFenetre() / 25, 5 * hauteurFenetre() / 6);
			afficheChaine("E G A L I T E", largeurFenetre() / 35, 14.2 * largeurFenetre() / 25, 5 * hauteurFenetre() / 6);
		}
	}
}

void PageOption(System *system)
{

	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 147.69);
	afficheChaine("O P T I O N S", largeurFenetre() / 38.4, largeurFenetre() / 2.6, hauteurFenetre() / 1.1);
	// 2 GROS CARRES GRIS
	couleurCourante(113, 128, 147);
	rectangle(0, hauteurFenetre() / 2.7, largeurFenetre() / 2.3, hauteurFenetre() / 1.2);
	rectangle(largeurFenetre() / 1.8, hauteurFenetre() / 2.7, largeurFenetre(), hauteurFenetre() / 1.2);

	couleurCourante(251, 197, 49);
	//SELECTION MUSIQUE
	triangle(largeurFenetre() / 3.5, hauteurFenetre() / 3.7, largeurFenetre() / 3.5, hauteurFenetre() / 2.9, largeurFenetre() / 4.4, hauteurFenetre() / 3.3);
	triangle(largeurFenetre() / 1.4, hauteurFenetre() / 3.7, largeurFenetre() / 1.4, hauteurFenetre() / 2.9, largeurFenetre() / 1.3, hauteurFenetre() / 3.3);

	//SELECTION AUDIO MUSIQUE
	triangle(largeurFenetre() / 1.8, 4 * hauteurFenetre() / 20, largeurFenetre() / 1.8, 5 * hauteurFenetre() / 20, largeurFenetre() / 2, 4.5 * hauteurFenetre() / 20);
	triangle(largeurFenetre() / 1.4, 4 * hauteurFenetre() / 20, largeurFenetre() / 1.4, 5 * hauteurFenetre() / 20, largeurFenetre() / 1.3, 4.5 * hauteurFenetre() / 20);

	//SELECTION AUDIO BRUITAGE
	triangle(largeurFenetre() / 1.8, 2.5 * hauteurFenetre() / 20, largeurFenetre() / 1.8, 3.5 * hauteurFenetre() / 20, largeurFenetre() / 2, 3 * hauteurFenetre() / 20);
	triangle(largeurFenetre() / 1.4, 2.5 * hauteurFenetre() / 20, largeurFenetre() / 1.4, 3.5 * hauteurFenetre() / 20, largeurFenetre() / 1.3, 3 * hauteurFenetre() / 20);

	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 340);
	//création d'une liste d'équivalent nombre<->string
	char **nombre;
	nombre = (char **)malloc(11 * 2 * sizeof(char *));
	for (int i = 0; i < 11; i++)
	{
		nombre[i] = (char *)malloc(2 * sizeof(char));
	}
	strcpy(nombre[0], "0");
	strcpy(nombre[1], "1");
	strcpy(nombre[2], "2");
	strcpy(nombre[3], "3");
	strcpy(nombre[4], "4");
	strcpy(nombre[5], "5");
	strcpy(nombre[6], "6");
	strcpy(nombre[7], "7");
	strcpy(nombre[8], "8");
	strcpy(nombre[9], "9");
	strcpy(nombre[10], "10");

	//affichage du volume musique
	afficheChaine(nombre[system->infoMusique.Hmusique], largeurFenetre() / 40, largeurFenetre() / 1.6, 4 * hauteurFenetre() / 20);
	if (system->infoMusique.Hmusique == 0)
	{
		couleurCourante(47, 54, 64);
		//AUDIO MUSIQUE A ZERO
		triangle(largeurFenetre() / 1.8, 4 * hauteurFenetre() / 20, largeurFenetre() / 1.8, 5 * hauteurFenetre() / 20, largeurFenetre() / 2, 4.5 * hauteurFenetre() / 20);
	}
	else if (system->infoMusique.Hmusique == 10)
	{
		couleurCourante(47, 54, 64);
		//AUDIO MUSIQUE MAX
		triangle(largeurFenetre() / 1.4, 4 * hauteurFenetre() / 20, largeurFenetre() / 1.4, 5 * hauteurFenetre() / 20, largeurFenetre() / 1.3, 4.5 * hauteurFenetre() / 20);
	}

	//affichage du volume bruitage
	couleurCourante(251, 197, 49);
	afficheChaine(nombre[system->infoMusique.Hbruitage], largeurFenetre() / 40, largeurFenetre() / 1.6, 2.5 * hauteurFenetre() / 20);
	if (system->infoMusique.Hbruitage == 0)
	{
		couleurCourante(47, 54, 64);
		//AUDIO BRUITAGE A ZERO
		triangle(largeurFenetre() / 1.8, 2.5 * hauteurFenetre() / 20, largeurFenetre() / 1.8, 3.5 * hauteurFenetre() / 20, largeurFenetre() / 2, 3 * hauteurFenetre() / 20);
	}
	else if (system->infoMusique.Hbruitage == 10)
	{
		couleurCourante(47, 54, 64);
		//AUDIO BRUITAGE MAX
		triangle(largeurFenetre() / 1.4, 2.5 * hauteurFenetre() / 20, largeurFenetre() / 1.4, 3.5 * hauteurFenetre() / 20, largeurFenetre() / 1.3, 3 * hauteurFenetre() / 20);
	}

	//affichage de la musique
	if (system->infoMenu.Vselection == 0)
	{
		couleurCourante(251, 197, 49);
		//MUSIQUE EN COURS
		rectangle(largeurFenetre() / 3, hauteurFenetre() / 3.7, 2 * largeurFenetre() / 3, hauteurFenetre() / 2.9);
		couleurCourante(47, 54, 64);
		epaisseurDeTrait(largeurFenetre() / 340);
		if (system->infoMusique.Hchoix == 0)
			afficheChaine("T E T R I S 1", largeurFenetre() / 50, largeurFenetre() / 2.4, hauteurFenetre() / 3.4);
		if (system->infoMusique.Hchoix == 1)
			afficheChaine("T E T R I S 2", largeurFenetre() / 50, largeurFenetre() / 2.4, hauteurFenetre() / 3.4);
		if (system->infoMusique.Hchoix == 2)
			afficheChaine("O F F", largeurFenetre() / 50, largeurFenetre() / 2.2, hauteurFenetre() / 3.4);
	}
	else if (system->infoMenu.Vselection == 1)
	{
		couleurCourante(251, 197, 49);
		//Indique que tu fais le réglage audio de la musique
		rectangle(largeurFenetre() / 4.6, 4 * hauteurFenetre() / 22, largeurFenetre() / 2.3, 4 * hauteurFenetre() / 16);
		couleurCourante(47, 54, 64);
		epaisseurDeTrait(largeurFenetre() / 340);
		if (system->infoMusique.Hchoix == 0)
			afficheChaine("T E T R I S 1", largeurFenetre() / 50, largeurFenetre() / 2.4, hauteurFenetre() / 3.4);
		if (system->infoMusique.Hchoix == 1)
			afficheChaine("T E T R I S 2", largeurFenetre() / 50, largeurFenetre() / 2.4, hauteurFenetre() / 3.4);
		if (system->infoMusique.Hchoix == 2)
			afficheChaine("O F F", largeurFenetre() / 50, largeurFenetre() / 2.2, hauteurFenetre() / 3.4);
	}
	else if (system->infoMenu.Vselection == 2)
	{
		couleurCourante(251, 197, 49);
		//Indique que tu fais le réglage audio du bruitage
		rectangle(largeurFenetre() / 4.6, 4 * hauteurFenetre() / 35, largeurFenetre() / 2.2, 4 * hauteurFenetre() / 23);
		couleurCourante(47, 54, 64);
		epaisseurDeTrait(largeurFenetre() / 340);
		if (system->infoMusique.Hchoix == 0)
			afficheChaine("T E T R I S 1", largeurFenetre() / 50, largeurFenetre() / 2.4, hauteurFenetre() / 3.4);
		if (system->infoMusique.Hchoix == 1)
			afficheChaine("T E T R I S 2", largeurFenetre() / 50, largeurFenetre() / 2.4, hauteurFenetre() / 3.4);
		if (system->infoMusique.Hchoix == 2)
			afficheChaine("O F F", largeurFenetre() / 50, largeurFenetre() / 2.2, hauteurFenetre() / 3.4);
	}

	couleurCourante(47, 54, 64);
	epaisseurDeTrait(largeurFenetre() / 340);
	//JI
	afficheChaine("H A U T", largeurFenetre() / 75, largeurFenetre() / 10, hauteurFenetre() / 1.4);
	afficheChaine("G A U C H E", largeurFenetre() / 75, largeurFenetre() / 100, hauteurFenetre() / 1.75);
	afficheChaine("B A S", largeurFenetre() / 75, largeurFenetre() / 9, hauteurFenetre() / 2.5);
	afficheChaine("D R O I T E", largeurFenetre() / 75, largeurFenetre() / 7, hauteurFenetre() / 1.75);
	afficheChaine("C O N F I R M E R", largeurFenetre() / 75, largeurFenetre() / 4.5, hauteurFenetre() / 1.5);
	afficheChaine("R E T O U R", largeurFenetre() / 75, largeurFenetre() / 4.2, hauteurFenetre() / 2.2);

	//J2
	afficheChaine("H A U T", largeurFenetre() / 75, largeurFenetre() / 1.48, hauteurFenetre() / 1.4);
	afficheChaine("G A U C H E", largeurFenetre() / 75, largeurFenetre() / 1.72, hauteurFenetre() / 1.75);
	afficheChaine("B A S", largeurFenetre() / 75, largeurFenetre() / 1.46, hauteurFenetre() / 2.5);
	afficheChaine("D R O I T E", largeurFenetre() / 75, largeurFenetre() / 1.37, hauteurFenetre() / 1.75);
	afficheChaine("C O N F I R M E R", largeurFenetre() / 75, largeurFenetre() / 1.24, hauteurFenetre() / 1.5);
	afficheChaine("R E T O U R", largeurFenetre() / 75, largeurFenetre() / 1.21, hauteurFenetre() / 2.2);

	//afficheChaine("M U S I Q U E", largeurFenetre()/50, largeurFenetre()/2.5, hauteurFenetre()/3.4);

	couleurCourante(47, 54, 64);
	afficheChaine("M U S I Q U E", largeurFenetre() / 50, largeurFenetre() / 4.5, 4 * hauteurFenetre() / 20);
	afficheChaine("B R U I T A G E", largeurFenetre() / 50, largeurFenetre() / 4.5, 2.5 * hauteurFenetre() / 20);

	couleurCourante(251, 197, 49);
	epaisseurDeTrait(largeurFenetre() / 147.69);
	afficheChaine("J 1", largeurFenetre() / 40, largeurFenetre() / 5.5, hauteurFenetre() / 1.1);
	afficheChaine("J 2", largeurFenetre() / 40, largeurFenetre() / 1.2, hauteurFenetre() / 1.1);

	couleurCourante(47, 54, 64);
	epaisseurDeTrait(largeurFenetre() / 147.69);
	//J1
	afficheChaine("Z", largeurFenetre() / 40, largeurFenetre() / 8, hauteurFenetre() / 1.3);
	afficheChaine("Q", largeurFenetre() / 40, largeurFenetre() / 20, hauteurFenetre() / 1.6);
	afficheChaine("S", largeurFenetre() / 40, largeurFenetre() / 8, hauteurFenetre() / 2.2);
	afficheChaine("D", largeurFenetre() / 40, largeurFenetre() / 5.5, hauteurFenetre() / 1.6);
	afficheChaine("V", largeurFenetre() / 40, largeurFenetre() / 3.5, hauteurFenetre() / 1.4);
	afficheChaine("B", largeurFenetre() / 40, largeurFenetre() / 3.5, hauteurFenetre() / 1.9);
	//J2
	triangle(largeurFenetre() / 1.46, hauteurFenetre() / 1.3,
			 largeurFenetre() / 1.41, hauteurFenetre() / 1.24,
			 largeurFenetre() / 1.36, hauteurFenetre() / 1.3);

	triangle(largeurFenetre() / 1.65, hauteurFenetre() / 1.55,
			 largeurFenetre() / 1.55, hauteurFenetre() / 1.49,
			 largeurFenetre() / 1.55, hauteurFenetre() / 1.61);

	triangle(largeurFenetre() / 1.46, hauteurFenetre() / 2,
			 largeurFenetre() / 1.41, hauteurFenetre() / 2.17,
			 largeurFenetre() / 1.36, hauteurFenetre() / 2);

	triangle(largeurFenetre() / 1.24, hauteurFenetre() / 1.55,
			 largeurFenetre() / 1.3, hauteurFenetre() / 1.49,
			 largeurFenetre() / 1.3, hauteurFenetre() / 1.61);

	afficheChaine("L", largeurFenetre() / 40, largeurFenetre() / 1.15, hauteurFenetre() / 1.4);
	afficheChaine("M", largeurFenetre() / 40, largeurFenetre() / 1.15, hauteurFenetre() / 1.9);

	for (int i = 0; i < 11; i++)
	{
		free(nombre[i]);
	}
	free(nombre);
}
