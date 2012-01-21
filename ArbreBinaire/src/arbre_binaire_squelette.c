#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXMOT 256

typedef struct t_element
{
	char *elt;
} t_element;

typedef struct t_noeud
{
	t_element *valeur;
	int hauteur;
	struct t_noeud *gauche;
	struct t_noeud *droit;
} t_arbre;

//renvoie le plus grand nombre entre x et y
int max ( int x, int y )
{
	if ( x > y )
	{
		return x;
	}
	else
	{
		return y;
	}
}

//allocation mémoire d'un t_element et initialisation par recopie de x
t_element *creer_elt_str ( char x[MAXMOT] )
{
	t_element *element = (t_element*) malloc ( sizeof(t_element) );
	element->elt = (char *) malloc ( sizeof(char) * MAXMOT );
	strcpy ( element->elt, x );
	return element;
}
//libération de la mémoire occupée par un t_element
void detruire_elt ( t_element *e )
{
	free ( e->elt );
	free ( e );
}

// création d'un arbre binaire
t_arbre *creer_arbre ( t_element *e, t_arbre *g, t_arbre *d, int h )
{
	t_arbre *pArbre = (t_arbre*) malloc ( sizeof(t_arbre) );
	pArbre->valeur = e;
	pArbre->gauche = g;
	pArbre->droit = d;
	pArbre->hauteur = h;

	return pArbre;
}

// destruction d'un noeud de l'arbre, cette fonction renvoie l'élément,
t_element *detruire_noeud_arbre ( t_arbre *a )
{
	t_element *element = a->valeur;
	free ( a );
	return element;
}

// destruction d'un arbre binaire
void detruire_arbre ( t_arbre *a )
{
	if ( a )
	{
		if ( a->gauche )
		{
			detruire_arbre ( a->gauche );
		}
		if ( a->droit )
		{
			detruire_arbre ( a->droit );
		}
		detruire_elt ( detruire_noeud_arbre ( a ) );
	}
}

//Déclaration
t_arbre *inserer_arbre ( t_arbre *a, t_element *v );

// creer un arbre contenant tous les mots de fichier de nom f
t_arbre *creer_arbre_fichier ( char *f )
{
	FILE *fichier;
	t_arbre *arbre = NULL;
	char chaine[MAXMOT];

	fichier = fopen ( f, "r" );
	if ( fichier )
	{
		while (fscanf ( fichier, "%s", chaine ) != EOF)
		{
			arbre = inserer_arbre ( arbre, creer_elt_str ( chaine ) );
		}
	}

	fclose ( fichier );

	return arbre;
}

// insertion d'un t_element v dans un t_arbre a, cette fonction renvoie le nouvel arbre
t_arbre *inserer_arbre ( t_arbre *a, t_element *v )
{
	if ( a == NULL )
	{
		return creer_arbre ( v, NULL, NULL, 0 );
	}
	else
	{
		if ( strcmp ( v->elt, a->valeur->elt ) < 0 )
		{
			a->gauche = inserer_arbre ( a->gauche, v );
		}
		else
		{
			a->droit = inserer_arbre ( a->droit, v );
		}
		return a;
	}

}

t_element * supprimer_min ( t_arbre** pa )
{
	if ( (*pa)->gauche )
	{
		return supprimer_min ( &((*pa)->gauche) );
	}
	else
	{
		t_arbre *tmpArbre = *pa;
		*pa = (*pa)->droit;
		return (detruire_noeud_arbre ( tmpArbre ));
	}
}

// suppression d'un élément, cette fonction renvoie le nouvel arbre (voir cours)
t_arbre *supprimer_arbre ( t_arbre *a, t_element *v )
{
	if ( a )
	{
		if ( strcmp ( v->elt, a->valeur->elt ) < 0 )
		{
			a->gauche = supprimer_arbre ( a->gauche, v );
			return a;
		}
		else
		{
			if ( strcmp ( v->elt, a->valeur->elt ) > 0 )
			{
				a->droit = supprimer_arbre ( a->droit, v );
				return a;
			}
			else
			{
				if ( a->gauche )
				{
					if ( a->droit )
					{
						a->valeur = supprimer_min ( &(a->droit) );
						return a;
					}
					else
					{
						t_arbre *tmp = a->gauche;
						detruire_arbre ( a );
						return tmp;
					}
				}
				else
				{
					t_arbre *tmp = a->droit;
					detruire_arbre ( a );
					return tmp;
				}
			}
		}
	}
	else
	{
		return NULL;
	}
}

