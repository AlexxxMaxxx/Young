#include "Schutzenberger.h"

pu Schutzenberger::schutzenberger2D(lpu& tab, const bool randomize, dpu* sch_path, pu active)
{
	uint64 x;
	uint64 y;
	uint64 actX;
	uint64 actY;
	uint64 rnd;
	uint64 rnd2;
	
	lpu::iterator it;
	//randomization
	if (randomize)
	{
		it = tab.begin();
		if (*it == pu(0,0)) ++it;
		if (((it->second == 0) && (next(it, 1)->second == 1)) || ((it->second == 1) && (next(it, 1)->second == 0))) {
			rnd = rand_0_to_n_1(2);
			rnd2 = (rnd+1) % 2;

			it->first = rnd; it->second = rnd2;
			next(it, 1)->first = rnd2; next(it, 1)->second = rnd;
		}
	}

	if (active == pu(0,0)) {
		actX = tab.front().first;
		actY = tab.front().second;
	}
	else {
		actX = active.first;
		actY = active.second;
	}

	tab.pop_front();

	if (sch_path != NULL)
		sch_path->clear();

	it = tab.begin();
	while (it != tab.end()) {
		x = it->first;
		y = it->second;
		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY)) || ((x == actX) && (y == actY + 1)))
		{
			it = tab.erase(it);
			tab.insert(it, pu(actX, actY));
			if (sch_path != NULL)
				sch_path->push_back(pu(actX, actY));
			actX = x; actY = y;
 		}
		else ++it;

	}
	tab.push_back(pu(actX, actY));
 	if (sch_path != NULL)
		sch_path->push_back(pu(actX, actY));
	return pu(actX, actY);
}

pc Schutzenberger::schutzenberger2D(lpc& tab, const bool randomize, dpc* sch_path, pc active)
{
	uchar x, y, actX, actY;
	uint64 rnd, rnd2;
	lpc::iterator it;
	//randomization
	if (randomize)
	{
		it = tab.begin();
		if (*it == pc(0,0)) ++it;
		if (((it->second == 0) && (next(it, 1)->second == 1)) || ((it->second == 1) && (next(it, 1)->second == 0)))
		{
			rnd = rand_0_to_n_1(2);
			rnd2 = (rnd+1) % 2;

			it->first = rnd; it->second = rnd2;
			next(it, 1)->first = rnd2; next(it, 1)->second = rnd;
		}
	}

	if (active == pc(0,0))
	{
		actX = tab.front().first;
		actY = tab.front().second;
	}
	else
	{
		actX = active.first;
		actY = active.second;
	}
	tab.pop_front();

	if (sch_path != NULL)
		sch_path->clear();

	it = tab.begin();
	while (it != tab.end())
	{
		x = it->first;
		y = it->second;
std::cout << "Обрабатываемая клетка: ";
printPC(make_pair(x, y));
std::cout << "\n";

std::cout << "Активная клетка: "; 
printPC(make_pair(actX, actY));
std::cout << "\n\n";

std::cout << "Поиск соседней клетки\n";

		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY)) || ((x == actX) && (y == actY + 1)))
		{
std::cout << "(1) Соседняя клетка: ";
printPC(make_pair(x, y));
std::cout << "\n";

std::cout << "Промежуточный вид таблицы:\n";
printLPC(tab);

std::cout << "(2) Удаляемая клетка: ";
printPC(make_pair(it->first, it->second));
std::cout << "\n";


			it = tab.erase(it);

std::cout << "Промежуточный вид таблицы:\n";
printLPC(tab);

std::cout << "(2) Сдвиг соседней ячейки на место активной клетки:";
printPC(make_pair(actX, actY));
std::cout << "\n\n";

			tab.insert(it, pc(actX, actY));
std::cout << "Промежуточный вид таблицы:\n";
printLPC(tab);
			if (sch_path != NULL)
				sch_path->push_back(pu(actX, actY));
			actX = x; actY = y;
std::cout << "(4) На месте удаленной клетки образовалась активная: ";
printPC(make_pair(x, y));
std::cout << "\n";

std::cout << "Промежуточный вид таблицы:\n";
printLPC(tab);
 		}
		else ++it;

	}
	tab.push_back(pc(actX, actY));
 	if (sch_path != NULL)
		sch_path->push_back(pc(actX, actY));
	return pc(actX, actY);
}

uint64 Schutzenberger::schutzenberger2D(const char* in_tab, const char* out_tab, const char* out_path, const uint64 NUM, Diagram* diagr, const bool ADD_LAST, const bool PRINT_NON_ACTIVE)
{
	const bool PRINT_PATH = true; //print the path to the file (to columns: line number & active/non-active

	Diagram diag;
	uint64 x;
	uint64 y;
	uint64 actX;
	uint64 actY;
	uint64 i;
	double tmp_d;

	ifstream f_in(in_tab);
	ofstream f_out;
	ofstream f_path(out_path);

	if (out_tab != NULL)
		f_out.open(out_tab);

	f_in >> actX;
	f_in >> actY;
	//actX = diag.get_width(actY);
	f_in >> tmp_d; f_in >> tmp_d;

	i = 0;
	//while(f_in >> y)
	while (f_in >> x)
	{
		if (++i == NUM)
			break;

		f_in >> y;
		f_in >> tmp_d; f_in >> tmp_d;
		//x = diag.get_width(y);
		//if (y == actY)
		//	++x;

		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY)) || ((x == actX) && (y == actY + 1)))
		{
			diag.add(actY);
			if (out_tab != NULL)
				//f_out << actY << " " << actX << endl;
				f_out << actY << endl;
			if (PRINT_PATH)
			{
				//f_path << actY << " 1\n";
				f_path << diag.get_size() << " " << actY << " " << actX;
				if (PRINT_NON_ACTIVE)
					f_path << " 1";
				f_path << endl;
			}
			actX = x; actY = y;
		}
		else
		{
			diag.add(y);
			if (out_tab != NULL)
				//f_out << y << " " << x << endl;
				f_out << y << endl;
			if (PRINT_PATH && PRINT_NON_ACTIVE)
				//f_path << y << " 0\n";
				f_path << diag.get_size() << " " << y << " " << x << " 0\n";
			//cout << y << " " << x << " " << 0 << endl;
		}
	}
	//cout << endl;

	if (diagr != NULL)
		diagr->init(&diag);

	if (ADD_LAST)
	{
		diag.add(actY);
		if (out_tab != NULL)
			//f_out << actY << " " << actX << endl;
			f_out << actY << endl;
		if (PRINT_PATH)
		{
			f_path << diag.get_size() << " " << actY << " " << actX;
			if (PRINT_NON_ACTIVE)
				f_path << " 1";
			f_path << endl;
		}
	}

	return actY;
}

pu Schutzenberger::schutzenberger2D(dpu in_tab, dpu* out_tab, dpu* sch_path, pu active)
{

	uint64 x, y, actX, actY;
	dpu::iterator it;

	if (active == pu(0,0))
	{
		actX = in_tab.at(0).first;
		actY = in_tab.at(0).second;
	}
	else
	{
		actX = active.first;
		actY = active.second;
	}
//cout << actX << " " << actY << endl;
	if (out_tab != NULL)
	{
		out_tab->clear();
		//if (active == pu(0,0))
		//	out_tab->push_back(pu(0,0));
	}

	if (sch_path != NULL)
	{	
		sch_path->clear();
		//if (active == pu(0,0))
		//	sch_path->push_back(pu(0,0));
	}

	for (it = in_tab.begin(), ++it; it != in_tab.end(); ++it)
	{
		x = it->first;
		y = it->second;

		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY)) || ((x == actX) && (y == actY + 1)))
		{
			if (out_tab != NULL)
				out_tab->push_back(pu(actX, actY));
				
			if (sch_path != NULL)
				sch_path->push_back(pu(actX, actY));
			actX = x; actY = y;
 		}
		else
		{
			if (out_tab != NULL)
				out_tab->push_back(pu(x, y));
		}		
	}
	if (out_tab != NULL)
		out_tab->push_back(pu(actX, actY));
 	if (sch_path != NULL)
		sch_path->push_back(pu(actX, actY));
	return pu(actX, actY);
}

pu Schutzenberger::schutzenberger2D_randbegin(dpu* in_tab, dpu* out_tab, dpu* sch_path, pu active)
{
	uint64 x;
	uint64 y;
	uint64 actX;
	uint64 actY;
	uint64 i;
	uint64 rnd;
	uint64 rnd2;
	dpu::iterator it;

	if (out_tab != NULL)
		out_tab->clear();

	if (sch_path != NULL)
		sch_path->clear();

	//randomization
	i = 0;
	if (in_tab->at(0) == pu(0,0))
		i = 1;
//cout << in_tab->at(i).second << " " << in_tab->at(i+1).second << endl;
	if (((in_tab->at(i).second == 0) && (in_tab->at(i+1).second == 1)) || ((in_tab->at(i).second == 1) && (in_tab->at(i+1).second == 0)))
	{
		rnd = rand_0_to_n_1(2);
		rnd2 = (rnd+1) % 2;
/*
cout << "AA\n";
cout << rnd << " " <<rnd2 << endl;
for (auto it = in_tab->begin(); it != in_tab->end(); ++it)
{
	cout << it->first << "," << it->second << " ";
}
cout << endl;
*/
		in_tab->at(i).first = rnd; in_tab->at(i).second = rnd2;
		in_tab->at(i+1).first = rnd2; in_tab->at(i+1).second = rnd;
/*
for (auto it = in_tab->begin(); it != in_tab->end(); ++it)
{
	cout << it->first << "," << it->second << " ";
}
cout << endl << endl;
*/
	}
/*
if ((in_tab->at(0) == pu(1,0)) && (in_tab->at(1) == pu(0,1)))
{
for (auto it = in_tab->begin(); it != in_tab->end(); ++it)
{
	cout << it->first << "," << it->second << " ";
}
cout << endl;
}
*/

	if (active == pu(0,0)) {
		actX = in_tab->at(0).first;
		actY = in_tab->at(0).second;
	}
	else {
		actX = active.first;
		actY = active.second;
	}

	for (it = in_tab->begin(), ++it; it != in_tab->end(); ++it) {
		x = it->first;
		y = it->second;
//cout << x << " " << y << endl;
//cout << "active: " << actX << " " << actY << endl;
		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY)) || ((x == actX) && (y == actY + 1))) {
			if (out_tab != NULL) out_tab->push_back(pu(actX, actY));
			if (sch_path != NULL) sch_path->push_back(pu(actX, actY));
			actX = x; actY = y;
 		}
		else {
			if (out_tab != NULL) out_tab->push_back(pu(x, y));
		}		
	}
	if (out_tab != NULL) out_tab->push_back(pu(actX, actY));
 	if (sch_path != NULL) sch_path->push_back(pu(actX, actY));
/*
if ((in_tab->at(0) == pu(1,0)) && (in_tab->at(1) == pu(0,1)))
{
for (auto it = out_tab->begin(); it != out_tab->end(); ++it)
{
	cout << it->first << "," << it->second << " ";
}
cout << endl << endl;
}
*/
	return pu(actX, actY);
}

pu Schutzenberger::schutzenberger2D_no_lastadd(dpu in_tab, dpu& out_tab, dpu* sch_path) {

	uint64 x;
	uint64 y;
	uint64 actX;
	uint64 actY;
	dpu::iterator it;

	if (sch_path != NULL) sch_path->clear();

	actX = in_tab.at(0).first;
	actY = in_tab.at(0).second;
	out_tab.clear();
	for (it = in_tab.begin(), ++it; it != in_tab.end(); ++it)
	{
		x = it->first;
		y = it->second;
		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY)) || ((x == actX) && (y == actY + 1))) {
			out_tab.push_back(pu(actX, actY));
			if (sch_path != NULL) sch_path->push_back(pu(actX, actY));
			actX = x; actY = y;
		}
		else out_tab.push_back(pu(x, y));
	}
	return pu(actX, actY);
}


