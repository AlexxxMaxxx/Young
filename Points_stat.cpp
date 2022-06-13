#include "Points_stat.h"

/*
//Плохо, что при добавлении новой строчки - та строка, что была 0-й
//становится 1-й, т.к. придётся инкрементировать points.
//Надо исправить ВЕЗДЕ

Возможная ошибка:
+
+
++    - Код: 2 0 1 0
++

+
+
+     - Код: 2 0 1 0 0
++
++

Если в конце отбрасывать нули (как делается сейчас), то получится одинаковые коды. Но число клеток отличается, так что норм.
*/

Points_stat::Points_stat()
{
}


Points_stat::~Points_stat()
{
}
/*
void get_points(const deque<uint64>& diag, uint64 N, vector<uint64>& points)
{
uint64 last_row = diag.size() - 1;
uint64 last = diag.at(last_row);
uint64 next_row;
uint64 i;
uint64 psize;

points.clear();
points.push_back(last);
if (last_row != 0)
do
{
next_row = last_row - 1;
if (diag.at(next_row) != diag.at(last_row))
points.push_back(diag.at(next_row));
else points.push_back(0);
last_row = next_row;
} while (next_row != 0);
psize = points.size();
for (i = 0; i < N - psize; ++i)
points.push_back(0);

}
*/




/*
void get_points_stat(uint64 depth)

//TBD: попробовать кодировать точки не по длине, а по высоте. Проверить область значений разрядов.

{
	uint64 N;
	vector<deque<uint64>> diags;
	vector<uint64> points;
	uint64 i;
	vector<uint64> maxes;
	uint64 width;
	ofstream f_pstat, f_pstat2, f_log;
	uint64 indent;
	uint64 sum_of_bits;

	f_pstat.open("points_stat.txt");
	f_pstat2.open("points_stat2.txt");
	f_log.open("points_stat_log.txt");

	width = num_digits(depth) + 2;

	for (N = 1; N <= depth; ++N)
	{
		maxes.clear();
		maxes.resize(N);

		get_diags(N, diags);
		for (vector<deque<uint64>>::iterator it1 = diags.begin(); it1 != diags.end(); ++it1)
		{
			get_points(*it1, N, points);
			for (i = 0; i < N; ++i)
				if (maxes.at(i) < points.at(i))
					maxes.at(i) = points.at(i);
			
			//for (vector<uint64>::iterator it2 = points.begin(); it2 != points.end(); ++it2)
			//cout << (*it2) << " ";
			//cout << endl;
			
		}
		//for (i = 0; i < (uint64)(depth + 1)/2 - 1 - (N - 1) / 2; ++i)
		//	f_pstat2 << "  ";

		sum_of_bits = bit_num(N);
		indent = ((uint64)(depth + 1) / 2 - 1 - (N - 1) / 2)*(width + 1) + 2;
		cout << setw(width) << N;
		f_pstat << setw(width) << N << " ";
		f_pstat2 << "  " << setw(indent) << N << " ";
		f_log << setw(width) << N << "|" << setw(width) << bit_num(N) << " ";

		for (vector<uint64>::iterator it2 = ++(maxes.begin()); (it2 != maxes.end()) && (*it2 != 0); ++it2)
		{
			cout << setw(width) << (*it2) + 1;
			f_pstat << setw(width) << (*it2) + 1 << " ";
			f_pstat2 << setw(width) << (*it2) + 1 << " ";
			f_log << setw(width) << bit_num((*it2) + 1) << " ";
			sum_of_bits += bit_num((*it2) + 1);
		}
		f_log << setw(width);// << bit_num(part_num(N));
		f_log << setw(indent) << "|" << setw(3) << sum_of_bits;
		cout << endl;
		f_pstat << endl;
		f_pstat2 << endl;
		f_log << endl;
	}
	f_pstat.close();
	f_pstat2.close();
	f_log.close();
}

*/