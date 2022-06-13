#pragma once

#include "Diagram.h"
#include "Common.h"
#include <iterator>
#include <list>
#include <queue>
#include <omp.h>
#include <unordered_map>
#include <gmpxx.h>

using namespace std;

class Diagram3D
{
	public:
		struct cfg
		{
			string fname_img = "test.png";
			char* fname_coords = (char*)"tmp_coords.txt";
			char* fname_plt = (char*)"tmp_gnuplot.plt";
			char* fname_arrows = (char*)"tmp_arrows.txt";
			char* fname_balls = (char*)"tmp_balls.txt";
			char* fname_labels = (char*)"tmp_labels.txt";

			Diagram3D* diag2 = NULL;
			bool verbose = true;
			bool corn_only = false;
			bool add_fake_point = true;
			double scale = 1;//1.3;
			double view1 = 54.73;
			double view2 = 135;
			uint64 min_scale = 15;
			bool new_color = false;
			pu new_cube = pu(0, 0);
			int new_cube_color = 2;	
			uint64 max_XYZ = 0;
			string type = "png";
			map<uint64,mpp>* tab = NULL;

			bool show_cubes = true;
			bool show_arrows = false;
			bool nohead = true;
			bool show_balls = false;
			bool show_size = false;
			bool show_cornnum = false;
			bool show_clustnum = false;
			bool show_axes = false;
			bool show_labels = false;
			double lw = 1; //line width
			char* bgcol = (char*)"#ffffff";//"#e6efe6"
			vector<string> colors = {"#00ffff", "#ff0000", "#ffffff", "#6500ff", "#ffff00", "#ce4c7d", "#999999"};
/*
			char* col0 = (char*)"#00ffff";
			char* col1 = (char*)"#ff0000";
			char* col2 = (char*)"#ffffff";//"#006e0c"; //"#00a732";//"#ce4c7d"; "#166301"
			char* col3 = (char*)"#6500ff";
			char* col4 = (char*)"#ffff00";
			char* col5 = (char*)"#ce4c7d";
			char* col6 = (char*)"#999999";
*/
			bool crop = true;


			//print_outer_cubes_verbose:
			bool skew = false;

			bool show_corners = false;
			bool show_points = false;
			bool show_planch_probs = false;
			bool show_max_prob = false;
			bool show_dist = false; //indicates x+y+z by color
			bool show_crossection = false; //indicates crossection by color
			bool show_layers = false;
			bool show_strict_parts = false;

			double min_color = 0;
			double max_color = 1;

			double small_size = 0.35;
			double bigger_size = 0.7;




		};
		
	bool operator <(const Diagram3D &rhs ) const
        {
           return ( rows < rhs.rows );
        }
        
	private:
		vvi rows, cols, bars;
		mpu points, corners;
		uint64 n; //diagram size
		uint64 num_of_levels;
		map<tu, int> outer_cubes;
		bool STANDARD;
		double planch_sum;
//M.B. mozhno izbavitsya ot skew_xy,... (speed+):
		//mpp skew_xy, skew_zy, skew_zx;
		//unordered_map<uint64, unordered_map<uint64, pu> > skew_xy, skew_zy, skew_zx;

		//outer coordinates of skewed intervals (for hooks calculation!)
		vvi skew_xy, skew_zy, skew_zx;

		double strict_calc_planch_weight(uint64 z, uint64 y);
		bool are_neighbours(pu cube1, pu cube2);

		void get_central_points(map<tu, int> &cubes, tu &p1, tu &p2, tu &p3);
		static void print_dims(map<vvi, mpz_class> dims, ofstream& f);
	public:
		Diagram3D(bool standard = true);
		int operator==(const Diagram3D& d) const;
		void init(bool standard = true);
		void init(Diagram3D* copy_from);
		void init_by_cols(vvi c);
		void init_by_rows(vvi r);
		void init_by_bars(vvi b);
		void calc_size_from_cols();
		void calc_rows_from_cols();
		void calc_cols_from_rows();
		void calc_bars_from_cols();
		void calc_cols_from_bars();
		void calc_pointscorners();

		void print_all();
		void print_rows(ostream &output = cout, bool print_endl = true);
		void print_cols(ostream &output = cout, bool print_endl = true);
		void print_cols_manylines(ostream &output = cout, bool print_endl = true);
		void print_bars(ostream &output = cout);
		void print_points(ostream &output = cout);
		void print_corners(ostream &output = cout);

		void print_corners(const char* fname);
		void print_outer_cubes();
		void print_outer_cubes(const char* fname);
		void print_outer_cubes(fstream& f, double DX = 0, double DY = 0, double DZ = 0, const bool SHOW_STRICT_DIAGS = false);
		void print_outer_cubes_verbose(cfg& CFG);
		void print_outer_cubes_verbose(const char* fname);
		void print_outer_cubes_and_new(const char* fname, pu new_cube, int new_color);
		void print_tab(cfg& CFG);


		void print_to_img(const char* fname_png, bool new_color = false, pu new_cube = pu(0, 0), uint64 MAX_XYZ = 0);
		void print_to_img(cfg& CFG);

		void print_to_img2D(const char* fname_png);
		void print_verbose_to_img(const char* fname_png, uint64 maxX = 0, uint64 maxY = 0, uint64 maxZ = 0, double scale = 1);
		static void print_coords_to_img(const char* fname_coords, const char* fname_png, double max = 0, double scale = 1, Diagram3D* diag = NULL);
		static void print_coords_to_img(cfg& CFG);

