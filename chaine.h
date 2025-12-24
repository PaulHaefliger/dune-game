/*!
  \file   chaine.h
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Interface du module "chaine".
*/

#ifndef _CHAINE_H_
#define _CHAINE_H_
#include "tools.h"
#include <vector>
#include <cmath>

class Chaine{
private:
	std::vector<tools::S2d> chaine;
public:
	bool verifier_dans_arene(double r_max) const;
	bool verifier_longueur(double r_capture) const;
	tools::S2d get_derniere_articulation() const;
	void ajouter_articulation(tools::S2d point);
	void enlever_articulation();
	bool verifier_racine(double r_capture, double r_max) const;
	tools::S2d get_articulation(unsigned i) const;
	std::vector<tools::S2d> get_chaine() const;
	void clear();
};



#endif // _CHAINE_H_
