#include "Diagram4D.h"

Diagram4D::Diagram4D(){
        ocorners[ti(0,0,0)];
        short_vec.push_back(1);
        long_vec.push_back(short_vec);
        diam = 0;
}

void Diagram4D::add(oint128 level, oint128 z, oint128 y){
        if(ocorners.find(make_tuple(level, z, y)) == ocorners.end()){
            cerr << "Out of range: " << level << ' ' << z << ' ' << y << endl;
            exit(1);
        }

	oint128 x = get_oX(level, z, y);

        if(x == y && y == z && z == level)
            diam+=2;

        if(rows.size() == level){                                                                                   //if new level is going to be added
                rows.push_back(long_vec);
                cols.push_back(long_vec);
                bars.push_back(long_vec);
                
                if(level != 0)
                    dens.at(x).at(y).at(z)++;
                else
                    dens.push_back(long_vec);
        }else{
            if(rows.size() > level){                                                                                                                    //if level has been alredy existed
                if(rows.at(level).size() == z){                                                                                                         //if new z is going to be added
                    if(y == 0 && (level == 0 || rows.at(level - 1).size() > z)){                                                                        //if y is right and it can be added
                        rows.at(level).push_back(short_vec);
			cols.at(level).push_back(short_vec);
                        bars.at(level).at(x).at(y)++;
                        if(dens.at(x).at(y).size() == z)
                            dens.at(x).at(y).push_back(1);
                        else
                            dens.at(x).at(y).at(z)++;
                    }
                }else if(rows.at(level).size() > z){                                                                                                    //if z has been alredy existed
                    if(rows.at(level).at(z).size() == y && (level == 0 || rows.at(level - 1).at(z).size() > y)){                                        //if new y is going to be added
                        rows.at(level).at(z).push_back(1);
			cols.at(level).at(z).at(x)++;

		        if(bars.at(level).at(x).size() == y)
                            bars.at(level).at(x).push_back(1);
			else
			    bars.at(level).at(x).at(y)++;

                        if(dens.size() == x)
                            dens.push_back(long_vec);
			else if(dens.at(x).size() == y)
			    dens.at(x).push_back(short_vec);
			else if(dens.at(x).at(y).size() == z)
			    dens.at(x).at(y).push_back(1);
                        else
                            dens.at(x).at(y).at(z)++;
                    }else if(rows.at(level).at(z).size() > y && (level == 0 || rows.at(level - 1).at(z).at(y) > rows.at(level).at(z).at(y))){
                        rows.at(level).at(z).at(y) += 1;

			if(cols.at(level).at(z).size() == x)
                            cols.at(level).at(z).push_back(1);
			else
			    cols.at(level).at(z).at(x)++;

                        if(bars.at(level).size() == x)
                            bars.at(level).push_back(short_vec);
			else if(bars.at(level).at(x).size() == y)
			    bars.at(level).at(x).push_back(1);
                        else
                            bars.at(level).at(x).at(y)++;

                        if(dens.size() == x)
                            dens.push_back(long_vec);
			else if(dens.at(x).size() == y)
			    dens.at(x).push_back(short_vec);
			else if(dens.at(x).at(y).size() == z)
			    dens.at(x).at(y).push_back(1);
			else
			    dens.at(x).at(y).at(z)++;            
                    }
                }
            }
        }
        calc_ocorners(level, z, y);
}
    
void Diagram4D::add(ti coord){
        add(get<0>(coord), get<1>(coord), get<2>(coord));
}
    
void Diagram4D::init_ocorners_from_rows(){
        ti oc;
        int len;
        int l,z,y;
        
        ocorners.clear();
        
        for(l = 0; l < rows.size(); l++){
            for(z = 0; z < rows.at(l).size(); z++){
                
                len = rows.at(l).at(z).at(0);
                if((z == 0 || rows.at(l).at(z - 1).at(0) > rows.at(l).at(z).at(0)) && (l == 0 || rows.at(l - 1).at(z).at(0) > len))
                 ocorners[make_tuple(l, z, 0)];
                for(y = 1; y < rows.at(l).at(z).size(); y++){
                    if(len > rows.at(l).at(z).at(y)){
                        len = rows.at(l).at(z).at(y);
                        
                        if(z == 0 || rows.at(l).at(z - 1).at(y) > len){
                            if(l == 0 || rows.at(l - 1).at(z).at(y) > len){
                                ocorners[make_tuple(l, z, y)];
                            }
                        }
                     }
                 }
                 if((z == 0 || cols.at(l).at(z - 1).at(0) > cols.at(l).at(z).at(0)) && (l == 0 || cols.at(l - 1).at(z).at(0) > cols.at(l).at(z).at(0)))
                    ocorners[make_tuple(l, z, y)];
             }
            if(l == 0 || bars.at(l - 1).at(0).at(0) > bars.at(l).at(0).at(0)){
              ocorners[make_tuple(l, z, 0)];
            }
        }
        ocorners[make_tuple(l, 0, 0)];
}
    
