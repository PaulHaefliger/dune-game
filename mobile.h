/*!
  \file   mobile.h
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Interface du module "mobile".
*/

#ifndef _MOBILE_H_
#define _MOBILE_H_
#include "tools.h"
#include "constantes.h"
#include <vector>
#include <iostream>

class Entity {
protected:
	tools::S2d position;
	tools::Polaire direction;
public:
	Entity(tools::S2d pos, tools::Polaire dir);
	virtual bool verifier_dans_arene(double r_max) const;
	bool verifier_d_max(double d_max) const;
	tools::Polaire get_d() const;
	tools::S2d get_p() const;
	virtual void incremente();
};
	
class Faiseur: public Entity {
private:
	double rayon = 0;
	unsigned nbe = 0;
	std::vector<tools::S2d> corps {};
	unsigned element1 = 0;
	unsigned element2 = 0;
public:
	bool verifier_dans_arene(double r_max) const override;
	Faiseur(tools::S2d pos, double r, tools::Polaire dir,
			unsigned nbetemp);
	bool intersection_faiseur(tools::S2d point);
	bool verifier_rayon(double r_min_faiseur,
						double r_max_faiseur) const;
	bool verifier_nbe() const;	
	bool verifier_collision_faiseurs(Faiseur faiseur);
	void init_corps();
	//should remove/move to entity
	void set_direction(tools::Polaire deplacement);
	void set_elements(unsigned a, unsigned b);
	unsigned get_1() const;
	unsigned get_2() const;
	unsigned get_nbe() const;
	double get_r() const;
	std::vector<tools::S2d> get_corps() const;
	void incremente();
};
	
class Particule: public Entity{
private:
	unsigned time = 0;
public:
	Particule(tools::S2d pos, tools::Polaire dir, unsigned times);

	bool verifier_c(unsigned time) const;
	unsigned get_c() const;
	void temps();
	void ajust_angle(double a);
	void reset_time();
};

#endif // _MOBILE_H_
