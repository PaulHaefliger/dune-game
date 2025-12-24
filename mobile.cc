/*!
  \file   mobile.cc
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Implémentation du module "mobile"
*/

#include "mobile.h"

Entity::Entity(tools::S2d pos, tools::Polaire dir) :
	position(pos), direction(dir) {}

bool Entity::verifier_dans_arene(double r_max) const {
	tools::Cercle arene(0, 0, r_max);
	if (arene.point_dans_cercle(position)){
		return true;
	}
	return false;
}	

bool Entity::verifier_d_max(double d_max) const {
	if (direction.d > d_max) {
		return false;
	}
	return true;
}

bool Faiseur::verifier_dans_arene(double r_max) const {
	tools::Cercle arene(0, 0, r_max - rayon);
	if (arene.point_dans_cercle(position)){
		return true;
	}
	return false;
}	
	
Faiseur::Faiseur(tools::S2d pos, double r, tools::Polaire dir,
			unsigned nbetemp) :
	Entity(pos, dir), rayon(r), nbe(nbetemp) {}
	
Particule::Particule(tools::S2d pos, tools::Polaire dir, 
											unsigned times) :
	Entity(pos, dir), time(times) {}

bool Faiseur::intersection_faiseur(tools::S2d point) {
	for (unsigned i(0); i < nbe; i++){
		if (tools::Cercle(corps[i], rayon + 2*epsil_zero).point_dans_cercle(point)){
			set_elements(i, i);
			return false;
		}
	}
	return true;
}

bool Faiseur::verifier_rayon(double r_min_faiseur,
							double r_max_faiseur) const {
	if ((rayon <= r_max_faiseur) and (rayon >= r_min_faiseur)) {
		return true;
	}
	return false;
}

bool Faiseur::verifier_nbe() const{
	if (nbe <= 0){
		return false;
	}
	return true;
}

//set element weird stuff
bool Faiseur::verifier_collision_faiseurs(Faiseur faiseur) {
	for (unsigned n(0); n < nbe; n++){
		for (unsigned m(0); m < faiseur.nbe; m++) {
			double distance = calcule_distance(corps[n],
								faiseur.corps[m]);
			if ((rayon + faiseur.rayon) > distance){
				set_elements(n, m);
				return false;
			}
		}
	}
	return true;
}


bool Particule::verifier_c(unsigned time) const {
	if ((time >= 0) and (time_to_split > time)) {
		return true;
	}
	return false;
}

//virtual function for inside arena
void Faiseur::init_corps() {
	corps.clear();
	corps.push_back(position);
	tools::Polaire save_direction(direction);
	tools::Polaire inverse_direction(
					tools::normaliser_angle(direction.a), -direction.d);
	set_direction(inverse_direction);
	tools::Cercle arene(0, 0, r_max);
	for (unsigned k(1); k < nbe; k++) {
		tools::S2d module(corps.back());
		ajouter_deplacement(module, direction);
		if (arene.contient_cercle(tools::Cercle(module, rayon))) {
			corps.push_back(module);
			continue;
		} else {
			tools::S2d module_reflechi(corps.back());
			double normal = tools::calculer_angle(module_reflechi);
			double angle_reflechi = tools::calculer_angle_reflechi(
												direction.a, normal);
			tools::Polaire new_direction(angle_reflechi, direction.d);
			set_direction(new_direction);
			ajouter_deplacement(module_reflechi, direction);
			corps.push_back(module_reflechi);
		}
	}
	set_direction(save_direction);
}

void Faiseur::set_direction(tools::Polaire deplacement){
	direction = deplacement;
}

void Faiseur::set_elements(unsigned a, unsigned b){
	element1 = a;
	element2 = b;
}

tools::Polaire Entity::get_d() const {
	return direction;
}

tools::S2d Entity::get_p() const {
	return position;
}

unsigned Faiseur::get_1() const {
	return element1;
}

unsigned Faiseur::get_2() const {
	return element2;
}
	
unsigned Faiseur::get_nbe() const {
	return nbe;
}
double Faiseur::get_r() const {
	return rayon;
}

unsigned Particule::get_c() const {
	return time;
}

void Entity::incremente(){
	if (will_be_outside(position, direction, r_max)) {
		double normal = tools::calculer_angle(position);
		double reflechi = tools::calculer_angle_reflechi(direction.a, 
														 normal);
		direction.a = reflechi;
	}
	tools::ajouter_deplacement(position, direction);
}

void Faiseur::incremente(){
	if(will_be_outside(position, direction, r_max-rayon)) {
		double normal = tools::calculer_angle(position);
		double reflechi = tools::calculer_angle_reflechi(direction.a, 
														 normal);
		direction.a = reflechi;
	}
	tools::ajouter_deplacement(position, direction);
	if (corps.size() > 1) {
		corps.insert(corps.begin(), position);
		corps.pop_back();
	}
}

void Particule::ajust_angle(double j){
	direction.a += j;
}

void Particule::temps() {
	time += 1;
}

void Particule::reset_time() {
	time = 0;
}

std::vector<tools::S2d> Faiseur::get_corps() const {
	return corps;
}
