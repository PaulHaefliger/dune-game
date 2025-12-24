/*!
  \file   jeu.cc
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Implémentation du module "jeu"
*/

#include "jeu.h"
#include "message.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "tools.h"
//L'unique instance globale du jeu
static Jeu jeu_instance;

bool Jeu::check_total;
//methode classe jeu implementation
Jeu::Jeu() : score(0), mode(CONSTRUCTION), position_souris(0,0), 
			region_capture(0,0), but_final(0,0), etat_jeu(ONGOING) {}

unsigned Jeu::get_score() const{
	return score;
}

const std::vector<Particule>& Jeu::get_particules() const {
	return particules;
}
const std::vector<Faiseur>& Jeu::get_faiseurs() const {
	return faiseurs;
}

const Chaine& Jeu::get_chaine() const {
	return chaine;
}

Mode Jeu::get_mode() const {
	return mode;
}

bool Jeu::get_check() const {
	return check_total;
}

void Jeu::check(bool b) {
	check_total = b;
}

void set_check(bool b) {
	Jeu temp;
	temp.check(b);
}

tools::S2d Jeu::get_but() {
	return but_final;
}

void Jeu::set_but(tools::S2d but){
	but_final = but;
}

void lire_ligne(const std::string& nom_fichier, std::string& ligne,
				unsigned& nb_ligne){
	std::ifstream fichier(nom_fichier);
	for (unsigned i(0); i < nb_ligne; i++){
		getline(fichier>>std::ws, ligne);
	}
	while (std::getline(fichier>>std::ws, ligne)) {
		nb_ligne += 1;
		if (sauter_ligne(ligne)) {
			continue;
		}
		break;
	}
}

bool Jeu::check_score() const{
	if ((score <= 0) or (score > score_max)){
		std::cout << message::score_outside(score);
		set_check(false);
		return false;
	}
	return true;
}

bool check_nb_particules(double nb_particules){
	if ((nb_particules < 0) or (nb_particules > nb_particule_max)) {
		std::cout<<message::nb_particule_outside(nb_particules);
		set_check(false);
		return false;
	}
	return true;
}

bool check_particule(Particule particule){
	if (!particule.verifier_d_max(d_max)){
		std::cout<< message::mobile_displacement(particule.get_d().d);
		set_check(false);
		return false;
	}
	if (!particule.verifier_dans_arene(r_max)) {
		std::cout<<message::particule_outside(particule.get_p().x,
											particule.get_p().y);
		set_check(false);
		return false;
	}
	if (!particule.verifier_c(particule.get_c())) {
		std::cout<<message::particule_counter(particule.get_c());
		set_check(false);
		return false;
	}
	return true;
}

bool check_nb_faiseurs(unsigned nb_faiseurs){
	if (nb_faiseurs<0) {
		std::cout<<message::faiseur_nbe(nb_faiseurs);
		set_check(false);
		return false;
	}
	return true;
}

bool check_faiseur(Faiseur faiseur, std::vector<Faiseur> faiseurs,
					unsigned b){
	if (!faiseur.verifier_d_max(d_max)) {
		std::cout << message::mobile_displacement(faiseur.get_r());
		set_check(false);
		return false;
	}
	if (!faiseur.verifier_rayon(r_min_faiseur, r_max_faiseur)) {
		std::cout << message::faiseur_radius(faiseur.get_r());
		set_check(false);
		return false;
	}	
	if (!faiseur.verifier_nbe()){
		std::cout << message::faiseur_nbe(faiseur.get_nbe());
		set_check(false);
		return false;
	}
	if (!faiseur.verifier_dans_arene(r_max)) {
		std::cout << message::faiseur_outside(faiseur.get_p().x,
											faiseur.get_p().y);
		set_check(false);
		return false;
	}
	for (unsigned j(0); j < b; j++) {
		if (!faiseur.verifier_collision_faiseurs(faiseurs[j])) {	
			std::cout << message::faiseur_element_collision(
			b, faiseur.get_1(), j, faiseur.get_2());
			set_check(false);
			return false;
		}
	}
	return true;
}

