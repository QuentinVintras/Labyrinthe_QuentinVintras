#include "Cell.h"
#include "vibes.h"
#include "Maze.cpp"
#include <fstream>
#include "Path.h"
using namespace std;

void draw_gate(const Cell *n1, const Cell *n2)
{
	vibes::drawBox(min(n1->m_x, n2->m_x) + 0.1, max(n1->m_x, n2->m_x) + 0.9,
	min(n1->m_y, n2->m_y) + 0.1, max(n1->m_y, n2->m_y) + 0.9,
	"lightGray[lightGray]");
}


void display_cell(Cell *cell)
{
	vibes::drawBox(cell->m_x, cell->m_x + 1, cell->m_y, cell->m_y + 1, "[lightGray]"); // on consrtuit la cellule à la position m_x+1 m_y+1
	cell->m_displayed = true; // on notifie que la cellule est déjà affichée pour ne pas le faire plsuieurs fois
	for(int i = 0 ; i < cell->m_nb_neighb ; i++) // pour tous les voisins de la cellule sélectionnée
	{
		if(!cell->m_neighb[i]->m_displayed) // si le voisin n'est pas déjà affiché
		display_cell(cell->m_neighb[i]); // récursivité
		draw_gate(cell, cell->m_neighb[i]); // on construit une porte entre la cellule et son voisin
	}
}



Maze create_maze()
// on créé le labyrinthe du TP3 sur les pointeurs à la main
{
	Maze maze;
	int nb_line = 4;
	int nb_col = 4;
	Cell **cells = new Cell*[nb_line*nb_col];
	for (int i=0;i<nb_line;i++)
		{
			for(int j=0;j<nb_col;j++)
			{
				cells[i*nb_col+j] = new Cell(i,j);
			}
		}
	cells[3]->add_neighb(cells[7]);
	cells[7]->add_neighb(cells[11]);
	cells[11]->add_neighb(cells[15]);
	cells[15]->add_neighb(cells[14]);
	cells[14]->add_neighb(cells[13]);
	cells[13]->add_neighb(cells[12]);
	cells[12]->add_neighb(cells[8]);
	cells[8]->add_neighb(cells[9]);
	cells[9]->add_neighb(cells[5]);
	cells[5]->add_neighb(cells[4]);
	cells[5]->add_neighb(cells[6]);
	cells[6]->add_neighb(cells[10]);
	cells[4]->add_neighb(cells[0]);
	cells[0]->add_neighb(cells[1]);
	cells[1]->add_neighb(cells[2]);

	maze.start_cell = cells[3];
	maze.finish_cell = cells[2];

	return maze;
}

void display (Maze maze)
// affichage d'un labyrinthe en entier par récurisvité avec mise en évidence des cellules de départ et d'arrivée
{
	Cell* current_cell = maze.start_cell;
	display_cell(current_cell);
		for (int i=0; i< current_cell->m_nb_neighb; i++){
			display_cell(current_cell->m_neighb[i]);
		}

	Cell* start_cell = maze.start_cell;
	Cell* stop_cell = maze.finish_cell;
	// dessin de la cellule de départ
	vibes::drawCircle(
		start_cell->m_x+0.5, start_cell->m_y+0.5,0.4, // dessin d'un cercle de rayon 0,4 dans la cellule
		"Green[Green]");
	// dessin de la cellule d'arrivée
	vibes::drawCircle(
		stop_cell->m_x+0.5, stop_cell->m_y+0.5,0.4, // dessin d'un cercle de rayon 0,4 dans la cellule
		"Red[Red]");
} 

void save_cell(Cell *cell, ofstream *f)
// sauvegarde d'une cellule par écriture d'un fichier texte
{	

	*f << cell;  // on affiche la cellule
	*f << cell->m_nb_neighb; //son nombre de voisins
	cell->m_saved = true;
	for(int i = 0 ; i < cell->m_nb_neighb ; i++) // on affiche toutes les cellules voisines
	{
		*f << cell->m_neighb[i];
	}
	*f << endl; // on revient à la ligne
	for(int i = 0 ; i < cell->m_nb_neighb ; i++) // boucle de récursivité
	{
		if (!cell->m_neighb[i]->m_saved){
			save_cell(cell->m_neighb[i],f);
		}
	}

	}


void save_maze(const Maze& maze, const string& file_name)
// sauvegarde d'un labyrinthe par écriture d'un fichier texte
{
	ofstream ofst(file_name); // on ouvre un fichier texte en écriture dans un flux
	// on écrit dans ce flux les données qui nous intéressent
	ofst << "#Start :" << endl;
	ofst << maze.start_cell << endl;
	ofst << "#End :" << endl;
	ofst << maze.finish_cell << endl;
	ofst << "#Cells :" << endl;
	Cell* current_cell = maze.start_cell;
	// boucle où l'on fait appel à save_cell pour écrire chaque cellule avec le formalisme choisi
	for (int i = 0; i < current_cell->m_nb_neighb; i++){
		save_cell(current_cell,&ofst);
	}
}

