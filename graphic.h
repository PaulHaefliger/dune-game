/*!
  \file   graphic.h
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Interface du module "graphic".
*/

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <vector>

enum Color
{
	WHITE,
    GREY,
    BLACK,
    RED,
    GREEN,
    BLUE,
    ORANGE,
    PURPLE,
    CYAN
};


// Dessine un cercle (version avec coordonnées et rayon)
void dessiner_cercle(double x, double y, double r, bool fill, 
										Color color, Color fill_color);

// Dessine une ligne
void dessiner_ligne(double x1, double y1, double x2, double y2, 
														Color color);

void dessiner_cercle_centre(Color color, double r_max);

void dessiner_cercle_rempli(const double x, const double y, Color color,
						Color fill_color, double r_viz);

void dessiner_liste_cercle(const std::vector<double>& corps_x,
					  const std::vector<double>& corps_y,
					  double radius, Color color);

void dessiner_liste_points(const std::vector<double>& articulations_x,
					 const std::vector<double>& articulations_y, 
					 Color color, double r_viz, double r_capture);
#endif