static int valRetour;
// recherche d'un t_element e dans le t_arbre a, cette fonction renvoie la référence du noeud dans l'arbre
t_arbre *rechercher_arbre ( t_arbre *a, t_element *e )
{
	if ( a )
	{
		valRetour = strcmp ( e->elt, a->valeur->elt );
		if ( valRetour == 0 )
		{
			return a;
		}
		else
		{
			if ( valRetour < 0 )
			{
				return rechercher_arbre ( a->gauche, e );
			}
			else
			{
				return rechercher_arbre ( a->droit, e );
			}
		}
	}
	else
	{
		return NULL;
	}
}

long hauteurMaxEltsTrouves = 0;
// rechercher l'ensemble des mots du fichier de nom f
void rechercher_arbre_fichier ( t_arbre *a, char *f )
{
	FILE *fichier;
	char chaine[MAXMOT];
	t_arbre *noeud;

	fichier = fopen ( f, "r" );
	if ( fichier )
	{
		while (fscanf ( fichier, "%s", chaine ) != EOF)
		{
			noeud = rechercher_arbre ( a, creer_elt_str ( chaine ) );
			if ( noeud )
			{
				printf ( "%s \t trouve dans l'arbre \n", chaine );
				hauteurMaxEltsTrouves += noeud->hauteur; //ces instructions sont
				//à enlever pour avoir une mesure du temps d'exécution de la fonction plus exacte
			}
			else
			{
				printf ( "%s \t non trouve dans l'arbre \n", chaine ); //cette instruction est
				//à enlever pour avoir une mesure du temps d'exécution de la fonction plus exacte
			}
		}
	}
	fclose ( fichier );
}

long long sommeHauteur = 0;
int profondeurMax = 0; //donne la profondeur max d'un arbre
int profondeurMin = 0; //donne la profondeur min d'un arbre
int profondeurCourant = 0;
// calculer la hauteur des noeuds du t_arbre a
void calculer_hauteur ( t_arbre *a )
{
	if ( a )
	{
		int h = 0;
		if ( a->gauche )
		{
			profondeurCourant += 1;
			calculer_hauteur ( a->gauche );
			h = a->gauche->hauteur;

			if ( a->droit )
			{
				profondeurCourant += 1;
				calculer_hauteur ( a->droit );
				if ( h < a->droit->hauteur )
				{
					h = a->droit->hauteur;
				}
			}

			a->hauteur = h + 1;
			sommeHauteur += a->hauteur;
		}
		else
		{
			if ( a->droit )
			{
				profondeurCourant += 1;
				calculer_hauteur ( a->droit );
				a->hauteur = a->droit->hauteur + 1;
				sommeHauteur += a->hauteur;
			}
			//Feuille
			else
			{
				if ( profondeurMin == 0 )
				{
					profondeurMin = profondeurCourant;
				}
				else if ( profondeurCourant < profondeurMin )
				{
					profondeurMin = profondeurCourant;
				}
				a->hauteur = 0;
			}
		}
		profondeurCourant -= 1;
	}

}

// afficher l'arbre et la hauteur des noeuds
void afficher_arbre ( t_arbre *a )
{
	if ( a )
	{
		if ( a->gauche )
		{
			afficher_arbre ( a->gauche );
		}
		printf ( "hauteur : %d %s \n", a->hauteur, a->valeur->elt );
		if ( a->droit )
		{
			afficher_arbre ( a->droit );
		}
	}
}

static int h;
t_arbre *rotation_droite ( t_arbre *y )
{
	// Rotation
	t_arbre *x = y->gauche;
	y->gauche = x->droit;
	x->droit = y;

	// Maj des hauteurs
	h = 0;

	if ( y->gauche )
	{
		h = y->gauche->hauteur;
	}
	if ( y->droit && y->droit->hauteur > h )
	{
		h = y->droit->hauteur;
	}

	y->hauteur = h + 1;

	x->hauteur = max ( x->gauche->hauteur, x->droit->hauteur ) + 1;

	return x;
}