pu Schutzenberger::schutzenberger2D_reverse(dpu in_tab, dpu &out_tab, mpp* sch_path)
/*
We process in_tab from last to first element.
*/
{
	uint64 x, y, actX, actY;
	//pu res;
	//bool res_calculated;
	reverse_iterator<dpu::iterator> it;

	//res_calculated = false;
	actX = in_tab.back().first;
	actY = in_tab.back().second;
	out_tab.clear();

	if (sch_path != NULL)
	{
		sch_path->clear();
		//(*sch_path)[pu(0, 0)] = pu(0, 3);
	}

//cout << "BEGIN\n";
//cout << "Actives: " << actX << "," << actY << endl;

	for (it = in_tab.rbegin(), ++it; it != in_tab.rend() ; ++it)
	{
		x = it->first;
		y = it->second;

//cout << "tab: " << x << "," << y << endl;

		if (actX != 0)
		{
			if ((y == actY) && (x == actX - 1))
			{
				out_tab.push_front(pu(actX, actY));
				if (sch_path != NULL)
				{
					(*sch_path)[pu(actX, actY)] = pu(0, 3);
				}
				actX = x; actY = y;

				//if (!res_calculated)
				//{
					//res = pu(actX, actY);
				//	res_calculated = true;
				//}
				continue;
			}
		}

		if (actY != 0)
		{
			if ((y == actY - 1) && (x == actX))
			{
				out_tab.push_front(pu(actX, actY));
				if (sch_path != NULL)
					(*sch_path)[pu(actX, actY)] = pu(0, 3);
				actX = x; actY = y;

				//if (!res_calculated)
				//{
					//res = pu(actX, actY);
				//	res_calculated = true;
				//}
				continue;
			}
		}
		//if (!res_calculated)
		//{
			//res = pu(x, y);
		//	res_calculated = true;
		//}
		out_tab.push_front(pu(x, y));
		////////////////////if (sch_path != NULL)
		////////////////////	(*sch_path)[pu(x, y)] = pu(0, 2);
	}

	out_tab.push_front(pu(actX, actY));

	if (sch_path != NULL)
		(*sch_path)[pu(actX, actY)] = pu(0, 3);

	return out_tab.back();
	//return res;
}

void Schutzenberger::schutzenberger2D_reverse(dpu in_tab, dpu &out_tab, dpu& sch_path)
/*
We process in_tab from last to first element.

*/
{
	uint64 x, y, actX, actY;
	reverse_iterator<dpu::iterator> it;

	actX = in_tab.back().first;
	actY = in_tab.back().second;
	out_tab.clear();
	sch_path.clear();

	for (it = in_tab.rbegin(), ++it; it != in_tab.rend() ; ++it)
	{
		x = it->first;
		y = it->second;

		if (actX != 0)
		{
			if ((y == actY) && (x == actX - 1))
			{
				out_tab.push_front(pu(actX, actY));
				sch_path.push_front(pu(actX, actY));
				actX = x; actY = y;
				continue;
			}
		}

		if (actY != 0)
		{
			if ((y == actY - 1) && (x == actX))
			{
				out_tab.push_front(pu(actX, actY));
				sch_path.push_front(pu(actX, actY));
				actX = x; actY = y;
				continue;
			}
		}
		out_tab.push_front(pu(x, y));
	}

	out_tab.push_front(pu(actX, actY));

	sch_path.push_front(pu(actX, actY));
}




pu Schutzenberger::schutzenberger2D_reverse(dpu in_tab, dpu &out_tab)
{
	return schutzenberger2D_reverse(in_tab, out_tab, NULL);
}

void Schutzenberger::fullpath_schutzenberger2D(const char* IN_TAB, const char* OUT_TAB)
{
	const uint64 PATH_LEN = 200;
	const char* FNAME_TMP = "schutzenberger_tmp.txt";

	uint64 line, i;
	di path;

	line = schutzenberger2D(IN_TAB, FNAME_TMP, "schutzenberger2D_path.txt", PATH_LEN);
	path.push_front(line);

	for (i = PATH_LEN - 1; i != 0; --i)
	{
		line = schutzenberger2D(FNAME_TMP, OUT_TAB, "schutzenberger2D_path.txt", i);

		path.push_front(line);

		if (system((string("cp ") + OUT_TAB + " " + FNAME_TMP).c_str()));
	}

	ofstream f(OUT_TAB);
	for (i = 0; i < path.size(); ++i)
		f << path.at(i) << endl;
}

void Schutzenberger::lex_schutzenberger2D()
{
	//lexicographical addings!
	//if two points have same vals, take the one where x is smaller
	Diagram diag;
	mi *p = diag.get_points();
	uint64 val, min, minx, miny, x, y;
	ofstream f_lex("test_lex.txt");

	minx = 0;

	for (int i = 0; i < 200; ++i)
	{
		min = LLONG_MAX;
		for (auto it = p->begin(); it != p->end(); ++it)
		{
			y = it->first;
			x = diag.get_width(y);
			val = 2 * x + y;
			if ((val < min) || ((val == min) && (x < minx)))
			{
				min = val;
				minx = x;
				miny = y;
			}
		}
		diag.add(miny);
		f_lex << miny << endl;
	}
	fullpath_schutzenberger2D("test_lex.txt", "test_lex2.txt");
}

void Schutzenberger::schutzenberger2D_set_active(const char* IN_TAB, const char* OUT_TAB, const uint64 INIT, const uint64 LEN, const uint64 CORNER)
/*
 * An experiment: we do INIT steps in tab IN_TAB.
 * After that, we choose a point number SHIFT to be active.
 * Then we continue constructing the schutzenberger path until LEN.
 */
{
	//Diagram diag;
	uint64 x, y, actX, actY, i;
	ifstream f_in;
	map<uint64,uint64> widths;
	set<uint64> corners;

	f_in.open(IN_TAB);

	i = 0;
	widths[0] = 1;
	while (++i != INIT)
	{
		f_in >> y;
		widths[y]++;
	}
	//ofstream f_out(OUT_TAB,  ios::out | ios::app);
	ofstream f_out(OUT_TAB);

	//Choose an active corner.
	widths[CORNER]--;

	actY = CORNER;
	actX = widths[CORNER];

	//cout << actY << " " << actX << endl;
	while(f_in >> y)
	{
		if (i++ == INIT + LEN)
			break;

		x = widths[y];

		//if (y == actY+1)
		//	cout << actY << " " << y << " " << actX << " " << x << endl;

		if (y == actY)
			++x;

		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY)) || ((x == actX) && (y == actY + 1)))
		{
			//cout << ((x == actX + 1) && (y == actY)) << " " << ((x == actX) && (y == actY + 1)) << endl;
			//cout << "add " << actY << endl;
			widths[actY]++;
			//f_out << actY << endl;
			//f_out << fixed << setprecision(20) << (actY+1)*1.0 / (actX+1) << endl;

			f_out << actX << " " << actY << " 1\n";
			//f_out << actX << " " << actY << endl;

			actX = x; actY = y;
		}
		else
		{
			//cout << "Add " << y << endl;
			//cout << "a\n";
			widths[y]++;
			//cout << "b\n";
			f_out << x << " " << y << " 0\n";
		}
	}
	//cout << endl;
	//////
	f_out << actX << " " << actY << " 1\n";
	//////
	f_in.close();

}

tc Schutzenberger::schutzenberger3D(ltc& tab, const bool randomize, dtc* sch_path, const bool addlast)
{
	const vector<ltc> begintabs2 = {{{0,0,0}, {1,0,0}, {0,1,0}}, {{0,0,0}, {0,1,0}, {1,0,0}}, {{0,0,0}, {0,1,0}, {0,0,1}}, {{0,0,0}, {0,0,1}, {0,1,0}}, {{0,0,0}, {1,0,0}, {0,0,1}}, {{0,0,0}, {0,0,1}, {1,0,0}}};

	uchar x, y, z, actX, actY, actZ;
	ltc::iterator it;
	uint64 i;

	//check the beginning part of in_tab:
	ltc begintab(tab.begin(), next(tab.begin(), 3));
	if (tab.size() < 3)
		begintab = tab;

	for (i = 0; i < begintabs2.size(); ++i)
	{
		if (begintabs2.at(i) == begintab)
		{
			if (rand_0_to_n_1(2))
				swap(*tab.begin(), *next(tab.begin())); 
			break;
		}
	}
	
	actX = tab.front().x;
	actY = tab.front().y;
	actZ = tab.front().z;

	tab.pop_front();

	if (sch_path != NULL)
		sch_path->clear();

	it = tab.begin();
	while (it != tab.end())
	{
		x = it->x; y = it->y; z = it->z;
		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY) && (z == actZ)) || ((x == actX) && (y == actY + 1) && (z == actZ)) || ((x == actX) && (y == actY) && (z == actZ + 1)))
		{
			it = tab.erase(it);
			tab.insert(it, tc(actX, actY, actZ));
			if (sch_path != NULL)
				sch_path->push_back(tc(actX, actY, actZ));
			actX = x; actY = y; actZ = z;
 		}
		else ++it;

	}
	if (addlast)
		tab.push_back(tc(actX, actY, actZ));

 	if (sch_path != NULL)
		sch_path->push_back(tc(actX, actY, actZ));
	return tc(actX, actY, actZ);
}

tu Schutzenberger::schutzenberger3D(const char* IN_TAB, const char* OUT_TAB, const uint64 NUM)
{
	Diagram3D diag;
	uint64 x, y, z, actX, actY, actZ, i;
	Sequence3D seq_in("r", "tab", IN_TAB);
	ofstream f_out;

	if (OUT_TAB != NULL)
		f_out.open(OUT_TAB);

	seq_in.read_5col_tab(&diag, false);
	actZ = seq_in.get_z();
	actY = seq_in.get_y();
	actX = diag.getpointX(actZ, actY);

	//cout << "active: " << actZ << " " << actY << " " << actX << endl;

	i = 0;
	while(seq_in.read_5col_tab(&diag, false))
	{

		if (++i == NUM)
			break;

		//if (i % 1000 == 0)
		//	cout << i << endl;
		//cout << "iteration " << i << endl;

		z = seq_in.get_z();
		y = seq_in.get_y();
		//cout << "A\n";
		//cout << z << " " << y << endl;
		//diag.print_cols();
		x = diag.getpointX(z, y);
		//cout << "B\n";
		if ((z == actZ) && (y == actY))
			++x;

		//cout << z << " " << y << " " << x << endl;

		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY) && (z == actZ)) || ((x == actX) && (y == actY + 1) && (z == actZ)) || ((x == actX) && (y == actY) && (z == actZ + 1)))
		{
			//cout << "NEAR ACTIVE\n";
			if (OUT_TAB != NULL)
				f_out << actX << " " << actY << " " << actZ << " 0 0" << endl;

			//cout << "add active: " << actZ << " " << actY << endl;
			diag.add(pu(actZ, actY));
			actX = x; actY = y; actZ = z;
			//cout << "new active: " << actZ << " " << actY << " " << actX << endl;
 		}
		else
		{
			//cout << "not near active...\n";
			//cout << "add " << z << " " << y << endl;
			diag.add(pu(z, y));
			if (OUT_TAB != NULL)
			{
				//cout << z << " " << y << endl;
				f_out << x << " " << y << " " << z << " 0 0" << endl;
			}
		}
	}
	//cout << "RESULT: " << actZ << " " << actY << endl << endl;

 	return tu(actX, actY, actZ);
}

tu Schutzenberger::schutzenberger3D(dtu in_tab, dtu &out_tab)
{
	uint64 x, y, z, actX, actY, actZ;
	dtu::iterator it;

	actX = in_tab.at(0).x;
	actY = in_tab.at(0).y;
	actZ = in_tab.at(0).z;
	
	out_tab.clear();

	for (it = in_tab.begin(), ++it; it != in_tab.end(); ++it)
	{
		x = it->x; y = it->y; z = it->z;
		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY) && (z == actZ)) || ((x == actX) && (y == actY + 1) && (z == actZ)) || ((x == actX) && (y == actY) && (z == actZ + 1)))
		{
			out_tab.push_back(tu(actX, actY, actZ));
			actX = x; actY = y; actZ = z;
 		}
		else
		{
			out_tab.push_back(tu(x, y, z));
		}		
	}
	out_tab.push_back(tu(actX, actY, actZ));
 	
	return tu(actX, actY, actZ);
}

tu Schutzenberger::schutzenberger3D(dtu in_tab, dtu &out_tab, dtu &sch_path)
{
	uint64 x, y, z, actX, actY, actZ;
	dtu::iterator it;
	tu coord;

	actX = in_tab.at(0).x;
	actY = in_tab.at(0).y;
	actZ = in_tab.at(0).z;

	out_tab.clear();
	sch_path.clear();

	for (it = in_tab.begin(), ++it; it != in_tab.end(); ++it)
	{
		x = it->x; y = it->y; z = it->z;

//cout << "next from intab: " << x << " " << y << " " << z << endl;

		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY) && (z == actZ)) || ((x == actX) && (y == actY + 1) && (z == actZ)) || ((x == actX) && (y == actY) && (z == actZ + 1)))
		{
//cout << "Going to add a box neighbour to active\n";
//cout << "active: " << actX << " " << actY << " " << actZ << endl;
//cout << "this: " << x << " " << y << " " << z << endl;
			coord = tu(actX, actY, actZ);
			out_tab.push_back(coord);
			sch_path.push_back(coord);
			actX = x; actY = y; actZ = z;
 		}
		else
		{
			out_tab.push_back(tu(x, y, z));
		}		
	}
	out_tab.push_back(tu(actX, actY, actZ));
	sch_path.push_back(tu(actX, actY, actZ));

	return tu(actX, actY, actZ);
}


