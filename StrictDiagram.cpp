#include "StrictDiagram.h"

using namespace std;

mdd StrictDiagram::planch_limit;

StrictDiagram::StrictDiagram(bool use_gmp)
{
	init(use_gmp);
}

StrictDiagram::StrictDiagram(StrictDiagram* copy_from, bool use_gmp)
{
	init(copy_from);
}

StrictDiagram::~StrictDiagram()
{
	if (gmp_initialized)
	{
		mpz_clear(dim);
		mpz_clear(num);
		mpz_clear(den);
		mpq_clear(rat);
		mpq_clear(rat_best);
	}
}

void StrictDiagram::init(StrictDiagram* copy_from, bool use_gmp)
{
	/*
	 * Каждый раз mpz_set - медленно!
	 */
	rows = copy_from->rows;
	cols = copy_from->cols;
	points = copy_from->points;
	corners = copy_from->corners;
	norm_dim = copy_from->norm_dim;
	n = copy_from->n;
	max_pl_prob = copy_from->max_pl_prob;
	max_pl_line = copy_from->max_pl_line;

	if (use_gmp)
	{
		if (!gmp_initialized)
		{
			mpz_init(dim);
			mpz_init(num);
			mpz_init(den);
			mpq_init(rat);
			mpq_init(rat_best);
			gmp_initialized = true;
		}
		mpz_set(dim, copy_from->dim);
	}
}

void StrictDiagram::init(bool use_gmp)
{
	rows.clear(); cols.clear(); corners.clear();  points.clear();
	rows.push_back(1);
	cols.push_back(1);
	points[1] = 2 * sqrt(2); //initial alpha
	corners[0];
	n = 1;
	norm_dim = 1;

	if (use_gmp)
	{
		if (!gmp_initialized)
		{
			mpz_init(dim);
			mpz_init(num);
			mpz_init(den);
			mpq_init(rat);
			mpq_init(rat_best);
			gmp_initialized = true;
		}
		mpz_set_ui(dim, 1);
	}

	max_pl_prob = 0;
}

void StrictDiagram::init_by_cols(di c)
{
	cols = c;
	calc_rows_from_cols();
	calc_size_from_rows();
	calc_pointscorners_from_cols();
}

void StrictDiagram::calc_pointscorners_from_cols()
{

	uint64 i;

	points.clear();
	corners.clear();

	//points:
	points[cols[0]];
	for (i = 1; i < cols.size(); ++i)
		if (cols[i]+1 != cols[i-1])
			points[cols[i]];
	if (cols.back() > 1)
		points[0];

	//corners:
	for (i = 0; i < cols.size()-1; ++i)
		if (cols[i] != cols[i+1]+1)
			corners[cols[i]-1];
	if (n != 1)
		corners[cols.back() - 1];
}

void StrictDiagram::calc_strict_dim(mpz_t dim)
/*
 * Вычисление размерности СТРОГОЙ диаграммы Юнга
 * Формула из статьи Ф.Петрова "Polynomial approach to explicit formulae for generalized binomial coefficients"
 * (стр. 12)
 */
{
	uint64 i, j;
	//mpz_t num, den;

	mpz_set_ui(num, 1);
	mpz_set_ui(den, 1);

	for (i = 0; i < cols.size()-1; ++i)
		for (j = i + 1; j < cols.size(); ++j)
		{
			mpz_mul_ui(num, num, cols.at(i) - cols.at(j));
			mpz_mul_ui(den, den, cols.at(i) + cols.at(j));
		}

	//factorial n:
	for (i = n; i > 1; --i)
		mpz_mul_ui(num, num, i);

	for (i = 0; i < cols.size(); ++i)
		for (j = cols.at(i); j > 1; --j)
			mpz_mul_ui(den, den, j);

	//gmp_printf("%Zd %Zd\n", num, den);

	mpz_divexact(dim, num, den);
	//mpz_clear(num);
	//mpz_clear(den);
}

