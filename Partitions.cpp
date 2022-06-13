#include "Partitions.h"
#include <fstream>
#include <iostream>

map<pair<uint64, uint64>, uint64> Partitions::parts;
vector < vector<uint64> > Partitions::parts_table;

void Partitions::part_n_k_to_file(const uint64 n, const uint64 k, char* fname)
{
	ofstream f;
	uint64 n_, k_;

	f.open(fname);
	for (n_ = 0; n_ <= n; ++n_)
	{
		cout << n_ << endl;
		for (k_ = 0; k_ <= k; ++k_)
		{
			if (!n_ || !k_)
				f << "0 ";
			else
				f << number_less_than_k(n_, k_) << " ";
		}
		f << endl;
	}

	f.close();
}






uint64 Partitions::number_less_than_k(uint64 n, uint64 k)
{
	map<pair<uint64, uint64>, uint64>::iterator it = parts.find(pair<uint64, uint64>(n, k));
	if (it != parts.end())
		return it->second;

	uint64 res;
	if (n < k)
		k = n;
	if ((n <= 1)||(k == 1))
		return 1;

	//if ((n == 1) || (n < k))
	//	return 0;
	//if ((k == 1) || (n == k))
	//	return 1;

	////////////////////////res = n_part(n - 1, k - 1) + n_part(n - k, k);
	res = number_less_than_k(n - k, k) + number_less_than_k(n, k - 1);
	parts[pair<uint64, uint64>(n,k)] = res;
	return res;
}

uint64 Partitions::number_of_partitions(uint64 n, uint64 k, uint64 k1)
{
	return number_less_than_k(n, k) - number_less_than_k(n, k1);
}

/*
uint64 Partitions::part_num_slow(uint64 n)
{

	uint64 S, J, k, T;

	if (!n)
		return 1;
	S = 0;
	J = n - 1;
	k = 2;
	while (J >= 0)
	{
		T = part_num_bad(J);
		if ((k / 2) % 2 == 1)
			S += T;
		else
			S -= T;
		if (k % 2 == 1)
			J -= k;
		else
			J -= k / 2;
		k++;
	}

	return S;
}
*/



void Partitions::print_partitions(uint64 last)
/*
	Вывод числа разбиений (до last)
*/
{
	ofstream razb;
	razb.open("razb.txt");
	//uint64 tmp2;
	for (uint64 i = 1; i <= last; ++i)
	{
		//tmp2 = part_num_slow(i);
		//cout << i << " " << tmp2 << " " << bit_num(tmp2) << endl;
		//razb << i << " " << tmp2 << " " << bit_num(tmp2) << endl;
	}
	razb.close();
}

void Partitions::init()
{
	string line;
	string fname = "part_n_k_1000.txt";
	ifstream f(fname);

	if (!f.is_open())
	{
		std::cout << "Unable to open the file" << std::endl;
		exit(EXIT_FAILURE);
	}

	cout << "Reading from file " << fname << " ... ";
	while (getline(f, line, '\n')) {
		stringstream ss(line);
		vector<uint64> numbers;
		string in_line;
		while (getline(ss, in_line, ' ')) {
			numbers.push_back(std::stoull(in_line, 0));
		}
		parts_table.push_back(numbers);
	}
	cout << "complete!" << endl;
	f.close();
}

uint64 Partitions::get_part_num(uint64 n, uint64 k, uint64 k1)
//Number of partitions for width ???<= k!
{
	if ((n > parts_table.size()) || (k > parts_table.at(0).size()) || (k1 > k))
	{
		cout << "Error! (get_part_num)" << endl;
		//system("pause");
		exit(EXIT_FAILURE);
	}
	//return parts_table.at(n).at(k);
	return parts_table.at(n).at(k) - parts_table.at(n).at(k1);
}

uint64 Partitions::get_part_num(uint64 n)
//returns p(n)
{
	di r;
	Diagram* diag;

	diag = new Diagram();
	r.insert(r.end(), n, 1);
	diag->set_rows(r);
	return Enumerator::N_destroy(diag, n, n);
}
