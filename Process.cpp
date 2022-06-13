#include "Process.h"

/*
	TBD:
	- �������� �� �������, ������������ �� Process
	- diag, p, ... - ��������� ����!
	- �����������, � �� ������ ���������
	- ���������� - print � DELETE_DIAGRAM!!!

	- add_rand_plansh � add_rand_gen - ������������� ����������� ������� � ���������� 0..1 - �������� ��� ��� �������.
*/

double Process::alpha = 0.16;

void throw_error(char* err_mes)
{
	cout << "ERROR!!! ";
	cout << err_mes << endl;
	//system("pause");
	exit(EXIT_FAILURE);
}

void Process::rich(uint64 num)
{
	Diagram* diag = new Diagram();
	for (uint64 i = 1; i < num - 1; ++i)
		add_rand(diag);
	diag->print();
	diag->print_to_file((char*)"rich.txt");
}

void Process::rich(uint64 num, Diagram* diag)
{
	for (uint64 i = 1; i < num; ++i)
	{
		add_rand(diag);
	}
}

void Process::rich3D(uint64 num, Diagram3D* diag)
{
        if (diag == NULL)
                diag = new Diagram3D();
        for (uint64 i = 1; i < num; ++i)
                add_rand3D(diag);
}

void Process::strict_add_rand(StrictDiagram* diag)
/*
	��������� ��������� ������ (��)
*/
{
	mi* p;
	mi::iterator it;

	p = diag->get_points();
	it = p->begin();
	advance(it, rand_0_to_n_1(p->size()));

	//cout << "diag: ";
	//diag->print_cols();
	//cout << "line: " << it->first << endl;
	//cout << diag->calc_dim_ratio(it->first) << endl;

	if (diag->add(it->first))
		throw_error((char*)"add_rand");
}


double Process::strict_rich_to_target(StrictDiagram diag, StrictDiagram target)
/*
 * returns normdim of target diagram
 */
{
	mi p, processed_p;
	di *rows, *target_rows;
	mi::iterator it;
	double ln_dim;
	uint64 n, row;

	rows = diag.get_rows();
	target_rows = target.get_rows();


	//cout << "Target: ";
	//target.print_cols();

	ln_dim = 0;
	n = 1;
	while (!diag.is_equal_to(&target))
	{
		processed_p.clear();
		++n;
		p = *diag.get_points();

		//diag.print_cols();
		//cout << "Before deleting:\n";
		//for (it = p.begin(); it != p.end(); ++it)
		//{
		//	cout << it->first << " ";
		//}
		//cout << endl;

		//удаляем лишние points:
		for (it = p.begin(); it != p.end(); ++it)
		{
			row = it->first;
/*
			if (n == 308073)
			{
				cout << row << " ";
				if (row != diag.get_height())
				{
					cout << " " << rows->at(row) << " " << target_rows->at(row) << endl;
				}
				else
					cout << endl;
			}
*/

			if (row == diag.get_height())
			{
				if (row != target.get_height())
					processed_p[row];
			}
			else
			{
				if (rows->at(row) != target_rows->at(row))
					processed_p[row];
			}
		}

		//cout << "After deleting:\n";
		//for (it = p.begin(); it != p.end(); ++it)
		//{
		//	cout << it->first << " ";
		//}
		//cout << endl;

		it = processed_p.begin();
		advance(it, rand_0_to_n_1(processed_p.size()));

		//cout << "We choose: " << it->first << endl;

		ln_dim += log(diag.calc_dim_ratio_fast(it->first)) - log(n)/2;

		//cout << "ln_dim: " << ln_dim << endl;

		diag.add(it->first);



/*
		if (rows->at(it->first) > target_rows->at(it->first))
		{
			cout << "size: " << n << endl;
			cout << "added.row: " << it->first << endl;
			for (it = p.begin(); it != p.end(); ++it)
			{
				cout << it->first << " ";
			}
			cout << endl;
			diag.print_cols();
			diag.print();
			cout << endl;
			cout << "target:\n";
			target.print_cols();
			cout << endl << endl;
		}
*/



	}
	//cout << "final ln_dim: " << ln_dim << ", res = " << -(ln_dim + (n/2.) * log(2) ) / sqrt(n) << endl;
	return -(ln_dim + (n/2.) * log(2) ) / sqrt(n);
}

uint64 Process::add_rand(Diagram* diag)
{
	mi* p;
	mi::iterator it;

	p = diag->get_points();
	it = p->begin();
	advance(it, rand_0_to_n_1(p->size()));

	if (diag->add_with_probsum(it->first))
		throw_error((char*)"add_rand");

	return it->first;
}

