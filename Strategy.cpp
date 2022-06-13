/*
 * Strategy.cpp
 *
 *  Created on: Aug 2, 2015
 *      Author: vduzhin
 */

#include "Strategy.h"

using namespace std;

Strategy::Strategy(const int D) : DEPTH(D)
{
//	cout << "Constructor. diag_before = " << diag;

	diag = new StrictDiagram();

//	cout << "after = " << diag << endl;

	diag_old = new StrictDiagram();
	mpz_init(dim);
	mpz_init(dim_old);
//
//
	//
//
	mpz_set_ui(dim, 0);
//
//
//
//
}

Strategy::~Strategy()
{
	delete diag;
	delete diag_old;
	mpz_clear(dim);
	mpz_clear(dim_old);
}

void Strategy::print_cols()
{
	diag->print_cols();
}

uint64 Strategy::get_width()
{
	return diag->get_width();
}

void Strategy::get_dim(mpz_t d)
{
	mpz_set(d, dim);
}

void Strategy::increase_size()
{
	Process::strict_probgreedy_plansh(1, diag);
}

string Strategy1::get_name()
{
	if (DEPTH == 0)
		return "Greedy_(probability)";
	else
		return "A_single_prob-jump_of_size_=_" + to_string(DEPTH);
}

void Strategy1::execute()
/*
 * Делается ОДИН скачок на глубину DEPTH. Если размерность ухудшилась,то оставляем предыдущую диаграмму.
 */
{
	int i;

	diag_old->init(diag);
	diag_old->calc_strict_dim(dim_old);

	//1. Жадный рост
	Process::strict_probgreedy_plansh(DEPTH, diag);

	//2. Движение назад
	for (i = 0; i < DEPTH; ++i)
		diag->remove_min();

	//3. Вычисляем размерности диаграмм
	diag->calc_strict_dim(dim);

	//4. Если размерность ухудшилась, то оставляем диаграмму до скачка
	if (mpz_cmp(dim_old, dim) > 0)
	{
		diag->init(diag_old);
		mpz_set(dim, dim_old);
	}

	//gmp_printf("%Zd ", dim);
	//cout << diag << " ";
}

void Strategy2::execute()
/*
 * Делаются скачки на глубину DEPTH, пока размерность не перестанет улучшаться.
 */
{
	int i;
	int check;

	//diag, dim_old.

	diag_old->init(diag);
	diag_old->calc_strict_dim(dim_old);
	do
	{
		//1. Жадный рост
		Process::strict_probgreedy_plansh(DEPTH, diag);

		//2. Движение назад
		for (i = 0; i < DEPTH; ++i)
			diag->remove_min();

		//3. Вычисляем размерность диаграммы
		diag->calc_strict_dim(dim);

		//4.
		check = mpz_cmp(dim_old, dim);
		if (check > 0)
		{
			//Если размерность ухудшилась, то оставляем диаграмму до скачка
			diag->init(diag_old);
			mpz_set(dim, dim_old);
		}
		else
			if (check < 0)
			{
				//Если улучшилась
				diag_old->init(diag);
				mpz_set(dim_old, dim);
			}
	}
	while (check < 0); //продолжаем только, если размерность улучшилась

	//gmp_printf("%Zd ", dim);
	//cout << diag << " ";
}

void Strategy3::increase_width()
{
	di* cols;
	uint64 last_col;
	int check;

	cols = diag->get_cols();
	last_col = cols->at(cols->size() - 1);

	if (last_col > 1)
	{
		diag_old->init(diag);
		mpz_set(dim_old, dim);

		diag->add(0);
		diag->remove_min();

		diag->calc_strict_dim(dim);
		check = mpz_cmp(dim, dim_old);
		if (check < 0)
		{
			diag->init(diag_old);
			mpz_set(dim, dim_old);
		}
	}
}