void Schutzenberger::schutzenberger3D_reverse(dtu in_tab, dtu &out_tab, dtu& sch_path)
/*
We process in_tab from last to first element.
*/
{

	uint64 x, y, z, actX, actY, actZ;
	reverse_iterator<dtu::iterator> it;
	tu coord;
	bool is_neighbour;

	actX = in_tab.back().x;
	actY = in_tab.back().y;
	actZ = in_tab.back().z;
	out_tab.clear();
	sch_path.clear();

	for (it = in_tab.rbegin(), ++it; it != in_tab.rend() ; ++it)
	{
		x = it->x; y = it->y; z = it->z;

		is_neighbour = false;

		if (actX != 0)
			if ((z == actZ) && (y == actY) && (x == actX - 1))
				is_neighbour = true;
		
		if (actY != 0)
			if ((z == actZ) && (y == actY - 1) && (x == actX))
				is_neighbour = true;

		if (actZ != 0)
			if ((z == actZ - 1) && (y == actY) && (x == actX))
				is_neighbour = true;

		if (is_neighbour)
		{
			coord = tu(actX, actY, actZ);
			out_tab.push_front(coord);
			sch_path.push_front(coord);
			actX = x; actY = y; actZ = z;
			continue;
		}

		out_tab.push_front(tu(x, y, z));
	}

	coord = tu(actX, actY, actZ);
	out_tab.push_front(coord);
	sch_path.push_front(coord);
}



tu Schutzenberger::schutzenberger3D_randbegin(dtu in_tab, dtu &out_tab, const int SIZE)
{
	const vector<dtu> begintabs2 = {{{0,0,0}, {1,0,0}, {0,1,0}}, {{0,0,0}, {0,1,0}, {1,0,0}}, {{0,0,0}, {0,1,0}, {0,0,1}}, {{0,0,0}, {0,0,1}, {0,1,0}}, {{0,0,0}, {1,0,0}, {0,0,1}}, {{0,0,0}, {0,0,1}, {1,0,0}}};

	const vector<dtu> begintabs3 = {{{0,0,0}, {0,0,1}, {0,1,0}, {1,0,0}}, {{0,0,0}, {0,0,1}, {1,0,0}, {0,1,0}}, {{0,0,0}, {0,1,0}, {0,0,1}, {1,0,0}}, {{0,0,0}, {0,1,0}, {1,0,0}, {0,0,1}}, {{0,0,0}, {1,0,0}, {0,0,1}, {0,1,0}}, {{0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}}};

	uint64 x, y, z, actX, actY, actZ, i, randnum;
	tu swap;
	dtu::iterator it;

	if ((SIZE != 2) && (SIZE != 3))
	{
		cout <<	"schutzenberger3D_randbegin(): size must be equal to 2 or 3!\n";
		exit(1);
	}

	//check the beginning part of in_tab:
	dtu begintab(in_tab.begin(), in_tab.begin() + SIZE + 1);
	if (in_tab.size() < 3)
		begintab = in_tab;

	if (SIZE == 3)
		for (i = 0; i < begintabs3.size(); ++i)
		{
			if (begintabs3.at(i) == begintab)
			{
				//begintabs.at(i).at(0) = begintab.at(0);
				//begintabs.at(i).at(1) = begintab.at(1);
				//begintabs.at(i).at(2) = begintab.at(2);

	//for (it = in_tab.begin(); it != in_tab.begin()+10; ++it)
	//	cout << it->x << "," << it->y << "," << it->z << " ";
	//cout << endl;
				randnum = rand_0_to_n_1(begintabs3.size());
				copy(begintabs3.at(randnum).begin(), begintabs3.at(randnum).end(), in_tab.begin());
	//for (it = in_tab.begin(); it != in_tab.begin()+10; ++it)
	//	cout << it->x << "," << it->y << "," << it->z << " ";
	//cout << endl<< endl;
				break;
			}
		}
	else
	{
		for (i = 0; i < begintabs2.size(); ++i)
		{
/*
cout << "in_tab:\n";
for (int i = 0; i < in_tab.size(); ++i)
cout << in_tab.at(i).x << "," << in_tab.at(i).y << "," << in_tab.at(i).z << " ";
cout << endl; 

cout << "begintab:\n";
for (int i = 0; i < begintab.size(); ++i)
cout << begintab.at(i).x << "," << begintab.at(i).y << "," << begintab.at(i).z << " ";
cout << endl; 
cout << "begintabs2ati:\n";
for (int x = 0; x < begintabs2.at(i).size(); ++x)
cout << begintabs2.at(i).at(x).x << "," << begintabs2.at(i).at(x).y << "," << begintabs2.at(i).at(x).z << " ";
cout << endl; 
*/

			if (begintabs2.at(i) == begintab)
			{
//cout << "OK\n";
				if (rand_0_to_n_1(2))
				{
//cout << begintab.size() << " " << begintabs2.at(i).size() << endl;
					swap = in_tab.at(1);
					in_tab.at(1) = in_tab.at(2);
					in_tab.at(2) = swap; 
				}
				break;
			}
//else cout << "NO\n";
		}
	}


/*
for (it = begintab.begin(); it != begintab.end(); ++it)
	cout << it->x << "," << it->y << "," << it->z << " ";
cout << endl;
exit(1);
*/


	actX = in_tab.at(0).x;
	actY = in_tab.at(0).y;
	actZ = in_tab.at(0).z;
	
	out_tab.clear();

	for (it = in_tab.begin(), ++it; it != in_tab.end(); ++it)
	{
		x = it->x; y = it->y; z = it->z;
		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY) && (z == actZ)) || ((x == actX) && (y == actY + 1) && (z == actZ)) || ((x == actX) && (y == actY) && (z == actZ + 1)))
		{
			out_tab.push_back(tu(actX, actY, actZ));
			actX = x; actY = y; actZ = z;
 		}
		else
		{
			out_tab.push_back(tu(x, y, z));
		}		
	}
	out_tab.push_back(tu(actX, actY, actZ));
 	
	return tu(actX, actY, actZ);
}



tu Schutzenberger::schutzenberger3D(dtu in_tab, dtu &sch_path, tu active)
{
	uint64 x, y, z, actX, actY, actZ;
	dtu::iterator it;

	actX = active.x; actY = active.y; actZ = active.z;
	
	sch_path.clear();

	for (it = in_tab.begin(), ++it; it != in_tab.end(); ++it)
	{
		x = it->x; y = it->y; z = it->z;
		//if we are going to add a neighbour box of the active box:
		if (((x == actX + 1) && (y == actY) && (z == actZ)) || ((x == actX) && (y == actY + 1) && (z == actZ)) || ((x == actX) && (y == actY) && (z == actZ + 1)))
		{
			sch_path.push_back(tu(actX, actY, actZ));
			actX = x; actY = y; actZ = z;
 		}	
	}
	sch_path.push_back(tu(actX, actY, actZ));
 	
	return tu(actX, actY, actZ);
}

multiset<int> Schutzenberger::schutzenberger2D_cyclelengths(di cols, bool print_dim)
/* 2D:
 * Выбираем какую-нибудь диаграмму. Вычисляем все таблицы.
 * Берем первую попавшуюся таблицу. От нее строим много Шюценберже (с финальным добавлением клетки, чтобы попадать в ту же диаграмму).
 * Встретившиеся таблицы удаляем из списка. Когда попадаем в исходную таблицу, то берем следующую таблицу из списка
 * (таблицу, которая еще не была обработана)
 * Повторяем процесс, пока список таблиц не будет исчерпан.
 */
{
	Diagram diag;
	set<dpu> tabs;
	multiset<int> cycles;
	dpu tab, tab2, tab_init;
	uint64 i;

	diag.init_by_cols(cols);
	diag.calc_all_tabs(&tabs);

	if (print_dim)
	{
		mpz_t dim;
		mpz_init(dim);
		diag.calc_exact_dim(dim);
		gmp_printf("dimension: %Zd\n", dim);
		mpz_clear(dim);
	}

	while (tabs.size() != 0)
	{
		tab = *(tabs.begin());
		tabs.erase(tab);
		tab_init = tab;
		i = 0;
		do
		{
			++i;
			Schutzenberger::schutzenberger2D(tab, &tab2);
			tabs.erase(tab2);
			tab = tab2;
		}
		while(tab2 != tab_init);
		cycles.insert(i);
	}

	return cycles;
}

multimap<uint64, int> Schutzenberger::schutzenberger3D_cyclelengths(vvi cols)
{
	set<dtu> tabs;
	dtu init_tab, in_tab, out_tab;
	multimap<uint64, int> res;
	uint64 len;
	Diagram3D diag;

	diag.init_by_cols(cols);

	diag.calc_all_tabs(&tabs);
	while (!tabs.empty())
	{
		init_tab = *tabs.begin();
		in_tab = init_tab;
		len = 0;
		while (out_tab != init_tab)
		{
			tabs.erase(in_tab);
			schutzenberger3D(in_tab, out_tab);
			in_tab = out_tab;
			++len;
		}
		res.insert(pair<uint64,int>(len,0));

		in_tab = out_tab;
	}

	return res;
}

void Schutzenberger::fullpath_schutzenberger3D(const char* IN_TAB, const char* OUT_TAB, const uint64 PATH_LEN)
{
	const char* FNAME_TMP = "schutzenberger_tmp.txt";

	uint64 i;
	pu line;
	tu coord(0, 0, 0);
	deque<pu> path;

	coord = schutzenberger3D(IN_TAB, FNAME_TMP, PATH_LEN);
	line = pu(coord.z, coord.y);
	path.push_front(line);

	//cout << line.first << " " << line.second << endl;

	for (i = PATH_LEN - 1; i != 0; --i)
	{
		if (i%100 == 0)
			cout << i << endl;

		coord = schutzenberger3D(FNAME_TMP, OUT_TAB, i);
		line = pu(coord.z, coord.y);
		//cout << line.first << " " << line.second << endl;

		path.push_front(line);

		if (system((string("cp ") + OUT_TAB + " " + FNAME_TMP).c_str()));
	}

	ofstream f(OUT_TAB);
	for (i = 0; i < path.size(); ++i)
		f << "0 " << path.at(i).second << " " << path.at(i).first << " 0 0" << endl;

}

void Schutzenberger::print_hops_to_img2D(map<pair<pu,pu>, int> hops, const string FNAME_IMG, const double LW, const double THICK_FACTOR)
//Print Schutzenberger paths. The line width depends on how many lines passes through a hop.
{
	//THICK_FACTOR: thickening factor of line (depending on how many Schutz.paths passes through this hop) - MUST BE >1??????!!!!!!!!!!!!

	double MAX_LW = 1;	
	const int SX = 2000;
	const int SY = 2000;

	map<int, vector<pair<pu,pu> > > hops2;
	ofstream f_out;
	double lw, min, max;
	string ext;
	
	ext = FNAME_IMG.substr(FNAME_IMG.length()-4,4);
	if ((ext != ".png") && (ext != ".eps"))
		return;
	
	if (ext == ".eps")
		MAX_LW = 10;
		
	for (map<pair<pu,pu>, int>::iterator it = hops.begin(); it != hops.end(); ++it)
	{
		hops2[it->second].push_back(it->first);
	}
//cout << hops.size() << " " << hops2.size() << endl;
	for (map<int, vector<pair<pu,pu> > >::iterator it = hops2.begin(); it != hops2.end(); ++it)
	{

		f_out.open((string("test_lines_") + to_string(it->first) + ".txt").c_str());

		for (vector<pair<pu,pu> >::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			f_out << it2->first.first << " " << it2->first.second << endl;
			f_out << it2->second.first << " " << it2->second.second << endl << endl;
		}

		f_out.close();
	}

	f_out.open("tmp_gnuplot.plt");

	f_out << "set term pngcairo notransparent \nset nokey\n\n";

	if (ext == ".png")
		f_out << "set term pngcairo size " << SX << "," << SY << " notransparent \nset nokey\n\n";
	else
		f_out << "set term postscript eps enhanced\nset nokey\n\n";	

	f_out << "set output '" << FNAME_IMG << "'\n";
	//f_out << "set xrange [-0.05:" << size << "]\nset yrange [-0.05:" << size << "]\n";
	f_out << "set size ratio -1\n";
	f_out << "unset key; unset tics; unset border;\n";

	f_out << "pl ";

	map<int, vector<pair<pu,pu> > >::iterator it, it2;
	for (it = hops2.begin(), it2 = --hops2.end(); it != it2; ++it)
	{
		if (THICK_FACTOR == 0)
			lw = LW;
		else
			lw = it->first * THICK_FACTOR / hops2.size();
		if (lw > MAX_LW)
			lw = MAX_LW;
		f_out << "'test_lines_" << it->first << ".txt' w l lw " << lw << " lc -1 lt 1 not, ";
	}
	if (THICK_FACTOR == 0)
		lw = LW;
	else
		lw = it->first * THICK_FACTOR / hops2.size();
	if (lw > MAX_LW)
		lw = MAX_LW;
	f_out << "'test_lines_" << it->first << ".txt' w l lw " << lw << " lc -1 lt 1 not\n";

	f_out.close();
	if (system("gnuplot tmp_gnuplot.plt"));

	if (ext == ".png")
	{
		if (system((string("convert ") + FNAME_IMG + string(" -trim ") + FNAME_IMG).c_str()));
	}
	else
	{
                if (system((string("./fixbb ") + FNAME_IMG).c_str() ));
	}
}

