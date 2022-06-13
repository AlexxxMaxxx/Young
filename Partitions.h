#pragma once
#include <map>
#include <sstream>
#include "Common.h"
#include "Diagram.h"
#include "Enumerator.h"

class Partitions
{
	private:
		static map<pair<uint64, uint64>, uint64> parts;
		static uint64 number_less_than_k(uint64 n, uint64 k);
		static vector < vector<uint64> > parts_table;
	public:
		static void init();
		static void part_n_k_to_file(uint64 n, uint64 k, char* fname);
		static void print_partitions(uint64 last);
		static uint64 number_of_partitions(uint64 n, uint64 k, uint64 k1);
		static uint64 get_part_num(uint64 n);
		static uint64 get_part_num(uint64 n, uint64 k, uint64 k1);
		//static uint64 part_num_slow(uint64 n);
};

