#ifndef _TOOLS_H_
#define _TOOLS_H_
/*!
  \file   tools.h
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Interface du module "tools".
*/

#include <cmath>
#include <iostream>

const double epsil_zero(0.5);

namespace tools
{
	struct S2d {
		double x = 0.;
		double y = 0.; 	
	
		inline constexpr S2d() noexcept : x(0.0), y(0.0) {}

		constexpr S2d(const S2d&) noexcept = default;
		constexpr S2d(S2d&&) noexcept = default;
		constexpr S2d& operator=(const S2d&) noexcept = default;
		constexpr S2d& operator=(S2d&&) noexcept = default;

		S2d(double x_val, double y_val);
	};
	
	struct Polaire {
		double a = 0.;
		double d = 0.;
		
		Polaire(double a_val, double d_val);
	};
	
	class Cercle {
	private:
		S2d position;
		double r = 0.;
	public:
		void get_r(double& rtemp) const;
		void get_p(S2d& ptemp) const;
		S2d get_pos() const;
		Cercle(S2d pos, double ra) : position(pos), r(ra) {}
		Cercle(double x, double y, double ra) : 
			position(x, y), r(ra) {}
		bool point_dans_cercle(S2d p) const;
		bool contient_cercle(Cercle c) const;
		bool cercle_intersecte(Cercle c) const;
	};

	Polaire passage_cartesienne_polaire(S2d p);
	S2d passage_polaire_cartesienne(Polaire p);
	double calcule_distance(S2d p1, S2d p2);
	double calculer_angle_reflechi(double angle_incident,
								   double angle_normal);
	double normaliser_angle(double angle);
	Polaire calculer_vecteur_reflechi(const Polaire incident,
										Polaire normal);
	double calcule_norme(S2d vector);
	double calculer_angle(S2d point);
	S2d calculer_point_reflechi(S2d position,
								Polaire direction);
	void ajouter_deplacement(S2d& c, Polaire p);
	bool will_be_outside(S2d pos, Polaire dir, double r_max);
}


#endif // _TOOLS_H_