		static map<uint64, map<vvi, uint64> > calc_all_dimensions_UINT64(bool STANDARD, const uint64 MAX_LEVEL);
		static void print_all_and_max_dims(const bool STANDARD, const uint64 MAX_LEVEL);
		//static map<uint64, map<vvi, mpz_t> > calc_all_dimensions(bool STANDARD, const uint64 MAX_LEVEL);
		//static map<uint64, map<vvi, mpz_class> > calc_all_dimensions2(bool STANDARD, const uint64 MAX_LEVEL);
		unsigned long int getDim();
		void calc_exact_dim(mpz_t dim);
		void print_exact_dim();
		static void print_all_dimensions(bool STANDARD, const uint64 MAX_LEVEL);

		double calc_planch_sum(double alpha = 1);
		double calc_planch_prob(uint64 z, uint64 y, double alpha = 1);
		double calc_planch_prob(pu coord, double alpha = 1);
		void calc_planch_probs(pu* max_line = NULL, double* max_prob = NULL, double alpha = 1);
		void calc_planch_probs(double alpha);
		double calc_planch_weight(uint64 z, uint64 y);

		void rec_improve_probs(const int ITERS, pu& max_coord, double& max_prob, const double alpha);

		void improve_probs(pu& max_coord, double& max_prob, const double alpha = 1);
		void improve_probs2(pu& max_coord, double& max_prob, const double alpha = 1);
		void improve_probs3(pu& max_coord, double& max_prob, const double alpha = 1);
		void improve_probs4(pu& max_coord, double& max_prob, const double alpha = 1);
		void improve_probs5(pu& max_coord, double& max_prob, const double alpha = 1);

		void improve_probs(const double alpha = 1);
		void improve_probs2(const double alpha = 1);
		void improve_probs3(const double alpha = 1);
		void improve_probs4(const double alpha = 1);
		void improve_probs5(const double alpha = 1);

		void fill_cluster(uint64 z, uint64 y, uint64 x);
		uint64 calc_clusters_number();
		uint64 calc_clusters_number(map<tu, int>& nearby);
		uint64 calc_clusters_number_slow();
		int calc_number_of_corners_nearby(uint64 z, uint64 y);

		int add(const uint64 z, const uint64 y);
		int add(const pu place);
		int add_old(const uint64 z, const uint64 y);
		int add_to_target(pu coord, vvi target_cols);

		int remove(const uint64 z, const uint64 y);
		int remove(const pu place);

		void add_point(uint64 z, uint64 y, vvi* target_cols = NULL, uint64* zadd = NULL, uint64* yadd = NULL);
		void add_point2(uint64 z, uint64 y);
		void add_corner(uint64 z, uint64 y);

		void remove_invalid_point(pu coord, vvi& target_rows);
		void remove_invalid_points(vvi& target_rows);
		void rescale_probs();

		uint64 get_size();
		mpu* get_points();
		mpu* get_corners();
		vvi* get_cols();
		vvi* get_rows();
		vvi* get_bars();
		double get_planch_sum();
		//int add_new_level();
		void get_point_coords(mpu::iterator iter, uint64& x, uint64& y, uint64& z);
		uint64 get_number_of_large_probs(const double THLD = 0.95);
		int check_correctness();
		uint64 get_hook(uint64 x, uint64 y, uint64 z);
		uint64 get_skewed_hook(uint64 x, uint64 y, uint64 z);
		int calc_hooks_min_GCD();
		void get_legal_points(Diagram3D* target, mpu* leg_p);
		bool consists_of(uint64 x, uint64 y, uint64 z);
		bool is_corner(uint64 x, uint64 y, uint64 z);
		bool is_contour(uint64 x, uint64 y, uint64 z);
		bool is_equal_to(Diagram3D* comp);
		bool is_equal(Diagram3D comp);
		bool is_symmetric();
		bool is_symmetric_to(Diagram3D& diag);
		bool is_triangle();
		bool has_strict_structure();
		bool is_strict();
		bool get_standard_flag();
		bool contains(Diagram3D& inner);
		bool has_xyz(uint64 x, uint64 y, uint64 z);

		uint64 getX();
		uint64 getX(uint64 z);
		uint64 getX(uint64 z, uint64 y);
		uint64 getX(pu coord);
		uint64 getpointX(uint64 z, uint64 y);
		uint64 getpointY(uint64 z, uint64 x);
		uint64 getpointZ(uint64 x, uint64 y); 
		uint64 getY();
		uint64 getY(uint64 z);
		uint64 getY(uint64 z, uint64 x);
		uint64 getZ();
		uint64 getZ(uint64 x, uint64 y);

		int get_diam(tu *coord);

		void get_outer_cubes(map<tu, int>& cubes);
		void calc_outer_cubes_slow(map<tu, int>& cubes);
		void calc_outers_points_corners(map<tu, int>& cubes);
		void calc_outer_cubes_skew(map<tu, int>& cubes);
		void calc_all_tabs(set<dtu>* tabs, dtu tab = dtu(0), Diagram3D diag = Diagram3D());

		void get_tip(Diagram3D* tip);

		void standard_from_strict(Diagram3D& stand);
};