bool check_articulation(unsigned i, Chaine liste,
						std::vector<Faiseur> faiseurs, unsigned nb){
		tools::S2d point = liste.get_articulation(i);
		if (tools::calcule_norme(point)>r_max){
			liste.enlever_articulation();
			std::cout<<message::articulation_outside(point.x, point.y);
			set_check(false);
			return false;
		} 	
		if(!liste.verifier_racine(r_capture, r_max)){
			liste.enlever_articulation();
			std::cout<<message::chaine_racine(point.x, point.y);
			set_check(false);
			return false;
		}
		if (nb>1 and !liste.verifier_longueur(r_capture)) {
			liste.enlever_articulation();
			std::cout<<message::chaine_max_distance(i-1);
			set_check(false);
			return false;
		}
		for (unsigned j(0); j < faiseurs.size(); j++) {
			if (!faiseurs[j].intersection_faiseur(point)){
				liste.enlever_articulation();
				std::cout<<message::chaine_articulation_collision(
											i ,j, faiseurs[j].get_1());
				set_check(false);
				return false;
			}
		}
		return true;
	}

bool Jeu::lecture(const std::string& nom_fichier) {
	//Reinitialisation de letat du jeu
	if (nom_fichier == "") {
		etat_jeu = WON;
		return false;
	}
	etat_jeu = ONGOING;
	check_total = true;
	particules.clear();
	faiseurs.clear();
	but_final.x = 0;
	chaine = Chaine();
	std::string ligne;
	unsigned nb_ligne(0);
	lire_ligne(nom_fichier, ligne, nb_ligne);
	std::istringstream iss(ligne);
	iss >> score;
	check_score();
	//lecture et verification des particules
	unsigned nb_particules = 0;
	lire_ligne(nom_fichier, ligne, nb_ligne);
	std::istringstream iss1(ligne);
	iss1 >> nb_particules;
	check_nb_particules(nb_particules);
	for (unsigned i = 0; i < nb_particules; i++) {
		lire_ligne(nom_fichier, ligne, nb_ligne);
		std::istringstream iss2(ligne);
		double x, y, a, d;
		unsigned c;
		iss2 >> x >> y >> a >> d >> c;
		Particule nparticule(tools::S2d(x, y), tools::Polaire(a, d), c);
		check_particule(nparticule);
		particules.push_back(Particule(nparticule));
	}	
	//lecture et verification des faiseurs
	unsigned nb_faiseurs = 0;
	lire_ligne(nom_fichier, ligne, nb_ligne);
	std::istringstream iss3(ligne);
	iss3 >> nb_faiseurs;
	check_nb_faiseurs(nb_faiseurs);
	for(unsigned i = 0; i < nb_faiseurs; i++) {
		lire_ligne(nom_fichier, ligne, nb_ligne);
		std::istringstream iss4(ligne);
		double x, y, a, d, r;
		unsigned nbe;
		iss4 >> x >> y >> a >> d >> r >> nbe;
		Faiseur nouveau_faiseur(tools::S2d(x, y), r,
								tools::Polaire(a, d), nbe);
		nouveau_faiseur.init_corps();
		if (check_faiseur(nouveau_faiseur, faiseurs, i)){
			faiseurs.push_back(nouveau_faiseur);
		}
	}
	unsigned nb_articulations = 0;
	lire_ligne(nom_fichier, ligne, nb_ligne);
	std::istringstream iss5(ligne);
	iss5 >> nb_articulations;
	for(unsigned i=0; i<nb_articulations; i++) {
		lire_ligne(nom_fichier, ligne, nb_ligne);
		std::istringstream iss6(ligne);
		double x, y;	
		iss6 >> x >> y;
		tools::S2d position(x, y);
		chaine.ajouter_articulation(position);
		check_articulation(i, chaine, faiseurs, nb_articulations);
	}
	lire_ligne(nom_fichier, ligne, nb_ligne);
	std::istringstream iss7(ligne);
	std::string mode_str;
	iss7 >> mode_str;
	if(mode_str == "CONTRUCTION"){
		mode = CONSTRUCTION;
	} else if (mode_str =="GUIDAGE") {
		mode = GUIDAGE;
	}
	if (check_total == true){
		std::cout<<message::success();
		return true;
	}
	return false;
}


