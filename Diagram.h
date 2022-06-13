#pragma once

#include <deque>
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <string.h>
#include <climits>
#include <gmpxx.h>

#include "Common.h"

using namespace std;

class Diagram
{
public:
	struct cfg
	{
		string imgext = "png";
		string fname_img = "test." + imgext;
		char* fname_gnuplot = (char*)"tmp_gnuplot.plt";
		char* fname_coords = (char*)"tmp_coords.txt";
		char* fname_planchlimitshape = (char*)"tmp_planchlimit.txt";

		bool verbose = false;
		int type = 1;	  //0 - noboxes, 1 - boxes, 2 - skewed, 3 - points (Ferrier), 4 - ONLY RSK paths
		pu size = pu(0, 0);
		int notation = 0; //0 - French, 1 - English, 2 - Russian
		mpp* tab = NULL;
		mpp* oldtab = NULL; // In order to mark old RSK paths in different colour
		mppd* tabD = NULL; //tableau with doubles

		vector<string> colors = {"000000", "0000ff", "ffffff", "000000", "ff0000", "00ff00"};
		bool tab_colors = false; //true - use colors to depict tableau
		bool print_labels = true;
		//bool print_schutzenberger_paths_only = false;
		bool print_planch_limitshape = false;
		bool print_rich_limitshape = false;
		bool show_RSK_paths = false;

		double rectLW = 3; //Line width (rectangles)
		double planchLW = 3; //Line width (Plancherel curve)
		double RSKLW = 3; //Line width (RSK paths)
		bool RSKcol = false; //Use different colours for RSK bumping trees
		string bground = "ffffff";//"#dddddd";//"#e6efe6";
		
		bool add_fake_point = true; //add point at coordinate (SIZE, SIZE) (so, the image sizes after cropping are the same).

		int tab_label_size = 10; //size of numbers inside boxes

		int min_color = 255;
		int max_color = 0;
	};


        bool operator <( const Diagram &rhs ) const
        {
           return ( rows < rhs.rows );
        }
private:
	static mdd planch_limit;

	void calc_planch_limit();
	mi get_planch_limit();

	void bypass_tree(map<pu,vpu>& sources, pu& coord, const int color, const double lw, string& res);

protected:
	di rows, cols;
	mi corners;
	mi points;

	uint64 n; //Diagram size
	double norm_dim;
	double max_pl_prob;
	uint64 max_pl_line;
	uint64 last_added;

public:
	Diagram();
	Diagram(Diagram* copy_from);

	double calc_point_plansh_prob(uint64 line);
	mpq_class calc_exact_point_planch_prob(uint64 y);
	void calc_point_plansh_prob_rat(uint64 line, int* num, int* den);
	double calc_corner_plansh_prob(uint64 line);
	uint64 calc_plansh_probs(uint64 num = 0, mi* best_p = NULL);

	double calc_alpha(int x, int y);

	void calc_max_prob(uint64* line, double* prob);

	void set_to_last(uint64 n_, uint64 k_);

	int add(uint64 line, bool calculate_alpha = false);
	int add_with_probsum(uint64 line, bool calculate_alpha = false);
	int add_max(); //greedy
	int add_max_with_probsum();
	void add_max_blowup();
	int add_nth(uint64 number); //adds the nth best point
	int remove(uint64 line, double prob = -1);
	int remove_min();

	mi* get_points();
	mi* get_corners();

	void calc_n_min_corners(uint64 num, mi* c);

	uint64 get_height();
	uint64 get_height(uint64 line);
	uint64 get_width();
	uint64 get_width(uint64 line);

	uint64 get_size();
	//uint64 get_bottom();
	double get_corners_div_diameter();
	di* get_rows();
	di* get_cols();
	void get_rows(di r);
	uint64 get_last_added();

	void calc_all_tabs(set<dpu>* tabs, dpu tab = dpu(0), Diagram diag = Diagram());
	void calc_size_from_rows();

	double comp_dim(Diagram* diag);
	int comp_dim_exact(Diagram* diag);
	int comp_dim_slow(Diagram* diag);