uint64 Process::add_rand(Diagram* diag, double& prob)
{
        mi* p;
        mi::iterator it;

        p = diag->get_points();
        it = p->begin();
        advance(it, rand_0_to_n_1(p->size()));

        prob = diag->calc_point_plansh_prob(it->first);

        if (diag->add_with_probsum(it->first))
                throw_error((char*)"add_rand");

        return it->first;
}

uint64 Process::add_randrand(Diagram* diag)
//Firstly, the probabilities are distributed randomly. Next, we choose a random point to add.
{
	const double PROB_LIMIT = 0.5;

	mi* p;
	mi::iterator it;
	double excess, prob, sum;
	int num_of_points, i;
	uint64 y;

	p = diag->get_points();
	num_of_points = p->size();

	//generate a random distribution of probabilities:
	if (num_of_points == 1)
		p->begin()->second = 1;
	else
	{
		prob = rand_0_1() * PROB_LIMIT;
		p->begin()->second = prob;
		excess = 1 - prob;
	
		for (it = p->begin(), ++it, i = 2; i < num_of_points; ++it, ++i)
		{
			prob = rand_0_1() * PROB_LIMIT * excess;
			it->second = prob;
			excess -= prob;
		}	
		it->second = excess;
	}

	prob = rand_0_1();
	for (it = p->begin(), sum = 0; (it != p->end()) && (sum < prob); ++it)
	{	
		sum += it->second;
	}
	--it;

	y = it->first;

	if (diag->add(y))
	//if (diag->add_with_probsum(it->first))
		throw_error((char*)"add_rand");

	return y;
}

pu Process::add_randrand3D(Diagram3D* diag)
//Firstly, the probabilities are distributed randomly. Next, we choose a random point to add.
{
	const double PROB_LIMIT = 0.000001;//0.07;//0.045;
	const double EPS = 0.00001;

	mpu* p;
	mpu::iterator it;
	double excess, prob, sum, delta;
	int num_of_points, i;
	pu coord;
	vpu points_coords;

	p = diag->get_points();
	num_of_points = p->size();

	//arrange same probs to all points
	for (it = p->begin(); it != p->end(); ++it)
	{
		it->second = 1.0/num_of_points;
		points_coords.push_back(it->first);
	}
	//randomly change the values of probabilities
	for (it = p->begin(), i = 0; i != num_of_points - 1; ++it, ++i)
	{
		delta = rand_0_1() * (next(it, 1)->second / 2 + it->second / 2) - it->second / 2;
		it->second += delta;
		next(it, 1)->second -= delta;
	}	


/*
	//clear probs
	for (it = p->begin(); it != p->end(); ++it)
	{
		it->second = 0;
		points_coords.push_back(it->first);
	}
	//generate a random distribution of probabilities:
	excess = 1;
	if (num_of_points == 1)
		p->begin()->second = 1;
	else
	{
		for (it = p->begin(); excess > EPS; ++it)
		{
			if (it == p->end()) it = p->begin();	
			prob = rand_0_1() * PROB_LIMIT * excess;

			it->second += prob;
			excess -= prob;
		}	
		if (it == p->end()) it = p->begin();
		it->second += excess;
	}
*/

/*
double sumTMP = 0;
for (auto it2 = p->begin(); it2 != p->end(); ++it2)
{
	sumTMP += it2->second;
}
cout << "SUM: " << sumTMP << endl;
*/

	prob = rand_0_1();
	for (it = p->begin(), sum = 0; (it != p->end()) && (sum < prob); ++it)
	{	
		sum += it->second;
	}
	--it;


//cout << it->first.first << " " << it->first.second << endl;
	coord = it->first;

	if (diag->add(coord))
	//if (diag->add_with_probsum(it->first))
		throw_error((char*)"add_rand");

//cout << it->first.first << " " << it->first.second << endl;

	return coord;
}


pu Process::add_rand3D(Diagram3D* diag, double* prob, const double ALPHA)
{
	mpu* p;
	mpu::iterator it;
	pu res;

	p = diag->get_points();
	it = p->begin();
	advance(it, rand_0_to_n_1(p->size()));

	res.first = it->first.first;
	res.second = it->first.second;

	if (prob != NULL)
		*prob = diag->calc_planch_prob(res.first, res.second, ALPHA);

	if (diag->add(res))
	{
		cout << "Error: Process::add_rand3D()!" << endl;
		exit(EXIT_FAILURE);
	}	
	return res;
}

pu Process::add_greedy_planch3D(Diagram3D* diag)
{
	pu max_line;
	double max_prob;

	diag->calc_planch_probs(&max_line, &max_prob);
	diag->add(max_line);

	return max_line;
}

void Process::gen_rich(uint64 num)
{
	Diagram* diag = new Diagram();
	for (uint64 i = 1; i < num - 1; ++i)
	{
		if (i % (num/20) == 0)
			cout << i << "/" << num << endl;
		add_rand_gen(diag);
	}
	diag->print();
	diag->print_to_file((char*)"gen_rich.txt");
}

