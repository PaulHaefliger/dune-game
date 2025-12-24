/*!
  \file   tools.cc
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Implémentation du module "tools"
*/

#include <cmath>
#include "tools.h"

tools::S2d::S2d(double x_val, double y_val){
	x = x_val;
	y = y_val;
}

tools::Polaire::Polaire(double a_val, double d_val){
	a = a_val;
	d = d_val;
};

tools::Polaire tools::passage_cartesienne_polaire(S2d p){
	double a = atan2(p.y, p.x);
	double d = sqrt(p.x * p.x + p.y * p.y);
	Polaire pp(a, d);
	return pp;
}

tools::S2d tools::passage_polaire_cartesienne(Polaire p){
	double x = p.d * cos(p.a);
	double y = p.d * sin(p.a);
	S2d pp(x, y);
	return pp;
}

double tools::calcule_distance(S2d p1, S2d p2){
	double d1 = p1.x - p2.x;
	double d2 = p1.y - p2.y;
	double r = sqrt(d1*d1 + d2*d2);
	return r;
}

bool tools::Cercle::point_dans_cercle(S2d p) const {
	if (calcule_distance(position, p) < (r - epsil_zero)){
		return true;
	}
	return false;
}

bool tools::Cercle::contient_cercle(Cercle c) const {
	if (calcule_distance(position, c.position) < (r - c.r - epsil_zero))
		return true;
	return false;
}

bool tools::Cercle::cercle_intersecte(Cercle c) const {
	if (calcule_distance(position, c.position) < (r + c.r - epsil_zero))
		return true;
	return false;
}

double tools::calculer_angle_reflechi(double angle_incident,
									double angle_normal) {
	double angle_reflechi = M_PI + 2.0*angle_normal - angle_incident;
	return normaliser_angle(angle_reflechi);
}

double tools::normaliser_angle(double angle){
	while (angle > M_PI) {
		angle -= 2.0 * M_PI;
	}
	while (angle <= -M_PI) {
		angle += 2.0 * M_PI;
	}
	return angle;
}

tools::Polaire tools::calculer_vecteur_reflechi(const Polaire incident,
												Polaire normal){
	double angle = calculer_angle_reflechi(incident.a, normal.a);
	Polaire vecteur_reflechi(angle, incident.d);
	return vecteur_reflechi;
}

double tools::calcule_norme(S2d vector){
	double norme = sqrt(vector.x * vector.x + vector.y * vector.y);
	return norme;
}

void tools::Cercle::get_r(double& rtemp) const {
	rtemp = r;
}

void tools::Cercle::get_p(S2d& ptemp) const {
	ptemp = position;
}

double tools::calculer_angle(S2d point){
	return atan2(point.y, point.x);
}

tools::S2d tools::Cercle::get_pos() const{
	return position;
}

void tools::ajouter_deplacement(tools::S2d& c, tools::Polaire p){
	c.x += cos(p.a) * p.d;
	c.y += sin(p.a) * p.d;
}

bool tools::will_be_outside(S2d pos, Polaire dir, double r_max) {
	ajouter_deplacement(pos, dir);
	if (Cercle(0, 0, r_max).point_dans_cercle(pos)) {
		return false;
	}
	return true;
}
