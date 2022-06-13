#include "Diagram.h"

mdd Diagram::planch_limit;

Diagram::Diagram()
{
	init();
	//diag_tmp = new Diagram(true);
}

Diagram::Diagram(Diagram* copy_from)
{
	init(copy_from);
	//diag_tmp = new Diagram(true);
}

/*
Diagram::Diagram(bool)
{
	init();
	diag_tmp = NULL;
}


Diagram::~Diagram()
{
	if (diag_tmp != NULL)
		delete diag_tmp;
}
*/

//алгоритм ветвей
/*void Diagram::branches_alg(){
	int num_branches = 3;//число ветвей
	int num_change = 1;//число изменяемых клеток у изначальной диаграммы
	int depth = 2;//глубина жадного алгоритма
	random_diags(num_branches, num_change, depth);
}*/

void Diagram::random_diags(int num_br, int num_ch, int depth, vector <di> *bank){
	int num_change_in_branch  = 1;//число изменяемых клеток при повторе диаграммы в ветвях
	Diagram diag;
	Diagram diag_from_bank;
	di* old_cols;
	di new_cols;
	set <di> set_di;
	set <di> set_branch;
	
	set_di.insert(this->cols);
	old_cols = get_cols();
	while(set_di.size()<num_br){
		new_cols = random_change(old_cols, num_ch);
		set_di.insert(new_cols);
	}
	
	for (auto it=set_di.begin(); it!=set_di.end(); it++){
		diag.init_by_cols(*it);
		set_branch.insert(*it);
		
		int d = 0;
		int num = 0;
		
		num = diag.get_size();
		diag_from_bank.init_by_cols(bank->at(num-1));
		if (diag.comp_dim(&diag_from_bank)>1){
			#pragma omp critical
			{
				diag_from_bank.init_by_cols(bank->at(num-1));
				if (diag.comp_dim(&diag_from_bank)>1)
					bank->at(num-1) = *diag.get_cols();
			}
			diag_from_bank.init_by_cols(bank->at(num-1));
		}
				
		while (d < depth){
			diag.add_max();
			while(set_branch.find(*diag.get_cols()) != set_branch.end()){
					new_cols = random_change(diag.get_cols(), num_change_in_branch);
					diag.init_by_cols(new_cols);
			}
			
			num = diag.get_size();
			diag_from_bank.init_by_cols(bank->at(num-1));
			if (diag.comp_dim(&diag_from_bank)>1){
				#pragma omp critical
				{
					diag_from_bank.init_by_cols(bank->at(num-1));
					if (diag.comp_dim(&diag_from_bank)>1)
						bank->at(num-1) = *diag.get_cols();
				}
				diag_from_bank.init_by_cols(bank->at(num-1));
			}
			set_branch.insert(*diag.get_cols());
			++d;
		}
	}
}

di Diagram::random_change(di* cols, int num_ch){
	uint64 y;
	Diagram diag;
	di* new_cols;
	diag.init_by_cols(*cols);
	
	int i = 0;
	int k = 0;
	
	while (i < num_ch){
		auto it = diag.points.begin();
		advance(it, rand() % diag.points.size());
		y = it->first;
		diag.add(y);
		++i;
	}
	
	while (k < num_ch){
		auto it = diag.corners.begin();
		advance(it, rand() % diag.corners.size());
		y = it->first;
		diag.remove(y);
		++k;
	}
	new_cols = diag.get_cols();
	return(*new_cols);
}

//Нормализованные размерности 
double Diagram::norm_dim_hook(){
    int x;
    int y;
    int k = n;
    double ln = 0;
    double c = 0;
    for (x = 0; x<cols.size();++x){
        for (y = 0; y<cols.at(x);++y){
            ln = ln + log (sqrt(k)/get_hook(x,y));
            --k;
        }
    }
    c = -(1/sqrt(n))*ln;
    return c;
    //cout <<setprecision(20)<<c<<"\n";
}

double Diagram::new_norm_dim(Diagram *diag){
    //ofstream ffout ("res.txt");
    int i;
    int col, col_diag,diff;
    di* diag_cols;
    double c;
    double t;
    diag_cols = diag->get_cols();
	for (i = diag_cols->size()-1 ;i>=0;--i){
        if (i <=cols.size()-1){
            col = cols.at(i);
            }
            else 
                col = 0;
		col_diag = (*diag_cols).at(i);
		while (col_diag>col){
            diag->remove (col_diag-1,-1);
            col_diag = col_diag -1;
            }

    }

    for (i = 0; i <cols.size();++i){
        col = cols.at(i);
        if (i <=diag_cols->size()-1){
            col_diag = (*diag_cols).at(i);
            }
            else 
                col_diag  = 0;
        while (col>col_diag){
            diag->add_with_probsum(col_diag);
            col_diag = col_diag +1;
            }
        }
    norm_dim = diag->norm_dim;
    //cout <<setprecision(20)<<get_c()<<"\n";
    c = get_c();
    return c;
    
}


    
//Blow-up
void Diagram::remove_blow_up(){
    uint64 y;
    uint64 x;
    double y_worst;
    double al_prev = 0;
    double al_w = 0;
    double al;
    for(auto it=corners.begin(); it != corners.end(); it++){ 
        y = it->first;
        x = get_width(y);
        al = count_al(x, y);
        if (al_prev == 0){
                al_w = al;
                al_prev = al;
                y_worst = y;
                }
                else{
                    if (al>al_w){
                        al_w = al;
                        al_prev = al;
                        y_worst = y;
                        }
                        else{
                            al_prev = al;
                            }
                    }
    }
    remove(y_worst);
}
void Diagram::add_with_table(vector<vector<double>>&table,int par){//2 - без таблицы, 3 - с таблицей
    uint64 y;
    uint64 x;
    double y_best;
    double al_prev = 0;
    double al_b = 0;
    double al;
    int k = 3;
    multimap <double,int> m;
    for(auto it=points.begin(); it != points.end(); it++){ 
        y = it->first;
        x = get_width(y);
        if ((par == 3)||(par == 4)){
            uint64 s;
            s = table.size();
            if ((x<s)&&(y<s)){
                if (y<=x){
                    al = table [x][y];
                }
                if (y>x){
                    al = table [y][x];
                }
            }

            else{
                al = count_al(x,y);
            }
            if (par == 4){
                m.insert ( pair<double,int>(al,y)); 
            }
        }
        if (par == 2){ 
            al = count_al(x, y);
            }
        if(par!=4){
            if (al_prev == 0){
                al_b = al;
                al_prev = al;
                y_best = y;
            }
            else{
                if (al<al_b){
                    al_b = al;
                    al_prev = al;
                    y_best = y;
                }
                else{
                    al_prev = al;
                    }
            }
        }
    }
    if (par!=4){
        add(y_best);
        }
    if (par == 4){
        int i = 0;
        for (auto it_m = m.begin(); it_m != m.end(); ++it_m){
            if (i<k){
                //cout << it_m->first << " : " << it_m->second << endl;
                //++i;
                y_best = it_m->second;
                add(y_best);
                ++i;
            }
        } 
    } 
}
    void Diagram::remove_with_table(vector<vector<double>>&table,int par){//2 - без таблицы, 3 - с таблицей
    uint64 y;
    uint64 x;
    double y_worst;
    double al_prev = 0;
    double al_w = 0;
    double al;
    int k = 3;
    multimap <double,int,greater<double>> m;
    for(auto it=corners.begin(); it != corners.end(); it++){ 
        y = it->first;
        x = get_width(y);
        if ((par == 3)||(par ==4)){
            uint64 s;
            s = table.size();
            if ((x<s)&&(y<s)){
                if (y<=x){
                    al = table [x][y];
                }
                if (y>x){
                    al = table [y][x];
                }
            }
            else{
                al = count_al(x,y);
            }
            if (par == 4){
                m.insert ( pair<double,int>(al,y)); 
            }
        }
        else{ 
            al = count_al(x, y);
            }
            if (par!=4){
                if (al_prev == 0){
                    al_w = al;
                    al_prev = al;
                    y_worst = y;
                }
                else{
                    if (al>al_w){
                        al_w = al;
                        al_prev = al;
                        y_worst = y;
                    }
                    else{
                        al_prev = al;
                    }
                }
            }
    }
    if (par!=4){
        remove(y_worst);
        }
    if (par == 4){
        int i = 0;
        for (auto it_m = m.begin(); it_m != m.end(); ++it_m){
            if (i<k){
                //cout << it_m->first << " : " << it_m->second << endl;
               // ++i;
                y_worst = it_m->second;
                remove(y_worst);
                ++i;
            }
        } 
    } 
}
 

    /*uint64 y;
    uint64 x;
    double y_best;
    double al_prev = 0;
    double al_b = 0;
    double al;
    for(auto it=points.begin(); it != points.end(); it++){ 
        y = it->first;
        x = get_width(y);
        al = count_al(x, y);
        if (al_prev == 0){
                al_b = al;
                al_prev = al;
                y_best = y;
                }
                else{
                    if (al<al_b){
                        al_b = al;
                        al_prev = al;
                        y_best = y;
                        }
                        else{
                            al_prev = al;
                            }
                    }
    }
    add(y_best);*/
//}

void Diagram::add_blow_up(){
    uint64 y;
    uint64 x;
    double y_best;
    double al_prev = 0;
    double al_b = 0;
    double al;
    for(auto it=points.begin(); it != points.end(); it++){ 
        y = it->first;
        x = get_width(y);
        al = count_al(x, y);
        if (al_prev == 0){
                al_b = al;
                al_prev = al;
                y_best = y;
                }
                else{
                    if (al<al_b){
                        al_b = al;
                        al_prev = al;
                        y_best = y;
                        }
                        else{
                            al_prev = al;
                            }
                    }
    }
    add(y_best);
}

double Diagram::count_al(uint64 x, uint64 y){
    double u; 
    double v;
    double det;
    double det_1;
    double det_2;
    double x_prev;
    double y_prev;
    double x_B;//Координата x заданной точки
    double y_B;//Координата y заданной точки
    double y_E = 2;//Координата y начала предельной формы
    double x_F = 2;//Координата x конца предельной формы
    double x_C;//Координата x пересечения AB и EF
    double y_C;//Координата y пересечения AB и EF
    double x_D;//Координата x искомой точки
    double y_D;//Координата y искомой точки
    double _F = 0; //Значение функции -f
    double _G;//Значение функции -g
    double Ja[2][2];//Якобиан
    double d_x;//дельта x
    double d_y;//дельта y
    double e = 0.0001; //критерий остановки
    double al;//альфа
    double l_AB;//длина AB
    double l_AD;//длина AD
    x_D = 0;
    y_D = 0;
    x_B = x+0.5;
    y_B = y+0.5;
    //Точка пересечения AB и EF
    x_C = y_E/(y_B/x_B + y_E/x_F);
    y_C = (y_E*y_B*x_F)/(y_B*x_F+y_E*x_B);
    x_prev = x_C;
    y_prev = y_C;
    //Вычисление -f, -g
    while ((abs(x_prev-x_D) > e)||(abs(y_prev-y_D)>e)){
        u = (x_C-y_C)/2;
        v = (x_C+y_C)/2;
        _G = -(v - 2/M_PI * (u*asin(u) + sqrt(1- u*u)));
        //Вычисление Якобиана
        Ja[0][0] = 1/(x_C+e);
        Ja[0][1] = -1/(y_C+e);
        Ja[1][0] = 0.5 - (asin(u)/M_PI);
        Ja[1][1] = (asin(u)/M_PI) + 0.5;
        //Метод Крамера
        det = Ja[0][0] * Ja[1][1] - Ja[1][0]*Ja[0][1];
        if (det!=0){
            det_1 = _F*Ja[1][1]-_G*Ja[0][1];
            det_2 = Ja[0][0]*_G - Ja[1][0]*_F;
            d_x = det_1/det;
            d_y = det_2/det;
            x_prev = x_C;
            y_prev = y_C;
            x_D = x_C+d_x;
            y_D = y_C+d_y;
            x_C = x_D;
            y_C = y_D;
        }
    }
    //Вычисление альфы
    l_AB = sqrt((x_B)*(x_B)+(y_B)*(y_B));
    l_AD = sqrt((x_D)*(x_D)+(y_D)*(y_D));
    al = l_AB/l_AD;
    return al;
}