void Process::gen_rich3D(uint64 num, Diagram3D* diag)
/*
	��������� ������� ���������� (���), 3D
*/
{
	for (uint64 i = 0; i < num; ++i)
	{
		if (i % 10000 == 0)
			cout << i << "/" << num << endl;
		add_rand_gen3D(diag);
	}
	//diag->print();
}

uint64 Process::add_rand_gen(Diagram* diag, double a)
{
	mi* p;
	mi::iterator it;
	double sum, rnd;
	uint64 x, y;

	sum = 0;
	p = diag->get_points();

	for (it = p->begin(); it != p->end(); ++it)
	{
		y = it->first;
		x = diag->get_width(y);
		it->second = fi(x, y, a);
		sum += it->second;
	}
	for (it = p->begin(); it != p->end(); ++it)
	{
		it->second /= sum;
	}

	rnd = rand_0_1();
	for (sum = 0, it = p->begin(); (sum <= rnd) && (it != p->end()); ++it)
	{
		sum += it->second;
	}

	--it;

	y = it->first;

	if (diag->add(y))
		throw_error((char*)"add_rand_gen()");

	return y;
}

pu Process::add_rand_gen3D(Diagram3D* diag, double a)
/*
��������� ��������� ������ (���)
*/
{
	mpu* p;
	mpu::iterator it;
	double sum, rnd;
	uint64 x, y, z;
	pu res;

	sum = 0;
	p = diag->get_points();

	//1. ��������� �����������, ��������� �� � p->second

	for (it = p->begin(); it != p->end(); ++it)
	{
		diag->get_point_coords(it, x, y, z);
		it->second = fi3D(x, y, z, a);
		sum += it->second;
	}

	for (it = p->begin(); it != p->end(); ++it)
	{
		it->second /= sum;
	}

	//2. ���������� ��������� ����� rnd = 0..1
	rnd = rand_0_1();

	//3. ��������� ������ p->second, ���������������, ����� sum > rand ��� ����� it=...end.
	for (sum = 0, it = p->begin(); (sum <= rnd) && (it != p->end()); ++it)
	{
		sum += it->second;
	}
//cout << "add_rand_gen3D(): BUG ('--it' is commented because mpu's type map was changed to unordered_map)\n";
//exit(1);
	--it;
	res = it->first;

	diag->add(it->first.first, it->first.second);

	return res;
}

double Process::fi(uint64 x, uint64 y, double a)
/*
	������� ������� (���)
*/
{
	//return pow(x*x + y*y, a);
	return pow(x + y, a);
}

double Process::fi3D(uint64 x, uint64 y, uint64 z, double a)
/*
������� ������� (���)
*/
{
	return pow(x*x + y*y + z*z, a);
}

uint64 Process::add_rand_plansh(Diagram* diag, double* prob)
//	��������� ��������� ������ (��)
{
	mi *p;
	mi::iterator it;
	double rnd, sum;
	uint64 y;

	p = diag->get_points();

	//������� ����������� ��� ������ �����-���������:
	diag->calc_plansh_probs();

	//2. ���������� ��������� ����� rnd = 0..1
	rnd = rand_0_1();

	//3. ��������� ������ p->second, ���������������, ����� sum > rand ��� ����� it=...end.
	for (sum = 0, it = p->begin(); (sum <= rnd) && (it != p->end()); ++it)
	{
		sum += it->second;
	}
	--it;

	//cout << it->second << endl;

	diag->add_prob_sum(it->second);
	
	if (prob != NULL)
		*prob = it->second;

	y = it->first;

	if (diag->add(y))
		throw_error((char*)"add_rand_plansh()");

	return y;
}


uint64 Process::add_greedy_plansh(Diagram* diag)
//��������� � 2D ��������� ������ � ������������ ��������������� ������������
{
	mi::iterator it, max_it;
	diag->calc_plansh_probs();
	return diag->add_max_with_probsum();
}

pu Process::add_rand_plansh3D(Diagram3D* diag, double* prob, bool calc_probs, double alpha)
{
	mpu* p;
	mpu::iterator it, it2;
	double rnd, sum;
	pu res;
	pu max_line;
	double max_prob;

	p = diag->get_points();

	if (calc_probs)
		diag->calc_planch_probs(&max_line, &max_prob, alpha);

	rnd = rand_0_1();
//cout << "A\n";
//diag->print_cols();
//cout << "points.size: " << p->size() << endl;
//cout << "points:\n";
//diag->print_points();
//cout << endl;

	for (sum = 0, it = p->begin(); (sum <= rnd) && (it != p->end()); ++it)
	{
		sum += it->second;
		it2 = it;
	}
	it = it2;
	//--it;

	res.first = it->first.first;
	res.second = it->first.second;

	if (prob != NULL)
		//*prob = diag->calc_planch_prob(res.first, res.second);
		*prob = it->second;

	diag->add(it->first.first, it->first.second);

	return res;
}

