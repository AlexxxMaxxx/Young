/*
 * Strategy.h
 *
 *  Created on: Aug 2, 2015
 *      Author: vduzhin
 */

#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "StrictDiagram.h"
#include "Process.h"
#include <sstream>
#include <list>

using namespace std;

class Strategy
{
	protected:
		//const char* NAME;
		const int DEPTH;
		const string NAME;

		uint64 best_counter = 0;
		StrictDiagram *diag, *diag_old;
		mpz_t dim, dim_old;
	public:
		Strategy(const int D);
		virtual ~Strategy();

		virtual void execute() = 0;
		virtual void increase_size();
		virtual void print_cols();
		void get_dim(mpz_t d);
		uint64 get_width();
		virtual string get_name() = 0;
		void inc_best() { ++best_counter; }
		uint64 get_best_counter() { return best_counter; }
};

class Strategy1 : public Strategy
{
	public:
		Strategy1(const int D) : Strategy(D) {};
		void execute();
		string get_name();
};

class Strategy2 : public Strategy
{
	public:
		Strategy2(const int D) : Strategy(D) {};
		void execute();
		string get_name() {	return "Many_prob.jumps_of_size_=_" + to_string(DEPTH); }
};

class Strategy3 : public Strategy
{
	private:
		void increase_width();
	public:
		Strategy3(const int D) : Strategy(D) {};
		void execute();
		string get_name() {return string("Many_prob.jumps_of_size_<=_") + to_string(DEPTH) + "._+_Return.";}
};

class Strategy4 : public Strategy
{
	public:
		Strategy4(const int D) : Strategy(D) {};
		void increase_size();
		void execute();
		string get_name();
};

class Strategy5 : public Strategy
{
	public:
		Strategy5(const int D) : Strategy(D) {};
		void increase_size();
		void execute();
		string get_name() {return string("Many_dim.jumps_of_size_<=_") + to_string(DEPTH) + "._+_Return.";}
};

class DiagsFromFile : public Strategy
{
	private:
		string fname;
		uint64 size;
		fstream f;
		di cols;
	public:
		DiagsFromFile(const int D, string fn) : Strategy(D)
		{
			fname = fn;
			size = 1;
			f.open(fname, fstream::in);
		}

		~DiagsFromFile();
		void increase_size() {};
		void execute();
		string get_name() {return fname;}
		void print_cols();
};

class Sprouts : public Strategy
{
	typedef list<StrictDiagram*> ld;

	const bool MANY_JUMPS = true;
	const bool SHAKE_ONLY_BEST = true;

	const bool SAVE_DIAGS = false;
	const bool SAVE_TABLES = false;

	bool LONG_ARITHMETICS;
	uint64 MAX_SPROUTS;

	private:
		deque<StrictDiagram*> spr1, spr2; //vectors of old and new sprouts
		ld list1, list2;
		fstream f_diags, f_tab;
		mpz_t* dims;
		mpz_t min_dim, max_dim;
		uint64 min_ind, max_ind, ind1, ind2;
		vector<di> tabs1, tabs2; //tables of the current sprouts