void Diagram4D::init_by_rows(vvvi r){
        rows = r;
        init_cols_from_rows();
        init_bars_from_cols();
        init_dens_from_bars();
        init_ocorners_from_rows();
}
    
void Diagram4D::init_cols_from_rows(){
        int l,z,y,x;
        
        cols.clear();
        cols.resize(rows.size());
        
        for(l = 0; l < rows.size(); l++){
            cols.at(l).resize(rows.at(l).size());
            for(z = 0; z < rows.at(l).size(); z++){
                cols.at(l).at(z).resize(rows.at(l).at(z).at(0), 1);
                for(y = 1; y < rows.at(l).at(z).size(); y++){
                    for(x = 0; x < rows.at(l).at(z).at(y); x++){
                        cols.at(l).at(z).at(x)++;
                    }
                }
            }
        }
}
    
void Diagram4D::init_bars_from_cols(){
        int l,z,y,x;
        
        bars.clear();
        bars.resize(cols.size());
        
        for(l = 0; l < cols.size(); l++){
            bars.at(l).resize(cols.at(l).at(0).size());
            
            for(x = 0; x < cols.at(l).at(0).size(); x++){
                bars.at(l).at(x).resize(cols.at(l).at(0).at(x),1);
            }
            
            for(z = 1; z < cols.at(l).size(); z++){
                for(x = 0; x < cols.at(l).at(z).size(); x++){
                    for(y = 0; y < cols.at(l).at(z).at(x); y++){
                        bars.at(l).at(x).at(y)++;
                    }
                }
            }
        }
}
    
void Diagram4D::init_dens_from_bars(){
        int l,z,y,x;
        
        dens.clear();
        dens.resize(bars.at(0).size());
        
        for(x = 0; x < bars.at(0).size(); x++){
            dens.at(x).resize(bars.at(0).at(x).size());
            for(y = 0; y < bars.at(0).at(x).size(); y++){
                dens.at(x).at(y).resize(bars.at(0).at(x).at(y), 1);
                for(z = 0; z < bars.at(0).at(x).at(y); z++){
                    for(l = 1; l < bars.size(); l++){
                        if(x >= bars.at(l).size() || y >= bars.at(l).at(x).size())
                            break;
                        if(z < bars.at(l).at(x).at(y)){
                            dens.at(x).at(y).at(z)++;
                        }
                    }
                }
            }
        }
}
   
ti Diagram4D::add_pp_rand(double* prob, double* sum){
    mtid::iterator it;
    double rnd, prob_sum;

    *sum = calc_pp_probs();
    rnd = rand_0_1();
    for(prob_sum = 0, it = ocorners.begin(); (prob_sum <= rnd) && (it != ocorners.end()); ++it){
	prob_sum += it->second;
    }
    --it;
    if(prob != NULL)
	*prob = it->second;
    ti n = it->first;
    add(n);
    return n;
}
 
ti Diagram4D::add_pp_max(double* prob, double* sum){
    mtid::iterator it;
    ti max;

    *prob = 0;
    *sum = calc_pp_probs();
    for(it = ocorners.begin(); it != ocorners.end(); it++){
	if(*prob <= it->second){
	    *prob = it->second;
	    max = it->first;
	}
   }
   add(max);
   return max;
}

int Diagram4D::get_hook(int l, int z, int y, int x){
        return (rows.at(l).at(z).at(y) - x) + (cols.at(l).at(z).at(x) - y) + (bars.at(l).at(x).at(y) - z) + (dens.at(x).at(y).at(z) - l) + 1;
        /*
            rows -> l, z, y -> x
            cols -> l, z, x -> y
            bars -> l, x, y -> z
            dens -> x, y, z -> l
         */
}
    