pu Process::add_lexicogr3D(Diagram3D* diag, double a, double b, Diagram3D* target, bool ADD)
//Lexicographical adding with parameters a,b
{
	mpu* p;
	mpu::iterator it;
	pu max_coord;
	double max, val;
	uint64 x, y, z;

	p = diag->get_points();

	max = -1;

	//search the point to add (max_coord)
	for (it = p->begin(); it != p->end(); ++it)
	{
		z = it->first.first;
		y = it->first.second;

		if ((target != NULL) && (target->getpointX(z, y) > diag->getpointX(z, y)))
		{
			x = diag->getpointX(z, y);

			val = a * x + b * (y+1) + (1 - a - b) * (z+1);
			if (val > max)
			{
				max = val;
				max_coord = pu(z, y);
			}
		}
	}

	if (ADD)
		diag->add(max_coord);

	return max_coord;
}

void Process::plansh(uint64 num)
/*
	����������
*/
{
	Diagram* diag = new Diagram();
	for (uint64 i = 1; i < num; ++i)
	{
		add_rand_plansh(diag);
	}
	diag->print();
	diag->print_to_file((char*)"plansh.txt");
}

void Process::plansh(uint64 num, Diagram* diag)
/*
	����������
*/
{
	for (uint64 i = 0; i < num; ++i)
		add_rand_plansh(diag);
}

void Process::plansh_with_log(uint64 num, Diagram* diag)
{
	ofstream f("plansh_log.txt");
	
	for (uint64 i = 0; i < num; ++i)
	{
		add_rand_plansh(diag);
		if (i%100 == 0)
		{
			f << i << ":" << endl;
			diag->print_to_file((char*)"plansh_log.txt", f.rdbuf());
		}
	}
	f.close();
}

void Process::plansh3D(uint64 num, Diagram3D* diag)
/*
����������
*/
{
	if (diag == NULL)
		diag = new Diagram3D();
	for (uint64 i = 0; i < num; ++i)
	{
		add_rand_plansh3D(diag);
		//if (i % 10000 == 0)
		//	cout << i << "/" << num << endl;
	}
	//diag->print_all();
	//diag->print_all_to_files();
}

/*
void Process::plansh3D_best(uint64 num, Diagram3D* diag)
//��������� � diag num ������ ������� (�� ����������)
{
	if (diag == NULL)
		diag = new Diagram3D();
	for (uint64 i = 1; i < num - 1; ++i)
		add_best_plansh3D(diag);
}
*/



void Process::gen_rich(uint64 num, Diagram* diag, double a)
/*
��� (����� 2)
*/
{
	for (uint64 i = 0; i < num; ++i)
		add_rand_gen(diag, a);
}


//void Process::plansh_greedy(uint64 num, Diagram* diag, ostream &output)
void Process::plansh_greedy(uint64 num, Diagram* diag, FILE* f, bool tmpbool)
{
	if (f != NULL)
		fprintf(f, "+");
	for (uint64 i = 0; i < num; ++i)
	{
		add_greedy_plansh(diag);

		//tmpbool = false;
		if (tmpbool)
		{
			//cout << "added...\n";
			//diag->print_corners();
/*
			di* rows = diag->get_rows();
			mi* corners = diag->get_corners();
			mi::iterator it;

			printf("%llu 0\n", rows->at(corners->begin()->first));
			for (it = corners->begin(); it != corners->end(); ++it)
				printf("%llu %llu\n", rows->at(it->first), it->first + 1);
			--it;
			printf("0 %llu\n", it->first + 1);
	*/
			//diag->print_points_cute();
			//diag->print_corners_for_gnuplot(0, 0, 0, true);
			//cout << endl;
		}

		diag->print_max_pl(f);
	}
}

void Process::plansh_greedy_with_log(uint64 num, Diagram* diag, ostream &output)
{
	for (uint64 i = 0; i < num; ++i)
	{
		add_greedy_plansh(diag);
		diag->print_to_file(NULL, output.rdbuf());
	}
}