bool find_path(Cell *c, Cell *cf, Path *path, bool& over)
// résolution du labyrinthe par récusivité
{	
	c->m_covered = true;

	if (c == cf) // si la cellule rencontrée est l'objectif, on arrête la récursivité
	{
		over = true;
		return true;
	}

	for(int i=0; i< c->m_nb_neighb; i++)
	{
		if (!c->m_neighb[i]->m_covered)
		{
			if (!over)
				path->add_to_path(c->m_neighb[i]);
			find_path(c->m_neighb[i], cf, path->m_next, over); 
		}
	}
}



pair<int,int> convert_coords(string coords)
// permet la lecture de fichiers, fait la conversion entre les coordonnées données en chaines de caractères et une paire de int
{
	coords = coords.substr(1, coords.size()-2);
	int x = stoi(coords.substr(0, coords.find(',')));
	int y = stoi(coords.substr(coords.find(',')+1, coords.size()));
	return pair<int,int> (x, y);
}

Maze read_maze(const string& file_name)
// lit un fichier pour créer et lier des cellules et renvoie un labyrinthe
{
	ifstream f(file_name); // on ouvre le flux
	Maze maze;
	if(!f.is_open()) // teste si le fichier txt peut être ouvert ou non
	{
		cout << "Le fichier suivant ne peut pas être ouvert : " << file_name << endl;
		return maze;
	}

	string line = "";
	string startCoords = "";
	string stopCoords = "";

	// on lit les premières lignes et on retient les coordonnées des cellules de départ et d'arrivée
	getline(f, line);
	getline(f, startCoords);
	getline(f, line);
	getline(f, stopCoords);
	getline(f, line);

	pair<int,int> startCellCoords = convert_coords(startCoords);
	pair<int,int> stopCellCoords = convert_coords(stopCoords);

	// création du dictionnaire de stockage des cellules
	map<pair<int,int>, Cell*> cells;

	while(!f.eof()) // tant que l'on arrive pas à la fin du fichier
	{
		Cell *cell = new Cell(0,0); // on initialise un pointeur vers une cellule
		int nb_neighb;
		f >> *cell >> nb_neighb; // on lit sur le fichier la cellule et le nombre de voisins
		if(cell->m_corrupted) // si cette cellule existe déjà 
		{
			delete cell;
			break;
		}

		if(cells.find(pair<int,int> (cell->m_x, cell->m_y)) == cells.end()) //si la clé du dictionnaire n'existe pas
		{
			cells[pair<int,int> (cell->m_x, cell->m_y)] = cell; //cette clé est créée
		}
		else // sinon on reprend la cellule déjà créée grâce au dictionnaire
		{
			int x = cell->m_x;
			int y = cell->m_y;
			delete cell;
			cell = cells.find(pair<int,int> (x, y))->second; // on trouve la cellule avec la clé du dictionnaire
		}

		for (int i=0; i<nb_neighb; i++) // création des voisin de la cellule
		{
			Cell *neighb = new Cell(0,0);
			f >> *neighb;
			if(cells.find(pair<int,int> (neighb->m_x, neighb->m_y)) == cells.end())//si la clé du dictionnaire n'existe pas
			{
				cell->add_neighb(neighb);
				cells[pair<int,int> (neighb->m_x, neighb->m_y)] = neighb; // on créé la clé
			}
			else // sinon on ajoute le voisin par recherche dans le dictionnaire
			{
				cell->add_neighb(cells.find(pair<int,int> (neighb->m_x, neighb->m_y))->second);
				delete neighb;
			}
		}
		getline(f, line); // on lit la ligne suivante
	}

	// création du labyrinthe
	maze.start_cell = cells.find(startCellCoords)->second; 
	maze.finish_cell = cells.find(stopCellCoords)->second;

	return maze;
}



// Sérialise le labyrinthe du TP3
/*int main(){
	vibes::beginDrawing();
	vibes::newFigure("Jam");
	vibes::setFigureProperties("Jam",
	vibesParams("x", 100, "y", 100, "width", 400, "height", 400));
	vibes::axisLimits(-20., 20., -20., 20.);
	Maze maze = create_maze();
	display(maze);
	save_maze(maze,"labytp3");
	return EXIT_SUCCESS;
	}
*/

int main(){
	vibes::beginDrawing();
	vibes::newFigure("Jam");
	vibes::setFigureProperties("Jam",
	vibesParams("x", 100, "y", 100, "width", 400, "height", 400));
	vibes::axisLimits(-20., 20., -20., 20.);
	Maze maze = read_maze("laby8x8.txt");
	display(maze);
	bool over = false;
	Path* path = new Path(maze.start_cell);
	find_path(maze.start_cell, maze.finish_cell, path, over);
	path->display_path();
	return EXIT_SUCCESS;
}