double Diagram4D::calc_weight(int l, int z, int y, int x){
        int l_,z_,x_,y_, hook;
        double weight = 1;
        //if(ocorners.find(make_tuple(l,z,y)) != ocorners.end()){
            for(l_ = l - 1; l_ >= 0; l_--){
                hook = get_hook(l_, z, y, x);
                weight *= (hook * 1.0)/(hook + 1);
            }
            
            for(z_ = z - 1; z_ >= 0; z_--){
                hook = get_hook(l, z_, y, x);
                weight *= (hook * 1.0)/(hook + 1);
            }
            
            for(y_ = y - 1; y_ >= 0; y_--){
                hook = get_hook(l, z, y_, x);
                weight *= (hook * 1.0)/(hook + 1);
            }
            
            for(x_ = x - 1; x_ >= 0; x_--){
                hook = get_hook(l, z, y, x_);
                weight *= (hook * 1.0)/(hook + 1);
            }
       // }
        return weight;
}
    
double Diagram4D::calc_pp_sum(){
        mtid::iterator it;
        int l, z, y;
        double sum = 0, weight;
        
        for(it = ocorners.begin(); it != ocorners.end(); it++){
            l = get<0>(it->first);
            z = get<1>(it->first);
            y = get<2>(it->first);
            weight = calc_weight(l, z, y, get_oX(l, z, y));
            sum += weight;
            ocorners[make_tuple(l,z,y)] = weight;
        }
        return sum;
}

double Diagram4D::calc_pp_prob(int l, int z, int y, int x){
    return (calc_weight(l, z, y, x) / calc_pp_sum());
}

double Diagram4D::calc_pp_probs(){
    mtid :: iterator it;
    double sum;
    int l, z, y, i;

    sum = calc_pp_sum();
    for(it = ocorners.begin(), i = 0; it != ocorners.end(); it++, i++){
        l = get<0>(it->first);
	z = get<1>(it->first);
	y = get<2>(it->first);
	(*it).second = calc_weight(l, z, y, get_oX(l, z, y)) / sum;
   }
   return sum;
}

vector< set<ti> > Diagram4D::calc_outer_cubes(){
        vector< set<ti> > cubes;
        int l,z,y,x;

        cubes.resize(rows.size());

        for(l = 0; l < rows.size(); l++){
            for(z = 0; z < rows.at(l).size(); z++){
                for(y = 0; y < rows.at(l).at(z).size(); y++){
                    cubes.at(l).insert(make_tuple(z, y, rows.at(l).at(z).at(y) - 1));
                }
            }
        }


        for(l = 0; l < cols.size(); l++){
            for(z = 0; z < cols.at(l).size(); z++){
                for(x = 0; x < cols.at(l).at(z).size(); x++){
                    cubes.at(l).insert(make_tuple(z, cols.at(l).at(z).at(x) - 1, x));
                }
            }
        }

        for(l = 0; l < bars.size(); l++){
            for(x = 0; x < bars.at(l).size(); x++){
                for(y = 0; y < bars.at(l).at(x).size(); y++){
                    cubes.at(l).insert(make_tuple(bars.at(l).at(x).at(y) - 1, y,x));
                }
            }
        }
        return cubes;
}

bool Diagram4D:: checking_outer_cube(oint128 level, oint128 z, oint128 y, oint128 x){
    if(get_oX(level,z,y) == x){
        if(get_oY(level,z,x) == y){
	    if(get_oZ(level,x,y) == z){
		if(get_oD(x,y,z) == level){
		    return true;
		}
	    }
	}
    }
    return false;
}

void Diagram4D:: calc_ocorners(oint128 level, oint128 z, oint128 y){
    int x;
    mtid:: iterator it_X, it_Y, it_Z, it_L;

    it_X = ocorners.find(make_tuple(level,z,y));
    it_Y = ocorners.find(make_tuple(level,z,y + 1));
    it_Z = ocorners.find(make_tuple(level,z + 1,y));
    it_L = ocorners.find(make_tuple(level + 1,z,y));
    x = rows.at(level).at(z).at(y) - 1;

    if(checking_outer_cube(level, z, y, x+1)){
        ocorners[make_tuple(level,z,y)];
    }else if(it_X != ocorners.end())
        ocorners.erase(it_X);

    if(x == get_oX(level,z,y+1))
	    if(checking_outer_cube(level, z, y+1, x)){
		ocorners[make_tuple(level,z,y+1)];
	    }else if(it_Y != ocorners.end())
		ocorners.erase(it_Y);

    if(x == get_oX(level,z+1,y))
	    if(checking_outer_cube(level, z+1, y, x)){
		ocorners[make_tuple(level,z+1,y)];
	    }else if(it_Z != ocorners.end())
		ocorners.erase(it_Z);

    if(x == get_oX(level+1,z,y))
	    if(checking_outer_cube(level+1, z, y, x)){
		ocorners[make_tuple(level+1,z,y)];
	    }else if(it_L != ocorners.end())
		ocorners.erase(it_L);
}
     