double StrictDiagram::calc_plansh_prob(uint64 line)
/*
 Статья "Случайные блуждания... (Л. Петров)
 1. Подставляем (2.3) в (2.9) (убрав квадрат!!!)
    Получаем вероятность данного пути в диаграмму
 2. Делим эту формулу для диаграммы размера + 1 на формулу исходной диаграммы
    Многое сокращается.
    В результате:
    1) Изменяемый столбец перемножается со всеми остальными
    2) Если новый столбец не появился, делим на 2
    3) Домножаем на новую длину изменённого столбца
 */
{
	double res;
	uint64 col, i;
	long long H;

	col = get_width(line);

	if (col == cols.size())
		H = 0;
	else
		H = cols.at(col);

	res = 1;
	for (i = 0; i < cols.size(); ++i)
	{
		//cout << i << " " << H << " " << cols.at(i) << " " << H - (long long)cols.at(i) + 1 << " " << H + cols.at(i) << endl;
		if (i != col)
			//res *= ((H - cols.at(i) + 1) * (H + cols.at(i))) / ( (H + cols.at(i) + 1) * (H - cols.at(i)));
			res *= ((H - (long long)cols.at(i) + 1)*1. / (H - (long long)cols.at(i))) * ( (H + cols.at(i))*1. / (H + cols.at(i) + 1));
	}


	if (line != 0)
		res *= 2;

	res /= (H + 1);

	return res;
}


void StrictDiagram::calc_plansh_prob_rat(uint64 line, int* num, int* den)
{
	uint64 col, i;
	long long H;

	col = get_width(line);

	if (col == cols.size())
		H = 0;
	else
		H = cols.at(col);

	*num = 1;
	*den = 1;
	for (i = 0; i < cols.size(); ++i)
	{
		if (i != col)
		{
			*num *= (H - (long long)cols.at(i) + 1) * (H + cols.at(i));
			*den *= (H - (long long)cols.at(i)) * (H + cols.at(i) + 1);
		}
	}


	if (line != 0)
		*num *= 2;

	*den *= (H + 1);

}

double StrictDiagram::calc_plansh_prob_mpz(uint64 line)
/*
 Статья "Случайные блуждания... (Л. Петров)
 1. Подставляем (2.3) в (2.9) (убрав квадрат!!!)
    Получаем вероятность данного пути в диаграмму
 2. Делим эту формулу для диаграммы размера + 1 на формулу исходной диаграммы
    Многое сокращается.
    В результате:
    1) Изменяемый столбец перемножается со всеми остальными
    2) Если новый столбец не появился, делим на 2
    3) Домножаем на новую длину изменённого столбца
 */
{
	double res;
	uint64 col, H, i;
	//mpz_t num, den;
	//mpq_t rat;

	mpz_set_ui(num, 1);
	mpz_set_ui(den, 1);
	//mpq_init(rat);

	col = get_width(line);

	if (col == cols.size())
		H = 0;
	else
		H = cols.at(col);

	for (i = 0; i < col; ++i)
	{
		mpz_mul_ui(num, num, cols.at(i) - H - 1);
		mpz_mul_ui(den, den, cols.at(i) + H + 1);

		mpz_mul_ui(den, den, cols.at(i) - H);
		mpz_mul_ui(num, num, cols.at(i) + H);
	}

	for (i = col + 1; i < cols.size(); ++i)
	{
		mpz_mul_ui(num, num, H - cols.at(i) + 1);
		mpz_mul_ui(den, den, H + cols.at(i) + 1);

		mpz_mul_ui(den, den, H - cols.at(i));
		mpz_mul_ui(num, num, H + cols.at(i));
	}

	if (line != 0)
		mpz_mul_ui(num, num, 2);
	mpz_mul_ui(den, den, H + 1);

	//cout << "Cols:";
	//print_cols();
	//cout << "Line: " << line << endl;
	//gmp_printf("%Zd %Zd\n", num, den);
	//cout << endl;

	mpq_set_num(rat, num);
	mpq_set_den(rat, den);

	res = mpq_get_d(rat);

	//cout << res << endl;

	//mpz_clear(num);
	//mpz_clear(den);
	//mpq_clear(rat);

	return res;
}