t_arbre *rotation_gauche ( t_arbre *x )
{
	t_arbre *y = x->droit;
	x->droit = y->gauche;
	y->gauche = x;
	// Maj des hauteurs

	h = 0;

	if ( x->gauche )
	{
		h = x->gauche->hauteur;
	}
	if ( x->droit && x->droit->hauteur > h )
	{
		h = x->droit->hauteur;
	}

	x->hauteur = h + 1;

	y->hauteur = max ( y->gauche->hauteur, y->droit->hauteur ) + 1;
	return y;
}

//equlibre l'arbre en faisant des rotations droites ou gauches
t_arbre * equilibrer ( t_arbre * a )
{
	if ( a )
	{
		if ( a->gauche )
		{
			if ( a->droit )
			{
				if ( a->gauche->gauche )
				{
					if ( a->gauche->droit )
					{
						if ( a->gauche->gauche->hauteur
						        > max ( a->gauche->droit->hauteur,
						                a->droit->hauteur ) )
						{
							a = rotation_droite ( a );
							return equilibrer ( a );
						}
						else if ( a->droit->droit )
						{
							if ( a->droit->gauche )
							{
								if ( a->droit->droit->hauteur
								        > max ( a->droit->gauche->hauteur,
								                a->gauche->hauteur ) )
								{
									a = rotation_gauche ( a );
									return equilibrer ( a );
								}
								else
								{
									a->gauche = equilibrer ( a->gauche );
									a->droit = equilibrer ( a->droit );
									return (a);
								}
							}
							else
							{
								if ( a->droit->droit->hauteur
								        > a->gauche->hauteur )
								{
									a = rotation_gauche ( a );
									return equilibrer ( a );
								}
								else
								{
									a->gauche = equilibrer ( a->gauche );
									a->droit = equilibrer ( a->droit );
									return (a);
								}
							}
						}
						else
						{
							a->gauche = equilibrer ( a->gauche );
							a->droit = equilibrer ( a->droit );
							return (a);
						}
					}
					else
					{
						if ( a->gauche->gauche->hauteur > a->droit->hauteur )
						{
							a = rotation_droite ( a );
							return equilibrer ( a );
						}
						else if ( a->droit->droit )
						{
							if ( a->droit->gauche )
							{
								if ( a->droit->droit->hauteur
								        > max ( a->droit->gauche->hauteur,
								                a->gauche->hauteur ) )
								{
									a = rotation_gauche ( a );
									return equilibrer ( a );
								}
								else
								{
									a->gauche = equilibrer ( a->gauche );
									a->droit = equilibrer ( a->droit );
									return (a);
								}
							}
							else
							{
								if ( a->droit->droit->hauteur
								        > a->gauche->hauteur )
								{
									a = rotation_gauche ( a );
									return equilibrer ( a );
								}
								else
								{
									a->gauche = equilibrer ( a->gauche );
									a->droit = equilibrer ( a->droit );
									return (a);
								}
							}
						}
						else
						{
							a->gauche = equilibrer ( a->gauche );
							a->droit = equilibrer ( a->droit );
							return (a);
						}
					}
				}
				else if ( a->droit->droit )
				{
					if ( a->droit->gauche )
					{
						if ( a->droit->droit->hauteur
						        > max ( a->droit->gauche->hauteur,
						                a->gauche->hauteur ) )
						{
							a = rotation_gauche ( a );
							return equilibrer ( a );
						}
						else
						{
							a->gauche = equilibrer ( a->gauche );
							a->droit = equilibrer ( a->droit );
							return (a);
						}
					}
					else
					{
						if ( a->droit->droit->hauteur > a->gauche->hauteur )
						{
							a = rotation_gauche ( a );
							return equilibrer ( a );
						}
						else
						{
							a->gauche = equilibrer ( a->gauche );
							a->droit = equilibrer ( a->droit );
							return (a);
						}
					}
				}
				else
				{
					a->gauche = equilibrer ( a->gauche );
					a->droit = equilibrer ( a->droit );
					return (a);
				}
			}
			else if ( a->gauche->gauche )
			{
				if ( a->gauche->droit )
				{
					if ( a->gauche->gauche->hauteur
					        > a->gauche->droit->hauteur )
					{
						a = rotation_droite ( a );
						return equilibrer ( a );
					}
					a->gauche = equilibrer ( a->gauche );
					return (a);
				}
				else
				{
					a = rotation_droite ( a );
					return equilibrer ( a );
					a->gauche = equilibrer ( a->gauche );
					return (a);
				}
			}
			else
			{
				a->gauche = equilibrer ( a->gauche );
				return (a);
			}
		}
		else if ( a->droit )
		{
			if ( a->droit->droit )
			{
				if ( a->droit->gauche )
				{
					if ( a->droit->droit->hauteur > a->droit->gauche->hauteur )
					{
						a = rotation_gauche ( a );
						return equilibrer ( a );
					}
					a->droit = equilibrer ( a->droit );
					return (a);
				}
				else
				{
					a = rotation_gauche ( a );
					return equilibrer ( a );
					a->droit = equilibrer ( a->droit );
					return (a);
				}
			}
			else
			{
				a->droit = equilibrer ( a->droit );
				return (a);
			}
		}
		else
		{
			return (a);
		}
	}
	else
	{
		return NULL;
	}
}

