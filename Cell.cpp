
#include "Cell.h"
#include <math.h>
#include <iostream>
using namespace std;

Cell::Cell(int x,int y) : m_x(x),m_y(y)
{

}

void Cell::add_neighb(Cell *c)
{
	this->set_neighb(c);
	c->set_neighb(this);
}

void Cell::set_neighb(Cell *c)
{
	Cell *old_neighb[m_nb_neighb];
	for (int i=0;i<m_nb_neighb;i++)
	{
		old_neighb[i] = m_neighb[i];
	}
	delete m_neighb;
	m_nb_neighb ++;
	m_neighb = new Cell*[m_nb_neighb];

	for (int i=0; i< m_nb_neighb-1; i++){
		m_neighb[i] = old_neighb[i];
	}

	m_neighb[m_nb_neighb-1] = c;
}

void Cell::add_neighb(Cell *c1, Cell *c2)
{
	this->add_neighb(c1);
	this->add_neighb(c2);
}

ostream& operator<<(ostream& stream,  Cell &c)
{
	stream << "(" << c.m_x <<","<< c.m_y<<")";
	return stream;
}

istream &operator>>(istream &stream, Cell &c)
{
	string chain;
	stream >> chain;
	if (chain == "")
	{
		c.m_corrupted = true;
		return stream;
	}
	chain = chain.substr(1,chain.size()-2);
	c.m_x = stoi(chain.substr(0, chain.find(',')));
	c.m_y = stoi(chain.substr(chain.find(',')+1, chain.size()));
	return stream;
}