void Schutzenberger::print_hops_to_png3D(map<pair<tu,tu>, int> hops, const char* FNAME_PNG)
//Print Schutzenberger paths. The line width depends on how many lines passes through a hop.
{
	const double THICK_FACTOR = 1.5; //thickening factor of line (depending on how many Schutz.paths passes through this hop)
	const double MAX_LW = 1;

	map<int, vector<pair<tu,tu> > > hops2;
	ofstream f_out;
	double lw;

	if (hops.size() < 1)
		return;

	for (map<pair<tu,tu>, int>::iterator it = hops.begin(); it != hops.end(); ++it)
		hops2[it->second].push_back(it->first);

	for (map<int, vector<pair<tu,tu> > >::iterator it = hops2.begin(); it != hops2.end(); ++it)
	{
		f_out.open((string("test_lines_") + to_string(it->first) + ".txt").c_str());

		for (vector<pair<tu,tu> >::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			f_out << it2->first.x << " " << it2->first.y << " " << it2->first.z << endl;
			f_out << it2->second.x << " " << it2->second.y << " " << it2->second.z << endl << endl << endl;
		}

		f_out.close();
	}

	f_out.open("tmp_gnuplot.plt");
	f_out << "set term pngcairo notransparent \nset nokey\n\n";
	f_out << "set view equal xyz\nset view 54.73,135,1,1\nset origin 0,-0.15\n";
	f_out << "set output '" << FNAME_PNG << "'\n";
	//f_out << "set xrange [-0.05:" << size << "]\nset yrange [-0.05:" << size << "]\n";
	f_out << "set size ratio -1\n";
	f_out << "unset key; unset tics; unset border;\n";

	f_out << "spl ";

	map<int, vector<pair<tu,tu> > >::iterator it, it2;
	for (it = hops2.begin(), it2 = --hops2.end(); it != it2; ++it)
	{
		if (THICK_FACTOR == 0)
			lw = 1;
		else
			lw = it->first * THICK_FACTOR / hops2.size();
		if (lw > MAX_LW)
			lw = MAX_LW;
		f_out << "'test_lines_" << it->first << ".txt' w l lw " << lw << " lc -1 not, ";
	}
	if (THICK_FACTOR == 0)
		lw = 1;
	else
		lw = it->first * THICK_FACTOR / hops2.size();
	if (lw > MAX_LW)
		lw = MAX_LW;
	f_out << "'test_lines_" << it->first << ".txt' w l lw " << lw << " lc -1 not\n";

	f_out.close();
	if (system("gnuplot tmp_gnuplot.plt"));
	if (system((string("convert ") + FNAME_PNG + string(" -trim ") + FNAME_PNG).c_str()));
}

/*
bool is_triangle(vvi& rows)
{
	uint64 H, y, z;

	H = rows.at(0).at(0);
	
//cout << "H: " << H << endl;
	for (y = 1; y < rows.at(0).size(); ++y)
	{
//cout << "y: " << y << endl;
//cout << rows.at(0).at(y) + y << endl;
		if (rows.at(0).at(y) + y != H)
			return false;
	}

	if (rows.at(0).at(y-1) != 1)
		return false;

	for (z = 1; z < rows.size(); ++z)
	{
		if (rows.at(z).at(0) + 1 != H)
			return false;
		H = rows.at(z).at(0);
		for (y = 0; y < rows.at(z).size(); ++y)
			if (rows.at(z).at(y) + y != H)
				return false;
		if (rows.at(z).at(y-1) != 1)
			return false;
	}
	if (H != 1)
		return false;

	return true;
}
*/

void Schutzenberger::schutzenberger3D_seek_central_process()
/*
We generate a big 3D diagram (random planch from a file). We start to generate schutzenberger paths from it.
//We remember how frequently we generate each of the diagram in a new tab.
//We obtain the experimental probabilities equal to frequencies of diagrams.
Alternatively, we can calculate experimental transition probabilities
Then, we can make an assumption of real probabilities of exact central process.
*/
{
	const bool SAVE_DIAGS_FREQS = false;
	const bool SAVE_HOPS_FREQS = true;

	const uint64 BIG_DIAG_SIZE = 1000; //Size of initial big diagram
	const uint64 MAX_LEVEL = 10;          //Max.size of diagram for which we calculate the level
	const uint64 PRINT_STEP = 1000;
	const uint64 ITER_NUM = PRINT_STEP+1;          //Number of iterations (to calculate freqs)
	//const vi diags_count = {1, 1, 3, 6, 13, 24, 48, 86, 160, 282, 500, 859, 1479, 2485, 4167, 6879};
	Diagram3D diag, diag2;
	dtu in_tab, out_tab;
	uint64 x, y, z, i, j, k, sum, iter;
	double prob, lg;
	vi vec;
	vvi rows;
	map<vvi, pu> diag_freqs, tmp_diag_freqs;
	map<vvi, map<pu, uint64> > hop_freqs;


	mpu* points;
	//pu tmpcoord;
	ofstream f_out;
	ifstream f("results/standard3D_planch_tab_a_1.000000_i_0.txt");

	vec.push_back(1);

	points = diag.get_points();




//Big diagram - triangle:
/*
	uint64 N = 4;
	for (x = 0; x < N - 1; ++x)
	{
		diag.add(pu(0, 0));
		in_tab.push_back(pu(0, 0)); 
	}
	for (y = 1; y < N; ++y)
		for (x = 0; x < N-y; ++x)
		{
			diag.add(pu(0, y));
			in_tab.push_back(pu(0, y)); 
		}
	for (z = 1; z < N; ++z)
		for (y = 0; y < N-z; ++y)
			for (x = 0; x < N-z-y; ++x)
			{
				diag.add(pu(z, y));
				in_tab.push_back(pu(z, y)); 
			}
	in_tab.push_back(pu(2, 2));
	diag.add(pu(2,2));
*/

/*
	diag.add(pu(0,0));
	diag.add(pu(0,0));
	diag.add(pu(0,0));
	diag.add(pu(0,1));	
	diag.add(pu(1,0));
	in_tab.push_back(pu(0,0)); 
	in_tab.push_back(pu(0,0)); 
	in_tab.push_back(pu(0,0)); 
	in_tab.push_back(pu(0,1)); 
	in_tab.push_back(pu(1,0)); 
*/

	//diag.print_to_img("test.png");



//Big diagram - random plancherel:
	i = 0;
	while ((f >> x) && (++i < BIG_DIAG_SIZE))
	{
		f >> y; f >> z; f >> prob; f >> lg;
		in_tab.push_back(tu(x, y, z)); 
	}


	for (iter = 0; iter < ITER_NUM; ++iter)
	{

//for (auto it = in_tab.begin(); it != in_tab.end(); ++it)
//	cout << it->first << "," << it->second << " ";
//cout << endl;


//tmpcoord = in_tab[8];
//in_tab[8] = in_tab[9];
//in_tab[9] = tmpcoord;


/*
for (auto it = in_tab.begin(); it != in_tab.end(); ++it)
	cout << it->first << "," << it->second << " ";
cout << endl;
exit(1);
*/
		schutzenberger3D(in_tab, out_tab);
		in_tab = out_tab;


		rows.clear();
		rows.push_back(vec);
		for (j = 0; (j < out_tab.size()) && (j + 1 < MAX_LEVEL); ++j)
		{
			z = out_tab.at(j).z; y = out_tab.at(j).y;

			//if (is_triangle(rows))
				if (diag_freqs.find(rows) == diag_freqs.end())
				{
					diag_freqs[rows].first = 0;
					for (i = 0; i < rows.size(); ++i)
						for (k = 0; k < rows.at(i).size(); ++k) 
							diag_freqs[rows].first += rows.at(i).at(k);
					diag_freqs[rows].second = 1;
	/*
					cout << "A";
					for (i = 0; i < rows.size(); ++i)
					{
						for (k = 0; k < rows.at(i).size(); ++k)
						{
							cout << rows.at(i).at(k) << " ";
						}
						cout << endl;
					}
					cout << diag_freqs[rows].first;
					cout << "B\n";
	*/
				}
				else
					diag_freqs[rows].second++;
			hop_freqs[rows][pu(z, y)]++;

			if (z == rows.size())
				rows.push_back(vec);
			else
			{
				if (y == rows.at(z).size())
					rows.at(z).push_back(1);
				else
					rows.at(z).at(y)++;
			}
		}


		if (SAVE_DIAGS_FREQS)
		{	
			if ((iter + 1) % PRINT_STEP == 0)
			{
				f_out.open(("schutz_triangles_" + to_string(iter+1) + ".txt").c_str());


tmp_diag_freqs = diag_freqs;
uint64 max;
vvi maxvvi = {{0}};
while (tmp_diag_freqs.size() != 0)
{
	max = 0;
	for (map<vvi, pu>::iterator it = tmp_diag_freqs.begin(); it != tmp_diag_freqs.end(); ++it)
	{
		if (it->second.second >= max)
		{
			maxvvi = it->first;
			max = it->second.second;
		}
	}



//print diag:
f_out << "\"";
for (i = 0; i < maxvvi.size(); ++i)
{
	for (j = 0; j < maxvvi.at(i).size(); ++j)
	{
		f_out << maxvvi.at(i).at(j);
		if (j + 1 != maxvvi.at(i).size())
			f_out << " ";
	}
	if (i + 1 != maxvvi.size())
		f_out << ", ";
}
f_out << "\" ";

//print statistics:
f_out << max * 1.0 / (iter+1) << endl;




	tmp_diag_freqs.erase(maxvvi);
}

/*
				for (map<vvi, pu>::iterator it = diag_freqs.begin(); it != diag_freqs.end(); ++it)
				{

					//print diag:
					f_out << "\"";
					for (i = 0; i < it->first.size(); ++i)
					{
						for (j = 0; j < it->first.at(i).size(); ++j)
						{
							f_out << it->first.at(i).at(j);
							if (j + 1 != it->first.at(i).size())
								f_out << " ";
						}
						if (i + 1 != it->first.size())
							f_out << ", ";
					}
					f_out << "\" ";

					//print statistics:
					f_out << it->second.second * 1.0 / (iter+1) << endl;
				}
*/
				f_out.close();
			}
		}

		if (SAVE_HOPS_FREQS)
		{
			//print ONLY frequecies:
			cout << fixed << setprecision(6);
			if ((iter >= 1000) && (iter % PRINT_STEP == 0))
			{
				double prod1, prod2;

				rows = {{1},{1}};
				diag.init_by_rows(rows);
				sum = 0;
				for (auto it2 = points->begin(); it2 != points->end(); ++it2)
					sum += hop_freqs[rows][it2->first];

				cout << iter << " " << hop_freqs[{{1},{1}}][pu(2,0)] * 1.0/sum << " " << hop_freqs[{{1},{1}}][pu(0,1)] * 1.0/sum;
				prod1 = (hop_freqs[{{1},{1}}][pu(2,0)] * 1.0/sum) * (hop_freqs[{{1},{1}}][pu(0,1)] * 1.0/sum);
			
				rows = {{1},{1},{1}};
				diag.init_by_rows(rows);
				sum = 0;
				for (auto it2 = points->begin(); it2 != points->end(); ++it2)
					sum += hop_freqs[rows][it2->first];

				cout << " " << hop_freqs[{{1},{1},{1}}][pu(0,1)] * 1.0 / sum;

				prod2 = hop_freqs[{{1},{1},{1}}][pu(0,1)] * 1.0 / sum;

				rows = {{1, 1},{1}};
				diag.init_by_rows(rows);
				sum = 0;
				for (auto it2 = points->begin(); it2 != points->end(); ++it2)
					sum += hop_freqs[rows][it2->first];

				cout << " " << hop_freqs[{{1, 1},{1}}][pu(2,0)] * 1.0 / sum;

				prod2 *= hop_freqs[{{1, 1},{1}}][pu(2,0)] * 1.0 / sum;

				cout << " " << prod1 - prod2;

	/*
				for (auto it = hop_freqs.begin(); it != hop_freqs.end(); ++it)
				{
					diag.init_by_rows(it->first);

					sum = 0;
					for (auto it2 = points->begin(); it2 != points->end(); ++it2)
						sum += hop_freqs[it->first][it2->first];

					for (auto it2 = points->begin(); it2 != points->end(); ++it2)
					{
						z = it2->first.first;
						y = it2->first.second;
						cout << fixed << setprecision(6) << hop_freqs[it->first][it2->first] * 1.0 / sum << " ";
					}
				}
	*/
				cout << endl;
			}



	/*
			//print frequencies each PRINT_STEP iteration
			if ((iter + 1) % PRINT_STEP == 0)
			{
				//print empirical probabilities
				f_out.open(("schutz_probs" + to_string(iter+1) + ".txt").c_str());
				for (auto it = hop_freqs.begin(); it != hop_freqs.end(); ++it)
				{
					diag.init_by_rows(it->first);
					diag.print_cols(f_out);

					sum = 0;
					/////for (auto it2 = points->begin(); it2 != points->end(); ++it2)
					/////	sum += hop_freqs[it->first][it2->first];

					for (auto it2 = points->begin(); it2 != points->end(); ++it2)
					{
						z = it2->first.first;
						y = it2->first.second;
					
						f_out << z << "," << y << " " << hop_freqs[it->first][it2->first] << endl;
						//f_out << z << "," << y << " " << hop_freqs[it->first][it2->first] * 1.0 / sum << " " << diag.calc_planch_weight(z, y) << endl;
					}
					//f_out << endl;
				}
				f_out.close();
			}
	*/
	}
	}
}