uint64 Process::rand_to_target_onestep(Diagram* diag, Diagram* target)
/*
IT IS EXPECTED, that diag does not have illegal points!
*/
{
	const bool CALC_PLANCH_PROBS = false;

	//mi leg_p;
	mi* points;
	mi::iterator it;
	double prob;
	uint64 line;

	if (CALC_PLANCH_PROBS)
		diag->calc_plansh_probs();
	//diag->get_legal_points(target, &leg_p);

	//it = leg_p.begin();
	//advance(it, rand_0_to_n_1(leg_p.size()));
	points = diag->get_points();
	it = points->begin();
	advance(it, rand_0_to_n_1(points->size()));

	line = it->first;
	prob = it->second;

	if (CALC_PLANCH_PROBS)	
		diag->add_prob_sum(prob);

	if (diag->add(line))
		throw_error((char*)"rand_to_target()");

	//Delete illegal points!
	if (diag->get_width(line) == target->get_width(line))
		points->erase(line);
	if (diag->get_width(line + 1) == target->get_width(line + 1))
		points->erase(line + 1);

	return line;
}

void Process::rand_to_target(Diagram* diag, Diagram* target)
{
	mi leg_p;
	mi::iterator it;
	uint64 incr_size;

	incr_size = target->get_size() - diag->get_size();
	
	if (incr_size == 0)
		return;


	//cout << incr_size << endl;

	do
	{

		diag->calc_plansh_probs();
		diag->get_legal_points(target, &leg_p);

		incr_size--;
		it = leg_p.begin();
		advance(it, rand_0_to_n_1(leg_p.size()));

		diag->add_prob_sum(it->second);

		if (diag->add(it->first))
			throw_error((char*)"rand_to_target()");
		++it;
	} 
	while (incr_size != 0);
	//while ((incr_size != 0) && (!diag->sum_is_greater_than(target)));
}

double Process::conseq_to_target(di* target_rows)
//�������� target, ������������ ��������������� �����������.
{
	Diagram *diag, *diagTMP;
	di::iterator it;
	uint64 i, j;
	mi* p;
	double res;
	
	diagTMP = new Diagram();
	diag = new Diagram();

	p = diag->get_points();
	
	/*
	for (it = target_rows.begin(), i = 1; it != target_rows.end(); ++it, ++i)
		for (j = 0; j < *it; ++j)
		{
			diag->add_prob_sum(p->at(i));
			if (diag->add(i))
				throw_error((char*)"conseq_to_target()");
		}
	*/

	//��������� ������ ���
	for (i = 1; i <= target_rows->at(0) - 1; ++i)
	{
		////////////////calc_plansh_probs(diag); //� diag->points ��������� �����.�����������
		diag->calc_plansh_probs();
		diag->add_prob_sum(p->at(0));
		if (diag->add(0))
			throw_error((char*)"conseq_to_target()");
	}

	//��������� ��������� ����
	for (it = target_rows->begin() + 1, i = 1; it != target_rows->end(); ++it, ++i)
		for (j = 0; j < *it; ++j)
		{
			/////////////////calc_plansh_probs(diag); //� diag->points ��������� �����.�����������
			diag->calc_plansh_probs();
			diag->add_prob_sum(p->at(i));
			if (diag->add(i))
				throw_error((char*)"conseq_to_target()");
		}

	diagTMP->init_by_rows(*target_rows);
	if (!diag->is_equal_to(diagTMP))
		cout << "NOT EQUAL" << endl;
	//else
	//	cout << "equal" << endl;
	delete diagTMP;

	//diag->print();
	//diag->print_corners();

	res = diag->get_c();
	delete diag;

	return res;
}

void Process::strict_probgreedy_plansh(uint64 num, StrictDiagram* diag, const char* fname)
{
	const bool SAVE_TABLE = false;
	const bool SAVE_ZEROADD = false;

	uint64 line;
	double prob, ratio;//, ratioTMP;
	fstream f, f_tab, f_log, f_zeroadd; //f_zeroadd - сюда записываются отношения размерностей, когда добавляется новый столбец! (line=0)
	di* r;

	if (SAVE_ZEROADD)
	{
		f_zeroadd.open("strict_greedy_zeroadd.txt", fstream::out | fstream::trunc);
		if (!f_zeroadd.is_open())
		{
			cout << "Process::greedy_plansh(): Error opening file!\n";
			exit(0);
		}
	}

	if (SAVE_TABLE)
	{
		f_tab.open("strict_greedy_tab.txt", fstream::out | fstream::trunc);
		if (!f_tab.is_open())
		{
			cout << "Process::greedy_plansh(): Error opening file!\n";
			exit(0);
		}
	}

	if (fname != NULL)
	{
		f.open(fname, fstream::out | fstream::trunc);
		f_log.open("strict_greedy_loglog.txt", fstream::out | fstream::trunc);
		if (!f.is_open() || !f_log.is_open())
		{
			cout << "Process::greedy_plansh(): Error opening file!\n";
			exit(0);
		}
	}

	for (uint64 i = 0; i < num; ++i)
	{
		diag->calc_max_prob(&line, &prob);

		if (SAVE_TABLE)
			f_tab << line << " ";

		if (fname != NULL)
		{
			ratio = diag->calc_dim_ratio(line);
			f << ratio << endl;

			if (line == 0)
				f_zeroadd << i+1 << " " << ratio << endl;
			//if (line != 0)
			//	f_zeroadd << "0\n";
			//else
			//	f_zeroadd << ratio << endl;

			f << "n=" << i+1 << endl;
			r = diag->get_cols();
			for (di::iterator it = r->begin(); it != r->end(); ++it)
				f << *it << " ";
			f << endl;
			f << line << " " << ratio << endl << endl;

			f_log << log(log(ratio)) << endl;
		}

		diag->add(line, prob);

		diag->print_cols();
	}

	if (fname != NULL)
	{
		f.close();
		f_log.close();
	}
	if (SAVE_TABLE)
		f_tab.close();
	if (SAVE_ZEROADD)
		f_zeroadd.close();
}

