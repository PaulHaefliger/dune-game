/*!
  \file   graphic.cc
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Implémentation du module "graphic"
*/

#include <cmath>
#include <iostream>
#include "graphic_gui.h"

using namespace std;

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

// local prototypes
static void set_color(Color color);
// graphic_gui.h
void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr)
{
    ptcr = &cr;
}

// Ecriture de la fonction pour dessiner un cercle
void dessiner_cercle(double x, double y, double r, bool fill, 
													Color color,
													Color fill_color)
{
    if (!ptcr) return;
    (*ptcr)->set_line_width(1);
    set_color(color);
    (*ptcr)->arc(x, y, r, 0, 2 * M_PI);
    if (fill) {
		(*ptcr)->stroke_preserve();
    	set_color(fill_color);
        (*ptcr)->fill();
    } else {
        (*ptcr)->stroke();
	}
}
// Dessine une ligne
void dessiner_ligne(double x1, double y1, double x2, double y2, 
													Color color)
{
    if (!ptcr) return;
    (*ptcr)->set_line_width(0.95);
    set_color(color);
    (*ptcr)->move_to(x1, y1);
    (*ptcr)->line_to(x2, y2);
    (*ptcr)->stroke();
}

void dessiner_cercle_centre(Color color, double r_max)
{
   dessiner_cercle(0, 0, r_max, false, color, color);
}

void dessiner_cercle_rempli(const double x, const double y, Color color,
													Color fill_color,
													double r_viz)
{
    dessiner_cercle(x, y, r_viz, true, color, fill_color);
}

void dessiner_liste_cercle(const std::vector<double>& corps_x,
					  const std::vector<double>& corps_y,
					  double radius, Color color)
{
    for (size_t i(0); i < corps_x.size(); i++) {
        dessiner_cercle(corps_x[i], corps_y[i], radius, false, color, color);
    }
}

void dessiner_liste_points(const std::vector<double>& articulations_x,
					 const std::vector<double>& articulations_y, 
					 Color color, double r_viz, double r_capture)
{
    if (articulations_x.empty() and articulations_y.empty())  return;
    
    for (size_t i = 1; i < articulations_x.size(); ++i) {
		dessiner_ligne(articulations_x[i-1], articulations_y[i-1], 
					articulations_x[i], articulations_y[i], color);
    }
    
    for (size_t i(0); i < articulations_x.size(); i++) {
        dessiner_cercle(articulations_x[i], articulations_y[i],
						r_viz, false, color, color);
    }
    dessiner_cercle(articulations_x.back(), articulations_y.back(),
					r_capture, false, color, color);
}
// local function
static void set_color(Color color)
{
    double r(0.), g(0.), b(0.);
    switch (color)
    {
	case WHITE:
        r = g = b = 1;
        break;
    case GREY:
        r = g = b = 0.5;
        break;
    case BLACK:
        r = g = b = 0.;
        break;
    case RED:
        r = 1;
        break;
    case GREEN:
        g = 0.65;
        break;
    case BLUE:
        r = g = 0.65;
        b = 1;
        break;
    case ORANGE:
        r = 1;
        g = 0.65;
        break;
    case PURPLE:
        r = b = 0.65;
        break;
    case CYAN:
        g = b = 1;
        break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}