void Schutzenberger::schutzenberger3D_seek_central_process2(const uint64 DIAGS_NUM, vvi* last_rows)
/*
Difference from schutzenberger3D_seek_central_process:
Here we firstly construct DIAGS_NUM different random diagrams.
Then, we start schutzenberger's independently for each diagram.
We collect the united statistics from all diagrams.
*/
{
	const bool LAST_RANDOM  = false; //Last box is added randomly (unif.)
	const bool BEGIN_RANDOM = true; //The beginning part of tab (before diag {2 1, 1}) is random.
	const bool DIAGS_TO_PNG = true;

	const uint64 INIT_DIAG_SIZE = 100; //Size of initial big diagram
	const uint64 MAX_LEVEL = 10;          //Max.size of diagram for which we calculate the level
	const uint64 PRINT_STEP = 100;
	const uint64 ITER_NUM = PRINT_STEP+1;          //Number of iterations (to calculate freqs)
	Diagram3D diag, diag2;
	vector<Diagram3D> diags;
	vector<dtu> in_tabs;
	dtu out_tab;
	uint64 x, y, z, i, j, k, iter, tab_id;
	pu coord;
	tu xyz;
	vi vec;
	vvi rows;
	map<vvi, pu> diag_freqs;

	if ((DIAGS_NUM > 1) && (last_rows != NULL))
	{
		cout << "schutzenberger3D_seek_central_process2(): if last_rows !=NULL, DIAGS_NUM must be == 1\n";
		exit(1);
	}

	ofstream f_out;

	vec.push_back(1);

	in_tabs.resize(DIAGS_NUM);

	//Initial diagrams:
	for (i = 0; i < DIAGS_NUM; ++i)
	{
		diag.init();
		if (last_rows == NULL)
			for (j = 1; j < INIT_DIAG_SIZE; ++j)
			{
				coord = Process::add_rand_plansh3D(&diag, NULL, true, 0.769);
				z = coord.first; y = coord.second;
				x = diag.getX(z, y);
				in_tabs.at(i).push_back(tu(x, y, z));
			}
		else
		{
			diag2.init_by_rows(*last_rows);
			while (!diag.is_equal_to(&diag2))
			{
				coord = Process::add_lexicogr3D(&diag, 0.3, 0.3, &diag2);
				z = coord.first; y = coord.second;
				x = diag.getX(z, y);
				in_tabs.at(i).push_back(tu(x, y, z));
			}
/*
cout << in_tabs.at(i).size() << endl;
for (auto it = in_tabs.at(i).begin(); it != in_tabs.at(i).end(); ++it)
	cout << it->x << "," << it->y << "," << it->z << " ";
cout << endl;
exit(1);
*/
		}

		if (LAST_RANDOM)
			diags.push_back(diag);
	}


	for (iter = 0; iter < ITER_NUM; ++iter)
	{
		for (tab_id = 0; tab_id < DIAGS_NUM; ++tab_id)
		{
			if (BEGIN_RANDOM)
			{
//cout << "ITER: " << iter << endl;
				xyz = schutzenberger3D_randbegin(in_tabs.at(tab_id), out_tab, 3);
			}
			else
				xyz = schutzenberger3D(in_tabs.at(tab_id), out_tab);
			in_tabs.at(tab_id) = out_tab;

			if (LAST_RANDOM)
			{
				coord = pu(xyz.z, xyz.y);
				diags.at(tab_id).remove(coord);
				coord = Process::add_rand3D(&diags.at(tab_id));
				in_tabs.at(tab_id).back() = tu(diags.at(tab_id).getX(coord), coord.second, coord.first);
				//if (DIAGS_TO_PNG)
				//	diags.at(tab_id).print_to_img(.....d,,);
			}

			rows.clear();
			rows.push_back(vec);
			for (j = 0; (j <= out_tab.size()) && (j < MAX_LEVEL); ++j)
			{

				if (diag_freqs.find(rows) == diag_freqs.end())
				{
					diag_freqs[rows].first = 0;
					for (i = 0; i < rows.size(); ++i)
						for (k = 0; k < rows.at(i).size(); ++k) 
							diag_freqs[rows].first += rows.at(i).at(k);
					diag_freqs[rows].second = 1;
				}
				else
					diag_freqs[rows].second++;

				if (j == out_tab.size())
					break;


				z = out_tab.at(j).z; y = out_tab.at(j).y;
				if (z == rows.size())
					rows.push_back(vec);
				else
				{
					if (y == rows.at(z).size())
						rows.at(z).push_back(1);
					else
						rows.at(z).at(y)++;
				}
			}
		}

		if ((iter + 1) % PRINT_STEP == 0)
		{
			f_out.open(("schutz_manydiags_" + to_string(iter+1) + ".txt").c_str());

			for (map<vvi, pu>::iterator it = diag_freqs.begin(); it != diag_freqs.end(); ++it)
			{
				//print diag:
				f_out << "\"";
				for (i = 0; i < it->first.size(); ++i)
				{
					for (j = 0; j < it->first.at(i).size(); ++j)
					{
						f_out << it->first.at(i).at(j);
						if (j + 1 != it->first.at(i).size())
							f_out << " ";
					}
					if (i + 1 != it->first.size())
						f_out << ", ";
				}
				f_out << "\" ";

				//print statistics:
				f_out << it->second.second << " " << fixed << setprecision(20) << it->second.second * 1.0 / ((iter+1)*DIAGS_NUM) << endl;
			}
			f_out.close();
		}


	}
}

