/*
 * StrictDiagram.h
 *
 *  Created on: Jun 13, 2015
 *      Author: vduzhin
 */
#ifndef STRICTDIAGRAM_H_
#define STRICTDIAGRAM_H_

#include "Common.h"
#include "Diagram.h"
#include <iterator>

using namespace std;

class StrictDiagram : public Diagram
{
	private:
		static mdd planch_limit;
		vd plansh_probs;
		mpz_t dim, num, den;
		mpq_t rat, rat_best;
		double nd, lndim_sum;
		bool gmp_initialized = false;
	public:
		StrictDiagram(bool use_gmp = false);
		StrictDiagram(StrictDiagram* copy_from, bool use_gmp = false);
		~StrictDiagram();

		double calc_dim_ratio(uint64 line, bool no_return = false);
		double calc_dim_ratio_fast(uint64 line);
		int add(uint64 line, double prob = -1, bool calculate_alpha = false);
		int add_with_probsum(uint64 line);
		int add_maxdim_mpq(uint64 num);
		int add_maxdim_double(uint64 num = 1);
		void add_max_blowup();
		void init(bool use_gmp = false);
		void init(StrictDiagram* copy_from, bool use_gmp = false);
		void init_by_cols(di c);
		void calc_pointscorners_from_cols();
		void calc_strict_dim(mpz_t dim);
		double calc_plansh_prob(uint64 line);
		void calc_plansh_prob_rat(uint64 line, int* num, int* den);
		double calc_plansh_prob_mpz(uint64 line);
		double calc_plansh_prob_inaccurate(uint64 line);
		double calc_route_plansh_prob();
		double calc_plansh_prob_maybe_coprob(uint64 line);
		double calc_alpha(int x, int y);
		mdd calc_planch_limit(const int NUM_OF_POINTS = 10000);

		double get_c();


		double comp_dim(StrictDiagram* diag);

		bool is_inside(StrictDiagram* bigger_diag);

		uint64 calc_plansh_probs(uint64 num = 0, mi* best_p = NULL);

		//bool is_strict();

		void calc_max_prob(uint64* line, double* prob);
		void calc_max_dimratio(uint64* maxdr_line, double* maxdimrat);

		vd* get_plansh_probs_trace();

		int remove_min();
		int remove_min_dim();
		void remove_min_mpq(uint64 depth);
		uint64 remove_min_double(uint64 depth);
		double calc_corner_plansh_prob(uint64 line);
		int remove(uint64 line, double prob = -1);
		void set_cols_and_size(di c, uint64 size);

		double calc_strict_normdim_slow();

		int max_difference_in_rows(StrictDiagram diag);
};

#endif /* STRICTDIAGRAM_H_ */
