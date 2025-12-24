/*!
  \file   chaine.cc
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Implémentation du module "chaine"
*/

#include "chaine.h"
#include <cmath>

bool Chaine::verifier_dans_arene(double r_max) const{
	tools::Cercle c(0, 0, r_max);
	for (size_t i(0); i < chaine.size(); i++){
		if (!c.point_dans_cercle(chaine[i])){
			return false;
		}
	}
	return true;
}

bool Chaine::verifier_longueur(double r_capture) const {
	for (size_t i(1); i < chaine.size(); i++){
		if (calcule_distance(chaine[i], chaine[i-1]) > 
									r_capture + epsil_zero){
			return false;
		}
	}
	return true;
}

tools::S2d Chaine::get_derniere_articulation() const{
	return chaine.back();
}

void Chaine::ajouter_articulation(tools::S2d point){
	chaine.push_back(point);
}

bool Chaine::verifier_racine(double r_capture, double r_max) const{
	double distance = tools::calcule_norme(chaine[0]);
	if (abs(distance - r_max) > r_capture + epsil_zero){
		return false;
	}
	return true;
}

tools::S2d Chaine::get_articulation(unsigned i) const{
	return chaine[i];
}

void Chaine::enlever_articulation(){
	chaine.pop_back();
}

std::vector<tools::S2d> Chaine::get_chaine() const {
	return chaine;
}

void Chaine::clear() {
	chaine.clear();
}
