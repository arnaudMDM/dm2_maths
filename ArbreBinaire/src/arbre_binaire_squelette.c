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
		*pa = (*pa)->droit; //bizarre tout ça
		//return tmpArbre->valeur;
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
						//a->droit = supprimer_arbre(a->droit,a->valeur);
						return a;
					}
					else
					{
						t_arbre *tmp = a->gauche;
						//detruire_noeud_arbre(a);//faire plutot detruire arbre
						detruire_arbre ( a );
						return tmp;
					}
				}
				else
				{
					t_arbre *tmp = a->droit;
					//detruire_noeud_arbre(a);//faire plutot detruire arbre
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
				printf ( "%s \t trouve a la hauteur : \t %d\n", chaine,
				        noeud->hauteur );
			}
			else
			{
				printf ( "%s \t non trouve dans l'arbre \n", chaine );
			}
		}
	}
	fclose ( fichier );
}

// calculer la hauteur des noeuds du t_arbre a
void calculer_hauteur ( t_arbre *a )
{
	if ( a )
	{
		int h = 0;
		if ( a->gauche )
		{
			calculer_hauteur ( a->gauche );
			h = a->gauche->hauteur;

			if ( a->droit )
			{
				calculer_hauteur ( a->droit );
				if ( h < a->droit->hauteur )
				{
					h = a->droit->hauteur;
				}
			}

			a->hauteur = h + 1;
		}
		else
		{
			if ( a->droit )
			{
				calculer_hauteur ( a->droit );
				a->hauteur = a->droit->hauteur + 1;
			}
			//Feuille
			else
			{
				a->hauteur = 0;
			}
		}
	}

}

int hmax = 0;
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
		if ( hmax < a->hauteur )
		{
			hmax = a->hauteur;
		}
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
							return equilibrer(a);
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
									return equilibrer(a);
								}
								else
								{
									a->gauche = equilibrer(a->gauche);
									a->droit = equilibrer(a->droit);
									return (a);
								}
							}
							else
							{
								if ( a->droit->droit->hauteur
								        > a->gauche->hauteur )
								{
									a = rotation_gauche ( a );
									return equilibrer(a);
								}
								else
								{
									a->gauche = equilibrer(a->gauche);
									a->droit = equilibrer(a->droit);
									return (a);
								}
							}
						}
						else
						{
							a->gauche = equilibrer(a->gauche);
							a->droit = equilibrer(a->droit);
							return (a);
						}
					}
					else
					{
						if ( a->gauche->gauche->hauteur > a->droit->hauteur )
						{
							a = rotation_droite ( a );
							return equilibrer(a);
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
									return equilibrer(a);
								}
								else
								{
									a->gauche = equilibrer(a->gauche);
									a->droit = equilibrer(a->droit);
									return (a);
								}
							}
							else
							{
								if ( a->droit->droit->hauteur
								        > a->gauche->hauteur )
								{
									a = rotation_gauche ( a );
									return equilibrer(a);
								}
								else
								{
									a->gauche = equilibrer(a->gauche);
									a->droit = equilibrer(a->droit);
									return (a);
								}
							}
						}
						else
						{
							a->gauche = equilibrer(a->gauche);
							a->droit = equilibrer(a->droit);
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
							return equilibrer(a);
						}
						else
						{
							a->gauche = equilibrer(a->gauche);
							a->droit = equilibrer(a->droit);
							return (a);
						}
					}
					else
					{
						if ( a->droit->droit->hauteur > a->gauche->hauteur )
						{
							a = rotation_gauche ( a );
							return equilibrer(a);
						}
						else
						{
							a->gauche = equilibrer(a->gauche);
							a->droit = equilibrer(a->droit);
							return (a);
						}
					}
				}
				else
				{
					a->gauche = equilibrer(a->gauche);
					a->droit = equilibrer(a->droit);
					return(a);
				}
			}
			else if(a->gauche->gauche)
			{
				if ( a->gauche->droit )
				{
					if(a->gauche->gauche->hauteur > a->gauche->droit->hauteur)
					{
						a = rotation_droite(a);
						return equilibrer(a);
					}
					a->gauche = equilibrer(a->gauche);
					return(a);
				}
				else
				{
					if(a->gauche->gauche->hauteur > 0)
					{
						a = rotation_droite(a);
						return equilibrer(a);
					}
					a->gauche = equilibrer(a->gauche);
					return(a);
				}
			}
			else
			{
				a->gauche = equilibrer(a->gauche);
				return(a);
			}
		}
		else if(a->droit)
		{
			if(a->droit->droit)
			{
				if(a->droit->gauche)
				{
					if(a->droit->droit->hauteur > a->droit->gauche->hauteur)
					{
						a = rotation_gauche(a);
						return equilibrer(a);
					}
					a->droit = equilibrer(a->droit);
					return(a);
				}
				else
				{
					if(a->droit->droit->hauteur > 0)
					{
						a = rotation_gauche(a);
						return equilibrer(a);
					}
					a->droit = equilibrer(a->droit);
					return(a);
				}
			}
			else
			{
				a->droit = equilibrer(a->droit);
				return(a);
			}
		}
		else
		{
			return(a);
		}
	}
	else
	{
		return NULL;
	}
}
	int main ( int argc, char *argv[] )
	{
		long clk_tck = CLOCKS_PER_SEC;
		clock_t t1, t2, t3, t4;
		srand ( time ( NULL ) );
//	t1 = clock();
		t_arbre *a = creer_arbre_fichier ( "dico.txt" );
//	t2 = clock();
		calculer_hauteur ( a );

//	t1 = clock();
//	int i;
//	for(i = 0; i< 100;i++)
//	{
//		rechercher_arbre_fichier(a, "a_rechercher.txt");
//	}
//	t2 = clock();
//	a = equilibrer_arbre(a);
//	t3 = clock();
//	for(i = 0; i < 100; i++)
//	{
//		rechercher_arbre_fichier(a, "a_rechercher.txt");
//	}
//	t4 = clock();

//	t3 = clock();

//	afficher_arbre(a);
//	 a = equilibrer_arbre(a);
		afficher_arbre ( a );
		printf ( "\n\n" );
//		supprimer_arbre ( a, creer_elt_str ( "abandoning" ) );
//		calculer_hauteur ( a );
		a = equilibrer(a);
		//calculer_hauteur(a);
		afficher_arbre ( a );
		// a = supprimer_arbre(a,creer_elt_str("abandonment"));
		//calculer_hauteur(a);
		//afficher_arbre(a);
		/*	printf(
		 "Nb ticks/seconde = %ld,  Nb ticks depart : %ld,Nb ticks intermediaires : %ld, Nb ticks final : %ld\n",
		 clk_tck, (long) t1,(long) t2, (long) t3);
		 //	printf("Temps consomme creation :(s) : %f \n", (double) (t2 - t1) / (double) clk_tck);
		 //	printf("Temps consomme calculerHauteur :(s) : %f \n", (double) (t3 - t2) / (double) clk_tck);

		 printf("Temps avant equi :(s) : %f \n", (double) (t2 - t1) / (double) clk_tck);
		 printf("Temps apres equi :(s) : %f \n", (double) (t4 - t3) / (double) clk_tck);
		 printf("hmax : %d",hmax);
		 */
		/*


		 rechercher_arbre_fichier(a, "a_rechercher.txt");
		 * /
		 a = equilibrer_arbre(a);
		 //rechercher_arbre_fichier(a, "a_rechercher.txt");
		 afficher_arbre(a);*/
		detruire_arbre ( a );
		return EXIT_SUCCESS;
	}
