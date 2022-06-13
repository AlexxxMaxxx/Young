#include "Enumerator.h"

Diagram* Enumerator::last_diag = new Diagram();
uint64 Enumerator::counter = 0;

uint64 Enumerator::N(Diagram* Y)
{
	return N(Y, Y->get_size(), Y->get_size());
}

uint64 Enumerator::N(Diagram* Y, uint64 n, uint64 k)
{
	uint64 res;
	Diagram* diag = new Diagram(Y);
	res = N_destroy(diag, n, k);
	delete diag;
	return res;
}

uint64 Enumerator::N_destroy(Diagram* Y, uint64 n, uint64 k)
//Номер диаграммы Y из n клеток с учётом того, что ширина не более k.
//Диаграмма Y в процессе нумерации уничтожается
{
	uint64 k1;
	uint64 sum;

	k1 = Y->get_width();

	if (k1 > k)
	{
		throw("Enumeration::N_destroy");
	}

	if (Y->is_flat())
		return 1;

	Y->pop_bottom();
	sum = N_destroy(Y, n - k1, k1);
	if (k1 < k)
		sum += Partitions::get_part_num(n, k, k1);	

	return sum;
}

uint64 Enumerator::N_slow(Diagram* Y, uint64 n, uint64 k)
{
	uint64 k1;
	uint64 sum;

	k1 = Y->get_width();

	if (k1 > k)
	{
		throw("Enumeration::N_slow");
	}

	if (Y->is_flat())
		return 1;

	Y->pop_bottom();
	sum = N_slow(Y, n - k1, k1);
	if (k1 < k)
	{
		last_diag->set_to_last(n, k1 + 1);      //делаем уголок, столько же клеток, но основание++
		sum += N_slow(last_diag, n, k);              //Определяем номер этого уголка, при исходном основании
		//Y = new Diagram(n, k1 + 1); //создаётся "уголок"
		//sum += N(Y, n, k);
		//delete Y;
	}
	return sum;
}

uint64 Enumerator::diag2num(Diagram* diag, uint64 n)
//ТОЛЬКО ИЗ ТЕСТЕРА!!
{
	Diagram* d = new Diagram();
	d->set_rows(*(diag->get_rows()));
	return N_destroy(d, n, n);
}

uint64 Enumerator::diag2num(Diagram* diag, uint64 n, uint64 k)
//ТОЛЬКО ИЗ ТЕСТЕРА!!
{
	Diagram* d = new Diagram();
	d->set_rows(*(diag->get_rows()));
	return N_destroy(d, n, k);
}