double StrictDiagram::calc_plansh_prob_inaccurate(uint64 line)
{
	StrictDiagram* diag2;
	double res;
	double num, den;

	diag2 = new StrictDiagram(this);
	diag2->add(line, 1);

	num = diag2->calc_route_plansh_prob();
	den = calc_route_plansh_prob();

	if (n == 350)
		cout << num << " " << den << endl;

	res = num / den;

	delete diag2;
	return res;
}

double StrictDiagram::calc_route_plansh_prob()
{
	//БАГ: 2^coef - должно быть в знаменателе

	uint64 i, j, coef;
	//mpz_t num, den;
	//mpq_t rat;
	double res;

	//mpz_init(num);
	mpz_set_ui(den, 1);
	//mpq_init(rat);

	coef = n - cols.size();

	mpz_ui_pow_ui(num, 2, coef);

//	gmp_printf("num 2^(n-l): %Zd\n", num);

	for (i = 0; i < cols.size() - 1; ++i)
		for (j = i + 1; j < cols.size(); ++j)
		{
//			cout << i << " " << j << endl;
			mpz_mul_ui(num, num, cols.at(i) - cols.at(j));
			mpz_mul_ui(den, den, cols.at(i) + cols.at(j));
	//		cout << cols.at(i) + cols.at(j) << endl;
		}

//	gmp_printf("V 1-m proizvedenii otnosheniya: num: %Zd, den: %Zd\n", num, den);


	for (i = 0; i < cols.size(); ++i)
		for (j = cols.at(i); j > 1; --j)
			mpz_mul_ui(den, den, j);

//	gmp_printf("Vtoroe proizvedenie, znamenatel den: %Zd\n", den);

	//gmp_printf("%Zd %Zd\n", num, den);

	mpq_set_num(rat, num);
	mpq_set_den(rat, den);

	res = mpq_get_d(rat);

	//mpz_clear(num);
	//mpz_clear(den);
	//mpq_clear(rat);

	return res;
}

double StrictDiagram::calc_plansh_prob_maybe_coprob(uint64 line)
{
	//mpz_t dim1, dim2;
	//mpq_t rat;
	StrictDiagram* diag2;
	double res;

	//mpz_init(dim1);
	//mpz_init(dim2);
	//mpq_init(rat);
	diag2 = new StrictDiagram(this);

	diag2->add(line);

	calc_strict_dim(num);

	diag2->calc_strict_dim(den);

	mpq_set_num(rat, num);
	mpq_set_den(rat, den);

	//gmp_printf("%Zd %Zd\n", dim1, dim2);

	res = mpq_get_d(rat);

	delete diag2;
	//mpz_clear(dim1);
	//mpz_clear(dim2);
	//mpq_clear(rat);
	return res;
}