	public:
		Sprouts(const int D, uint64 num_of_sprouts, bool fast = true) : Strategy(D)
		{
			uint64 i;
			StrictDiagram* d;

			if (SAVE_DIAGS)
			{
				string fname;
				fname = string("sprouts") + to_string(num_of_sprouts) + string(".txt");
				f_diags.open(fname.c_str(), fstream::out | fstream::trunc);
			}
			if (SAVE_TABLES)
				f_tab.open("sprouts_tables.txt", fstream::out | fstream::trunc);

			MAX_SPROUTS = num_of_sprouts;

			LONG_ARITHMETICS = !fast;

			mpz_init_set_ui(min_dim, 1);
			mpz_init_set_ui(max_dim, 1);
			min_ind = 0;
			max_ind = 0;
			ind1 = 1;
			ind2 = 0;

			tabs1.resize(MAX_SPROUTS);
			tabs2.resize(MAX_SPROUTS);

			if (LONG_ARITHMETICS)
			{
				dims = (mpz_t*)malloc(MAX_SPROUTS * sizeof(mpz_t));
				for (i = 0; i < MAX_SPROUTS; ++i)
				{
					d = new StrictDiagram();
					spr1.push_back(d);
					d = new StrictDiagram();
					spr2.push_back(d);
					mpz_init(dims[i]);
				}
			}
			else
			{
				for (i = 0; i < MAX_SPROUTS; ++i)
				{
					list1.push_back(new StrictDiagram());
					list2.push_back(new StrictDiagram());
				}
			}
		}
		~Sprouts()
		{
			uint64 i;

			if (SAVE_DIAGS)
				f_diags.close();
			if (SAVE_TABLES)
				f_tab.close();

			if (LONG_ARITHMETICS)
			{
				mpz_clear(min_dim);
				mpz_clear(max_dim);
				for (i = 0; i < MAX_SPROUTS; ++i)
				{
					delete spr1.at(i);
					delete spr2.at(i);
				}
				free(dims);
			}
			else
			{
				for (i = 0; i < MAX_SPROUTS; ++i)
				{
					delete list1.front();
					delete list2.front();
					list1.pop_front();
					list2.pop_front();
				}
			}
		}
		void execute();
		void execute_slow();
		void execute_fast();
		string get_name() {return to_string(MAX_SPROUTS) + string("_sprouts._jump=") + to_string(DEPTH);}
		void increase_size();
		void increase_size_slow();
		void increase_size_fast();
		void put_diag_in_spr2(uint64 old_index, uint64 point);
		void put_diag_in_list(ld *list, uint64 *ind);
		bool diag_is_in_spr2();
};

class Sprouts2 : public Strategy
{
	typedef list<StrictDiagram*> ld;

	private:
		const char* FNAME_BANK = "strict_bank/strict_bank.txt";
		const char* FNAME_RES  = "strict_bank/res.txt";
		const bool SCURPULOUS = false;
		const uint64 WIN_SIZE = 50000; //через сколько шагов загружать новые диаграммы, а старые отгружать
		const uint64 NUM_DIFF = 3; //на сколько <= клеток будут отличаться сгенерированные от исходной
		const uint64 MAX_SPROUTS = 100; //макс. количество ветвей
		const uint64 GEN_NUM = MAX_SPROUTS; //сколько раз генерировать похожие диаграммы
		const uint64 BANK_MAX = 1000000; //максимальный размер диаграммы в банке

		vector<di> best_diags;
		fstream f_bank;
		ld list1, list2;
		uint64 ind1, ind2;

		void generate_close_diags(di cols);
		void put_diag_in_list(ld *list, uint64 *ind);
	public:
		Sprouts2(const int D) : Strategy(D)
		{
			uint64 i, value;
			string str;
			di cols;

			best_diags.push_back(cols); //заполняем ноль

			f_bank.open(FNAME_BANK, ifstream::in);
			//1. Заполняем массив начальными лучшими диаграммами из банка
			if (SCURPULOUS)
				for (i = 0; i < WIN_SIZE*2; ++i)
				{
					if (!getline(f_bank, str))
					{
						cout << "Error: the bank size is too small.\n";
						exit(1);
					}
					istringstream is (str);
					while (is >> value)
						cols.push_back(value);
					best_diags.push_back(cols);
					cols.clear();
				}

			//2. Инициализация list1, list2
			for (i = 0; i < MAX_SPROUTS; ++i)
			{
				list1.push_back(new StrictDiagram());
				list2.push_back(new StrictDiagram());
			}
			ind1 = 0;
			ind2 = 0;
		}

		~Sprouts2()
		{
			uint64 i;

			f_bank.close();
			for (i = 0; i < MAX_SPROUTS; ++i)
			{
				delete list1.front();
				delete list2.front();
				list1.pop_front();
				list2.pop_front();
			}
		}
		void increase_size();
		void increase_size_scurpulous();
		//void increase_size_verbose_OLD();
		void execute();
		string get_name() {return string("Sprouts2");}
};

#endif /* STRATEGY_H_ */