int Diagram4D::get_oX(int l, int z, int y){
        int x;
         
        if(l >= rows.size())
            x = 0;
        else if(z >= rows.at(l).size())
            x = 0;
        else if(y >= rows.at(l).at(z).size())
            x = 0;
        else
            x = rows.at(l).at(z).at(y);
        return x;
}

int Diagram4D::get_oY(int l, int z, int x){
        int y;
         
        if(l >= cols.size())
            y = 0;
        else if(z >= cols.at(l).size())
            y = 0;
        else if(x >= cols.at(l).at(z).size())
            y = 0;
        else
            y = cols.at(l).at(z).at(x);
        return y;
}

int Diagram4D::get_oZ(int l, int x, int y){
        int z;
         
        if(l >= bars.size())
            z = 0;
        else if(x >= bars.at(l).size())
            z = 0;
        else if(y >= bars.at(l).at(x).size())
            z = 0;
        else
            z = bars.at(l).at(x).at(y);
        return z;
}

int Diagram4D::get_oD(int x, int y, int z){
        int d;
         
        if(x >= dens.size())
            d = 0;
        else if(y >= dens.at(x).size())
            d = 0;
        else if(z >= dens.at(x).at(y).size())
            d = 0;
        else
            d = dens.at(x).at(y).at(z);
        return d;
}

int Diagram4D::get_maxXYZ(){
    return max(rows.at(0).at(0).at(0), max(cols.at(0).at(0).at(0), bars.at(0).at(0).at(0)));
}

vvvi Diagram4D::getCols(){
        return cols;
}

mtid Diagram4D::getOcorners(){
        return ocorners;
}

vvvi Diagram4D::getRows(){
        return rows;
}




void Diagram4D::print(ostream &output){
        for(int i = 0; i < this->rows.size(); i++){
            for(int j = 0; j < this->rows.at(i).size(); j++){
                if(j > 0)
                    output << ", ";
                for(int k = 0; k < this->rows.at(i).at(j).size(); k++){
                    if(k > 0)
                        output << ' ';
                    output << rows.at(i).at(j).at(k);
                }
            }
            output << endl;
        }
}
    
void Diagram4D::print_cols(ostream &output){
        for(int l = 0; l < this->cols.size(); l++){
            for(int z = 0; z < this->cols.at(l).size(); z++){
                if(z > 0)
                    output << ", ";
                for(int x = 0; x < this->cols.at(l).at(z).size(); x++){
                    if(x > 0)
                        output << ' ';
                    output << cols.at(l).at(z).at(x);
                }
            }
            output << ";";
        }
}
    
void Diagram4D::print_bars(ostream &output){
        for(int l = 0; l < this->bars.size(); l++){
            for(int x = 0; x < this->bars.at(l).size(); x++){
                if(x > 0)
                    output << ", ";
                for(int y = 0; y < this->bars.at(l).at(x).size(); y++){
                    if(y > 0)
                        output << ' ';
                    output << bars.at(l).at(x).at(y);
                }
            }
            output << endl;
        }
}
    
void Diagram4D::print_dens(ostream &output){
        for(int x = 0; x < this->dens.size(); x++){
            for(int y = 0; y < this->dens.at(x).size(); y++){
                if(y > 0)
                    output << ", ";
                for(int z = 0; z < this->dens.at(x).at(y).size(); z++){
                    if(z > 0)
                        output << ' ';
                    output << dens.at(x).at(y).at(z);
                }
            }
            output << endl;
        }
}
    
ti Diagram4D::add_rich(){
        mtid::iterator it;
        ti coord;
        
        it = ocorners.begin();
        advance(it, rand_0_to_n_1(ocorners.size()));
        coord = it->first;
        add(coord);
        return coord;
}