int parcours ( t_arbre *a )
{
	if ( a )
	{
		return parcours ( a->gauche ) + parcours ( a->droit ) + 1;
	}
	return 0;
}

void testRecherche ( t_arbre * a, char * nomFic )
{
	long clk_tck = CLOCKS_PER_SEC;
	clock_t t1, t2;
	srand ( time ( NULL ) );

	hauteurMaxEltsTrouves = 0;
	rechercher_arbre_fichier ( a, "a_rechercher.txt" );
	printf ( "hauteur total des noeuds trouvés du fichier %s : %ld\n", nomFic,
	        hauteurMaxEltsTrouves );

	t1 = clock ( );
	int i;
	for ( i = 0; i < 1000; i++ )
	{
		rechercher_arbre_fichier ( a, "a_rechercher.txt" );
	}
	t2 = clock ( );

	printf ( "Temps d'exécution de la recherche du fichier %s :%f\n", nomFic,
	        (double) (t2 - t1) / (double) clk_tck );
	printf ( "\n" );
}

t_arbre *testEquilibrage ( t_arbre * a, char * nomFic )
{
	printf ( "profondeur min avant equilibrage du fichier %s : %d\n", nomFic,
	        profondeurMin );
	if ( a )
	{
		profondeurMax = a->hauteur;
	}
	printf ( "profondeur max avant equilibrage du fichier %s : %d\n", nomFic,
	        profondeurMax );

	profondeurMax = 0;
	profondeurMin = 0;
	profondeurCourant = 0;

	a = equilibrer ( a );
	calculer_hauteur ( a );

	printf ( "profondeur min après equilibrage du fichier %s : %d\n", nomFic,
	        profondeurMin );
	if ( a )
	{
		profondeurMax = a->hauteur;
	}
	printf ( "profondeur max après equilibrage du fichier %s : %d\n \n", nomFic,
	        profondeurMax );
	printf ( "\n" );
	return a;
}

int main ( int argc, char *argv[] )
{
	char *nomFic1 = malloc ( sizeof(MAXMOT) );
	nomFic1 = "dico1.txt";
	t_arbre *a1 = creer_arbre_fichier ( nomFic1 );
	calculer_hauteur ( a1 );

	char *nomFic2 = malloc ( sizeof(MAXMOT) );
	nomFic2 = "dico2.txt";
	t_arbre *a2 = creer_arbre_fichier ( nomFic2 );
	calculer_hauteur ( a2 );

	char *nomFic3 = malloc ( sizeof(MAXMOT) );
	nomFic3 = "dico3.txt";
	t_arbre *a3 = creer_arbre_fichier ( nomFic3 );
	calculer_hauteur ( a3 );

//	testRecherche(a1,nomFic1);
//	testRecherche(a2,nomFic2);
//	testRecherche(a3,nomFic3);

	a1 = testEquilibrage ( a1, nomFic1 );
	a2 = testEquilibrage ( a2, nomFic2 );
	a3 = testEquilibrage ( a3, nomFic3 );

//	testRecherche(a1,nomFic1);
//	testRecherche(a2,nomFic2);
//	testRecherche(a3,nomFic3);

	detruire_arbre ( a1 );
	detruire_arbre ( a2 );
	detruire_arbre ( a3 );
	return EXIT_SUCCESS;
}