void Strategy3::execute()
/*
 * Делаются скачки на глубину не более DEPTH.
 * Если размерность улучшилась, сбрасываем глубину прыжка до 1
 */
{
	//uint64 i;
	int i, check, depth;

	if (DEPTH < 1)
		return;

	depth = 1; //текущая глубина

	diag_old->init(diag);
	diag_old->calc_strict_dim(dim_old);
	do
	{
		//1. Жадный рост
		Process::strict_probgreedy_plansh(depth, diag);

		//2. Движение назад
		for (i = 0; i < depth; ++i)
			diag->remove_min();

		//3. Вычисляем размерность диаграммы
		diag->calc_strict_dim(dim);

		//4. Проверяем, как изменилась размерность после скачка
		check = mpz_cmp(dim_old, dim);
		if (check > 0)
		{
			//Если размерность ухудшилась, то оставляем диаграмму до скачка
			diag->init(diag_old);
			mpz_set(dim, dim_old);
		}
		else
			if (check < 0)
			{
				//Если улучшилась, новая становится старой, переходим на глубину 1
				diag_old->init(diag);
				mpz_set(dim_old, dim);
				depth = 1;
				continue;
			}
		++depth;
	}
	while (depth <= DEPTH);

	//Пробуем добавить справа клетку, удаляем с минимальной вероятностью.
	//Если новая диаграмма лучше, сохраняем её.
	//increase_width();

	//cout << diag << " ";
	//gmp_printf("%Zd ", dim);
}

string Strategy4::get_name()
{
	if (DEPTH == 0)
		return "Greedy_(dimension)";
	else
		return "A_single_dim-jump_of_size_=_" + to_string(DEPTH);
}

void Strategy4::execute()
{
	diag_old->init(diag);
	//diag_old->calc_strict_dim(dim_old);

	//1. Жадный рост
	diag->add_maxdim_double(DEPTH);

	//2. Движение назад
	diag->remove_min_double(DEPTH);

	//3. Вычисляем размерности диаграмм
	//diag->calc_strict_dim(dim);

	//4. Если размерность ухудшилась, то оставляем диаграмму до скачка
	//if (mpz_cmp(dim_old, dim) > 0)
	if (diag->comp_dim(diag_old) < 1)
	{
		diag->init(diag_old);
		//mpz_set(dim, dim_old);
	}

	//diag->print_cols();
	//cout << diag << endl;
	//gmp_printf("%Zd\n", dim);
}

void Strategy4::increase_size()
{
	diag->add_maxdim_double(1);
}

void Strategy5::increase_size()
{
	diag->add_maxdim_double(1);
}

void Strategy5::execute()
{
	int check, depth;

	if (DEPTH < 1)
		return;

	depth = 1; //текущая глубина

	diag_old->init(diag);
	diag_old->calc_strict_dim(dim_old);
	do
	{
		//1. Жадный рост
		diag->add_maxdim_double(depth);

		//2. Движение назад
		diag->remove_min_double(depth);

		//3. Вычисляем размерность диаграммы
		diag->calc_strict_dim(dim);

		//4. Проверяем, как изменилась размерность после скачка
		check = mpz_cmp(dim_old, dim);
		if (check > 0)
		{
			//Если размерность ухудшилась, то оставляем диаграмму до скачка
			diag->init(diag_old);
			mpz_set(dim, dim_old);
		}
		else
			if (check < 0)
			{
				//Если улучшилась, новая становится старой, переходим на глубину 1
				diag_old->init(diag);
				mpz_set(dim_old, dim);
				depth = 1;
				continue;
			}
		++depth;
	}
	while (depth <= DEPTH);

	//Пробуем добавить справа клетку, удаляем с минимальной вероятностью.
	//Если новая диаграмма лучше, сохраняем её.
	//increase_width();

	//cout << diag << " ";
	//gmp_printf("%Zd ", dim);

}

DiagsFromFile::~DiagsFromFile()
{
	f.close();
}

void DiagsFromFile::execute()
{
	string line;

	if(getline(f, line))
	{
		stringstream ss(line);
		string in_line;

		cols.clear();
		while (getline(ss, in_line, ' '))
			cols.push_back(std::stoull(in_line, 0));

		diag->set_cols_and_size(cols, size++);
		diag->calc_strict_dim(dim);
	}
	else
		cout << "DiagsFromFile::execute(): ERROR reading from file " << fname << endl;
}