	uint64 get_hook(uint64 x, uint64 y);
	uint64 get_hook_slow(uint64 x, uint64 y);
	void pop_bottom();
	bool is_flat();
	bool is_strict();
	void pop(uint64 line);
	void set_rows(di r);
	void init();
	void init(Diagram* copy_from);
	void init_by_rows(di r);
	void init_by_cols(di c);
	void init_by_cols(vi c);
	void init_by_tab(dpu& tab, const uint64 LEN = 0);
	int rows_correctness();
	int rowcol_correctness();
	void add_prob_sum(double psum);
	void get_legal_points(Diagram* target, mi* leg_p);
	void remove_illegal_points(Diagram& target);
	void remove_row0_point();
	void add_row0_point();
	bool sum_is_greater_than(Diagram* t);
	//double get_prob_sum();
	bool is_equal_to(Diagram* comp);
	bool is_equal(Diagram comp);
	bool is_equal_to_cols(di c);
	void calc_cols_from_rows();
	void calc_rows_from_cols();
	void calc_pointscorners_from_rows();
	double hook_prod_ratio(Diagram* diag2);
	bool is_symmetric();
	bool is_symmetric_to(Diagram* diag2);
	bool consists_of(uint64 x, uint64 y);

	//double get_normdim();
	double get_c();
	double calc_c_slow();
	//double get_c_bug();
	//double get_c_bad();
	void calc_exact_dim(mpz_t dim);
	//void get_last_dim(mpz_t dim);
	double calc_planch_stadev();
	double calc_planch_maxdiff();
	//uint64 calc_rowcol_diff();

	string calc_RSK_paths_for_gnuplot(const double LW, mppd& tab, mppd* oldtab = NULL);
	string calc_coloured_RSK_paths_for_gnuplot(const double LW, mppd& tab);
	unsigned long int getDim();
	void print();
	void print_cols(ostream &output = cout);
	void print_rows_ASCII();
	void print_rowsdim();
	void print_rowcols();
	void print_verbose();
	void print_to_file(char* fname, streambuf* osbuf = nullptr);
	void print_rows_to_file(FILE* f);
	void print_points(const char* FNAME = NULL);
	void print_points_cute();
	void print_corners(ostream &output = cout);
	void print_corners_with_braces(ostream &output = cout);
	void print_corners(FILE* f);
	void print_corners_for_gnuplot(uint64 exper_num, uint64 diag_num, uint64 depth, bool print_to_cout = false);
	void print_corners_for_gnuplot_all_boxes(const char* data_fname);
	void print_corners_for_gnuplot_skewed(const char* data_fname);
	void print_corners_for_gnuplot_contour_only(const char* data_fname);
	void print_to_gif(bool contour, const char* gifname, uint64 maxX = 0, uint64 maxY = 0, const char* added="", const char* removed="");
	void print_to_eps(const char* epsname);
	void print_to_png(const char* FNAME_PNG, bool verbose = false, const int TYPE = 0, const pu SIZE = pu(0,0), const int NOTATION = 0, mpp* TAB = NULL);
	void print_to_png(cfg& CFG);
	void print_hooks_to_png(const char* FNAME_PNG);
	void print_to_png_OLD(const char* FNAME_PNG, bool verbose = false, const int TYPE = 0, const uint64 SIZE = 0);
	void print_corners_cute(ostream &output = cout);
	//void print_max_pl(ostream &output = cerr);
	void print_max_pl(FILE* f = NULL);
	void print_to_txt(const char* fname);
	void print_exact_dim();
	void print_rat_pprobs();

    void add_blow_up();
    void remove_blow_up();
    void add_with_check();
    double count_al(uint64 x, uint64 y);
    double new_norm_dim(Diagram* diag);
    double norm_dim_hook();
    void add_with_table(vector<vector<double>>&table,int par);
    void remove_with_table(vector<vector<double>>&table,int par);
    void calc_rows_from_cols(di d_cols);
    void calc_cols_from_rows(di d_rows);
    void calc_cols_from_rows(Diagram diag);
	//void branches_alg();
	void random_diags(int num_br, int num_ch, int depth, vector <di> *bank);
	di random_change(di* cols, int num_ch);
};