int StrictDiagram::add(uint64 line, double prob, bool calculate_alpha)
{
	uint64 col, colsize;
	int y;

	if (points.find(line) == points.end())
	{
	/*
		cout << "ERROR: StrictDiagram::add, incorrect line" << endl;
		cout << "size: " << n << endl;
		cout << "rows: ";
		print();
		cout << endl;
		cout << "cols: ";
		print_cols();
		cout << "points: ";
		print_points();
		cout << "line: " << line << endl << endl;
	*/
		return 1;
	}

	if (line == rows.size())
		col = 0;
	else
		col = rows[line];

	if (col == cols.size())
		colsize = 0;
	else
		colsize = cols[col];

	if (col != 0)
	{
		if (col != cols.size())
		{
			if (cols[col] + 1 == cols[col - 1])
			{
				cout << "ERROR: StrictDiagram::add. Adding to this line makes diagram non-strict." << endl;
				//print_cols();
				//cout << "line: " << line << endl;
			}
		}
		else
			if (cols[col - 1] == 1)
			{
				cout << "ERROR: StrictDiagram::add. Adding to this line makes diagram non-strict." << endl;
				//print_cols();
				//cout << "line: " << line << endl;
			}
	}
//
//
//
/*
	if (prob < 0)
	{
		prob = calc_plansh_prob(line);
		//plansh_probs.push_back(prob);
		//calc_dim = true;
	}
*/
//
//
//

	corners[line];
	points.erase(line);

	//Esli kletka - novaya sverhu (stolb)
	if (col == 0)
	{
		//my mozhem dobavit' sverhu nee
		if (calculate_alpha)
		{
			if (points.find(line + 1) == points.end())
				points[line + 1] = calc_alpha(0, line + 1);
		}
		else
			points[line + 1];

		//ubiraem staryy ugolok.
		corners.erase(line - 1);
	}
	else
	{
		//esli dobavlen novyy stolbec
		if (col == cols.size())
		{
			if (cols[col - 1] > 2)
			{
				if (calculate_alpha)
				{
					if (points.find(line + 1) == points.end())
						points[line + 1] = calc_alpha(get_width(line + 1), line + 1);
				}
				else
					points[line + 1];
			}
		}
		else
			if (cols[col] + 2 != cols[col - 1])
			{
				if (calculate_alpha)
				{
					if (points.find(line + 1) == points.end())
						points[line + 1] = calc_alpha(get_width(line + 1), line + 1);
				}
				else
					points[line + 1];
			}

		if ((line != 0) && (rows[line - 1] == col + 1))
			corners.erase(line - 1);

		if (cols[col - 1] == colsize + 2)
			corners.erase(line + 1);
	}

	if (col != cols.size())
	{
		if (col == cols.size() - 1)
			y = 0;
		else
			y = cols[col+1];

		if (calculate_alpha)
		{
			if (points.find(y) == points.end())
				points[y] = calc_alpha(get_width(y), y);
		}
		else
			points[y];
	}

	//Update rows:
	if (line == rows.size())
		rows.push_back(1);
	else
		rows[line]++;

	//Update cols:
	if (line == 0)
		cols.push_back(1);
	else
		cols[rows[line]-1]++;

	n++;

	//table.push_back(line);

	return 0;
}

int StrictDiagram::add_with_probsum(uint64 line)
{
	double lndim;

	lndim = log(calc_dim_ratio_fast(line));
	lndim_sum += lndim - log(n)/2;
	nd = -(lndim_sum + (n/2.) * log(2) ) / sqrt(n);

	return add(line);
}

double StrictDiagram::get_c()
{
	return nd;
}
/*
NOW it is in Diagram.cpp

bool StrictDiagram::is_strict()
{
	uint64 i;

	for (i = 0; i < cols.size() - 1; ++i)
		if(cols.at(i) <= cols.at(i + 1))
			return false;

	return true;
}
*/
void StrictDiagram::calc_max_prob(uint64* l, double* p)
{
	mi::iterator it;
	double prob, max_prob;
	uint64 max_line;

	max_line = 0;
	max_prob = 0;

	//cout << "n=" << n << endl;

	for (it = points.begin(); it != points.end(); ++it)
	{
		prob = calc_plansh_prob(it->first);

		if (prob > max_prob)
		{
			max_prob = prob;
			max_line = it->first;
		}

		//cout << it->first << " " << prob << endl;

	}

	//cout << "max: " << max_prob << endl << endl;

	*p = max_prob;
	*l = max_line;
}