void DiagsFromFile::print_cols()
{
	for (di::iterator it = cols.begin(); it != cols.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

bool Sprouts::diag_is_in_spr2()
{
	uint64 i;

	for (i = 0; i < ind2; ++i)
		if (spr2.at(i)->is_equal_to(diag))
			return true;
	return false;
}

void Sprouts::execute()
{
	if (LONG_ARITHMETICS)
		execute_slow();
	else
		execute_fast();
}

void Sprouts::execute_fast()
{
	/*
	cout << "n = " << list1.front()->get_size() << endl;
	cout << "ind1 = " << ind1 << ", ind2 = " << ind2 << endl;
	for (ld::iterator it = list1.begin(); it != list1.end(); ++it)
		(*it)->print_cols();
	cout << endl;
	for (ld::iterator it = list2.begin(); it != list2.end(); ++it)
		(*it)->print_cols();
	cout << endl;
	*/
	list1.front()->print_cols();

	//diag->init(list1.front());
	//diag->calc_strict_dim(dim);
}

void Sprouts::execute_slow()
{
	/*
	 * БАГ: из разных sprouts таблицы валятся в одинаковый файл!
	 * хотя сейчас все равно cout...
	 */
	di::iterator it;

	diag->init(spr1.at(max_ind));

	if (SAVE_DIAGS)
		diag->print_cols(f_diags);

	if (SAVE_TABLES)
	{
		for (it = tabs1.at(max_ind).begin(); it != tabs1.at(max_ind).end(); ++it)
			cout << *it << " ";
		cout << endl;
	}
	mpz_set(dim, max_dim);
}

void Sprouts::put_diag_in_spr2(uint64 old_index, uint64 point)
{
	uint64 j;

	//Новой диаграммы нет в spr2
	if (!diag_is_in_spr2())
	{
		//cout << "No diag in spr2\n\n";
		//Размерность новой диаграммы
		diag->calc_strict_dim(dim);

		//Массив вторых ростков переполнен
		if (ind2 == MAX_SPROUTS)
		{
			//cout << "Spr2 is full.\n";
			//Новая диаграмма лучше худшей
			if (mpz_cmp(dim, min_dim) > 0)
			{
				//cout << "The new diagram is better than worse one\n";
				spr2.at(min_ind)->init(diag);
				mpz_set(dims[min_ind], dim);

				tabs2.at(min_ind) = tabs1.at(old_index);
				tabs2.at(min_ind).push_back(point);

				//Новая диаграмма лучше лучшей
				if (mpz_cmp(dim, max_dim) > 0)
				{
					mpz_set(max_dim, dim);
					max_ind = min_ind;
				}

				//Ищем новый минимум
				mpz_set(min_dim, max_dim);
				for (j = 0; j < MAX_SPROUTS; ++j)
					if (mpz_cmp (dims[j], min_dim) < 0)
					{
						mpz_set(min_dim, dims[j]);
						min_ind = j;
					}
			}
		}
		else
		{
			//Добавляем новую диаграмму
			spr2.at(ind2)->init(diag);
			//Модифицируем массив dims
			mpz_set(dims[ind2], dim);

			tabs2.at(ind2) = tabs1.at(old_index);
			tabs2.at(ind2).push_back(point);


			//Модифицируем min, max
			if (mpz_cmp(dim, min_dim) < 0)
			{
				mpz_set(min_dim, dim);
				min_ind = ind2;
			}
			if (mpz_cmp(dim, max_dim) > 0)
			{
				mpz_set(max_dim, dim);
				max_ind = ind2;
			}
			++ind2;
		}
	}
	//else
	//	cout << "Diag is in spr2!\n\n";
}

void Sprouts::increase_size()
{
	if (LONG_ARITHMETICS)
		increase_size_slow();
	else
		increase_size_fast();
}


void Sprouts::increase_size_slow()
{
	mi* p;
	uint64 i;
	mi::iterator it;
	//di old_cols, new_cols;




	//1. Обновляем max_dim, max_ind, min_dim, min_ind:

	//cout << spr1.front() << " " << diag << endl;

	//for (i = 0; i < MAX_SPROUTS; ++i)
	//{
	//	cout << spr1.at(i) << " " << spr2.at(i) << endl;
	//}

	//cout << diag << endl;

	diag->init(spr1.front());

	//cout << diag << endl << endl;

	p = diag->get_points();
	diag->add(p->begin()->first);
	diag->calc_strict_dim(max_dim);
	mpz_set(min_dim, max_dim);
	max_ind = 0;
	min_ind = 0;



/*
	cout << "BEFORE sprout growth:\n";
	cout << "spr1:\n";
	for (i = 0; i < ind1; ++i)
		spr1.at(i)->print_cols();
	cout << "spr2:\n";
	for (i = 0; i < ind2; ++i)
		spr2.at(i)->print_cols();
	cout << "min_ind: " << min_ind << ", max_ind: " << max_ind << endl << endl;
*/




	//2. Цикл по старым росткам
	for (i = 0; i < ind1; ++i)
	{

		diag_old->init(spr1.at(i));

		p = diag_old->get_points();
		diag->init(diag_old);



		//Точки в старом ростке
		for (it = p->begin(); it != p->end(); ++it)
		{
			//++росток
			diag->add(it->first);

			//cout << "A new sprout was made.\n";

			put_diag_in_spr2(i, it->first);

			//old_cols = *(diag->get_cols());

			if (DEPTH > 0)
			{
				//1. Жадный рост
				diag->add_maxdim_mpq(DEPTH);
				//2. Движение назад
				diag->remove_min_mpq(DEPTH);

				//new_cols = *(diag->get_cols());

				/*
				for (di::iterator it = old_cols->begin(); it != old_cols->end(); ++it)
					cout << *it << " ";
				cout << endl;
				for (di::iterator it = new_cols->begin(); it != new_cols->end(); ++it)
					cout << *it << " ";
				cout << endl << endl;
*/

				//if (old_cols == new_cols)
				//{
					//cout << "Diag didn't change after shaking\n";
					//continue;
				//}
				//else
				//	cout << "New diag after shaking.\n";
				put_diag_in_spr2(i, it->first);
			}

			//Возвращаем диаграмму к исходному состоянию
			//diag->remove(it->first);
			diag->init(diag_old);
		}
	}
/*
	cout << "AFTER sprout growth:\n";
	cout << "spr1:\n";
	for (i = 0; i < ind1; ++i)
		spr1.at(i)->print_cols();
	cout << "spr2:\n";
	for (i = 0; i < ind2; ++i)
		spr2.at(i)->print_cols();
	cout << "min_ind: " << min_ind << ", max_ind: " << max_ind << endl << endl;
*/

/*
	for (vector<di>::iterator it1 = tabs2.begin(); it1 != tabs2.end(); ++it1)
	{
		for (di::iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
			cout << *it2 << " ";
		cout << endl;
	}
*/
	//for (di::iterator it2 = tabs1.at(0).begin(); it2 != tabs1.at(0).end(); ++it2)
	//	cout << *it2 << " ";
	//cout << " | ";
	//for (di::iterator it2 = tabs2.at(0).begin(); it2 != tabs2.at(0).end(); ++it2)
	//	cout << *it2 << " ";
	//cout << endl;

	//Новые ростки становятся старыми
	tabs1 = tabs2;
	for (i = 0; i < ind2; ++i)
	{
		spr1.at(i)->init(spr2.at(i));
		tabs2.at(i).clear();
	}
	ind1 = ind2;
	ind2 = 0;
}

void Sprouts::increase_size_fast()
{
	mi* p;
	mi::iterator it;
	ld::iterator old_spr, new_spr, limit;
	int d, check;
	StrictDiagram diag2;

	limit = list1.begin();
	advance(limit, ind1);

	//Старые ростки
	for (old_spr = list1.begin(); old_spr != limit; ++old_spr)
	{
		diag_old->init(*old_spr);

		p = diag_old->get_points();
		diag->init(diag_old);

		//Точки в старом ростке
		for (it = p->begin(); it != p->end(); ++it)
		{
			//++росток
			diag->add(it->first);

			put_diag_in_list(&list2, &ind2);

			if ((DEPTH > 0) && (!SHAKE_ONLY_BEST))
			{
				if (MANY_JUMPS)
					for (d = 1; d <= DEPTH; ++d)
					{
						diag->add_maxdim_double(d);
						diag->remove_min_double(d);
						put_diag_in_list(&list2, &ind2);
					}
				else
				{
					diag->add_maxdim_double(DEPTH);
					diag->remove_min_double(DEPTH);
					put_diag_in_list(&list2, &ind2);
				}
			}

			//Возвращаем диаграмму к исходному состоянию
			diag->init(diag_old);
		}
	}
/*
	cout << "ind1: " << ind1 << endl;
	cout << "list1:\n";
	for (old_spr = list1.begin(); old_spr != list1.end(); ++old_spr)
		(*old_spr)->print_cols();
	cout << endl;
	cout << "ind2: " << ind2 << endl;
	cout << "list2:\n";
	for (old_spr = list2.begin(); old_spr != list2.end(); ++old_spr)
		(*old_spr)->print_cols();
	cout << endl;
*/

	limit = list2.begin();
	advance(limit, ind2);

	//Новые ростки становятся старыми
	for (old_spr = list1.begin(), new_spr = list2.begin(); new_spr != limit; ++old_spr, ++new_spr)
		(*old_spr)->init(*new_spr);
	ind1 = ind2;

	if (SHAKE_ONLY_BEST)
	{
		for (new_spr = list2.begin(); new_spr != limit; ++new_spr)
			if (MANY_JUMPS)
			{
				diag->init(*new_spr);
				diag2.init(diag);

				for (d = 1; d <= DEPTH; ++d)
				{
					diag->add_maxdim_double(d);
					diag->remove_min_double(d);

					//cout << "d = " << d << endl;
					//diag2.print_cols();
					//diag->print_cols();
					//cout << diag->comp_dim(&diag2) << endl;
					//cout << endl;

					check = diag->comp_dim(&diag2);
					if (check > 1)
					{
						put_diag_in_list(&list1, &ind1);
						d = 0;
						diag2.init(diag);
					}
					else
						if (check < 1)
							diag->init(&diag2);
				}
			}
			else
			{
				diag->init(*new_spr);
				diag->add_maxdim_double(DEPTH);
				diag->remove_min_double(DEPTH);
				put_diag_in_list(&list1, &ind1);
			}
	}

	ind2 = 0;
}

void Sprouts::put_diag_in_list(ld *list, uint64 *ind)
{
	ld::iterator it, last, it2;
//	StrictDiagram* swap_diag;


	//Проверка, есть ли диаграмма в list2
	last = list->begin();
	advance(last, *ind);
	for(it = list->begin(); it != last; ++it)
		if ((*it)->is_equal_to(diag))
			return;

	//Массив вторых ростков переполнен
	if (*ind == MAX_SPROUTS)
	{
		//diag лучше худшей
		if (diag->comp_dim(list->back()) > 1)
		{
			list->back()->init(diag);

			//ищем, куда нужно положить diag:
			for(it = list->begin(); it != --(list->end()); ++it)
			{
				//У diag размерность больше
				if (diag->comp_dim(*it) > 1)
				{
					//swap_diag = *it;
					//*it = list2.back();
					//list2.back() = swap_diag;

					list->insert(it, --list->end(), list->end());
					list->pop_back();
					break;
				}
			}

/*
			swap_diag = list2.back();
			last = it;
			for (; it != --(list2.end()); ++it)
			{
				it2 = it;
				++it2;
				*it2 = *it;
			}
			*last = diag;
			diag = swap_diag;
*/
		}
	}
	else
	{
		//Добавляем новую диаграмму

		last = list->begin();
		advance(last, *ind);

		list->back()->init(diag);

		//ищем, куда добавить диаграмму
		for(it = list->begin(); it != last; ++it)
		{
			//У diag размерность больше
			if (diag->comp_dim(*it) > 1)
				break;
		}

		//cout << (*list2.end()) << " " << *(--list2.end()) << " "<< *(--list2.end()) <<endl;
/*
		cout << "ind2 = " << ind2 << endl;
		cout << "before insert:\n";
		for (last = list2.begin(); last != list2.end(); ++last)
			(*last)->print_cols();
		cout << endl;
*/
		//swap_diag = *it;
		//*it = list2.back();
		//list2.back() = swap_diag;
	/*
		swap_diag = list2.back();
		last = it;
		for (; it != --(list2.end()); ++it)
		{
			it2 = it;
			++it2;
			*it2 = *it;
		}
		*last = diag;
		diag = swap_diag;
*/

		list->insert(it, --list->end(), list->end());
/*
		cout << "after insert:\n";
		for (last = list2.begin(); last != list2.end(); ++last)
			(*last)->print_cols();
		cout << endl;
*/
		list->pop_back();




/*
		cout << "after pop_back:\n";
		for (last = list2.begin(); last != list2.end(); ++last)
			(*last)->print_cols();
		cout << endl << endl;
*/
		++(*ind);
	}
}

void Sprouts2::put_diag_in_list(ld *list, uint64 *ind)
{
	ld::iterator it, last, it2;

	//Проверка, есть ли диаграмма в list
	last = list->begin();
	advance(last, *ind);
	for(it = list->begin(); it != last; ++it)
		if ((*it)->is_equal_to(diag))
			return;

	//Массив ростков переполнен
	if (*ind == MAX_SPROUTS)
	{
		//diag лучше худшей
		if (diag->comp_dim(list->back()) > 1)
		{
			list->back()->init(diag);

			//ищем, куда нужно положить diag:
			for(it = list->begin(); it != --(list->end()); ++it)
			{
				//У diag размерность больше
				if (diag->comp_dim(*it) > 1)
				{
					list->insert(it, --list->end(), list->end());
					list->pop_back();
					break;
				}
			}
		}
	}
	else
	{
		//Добавляем новую диаграмму
		last = list->begin();
		advance(last, *ind);
		list->back()->init(diag);

		//ищем, куда добавить диаграмму
		for(it = list->begin(); it != last; ++it)
		{
			//У diag размерность больше
			if (diag->comp_dim(*it) > 1)
				break;
		}

		list->insert(it, --list->end(), list->end());
		list->pop_back();
		++(*ind);
	}
}



void Sprouts2::execute(){}

void Sprouts2::generate_close_diags(di cols)
/*
 * Генерируются случайные диаграммы, отличающиеся на NUM_DIFF клетки от лучшей и помещаются в list1
 */
{
	uint64 i, j;
	mi *p, *c;
	mi::iterator it;

	p = diag->get_points();
	c = diag->get_corners();

	for (i = 0; i < GEN_NUM; ++i)
	{
		diag->init_by_cols(cols);

		//добавляем NUM_DIFF клеток
		for (j = 0; j < NUM_DIFF; ++j)
		{
			it = p->begin();
			advance(it, rand_0_to_n_1(p->size()));
			diag->add(it->first);
		}
		//отрезаем NUM_DIFF клеток
		for (j = 0; j < NUM_DIFF; ++j)
		{
			it = c->begin();
			advance(it, rand_0_to_n_1(c->size()));
			diag->remove(it->first);
		}

		//пытаемся добавить полученную диаграмму в list1
		put_diag_in_list(&list1, &ind1);
	}

	///////////////////////
	// STAT
	//////////////////////
	//if(!list1.front()->is_equal_to_cols(cols))
	//	cout << "exh " << list1.front()->get_size() << endl;
}

void Sprouts2::increase_size()
/*
 * Находим диаграммы, похожие на данную, от каждой запускаем жадные процессы, пока они все не сольются
 * в одну диаграмму. Затем повторяем процедуру для данной диаграммы.
 * На всех промежуточных узлах сравниваем лучшую диаграмму из наших ветвей с текущей наилучшей.
 */
{
	uint64 i;
	uint64 n;
	ld::iterator it_list1, it_list2, limit;
	fstream f_res;
	string str;
	di best_cols;
	vector<di>::iterator it_vec1, it_vec2;

	if (SCURPULOUS)
	{
		increase_size_scurpulous();
		return;
	}

	f_res.open(FNAME_RES, fstream::out | fstream::trunc);


	//1. Считываем первую диаграмму
	getline(f_bank, str);
	istringstream is (str);
	best_cols.clear();
	while (is >> i)
		best_cols.push_back(i);
	n = 1;
	//2. Кладём её в list1
	diag->init_by_cols(best_cols);
	put_diag_in_list(&list1, &ind1);

	while (n < BANK_MAX)
	{
		//Генерируем похожие на лучшую диаграммы в list1
		generate_close_diags(best_cols);

		//Лучшую диаграмму этого этажа отгружаем в выходной файл:
		list1.front()->print_cols(f_res);
		//list1.front()->print_cols();

		//3. Если не были сгенерированы другие диаграммы, просто берём из банка следующую диаграмму
		if (ind1 == 1)
		{
			++n;

			getline(f_bank, str);
			istringstream is (str);
			best_cols.clear();
			while (is >> i)
				best_cols.push_back(i);

			diag->init_by_cols(best_cols);
			ind1 = 0;
			put_diag_in_list(&list1, &ind1);

			continue;
		}

		//4. Если диаграмм много, запускаем жадные процессы
		while((ind1 != 1) && (n < BANK_MAX))
		{
			//4.1. list1 ==> greedy ==> list2
			limit = list1.begin();
			advance(limit, ind1);
			for (it_list1 = list1.begin(); it_list1 != limit; ++it_list1)
			{
				diag->init(*it_list1);
				diag->add_maxdim_double(1);
				put_diag_in_list(&list2, &ind2);
			}

			//4.2. list2 ==> list1
			limit = list2.begin();
			advance(limit, ind2);
			for (it_list1 = list1.begin(), it_list2 = list2.begin(); it_list2 != limit; ++it_list1, ++it_list2)
				(*it_list1)->init(*it_list2);

			ind1 = ind2;
			ind2 = 0;
			++n;

			//Считываем текущую лучшую диаграмму
			getline(f_bank, str);
			istringstream is (str);
			best_cols.clear();
			while (is >> i)
				best_cols.push_back(i);
			diag->init_by_cols(best_cols);


			put_diag_in_list(&list1, &ind1);

			if ((ind1 == 1) && (n != BANK_MAX))
			{
				best_cols = *(list1.front()->get_cols());
			}
			else
			{
				list1.front()->print_cols(f_res);
			}
		}
		ind1 = 0;
	}

	f_res.close();
}










void Sprouts2::increase_size_scurpulous()
/*
 * Находим диаграммы, похожие на данную (размера i), от каждой запускаем жадные процессы, пока они все не сольются
 * в одну диаграмму. Затем повторяем процедуру для текущей наилучшей диаграммы размера i+1.
 * На всех промежуточных узлах сравниваем лучшую диаграмму из наших ветвей с текущей наилучшей.
 */
{
	uint64 i, j;
	uint64 n, starting_n, win_num;
	ld::iterator it_list1, it_list2, limit;
	fstream f_res;
	string str;
	di cols;
	vector<di>::iterator it_vec1, it_vec2;

	f_res.open(FNAME_RES, fstream::out | fstream::trunc);

	for (win_num = 1; win_num * WIN_SIZE <= BANK_MAX; ++win_num)
	{
		for (starting_n = 1; starting_n <= WIN_SIZE; ++starting_n)
		{
			//1. Берём диаграмму из банка и кладём её в первый список:
			diag->init_by_cols(best_diags.at(starting_n));
			put_diag_in_list(&list1, &ind1);

			//2. Генерируем похожие на лучшую диаграммы в list1
			generate_close_diags(best_diags.at(starting_n));

			//cout << (win_num-1)*WIN_SIZE+starting_n << " " << ind1 << endl;

			//3. Лучшую диаграмму этого этажа отгружаем в выходной файл:
			list1.front()->print_cols(f_res);

			//3. Запускаем жадные, пока все диаграммы не сольются, либо пока не дойдём до конца окна...
			n = starting_n;
			while((ind1 != 1) && ((win_num-1) * WIN_SIZE + n < BANK_MAX))
			{
				//list1 ==> greedy ==> list2
				limit = list1.begin();
				advance(limit, ind1);
				for (it_list1 = list1.begin(); it_list1 != limit; ++it_list1)
				{
					diag->init(*it_list1);
					diag->add_maxdim_double(1);
					put_diag_in_list(&list2, &ind2);
				}

				//list2 ==> list1
				limit = list2.begin();
				advance(limit, ind2);
				for (it_list1 = list1.begin(), it_list2 = list2.begin(); it_list2 != limit; ++it_list1, ++it_list2)
					(*it_list1)->init(*it_list2);

				ind1 = ind2;
				ind2 = 0;
				++n;

				//best from list1 ==> compare!
				diag->init_by_cols(best_diags.at(n));

				if (list1.front()->comp_dim(diag) > 1)
				{
					best_diags.at(n) = *(list1.front()->get_cols());
					cout << (win_num-1)*WIN_SIZE+starting_n << endl;
				}
			}

			ind1=0;
		}

		//Удаляем предыдущее окно
		it_vec1 = best_diags.begin();
		++it_vec1;
		it_vec2 = it_vec1;
		advance(it_vec2, WIN_SIZE);
		best_diags.erase(it_vec1, it_vec2);

		//Загружаем новое окно из файла
		if ((win_num + 1) * WIN_SIZE < BANK_MAX)
			for (i = 0; i < WIN_SIZE; ++i)
			{
				if (!getline(f_bank, str))
				{
					cout << "Error: the bank size is too small.\n";
					exit(1);
				}
				istringstream is (str);
				while (is >> j)
					cols.push_back(j);
				best_diags.push_back(cols);
				cols.clear();
			}
	}

	f_res.close();
}
/*
void Sprouts2::increase_size_verbose_OLD()
{
	uint64 i, j;
	uint64 n, starting_n, win_num;
	ld::iterator it_list1, it_list2, limit;
	fstream f_res;
	string str;
	di cols;
	vector<di>::iterator it_vec1, it_vec2;

	f_res.open(FNAME_RES, fstream::out | fstream::trunc);

	for (win_num = 1; win_num * WIN_SIZE <= BANK_MAX; ++win_num)
	{
		cout << "win_num = " << win_num << endl;
		for (starting_n = 1; starting_n <= WIN_SIZE; ++starting_n)
		{
			//if ((win_num * WIN_SIZE == BANK_MAX) && (starting_n == WIN_SIZE))
			//	break;

			cout << "\tstarting_n = " << starting_n << endl;
			//1. Берём диаграмму из банка и кладём её в первый список:
			diag->init_by_cols(best_diags.at(starting_n));

			cout << "\t\tbest diag: "; diag->print_cols();

			put_diag_in_list(&list1, &ind1);

			cout << "\t\tlist1 (init):\n";
			i=0;
			for (ld::iterator it = list1.begin(); it != list1.end(); ++it)
				if (i++ != ind1)
				{
					cout << "\t\t";
					(*it)->print_cols();
				}
				else
					break;

			//2. Генерируем похожие на лучшую диаграммы в list1
			generate_close_diags(best_diags.at(starting_n));

			//Лучшую диаграмму этого этажа отгружаем в выходной файл:
			list1.front()->print_cols(f_res);

			//выводим похожие диаграммы
			//cout << diag->get_size() << ":\n";
			cout << "\t\tlist1 (after adding similar):\n";
			i=0;
			for (ld::iterator it = list1.begin(); it != list1.end(); ++it)
				if (i++ != ind1)
				{
					cout << "\t\t";
					(*it)->print_cols();
				}
				else
					break;
			cout << endl;

			cout << "\t\tGreedy growth has started.\n";

			//3. Запускаем жадные, пока все диаграммы не сольются, либо пока не дойдём до конца окна...
			n = starting_n;
			while((ind1 != 1) && ((win_num-1) * WIN_SIZE + n < BANK_MAX))
			{
				cout << "\t\tn = " << n+1 << endl;

				//list1 ==> greedy ==> list2
				limit = list1.begin();
				advance(limit, ind1);
				for (it_list1 = list1.begin(); it_list1 != limit; ++it_list1)
				{
					diag->init(*it_list1);
					diag->add_maxdim_double(1);
					put_diag_in_list(&list2, &ind2);
				}

				//list2 ==> list1
				limit = list2.begin();
				advance(limit, ind2);
				for (it_list1 = list1.begin(), it_list2 = list2.begin(); it_list2 != limit; ++it_list1, ++it_list2)
					(*it_list1)->init(*it_list2);

				ind1 = ind2;
				ind2 = 0;
				++n;

				cout << "\t\t\tlist1:\n";
							i=0;
							for (ld::iterator it = list1.begin(); it != list1.end(); ++it)
								if (i++ != ind1)
								{
									cout << "\t\t\t";
									(*it)->print_cols();
								}
								else
									break;

				//best from list1 ==> compare!
				diag->init_by_cols(best_diags.at(n));

				cout << "\t\t\t\tbest diag from vector: ";
				diag->print_cols();
				cout << "\t\t\t\tbest diag from list1  : ";
				list1.front()->print_cols();

				if (list1.front()->comp_dim(diag) > 1)
				{
					best_diags.at(n) = *(list1.front()->get_cols());
					cout << "\t\t\t\t!!IMPROVED!!\n";
				}
				else
					cout << "\t\t\t\tNOT improved.\n";
				//cout << "\t\t\t\tind1=" << ind1 << endl;
			}

			//cout << n - starting_n << endl;

			ind1=0;
		}

		cout << "\tbest_diags before deleting first WIN:\n";
		for (i = 0; i < best_diags.size(); ++i)
		{
			cout << "\t" << i << ": ";
			for(j = 0; j < best_diags.at(i).size(); ++j)
				cout <<  best_diags.at(i).at(j) << " ";
			cout << endl;
		}
		cout << endl;

		//Удаляем предыдущее окно
		it_vec1 = best_diags.begin();
		++it_vec1;

		it_vec2 = it_vec1;
		advance(it_vec2, WIN_SIZE);
		best_diags.erase(it_vec1, it_vec2);

		cout << "\tbest_diags after deleting first WIN:\n";
		for (i = 0; i < best_diags.size(); ++i)
		{
			cout << "\t" << i << ": ";
			for(j = 0; j < best_diags.at(i).size(); ++j)
				cout << best_diags.at(i).at(j) << " ";
			cout << endl;
		}
		cout << endl;


		//Загружаем новое окно из файла
		if ((win_num + 1) * WIN_SIZE < BANK_MAX)
			for (i = 0; i < WIN_SIZE; ++i)
			{
				if (!getline(f_bank, str))
				{
					cout << "Error: the bank size is too small.\n";
					exit(1);
				}
				istringstream is (str);
				while (is >> j)
					cols.push_back(j);
				best_diags.push_back(cols);
				cols.clear();
			}

		cout << "\tbest_diags after adding next WIN:\n";
		for (i = 0; i < best_diags.size(); ++i)
		{
			cout << "\t" << i << ": ";
			for(j = 0; j < best_diags.at(i).size(); ++j)
				cout << best_diags.at(i).at(j) << " ";
			cout << endl;
		}
		cout << endl;
	}

	f_res.close();
}
*/
