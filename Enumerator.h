#pragma once
#include "Diagram.h"
#include "Common.h"
#include "Partitions.h"

class Enumerator
{
	private:
		static uint64 counter;
		static Diagram* last_diag;
	public:
		static uint64 N(Diagram* Y);
		static uint64 N(Diagram* Y, uint64 n, uint64 k);
		static uint64 N_destroy(Diagram* Y, uint64 n, uint64 k);
		static uint64 N_slow(Diagram* Y, uint64 n, uint64 k);
		static uint64 diag2num(Diagram* diag, uint64 n);
		static uint64 diag2num(Diagram* diag, uint64 n, uint64 k);
};