uint64 StrictDiagram::calc_plansh_probs(uint64 num, mi* best_p)
/*
 *  FULL COPY (except the call of call_plansh_prob) OF Diagram::calc_plansh_probs.
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
		it->second = calc_plansh_prob(it->first);
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

double StrictDiagram::calc_dim_ratio(uint64 line, bool no_return)
{
	double res;
	uint64 col, H, i;
	//mpz_t num, den;
	//mpq_t rat;

	mpz_set_ui(num, 1);
	mpz_set_ui(den, 1);
	//mpq_init(rat);

	col = get_width(line);

	if (col == cols.size())
		H = 0;
	else
		H = cols.at(col);

	for (i = 0; i < col; ++i)
	{
		mpz_mul_ui(num, num, cols.at(i) - H - 1);
		mpz_mul_ui(den, den, cols.at(i) + H + 1);

		mpz_mul_ui(den, den, cols.at(i) - H);
		mpz_mul_ui(num, num, cols.at(i) + H);
	}

	for (i = col + 1; i < cols.size(); ++i)
	{
		mpz_mul_ui(num, num, H - cols.at(i) + 1);
		mpz_mul_ui(den, den, H + cols.at(i) + 1);

		mpz_mul_ui(den, den, H - cols.at(i));
		mpz_mul_ui(num, num, H + cols.at(i));
	}

	mpz_mul_ui(num, num, n + 1);
	mpz_mul_ui(den, den, H + 1);

	mpq_set_num(rat, num);
	mpq_set_den(rat, den);

	//gmp_printf("%Zd %Zd\n", num, den);

	if (!no_return)
		res = mpq_get_d(rat);
	else
		res = 1;
	//mpz_clear(num);
	//mpz_clear(den);
	//mpq_clear(rat);

	return res;
}


double StrictDiagram::calc_dim_ratio_fast(uint64 line)
/*
 * Returns dim_new / dim_old
 */
{
	double res;
	uint64 col, i;
	long long H;

	col = get_width(line);

	if (col == cols.size())
		H = 0;
	else
		H = cols[col];

	res = 1;

/*
    for (i = 0; i < col; ++i)
            res *= (   (cols.at(i) - H - 1) * (cols.at(i) + H)  ) * 1. / ( (cols.at(i) + H + 1) * (cols.at(i) - H));

    for (i = col + 1; i < cols.size(); ++i)
            res *= (   (H - cols.at(i) + 1) * (H + cols.at(i)) ) * 1. / ( (H + cols.at(i) + 1) * (H - cols.at(i)));
*/

	for (i = 0; i < cols.size(); ++i)
		if (i != col)
			res *= ((H - (long long)cols[i] + 1)*1. / (H - (long long)cols[i])) * ( (H + cols[i])*1. / (H + cols[i] + 1));

	res *= (n + 1) * 1. / (H + 1);

	return res;
}

vd* StrictDiagram::get_plansh_probs_trace()
{
	return &plansh_probs;
}

int StrictDiagram::remove_min()
{
	double prob, min_prob;
	uint64 min_corner;
	mi::iterator it;
	mi corners_copy;
	//uint64 i, corner_num;

	min_prob = 1;
	min_corner = corners.begin()->first;
	//cout << "corners_plansh_probs:" << endl;

	corners_copy = corners;

	//Пробегаем по всем уголкам, сравниваем вероятности:
	for (it = corners_copy.begin(); it != corners_copy.end(); ++it)
	{
		//получаем вероятность данного уголка:
		prob = calc_corner_plansh_prob(it->first);

		it->second = prob;

		if (prob < min_prob)
		{
			min_prob = prob;
			min_corner = it->first;
		}
	}
	return remove(min_corner, min_prob);
}


void StrictDiagram::remove_min_mpq(uint64 depth)
/*
 * Удалить num клеток с наименьшей разницей размерностей
 */
{
	uint64 min_corner;
	mi::iterator it;
	mi corners_copy;

	if (depth >= n)
		depth = n - 1;

	for (; depth != 0; --depth)
	{
		corners_copy = corners;

		//инициализация rat_best
		it = corners_copy.begin();
		remove(it->first);
		min_corner = it->first;
		calc_dim_ratio(it->first, true);
		mpq_set(rat_best, rat);
		add(it->first);

		++it;

		//Пробегаем по оставшимся уголкам, сравниваем отношения размерностей:
		for (; it != corners_copy.end(); ++it)
		{
			remove(it->first);

			calc_dim_ratio(it->first, true);

			if (mpq_cmp(rat, rat_best) < 0)
			{
				mpq_set(rat_best, rat);
				min_corner = it->first;
			}
			add(it->first);
		}
		remove(min_corner);
	}
}

uint64 StrictDiagram::remove_min_double(uint64 depth)
/*
 * Удалить num клеток с наименьшей разницей размерностей
 */
{
	uint64 min_corner;
	mi::iterator it;
	mi corners_copy;
	double drat_best, drat; //double ratio

	if (depth >= n)
		depth = n - 1;

	min_corner = 0;
	for (; depth != 0; --depth)
	{
		corners_copy = corners;

		//инициализация rat_best
		it = corners_copy.begin();
		remove(it->first);
		min_corner = it->first;
		drat_best = calc_dim_ratio_fast(it->first);
		add(it->first);

		++it;

		//Пробегаем по оставшимся уголкам, сравниваем отношения размерностей:
		for (; it != corners_copy.end(); ++it)
		{
			remove(it->first);

			drat = calc_dim_ratio_fast(it->first);

			if (drat < drat_best)
			{
				drat_best = drat;
				min_corner = it->first;
			}
			add(it->first);
		}
		remove(min_corner);
	}
	return min_corner;
}

