#pragma once
#include "Diagram.h"
#include "Diagram3D.h"
#include "StrictDiagram.h"
#include "Enumerator.h"

using namespace std;

class Process
{
	private:
		static double alpha;

		static double fi(uint64 x, uint64 y, double a);
		static double fi3D(uint64 x, uint64 y, uint64 z, double a);

	public:
		static uint64 add_rand(Diagram* diag);
		static uint64 add_rand(Diagram* diag, double& prob);
		static uint64 add_randrand(Diagram* diag);
		static uint64 add_greedy_plansh(Diagram* diag);

		static void strict_add_rand(StrictDiagram* diag);
		static void strict_probgreedy_plansh(uint64 num, StrictDiagram* diag, const char* fname = NULL);
		static double strict_probgreedy_plansh(StrictDiagram* diag);
		static void strict_plansh(uint64 num, StrictDiagram* diag, const char* fname = NULL, vvd* ratios = NULL);
		static uint64 strict_plansh(StrictDiagram* diag, double* prob = NULL);
		static void strict_rich(uint64 num, StrictDiagram* diag);
		static double strict_rich_to_target(StrictDiagram diag, StrictDiagram target);
		static int strict_add_maxdim(uint64 num, StrictDiagram* diag);

		static pu add_rand3D(Diagram3D* diag, double* prob = NULL, const double alpha = 1);
		static pu add_greedy_planch3D(Diagram3D* diag);
		static uint64 add_rand_gen(Diagram* diag, double a = alpha);
		static pu add_rand_gen3D(Diagram3D* diag, double a = alpha);
		static uint64 add_rand_plansh(Diagram* diag, double* prob = NULL);
		static pu add_rand_plansh3D(Diagram3D* diag, double* prob = NULL, bool calc_probs = true, const double alpha = 1);
		static pu add_randrand3D(Diagram3D* diag);

		//static void greedy_plansh(uint64 num, Diagram* diag);
		static pu add_lexicogr3D(Diagram3D* diag, double a, double b, Diagram3D* target = NULL, bool ADD = true);
		static void rich(uint64 num);     //Richardson process
		static void rich(uint64 num, Diagram* diag);
		static void rich3D(uint64 num, Diagram3D* diag = NULL);
		static void gen_rich(uint64 num); //Generalized Richardson process
		static void gen_rich(uint64 num, Diagram* diag, double a = alpha);
		static void gen_rich3D(uint64 num, Diagram3D* diag);
		static void plansh(uint64 num);   //Plansherel process
		static void plansh(uint64 num, Diagram* diag);
		static void plansh_on_strict(uint64 num, Diagram* diag);
		static void plansh_with_log(uint64 num, Diagram* diag);
		//static void plansh_greedy(uint64 num, Diagram* diag, ostream &output = cerr);
		static void plansh_greedy(uint64 num, Diagram* diag, FILE* f = NULL, bool tmpbool = false);
		static void plansh_greedy_with_log(uint64 num, Diagram* diag, ostream &output);
		static void plansh3D(uint64 num, Diagram3D* diag = NULL);
		static void rand_to_target(Diagram* diag, Diagram* target);
		static uint64 rand_to_target_onestep(Diagram* diag, Diagram* target);
		static double conseq_to_target(di* target_rows);
		static void planch2D_to_target(Diagram& diag, Diagram& diag2, Diagram& target, double& prob);
		static uint64 planch2D_to_target_incorrectprobs(Diagram& diag, Diagram& target);
		static void planch3D_to_target(Diagram3D& diag, Diagram3D& diag2, Diagram3D& target, double& prob, double ALPHA = 1);
};