void Jeu::sauvegarder(const std::string& nom_fichier) {
	std::ofstream fichier(nom_fichier);
	fichier << "# " << nom_fichier << "\n# Score\n" << get_score()
			<< "\n\n# Particules\n" << particules.size() << "\n";
	for (size_t i(0); i < particules.size(); i++) {
		fichier << "\t" << point_check(particules[i].get_p().x) << "\t"
				<< point_check(particules[i].get_p().y) << "\t"
				<< point_check(particules[i].get_d().a) << "\t"
				<< point_check(particules[i].get_d().d) << "\t"
				<< particules[i].get_c() << "\n";
	}
	fichier << "\n# Faiseurs\n" << faiseurs.size() << "\n";
	for (size_t i(0); i < faiseurs.size(); i++) {
		fichier << "\t" << point_check(faiseurs[i].get_p().x) << "\t"
				<< point_check(faiseurs[i].get_p().y) << "\t" 
				<< point_check(faiseurs[i].get_d().a) << "\t"
				<< point_check(faiseurs[i].get_d().d) << "\t"
				<< point_check(faiseurs[i].get_r()) << "\t"
				<< faiseurs[i].get_nbe() << "\n";
	}
	std::vector<tools::S2d> liste = chaine.get_chaine();
	fichier << "\n# Articulations\n" << liste.size() << "\n";
	for (size_t i(0); i < liste.size(); i++) {
		fichier << "\t" << point_check(liste[i].x) << "\t"
				<< point_check(liste[i].y) << "\n";
	} 
	fichier << "\n# Mode\n" << get_mode();
}

void Jeu::set_mode(Mode nouveau_mode){
	mode = nouveau_mode;
}

Jeu::Etat Jeu::get_etat_jeu() const {
	return etat_jeu;
}

void Jeu::set_position_souris(const tools::S2d& pos) {
	position_souris = pos;
	if (chaine.get_chaine().empty()) {
		double norme = tools::calcule_norme(position_souris);
		if (norme < epsil_zero) {
			//pour eviter divison par 0
			region_capture = tools::S2d(r_max, 0);
		} else {
			region_capture.x = position_souris.x*r_max/norme;
			region_capture.y = position_souris.y*r_max/norme;
		}
	} else {
		region_capture = chaine.get_derniere_articulation();
	}
}

tools::S2d Jeu::get_region_capture(){
	return region_capture;
}

bool Jeu::capturer_particule() {
	if (mode != CONSTRUCTION) {
		return false;
	}

	tools::Cercle zone_capture(region_capture, r_capture);
	int particules_dans_zone = 0;
	int index_particule = -1;
	
	for (size_t i = 0; i<particules.size(); ++i) {
		if (zone_capture.point_dans_cercle(particules[i].get_p())) {
			particules_dans_zone +=1;
			index_particule = i;
		}	
	}
	//si il y a exactement une particule dans la zone --> capture
	if (particules_dans_zone==1 and index_particule != -1) {
		chaine.ajouter_articulation(
		particules[index_particule].get_p());
		if (chaine.get_chaine().size()==1) {
			but_final = calculer_but_final(chaine.get_chaine()[0]);
		}
		//supprime la particule capturée
		particules.erase(particules.begin() + index_particule);
		score -= 1;
		
		return true;
	}

	return false;
}

tools::S2d Jeu::calculer_but_final(const tools::S2d& racine) const{
    double norme = tools::calcule_norme(racine);
    if (norme < epsil_zero) {
        // Éviter la division par zéro
        return tools::S2d(-r_max, 0);
    }
    
    return tools::S2d(-racine.x * r_max / norme, 
										-racine.y * r_max / norme);
}