//Добавление клетки с лучшей альфой с проверками
/*void Diagram::best_add(){
    uint64 y;
    uint64 x;
    double y_best;
    //Служебные переменные:
    int i;
    int j;
    int k;
    double u; 
    double v;
    double det;
    double det_1;
    double det_2;
    double x_prev;
    double y_prev;
    //Все известные точки
    double x_B;//Координата x заданной точки
    double y_B;//Координата y заданной точки
    double y_E = 2;//Координата y начала предельной формы
    double x_F = 2;//Координата x конца предельной формы
    double x_C;//Координата x пересечения AB и EF
    double y_C;//Координата y пересечения AB и EF
    double x_D;//Координата x искомой точки
    double y_D;//Координата y искомой точки
    double _F = 0; //Значение функции -f
    double _G;//Значение функции -g
    double Ja[2][2];//Якобиан
    double d_x;//дельта x
    double d_y;//дельта y
    double e = 0.001; //критерий остановки
    double al;//альфа
    double l_AB;//длина AB
    double l_AD;//длина AD
    x_D = 0;
    y_D = 0;
    double al_prev = 0;
    double al_b = 0;
    for(auto it=points.begin(); it != points.end(); it++){ 
        y = it->first;
        x = get_width(y);
        cout <<"y="<< y<< "\n";
        cout <<"x="<<x<<"\n";
        x_B = x;
        y_B = y;
        //Точка пересечения AB и EF
        x_C = y_E/(y_B/x_B + y_E/x_F);
        y_C = (y_E*y_B*x_F)/(y_B*x_F+y_E*x_B);
        x_prev = x_C;
        y_prev = y_C;
        //Вычисление -f, -g
            while ((abs(x_prev-x_D) > e)||(abs(y_prev-y_D)>e)){
                u = (x_C-y_C)/2;
                v = (x_C+y_C)/2;
                _G = -(v - 2/M_PI * (u*asin(u) + sqrt(1- u*u)));
                //Вычисление Якобиана
                Ja[0][0] = 1/(x_C+e);
                Ja[0][1] = -1/(y_C+e);
                Ja[1][0] = 0.5 - (asin(u)/M_PI);
                Ja[1][1] = (asin(u)/M_PI) + 0.5;
                //Метод Крамера
                det = Ja[0][0] * Ja[1][1] - Ja[1][0]*Ja[0][1];
                if (det!=0){
                    det_1 = _F*Ja[1][1]-_G*Ja[0][1];
                    det_2 = Ja[0][0]*_G - Ja[1][0]*_F;
                    d_x = det_1/det;
                    d_y = det_2/det;
                    x_prev = x_C;
                    y_prev = y_C;
                    x_D = x_C+d_x;
                    y_D = y_C+d_y;
                    x_C = x_D;
                    y_C = y_D;
                }
            }
            //Вычисление альфы
            l_AB = sqrt((x_B)*(x_B)+(y_B)*(y_B));
            l_AD = sqrt((x_D)*(x_D)+(y_D)*(y_D));
            al = l_AB/l_AD;
            cout <<"альфа = "<<al<<"\n";
            if (al_prev == 0){
                al_b = al;
                al_prev = al;
                y_best = y;
                }
                else{
                    if (al<al_b){
                        al_b = al;
                        al_prev = al;
                        y_best = y;
                        }
                        else{
                            al_prev = al;
                            }
                    }
            cout<<"лучшая альфа = "<<al_b<<"\n";
            cout<<"ее y = "<<y_best<<"\n";  
    }
    cout <<"проверка y "<<y_best<< endl;
    print_cols();
    add(y_best);
    cout <<"y добавленной = "<<y_best<<"\n";
    y_best = 0;
    al_prev = 0;
    al_b = 0;
}*/



void Diagram::init()
{
	rows.clear(); cols.clear(); corners.clear();  points.clear(); //prob_sums.clear();
	rows.push_back(1);
	cols.push_back(1);
	//table.push_back(0); table.push_back(0);
	points[0] = 0.43446725642244343746; points[1] = 0.43446725642244343746; //alpha...
	n = 1;
	norm_dim = 0;
	max_pl_prob = 0;
	last_added = 0;
}

void Diagram::init(Diagram* copy_from)
{
	rows = copy_from->rows;
	cols = copy_from->cols;
	points = copy_from->points;
	corners = copy_from->corners;
	//prob_sums = copy_from->prob_sums;
	norm_dim = copy_from->norm_dim;
	n = copy_from->n;
	max_pl_prob = copy_from->max_pl_prob;
	max_pl_line = copy_from->max_pl_line;
	//mpz_dim -????
	last_added = 0;
}

void Diagram::init_by_rows(di r)
{
	rows = r;
	calc_size_from_rows();
	calc_cols_from_rows();
	calc_pointscorners_from_rows();
	//table???
	//norm_dim???
}

void Diagram::init_by_cols(di c)
{
	cols = c;
	calc_rows_from_cols();
	calc_size_from_rows();
	calc_pointscorners_from_rows();
}

void Diagram::init_by_cols(vi c)
{
	di deq;

	deq.resize(c.size());
	copy(c.begin(), c.end(), deq.begin());
	init_by_cols(deq);
}

void Diagram::init_by_tab(dpu& tab, const uint64 LEN)
{
	di r;
	uint64 i, y;
	dpu::iterator it;
	
	r.push_back(1);
	for (it = tab.begin(), i = 0; (it != tab.end()) && (i < LEN - 1); ++it, ++i)
	{
		y = it->second;
		if (y + 1 > r.size())
			r.push_back(0);
		r[y]++;
	}

	init_by_rows(r);
}

void Diagram::calc_pointscorners_from_rows()
{
	uint64 prev_width, i;

	points.clear();
	corners.clear();

	points[0];

	if (n > 1)
		corners[rows.size() - 1];

	prev_width = rows.at(0);
	for (i = 1; i < rows.size(); i++)
	{
		if (rows.at(i) != prev_width)
		{
			points[i];
			corners[i - 1];
		}
		prev_width = rows.at(i);
	}
	points[i];
}

void Diagram::print()
{
	di::iterator it;

	for (it = rows.begin(); it != rows.end(); ++it)
		cout << *it << " ";
	//cout << endl;
}

void Diagram::print_rowsdim()
{
	di::iterator it;
	mpz_t dim;

	mpz_init(dim);
	for (it = rows.begin(); it != rows.end(); ++it)
		cout << *it << " ";
	cout << "| ";
	calc_exact_dim(dim);
	gmp_printf("%Zd\n", dim);

	mpz_clear(dim);
}

unsigned long int Diagram::getDim()
{
	mpz_t dim;
	mpz_init(dim);
	calc_exact_dim(dim);
	return mpz_get_ui(dim);
}

void Diagram::print_cols(ostream &output)
{
	di::iterator it;

	for (it = cols.begin(); it != cols.end(); ++it)
		output << *it << " ";
	output << endl;
}

void Diagram::print_rows_ASCII()
{
	di::iterator it;
	uint64 i;

	it = rows.end();
	--it;
	for (; it != rows.begin(); --it)
	{
		for (i = 0; i < *it; ++i)
		{
			cout << "*";
		}
		cout << endl;
	}
	for (i = 0; i < *it; ++i)
	{
		cout << "*";
	}
	cout << endl;
}

