#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <cmath>

#include "Common.h"

using namespace std;

class Diagram4D{
public:
    Diagram4D();
    
    void add(oint128 level, oint128 z, oint128 y);
    void add(ti coord);
    ti add_rich();
    ti add_pp_rand(double* prob, double* sum); //this параметры не передавать
    ti add_pp_max(double* prob, double* sum);

    void clear();
    void init_ocorners_from_rows();
    void init_by_rows(vvvi r);
    void init_cols_from_rows();
    void init_bars_from_cols();
    void init_dens_from_bars();
    
    int get_hook(int l, int z, int y, int x);
    int get_oX(int l, int z, int y);
    int get_oY(int l, int z, int x);
    int get_oZ(int l, int x, int y);
    int get_oD(int x, int y, int z);
    int get_maxXYZ();
    vvvi getCols();
    mtid getOcorners();
    vvvi getRows();

    double calc_weight(int l, int z, int y, int x);
    double calc_pp_sum();
    vector< set<ti> > calc_outer_cubes();  
    bool checking_outer_cube(oint128 level, oint128 z, oint128 y, oint128 x);
    void calc_ocorners(oint128 level, oint128 z, oint128 y);
    double calc_pp_prob(int l, int z, int y, int x);
    double calc_pp_probs();
    double cube_num(vvvi arr, int l, int coord, int flag);
 
    void print(ostream &output = cout);
    void print_cols(ostream &output = cout); //
    void print_bars(ostream &output = cout);    
    void print_dens(ostream &output = cout);
    void print_ocorners(ostream &output = cout);
    void print_all(ostream &output = cout);
    void tab2img(string fname, int num, string pngname);
    void print_imgs(const string file_name, int L = 0);
    void print_outer_cubes(ostream &output = cout);
    void build_tab(int flag, string fname, oint128 size);
    
    bool operator <(const Diagram4D &rhs ) const
    {
           return ( rows < rhs.rows );
    }
    
private:
    vector<oint128> short_vec;
    vector<vector<oint128>> long_vec;
    vvvi rows;
    vvvi cols;
    vvvi bars;
    vvvi dens;
    mtid ocorners;
    int diam;
};

