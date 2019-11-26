#include "Path.h"

#include <math.h>
#include <iostream>
using namespace std;

Path::Path(const Cell *c) : m_c(c)
{

}

void Path::add_to_path(const Cell* c)
{
	m_next = new Path(c);
}

void Path::display_path()
{	
	if (m_next != NULL)
	{
		vibes::drawBox(
			min(m_c->m_x, m_next->m_c->m_x) + 0.4, max(m_c->m_x, m_next->m_c->m_x) + 0.6,
			min(m_c->m_y, m_next->m_c->m_y) + 0.4, max(m_c->m_y, m_next->m_c->m_y) + 0.6,
			"Blue[Blue]");

		m_next->display_path();
	}
}

Path::~Path()
{
	delete m_c;
	delete m_next;
}