double Process::strict_probgreedy_plansh(StrictDiagram* diag)
{
	uint64 line;
	double prob, res;

	diag->calc_max_prob(&line, &prob);
	res = diag->calc_dim_ratio(line);
	diag->add(line, prob);

	return res;
}

void Process::strict_plansh(uint64 num, StrictDiagram* diag, const char* fname, vvd* ratios)
/*
 * Строгий жадный Планшерель.
 * num - число добавляемых клеток
 * diag - диаграмма
 * fname - имя файла, куда будут записаны отношения размерностей
 * ratios - двумерный массив, в который будут записаны отношения размерностей
 */
{
	const bool SAVE_TABLE = true;
	mi *p;
	mi::iterator it;
	double rnd, sum, ratio;
	fstream f, f_tab, f_log;

	if (SAVE_TABLE)
	{
		f_tab.open("strict_plansh_tab.txt", fstream::out | fstream::trunc);
		if (!f_tab.is_open())
		{
			cout << "Process::strict_plansh(): Error opening file!\n";
			exit(0);
		}
	}

	if (fname != NULL)
	{
		f.open(fname, fstream::out | fstream::trunc);
		f_log.open("strict_plansh_loglog.txt", fstream::out | fstream::trunc);
		if (!f.is_open() || !f_log.is_open())
		{
			cout << "Process::strict_plansh(): Error opening file!\n";
			exit(0);
		}
	}

	for (uint64 i = 0; i < num; ++i)
	{
		//if (i % 10000 == 0)
		//	cout << i / 10000 << "%" << endl;

		//cout << i+1 << " " << diag->get_c() << endl;

		//cout << "cols: ";
		//diag->print_cols();

		p = diag->get_points();
		diag->calc_plansh_probs();
		rnd = rand_0_1();
		for (sum = 0, it = p->begin(); (sum <= rnd) && (it != p->end()); ++it)
		{
			sum += it->second;
		}
		--it;

		if (SAVE_TABLE)
			f_tab << it->first << endl;

		if ((fname != NULL) || (ratios != NULL))
			ratio = diag->calc_dim_ratio(it->first);

		if (fname != NULL)
		{
			f << ratio  << endl;
			f_log << log(log(ratio)) << endl;
		}

		if (ratios != NULL)
			ratios->at(i).push_back(ratio);

		if (diag->add(it->first))
			throw_error((char*)"strict_plansh()");
	}

	f_log.close();
	f_tab.close();
	f.close();
}



uint64 Process::strict_plansh(StrictDiagram* diag, double* prob)
{
	mi *p;
	mi::iterator it;
	double rnd, sum;
	uint64 line;

	p = diag->get_points();
	diag->calc_plansh_probs();

	rnd = rand_0_1();

	for (sum = 0, it = p->begin(); (sum <= rnd) && (it != p->end()); ++it)
	{
		sum += it->second;
	}
	--it;

	if (prob != NULL)
		*prob = it->second;
	line = it->first;

	if (diag->add_with_probsum(line))
		throw_error((char*)"strict_plansh()");

	return line;
}

void Process::strict_rich(uint64 num, StrictDiagram* diag)
{
	for (uint64 i = 1; i < num; ++i)
		strict_add_rand(diag);
}