//double schutzenberger3D_seek_central_process3(const uint64 DIAGS_NUM, vvi* last_rows = NULL)
double Schutzenberger::schutzenberger3D_seek_central_process3(const uint64 DIAGS_NUM, dtu* init_tab)
/*
Difference from schutzenberger3D_seek_central_process:
Here we firstly construct DIAGS_NUM different random diagrams.
Then, we start schutzenberger's independently for each diagram.
We collect the united statistics from all diagrams.
We calculate the frequencies of HOPS, not of diagrams!
If last_rows != NULL, we construct only one diagram initialized by these rows.
*/
{
	const bool LAST_RANDOM  = false; //Last box is added randomly (unif.)
	const bool BEGIN_RANDOM2 = true;  //The beginning part of tab (before {1,1},{1 1},{2}) is random.
	const bool BEGIN_RANDOM3 = false; //The beginning part of tab (before diag {2 1, 1}) is random.
	const bool DIAGS_TO_PNG = false;
	const bool CHECK_LAST_HOP_ONLY = false;

	const uint64 ITER_NUM = 1000000000;          //Number of iterations (to calculate freqs)
	const uint64 PRINT_STEP = 10000;
	const uint64 INIT_DIAG_SIZE = 1000; //Size of initial big diagram
	const uint64 MAX_LEVEL = 9;          //Max.size of diagram for which we calculate the level

	Diagram3D diag, diag2, penult_diag;
	vector<Diagram3D> diags;
	vector<dtu> in_tabs;
	dtu out_tab;
	uint64 x, y, z, i, j, k, iter, tab_id, num;
	pu coord, lastcoord;
	tu xyz;
	vi vec;
	vvi rows;
	map<vvi, map<pu, uint64> > hop_freqs;
	double tmpd;
//	map<vvi, pu> diag_freqs;

	if ((DIAGS_NUM > 1) && (init_tab != NULL))
	{
		cout << "schutzenberger3D_seek_central_process3(): if init_tab !=NULL, DIAGS_NUM must be == 1\n";
		exit(1);
	}

	ofstream f_out;

	vec.push_back(1);

	in_tabs.resize(DIAGS_NUM);

	//Initial diagrams:
	for (i = 0; i < DIAGS_NUM; ++i)
	{
		diag.init();
		if (init_tab == NULL)
		{
			ifstream f_in("results/standard3D_planch_tab_a_1.000000_i_0.txt");
			for (j = 1; j < INIT_DIAG_SIZE; ++j)
			{
				//coord = Process::add_rand_plansh3D(&diag, NULL, true, 0.769);
				//z = coord.first; y = coord.second; x = diag.getX(z, y);
				f_in >> x; f_in >> y; f_in >> z; f_in >> tmpd; f_in >> tmpd;
				in_tabs.at(i).push_back(tu(x, y, z));
				diag.add(pu(z, y));
			}
		}
		else
		{
/*
			diag2.init_by_rows(*last_rows);
			while (!diag.is_equal_to(&diag2))
			{
				coord = Process::add_lexicogr3D(&diag, 0.3, 0.3, &diag2);
				z = coord.first; y = coord.second;
				x = diag.getX(z, y);
				in_tabs.at(i).push_back(tu(x, y, z));
			}
*/
			in_tabs.at(0) = *init_tab;
			for (j = 0; j < init_tab->size(); ++j)
			{
				z = init_tab->at(j).z; y = init_tab->at(j).y;
				coord = pu(z, y);
				diag.add(coord);
			}
		}
		lastcoord = coord;
		penult_diag.init(&diag); 

penult_diag.remove(lastcoord);

		if (LAST_RANDOM)
			diags.push_back(diag);
	}

	for (iter = 1; iter <= ITER_NUM; ++iter)
	{
		for (tab_id = 0; tab_id < DIAGS_NUM; ++tab_id)
		{

			if (BEGIN_RANDOM2)
				xyz = schutzenberger3D_randbegin(in_tabs.at(tab_id), out_tab, 2);
			else
				if (BEGIN_RANDOM3)
					xyz = schutzenberger3D_randbegin(in_tabs.at(tab_id), out_tab, 3);
				else
					xyz = schutzenberger3D(in_tabs.at(tab_id), out_tab);
			in_tabs.at(tab_id) = out_tab;

			if (LAST_RANDOM)
			{
				coord = pu(xyz.z, xyz.y);
				diags.at(tab_id).remove(coord);
				coord = Process::add_rand3D(&diags.at(tab_id));
				in_tabs.at(tab_id).back() = tu(diags.at(tab_id).getX(coord), coord.second, coord.first);
				//if (DIAGS_TO_PNG)
				//	diags.at(tab_id).print_to_img(.....d,,);
			}

			rows.clear();
			rows.push_back(vec);
			for (j = 0; (j < out_tab.size()) && ((j < MAX_LEVEL) || CHECK_LAST_HOP_ONLY); ++j)
			{
//NEED TO CHECK WITH MAX_LEVEL < out_tab.size()!!!!!!!!!!!!!!!!!!!!!!!!!!!
				xyz = out_tab.at(j);
				z = xyz.z; y = xyz.y;

				if ((!CHECK_LAST_HOP_ONLY) || (j + 1 == out_tab.size()))
				{
					coord = pu(z, y);
					if (hop_freqs.find(rows) == hop_freqs.end())
						hop_freqs[rows][coord] = 1;
					else
						++hop_freqs[rows][coord];
				}

				if (z == rows.size())
					rows.push_back(vec);
				else
				{
					if (y == rows.at(z).size())
						rows.at(z).push_back(1);
					else
						rows.at(z).at(y)++;
				}
			}

		}

		if ((iter) % PRINT_STEP == 0)
		{
			f_out.open(("schutz_hopfreqs_" + to_string(iter) + ".txt").c_str());
			f_out << iter << " iterations\n";
			f_out << DIAGS_NUM << " different tabs\n";
			f_out << "last coord: " << lastcoord.first << "," << lastcoord.second << endl;
			f_out << fixed << setprecision(20);
			for (map<vvi, map<pu, uint64> >::iterator it = hop_freqs.begin(); it != hop_freqs.end(); ++it)
			{
				//print diag:
				f_out << "\"";
				for (i = 0; i < it->first.size(); ++i)
				{
					for (j = 0; j < it->first.at(i).size(); ++j)
					{
						f_out << it->first.at(i).at(j);
						if (j + 1 != it->first.at(i).size())
							f_out << " ";
					}
					if (i + 1 != it->first.size())
						f_out << ", ";
				}
				f_out << "\"\n";

				//print statistics:
//map<vvi, map<pu, uint64> > hop_freqs;
				for (map<pu, uint64>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
				{
					z = it2->first.first; y = it2->first.second;
					num = it2->second;
					f_out << z << "," << y << " " << num << " " << num * 1.0 / ((iter)*DIAGS_NUM) << endl;
				}
				f_out << endl;

			}
			f_out.close();
		}


	}

	return hop_freqs.at(*penult_diag.get_rows()).at(lastcoord) * 1.0 / (ITER_NUM*DIAGS_NUM);
}

void Schutzenberger::schutzenberger3D_seek_central_process3_process(const char* source)
/*
Processes the data from file produced by schutzenberger3D_seek_central_process3 and
presents it in a better way.
*/
{
	const bool STANDARD = true;
	const bool CALC_DIM = true;
	const double DIAG_FREQ_FRACS = true;

	struct diag_stat
	{
		uint64 size;
		uint64 dim;
		uint64 count;
		double freq;
		map<pu, pair<uint64, double> > trans;

		diag_stat()
		{
			size = 1;
			dim = 1;
			count = 0;
			freq = 0;
		}
	};


	Diagram3D diag(STANDARD), diag2(STANDARD);

	map<vvi, diag_stat> diags;

	string str, str2, skip;
	uint64 num, i, j, k, size, sym_num, z, y, dim, dim2, max_size, ITERNUM;
	double freq, freq2, trans_freq, average;
	vvi rows;
	ifstream f;
	map<uint64, map<vvi, uint64> > dims;
	mpu* points;
	vi noms, dens;
	map<pu, pair<uint64, double> > trans;
	pu coord;

	f.open(source);
	if (!f.is_open())
	{
		cout << "Could not open file " << source << endl;
		exit(1);
	}

	points = diag.get_points();

	//read the diagrams from a source file
	max_size = 0;
	getline(f, str);
	sscanf(str.c_str(), "%llu", &ITERNUM);
	getline(f, str);
	sscanf(str.c_str(), "%llu", &i);
	ITERNUM *= i;

	while(getline(getline(f, skip, '"'), str, '"'))
	{
		getline(f,skip);
		str_to_rows(str, rows);
		diag.init_by_rows(rows);

		trans.clear();

		while(getline(f, str))
		{
			if (str == "")
				break;

			sscanf(str.c_str(), "%llu,%llu %llu %lf", &z, &y, &num, &freq);
			//cout << z << " " << y << " " << num << " " << freq << endl;

			trans[pu(z, y)].first = num;
			trans[pu(z, y)].second = freq;

			diag2.init(&diag);
			diag2.add(z, y);
			diags[*diag2.get_rows()].count += num;
			diags[*diag2.get_rows()].size = diag2.get_size();
			if (diag2.get_size() > max_size)
				max_size = diag2.get_size();
		}

		diags[rows].trans = trans;
		diags[rows].size = diag.get_size();
	
	}
	f.close();

	//calculate dimensions and frequencies of diagrams:
	if (CALC_DIM)
		dims = Diagram3D::calc_all_dimensions_UINT64(STANDARD, max_size);
	
	for (auto it = diags.begin(); it != diags.end(); ++it)
	{
		if (it->second.size == 1)
		{
			it->second.count = ITERNUM;
			it->second.freq = 1;
		}

		if (CALC_DIM)
			it->second.dim = dims.at(it->second.size).at(it->first);
		it->second.freq = it->second.count * 1.0 / ITERNUM;
	}

	//OUTPUT
	cout << fixed << setprecision(20);
	for (auto it = diags.begin(); it != diags.end(); ++it)
	{
		//Print rows
		rows = it->first;
		diag.init_by_rows(rows);
		cout << "\"";
		diag.print_rows(cout, false);
		cout << "\" "  << endl;

		//Prnt diag's stat
		cout << "size: " << it->second.size << endl;
		if (CALC_DIM)
			cout << "dim: " << it->second.dim << endl;
		cout << "count: " << it->second.count << endl;
		cout << "freq: " << it->second.freq << " ";
		if (DIAG_FREQ_FRACS)
		{
			noms.clear(); dens.clear();
			calc_fractions(it->second.freq, noms, dens);
			for (i = 0; i < noms.size(); ++i)
				cout << noms.at(i) << "/" << dens.at(i) << " ";
		}
		cout << endl;

		//Print transitions
		if (it->second.trans.size() > 0)
			cout << "transitions:" << endl;
		for (auto it2 = it->second.trans.begin(); it2 != it->second.trans.end(); ++it2)
		{
			coord = it2->first;
			num = it2->second.first;
			freq = it2->second.second;

			diag2.init(&diag);
			diag2.add(coord);
			if (CALC_DIM)
			{
				dim = it->second.dim;
				dim2 = dims.at(it->second.size + 1).at(*diag2.get_rows());
				cout << coord.first << "," << coord.second << ": " << dim << "/" << dim2 << " " << num << " " << freq << endl;
			}
			else
				cout << coord.first << "," << coord.second << ": " << num << " " << freq << endl;
		}
		cout << endl;
	}
}

void Schutzenberger::schutzenberger3D_seek_central_process3_get_diagfreqs(const vvi rows)
/*
This function analyses many files produced by schutzenberger3D_seek_central_process3 (for different number
of iterations passed)
In each file it collects the frequency of diagram with rows "rows".
*/
{
/*
	const char* DIRNAME = "results/schutz_hopfreqs_single_diag_n_1000_1000000000iters/";
	const uint64 MAX_ITER = 1000000000;
	const uint64 STEP = 10000000;

	uint64 iternum, diag_size;
	ifstream f_in;
	string str, skip;
	Diagram3D diag;


	diag.init_by_rows(rows);
	diag_size = diag.get_size();

	for (iternum = STEP; iternum <= MAX_ITER; iternum += STEP)
	{
		f_in.open((string(DIRNAME) + "schutz_hopfreqs_" + to_string(iternum) + ".txt").c_str());

		while(getline(getline(f, skip, '"'), str, '"'))
		{
			getline(f,skip);
			str_to_rows(str, rows);
			diag.init_by_rows(rows);

			if (diag.get_size() >= diag_size())
			{
				do
				{
					getline(f, str);
				}
				while(str != "");
				continue;
			}
			trans.clear();

			while(getline(f, str))
			{
				if (str == "")
					break;

				sscanf(str.c_str(), "%llu,%llu %llu", &z, &y, &num);

				trans[pu(z, y)].first = num;

				diag2.init(&diag);
				diag2.add(z, y);
				diags[*diag2.get_rows()].count += num;
				diags[*diag2.get_rows()].size = diag2.get_size();
				if (diag2.get_size() > max_size)
					max_size = diag2.get_size();
			}

			diags[rows].trans = trans;
			diags[rows].size = diag.get_size();

		}



		f_in.close();
	}
*/
}

void Schutzenberger::schutzenberger3D_seek_central_process2_process(const char* source)
/*
Processes the data from file produced by schutzenberger3D_seek_central_process2 and
presents it in a better way.
*/
{
	const bool STANDARD = true;

	struct diags_stat
	{
		vector<Diagram3D> diags; //symmetric diagrams
		vi nums;		 //empirical number of occurancies 
		vd freqs;		 //empirical frequencies (of all symmetric diagrams)
		double aver_freq;        //empirical frequency
		vi noms, dens;           //nominators and denominators of rational approximations of freq.
		uint64 dim;		 //dimension
		double pp_weight;        //pseudo-Plancherel weight
	};
	
	struct fraccmp
	{
		bool operator()(const pu frac1, const pu frac2) const
		{
			return frac1.second < frac2.second;
		}
	};

	Diagram3D diag(STANDARD), diag2(STANDARD);
	map<uint64, vector<tuple<Diagram3D, uint64, double> > > diags, diags2;
	map<uint64, vector<diags_stat> > sym_diags;
	//ifstream f;
	string str_cols, layer, skip;
	uint64 num, i, j, k, size, sym_num, z, y, dim, dim2;
	double freq, freq2, trans_freq, average;
	vvi cols;
	vi col;
	ifstream f;
	map<uint64, map<vvi, uint64> > dims;
	mpu* points;
	vi noms, dens;
	map<pair<uint64, uint64>, double, fraccmp> freq_apprxs;
	map<uint64, vvi> testing_noms, testing_dens;
	vvpu solutions;

	f.open(source);
	if (!f.is_open())
	{
		cout << "Could not open file " << source << endl;
		exit(1);
	}

	points = diag.get_points();

	//read the diagrams from a source file
	while(getline(getline(f, skip, '"'), str_cols, '"') >> num >> freq)
	{
		std::size_t prev = 0, pos;
		cols.clear();

		do
		{
			pos = str_cols.find_first_of(",", prev);

			if (pos == std::string::npos)
				layer = str_cols.substr(prev, std::string::npos);
			else
				if (pos > prev)
					layer = str_cols.substr(prev, pos-prev);
			prev = pos + 1;

			istringstream is (layer);
			col.clear();
			while (is >> i)
				col.push_back(i);
			cols.push_back(col);
		}
		while (pos != std::string::npos);

		diag.init_by_cols(cols);

		diags[diag.get_size()].push_back(tuple<Diagram3D, uint64, double>(diag, num, freq));
	}
	f.close();

	//seek for symmetric diagrams:
	diags2 = diags;

	for (auto it = diags.begin(); it != diags.end(); ++it)
	{
		size = it->first;
		//cout << "SIZE: " << size << endl;

		do
		{
			diag = get<0>(diags.at(size).at(0));
			num  = get<1>(diags.at(size).at(0));
			freq = get<2>(diags.at(size).at(0));

			sym_diags[size].push_back(diags_stat());
			sym_diags[size].back().diags.push_back(diag);
			sym_diags[size].back().nums.push_back(num);
			sym_diags[size].back().freqs.push_back(freq);

			average = freq;
			sym_num = 1;

			//cout << "\""; diag.print_cols(cout, false);
			//cout << "\"\n"; 
			//cout << num << " " << fixed << setprecision(20) << freq << endl;

			diags2.at(size).erase(remove(diags2.at(size).begin(), diags2.at(size).end(), tuple<Diagram3D, uint64, double>(diag, num, freq)), diags2.at(size).end());
			for (i = 1; i < diags.at(size).size(); ++i)
			{
				diag2 = get<0>(diags.at(size).at(i));
				if (diag.is_symmetric_to(diag2))
				{
					num   = get<1>(diags.at(size).at(i));
					freq  = get<2>(diags.at(size).at(i));
					//cout << "\""; diag2.print_cols(cout, false);
					//cout << "\" " << num << " " << fixed << setprecision(20) << freq << endl;
					sym_diags[size].back().diags.push_back(diag2);
					sym_diags[size].back().nums.push_back(num);
					sym_diags[size].back().freqs.push_back(freq);
					diags2.at(size).erase(remove(diags2.at(size).begin(), diags2.at(size).end(), tuple<Diagram3D, uint64, double>(diag2, num, freq)), diags2.at(size).end());

					average += freq;
					++sym_num;
				}
			}
			if (sym_num > 1)
				average /= sym_num;

			sym_diags[size].back().aver_freq = average;

			diags.at(size) = diags2.at(size);
		}
		while (diags2.at(size).size() != 0);

	}

	//collect an additional statistics about diagrams:	
	dims = Diagram3D::calc_all_dimensions_UINT64(STANDARD, diags.size());

	sym_diags[1].at(0).noms = vi({1});
	sym_diags[1].at(0).dens = vi({1});
	for (size = 2; size <= sym_diags.size(); ++size)
		for (i = 0; i < sym_diags[size].size(); ++i)
		{
			diag = sym_diags[size].at(i).diags.at(0);
			sym_diags[size].at(i).dim = dims[size][*diag.get_rows()];

			calc_fractions(sym_diags[size].at(i).aver_freq, sym_diags[size].at(i).noms, sym_diags[size].at(i).dens);
			//sym_diags[size].at(i).pp_weight = diag.calc_planch_weight()???;
		}

	//Print the statistics:
	cout << fixed << setprecision(20);
	for (size = 1; size <= sym_diags.size(); ++size)
	{
		cout << "SIZE " << size << endl;
		for (i = 0; i < sym_diags.at(size).size(); ++i)
		{
			diag = sym_diags.at(size).at(i).diags.at(0);

			diag.print_cols();
			cout << "number of such diagrams: " << sym_diags[size].at(i).diags.size() << endl;

			cout << "dim: " << sym_diags.at(size).at(i).dim << endl;

			//cout << "generated " << sym_diags[size].at(i).num << " times" << endl;
			cout << "aver.freq: " << sym_diags[size].at(i).aver_freq << " ";
			for (j = 0; j < sym_diags[size].at(i).noms.size(); ++j)
				cout << sym_diags[size].at(i).noms.at(j) << "/" << sym_diags[size].at(i).dens.at(j) << " ";
			cout << endl;
			cout << "all freqs:\n";
			for (j = 0; j < sym_diags[size].at(i).freqs.size(); ++j)
			{
				freq = sym_diags[size].at(i).freqs.at(j);
				num  = sym_diags[size].at(i).nums.at(j);
				cout << freq << ", " << num << endl;

				//approximation:
				noms.clear(); dens.clear(); freq_apprxs.clear();
				calc_fractions(freq, noms, dens);
				for (k = 0; k < noms.size(); ++k)
					freq_apprxs[pu(noms.at(k), dens.at(k))];
			}
			for (auto it = freq_apprxs.begin(); it != freq_apprxs.end(); ++it)
				cout << it->first.first << "/" << it->first.second << " ";
			cout << endl;

			cout << "path prob: " << sym_diags[size].at(i).aver_freq / sym_diags[size].at(i).dim << endl;

			if (size == sym_diags.size())
			{
				cout << endl;
				continue;
			}
			cout << "transitions:\n";
			for (mpu::iterator it = points->begin(); it != points->end(); ++it)
			{
				z = it->first.first; y = it->first.second;
				diag2.init(&diag);
				diag2.add(z, y);
				//find the record of a bigger diagram
				for (j = 0; j < sym_diags[size+1].size(); ++j)
				{
					for (k = 0; k < sym_diags[size+1].at(j).diags.size(); ++k)
					{
						if (sym_diags[size+1].at(j).diags.at(k) == diag2)
							break;
					}
					if (k != sym_diags[size+1].at(j).diags.size())
						break;
				}

				cout << z << "," << y << ": ";

				if (j == sym_diags[size+1].size())
					cout << "no info";
				else
				{
					dim = sym_diags[size].at(i).dim;
					freq = sym_diags[size].at(i).aver_freq;
					dim2 = sym_diags[size+1].at(j).dim;
					freq2 = sym_diags[size+1].at(j).aver_freq;

					trans_freq = (freq2 / dim2) / (freq / dim);

					cout << dim << "/" << dim2 << " " << trans_freq << " ";
					noms.clear(); dens.clear();
					calc_fractions(trans_freq, noms, dens);
					for (j = 0; j < noms.size(); ++j)
						cout << noms.at(j) << "/" << dens.at(j) << " ";
				} 
				cout << endl;
			}
			cout << endl << endl;


		}
		cout << endl << endl;
	}

	//Graph: freq (dim)
	map<uint64, vd> dim_freq;
	ofstream f_out("freq_of_dim.txt");
	for (size = 1; size <= sym_diags.size(); ++size)
	{
		dim_freq.clear();
		for (i = 0; i < sym_diags.at(size).size(); ++i)
			dim_freq[sym_diags.at(size).at(i).dim].push_back(sym_diags.at(size).at(i).aver_freq);
		for (auto it = dim_freq.begin(); it != dim_freq.end(); ++it)
			for (i = 0; i < it->second.size(); ++i)
				f_out << size << " " << it->first << " " << it->second.at(i) << endl;

	}

	f_out.close();

/*
	cout << "POSSIBLE APPROXIMATIONS OF FREQUENCIES\n";
	for (size = 1; size <= sym_diags.size(); ++size)
	//for (size = 1; size <= 4; ++size)
	{
		for (i = 0; i < sym_diags.at(size).size(); ++i)
		{
//cout << "i: " << i << endl;
//cout << sym_diags.at(size).at(i).noms.size() << endl;

			testing_noms[size].push_back(sym_diags.at(size).at(i).noms);
			testing_dens[size].push_back(sym_diags.at(size).at(i).dens);
			num = sym_diags.at(size).at(i).diags.size();
//cout << num << endl;
			if (num > 1)
				for (j = 0; j < testing_noms[size].back().size(); ++j)
					testing_noms[size].back().at(j) *= num;
		}

		cout << "size " << size << endl;
		solutions.clear();

//cout << "to fit:\n";
//for (i = 0; i < testing_noms.at(size).size(); ++i)
//{
//	for (j = 0; j < testing_noms.at(size).at(i).size(); ++j)
//		cout << testing_noms.at(size).at(i).at(j) << "/" << testing_dens.at(size).at(i).at(j) << " ";
//	cout << endl;
//}
//cout << endl;

		fractions_fitting(testing_noms.at(size), testing_dens.at(size), solutions);
		if (solutions.size() == 0)
			cout << "no solutions\n";
		else
		{
			for (i = 0; i < solutions.size(); ++i)
			{
				for (j = 0; j < solutions.at(i).size(); ++j)
					cout << solutions.at(i).at(j).first << "/" << solutions.at(i).at(j).second << " ";
				cout << endl;
			}
		}
	}
*/
}

void Schutzenberger::add_min_coprob3D(Diagram3D& diag, dtu& tab, const uint64 ITER_NUM)
/*
	For each point we calculate the cotransition probability (using Schutzenberger iterations).
	ITER_NUM iterations for each point.
	The coordinate of a point with minimum cotransition prob is added to diag and to tab.
*/
{
	mpu::iterator it;
	//dtu tab2;
	mpu* points;
	uint64 x, y, z;	
	double coprob, min;
	tu coord, mincoord;

	points = diag.get_points();
	
	min = 1.1;
	for (it = points->begin(); it != points->end(); ++it)
	{
		//tab2 = tab;
		z = it->first.first; y = it->first.second;
		x = diag.getpointX(z, y);
		coord = tu(x, y, z);
		//perenes vniz: tab2.push_back(coord);

		///////////////coprob = schutzenberger3D_seek_central_process3(1, &tab2);
		coprob = Schutzenberger::calc_coprob3D(tab, coord, ITER_NUM);
		//tab2.push_back(coord);

		if (coprob < min)
		{
			min = coprob;
			mincoord = coord;
		}
	}
	tab.push_back(mincoord);

	diag.add(mincoord.z, mincoord.y);
}

double Schutzenberger::calc_coprob3D(dtu& init_tab, const tu lastcoord, const uint64 ITER_NUM)
{
	dtu in_tab, out_tab;
	uint64 iter, num, i;
	tu coord;

	map<tu,int> TMP;
	
	in_tab = init_tab;

//!!!!!!!!!!!!!!!!!!!!!!!!!!1
uint64 cyclelen;
dtu cycletab = init_tab;
cycletab.push_back(lastcoord);
//!!!!!!!!!!!!!!!!!!!!!!!!!

	in_tab.push_back(lastcoord);

	num = 0;
//////////////
//srand(time(NULL));
/////////////

cyclelen=0;

	for (iter = 1; iter <= ITER_NUM; ++iter)
	{
/*
cout << "intab:\n";
for (i = 0; i < in_tab.size(); ++i)
	cout << in_tab.at(i).x << "," << in_tab.at(i).y << "," << in_tab.at(i).z << " ";
cout << endl;
*/
		coord = schutzenberger3D_randbegin(in_tab, out_tab, 2); 
		//coord = schutzenberger3D(in_tab, out_tab);
		////////

/*
cout << "outtab:\n";
for (i = 0; i < out_tab.size(); ++i)
	cout << out_tab.at(i).x << "," << out_tab.at(i).y << "," << out_tab.at(i).z << " ";
cout << endl;
*/
		++cyclelen;
		if (out_tab == cycletab)
		{
			//cout << "CYCLE: " << cyclelen << endl;
			//cyclelen = 0;
		}

		
		if (coord == lastcoord)
			++num;
		
		TMP[coord]++;
/*
cout << "in:\n";
for (auto it = in_tab.begin(); it != in_tab.end(); ++it)
	cout << it->x << "," << it->y << "," << it->z << " ";
cout << endl;
cout << "out:\n";
for (auto it = out_tab.begin(); it != out_tab.end(); ++it)
	cout << it->x << "," << it->y << "," << it->z << " ";
cout << endl;
*/
		in_tab = out_tab;
	}

	//cout << "\nkoperehodnye\n";
	//for (auto it = TMP.begin(); it != TMP.end(); ++it)
	//	cout << it->first.x << "," << it->first.y << "," << it->first.z << " " << it->second << endl;

	return num * 1.0 / ITER_NUM;
}

void Schutzenberger::nerve_asymptotics2D()
{
	const uint64 DIAG_SIZE = 14000000;
	const uint64 NUM_OF_NERVES = 10;

	ifstream f_in;
	ofstream f_out;
	//Diagram diag;
	string str;
	uint64 n, x, y, i, nervenum;
	dpu in_tab, out_tab, nerve;

	vi diag_sizes;

	f_in.open("results/standard_planch_tab_14mln.txt");

	if (!f_in.is_open())
	{
		cout << "Error: nerve_asymptotics2D: cannot open file\n";
		exit(1);
	}

	//1. Prepare a big diagram
	n = 1;
	while (getline(f_in, str) && (++n <= DIAG_SIZE))
	{
		sscanf(str.c_str(), "%llu %llu", &x, &y);
		//diag.add(y);
		in_tab.push_back(pu(x,y));
	}
	f_in.close();

	for (nervenum = 0; nervenum < NUM_OF_NERVES; ++nervenum)
	{
		f_out.open(("2Dnerve_asymptotics" + to_string(nervenum) + ".txt").c_str());
		//2. Calculate a schutzenberger nerve:
		schutzenberger2D(in_tab, &out_tab, &nerve);

		//3. For each point of nerve, we calculate the corresponding diagram's size:
		for (n = 0, i = 0; n < out_tab.size(); ++n)
		{
			x = out_tab.at(n).first;
			y = out_tab.at(n).second;
			if (pu(x,y) == nerve.at(i))
			{
				diag_sizes.push_back(n+1);
				if (++i == nerve.size())
					break;
			}
		}

		//4. For each point of nerve, print (x+y)/sqrt(n):
		//f_out << fixed << setprecision(20);
		for (i = 0; i < nerve.size(); ++i)
		{
			x = nerve.at(i).first;
			y = nerve.at(i).second;
			//f_out << (x + y) / sqrt(diag_sizes.at(i)) << endl;
			f_out << diag_sizes.at(i) << " " << x << " " << y << endl;
		}
		f_out.close();
		diag_sizes.clear();
		in_tab = out_tab;
	}
}


//_____________________________________new_version______________________________________

QC Schutzenberger::schutzenberger4D(lqc& tab, dqc* sch_path, const bool addLast) {
        uchar x;
        uchar y;
        uchar z;
        uchar l;
        
        uchar actX;
        uchar actY;
        uchar actZ;
        uchar actL;
        
        lqc::iterator iter;

        actX = tab.front().getX();	
        actY = tab.front().getY();
        actZ = tab.front().getZ();
        actL = tab.front().getL();
        tab.pop_front();

       if (sch_path != NULL)
		sch_path->clear();

	iter = tab.begin();
        while (iter != tab.end()) {
                x = iter->getX();
                y = iter->getY();
                z = iter->getZ();
                l = iter->getL();

                if ((z == actZ && l == actL) && 
                ((x == actX + 1 && y == actY) || (x == actX && y == actY + 1))
                || (x == actX && y == actY) && 
                ((z == actZ + 1 && l == actL) || (z == actZ && l == actL + 1))) {

                        iter = tab.erase(iter);
                        tab.insert(iter, QC(actX, actY, actZ, actL));
                        if (sch_path != NULL)
                        	sch_path->push_back(QC(actX, actY, actZ, actL));

                	actX = x;
                	actY = y;
                	actZ = z;
                	actL = l;
                }
                else ++iter;
        }

        if (addLast) tab.push_back(QC(actX, actY, actZ, actL));

        if (sch_path != NULL)
        	sch_path->push_back(QC(actX, actY, actZ, actL));
        return QC(actX, actY, actZ, actL); 
}

Diagram Schutzenberger::buildDiag2D(lpc tab) {
	Diagram tempDiag;
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	while(i < size) {
	        pc temp = tab.front();
		if(i != 0) tempDiag.add(temp.second);	//по у
	        tab.pop_front();
	        i = i + 1;
	}   
	return tempDiag;
}

Diagram3D Schutzenberger::buildDiag3D(ltc tab) { // таблица -> диаграмма
	Diagram3D tempDiag;
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	while(i < size) {
        	tc temp = tab.front();
        	if(i != 0) tempDiag.add(temp.z, temp.y); 
        	tab.pop_front();
        	i = i + 1;
    	} 
    	
 	return tempDiag;   	
}

Diagram4D Schutzenberger::buildDiag4D(lqc tab) {
	Diagram4D tempDiag;
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	
	while(i < size) {
		QC temp = tab.front();
		tempDiag.add(temp.getL(), temp.getZ(), temp.getY());
		tab.pop_front();
        	i = i + 1;
	}
	
 	return tempDiag;   
}

void Schutzenberger::findOcorners2D(lpc curTabs, map <lpc, oint128> &arrayTabs, Diagram form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit) {	
	mi *ocorners; 
	mi::iterator it; 
	
	Diagram tempDiag; 
	uchar x;
	uchar y;

	if(curTabs.size() > 1){
		tempDiag = buildDiag2D(curTabs);	
	}			
        ocorners = tempDiag.get_points();		
	it = ocorners->begin();
	
	while(it != ocorners->end()) {
     		y = it->first;
        	x = tempDiag.get_width(y);

            	if(x < form.get_width(y) && y < form.get_height(x)) {
    			pc newCell = make_pair(x, y);
            		createTabs2D(newCell, curTabs, arrayTabs, form, sizeForm, fout_logs, isLogs, memoryLimit);	
            	}
        it++;
        }
}

void Schutzenberger::findOcorners3D(ltc curTabs, map <ltc, oint128> &arrayTabs, Diagram3D form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit) {
	mpu *ocorners; 	// места, куда можно добавить клетку в текущей табл
	mpu::iterator it;
	
	Diagram3D tempDiag;    // таблица -> диаграмму (для работы с окорнерами)(изначально размер=1!!!!)
	
	uchar x;
	uchar y;
	uchar z;
	
	if(curTabs.size() > 1){ 
		tempDiag = buildDiag3D(curTabs);
	}	
	ocorners = tempDiag.get_points();
	it = ocorners->begin();
	
	while(it != ocorners->end()) {
		z = (it->first).first;
		y = (it->first).second;
		x = tempDiag.getpointX(z, y);
		if (x < form.getpointX(z, y) && y < form.getpointY(z, x)  && z < form.getpointZ(x, y)) {
			tc newCell = tc(x, y, z);
			createTabs3D(newCell, curTabs, arrayTabs, form, sizeForm, fout_logs, isLogs, memoryLimit);
		}
		it++;
	}
}

void Schutzenberger::findOcorners4D(lqc curTabs, map <lqc, oint128> &arrayTabs, Diagram4D form, int sizeForm, ofstream &logging, bool isLogs, float memoryLimit) {

	mtid ocorners;			// места, куда можно добавить клетку в текущей табл.
	mtid::iterator it;
	
	Diagram4D tempDiag;		 // таблица -> диаграмму (для работы с окорнерами)(изначально размер=0)
	
	uchar x;
	uchar y;
	uchar z;
	uchar l;
	
	tempDiag = buildDiag4D(curTabs);
	
	ocorners = tempDiag.getOcorners();	
	it = ocorners.begin();
	
	while(it != ocorners.end()) {
		x = tempDiag.get_oX(get<0>(it->first), get<1>(it->first), get<2>(it->first));
            	y = get<2>(it->first);
            	z = get<1>(it->first);
            	l = get<0>(it->first);

		if(x < form.get_oX(l, z, y) && y < form.get_oY(l, z, x) &&
            	z < form.get_oZ(l, x, y) && l < form.get_oD(x, y, z)) { 
            		QC newCell = QC(x, y, z, l);
            		createTabs4D(newCell, curTabs, arrayTabs, form, sizeForm, logging, isLogs, memoryLimit);
            	}
            	it++;	
	}
}

//можно сделать шаблоном!!?
void Schutzenberger::createTabs2D(pc newCell, lpc curTabs, map <lpc, oint128> &arrayTabs, Diagram form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit) {
	curTabs.push_back(newCell);
    			
    	if (curTabs.size() != sizeForm) {
    		checkMemory(memoryLimit, fout_logs, isLogs);
    		findOcorners2D(curTabs, arrayTabs, form, sizeForm, fout_logs, isLogs, memoryLimit);
    	}
    	else {
    		oint128 index = arrayTabs.size();
    		if(isLogs) fout_logs << "arrayTabs.size() = " << arrayTabs.size() / 1000000 << " mln " << endl;
    		arrayTabs.insert(make_pair(curTabs, index));
    		curTabs.clear();
    	}
}

void Schutzenberger::createTabs3D(tc newCell, ltc curTabs, map <ltc, oint128> &arrayTabs, Diagram3D form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit) {
	curTabs.push_back(newCell);
    			
    	if (curTabs.size() != sizeForm) {
    		checkMemory(memoryLimit, fout_logs, isLogs); 
    		findOcorners3D(curTabs, arrayTabs, form, sizeForm, fout_logs, isLogs, memoryLimit);
    	}
    	else {
    		oint128 index = arrayTabs.size();
    		if(isLogs) fout_logs << "arrayTabs.size() = " << arrayTabs.size() / 1000000 << " mln " << endl;
    		arrayTabs.insert(make_pair(curTabs, index));
    		curTabs.clear();
    	}
}

void Schutzenberger::createTabs4D(QC newCell, lqc curTabs, map <lqc, oint128> &arrayTabs, Diagram4D form, int sizeForm, ofstream &logging, bool isLogs, float memoryLimit) {
	curTabs.push_back(newCell);
	
	if (curTabs.size() != sizeForm) { 
		checkMemory(memoryLimit, logging, isLogs); 
		findOcorners4D(curTabs, arrayTabs, form, sizeForm, logging, isLogs, memoryLimit);
	}
	else { 
		oint128 index = arrayTabs.size();
		if(isLogs) logging << "arrayTabs.size() = " << arrayTabs.size() / 1000000 << " mln " << endl;
    		arrayTabs.insert(make_pair(curTabs, index));
    		curTabs.clear();
	}
 }

void Schutzenberger::CucleLengths2D(map <lpc, oint128> &allTabs, lpc &tab, ofstream &output) {
        lpc startTab;
        oint128 length;
        
        startTab = tab;
        length = 0;      
        do {              
		schutzenberger2D(tab);
		map <lpc, oint128>::iterator iterForRem = allTabs.find(tab);
  		if(iterForRem != allTabs.end()) allTabs.erase(iterForRem);
                length = length + 1;
        } while(tab!=startTab);
        
        output << length << " ";
}

void Schutzenberger::CucleLengths3D(map <ltc, oint128> &allTabs, ltc &tab, ofstream &output) {
        ltc startTab;
        oint128 length;
        
        startTab = tab;
        length = 0;
        do {       
		schutzenberger3D(tab);
		map <ltc, oint128>::iterator iterForRem = allTabs.find(tab);
  		if(iterForRem != allTabs.end()) allTabs.erase(iterForRem);
                length = length + 1;
        } while(tab!=startTab);
 	
 	output << length << " ";
}

void Schutzenberger::CucleLengths4D(map <lqc, oint128> &allTabs, lqc &tab, ofstream &output) {
        lqc startTab;
        oint128 length;
        
        startTab = tab;
        length = 0;   
        
        do {
        	schutzenberger4D(tab);
        	map <lqc, oint128>::iterator iterForRem = allTabs.find(tab);
  		if(iterForRem != allTabs.end()) allTabs.erase(iterForRem);
        	length = length + 1;
        } while(tab!=startTab); 
        
 	output << length << " ";         
}

int Schutzenberger::setCucleLengths2D(map <lpc, oint128> &allTabs, ofstream &output) { 
	int sizeLen = 0;
	while(!allTabs.empty()) {
        	lpc tab = allTabs.begin()->first; 
        	CucleLengths2D(allTabs, tab, output);  
        	sizeLen = sizeLen + 1;    	
    	}
    	output << endl;
    	return sizeLen;
}
// перенесети в абстрактный класс
int Schutzenberger::setCucleLengths3D(map <ltc, oint128> &allTabs, ofstream &output) {

	int sizeLen = 0;
	while(!allTabs.empty()) {
        	ltc tab = allTabs.begin()->first;  
        	CucleLengths3D(allTabs, tab, output);  
        	sizeLen = sizeLen + 1; 
    	}
    	output << endl;
    	return sizeLen;
}

int Schutzenberger::setCucleLengths4D(map <lqc, oint128> &allTabs, ofstream &output) {

	int sizeLen = 0;
	while(!allTabs.empty()) { 
		lqc tab = allTabs.begin()->first;
		CucleLengths4D(allTabs, tab, output);
		sizeLen = sizeLen + 1;
	}
	output << endl;
    	return sizeLen;
}

// почему дважды?
void Schutzenberger::checkMemory(float memoryLimit, ofstream &fout_logs, bool isLogs) {
	struct sysinfo si;
	sysinfo(&si);

	long double totalRAM;
	long double freeRAM;
	
	totalRAM = si.totalram;
	freeRAM = si.freeram;

	const double percent = totalRAM / 100.0;
	double freePercent;
	freePercent = freeRAM / percent;

	if(freePercent <= (double)memoryLimit) {
		if(isLogs) fout_logs << "<< Превышение ограничения по памяти >>" << endl;
		exit(0);
	}
}

void Schutzenberger::printUchar(uchar smb) {
	printf("%d", smb);
}

void Schutzenberger::printQU(QU active) {
        std::cout << "(" << active.getX() << "; " << active.getY() << "; ";
        std::cout << active.getZ() << "; " << active.getL() << ")";
        std::cout << "\n";
}

void Schutzenberger::printPU(pu active, ofstream &fout) {
        fout << "(" << active.first << "; " << active.second << ") ";
}

void Schutzenberger::printPC(pc active, ostream &output) {
        output << "(" ;
        printf("%d", active.first);
        output << "; ";
        printf("%d", active.second);
        output << ") " ;
        
}

void Schutzenberger::printTC(tc active) {
	std::cout << "(";
	printUchar(active.x);
	std::cout << "; ";
	printUchar(active.y);
	std::cout << "; ";
	printUchar(active.z);
	std::cout << ")" << endl;
}

void Schutzenberger::printQC(QC active, ostream &output) {	

	output << "(";
	printUchar(active.getX());
	output << "; ";
	printUchar(active.getY());
	output << "; ";
	printUchar(active.getZ());
	output << "; ";
	printUchar(active.getL());
	output << ")" << endl;
}

void Schutzenberger::printNerve(dqu sch_path){
        size_t i; 
        i = 0; 
        
        while(i < sch_path.size()) {
                printQU(sch_path[i]);
                std::cout << "\n";  
                i = i + 1;    
        } 
        
        std::cout << "\n";
}


void Schutzenberger::printLQU(lqu tab) {
        size_t size;
        size_t i;
        
        size = tab.size();
        i = 0;
        
        while(i < size) {
                printQU(tab.front());
                tab.pop_front();
                std::cout << "\n";
                ++i;
        } 
        std::cout << "\n";
}

void Schutzenberger::printLTC(ltc tab) {
	size_t size;
        size_t i;
        
        size = tab.size();
        i = 0;
        
         while(i < size) {
                printTC(tab.front());
                tab.pop_front();
                std::cout << "\n";
                ++i;
        } 
        std::cout << "\n";  
}

void Schutzenberger::printLQC(lqc tab, ostream &output) {
	size_t size;
        size_t i;
        
        size = tab.size();
        i = 0;
        
        while(i < size) {
                printQC(tab.front());
                tab.pop_front();
                output << "\n";
                ++i;
        } 
        output << "\n";  
}

void Schutzenberger::printLPU(lpu tab, ofstream &fout) {
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	
	while(i < size) {
                printPU(tab.front(), fout);
                tab.pop_front();
                ++i;
        } 
        fout << "\n";
}

void Schutzenberger::printLPC(lpc tab, ostream &output) {
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	
	while(i < size) {
                printPC(tab.front(), output);
                tab.pop_front();
                ++i;
        } 
        output << "\n";
}

/*lqu Schutzenberger::involution(lqu tab) {
        dqu sch_path;
        QU new_active;
        oint128 size;
        lqu tab2;
	size_t i;
	
	size = tab.size();
	i = 1;
	
	while(i <= size) {
                new_active = schutzenberger4D(tab, sch_path, false);
                tab2.push_front(new_active);

                std::cout << "Iteration is " << i << "\n";
                std::cout << "Active : (" << new_active.getX() << "; " << new_active.getY() << "; ";
                std::cout << new_active.getZ() << "; " << new_active.getL() << ")\n";
                std::cout << "Nerve: ";
                size_t j;
                j = 0;
                
                while(j < sch_path.size()) {
                       std::cout << "(" << sch_path[j].getX() << "; " << sch_path[j].getY() << "; ";
                       std::cout << sch_path[j].getZ() << "; " << sch_path[j].getL() << ")   ";
                	j = j + 1;
                } 
                std::cout << "\nTab size: " << tab.size() << "\n\n";
        i = i + 1;
        }
        return tab2;
}*/