int StrictDiagram::remove_min_dim()
/*
 * slow
 */
{
	uint64 min_corner;
	mi::iterator it;
	mi corners_copy;

	min_corner = 0;

	corners_copy = corners;

	mpz_set_ui(dim, 0); //исходная максимальная размерность меньшей диаграммы

	//Пробегаем по всем уголкам, сравниваем размерности:
	for (it = corners_copy.begin(); it != corners_copy.end(); ++it)
	{
		remove(it->first);
		calc_strict_dim(num);
		if (mpz_cmp(num, dim) > 0)
		{
			mpz_set(dim, num);
			min_corner = it->first;
		}
		add(it->first);
	}
	return remove(min_corner);

/*
	uint64 min_corner;
	mi::iterator it;
	mi corners_copy;

	min_corner = 0;

	corners_copy = corners;

	calc_strict_dim(dim); //исходная размерность
	mpz_set_ui(den, 0); //минимальная разница

	//Пробегаем по всем уголкам, сравниваем размерности:
	for (it = corners_copy.begin(); it != corners_copy.end(); ++it)
	{
		//получаем разницу размерностей данного уголка:
		remove(it->first);
		calc_strict_dim(num);

		mpz_sub(num, dim, num); //вычислили, на сколько уменьшится размерность при удалении уголка

		if (mpz_cmp(num, den) <= 0)
		{
			mpz_set(den, num);
			min_corner = it->first;
		}

		add(it->first);
	}
	//cout << endl;

	//cout << endl << "min.prob: " << min_prob << " " << min_corner << endl;
	//delete diag_tmp;
	return remove(min_corner);
*/
}

double StrictDiagram::calc_corner_plansh_prob(uint64 line)
// Удаляем уголок line, считаем вероятность его добавления
{

	double res;

	if (n == 1)
		return 1;

	//cout << "line: " << line << endl;
	//print();
	//print_cols();

	remove(line, -2);
	res =  calc_plansh_prob(line);
	add(line, res);

	return res;


/*
	StrictDiagram* diag_tmp;
	double res;

	if (n == 1)
		return 1;

	diag_tmp = new StrictDiagram();
	diag_tmp->init(this);



	//cout << "line: " << line << endl;
	//diag_tmp->print();
	//diag_tmp->print_cols();



	diag_tmp->remove(line, -2);



	//получаем вероятность данного уголка:
	res =  diag_tmp->calc_plansh_prob(line);

	delete diag_tmp;

	return res;
*/
}

int StrictDiagram::remove(uint64 line, double prob)
{
	uint64 col;

	if (n == 1)
	{
		cout << "ERROR: StrictDiagram::remove, could not remove, diagram size is equal to 1!\n";
		return 1;
	}

	if (corners.find(line) == corners.end())
	{
/*
		cout << "\nERROR: StrictDiagram::remove, incorrect line: " << line << endl;
		cout << "diag:" << endl;
		print(); 		cout << endl;
		print_cols();
*/
		return 1;
	}

	col = rows[line] - 1;

	if (col != cols.size() - 1)
		if (cols[col] == cols[col + 1] + 1)
		{
			//cout << "\nERROR: StrictDiagram::remove, removing this line will make the diagram non-strict." << endl;
			return 1;
		}
//
//
//
/*
	if (prob != -2)
	{
		if (prob == -1)
			prob = calc_corner_plansh_prob(line);
		//cout << "norm_dim -= log(" << prob << "*" << sqrt(n) << ") ["<< log(prob*sqrt(n)) << "]" << endl;
		//cout << "remove " << prob << endl;
		//norm_dim -= log(prob * sqrt(n));
		//prob_sums.pop_back();
	}
*/
//
//
//
	//Update points & corners:
	points[line];

	//esli udalyaem ne iz nizhney strochki
	if (line > 0)
	{
		//esli udalyaem iz pravogo stolbca
		if (col >= cols.size() - 1)
			corners[line - 1];
		else
			if (cols[col] != cols[col + 1] + 2)
				corners[line - 1];
	}

	if (col != 0)
		if (cols[col - 1] == cols[col] + 1)
			corners[line + 1];

	points.erase(line + 1);
	points.erase(line - 1);
	corners.erase(line);

	//Update cols:
	if (line == 0)
		cols.pop_back();
	else
		cols[rows[line] - 1]--;

	//Update rows:
	if ((line == rows.size() - 1) && (rows[line] == 1))
		rows.pop_back();
	else
		rows[line]--;

	n--;

	//table.pop_back();

	return 0;

}