void Diagram::print_rowcols()
{
	di::iterator it;

	for (it = rows.begin(); it != rows.end(); ++it)
		cout << *it << " ";
	cout << endl;
	for (it = cols.begin(); it != cols.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

void Diagram::print_corners(ostream &output)
{
	for (mi::iterator it = corners.begin(); it != corners.end(); ++it)
		output << it->first << " ";
	output << endl;
}

void Diagram::print_corners_with_braces(ostream &output)
{
	for (mi::iterator it = corners.begin(); it != corners.end(); ++it)
		output << "(" << it->first << " " << rows.at(it->first) - 1 << ")";
	output << endl;
}

void Diagram::print_corners_cute(ostream &output)
{
	for (mi::iterator it = corners.begin(); it != corners.end(); ++it)
		output << rows.at(it->first) - 0.5  << " " << it->first+0.5 << endl;
	output << endl;
}

void Diagram::print_corners(FILE* f)
{
	for (mi::iterator it = corners.begin(); it != corners.end(); ++it)
		fprintf(f, "(%llu %llu)", it->first, rows.at(it->first) - 1);
	fprintf(f, "\n");
}

void Diagram::print_corners_for_gnuplot(uint64 exper_num, uint64 diag_num, uint64 depth, bool print_to_cout)
{
	const int DIG_NUM = 12;

	int zeros_exper, zeros_diag, zeros_depth;
	uint64 i, j;


	//print_corners();
	if (print_to_cout)
	{
		//Едем по строчкам снизу вверх
		for (i = 0; i != rows.size(); ++i)
		{
			for (j = rows.at(i); j != 0; --j)
			{
				printf("%llu 0\n", j);
				printf("%llu %llu\n", j, i+1);
				printf("0 %llu\n", i+1);
			}
		}
/*
		//Едем по корнерам снизу вверх
		for (it = corners.begin(); it != corners.end(); ++it)
		{
			cout << "it->first = " << it->first << endl;
			cout << "rows.at(it->first) = " << rows.at(it->first) << endl;
			printf("%llu 0\n", rows.at(it->first));
			printf("%llu %llu\n", rows.at(it->first), it->first + 1);
			printf("0 %llu\n", it->first + 1);
			if (it->first != rows.size() - 1)
			{
				it2 = it;
				++it2;
				for (i = rows.at(it->first) - 2; (i != it2->first) && (rows.at(it->first) >=2); --i)
				{
					cout << "i: " << i << endl;
					cout << "cols.at(i-1) = " << cols.at(i-1) << endl;
					for (j = 1; j <= cols.at(i-1); ++j)
					{
						printf("%llu 0\n", i);
						printf("%llu %llu\n", i, j);
						printf("0 %llu\n", j);
					}
				}
			}
		}*/
		return;
	}

	zeros_depth = DIG_NUM - num_digits(depth);
	zeros_exper = DIG_NUM - num_digits(exper_num);
	zeros_diag = DIG_NUM - num_digits(diag_num);


	char fname[100];
	char number[100];

	strcpy(fname, "gnuplot/diag");


	while (zeros_depth > 0)
	{
		strcat(fname, "0");
		--zeros_depth;
	}

	sprintf(number, "%llu", depth);

	strcat(fname, number);
	strcat(fname, "_");

	while (zeros_exper > 0)
	{
		strcat(fname, "0");
		--zeros_exper;
	}

	sprintf(number, "%llu", exper_num);

	strcat(fname, number);
	strcat(fname, "_");

	while (zeros_diag > 0)
	{
		strcat(fname, "0");
		--zeros_diag;
	}

	sprintf(number, "%llu", diag_num);
	strcat(fname, number);
	strcat(fname, ".dat");

	print_corners_for_gnuplot_all_boxes(fname);
}

void Diagram::print_corners_for_gnuplot_all_boxes(const char* data_fname)
{
	FILE* f;
	//mi::iterator it;
	uint64 i,j;

	f = fopen(data_fname, "w");

	fprintf(f, "%llu 0\n0 0\n0 %llu\n0 0\n", rows.at(0), cols.at(0));

	for (i = 0; i != rows.size(); ++i)
	{
		for (j = rows.at(i); j != 0; --j)
		{
			fprintf(f, "%llu 0\n", j);
			fprintf(f, "%llu %llu\n", j, i+1);
			fprintf(f, "0 %llu\n", i+1);
		}
	}
	/*
	fprintf(f, "%llu 0\n", rows.at(corners.begin()->first));
	for (it = corners.begin(); it != corners.end(); ++it)
		fprintf(f, "%llu %llu\n", rows.at(it->first), it->first + 1);
	--it;
	fprintf(f, "0 %llu\n", it->first + 1);
	*/
	fclose(f);
}

void Diagram::print_corners_for_gnuplot_skewed(const char* data_fname)
{
	FILE* f;
	//mi::iterator it;
	uint64 i,j;

	f = fopen(data_fname, "w");

	fprintf(f, "%llu 0\n0 0\n0 %llu\n0 0\n", rows.at(0), cols.at(0));

	for (i = 0; i != rows.size(); ++i)
	{
		for (j = rows.at(i); j != 0; --j)
		{
			fprintf(f, "%llu 0\n", j);
			fprintf(f, "%llu %llu\n", j, i+1);
			fprintf(f, "0 %llu\n", i+1);
		}
	}
	/*
	fprintf(f, "%llu 0\n", rows.at(corners.begin()->first));
	for (it = corners.begin(); it != corners.end(); ++it)
		fprintf(f, "%llu %llu\n", rows.at(it->first), it->first + 1);
	--it;
	fprintf(f, "0 %llu\n", it->first + 1);
	*/
	fclose(f);
}

void Diagram::print_corners_for_gnuplot_contour_only(const char* data_fname)
{
	FILE* f;
	mi::iterator it;
	uint64 row, col, prev;

	f = fopen(data_fname, "w");

	//???
	//fprintf(f, "%llu 0\n0 0\n0 %llu\n0 0\n", rows.at(0), cols.at(0));

	prev = 0;

	for (it = points.begin(); it != points.end(); ++it)
	{
		row = it->first;

		if (row >= rows.size())
			col = 0;
		else
			col = rows.at(row);

		fprintf(f, "%llu %llu ", col, row);

		if (it == points.begin())
			fprintf(f, "0\n");
		else
			fprintf(f, "%llu\n", (prev-col)*2);

		prev = col;
	}
	////////////////////fprintf(f, "0 %llu", cols.at(0));
	fclose(f);
}

void Diagram::print_to_gif(bool contour, const char* gifname, uint64 maxX, uint64 maxY, const char* added, const char* removed)
{
	FILE* f;
	double scaled_size, scaled_size2;

	if (contour)
		print_corners_for_gnuplot_contour_only("data.dat");
	else
	{
		//print_corners_for_gnuplot_all_boxes("data.dat");
		print_corners_for_gnuplot_skewed("data.dat");
	}

	if (maxX == 0)
		maxX = rows.at(0)+1;
	if (maxY == 0)
		maxY = cols.at(0)+1;

	f = fopen("tmp_gnuplot.plt", "w");

	fprintf(f, "set term gif\nset nokey\n\n");

	//if (!contour)
		fprintf(f, "set xrange [0:%llu]\nset yrange [0:%llu]\n", maxX, maxY);
	//fprintf(f, "set xrange [0:10]\nset yrange [0:30]\n");

	//fprintf(f, "set xtics 1 offset 2,0.3 font \"Times, 10\"\nset ytics 1 offset 0.6, 0.8 font \"Times, 10\"\nset xtics 0,1,%llu-1\nset ytics 0,1,%llu-1\n", maxX, maxY);
	fprintf(f, "set size ratio -1\n");
	//fprintf(f, "set lmargin 0\n");

	fprintf(f, "unset key; unset tics; unset border;\n");
	fprintf(f, "set output '%s'\n", gifname);

	//scaled_size = 13.5/sqrt(n);
	//scaled_size2 = 10/sqrt(n);

	//scaled_size = 1.5;
	scaled_size = 3.2;

	scaled_size2 = 1;

	if (strcmp(added,""))
	{
		fprintf(f,"plot '%s' u ($1*%f):($2*%f) w points pointtype 5 pointsize %f lc rgb 'blue'", added, scaled_size2, scaled_size2, scaled_size);
		if (strcmp(removed,""))
			fprintf(f,", '%s' u ($1*%f):($2*%f) w points pointtype 3 pointsize %f lc rgb 'red'", removed, scaled_size2, scaled_size2, scaled_size);
		fprintf(f, ", '%s' u ($1*%f):($2*%f) w fsteps linetype -1", "data.dat", scaled_size2, scaled_size2);
	}
	else
		//fprintf(f, "plot '%s' u 1:2 w steps linetype -1", "data.dat");
		fprintf(f, "plot '%s' u ($1*%f):($2*%f) w fsteps linetype -1", "data.dat", scaled_size2, scaled_size2);
	fclose(f);

	if (system("gnuplot tmp_gnuplot.plt"))
		f = NULL;

	//system("rm tmp_gnuplot.plt data.dat");
}

void Diagram::print_to_eps(const char* epsname)
{
	FILE* f;
	uint64 maxX, maxY;

	print_corners_for_gnuplot_all_boxes("data.dat");

	maxX = rows.at(0)+1;
	maxY = cols.at(0)+1;
	//maxX = 12;
	//maxY = 12;


	f = fopen("tmp_gnuplot.plt", "w");

	fprintf(f, "set term postscript eps enhanced\nset nokey\n\nX = %llu\nY = %llu\n\n", maxX, maxY);
	fprintf(f, "set xrange [0:X]\nset yrange [0:Y]\nset xtics 1 offset 2,0.3 font \"Times, 10\"\nset ytics 1 offset 0.6, 0.8 font \"Times, 10\"\nset xtics 0,1,X-1\nset ytics 0,1,Y-1\nset size ratio -1\n");

	fprintf(f, "unset key; unset tics; unset border\n");

	fprintf(f, "set output '%s'\nplot '%s' u 1:2 w steps lc 'black'", epsname, "data.dat");
	fclose(f);

	if (system("gnuplot tmp_gnuplot.plt"))
		f = NULL;

	//system("rm tmp_gnuplot.plt data.dat");
}

void Diagram::calc_planch_limit()
/*
Calculates the plancherel limit curve
*/
{
	const int NUM_OF_POINTS = 10000;

	int i;
	double x, y, X, Y;

	for (i = -NUM_OF_POINTS / 2; i <= NUM_OF_POINTS / 2; ++i)
	{
		X = i * 1.0 / (NUM_OF_POINTS / 2);
		Y = (2.0/M_PI) * (X * asin(X) + sqrt(1-X*X));
		x = X + Y;
		y = Y - X;
		planch_limit[x] = y;	
	}
}

mi Diagram::get_planch_limit()
/*
Returns plancherel limit curve
*/
{
	mdd::iterator it;
	mi res;
	double sqrtn, x, y, x_prev, y_prev;
	uint64 i;

	sqrtn = sqrt(n);

	if (planch_limit.size() == 0)
		calc_planch_limit();

	i = 1;

	x_prev = planch_limit.begin()->first;
	y_prev = planch_limit.begin()->second;

	res[0] = y_prev * sqrtn;
	for (it = planch_limit.begin(), ++it; it != planch_limit.end(); ++it)
	{
		x = it->first * sqrtn;
		y = it->second * sqrtn;

		if (x > i)
		{
			if (i*1.0 - x_prev < x - i*1.0)
				res[i] = y_prev;
			else
				res[i] = y;
			++i;
		}

		x_prev = x; y_prev = y;
	}

	return res;
}

double Diagram::calc_planch_stadev()
/*
Calculates the standard deviation between cols and plancherel curve
*/
{
	mi::iterator it;
	//mdd::iterator mdd_it, mdd_it_prev;
	double res, diff; //x1, y1, x2, y2
	uint64 x, y;
	mi limit_curve;

	limit_curve = get_planch_limit();

	res = 0;
	for (it = corners.begin(); it != corners.end(); ++it)
	{
		y = it->first;
		x = get_width(y) - 1;

		diff = limit_curve.at(x) - y * 1.0;
		res += diff * diff;

		/*
		mdd_it = limit_curve.lower_bound(x);
		if (mdd_it == limit_curve.begin())
			res += (mdd_it->second - y) * (mdd_it->second - y);	
		else
			if (mdd_it == limit_curve.end())
			{
				--mdd_it;
				res += (mdd_it->second - y) * (mdd_it->second - y);
			}
			else
			{
				x1 = abs(mdd_it->first - x);
				y1 = mdd_it->second - y;
				--mdd_it;
				x2 = abs(mdd_it->first - x);
				y2 = mdd_it->second - y;

				if (x1 < x2)
					res += y1 * y1;
				else
					res += y2 * y2;
			}
		*/
	}

	return sqrt(res / (corners.size() - 1));
}

double Diagram::calc_planch_maxdiff()
//
//Calculates the max.diff between cols and plancherel curve
//
{
	mi::iterator it;
	//mdd::iterator mdd_it, mdd_it_prev;
	double diff, maxdiff;//, x1, y1, x2, y2;
	uint64 x, y;
	mi limit_curve;

	limit_curve = get_planch_limit();

	//for (auto it = planch_limit.begin(); it != planch_limit.end(); ++it)
	//	cout << it->first << " " << it->second << endl;
	//exit(1);

	maxdiff = 0;
	for (it = corners.begin(); it != corners.end(); ++it)
	{
		y = it->first;
		x = get_width(y) - 1;

		diff = abs(limit_curve.at(x) - y * 1.0);
		if (diff > maxdiff)
			maxdiff = diff;

/*
		mdd_it = limit_curve.lower_bound(x);
		if (mdd_it == limit_curve.begin())
		{
			diff = abs(mdd_it->second - y);
			if (diff > maxdiff)
			{
				//cout << x << " " << maxdiff << " " << mdd_it->second << " " << y << endl;
				maxdiff = diff;
			}
		}
		else
			if (mdd_it == limit_curve.end())
			{
				--mdd_it;
				diff = abs(mdd_it->second - y);
				if (diff > maxdiff)
				{
					//cout << x << " " << maxdiff << " " << mdd_it->second << " " << y << endl;
					maxdiff = diff;
				}
			}
			else
			{
				x1 = abs(mdd_it->first - x);
				y1 = abs(mdd_it->second - y);
				--mdd_it;
				x2 = abs(mdd_it->first - x);
				y2 = abs(mdd_it->second - y);

				if (x1 < x2)
				{
					if (y1 > maxdiff)
					{
						//cout << x << " " << maxdiff << " " << mdd_it->second << " " << y << endl;
						maxdiff = y1;
					}
				}
				else
				{
					if (y2 > maxdiff)
					{
						//cout << x << " " << maxdiff << " " << mdd_it->second << " " << y << endl;
						maxdiff = y2;
					}
				}
			}
*/
	}

	return maxdiff;
}

void Diagram::print_to_png(const char* FNAME_PNG, bool verbose, const int TYPE, const pu SIZE, const int NOTATION, mpp* TAB)
/*
 *    == NO CFG ==
 *
 * TYPE = 0  noboxes
 *        1  boxes
 *        2  skewed
 *        3  points (Ferrier)
 * NOTATION = 0 French
 *            1 English
 *            2 Russian
 */
{
	const bool TAB_COLORS = false; //true - use colors to depict tableau
	const bool PRINT_LABELS = true;
	const bool PRINT_SCHUTZENBERGER_PATHS_ONLY = false;
	const bool PRINT_PLANCH_LIMITSHAPE = false;

	const char* BGROUND = "#ffffff"; //"#dddddd";//"#e6efe6";
	const bool ADD_FAKE_POINT = true; //add point at coordinate (SIZE, SIZE) (so, the image sizes after cropping are the same).
	//const char* BGROUND = "#ffffff";
	const char* fname_gnuplot = "tmp_gnuplot.plt";
	const char* fname_coords = "tmp_coords.txt";
	const char* fname_planchlimitshape = "tmp_planchlimit.txt";

	const int TAB_LABEL_SIZE = 20; //size of numbers inside boxes

	const int MIN_COLOR = 0;
	const int MAX_COLOR = 255;

	fstream f;
	ofstream f_planch;
	uint64 width, height, size, i, j, x1, y1, x2, y2, box_type;//, tab_num;
	int color;
	double x, y, min_prob, max_prob, x_label, sqrtn;
	mi::iterator it;
	char str_col3[3];
	char str_col7[7];
	//mdd planch_lim;
	mdd::iterator mdd_it;

	sqrtn = sqrt(n);

	if (TAB != NULL)
	{
		if (TAB->size() != n)
		{
			cout << "Diagram::print_to_png(): incorrect table!\n";
			cout << "TAB->size() = " << TAB->size() << ", n = " << n << endl;
			exit(1);
		}
		//tab_num = 0;
	}
	width = get_width();
	height = get_height();
	size = max(max(width, height), max(SIZE.first, SIZE.second)) + 1;

	if (PRINT_PLANCH_LIMITSHAPE)
	{
		if (planch_limit.size() == 0)
			calc_planch_limit();
		//planch_lim = get_planch_limit();
		f_planch.open(fname_planchlimitshape);
		for (mdd_it = planch_limit.begin(); mdd_it != planch_limit.end(); ++mdd_it)
		{
			x = mdd_it->first; 
			y = mdd_it->second;
			f_planch << fixed << setprecision(20) << x * sqrtn << " " << y * sqrtn << endl;			
		}
		f_planch.close();
/*
		f_planch.open(fname_planchlimitshape);
		for (int i = -100; i <= 100; ++i)
		{
			X = i/100.0;
			Y = (2.0/M_PI) * (X * asin(X) + sqrt(1-X*X));
			//x = sqrt(n) * (X+Y);
			//y = sqrt(n) * (Y-X);
			x = sqrt(n) * (X + Y);
			y = sqrt(n) * (Y - X);
			f_planch << x << " " << y << endl;
		}
		f_planch.close();
*/
	}

	f.open(fname_gnuplot, fstream::out | fstream::trunc);
	f << "set term pngcairo background rgb \"" << BGROUND << "\" notransparent size 1000,1000\nset nokey\n\n";

	f << "set output '" << FNAME_PNG << "'\n";
	//f << "set lmargin 0 \n";
	//f << "set lmargin 0 \n set rmargin 0 \n set tmargin 0 \n set bmargin 0 \n";

	f << "set xrange [-0.05:" << size << "]\nset yrange [-0.05:" << size << "]\n";
	f << "set size ratio -1\n";
	f << "unset key; unset tics; unset border;\n";



	if (TYPE == 0)
	{
		print_corners_for_gnuplot_contour_only(fname_coords);
		f << "pl '" << fname_coords << "' u 1:2:3 w boxes fill solid fc rgb \"#CCCCDD\", '' u 1:2 w fs lc -1 lt 1, \"<echo '" << width << " 0\\n0 " << height << "'\" w steps lt 1 lc -1";

		if (PRINT_PLANCH_LIMITSHAPE)
			f << ", '" << fname_planchlimitshape << "' u 1:2 w l lc rgb \"red\" lw 1 not";
		f << endl;
	}
	else
	{
		if (TYPE == 3)
			f << "pl ";
		for (i = 0; i < rows.size(); ++i)
			for (j = 0; j < rows.at(i); ++j)
			{
				TYPE == 2 ? y1 = i + j : y1 = i;
				x1 = j;
				x2 = x1 + 1; y2 = y1 + 1;

				if (TYPE == 1) //if boxes
				{
					if (TAB != NULL)
					{
						//if ((TAB->at(pu(j, i)).second == 1) && PRINT_SCHUTZENBERGER_PATH)
						//{
						//	TAB->at(pu(j, i)).second = 0;
						//	//f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"#000000\" fillstyle pattern 18 lw 0 front\n";//" fc rgb \"#ffbbbb\" lw 3 front\n";
						//	f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"#000000\" fillstyle solid lw 0 front\n";//" fc rgb \"#ffbbbb\" lw 3 front\n";
						//}
						//else
						//{
							if (TAB_COLORS)
								//f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"" << hsv2rgb(TAB->at(pu(j, i)).first % 360) << "\" lw 1 back\n";
								f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"" << hsv2rgb(TAB->at(pu(j, i)).first * 340.0 / n) << "\" fs noborder back\n";
							else
							{
								if ((TAB->at(pu(j, i)).first == 0) && PRINT_LABELS)
								{
									memcpy( &str_col7, &BGROUND[1], 6*sizeof(char));
								}
								else {
								box_type = TAB->at(pu(j, i)).second;
								if (!((box_type != 3) && PRINT_SCHUTZENBERGER_PATHS_ONLY))
								{

									switch(box_type)
									{
										case 1: sprintf(str_col7, "%02x%02x%02x", 0, 0, 255); break;
										case 2: sprintf(str_col7, "%02x%02x%02x", 255, 255, 255); break;
										case 3: sprintf(str_col7, "%02x%02x%02x", 0, 0, 0); break;
										case 4: sprintf(str_col7, "%02x%02x%02x", 255, 0, 0); break;
										case 5: sprintf(str_col7, "%02x%02x%02x", 0, 255, 0); break;
										default: cout << "Diagram.print_to_png(): INCORRECT BOX TYPE.\n"; exit(1);
									}

									f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"#" << str_col7 << "\" lw 1 back\n";
								}}
							}
						//}
					}
					else
						f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"white\" lw 1 back\n";
				}
				else
					if (TYPE == 3)
					{
						f << "'<echo " << x1 << " " << y1 << "' w p pt 7 ps 1 lc -1";
						if (!((j == rows.at(i)) && (i + 1 == rows.size())))
							f << ", ";
					}

				if (((TYPE == 1) || (TYPE == 2)) && (TAB != NULL) && PRINT_LABELS && TAB->at(pu(j, i)).first != 0)
				{
/*
					if (TAB->at(pu(j, i)).first < 10)
						x_label = x1 + 0.35;
					else
						if (TAB->at(pu(j, i)).first < 100)
							x_label = x1 + 0.1;
						else
							if (TAB->at(pu(j, i)).first < 1000)
								x_label = x1;//+ 0.25;
					f << "set label \"" << TAB->at(pu(j, i)).first << "\" at " << x_label << "," << y1 + 0.5 << "font \"Arial," << TAB_LABEL_SIZE << "\" front\n";
*/
					f << "set label \"" << TAB->at(pu(j, i)).first << "\" at " << x1+0.5 << "," << y1 + 0.5 << " center font \"Arial," << TAB_LABEL_SIZE << "\" front\n";
					//++tab_num;
				}
			}
	}
	if (verbose)
	{
		calc_plansh_probs();
		max_prob = max_pl_prob;
		min_prob = max_prob;
		for (it = points.begin(); it != points.end(); ++it)
			if (it->second < min_prob)
				min_prob = it->second;

		for (it = points.begin(); it != points.end(); ++it)
		{
			y = it->first;
			y == height ? x = 0 : x = rows.at(it->first);

			if (TYPE == 2)
				y += x;
			if (min_prob != max_prob)
			{
				color = (it->second - min_prob)*(MAX_COLOR-MIN_COLOR)/(max_prob-min_prob)+MIN_COLOR;
				color = MAX_COLOR - color;
			}
			else
				color = MAX_COLOR;

			sprintf(str_col3, "%02x", MAX_COLOR - color);


			f << "set object rect from " << x+0.2 << "," << y+0.2 << " to " << x+0.8 << "," << y+0.8 << " fc rgb \"#ff" << str_col3 << str_col3 << "\" front\n";

		}
	}
	if (TYPE == 0)
	{
		f << "set output '" << FNAME_PNG << "'\n";
		f << "repl;" << endl << endl;
	}
	else
	{
		if (ADD_FAKE_POINT)
		{
			if (TYPE != 3)
				f << "pl ";
			f << "\"<echo " << SIZE.first+0.05 << " " << SIZE.second+0.05 <<"\" w d lc rgb \"#e6efe5\", \"<echo -0.05 -0.05\" w d lc rgb \"#e6efe5\"";
		}
		else
			f << "plot [][0:" << size << "] 1/0 t\"\"";
		if (PRINT_PLANCH_LIMITSHAPE)
			f << ", '" << fname_planchlimitshape << "' u 1:2 w l lc rgb \"red\" lw 1 not";
		f << "\n";
	}
	f.close();
	if (system((string("gnuplot ")+ string(fname_gnuplot)).c_str() ));

	if (NOTATION == 1)
	{
		if (system((string("convert ") + FNAME_PNG + string(" -rotate 90 ") + FNAME_PNG).c_str()));
	}
	else
		if (NOTATION == 2)
		{
			if (system((string("convert -background \"") + BGROUND + "\" " + FNAME_PNG + string(" -rotate -45 ") + FNAME_PNG).c_str()));
		}

	if (system((string("convert ") + FNAME_PNG + string(" -trim ") + FNAME_PNG).c_str()));
}

void Diagram::print_hooks_to_png(const char* FNAME_PNG)
{
	mpp hooks;

	uint64 x, y;

	for (y = 0; y < rows.size(); ++y)
		for (x = 0; x < rows.at(y); ++x)
			hooks[pu(x, y)] = pu(get_hook(x, y), 0);

	print_to_png(FNAME_PNG, false, 1, pu(0, 0), 0, &hooks);
}

void Diagram::print_to_png(cfg& CFG)
{
	const bool SAME_TC = true;

	const int SX = 2000;
	const int SY = 2000;

	fstream f;
	ofstream f_planch;
	uint64 width, height, size, i, j, x1, y1, x2, y2, box_type, labelsize;
	int color;
	double x, y, min_prob, max_prob, x_label, sqrtn;
	mi::iterator it;
	char str_col3[3];
	//char str_col7[7];
	mdd::iterator mdd_it;
	string tc;
	mppd tabD, oldtabD;


	sqrtn = sqrt(n);

	if ((CFG.rectLW == 3) && (CFG.imgext == "eps"))
		CFG.rectLW = 1;

	if (CFG.tab != NULL)
	{
		if (CFG.tab->size() != n)
		{
			cout << "Diagram::print_to_png(): incorrect table!\n";
			cout << "CFG.tab->size() = " << CFG.tab->size() << ", n = " << n << endl;
			exit(1);
		}
	}
	width = get_width();
	height = get_height();
	size = max(max(width, height), max(CFG.size.first, CFG.size.second)) + 1;

	if (CFG.print_planch_limitshape)
	{
		if (planch_limit.size() == 0)
			calc_planch_limit();
		//planch_lim = get_planch_limit();
		f_planch.open(CFG.fname_planchlimitshape);
		for (mdd_it = planch_limit.begin(); mdd_it != planch_limit.end(); ++mdd_it)
		{
			x = mdd_it->first; 
			y = mdd_it->second;
			f_planch << fixed << setprecision(20) << x * sqrtn << " " << y * sqrtn << endl;			
		}
		f_planch.close();
	}
	f.open(CFG.fname_gnuplot, fstream::out | fstream::trunc);

	if (CFG.imgext == "png")
		f << "set term pngcairo background rgb \"#" << CFG.bground << "\" size " << SX << "," << SY << " notransparent \nset nokey\n\n";
	else
		f << "set term postscript eps color enhanced background rgb \"#" << CFG.bground << "\"\nset nokey\n\n";

	f << "set output '" << CFG.fname_img << "'\n";
	f << "set xrange [-0.05:" << size << "]\nset yrange [-0.05:" << size << "]\n";
	f << "set size ratio -1\n";
	f << "unset key; unset tics; unset border;\n";

	if ((CFG.show_RSK_paths) || (CFG.type == 4))
	{
		if (CFG.tabD != NULL)
		{
			tabD = *(CFG.tabD);
			oldtabD = *(CFG.tabD); //BUG!!! NEED TO ADD oldtabD parameter to Diagram.h!!!
		}
		else
		{
			//mpp => mppd
			mpp2mppd(CFG.tab, &tabD);
			mpp2mppd(CFG.oldtab, &oldtabD);
		}

	        if (CFG.RSKcol)
        		f << calc_coloured_RSK_paths_for_gnuplot(CFG.RSKLW, tabD);
                else
		        f << calc_RSK_paths_for_gnuplot(CFG.RSKLW, tabD, &oldtabD);

	        //if (CFG.RSKcol)
        	//	f << calc_coloured_RSK_paths_for_gnuplot(CFG.RSKLW, *CFG.tab);
                //else
		//        f << calc_RSK_paths_for_gnuplot(CFG.RSKLW, *CFG.tab, CFG.oldtab);
        }
	if (CFG.type == 0)
	{
		print_corners_for_gnuplot_contour_only(CFG.fname_coords);
		f << "pl '" << CFG.fname_coords << "' u 1:2:3 w boxes fill solid fc rgb \"#CCCCDD\", '' u 1:2 w fs lc -1 lt 1, \"<echo '" << width << " 0\\n0 " << height << "'\" w steps lt 1 lc -1";

		if (CFG.print_planch_limitshape)
		{	if (CFG.imgext == "png")
				f << ", '" << CFG.fname_planchlimitshape << "' u 1:2 w l lc rgb \"red\" lw " << CFG.planchLW << " not";
			else
			        //eps, dashed line:
                                //f << ", '" << CFG.fname_planchlimitshape << "' u 1:2 w l lc -1 lw " << CFG.planchLW << " dt 2 not";
				//eps, solid line:
				f << ", '" << CFG.fname_planchlimitshape << "' u 1:2 w l lc -1 lw " << CFG.planchLW << " not";
				
		}
		f << endl;
	}
	else
	{
		if (CFG.type == 3)
			f << "pl ";
		for (i = 0; i < rows.size(); ++i)
			for (j = 0; j < rows.at(i); ++j)
			{
				CFG.type == 2 ? y1 = i + j : y1 = i;
				x1 = j;
				x2 = x1 + 1; y2 = y1 + 1;

				if ((CFG.type == 1) || (CFG.type == 2)) //if boxes
				{
					if (CFG.tab != NULL)
					{
						//if ((TAB->at(pu(j, i)).second == 1) && PRINT_SCHUTZENBERGER_PATH)
						//{
						//	TAB->at(pu(j, i)).second = 0;
						//	//f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"#000000\" fillstyle pattern 18 lw 0 front\n";//" fc rgb \"#ffbbbb\" lw 3 front\n";
						//	f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"#000000\" fillstyle solid lw 0 front\n";//" fc rgb \"#ffbbbb\" lw 3 front\n";
						//}
						//else
						//{
							if (CFG.tab_colors)
								//f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"" << hsv2rgb(TAB->at(pu(j, i)).first % 360) << "\" lw 1 back\n";
								f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"" << hsv2rgb(CFG.tab->at(pu(j, i)).first * 340.0 / n) << "\" fs noborder back\n";
							else
							{
								//if ((CFG.tab->at(pu(j, i)).first == 0) && CFG.print_labels)
								//{
								//	//memcpy( &str_col7, &CFG.bground[1], 6*sizeof(char));
								//}
								//else 
								//{
									box_type = CFG.tab->at(pu(j, i)).second;
									f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"#" << CFG.colors.at(box_type)/*str_col7*/ << "\" lw " << CFG.rectLW << " back\n";
								//}
							}
					}
					else
						f << "set object rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fc rgb \"white\" lw " << CFG.rectLW << " back\n";
				}
				else
					if (CFG.type == 3)
					{
						f << "'<echo " << x1 << " " << y1 << "' w p pt 7 ps 1 lc -1";
						if (!((j == rows.at(i)) && (i + 1 == rows.size())))
							f << ", ";
					}

				if (((CFG.type == 1) || (CFG.type == 2)) && (CFG.tab != NULL) && CFG.print_labels && CFG.tab->at(pu(j, i)).first != 0)
				{
					if (CFG.tab->at(pu(j, i)).first < 10)
						x_label = x1;// + 0.35;
					else
						if (CFG.tab->at(pu(j, i)).first < 100)
							x_label = x1;// + 0.1;
						else
							if (CFG.tab->at(pu(j, i)).first < 1000)
								x_label = x1;//+ 0.25;
					x_label = x_label + 0.5;
					
					//if (SAME_TC || (CFG.tab->at(pu(j, i)).second == 2))
					//	tc = "black";
					//else
					//	tc = "white";
					
					tc = invert_rgb(CFG.colors.at(CFG.tab->at(pu(j, i)).second));

//for (auto it = CFG.colors.begin(); it != CFG.colors.end(); ++it)
//	cout << *it << endl;
//exit(1);

					//cout << i << " " << j << " " << CFG.tab->at(pu(j, i)).second << " " << CFG.colors.at(CFG.tab->at(pu(j, i)).second) << " " << tc << endl;

					if (CFG.tabD == NULL)
					{
						if (CFG.tab_label_size == 0)
						{
							if (CFG.imgext == "png")
								labelsize = 700./size;
							else
								labelsize = 300./size;
						}
						else labelsize = CFG.tab_label_size;
						f << "set label \"" << CFG.tab->at(pu(j, i)).first << "\" at " << x_label << "," << y1 + 0.5 << " center font \"Arial," << labelsize << "\" front tc rgb \"#" << tc << "\"\n";
					}
					else
						f << fixed << setprecision(3) << "set label \"" << CFG.tabD->at(pu(j, i)).first << "\" at " << x_label << "," << y1 + 0.5 << " center font \"Arial," << CFG.tab_label_size << "\" front tc rgb \"#" << tc << "\"\n";

				}
			}
	}
	if (CFG.verbose)
	{
		calc_plansh_probs();
		max_prob = max_pl_prob;
		min_prob = max_prob;
		for (it = points.begin(); it != points.end(); ++it)
			if (it->second < min_prob)
				min_prob = it->second;

		for (it = points.begin(); it != points.end(); ++it)
		{
			y = it->first;
			y == height ? x = 0 : x = rows.at(it->first);

			if (CFG.type == 2)
				y += x;
			if (min_prob != max_prob)
			{
				color = (it->second - min_prob)*(CFG.max_color-CFG.min_color)/(max_prob-min_prob)+CFG.min_color;
				color = CFG.max_color - color;
			}
			else
				color = CFG.max_color;

			sprintf(str_col3, "%02x", CFG.max_color - color);

			f << "set object rect from " << x+0.2 << "," << y+0.2 << " to " << x+0.8 << "," << y+0.8 << " fc rgb \"#ff" << str_col3 << str_col3 << "\" front\n";

		}
	}
	if (CFG.type == 0)
	{
		f << "set output '" << CFG.fname_img << "'\n";
		f << "repl;" << endl << endl;
	}
	else
	{
		if (CFG.add_fake_point)
		{
			if (CFG.type != 3)
				f << "pl ";
			f << "\"<echo " << CFG.size.first+0.05 << " " << CFG.size.second+0.05 <<"\" w d lc rgb \"#e6efe5\", \"<echo -0.05 -0.05\" w d lc rgb \"#e6efe5\"";
		}
		else
			f << "plot [][0:" << size << "] 1/0 t\"\"";
		if (CFG.print_planch_limitshape)
		{
			if (CFG.imgext == "png")
                                f << ", '" << CFG.fname_planchlimitshape << "' u 1:2 w l lc rgb \"red\" lw " << CFG.planchLW << " not";
                        else
 			        //eps, dashed line:
                                //f << ", '" << CFG.fname_planchlimitshape << "' u 1:2 w l lc -1 lw " << CFG.planchLW << " dt 2 not";
				//eps, solid line:
				f << ", '" << CFG.fname_planchlimitshape << "' u 1:2 w l lc -1 lw " << CFG.planchLW << " not";
			
		}
		if (CFG.print_rich_limitshape)
		{
			if (CFG.imgext == "png")
                                f << ", (((6**0.25)-sqrt(x/" << sqrt(n) << "))*((6**0.25)-sqrt(x/" << sqrt(n) << ")))*" << sqrt(n) << " w l lc rgb \"green\" lw " << CFG.planchLW << " not";
                        else
				f << ", (((6**0.25)-sqrt(x/" << sqrt(n) << "))*((6**0.25)-sqrt(x/" << sqrt(n) << ")))*" << sqrt(n) << " w l lc -1 lw " << CFG.planchLW << " dt 3 not";
		
		}
		f << "\n";
	}
	f.close();
	
	if (system((string("gnuplot-x11 ")+ string(CFG.fname_gnuplot)).c_str() ));
	if (CFG.imgext != "eps")
	{
		if (CFG.notation == 1)
		{
			if (system((string("convert ") + CFG.fname_img + string(" -rotate 90 ") + CFG.fname_img).c_str()));
		}
		else
			if (CFG.notation == 2)
			{
				if (system((string("convert -background \"#") + CFG.bground + "\" " + CFG.fname_img + string(" -rotate -45 ") + CFG.fname_img).c_str()));
			}

		if (system((string("convert ") + CFG.fname_img + string(" -trim ") + CFG.fname_img).c_str()));
	}
	else
	{
		//cout << nedodel...CFG.fname_img << endl;
		//cout << ...CFG.fname_img.replace(CFG.fname_img.length()-3,e,"pdf") << endl;
		//exit(1);
		if (system((string("./fixbb ") + CFG.fname_img).c_str() ));
		//if (system(string("epstopdf ") + CFG.fname_img).c_str());;
		//if (system(string("pdftops -eps  ") + CFG.fname_img).c_str());
		//if (system(string("rm ") + ...));
	}
}

void Diagram::print_to_png_OLD(const char* FNAME_PNG, bool verbose, const int TYPE, const uint64 SIZE)

{
	const char* fname_coords = "tmp_coords.txt";
	const char* fname_gnuplot = "tmp_gnuplot.plt";

	const int MIN_COLOR = 0;
	const int MAX_COLOR = 255;

	fstream f;
	uint64 width, height, size;
	int color;
	double x, y, min_prob, max_prob;
	mi::iterator it;
	char str_col[3];

	switch(TYPE)
	{
		//case 0: case 1: print_corners_for_gnuplot_contour_only(fname_coords); break;
		//case 1: print_corners_for_gnuplot_all_boxes(fname_coords); break;
		//case 2: print_corners_for_gnuplot_skewed(fname_coords); break;
	}


	width = get_width();
	height = get_height();
	size = max(max(width, height), SIZE) + 1;

	f.open(fname_gnuplot, fstream::out | fstream::trunc);

	f << "set term pngcairo \nset nokey\n\n";
	f << "set output '" << FNAME_PNG << "'\n";
	f << "set xrange [0:" << size << "]\nset yrange [0:" << size << "]\n";
	f << "set size ratio -1\n";

	f << "unset key; unset tics; unset border;\n";
	//f << "set palette defined (1 '" << COL1 << "', 2 '" << COL2 << "')\nunset colorbox\n";

	f << "pl '" << fname_coords << "' u 1:2:3 w boxes fill solid fc rgb \"#CCCCDD\", '' u 1:2 w fs lc -1 lt 1, \"<echo '" << width << " 0\\n0 " << height << "'\" w steps lt 1 lc -1" << endl;
	if (verbose)
	{
		calc_plansh_probs();
		max_prob = max_pl_prob;
		min_prob = max_prob;
		for (it = points.begin(); it != points.end(); ++it)
			if (it->second < min_prob)
				min_prob = it->second;

		//f << ", \"<echo '";
		for (it = points.begin(); it != points.end(); ++it)
		{
			y = it->first;
			if (y == height)
				x = 0;
			else
				x = rows.at(it->first);
			x += 0.5;
			y += 0.5;

			if (min_prob != max_prob)
			{
				color = (it->second - min_prob)*(MAX_COLOR-MIN_COLOR)/(max_prob-min_prob)+MIN_COLOR;
				color = MAX_COLOR - color;
			}
			else
				color = MAX_COLOR;

			f << "set object rect fc rgb \"#ff";

			sprintf(str_col, "%x", color);

			if (str_col[1]==0)
				f << '0';

			f << str_col;

			f << "00\" fs solid 1.0 lw ";
			if (it->first == max_pl_line)
				f << "1 ";
			else
				f << "0 ";

			f << "center " << x << "," << y << " size 0.8,0.8 front\n";
			//f << "from " << x << "-0.5," << y << "-0.5 to " << x << "+0.5, " << y << "+0.5 front\n";
			//f << "from 0," << y << "-0.5 to " << x << "-0.5, " << y << "+0.5 front\n";

			//f << x << " " << y << " " << color << "\\n";
			if (it->first == max_pl_line)
				f << "set object rect fc rgb \"magenta\" fs solid 1.0 lw 0 center " << x << "," << y << " size 0.4,0.4 front\n";

		}
		//f << "'\" u 1:2:3 w p pt 5 ps 1 palette";

		//if (max_pl_line == height)
		//	max_col = 0;
		//else
		//	max_col = rows.at(max_pl_line);
		//f << ", \"<echo '" << max_col+0.5 << " " << max_pl_line+0.5 << "\\n'\" w p pt 2 ps 3 lc rgb \"green\" lw 4";
	}
	//f << endl;

	f << "set output '" << FNAME_PNG << "'\n";
	f << "repl;" << endl << endl;
	f.close();
	if (system((string("gnuplot ")+ string(fname_gnuplot)).c_str() ));
}

void Diagram::print_verbose()
{
	cout << "n = " << n << endl;
	cout << "Rows: ";
	for (di::iterator it = rows.begin(); it != rows.end(); ++it)
	{
		cout << *it << " ";
	}

	cout << "\nCols: ";
	for (di::iterator it = cols.begin(); it != cols.end(); ++it)
	{
		cout << *it << " ";
	}

	cout << "\nPoints: ";
	for (mi::iterator it = points.begin(); it != points.end(); ++it)
	{
		cout << it->first << " ";
	}

	cout << "\nCorners: ";
	for (mi::iterator it = corners.begin(); it != corners.end(); ++it)
	{
		cout << it->first << " ";
	}
	
	cout << endl;
	/*
	cout << "Table:" << endl;
	for (vi::iterator it = table.begin() + 2; it != table.end(); ++it)
		cout << *it << " ";
	cout << endl;

	cout << "Prob.sums:" << endl;
	for (vd::iterator it = prob_sums.begin(); it != prob_sums.end(); ++it)
		cout << *it << " ";
	cout << endl;
	*/
}

void Diagram::print_points_cute()
{
	mi::iterator it;
	for (it = points.begin(); it != points.end(); ++it)
	{
		if (it->first == rows.size())
			cout << "0.5 " << it->first+0.5 << endl;
		else
		cout << rows.at(it->first)+0.5 << " " << it->first+0.5 << endl;
	}
	cout << endl;
}

void Diagram::print_points(const char* FNAME)
{
	ofstream f;

	if (FNAME != NULL)
		f.open(FNAME);
	mi::iterator it;
	for (it = points.begin(); it != points.end(); ++it)
	{
		if (FNAME == NULL)
			cout << get_width(it->first) << " " << it->first << " " << fixed << setprecision(20) << it->second << endl;
		else
			f << get_width(it->first) << " " << it->first << " " << fixed << setprecision(20) << it->second << endl;
		//cout << it->first << " ";
		//cout << it->first << " " << it->second * sqrt(n) << endl;
		//cout << (int)it->first - (int)get_width(it->first) << " " << it->second * sqrt(n) << endl;
	}
	if (FNAME == NULL)
		cout << endl;
	else
	{
		f << endl;
		f.close();
	}

}

//void Diagram::print_max_pl(ostream &output)
void Diagram::print_max_pl(FILE* f)
{
	if (f != NULL)
		fprintf(f, "(%llu, %.3f)", max_pl_line, max_pl_prob);
}

double Diagram::calc_alpha(int x, int y)
/*
 * BUG: nepravilnoe nach.priblizhenie ALPHA!!!!!
 *
 *
 */
{
	const double EPS = 0.000001;

	double a, a_prev, c, x_new;//, A, B;
	mi::iterator it;
	map<double, uint64>::iterator last;

	x++; y++;

	//initial approximation of alpha:
	c = y*1.0/x;

	x_new = c / (1 - 2.0/M_PI);
/*
	A = (M_PI - 2) * c * c + (4 - 2 * M_PI) * c + M_PI - 2;
	B = - 2 * M_PI * (1 + c);
	if (A != 0)
		x_new = (-B + sqrt(B*B-32*A))/(2*A);
	else
		x_new = -8/B;
*/
	if (x != 1)
		a = x_new / ((x-1) / sqrt(n));
	else
		a = .5;
	cout << "x: " << x << ", y: " << y << endl << a << endl;
	//The estimation of a
	do
	{

		a_prev = a;

		cout << a << " " << abs(0.5*a*(x-y)) << endl;

		if (abs(0.5*a*(x-y)) > 1)
		{
			a = a - 2 * (0.5 * a * (x+y) - 0.5 * abs(a * (x-y))) / (-(a*(x-y)*(x-y))/abs(a*(x-y)) + x + y);
			//cout << "first " << a << endl;
		}
		else
		{
			a = a - 2 * (0.5 * a * (x+y) - 2*(sqrt(1-0.25*a*a*(x-y)*(x-y)) + 0.5*a*(x-y)*asin(0.5*a*(x-y)))/M_PI ) / (-2*(x-y)*asin(0.5*a*(x-y))/M_PI + x + y);
			//cout << "second " << a << endl;
		}
		//cout << a_prev << " " << a << endl;

	}
	while (abs(a_prev - a) > EPS);

	//cout << endl;

	//cout << std::fixed << std::setprecision(20) << a << endl;
//exit(1);
	return a;
}


int Diagram::add_with_probsum(uint64 line, bool calculate_alpha)
{
//cout << n << " " << line << " " << get_width(line) << " ";
	add_prob_sum(calc_point_plansh_prob(line));
//cout << endl;
	return add(line, calculate_alpha);
}


int Diagram::add(uint64 line, bool calculate_alpha)
/*
	������� ���������� ������ �������� � ���������.
	line - ����� ������, ���� ���������.
*/
{
	/*
		NUZHNO ZAMENIT POINTS NA ALPHA (dop.struktura)
	*/
	if (points.find(line) == points.end())
	{
		cout << "ERROR: Diagram::add, incorrect line" << endl;
		exit(1);
		return 1;
	}

	//Update points & corners:
	if (points.find(line + 1) == points.end())
	{
		if (calculate_alpha)
			points[line + 1] = calc_alpha(get_width(line + 1), line + 1);
		else
			points[line + 1];
	}
	corners[line];


	if (line == rows.size())
	{
		if (rows.at(rows.size() - 1) == 1)
		{
			points.erase(line);
			corners.erase(line - 1);
		}
		else
			if (calculate_alpha)
				points[line] = calc_alpha(get_width(line) + 1, line);
	}
	else
	{
		if ((line > 0) && (rows.at(line - 1) == rows.at(line) + 1))
		{
			points.erase(line);
			corners.erase(line - 1);
		}
		else
		{
			//cout << points[line] << endl;

			if (calculate_alpha)
				points[line] = calc_alpha(get_width(line) + 1, line);

			//cout << points[line] << endl;
			//exit(1);
		}
	}

	//Update rows:
	if (line == rows.size())
		rows.push_back(1);
	else
		rows.at(line)++;

	//Update cols:
	if (line == 0)
		cols.push_back(1);
	else
		cols.at(rows.at(line)-1)++;

	last_added = line;
	++n;

	return 0;
}

int Diagram::add_max()
{
	calc_plansh_probs();
	add(max_pl_line);
	return max_pl_line;
}


int Diagram::add_max_with_probsum()
{
//cout << std::fixed << std::setprecision(20) << max_pl_prob << endl;
	add_prob_sum(max_pl_prob);
	add(max_pl_line);
	return max_pl_line;
}

void Diagram::add_max_blowup()
/*
 * adds a box to match the limit shape...
 */
{
/*
	map<double, uint64> as;
	int x, y;
	mi::iterator it;
	map<double, uint64>::iterator last;

	for (it = points.begin(); it != points.end(); ++it)
	{
		y = it->first;
		x = get_width(y);
		as[calc_alpha(x, y)] = y;
	}
*/

	//for (auto itt = as.begin(); itt != as.end(); ++itt)
	//	cout << itt->second << " " << itt->first << endl;
	//cout << endl;


	//last = as.end();
	//--last;

	//y = last->second;
	//x = get_width(y);


	//cout << n << " " << x << " " << y << " " << std::fixed << std::setprecision(20) << calc_point_plansh_prob(y) << " " << get_c() << endl;
	//cout << std::fixed << std::setprecision(20) << get_c() - calc_c_slow() << endl;


	//add_with_probsum(y, true);

	int y;
	y = std::max_element(points.begin(), points.end(),
	    [](const pair<uint64, double>& p1, const pair<uint64, double>& p2) {
	        return p1.second < p2.second; })->first;

	add_with_probsum(y, true);
}

int Diagram::add_nth(uint64 number)
/*
 Добавляет в диаграмму numberную точку с наибольшей вероятностью (не самая макс., а number с конца)
 */
{
	uint64 row_num;

	row_num = calc_plansh_probs(number);
	add_prob_sum(points.at(row_num));
	return add(row_num);
}

double Diagram::calc_corner_plansh_prob(uint64 line)
// Удаляем уголок line, считаем вероятность его добавления
{
	Diagram* diag_tmp;
	double res;

	if (n == 1)
		return 1;

	diag_tmp = new Diagram();
	diag_tmp->init(this);
	diag_tmp->remove(line, -2);

	//получаем вероятность данного уголка:
	res =  diag_tmp->calc_point_plansh_prob(line);

	delete diag_tmp;

	return res;
}

int Diagram::remove_min()
{
	double prob, min_prob;
	//Diagram* diag_tmp;
	uint64 min_corner;
	mi::iterator it;

	//diag_tmp = new Diagram();
	min_prob = 1;
	min_corner = corners.begin()->first;

	//cout << "corners_plansh_probs:" << endl;

	//Пробегаем по всем уголкам, сравниваем вероятности:
	for (it = corners.begin(); it != corners.end(); ++it)
	{
		//получаем вероятность данного уголка:
		prob = calc_corner_plansh_prob(it->first);

		//cout << prob << " ";

		it->second = prob;

		if (prob < min_prob)
		{
			min_prob = prob;
			min_corner = it->first;
		}
	}
	//cout << endl << "min.prob: " << min_prob << " " << min_corner << endl;
	//delete diag_tmp;
	return remove(min_corner, min_prob);
}

/*
void Diagram::remove_prob_sum(double psum)
{
	norm_dim -= log(psum * sqrt(n));
	prob_sums.pop_back();
}
*/

int Diagram::remove(uint64 line, double prob)
{
	if (n == 1)
	{
		cout << "ERROR: Diagram::remove, could not remove, diagram size is equal to 1!";
		return 1;
	}

	if (corners.find(line) == corners.end())
	{
		cout << "\nERROR: Diagram::remove, incorrect line: " << line << endl;
		cout << "diag:" << endl;
		print();
		cout << endl;
		return 1;
	}


	if (prob != -2)
	{
		if (prob == -1)
			prob = calc_corner_plansh_prob(line);
		//cout << "norm_dim -= log(" << prob << "*" << sqrt(n) << ") ["<< log(prob*sqrt(n)) << "]" << endl;
		//cout << "remove " << prob << endl;
		norm_dim -= log(prob * sqrt(n));
		//if (prob_sums.size() > 0)
		//	prob_sums.pop_back();
	}



	//Update points & corners:
	points[line];
	if (line > 0)
		corners[line - 1];

	if (line == rows.size() - 1) //���� ������� �� �������� ����
	{
		if (rows.at(line) == 1) //���� ��� ��������� ������
		{
			points.erase(line + 1);
			corners.erase(line);
		}
	}
	else
	{
		if (rows.at(line + 1) + 1 == rows.at(line)) //���� ����������� ����� � ������� �����
		{
			points.erase(line + 1);
			corners.erase(line);
		}
	}




	//Update cols:
	if (line == 0)
		cols.pop_back();
	else
		cols.at(rows.at(line) - 1)--;

	//Update rows:
	if ((line == rows.size() - 1) && (rows.at(line) == 1))
		rows.pop_back();
	else
		rows.at(line)--;

	n--;

	return 0;
	
}

void Diagram::print_rat_pprobs()
{
	uint64 x, y, i;
	string num, den;

	for (auto it = points.begin(); it != points.end(); ++it)
	{
		num.clear(); den.clear();
		y = it->first; x = get_width(y);

		for (i = 0; i < x; ++i)
		{
			num += to_string(get_hook(i, y)) + "*";
			den += to_string(get_hook(i, y)+1) + "*";			
		}

		for (i = 0; i < y; ++i)
		{
			num += to_string(get_hook(x, i)) + "*";
			den += to_string(get_hook(x, i)+1) + "*";
		}
		num[num.length() - 1] = ')'; den[den.length() - 1] = ')';
		cout << "(" + to_string(x) + "," + to_string(y) + "): (" << num << "/(" << den << " " << calc_point_plansh_prob(y) << endl;
	}		


}

void Diagram::print_to_file(char* fname, streambuf* osbuf)
{
	ofstream f;

	if (osbuf != nullptr)
	{
		ostream os(osbuf);		
		os << n << endl;
		for (di::iterator it = rows.begin(); it != rows.end(); ++it)
			os << *it << " ";
		os << endl;	
		for (di::iterator it = cols.begin(); it != cols.end(); ++it)
			os << *it << " "; 
		os << endl;
		for (mi::iterator it = corners.begin(); it != corners.end(); ++it)
			os << it->first << " ";
		os << endl;
		for (mi::iterator it = points.begin(); it != points.end(); ++it)
			os << it->first << " ";
		os << endl;
		os << get_c() << endl << endl;
	}
	else
	{
		f.open(fname, ofstream::out | ofstream::ate | ofstream::app);
		for (di::iterator it = rows.begin(); it != rows.end(); ++it)
			f << *it << " ";
		f << endl;
		f.close();
	}
}

void Diagram::print_rows_to_file(FILE* f)
{
	di::iterator it;

	for (it = rows.begin(); it != rows.end(); ++it)
		fprintf(f, "%llu ", *it);
	fprintf(f, "\n");

}

mi* Diagram::get_points()
{
	return(&points);
}

mi* Diagram::get_corners()
{
	return(&corners);
}

uint64 Diagram::get_height()
{
	return rows.size();
}

uint64 Diagram::get_height(uint64 col)
{
	//cout << "A\n";
	if (col >= cols.size())
	{
		//cout << "B\n";
		return 0;
	}
	else
	{
		//cout << "C\n";
		//print_cols();
		//cout << col << endl;
		return cols.at(col);
	}
}

uint64 Diagram::get_width()
{
	return cols.size();
}

uint64 Diagram::get_width(uint64 line)
/*
	���������� ����� �� ����������� � ��������� (� ������ line)
*/
{
	if (line >= rows.size())
		return 0;
	else
		return rows.at(line);
}

double Diagram:: get_corners_div_diameter()
//Вычисляет "поперечник", т.е. максимальный x=y в диаграмме (это не совсем поперечник, т.к. катет, а не гипотенуза)
//Возвращает количество угловых клеток (даже тех, к-х нельзя отрезать!) / поперечник
{
	uint64 i, diam, corner_boxes;

	for (diam = 0; diam < rows.size(); ++diam)
		if (diam+1 > rows.at(diam))
			break;

	corner_boxes = 1;
	for (i = 1; i < cols.size(); ++i)
		if (cols.at(i) != cols.at(i-1))
			++corner_boxes;

	return corner_boxes*1./diam;
}

uint64 Diagram::get_hook(uint64 x, uint64 y)
/*
	����� ����� �������� (x,y)
*/
{
	return rows.at(y) + cols.at(x) - x - y - 1;
}

uint64 Diagram::get_hook_slow(uint64 x, uint64 y)
{
	di::iterator it;
	uint64 res;
	uint64 width = get_width(y);

	res = width - x;

	it = upper_bound(rows.begin() + y + 1, rows.end(), x + 1, comp);
	return res + distance(rows.begin() + y + 1, it);
}

uint64 Diagram::get_size()
{
	return n;
}

void Diagram::pop_bottom()
/*
!! ������������ ������ �� Enumeration. ������ ��������� �����������!!! (�� �������������� cols, points, n.
*/
{
	rows.pop_front();
}

void Diagram::pop(uint64 line)
/*
!! ������������ ������ �� Tester. ������� ��������� �����������!!! (�� �������������� cols, points, n.
������ ��� ����� ������������ remove
*/
{
	//���� ������ �� �������� �� ������������ ������
    //��� ���� �������� �� �������� ������� CORNERS
	if (line >= rows.size())
	{
		cout << "Error: Diagram::pop";
		//system("pause");
		exit(EXIT_FAILURE);
	}

	//���� ��� �������� ������ ����� ������
	if (line < rows.size() - 1)
	{
		if (rows.at(line + 1) == rows.at(line))
		{
			cout << "Error: Diagram::pop";
			//system("pause");
			exit(EXIT_FAILURE);
		}
	}

	if (rows.at(line) == 1)
	{
		if (line == rows.size())
			rows.pop_back();
		else
		{
			cout << "Error: Diagram::pop";
			//system("pause");
			exit(EXIT_FAILURE);
		}
	}
	else
		rows.at(line)--;
}

di* Diagram::get_rows()
{
	return &rows;
}

di* Diagram::get_cols()
{
	return &cols;
}

void Diagram::get_rows(di r)
{
	//di::iterator it;

	r.assign(rows.begin(), rows.end());
	//for (it = r.begin(); it != r.end(); ++it)
	//	cout << *it << " ";
}

bool Diagram::is_flat()
{
	return rows.size() == 1;
}

void Diagram::set_rows(di r)
{
	rows = r;
	n = rows.size();
}

void Diagram::set_to_last(uint64 n_, uint64 k_)
{
	uint64 low;

	rows.clear();
	n = n_;

	low = min(n_, k_);

	rows.push_back(low);
	rows.insert(rows.end(), n_ - low, 1);
}

int Diagram::rows_correctness()
{
	uint64 width, next_width, depth;

	for (depth = 0; depth < rows.size(); ++depth)
	{
		width = rows.at(depth);

		if (depth != rows.size() - 1)
			next_width = rows.at(depth + 1);
		else next_width = width;

		if (next_width > width)
		{
			cout << "Diagram::check_correctness(): depth = " << depth << endl;
			return 1;
		}
	}
	//cout << "The diagram is correct." << endl;
	return 0;
}

int Diagram::rowcol_correctness()
//Проверяется корректность между rows и cols
{
	di cols_bkp;
	int res;

	cols_bkp = cols;
	calc_cols_from_rows();

	if (cols != cols_bkp)
		res = 1;
	else
		res = 0;

	cols = cols_bkp;

	return res;
}

void Diagram::add_prob_sum(double psum)
{
//cout << std::fixed << std::setprecision(20) << psum;
	norm_dim += log(psum * sqrt(n+1));
/*
	if (prob_sums.size() == 0)
		prob_sums.push_back(-log(psum));
	else
		prob_sums.push_back(-log(psum) + prob_sums.at(prob_sums.size() - 1));
*/
}

void Diagram::get_legal_points(Diagram* target, mi* leg_p)
//�������� �� ��������� ������ �����, ������� ����� �������� � target � ���������� �� � leg_p.
{
	mi::iterator it;
	uint64 x, y;

	//cout << "Points before deletion:" << endl;
	//print_points();

	//double tmpsum = 0;
	//for (mi::iterator it2 = points.begin(); it2 != points.end(); ++it2)
	//	tmpsum = tmpsum + it2->second;
	//cout << "SUM: " << tmpsum << endl;


	leg_p->clear();
	for (it = points.begin(); it != points.end(); ++it)
	{
		y = it->first;
		if (y == rows.size())
			x = 0;
		else
			x = rows.at(it->first);
		if (target->consists_of(x, y))
			(*leg_p)[y] = it->second;
	}

	//cout << "p" << points.size() << "leg" << leg_p->size() << " ";

	//printf("p%luleg%2lu ", points.size(), leg_p->size());

	//cout << "Points after deletion:" << endl;
	//print_points();
}

void Diagram::remove_illegal_points(Diagram& target)
{
	mi leg_p;
	get_legal_points(&target, &leg_p);
	points = leg_p;
}

void Diagram::remove_row0_point()
{
	points.erase(0);
}

void Diagram::add_row0_point()
{
	points[0];
}

bool Diagram::sum_is_greater_than(Diagram* t)
{
	//const double EPSILON = 0.0000001;

	//cout << "prob_sums.size(): " << prob_sums.size() << endl;
	//cout << "t->prob_sums.size(): " << t->prob_sums.size() << endl;
	//cout << prob_sums.size() - 1 << endl;
	
	//cout << endl <<  "..." << endl;
	//cout << prob_sums.at(prob_sums.size() - 1) << endl;
	//cout << t->prob_sums.at(prob_sums.size() - 1) << endl;
	//cout << t->prob_sums.at(prob_sums.size() - 1) + EPSILON << endl;
	//cout << "..." << endl;
/*
	if (prob_sums.at(prob_sums.size() - 1) > t->prob_sums.at(prob_sums.size() - 1) + EPSILON)
	{
		//cout << "[" << prob_sums.at(prob_sums.size() - 1) << " > " << t->prob_sums.at(prob_sums.size() - 1) << "] ";
		printf("[%.2lf > %.2lf] ", prob_sums.at(prob_sums.size() - 1), t->prob_sums.at(prob_sums.size() - 1));
		return 1;
	}
	else return 0;
*/
	return 0;///////
}

//double Diagram::get_prob_sum()
//{
//	return prob_sums.at(prob_sums.size() - 1);
//}

bool Diagram::is_equal_to(Diagram* comp)
{
	if (rows != comp->rows)
		return 0;
	return 1;
}

bool Diagram::is_equal(Diagram comp)
{
	if (rows != comp.rows)
		return 0;
	return 1;
}

bool Diagram::is_equal_to_cols(di c)
{
	if (cols != c)
		return 0;
	return 1;
}

/*
double Diagram::get_c_bug()
{

	//cout << "get_c: num/dem" << endl;

	mpz_t num, den;
	mpq_t ln_content;
	uint64 i, j;
	double res;

	mpq_init(ln_content);

	mpz_init_set_ui(numerator, n);

	//gmp_printf("%.Ff\n", numerator);

	for (i = n - 1; i > 1; --i)
	{
		 mpz_mul_ui(numerator, numerator, i);
	}
	//cout << "n!: ";
	//gmp_printf("%Zd\n", numerator);
	mpz_sqrt(numerator, numerator);
	//cout << "\nsqrt(n!): ";
	//gmp_printf("%Zd\n", numerator);
	//cout << endl;


	mpz_init_set_ui(denominator, 1);
	for (i = 0; i != rows.size(); ++i)
		for (j = 0; j != rows.at(i); ++j)
			mpz_mul_ui(denominator, denominator, get_hook(j, i));

	//cout << "Hook(den): ";
	//gmp_printf("%Zd\n", denominator);
	//mpz_div_exact(dim, numerator, denominator);

	mpq_set_num(ln_content, numerator);
	mpq_set_den(ln_content, denominator);

	//cout << "ln (";
	//gmp_printf("%Qd", ln_content);
	//cout << ") = " << LogE(ln_content) << endl;

	//cout << LogE(ln_content) << " * (-1.) / " << sqrt(n) << " = ";
	res = LogE(ln_content) * (-1.) / sqrt(n);

	//cout << res << endl << endl;

	mpz_clear(numerator);
	mpz_clear(denominator);
	mpq_clear(ln_content);

	return res;
}
*/
double Diagram::get_c()
{
	//cout << "get_c: normdim = " << norm_dim << ", n = " << n << ", sqrt(n) = " << sqrt(n) << endl;
	//cout << "res: " << -norm_dim / sqrt(n) << endl;
	return -norm_dim / sqrt(n);
}

double Diagram::calc_c_slow()
{
	Diagram diag;
	uint64 x, y;

	for (x = 1; x < rows.at(0); ++x)
		diag.add_with_probsum(0);

	for (y = 1; y < rows.size(); ++y)
		for (x = 0; x < rows.at(y); ++x)
			diag.add_with_probsum(y);

	return diag.get_c();
}

/*
double Diagram::get_c_bad()
//��������� ��������������� �����������
//Bug: fact(n) may be too big for uint64.
{
	double nom, dim, res;
	uint64 den, row, col;

	////////////////////////////////////////////////nom = sqrt(fact(n));
	den = 1;
	for (row = 0; row != rows.size(); ++row)
		for (col = 0; col != rows.at(row); ++col)
			den *= get_hook(col, row);
	dim = nom / den; //dim - ��� �� ������ �����������, ��-�� sqrt (��.�������)

	cout << nom << " / " << den << " = " << dim << " " << log(dim) << endl;

	res = -2. * log(dim) / sqrt(n);

	return res;
}
*/

void Diagram::calc_cols_from_rows()
//�� rows �������� cols
{
	di::iterator it;
	uint64 i;

	cols.clear();
	for (i = 0; i < rows.at(0); ++i)
	{
		cols.push_back(1);
		for (it = rows.begin() + 1; (it != rows.end()) && (*it > i); ++it)
			cols.at(i)++;
	}
}

void Diagram::calc_cols_from_rows(di d_rows)
//�� rows �������� cols
{
	di::iterator it;
	uint64 i;

	cols.clear();
	for (i = 0; i < d_rows.at(0); ++i)
	{
		cols.push_back(1);
		for (it = d_rows.begin() + 1; (it != d_rows.end()) && (*it > i); ++it)
			cols.at(i)++;
	}
}

void Diagram::calc_cols_from_rows(Diagram diag)
//�� rows �������� cols
{
	di::iterator it;
	uint64 i;

	diag.cols.clear();
	for (i = 0; i < diag.rows.at(0); ++i)
	{
		diag.cols.push_back(1);
		for (it = diag.rows.begin() + 1; (it != diag.rows.end()) && (*it > i); ++it)
			diag.cols.at(i)++;
	}
}

void Diagram::calc_rows_from_cols()
{
	di::iterator it;
	uint64 i, j, prev_col, new_col;

	rows.clear();

	prev_col = cols.back();

	for (i = 0; i < prev_col; ++i)
		rows.push_back(cols.size());

	new_col = 0;
	for (i = cols.size()-1; i != 0; --i)
	{
		new_col = cols[i];
		if (new_col != prev_col)
		{
			for (j = 1; j <= new_col - prev_col; ++j)
				rows.push_back(i+1);
			prev_col = new_col;
		}
	}

	if (cols.size() > 1)
	{
		for (i = 0; i < cols[0] - new_col; ++i)
			rows.push_back(1);
	}
}

void Diagram::calc_rows_from_cols(di d_cols)
{
	di::iterator it;
	uint64 i, j, prev_col, new_col;

	rows.clear();

	prev_col = d_cols.back();

	for (i = 0; i < prev_col; ++i)
		rows.push_back(d_cols.size());

	new_col = 0;
	for (i = d_cols.size()-1; i != 0; --i)
	{
		new_col = d_cols[i];
		if (new_col != prev_col)
		{
			for (j = 1; j <= new_col - prev_col; ++j)
				rows.push_back(i+1);
			prev_col = new_col;
		}
	}

	if (d_cols.size() > 1)
	{
		for (i = 0; i < d_cols[0] - new_col; ++i)
			rows.push_back(1);
	}
}

double Diagram::hook_prod_ratio(Diagram* diag2)
//����������� ��������� ������������ ���� ������ ���� ������ ������ ���������
//� ������������ ������������ � diag2
{
	uint64 row1, col1, row2, col2, i;
	double res;

	if (n != diag2->n)
	{
		cout << "ERROR: Diagram::hook_prod_ratio" << endl;
		//system("pause");
		exit(EXIT_FAILURE);
	}

	res = 1;
	row1 = 0; col1 = 0; row2 = 0; col2 = 0;
	i = 1;
	do
	{
		res *= get_hook(col1, row1) * 1. / diag2->get_hook(col2, row2);
		if (++col1 == rows.at(row1))
		{
			++row1; col1 = 0;
		}
		if (++col2 == diag2->rows.at(row2))
		{
			++row2; col2 = 0;
		}
		++i;
	} 
	while (i != n);

	return res;
}

void Diagram::calc_size_from_rows()
{
	di::iterator it;
	n = 0;
	for (it = rows.begin(); it != rows.end(); ++it)
		n += *it;
}

bool Diagram::is_symmetric()
{
	return rows == cols;
}

bool Diagram::is_symmetric_to(Diagram* diag2)
{
	return rows == diag2->cols;
}

mpq_class Diagram::calc_exact_point_planch_prob(uint64 y)
{
	mpq_class prob (1);

	int hook_len;
	uint64 i;
	uint64 x;

	x = get_width(y);

	for (i = 0; i < x; ++i)
	{
		hook_len = get_hook(i, y);
		prob *= mpq_class(hook_len, hook_len + 1);
	}

	for (i = 0; i < y; ++i)
	{
		hook_len = get_hook(x, i);
		prob *= mpq_class(hook_len, hook_len + 1);
	}
	
	if (prob > max_pl_prob)
	{
		max_pl_prob = prob.get_d();
		max_pl_line = y;
	}

	return prob;
}


double Diagram::calc_point_plansh_prob(uint64 y)
{
	double prob;
	uint64 hook_len;
	uint64 i;
	uint64 x;

	x = get_width(y);

	prob = 1;

	for (i = 0; i < x; ++i)
	{
		hook_len = get_hook(i, y);
		prob *= hook_len * 1. / (hook_len + 1);
	}
	for (i = 0; i < y; ++i)
	{
		hook_len = get_hook(x, i);
		prob *= hook_len * 1. / (hook_len + 1);
	}

	
	if (prob > max_pl_prob)
	{
		max_pl_prob = prob;
		max_pl_line = y;
	}
	return prob;
}

void Diagram::calc_point_plansh_prob_rat(uint64 y, int* num, int* den)
{
	uint64 hook_len;
	uint64 i;
	uint64 x;

	x = get_width(y);

	*num = 1;
	*den = 1;
	for (i = 0; i < x; ++i)
	{
		hook_len = get_hook(i, y);
		*num *= hook_len;
		*den *= hook_len + 1;
	}
	for (i = 0; i < y; ++i)
	{
		hook_len = get_hook(x, i);
		*num *= hook_len;
		*den *= hook_len + 1;
	}
}

uint64 Diagram::calc_plansh_probs(uint64 num, mi* best_p)
/*
	Calculates Plancherel probabilities.
	Returns the "num"th best point.
*/
{
	mi::iterator it;
	mi p;
	std::multimap<double, uint64> probs;
	std::multimap<double, uint64>::reverse_iterator mit;
	uint64 i;

	if (num > points.size())
	{
		num = points.size();
		//cout << "ERROR: Diagram::calc_plansh_probs()" << endl;
		//exit(1);
	}

	max_pl_prob = 0;

	for (it = points.begin(); it != points.end(); ++it)
	{
		it->second = calc_point_plansh_prob(it->first);
		if (num != 0)
			probs.insert(pair<double, uint64>(it->second, it->first));
	}

	if (num != 0)
	{
		if (best_p != NULL)
		{
			best_p->clear();
			for (i = 0, mit = probs.rbegin(); (mit != probs.rend()) && (i != num); ++mit, ++i)
				(*best_p)[mit->second] = mit->first;
		}

		mit = probs.rbegin();
		advance(mit, num - 1);
		return mit->second;
	}
	else
		return 0;
}

void Diagram::calc_exact_dim(mpz_t dim)
{
	//М.б. заменить numerator, denominator на mpz_q ?
	//Возможно, будет быстрее

	mpz_t numerator, denominator;
	uint64 i, j;

	mpz_init_set_ui(numerator, n);

	//cout << "Numerator:" << endl;

	for (i = n - 1; i > 1; --i)
	{
		 mpz_mul_ui(numerator, numerator, i);
	}

	//cout << endl << "Denominator: " << endl;

	mpz_init_set_ui(denominator, 1);
	for (i = 0; i != rows.size(); ++i)
		for (j = 0; j != rows.at(i); ++j)
		{
			mpz_mul_ui(denominator, denominator, get_hook(j, i));
			//cout << get_hook(j, i) << " ";
		}

	//cout << endl;
	//gmp_printf("%Zd\n%Zd\n", numerator, denominator);
	mpz_divexact(dim, numerator, denominator);
	//mpz_cdiv_q(dim, numerator, denominator);





	mpz_clear(numerator);
	mpz_clear(denominator);

	//cout << "aa" << endl;
    //gmp_printf("%Zd\n", mpz_dim);
	//cout << "aa2" << endl;
	//mpz_clear(mpz_dim);


	//mpz_init_set(mpz_dim, dim);

}

//void Diagram::get_last_dim(mpz_t dim)
//{
//	mpz_set(dim, mpz_dim);
//}
/*
uint64 Diagram::calc_rowcol_diff()
{
	uint64 i;
	uint64 diff;

	diff = 0;
	for (i = 0; i < rows.size(); ++i)
	{
		if (i >= cols.size())
			diff += rows.at(i);
		else
		{
			if (rows.at(i) > cols.at(i))
				diff += rows.at(i) - cols.at(i);
			else
				diff += cols.at(i) - rows.at(i);
		}
	}
	for (; i < cols.size(); ++i)
		diff += cols.at(i);
	return diff / 2;
}
*/
void Diagram::print_to_txt(const char* fname)
{
	FILE* f;
	mi::iterator it;

	f = fopen(fname, "w");
	fprintf(f, "%llu 0\n", rows.at(corners.begin()->first));
	for (it = corners.begin(); it != corners.end(); ++it)
		fprintf(f, "%llu %llu\n", rows.at(it->first), it->first + 1);
	--it;
	fprintf(f, "0 %llu\n", it->first + 1);
	fclose(f);

}

void Diagram::calc_n_min_corners(uint64 num, mi* c)
/*
	Вычисляет num худших уголков в диаграмме, передаёт их в c.
*/

{
	mi::iterator it;
	std::multimap<double, uint64> probs;
	//std::multimap<double, uint64>::reverse_iterator mit;
	std::multimap<double, uint64>::iterator mit;

	uint64 i;

	if (num > corners.size())
		num = corners.size();

	for (it = corners.begin(); it != corners.end(); ++it)
		probs.insert(pair<double, uint64>(calc_corner_plansh_prob(it->first), it->first));

	c->clear();
	//for (i = 0, mit = probs.rbegin(); (mit != probs.rend()) && (i != num); ++mit, ++i)
	for (i = 0, mit = probs.begin(); (mit != probs.end()) && (i != num); ++mit, ++i)
		(*c)[mit->second] = mit->first;
}

double Diagram::comp_dim(Diagram* diag)
/*
 * Сравниваются рамерности данной диаграммы и diag
 *
 * Return: Во сколько раз размерность данной диаграммы больше?
 * */
{
	Diagram cmpdiag; //диаграмма для перехода от диаграммы-пересечения в сравниваемые диаграмым
	di cross_cols;            //Столбцы диаграммы-пересечения
	di* diag_cols;            //столбцы diag
	uint64 i, j;
	map<uint64, uint64> cols_to_add1, cols_to_add2; //клетки, которые надо добавить, чтобы из пересечения попасть в соответствующую диаграмму
	map<uint64, uint64>::iterator it;
	uint64 col, col_diag, rownum, counter;
	double ratio, ratio_temp;
	vd ratios;

        //0. Проверяем диаграммы на предмет симметричности:
        if (rows == *diag->get_cols())
        {
                return 1;
        }

	//1. Получаем диаграмму-пересечение:
	diag_cols = diag->get_cols();
	for (i = 0; (i < cols.size()) && (i < diag_cols->size()); ++i)
	{
		col = cols[i];
		col_diag = (*diag_cols)[i];

		if (col > col_diag)
		{
			cols_to_add1[i] = col - col_diag;
			cross_cols.push_back(col_diag);
		}
		else
			if (col < col_diag)
			{
				cols_to_add2[i] = col_diag - col;
				cross_cols.push_back(col);
			}
			else
				cross_cols.push_back(col);
	}

	for (j = i; j < cols.size(); ++j)
		cols_to_add1[j] = cols[j];
	for (j = i; j < diag_cols->size(); ++j)
		cols_to_add2[j] = (*diag_cols)[j];

	//2. Доходим из cmpdiag в данную диаграмму:
	cmpdiag.init_by_cols(cross_cols);
	for (it = cols_to_add1.begin(); it != cols_to_add1.end(); ++it)
	{
		for (i = 0; i < it->second; ++i)
		{
			rownum = cmpdiag.get_height(it->first);
			ratios.push_back(cmpdiag.calc_point_plansh_prob(rownum));
			cmpdiag.add(rownum);
		}
	}

	if (n < diag->get_size())
		for (i = 0; i < diag->get_size() - n; ++i)
			ratios.push_back(1);

	//3. Доходим из cmpdiag в diag
	ratio = 1;
	counter = 0;
	cmpdiag.init_by_cols(cross_cols);

	for (it = cols_to_add2.begin(); it != cols_to_add2.end(); ++it)
	{
		for (i = 0; i < it->second; ++i)
		{
			rownum = cmpdiag.get_height(it->first);
			ratio_temp = cmpdiag.calc_point_plansh_prob(rownum);

//cout << fixed << setprecision(5) << ratios[counter] << "/" << ratio_temp << "=" << ratios[counter]/ratio_temp << endl;

			ratio *= ratios[counter++] / ratio_temp;
//cout << ratio << endl;
			cmpdiag.add(rownum);
		}
	}
//cout << endl;

	for (; counter < ratios.size(); ++counter)
		ratio *= ratios[counter];
	return ratio;
}

int Diagram::comp_dim_exact(Diagram* diag)
{
	Diagram cmpdiag; //диаграмма для перехода от диаграммы-пересечения в сравниваемые диаграмым
	di cross_cols;            //Столбцы диаграммы-пересечения
	di* diag_cols;            //столбцы diag
	uint64 i, j;
	map<uint64, uint64> cols_to_add1, cols_to_add2; //клетки, которые надо добавить, чтобы из пересечения попасть в соответствующую диаграмму
	map<uint64, uint64>::iterator it;
	uint64 col, col_diag, rownum, counter;
	mpq_class ratio, ratio_temp;
	vector<mpq_class> ratios;

	//0. Проверяем диаграммы на предмет симметричности:
	if (rows == *diag->get_cols())
	{
		return 0;
	}

	//1. Получаем диаграмму-пересечение:
	diag_cols = diag->get_cols();
	for (i = 0; (i < cols.size()) && (i < diag_cols->size()); ++i)
	{
		col = cols[i];
		col_diag = (*diag_cols)[i];

		if (col > col_diag)
		{
			cols_to_add1[i] = col - col_diag;
			cross_cols.push_back(col_diag);
		}
		else
			if (col < col_diag)
			{
				cols_to_add2[i] = col_diag - col;
				cross_cols.push_back(col);
			}
			else
				cross_cols.push_back(col);
	}

	for (j = i; j < cols.size(); ++j)
		cols_to_add1[j] = cols[j];
	for (j = i; j < diag_cols->size(); ++j)
		cols_to_add2[j] = (*diag_cols)[j];

	//2. Доходим из cmpdiag в данную диаграмму:
	cmpdiag.init_by_cols(cross_cols);
	for (it = cols_to_add1.begin(); it != cols_to_add1.end(); ++it)
	{
		for (i = 0; i < it->second; ++i)
		{
			rownum = cmpdiag.get_height(it->first);
			ratios.push_back(cmpdiag.calc_exact_point_planch_prob(rownum));
//cout << ratios.back().get_num() << " " << ratios.back().get_den() << endl;
//cout << rownum << " " << cmpdiag.calc_exact_point_planch_prob(rownum) << " " << cmpdiag.calc_point_plansh_prob(rownum) << endl;

			cmpdiag.add(rownum);
		}
	}

	if (n < diag->get_size())
		for (i = 0; i < diag->get_size() - n; ++i)
			ratios.push_back(1);

	//3. Доходим из cmpdiag в diag
	ratio = 1;
	counter = 0;
	cmpdiag.init_by_cols(cross_cols);

	for (it = cols_to_add2.begin(); it != cols_to_add2.end(); ++it)
	{
		for (i = 0; i < it->second; ++i)
		{
			rownum = cmpdiag.get_height(it->first);
			ratio_temp = cmpdiag.calc_exact_point_planch_prob(rownum);
			ratio *= (ratios[counter++] / ratio_temp);
			//ratio *= ratios[counter++];
			//ratio /= ratio_temp;
			cmpdiag.add(rownum);
		}
	}

	for (; counter < ratios.size(); ++counter)
		ratio *= ratios[counter];

	if (ratio < 1)
		return -1;
	if (ratio > 1)
		return 1;
	if (ratio == 1)
		return 0;
}

int Diagram::comp_dim_slow(Diagram* diag)
{
	mpz_t dim1, dim2;
	int res;

	mpz_init(dim1); mpz_init(dim2);
	calc_exact_dim(dim1);
	diag->calc_exact_dim(dim2);
	res = mpz_cmp(dim1, dim2);	

	mpz_clear(dim1); mpz_clear(dim2);
	return res;
}

void Diagram::calc_max_prob(uint64* line, double* prob)
{
	calc_plansh_probs();
	*line = max_pl_line;
	*prob = max_pl_prob;
}

uint64 Diagram::get_last_added()
{
	return last_added;
}

void Diagram::calc_all_tabs(set<dpu>* tabs, dpu tab, Diagram diag)
{
	mi leg_p;
	mi::iterator it;
	uint64 line;
	//Diagram target;

	if (is_equal_to(&diag))
	{
		tabs->insert(tab);
		return;
	}

	//target.init_by_cols(cols);

	diag.get_legal_points(this, &leg_p);

	for (it = leg_p.begin(); it != leg_p.end(); ++it)
	{
		line = it->first;
		tab.push_back(pu(diag.get_width(line), line));

//diag.print_cols();
//diag.print_verbose();
//cout << "ololo\n";
//cout << line << " " << rows.at(0) << endl;
//for (int iii=0; iii < rows.size(); ++iii)
//cout << rows.at(iii) << " ";
//cout << endl;
//exit(1);

		diag.add(line);
		calc_all_tabs(tabs, tab, diag);
		tab.pop_back();
		diag.remove(line);
	}
}

bool Diagram::consists_of(uint64 x, uint64 y)
{
	if (rows.size() > y) 
		if (rows.at(y) > x) 
			return true;
	return false;
}

string Diagram::calc_RSK_paths_for_gnuplot(const double LW, mppd& tab, mppd* oldtab)
//Output - part of code for gnuplot to draw RSK paths on the diagram
{
	const string COL_OLD = "ff0000";
	const string COL = "00ff00";
	//const int LW_OLD = 1;//3;
	//const int LW = 1;//8;
	const double x_ = 0.5, y_ = 0.5;


	double LW_OLD = LW;
	

	bool oldpath;
	uint64 x, y, y2;
	string res = "", col;
	di oldcols;
	mppd::iterator it;
	mpp tr_old;
	Diagram diag;
	double lw;

	if (oldtab != NULL)
	{
		//calculate oldcols:

		for (it = oldtab->begin(); it != oldtab->end(); ++it)
			if (!(it->first.first == 0 && it->first.second == 0))
				diag.add(it->first.second);
		oldcols = *diag.get_cols();

		//transitions in the old tab
		for (x = 0; x < oldcols.size(); ++x)
		{
			for (y = 0, y2 = 0; y < oldcols.at(x); ++y) 
			{
				if ((x != oldcols.size() - 1) && (y2 != oldcols.at(x+1)))
					while (oldtab->at(pu(x,y)).first > oldtab->at(pu(x+1,y2)).first)
					{
						if (++y2 == oldcols.at(x+1))
							break;
					}
				//Connection (x+x_,y+y_) => (x+1+x_,y2+y_):
				tr_old[pu(x+x_, y+y_)] = pu(x+1+x_, y2+y_);
			}
		}
	}

	for (x = 0; x < cols.size(); ++x)
	{
		for (y = 0, y2 = 0; y < cols.at(x); ++y) 
		{
			if ((x != cols.size() - 1) && (y2 != cols.at(x+1)))
				while (tab.at(pu(x,y)).first > tab.at(pu(x+1,y2)).first)
				{
					if (++y2 == cols.at(x+1))
						break;
				}
			//Connection (x+x_,y+y_) => (x+1+x_,y2+y_):
			oldpath = true;
			if (oldtab == NULL)
				oldpath = true;
			else
			{
				if (tr_old.find(pu(x+x_, y+y_)) == tr_old.end())
					oldpath = false;
				else
				{
					if (tr_old[pu(x+x_, y+y_)] != pu(x+1+x_, y2+y_))
						oldpath = false;
					else
						oldpath = true;
				}
			}

			if (oldpath)
			{
				lw = LW_OLD; col = COL_OLD;
			}
			else
			{
				lw = LW; col = COL;
			}

			res += "set arrow nohead from " + to_string(x+x_) + "," + to_string(y+y_);
			res += " to " + to_string(x+1+x_) + "," + to_string(y2+y_) + " lw " + to_string(lw);
			res += " lc rgb \"#";
			res += col;
			res += "\"\n";
		}
	}

	return res;
}

void Diagram::bypass_tree(map<pu,vpu>& sources, pu& coord, const int color, const double lw, string& res)
{
	const double x_ = 0.5, y_ = 0.5;
	//vector<string> colors = {"#000000", "#e6194b", "#3cb44b", "#ffe119", "#4363d8", "#f58231", "#911eb4", "#46f0f0", "#f032e6", "#bcf60c", "#fabebe", "#008080", "#e6beff", "#9a6324", "#fffac8", "#800000", "#aaffc3", "#808000", "#ffd8b1", "#000075", "#808080"};

	uint64 x, y, i;

	if (sources.find(coord) == sources.end())
		return;

	for (i = 0; i != sources[coord].size(); ++i)
	{
		x = sources[coord].at(i).first; y = sources[coord].at(i).second;

		res += "set arrow nohead from " + to_string(x+x_) + "," + to_string(y+y_);
		res += " to " + to_string(coord.first+x_) + "," + to_string(coord.second+y_) + " lw " + to_string(lw);
		res += " lc " + to_string(color) + "\n";
		//res += " lc rgb \"";
		//res += colors.at(color);
		//res += "\"\n";	

		bypass_tree(sources, sources[coord].at(i), color, lw, res);
	}
}

string Diagram::calc_coloured_RSK_paths_for_gnuplot(const double LW, mppd& tab)
//Output - part of code for gnuplot to draw RSK paths on the diagram
//Different bumping trees are coloured differently.
{
	uint64 x, y, y2;
	string res = "";
	mi::iterator it;
	//string col = "ff0000";
	int col;
	pu coord;

	map<pu,vpu> sources; // first: (x,y), second: vector of sources of bumping routes

	for (x = 0; x < cols.size(); ++x)
	{
		for (y = 0, y2 = 0; y < cols.at(x); ++y) 
		{
			if ((x != cols.size() - 1) && (y2 != cols.at(x+1)))
				while (tab.at(pu(x,y)).first > tab.at(pu(x+1,y2)).first)
				{
					if (++y2 == cols.at(x+1))
						break;
				}
			//connection (x, y) => (x+1, y2)
			sources[pu(x+1,y2)].push_back(pu(x,y));
		}
	}

	for (it = points.begin(), col = 1; it != points.end(); ++it)
	{
		y = it->first; x = get_width(y);
		coord = pu(x,y);

		if (sources.find(coord) == sources.end())
			continue;

                if (col == 7) ++col;
	        if (col == 10) col = 1;
		bypass_tree(sources, coord, col, LW, res);
		++col;
	}

	return res;
}

void Diagram::print_exact_dim()
{
	mpz_t dim;

	mpz_init(dim);
	calc_exact_dim(dim);
	gmp_printf("%Zd\n", dim);
	mpz_clear(dim);
}

bool Diagram::is_strict()
{
        uint64 i;

        for (i = 0; i < cols.size() - 1; ++i)
                if(cols.at(i) <= cols.at(i + 1))
                        return false;

        return true;
}