void Process::plansh_on_strict(uint64 num, Diagram* diag)
/*
 * "Условный Планшерелевский процесс".
 * Вероятности считаются как для обычных диаграмм. Но при добавлении новой клетки следим, чтобы диаграммы
 * были строгими. Вероятности масштабируются так, чтобы сумма была равна 1.
 */
{
	uint64 i, col, col_height;
	//di::iterator dit;
	mi::iterator it;
	mi *p;
	di *c;
	mi strict_p;
	double rnd, sum;

	p = diag->get_points();
	c = diag->get_cols();
	for (i = 0; i < num; ++i)
	{
		//diag->print_cols();

		strict_p.clear();
		sum = 0;
		for (it = p->begin(); it != p->end(); ++it)
		{
			//Если при добавлении данной точки получится строгая диаграмма, то сохраняем эту точку:
			col = diag->get_width(it->first);
			if (col == c->size())
				col_height = 0;
			else
				col_height = c->at(col);
			if (col != 0)
				if (col_height + 1 == c->at(col-1))
					continue;
			it->second = diag->calc_point_plansh_prob(it->first);
			strict_p[it->first] = it->second;
			sum += strict_p[it->first];
		}

		/*
		diag->print_cols();
		diag->calc_plansh_probs();
		diag->print_points();
		for (it = strict_p.begin(); it != strict_p.end(); ++it)
		{
			cout << it->first << ": " << it->second << endl;
		}
		cout << endl;
		*/

		//cout << "probs: ";
		//for (it = strict_p.begin(); it != strict_p.end(); ++it)
		//{
		//	cout << it->second << " ";
		//}
		//cout << endl;

		//Масштабируем хорошие вероятности
		for (it = strict_p.begin(); it != strict_p.end(); ++it)
		{
			it->second /= sum;
		}


		//cout << "probs: ";
		//for (it = strict_p.begin(); it != strict_p.end(); ++it)
		//{
		//	cout << it->second << " ";
		//}
		//cout << endl;
		/*
		cout << "After scaling:\n";
		for (it = strict_p.begin(); it != strict_p.end(); ++it)
		{
			cout << it->first << ": " << it->second << endl;
		}
		cout << endl << endl << endl;
		*/

		rnd = rand_0_1();

		for (sum = 0, it = strict_p.begin(); (sum <= rnd) && (it != strict_p.end()); ++it)
		{
			sum += it->second;
		}
		--it;

		//cout << it->second << endl;

		//diag->add_prob_sum(it->second);

		//cout << it->first << " " << it->second << " " << p->at(it->first) << endl;

		diag->add_prob_sum(p->at(it->first));

		if (diag->add(it->first))
			throw_error((char*)"plansh_on_strict()");
	}
}

int Process::strict_add_maxdim(uint64 num, StrictDiagram* diag)
{
	const bool SAVE_TABLE = false;
	const bool SAVE_RATIO = false;

	fstream f_tab, f_rat;
	mi::iterator it;
	StrictDiagram* diag2;//лучше подавать инициализированную, как параметр
	mpz_t dim, dim_max;
	mpq_t rat;
	uint64 max;
	mi* p;
	int res;




	//МЕДЛЕННО: постоянный init.
	mpz_init_set_ui(dim, 1);
	mpz_init(dim_max);





	if (SAVE_TABLE)
	{
		f_tab.open("add_maxdim_tab.txt", fstream::out | fstream::app);
		if (!f_tab.is_open())
		{
			cout << "Process::strict_add_maxdim(): Error opening file!\n";
			exit(0);
		}
	}

	if (SAVE_RATIO)
	{
		f_rat.open("add_maxdim_rat.txt", fstream::out | fstream::trunc);
		if (!f_tab.is_open())
		{
			cout << "Process::strict_add_maxdim(): Error opening file!\n";
			exit(0);
		}
		mpq_init(rat);
		mpq_set_den(rat, dim);
	}
/*
 *
 *
 * БАГ: второй раз инит!!!!!!!
 *
 *
 */
	mpz_init(dim);
	mpz_init(dim_max);
	diag2 = new StrictDiagram();

	p = diag->get_points();

	res = 1;

	for (uint64 i = 0; i < num; ++i)
	{
		max = 0;
		mpz_set_ui(dim_max, 0);

		//Для текущей диаграммы вычисляем max!
		for (it = p->begin(); it != p->end(); ++it)
		{
			diag2->init(diag);
			diag2->add(it->first);

			diag2->calc_strict_dim(dim);

			if (mpz_cmp(dim, dim_max) > 0)
			{
				mpz_set(dim_max, dim);
				max = it->first;
			}
		}

		if (SAVE_RATIO)
		{
			//f_rat << diag->calc_dim_ratio(max) << "\n";
			mpq_set_num(rat, dim_max);
			f_rat << mpq_get_d(rat) << "\n";
			mpq_set_den(rat, dim_max);
		}
		if (SAVE_TABLE)
			f_tab << max << "\n";

		res = diag->add(max);
	}

	if (SAVE_RATIO)
	{
		f_rat.close();
		mpq_clear(rat);
	}
	if (SAVE_TABLE)
		f_tab.close();

	mpz_clear(dim);
	mpz_clear(dim_max);

	delete diag2;
	return res;
}