void StrictDiagram::set_cols_and_size(di c, uint64 size)
{
	cols = c;
	n = size;
}

bool StrictDiagram::is_inside(StrictDiagram* bigger_diag)
/*
 * Проверка, что эта диаграмма находится внутри bigger_diag
 */
{
	di* bigger_cols;
	uint64 i;

	bigger_cols = bigger_diag->get_cols();

	//Если в нашей диаграмме больше столбцов, она не вписывается в bigger_diag
	if (cols.size() > bigger_cols->size())
		return false;

	//Если какой-либо из столбцов нашей диаграммы больше соответствующего столбца в bigger_diag, то диаграмма не вписывается
	for (i = 0; i < cols.size(); ++i)
		if (cols.at(i) > bigger_cols->at(i))
			return false;

	return true;
}


int StrictDiagram::add_maxdim_mpq(uint64 num)
{
	/*
	 * num раз добавляется клетка с максимальной размерностью
	 */

	mi::iterator it;
	uint64 max;
	int res;

	res = 1;

	for (uint64 i = 0; i < num; ++i)
	{
		max = 0;
		mpq_set_ui(rat_best, 0, 1);

		//Для текущей диаграммы вычисляем max!
		for (it = points.begin(); it != points.end(); ++it)
		{
			//gmp_printf("%lf\n", calc_dim_ratio(it->first, false)-calc_dim_ratio_fast(it->first));
			calc_dim_ratio(it->first, true);
			if (mpq_cmp(rat, rat_best) > 0)
			{
				mpq_set(rat_best, rat);
				max = it->first;
			}
		}
		print_cols();
		res = add(max);
	}

	return res;
}

int StrictDiagram::add_maxdim_double(uint64 num)
/*
 * num раз добавляется клетка с максимальной размерностью
 */
{
	uint64 maxdr_line = 0;
	double maxdr_prob = 0;

	for (uint64 i = 0; i < num; ++i)
	{
		calc_max_dimratio(&maxdr_line, &maxdr_prob);

		add(maxdr_line);
	}
	return maxdr_line;
}

double StrictDiagram::calc_alpha(int x, int y)
/*
 *  BUG: nepravilnoe nach.priblizhenie ALPHA!!!!!
 */
{
	const double EPS = 0.000001;

	double a, a_prev;

	//x++; y++;

	//initial approximation of alpha:
	a = 0;

	//The estimation of a
	do
	{
		a_prev = a;
		if (abs(y * a) > 2 * sqrt(2))
			a = 0;
		else
			a = sqrt(8 - a*a*y*y) / (y * acos(a*y*1.0/(2 * sqrt(2))) + M_PI * x);
	}
	while (abs(a_prev - a) > EPS);

	return a;
}

void StrictDiagram::add_max_blowup()
/*
 * BUG!!!!!!!!!!!! DOES NOT WORK!!!!!!!!!!!!!!!!!!!!!
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


	//print_rows_ASCII();
	//cout << endl;
	//for (auto it = as_tmp.begin(); it != as_tmp.end(); ++it)
	//{
	//	cout << it->first-1 << " " << it->second << endl;
	//}
	//cout << endl;


//cout << as.begin()->second - 1 << endl;

	//////////////////////////////diag.add(as.begin()->second);
	//add(as.begin()->second - 1);



	last = as.end();
	--last;

	y = last->second;
	x = get_width(y);
	//cout << n << " " << x << " " << y << " " << std::fixed << std::setprecision(20) << calc_plansh_prob(y) << endl;
	add(y);
*/
	int y;
	y = std::max_element(points.begin(), points.end(),
	    [](const pair<uint64, double>& p1, const pair<uint64, double>& p2) {
	        return p1.second < p2.second; })->first;

	add(y, true);

}