tools::S2d Jeu::calculer_but_intermediaire() const {
    if (chaine.get_chaine().empty()) {
        return position_souris;
    }
    
    tools::S2d effecteur = chaine.get_derniere_articulation();
    
    tools::S2d direction(0, 0);
    direction.x = position_souris.x - effecteur.x;
    direction.y = position_souris.y - effecteur.y;
    
    double distance = tools::calcule_norme(direction);
    
    if (distance <= r_capture) {
        return position_souris;
    }
    
    if (distance > epsil_zero) { 
        direction.x = direction.x * r_capture / distance;
        direction.y = direction.y * r_capture / distance;
    } else {
        // Cas improbable mais à gérer au cas ou 
        direction.x = r_capture;
        direction.y = 0;
    }
    
    return tools::S2d(effecteur.x + direction.x, effecteur.y + direction.y);
}

bool Jeu::verifier_victoire() {
    if (chaine.get_chaine().empty()) {
        return false;
    }
    tools::S2d effecteur = chaine.get_derniere_articulation();
    tools::Cercle zone_capture(effecteur, r_capture);
    if (zone_capture.point_dans_cercle(but_final)) {
        etat_jeu = WON;
        set_check(true);
        return true;
    }
    
    return false;
}

bool Jeu::verifier_collisions_faiseurs() {
	unsigned size_c = chaine.get_chaine().size();
	for(unsigned i=0; i < size_c; i++) {			
		if (!check_articulation(i, chaine, faiseurs, size_c)){
			set_check(true);
			chaine.clear();
			std::cout << "You Lost !" << std::endl;
			return true;
		}
	}
    
    return false;
}


bool Jeu::verifier_articulations_dans_arene(
			const std::vector<tools::S2d>& articulations) const {
    tools::Cercle arene(0, 0, r_max);
    for (const auto& articulation : articulations) {
        if (!arene.point_dans_cercle(articulation)) {
            return false;
        }
    }
    return true;
}

// Calcule les longueurs entre articulations consécutives
std::vector<double> Jeu::calculer_longueurs(
				const std::vector<tools::S2d>& articulations) const {
    std::vector<double> longueurs(articulations.size() - 1);
    for (size_t i = 0; i < articulations.size() - 1; i++) {
        longueurs[i] = tools::calcule_distance(articulations[i], 
												articulations[i+1]);
    }
    return longueurs;
}

// Première itération de l'algorithme de guidage: de l'effecteur 
// vers la racine
bool Jeu::iteration_effecteur_vers_racine(
        const std::vector<tools::S2d>& articulations,
        const std::vector<double>& longueurs,
        const tools::S2d& point_intermediaire,
        std::vector<tools::S2d>& articulations_prime) const {
    
    // On place l'effecteur sur le but intermédiaire
    articulations_prime[articulations.size() - 1] = point_intermediaire;
    
    // Calcule les positions des autres articulations
    for (int i = articulations.size() - 2; i >= 0; --i) {
        tools::S2d direction(0, 0);
        direction.x = articulations[i].x - articulations_prime[i+1].x;
        direction.y = articulations[i].y - articulations_prime[i+1].y;
        
        double norme = tools::calcule_norme(direction);
        if (norme < epsil_zero) {
            return false;  // Éviter la division par zéro
        }
        
        direction.x = direction.x * longueurs[i] / norme;
        direction.y = direction.y * longueurs[i] / norme;
        
        articulations_prime[i].x = articulations_prime[i+1].x +
														direction.x;
        articulations_prime[i].y = articulations_prime[i+1].y + 
														direction.y;
    }
    
    return true;
}

// Deuxième itération de l'algorithme de guidage: de la racine
//vers l'effecteur
bool Jeu::iteration_racine_vers_effecteur(
        const std::vector<tools::S2d>& articulations,
        const std::vector<tools::S2d>& articulations_prime,
        const std::vector<double>& longueurs,
        std::vector<tools::S2d>& articulations_prime_prime) const {
    
    // La racine reste fixe
    articulations_prime_prime[0] = articulations[0];
    
    // Calculer les positions des autres articulations
    for (size_t i = 0; i < articulations.size() - 1; i++) {
        tools::S2d direction(0, 0);
        direction.x = articulations_prime[i+1].x - 
									articulations_prime_prime[i].x;
        direction.y = articulations_prime[i+1].y - 
									articulations_prime_prime[i].y;
        
        double norme = tools::calcule_norme(direction);
        if (norme < epsil_zero) {
            return false;  // Éviter la division par zéro
        }
        
        direction.x = direction.x * longueurs[i] / norme;
        direction.y = direction.y * longueurs[i] / norme;
        
        articulations_prime_prime[i+1].x = 
						articulations_prime_prime[i].x + direction.x;
        articulations_prime_prime[i+1].y = 
        
						articulations_prime_prime[i].y + direction.y;
    }
    
    return true;
}

