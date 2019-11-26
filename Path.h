#ifndef __PATH_H__
#define __PATH_H__

#include "Cell.h"
#include "vibes.h"

#include <unistd.h>
#include <string>



/**
\class Path
\brief Permet de trouver et d'afficher le chemin d'un labyrinthe
\author Quentin Vintras
\date  2019
*/

class Path{

	public:
		/**
		\brief Créé un chemin.
		\param c un pointeur vers l'objet Cell
		*/
		Path(const Cell *c);

		/**
		\brief Détruit l'objet Path.
		*/
		~Path();

		/**
		\brief Ajoute une cellule au chemin
		\param c un pointeur vers l'objet Cell
		*/
		void add_to_path(const Cell *c);

		/**
		\brief Affiche un chemin
		*/
		void display_path();


		const Cell *m_c;
		Path *m_next = NULL;

};

#endif