void Diagram4D::clear(){
    rows.clear();
    cols.clear();
    bars.clear();
    dens.clear();
    ocorners.clear();
    ocorners[ti(0,0,0)];
    short_vec.clear();
    short_vec.push_back(1);
    long_vec.clear();
    long_vec.push_back(short_vec);
    diam = 0;
}
    
void Diagram4D::print_ocorners(ostream &output){
        mtid::iterator it;
        for(it = ocorners.begin(); it != ocorners.end(); it++){
            output << get<0>(it->first) << ' ' << get<1>(it->first) << ' ' << get<2>(it->first) << ' ' << it->second << endl;
        }
}
    
void Diagram4D::print_all(ostream &output){
        output << "Rows" << endl;
        print(output);
        output << "Cols" << endl;
        print_cols(output);
        output << "Bars" << endl;
        print_bars(output);
        output << "Dens" << endl;
        print_dens(output);
        output << "Ocorners" << endl;
        print_ocorners(output);
}

void Diagram4D::print_outer_cubes(ostream &output){
    vector< set<ti> > cubes;
    set<ti>::iterator it;
    int z, y, x, max_X, max_Y, max_Z;

    cubes = calc_outer_cubes();
    for(int i = 0; i < cubes.size(); i++){
        for(it = cubes.at(i).begin(); it != cubes.at(i).end(); it++){
	    z = get<0>(*it);
	    y = get<1>(*it);
	    x = get<2>(*it);
	    max_X = rows.at(i).at(z).at(y) - 1;
	    max_Y = cols.at(i).at(z).at(x) - 1;
	    max_Z = bars.at(i).at(x).at(y) - 1;
   	    output << x << ' ' << y << ' ' << z << ' ' << i << ' '; 
	    if( (z == 0 && (x == max_X || y == max_Y)) || (y == 0 && (x == max_X || z == max_Z)) || (x == 0 && (z == max_Z || y == max_Y)))
   	        output << 1 << endl;
            else
                output << 0 << endl;  
        }
    }
}

void Diagram4D::build_tab(int flag, string fname, oint128 size){
    ti added_cube;
    double prob, sum, logsum;
    int l, z, y, i;
    ofstream fout(fname);
    
    fout << fixed << setprecision(20);
    logsum = 0;
    if(fout){
        for(i = 1; i != size; i++){
	    switch(flag){
		case 0:
		    added_cube = add_rich();
		    break;
		case 1:
		    added_cube = add_pp_rand(&prob, &sum);
		    break;
		case 2:
		    prob = 0;
		    added_cube = add_pp_max(&prob, &sum);
		    break;
		default:
		    break;
	    }
	    l = get<0>(added_cube);
	    z = get<1>(added_cube);
	    y = get<2>(added_cube);
	    fout << get_oX(l, z, y) - 1 << ' ' << y << ' ' << z << ' ' << l << ' ' << ocorners.size() << ' ' << diam;
	    if(flag == 1 || flag == 2){
	        logsum -= log(prob);
	        fout << ' ' << prob << ' ' << sum << ' ' << logsum;
            }
            fout << endl;
        }
    }
    fout.close();
}

void Diagram4D::tab2img(string fname, int num, string pngname){
    int i, j;
    int arr[4];
    string str;
    ifstream fin(fname);

    clear();    
    if(fin){
        for(i = 0; i < (num); i++){
	    getline(fin, str);
            stringstream ss(str);
            j = 0;
            while(getline(ss,str,' ') && j < 4){
	       arr[j]  = stoi(str);
               j++;
	    }
	    add(arr[3], arr[2], arr[1]);
        }
        print_imgs(pngname, get_maxXYZ());
    }
}

double Diagram4D::cube_num(vvvi arr, int l, int coord, int flag){
    int i, sum;
    sum = 0;

    if(flag == 1){
	for(i = 0; i < arr.at(l).size(); i++){
	    sum += arr.at(l).at(i).at(coord);
        }
        return sqrt(sum);
    }
    for(i = 0; i < arr.at(l).at(coord).size(); i++){
	sum += arr.at(l).at(coord).at(i);
    }
    return sqrt(sum);
}