void Process::planch2D_to_target(Diagram& diag, Diagram& diag2, Diagram& target, double& prob)
/*
IT IS EXPECTED, that diag does not have illegal points!
diag2 is needed to remember all points
*/
{
/*
	mi points;
	//mpu points_copy;
	mi::iterator it;
	double sum;
	pu coord;
	uint64 y;

	//calc probs!
	diag.calc_plansh_probs();
	//points_copy = *diag.get_points();
	points = *diag.get_points();

	//SCALE probs!
	sum = 0;
	for (it = points.begin(); it != points.end(); ++it)
		sum += it->second;
	for (it = points.begin(); it != points.end(); ++it)
	{
		it->second /= sum;
//cout << it->second << endl;
	}
//cout << endl << endl;
	
	coord = add_rand_plansh3D(&diag, NULL, true, ALPHA);
	z = coord.first;
	y = coord.second;

	//prob = points_copy[coord];
	prob = diag2.calc_planch_prob(coord, ALPHA);
	diag2.add(coord);

	//Delete illegal points!
	if (diag.getpointX(z, y) == target.getpointX(z, y))
		points->erase(pu(z, y));
	if (diag.getpointX(z, y + 1) == target.getpointX(z, y + 1))
		points->erase(pu(z, y + 1));
	if (diag.getpointX(z + 1, y) == target.getpointX(z + 1, y))
		points->erase(pu(z + 1, y));
*/
}


uint64 Process::planch2D_to_target_incorrectprobs(Diagram& diag, Diagram& target)
/*
!!! The Plancherel probs are distributed between legal points.
BUT they are not real Pl.Probs (not suitable for output to file!)
*/
{
	mi::iterator it;
	double rnd, sum;
	pu coord;
	uint64 y;
	mi *p;
	Diagram diag_;

	diag_.init(&diag);
	p = diag_.get_points();
	diag_.calc_plansh_probs();
	diag_.remove_illegal_points(target);
	sum = 0;
	for (it = p->begin(); it != p->end(); ++it)
		sum += it->second;
	for (it = p->begin(); it != p->end(); ++it)
	{
		it->second /= sum;
//cout << it->second << endl;
	}

	rnd = rand_0_1();
	for (sum = 0, it = p->begin(); (sum <= rnd) && (it != p->end()); ++it)
	{
		sum += it->second;
	}
	--it;

	//cout << it->second << endl;

	diag.add_prob_sum(it->second);

	y = it->first;	

	if (diag.add(y))
		throw_error((char*)"add_rand_plansh()");

	return y;
}

void Process::planch3D_to_target(Diagram3D& diag, Diagram3D& diag2, Diagram3D& target, double& prob, double ALPHA)
/*
IT IS EXPECTED, that diag does not have illegal points!
diag2 is needed to remember ALL points
*/
{
	mpu* points;
	//mpu points_copy;
	mpu::iterator it;
	pu coord;
	uint64 z, y, x;


	points = diag.get_points();

	coord = add_rand_plansh3D(&diag, NULL, true, ALPHA);
	z = coord.first;
	y = coord.second;

	//prob = points_copy[coord];
	prob = diag2.calc_planch_prob(coord, ALPHA);
	diag2.add(coord);

//cout << "LETS CHECK around " << z << " " << y << endl; 

	x = diag.getX(z, y);

/*
VOZMOZHEN BUG: esli v targete net Z s koordinatami diag.getZ(x, y+1).
Problema, chto dlya targeta beretsya getZ is DIAG, a ne iz TARGETa.
Takzhe, dlya dannogo Z mozhet ne byt' y+1
*/

	if (diag.getpointX(diag.getZ(x, y + 1), y + 1) == target.getpointX(diag.getZ(x, y + 1), y + 1))
		points->erase(pu(diag.getZ(x, y + 1), y + 1));
	//Delete illegal points!
	if (diag.getpointX(z, y) == target.getpointX(z, y))
		points->erase(pu(z, y));

	if (diag.getpointX(z, y + 1) == target.getpointX(z, y + 1))
		points->erase(pu(z, y + 1));
	if (diag.getpointX(z + 1, y) == target.getpointX(z + 1, y))
		points->erase(pu(z + 1, y));

	if (y != 0)
	{
		if (diag.getpointX(z, y - 1) == target.getpointX(z, y - 1))
			points->erase(pu(z, y - 1));
	}



/*
	if (z != 0)
	{
		if (diag.getpointX(z - 1, y) == target.getpointX(z - 1, y))
			points->erase(pu(z - 1, y));
		if (diag.getpointX(z - 1, y + 1) == target.getpointX(z - 1, y + 1))
			points->erase(pu(z - 1, y + 1));
	}

	if ((z != 0) && (y != 0))
	{
		if (diag.getpointX(z - 1, y - 1) == target.getpointX(z - 1, y - 1))
			points->erase(pu(z - 1, y - 1));
	}
*/
}
