/*!
  \file   jeu.h
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Interface du module "jeu".
*/

#ifndef JEU_H
#define JEU_H

#include <vector>
#include <string>
#include "mobile.h"
#include "chaine.h"
#include "constantes.h"

class Jeu {
private:
	unsigned score;
	std::vector<Particule> particules;
	std::vector<Faiseur> faiseurs;
	Chaine chaine;
	Mode mode;
	static bool check_total;
	tools::S2d position_souris;
	tools::S2d region_capture;
	tools::S2d but_final;
	enum Etat {ONGOING, WON, LOST} etat_jeu;
	bool verifier_articulations_dans_arene(
				const std::vector<tools::S2d>& articulations) const;
    std::vector<double> calculer_longueurs(
				const std::vector<tools::S2d>& articulations) const;
    bool iteration_effecteur_vers_racine(
        const std::vector<tools::S2d>& articulations,
        const std::vector<double>& longueurs,
        const tools::S2d& point_intermediaire,
        std::vector<tools::S2d>& articulations_prime) const;
    bool iteration_racine_vers_effecteur(
        const std::vector<tools::S2d>& articulations,
        const std::vector<tools::S2d>& articulations_prime,
        const std::vector<double>& longueurs,
        std::vector<tools::S2d>& articulations_prime_prime) const;

public:
	//constructeur
	Jeu();
	//getters
	unsigned get_score() const;
	const std::vector<Particule>& get_particules() const;
	const std::vector<Faiseur>& get_faiseurs() const;
	const Chaine& get_chaine() const;
	Mode get_mode() const;
	bool check_score() const;
	bool step();
	bool get_check() const;
	void check(bool b);
	tools::S2d get_region_capture();
	tools::S2d get_but();
	void set_but(tools::S2d but);
	//operations sur les fichiers
	bool lecture(const std::string& nom_fichier);
	void sauvegarder(const std::string& nom_fichier);
	
	void set_mode(Mode nouveau_mode);
	Etat get_etat_jeu() const;
	bool capturer_particule();
	void clear();
	
	// gérer la souris
	void set_position_souris(const tools::S2d& pos);
	tools::S2d calculer_but_intermediaire() const;
	//vérification des conditions de fin de jeu
	bool verifier_victoire();
	bool verifier_collisions_faiseurs();
	
	//calculer les distances et les positions 
	tools::S2d calculer_but_final(const tools::S2d& racine) const;
	//pour acceder aux enumerations via switch dans gui.cc
	bool a_gagner() const {return (etat_jeu == WON);}
	bool a_perdu() const {return (etat_jeu ==LOST);}
	bool est_en_cours() const {return (etat_jeu==ONGOING); }
	bool algo_guidage(const tools::S2d& point_intermediaire);
	bool a_but();
	
};

bool sauter_ligne(std::string ligne);
//fonction pour acceder a lunique instance de jeu
Jeu& get_jeu_instance();

void lire_ligne(const std::string& fichier, std::string& ligne,
				unsigned& nb_ligne);
bool check_nb_particules(double nb_particules);
bool check_particule(Particule particule);
bool check_nb_faiseurs(unsigned nb_faiseurs);
bool check_faiseur(Faiseur faiseur, std::vector<Faiseur> faiseurs,
					unsigned b);
bool check_articulation(unsigned i, Chaine liste,
						std::vector<Faiseur> faiseurs, unsigned nb);
std::string point_check(double i);
void set_check(bool b);
bool can_increment(std::vector<Faiseur> faiseurs, size_t j);

#endif//JEU_H
