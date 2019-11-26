#ifndef __CELL_H__
#define __CELL_H__


#include <unistd.h>
#include <string>
#include <fstream>
using namespace std;


/**
\class Cell
\brief Créé une cellule de labytrinthe à un endroit donné et définit ses voisins.
\author Quentin Vintras
\date  2019
*/
class Cell
{

	public:
		/**
		\brief Créé une cellule.
		\param x un int, l'abscisse de la cellule
		\param y un int, l'ordonnée de la cellule
		*/
		Cell(int x, int y);
		float m_x;
		float m_y;
		int m_nb_neighb = 0;
		Cell **m_neighb = NULL;
		/**
		\brief Ajoute réciproquement comme voisin cette cellule et la cellule voisine
		\param c un pointeur vers une autre cellule
		*/
		void add_neighb(Cell *c);
		/**
		\brief Ajoute une cellule voisine à la cellule courante
		\param c un pointeur vers une autre cellule
		*/
		void set_neighb(Cell *c);
		/**
		\brief Ajoute réciproquement comme voisin cette cellule et deux cellules voisines
		\param c1 un pointeur vers une autre cellule
		\param c2 un pointeur vers une autre cellule
		*/
		void add_neighb(Cell *c1, Cell *c2);
		bool m_displayed = false;
		bool m_saved = false;
		bool m_corrupted = false;
		bool m_covered = false;
		friend ostream& operator<<(ostream& stream, Cell &c);
		friend istream &operator>>(istream &stream, Cell &c);

	private:
		

};

#endif