bool Jeu::algo_guidage(const tools::S2d& point_intermediaire) {
    if (mode != GUIDAGE or chaine.get_chaine().size() < 2) {
        return false;
    }
    
    std::vector<tools::S2d> articulations = chaine.get_chaine();
    std::vector<tools::S2d> articulations_prime(articulations.size());
    std::vector<tools::S2d> articulations_prime_prime(articulations.size());
    
    // Calcul des longueurs entre articulations consécutives
    std::vector<double> longueurs = calculer_longueurs(articulations);
    
    // Première itération : de l'effecteur vers la racine
    if (!iteration_effecteur_vers_racine(articulations, longueurs, 
                         point_intermediaire, articulations_prime)) {
        return false;
    }
    
    // Deuxième itération : de la racine vers l'effecteur
    if (!iteration_racine_vers_effecteur(articulations,  
          articulations_prime, longueurs, articulations_prime_prime)) {
        return false;
    }
    
    if (!verifier_articulations_dans_arene(articulations_prime_prime)) {
        return false;
    }
    
    chaine.clear();
    for (const auto& articulation : articulations_prime_prime) {
        chaine.ajouter_articulation(articulation);
    }
    
    return true;
}

// fonction globale qui utilise l'unique instance globale du jeu
Jeu& get_jeu_instance() {
	return jeu_instance;
}

bool sauter_ligne(std::string ligne){
	if ((ligne[0]=='#') or ((ligne[0] == '/') and (ligne[1] == '/'))){
		return true;
	}
	return false;
}

std::string point_check(double i){
	std::string out;
	out = std::to_string(i);
	out.erase(out.find_last_not_of('0') + 1);
	return out;
}

bool Jeu::step() {
	if (etat_jeu == WON) {
		return false;
	} else if (score <= 0) {
		etat_jeu = LOST;
		return false;
	}
	score -= 1;
	size_t i(0);
	while (i < particules.size()){
		particules[i].temps();
		if (particules[i].get_c() == time_to_split){
			if (particules.size() >= nb_particule_max) {
				particules.erase(particules.begin() + i);
				continue;
			} else {
				particules.insert(particules.begin(), Particule(particules[i]));
				particules[0].ajust_angle(delta_split);
				particules[0].incremente();
				particules[0].reset_time();
				i += 1;
				particules[i].ajust_angle(-delta_split);
				particules[i].reset_time();
			}
		}
		particules[i].incremente();
		i += 1;
	}
	for (size_t j(0); j < faiseurs.size(); j++) {
		if (can_increment(faiseurs, j)) faiseurs[j].incremente();
	}
	verifier_collisions_faiseurs();
	if ((mode==GUIDAGE) and (!chaine.get_chaine().empty())){
		tools::S2d but_intermediaire = calculer_but_intermediaire();
		algo_guidage(but_intermediaire);
		verifier_collisions_faiseurs();
	}
	verifier_victoire();
	if (etat_jeu!=ONGOING) return false;
	return true;
}



bool can_increment(std::vector<Faiseur> faiseurs, size_t j) {
	faiseurs[j].incremente();
	for (size_t i(0); i < faiseurs.size(); i++) {
		if (i == j) {continue;}
		if (!(faiseurs[j].verifier_collision_faiseurs(faiseurs[i]))){
			return false;
		}
	}
	return true;
}

void Jeu::clear(){
	score = 0;
	particules.clear();
	faiseurs.clear();
	chaine.clear();
}

bool Jeu::a_but(){
	if (but_final.x == 0) {
		return false;
	}
	return true;
}