void StrictDiagram::calc_max_dimratio(uint64* maxdr_line, double* maxdimrat)
{
	double dimrat;
	mi::iterator it;

	*maxdr_line = 0;
	*maxdimrat = 0;

	//Для текущей диаграммы вычисляем max!
	for (it = points.begin(); it != points.end(); ++it)
	{
		dimrat = calc_dim_ratio_fast(it->first);
		if (dimrat > *maxdimrat)
		{
			*maxdimrat = dimrat;
			*maxdr_line = it->first;
		}
	}

}

double StrictDiagram::comp_dim(StrictDiagram* diag)
/*
 * Сравниваются рамерности данной диаграммы и diag
 *
 * Return: Во сколько раз размерность данной диаграммы больше?
 * */
{
	StrictDiagram cmpdiag; //диаграмма для перехода от диаграммы-пересечения в сравниваемые диаграмым
	di cross_cols;            //Столбцы диаграммы-пересечения
	di* diag_cols;            //столбцы diag
	uint64 i, j;
	map<uint64, uint64> cols_to_add1, cols_to_add2; //клетки, которые надо добавить, чтобы из пересечения попасть в соответствующую диаграмму
	map<uint64, uint64>::iterator it;
	uint64 col, col_diag, rownum, counter;
	double ratio, ratio_temp;
	vd ratios;

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
			ratios.push_back(cmpdiag.calc_dim_ratio_fast(rownum));
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
			ratio_temp = cmpdiag.calc_dim_ratio_fast(rownum);
			ratio *= ratios[counter++] / ratio_temp;
			cmpdiag.add(rownum);
		}
	}

	for (; counter < ratios.size(); ++counter)
		ratio *= ratios[counter];

	return ratio;
}

double StrictDiagram::calc_strict_normdim_slow()
{
	StrictDiagram diag;
	double lndim, lndim_sum;
	long long n;
	uint64 col, row;

	lndim_sum = 0;
	n = 1;
	//cols
	for (col = 0; col < cols.size(); ++col)
	{
		for (row = 0; row < cols.at(col); ++row)
		{
			if ((row == 0) && (col == 0))
				continue;

			++n;
			lndim = log(diag.calc_dim_ratio_fast(row));
			lndim_sum += lndim - log(n)/2;
			diag.add(row);
		}
	}

	return -(lndim_sum + (n/2.) * log(2) ) / sqrt(n);
}

int StrictDiagram::max_difference_in_rows(StrictDiagram diag)
/*
 * Compares the row lengths of 2 strict diagrams.
 * Returns the maximum difference.
 */
{
	uint64 i;
	int diff, max_diff;

	max_diff = 0;
	for (i = 0; i < min(get_height(), diag.get_height()); ++i)
	{
		diff = udiff(get_width(i), diag.get_width(i));
		if (diff > max_diff)
			max_diff = diff;
	}

	if (get_height() > diag.get_height())
	{
		for (; i < get_height(); ++i)
		{
			diff = rows.at(i);
			if (diff > max_diff)
				max_diff = diff;
		}
	}
	else
	{
		if (get_height() < diag.get_height())
		{
			for (; i < diag.get_height(); ++i)
			{
				diff = diag.get_width(i);
				if (diff > max_diff)
					max_diff = diff;
			}
		}
	}
	return max_diff;
}


mdd StrictDiagram::calc_planch_limit(const int NUM_OF_POINTS)
/*
Calculates the plancherel limit curve
*/
{
	int i;
	double x, y, X, Y;

	for (i = 0; i <= NUM_OF_POINTS; ++i)
	{
		X = i * (2.*sqrt(2)) / NUM_OF_POINTS;
		Y = (1.0/M_PI) * (X * acos(X/(2.*sqrt(2))) - sqrt(8-X*X));
		
		x = -Y;	y = X;
		planch_limit[x] = y;
	}

	return planch_limit;
}
