// for algolab, by taubnert@ethz.ch
#include <climits>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
//#include <CGAL/Gmpz.h>
//typedef CGAL::Gmpz ET;

#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;

using namespace std;

typedef long long dist;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution <ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef Delaunay::Point Point;
typedef Delaunay::Vertex_handle Vhandle;

dist distance(dist x1, dist y1, dist x2, dist y2) {
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

void testcase() {
	unsigned a, s, b, e;
	cin >> a >> s >> b;
	cin >> e;

	vector <dist> p_x(a), p_y(a), rho(a);
	for (unsigned i = 0; i < a; i++) {
		cin >> p_x[i] >> p_y[i] >> rho[i];
	}
	vector <dist> s_x(s), s_y(s);
	for (unsigned i = 0; i < s; i++) {
		cin >> s_x[i] >> s_y[i];
	}

	vector <Point> b_p(b);
	for (unsigned i = 0; i < b; i++) {
		cin >> b_p[i];
	}

	Delaunay triangulation;
	triangulation.insert(b_p.begin(), b_p.end());

	vector <dist> min_dist2(s);
	vector <vector<unsigned>> p_affected(a);
	// Find affectable for every shooting position
	for (unsigned i = 0; i < s; i++) {
		// Find nearest bounty hunter
		dist dist2 = LLONG_MAX;
		if (b != 0) {
			Point p = Point(s_x[i], s_y[i]);
			Vhandle closest = triangulation.nearest_vertex(p);
			dist2 = CGAL::squared_distance(closest->point(), p);
		}
		for (unsigned j = 0; j < a; j++) {
			dist d = distance(p_x[j], p_y[j], s_x[i], s_y[i]);
			if (d < dist2) {
				p_affected[j].push_back(i);
			}
		}
	}

	Program lp(CGAL::LARGER, true, 0, false, 0);
	for (unsigned i = 0; i < a; i++) {
		for (auto it = p_affected[i].begin(); it != p_affected[i].end(); ++it) {
			unsigned j = *it;
			dist d = distance(p_x[i], p_y[i], s_x[j], s_y[j]);
			if (d <= 1) {
				d = 1;
			}
			double d_inv = 1.0 / ((double) d);
			lp.set_a(j, i, d_inv);
		}
		lp.set_b(i, rho[i]);
	}

	for (unsigned i = 0; i < s; i++) {
		lp.set_c(i, 1);
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());

	if (!sol.is_infeasible() && to_double(sol.objective_value()) <= e) {
		cout << "y" << endl;
	} else {
		cout << "n" << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}