void Diagram4D::print_imgs(const string file_name, int L_big){
	const int DPI = 300;

        ofstream fpy;
	bool print_ocorners, draw_plansh_limit_shape, do_crop;
        int l,z,y,x,L, L_ocorners;
        vector< vector< vector<bool> > > big_cube, big_ocorners;
        vector<set<ti>> outer_cube = calc_outer_cubes();
        double cube_sum_XoY,cube_sum_XoZ,cube_sum_ZoY;

	cube_sum_XoY = 0;
	cube_sum_XoZ = 0;
	cube_sum_ZoY = 0;
	print_ocorners = false;
	draw_plansh_limit_shape = true;
	do_crop = true;
	if(L_big == 0)
            L = max(max(rows.at(0).at(0).at(0),cols.at(0).at(0).at(0)),bars.at(0).at(0).at(0));
        else
            L = L_big;

        L_ocorners = L + 1;
        set<ti>::iterator itt;
          
        for(l = 0; l < rows.size(); l++){

//??? maybe use individual L instead of L_ocorners???
            big_cube.clear();
            big_cube.resize(L_ocorners, vector<vector<bool>>(L_ocorners,vector<bool>(L_ocorners,0)));
            for(x = 0; x < L; x++){
                for(y = 0; y < L; y++){
                    for(z = 0; z < L; z++){
                        if((outer_cube.at(l)).find(make_tuple(z,y,x)) != outer_cube.at(l).end()){
                        big_cube.at(x).at(y).at(z) = 1;
                    }
                }
            }
        }
  
        big_ocorners.clear();
        big_ocorners.resize(L_ocorners, vector<vector<bool>>(L_ocorners,vector<bool>(L_ocorners,0)));
        for(z = 0; z < L_ocorners; z++){
            for(y = 0; y < L_ocorners; y++){
                if(ocorners.find(make_tuple(l,z,y)) != ocorners.end()){
                    big_ocorners.at(z).at(y).at( get_oX(l,z,y) ) = 1;
                }
            }
        }

        
        fpy.open("imgScript.py");
        fpy << "import matplotlib.pyplot as plt\n"; 
        fpy << "import numpy as np\n";
        fpy << "from mpl_toolkits.mplot3d import Axes3D\n";
        fpy << "rows = np.array([\n";
        for(x = 0; x < L_ocorners; x++){
            fpy << "[";
            for(y = 0; y < L_ocorners; y++){
                fpy << "[";
                for(z = 0; z < L_ocorners; z++){
                    if(big_cube.at(x).at(y).at(z))
                        fpy << "True";
                    else
                        fpy << "False";
                    if(z < L_ocorners - 1)
                        fpy << ", ";
                } 
                fpy << "]";
                if(y < L_ocorners - 1)
                    fpy << ", ";
                
            }
            fpy << "]\n";
            if(x < L_ocorners - 1)
                fpy << ", ";

        }
        fpy << "])\n"; 
        
        fpy << "ocorners = np.array([\n";
        for(x = 0; x < L_ocorners; x++){
            fpy << "[";
            for(y = 0; y < L_ocorners; y++){
                fpy << "[";
                for(z = 0; z < L_ocorners; z++){
                    if(print_ocorners == true && big_ocorners.at(z).at(y).at(x))
                        fpy << "True";
                    else
                        fpy << "False";
                    if(z < L_ocorners - 1)
                        fpy << ", ";
                } 
                fpy << "]";
                if(y < L_ocorners - 1)
                    fpy << ", ";
                
            }
            fpy << "]\n";
            if(x < L_ocorners - 1)
                fpy << ", ";

        }
        fpy << "])\n"; 
        
	cube_sum_XoY = cube_num(rows,l,0,0);
	cube_sum_XoZ = cube_num(bars,l,0,1);
	cube_sum_ZoY = cube_num(bars,l,0,0);
	if(draw_plansh_limit_shape){
		fpy << "handle = open(\"2Dplanch_limitshape.txt\", \"r\")\n";
		fpy << "data = handle.read().splitlines()\n";
		fpy << "coord_X = []\n";
		fpy << "coord_Y = []\n";
		fpy << "for i in data:\n";
		fpy << "  coord_X.append(((float)((i.split())[0])))\n";
		fpy << "  coord_Y.append(((float)((i.split())[1])))\n";
		fpy << "coord_Z = np.repeat(0.5, len(coord_X))\n";
		fpy << "coord_X1 = [x*" << cube_sum_XoY << " for x in coord_X]\n";
		fpy << "coord_Y1 = [y*" << cube_sum_XoY << " for y in coord_Y]\n";
		fpy << "coord_X2 = [x*" << cube_sum_XoZ << " for x in coord_X]\n";
		fpy << "coord_Z2 = [z*" << cube_sum_XoZ << " for z in coord_Y]\n";
		fpy << "coord_Z3 = [z*" << cube_sum_ZoY << " for z in coord_X]\n";
		fpy << "coord_Y3 = [y*" << cube_sum_ZoY << " for y in coord_Y]\n";
		fpy << "handle.close()\n";
	}

        fpy << "voxels = rows | ocorners\n";
        fpy << "colors = np.empty(voxels.shape, dtype=object)\n";
        fpy << "colors[rows] = 'white'\n";
        fpy << "colors[ocorners] = '#00ff0099'\n";
        fpy << "fig = plt.figure()\n";
        fpy << "ax = fig.gca(projection='3d')\n";
	fpy << "plt.plot(coord_X1, coord_Y1, coord_Z)\n";
	fpy << "plt.plot(coord_X2, coord_Z, coord_Z2)\n";
	fpy << "plt.plot(coord_Z, coord_Y3, coord_Z3)\n";
        fpy << "ax.set_aspect('equal')\n";
        fpy << "plt.axis('off')\n";
        fpy << "ax.view_init(azim = 45, elev = 45)\n";
        //fpy << "ax.set_xlim3d(0," << L_ocorners << ")\n";
        //fpy << "ax.set_ylim3d(0," << L_ocorners << ")\n";
        //fpy << "ax.set_zlim3d(0," << L_ocorners << ")\n";
        fpy << "ax.voxels(voxels, facecolors=colors, edgecolor='k',linewidth=0.1)\n";
	if(do_crop){
	        fpy << "ax.scatter([" << 0 << "],[" << 0 << "],[" << L_ocorners + 1 << "],color='#ff000001')\n";
        	fpy << "ax.scatter([" << L_ocorners + 1 << "],[" << 0 << "],[" << 0 << "],color='#ff000001')\n";
        	fpy << "ax.scatter([" << 0 << "],[" << L_ocorners + 1 << "],[" << 0 << "],color='#ff000001')\n";
	}
        fpy << "plt.show()\n";
        //fpy << "plt.savefig('" << file_name << '_' << l << ".png', dpi = " << DPI << ")";
        fpy.close();
        if (system("python3 imgScript.py"));
        //if (system((string("convert ") + file_name + '_' + to_string(l) + ".png -trim " + file_name + '_' + to_string(l) + ".png").c_str()));
        }
	//The last level (always the same):
        if(print_ocorners){
		fpy.open("imgScript.py");
		fpy << "import matplotlib.pyplot as plt\n"; 
		fpy << "import numpy as np\n";
		fpy << "from mpl_toolkits.mplot3d import Axes3D\n";
		fpy << "cube = np.array([[[True]]])\n";
		fpy << "fig = plt.figure()\n";
		fpy << "ax = fig.gca(projection='3d')\n";
		fpy << "ax.set_aspect('equal')\n";
		fpy << "plt.axis('off')\n";
		fpy << "ax.view_init(azim = 45, elev = 45)\n";
		//fpy << "ax.set_xlim3d(0," << L_ocorners << ")\n";
		//fpy << "ax.set_ylim3d(0," << L_ocorners << ")\n";
		//fpy << "ax.set_zlim3d(0," << L_ocorners << ")\n";
		fpy << "ax.voxels(cube, facecolors='#00ff0099', edgecolor='k',linewidth = 0.1)\n";
		if(do_crop){
			fpy << "ax.scatter([" << 0 << "],[" << 0 << "],[" << L_ocorners + 1 << "],color='#ff000001')\n";
			fpy << "ax.scatter([" << L_ocorners + 1 << "],[" << 0 << "],[" << 0 << "],color='#ff000001')\n";
			fpy << "ax.scatter([" << 0 << "],[" << L_ocorners + 1 << "],[" << 0 << "],color='#ff000001')\n";
//		fpy << "plt.savefig('" << file_name << '_' << l << ".png', dpi = " << DPI << ")";
		}
		fpy.close();
		if(system("python3 imgScript.py"));
		//if (system((string("convert ") + file_name + '_' + to_string(l) + ".png -trim " + file_name + '_' + to_string(l) + ".png").c_str()));
	}
}
