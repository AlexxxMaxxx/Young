/*
 * Sequence.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: vduzhin
 */

#include "Sequence.h"

Sequence::Sequence(const char* inputoutput, const char* t, const char* fn)
{
	if ((strcmp(inputoutput, "r") && strcmp(inputoutput, "w")) || (strcmp(t, "tab") && strcmp(t, "col")))
	{
		cout << "Sequence::Sequence: incorrect argument. inputoutput: " << inputoutput << ", t: " << t << endl;
		exit(1);
	}

	io = inputoutput;
	type = t;
	fname = fn;

	if (io == "r")
		f.open(fname, fstream::in);
	else
		f.open(fname, fstream::out | fstream::trunc);
}

Sequence::~Sequence()
{
	f.close();
}

int Sequence::read_standard(Diagram* diag)
{
	di cols;
	string str;
	int i;

	if (io != "r")
		return 0;

	if (type == "tab")
	{
		if (!(f >> line))
			return 0;
		if (diag != NULL)
			diag->add_with_probsum(line);
	}
	else
		if ((type == "col") && (diag != NULL))
		{
			if (!getline(f, str))
				return 0;
			istringstream is (str);
			while (is >> i)
				cols.push_back(i);
			diag->init_by_cols(cols);
		}
	return 1;
}

int Sequence::read_int(int* row)
{
	int i;
	if (type != "tab")
	{
		cout << "Sequence::read_strict(): incorrect type of sequence.\n";
		exit(1);
	}
	if (!(f >> i))
		return 0;

	*row = i;
	return 1;
}

int Sequence::read_strict(StrictDiagram* diag)
{
	di cols;
	string str;
	int i;

	if (io != "r")
		return 0;

	if (type == "tab")
	{
		if (!(f >> i))
			return 0;

		diag->add(i);
	}
	else
		if (!getline(f, str))
			return 0;
		if (type =="col")
		{
			istringstream is (str);
			while (is >> i)
				cols.push_back(i);
			diag->init_by_cols(cols);
		}
	return 1;
}

void Sequence::write_standard_maxprob(Diagram diag)
{
	double prob;

	diag.calc_max_prob(&line, &prob);
	f << diag.get_size() << " " << std::fixed << std::setprecision(PRECISION) << prob << endl;
}

void Sequence::write_standard_fullinfo(Diagram &diag, int row, bool greedy)
/*
 * Format of output file:
 * Standard greedy process:
 * 		n y x prob dimratio         TBD: c cdiff
 *
 * Standard planch process:
 * 		n y_added x_added prob_added dimratio_added y_maxprob x_maxprob maxprob dimratio_maxprob
*/
{
	uint64 n, col, maxrow, maxcol;
	double prob, dr, maxprob, maxdr;

	n = diag.get_size();

	f << std::fixed << std::setprecision(PRECISION) << n << " ";

	//Info about the added line
	if (row != -1)
	{
		col = diag.get_width(row);
		prob = diag.calc_point_plansh_prob(row);
		dr = prob * (n + 1);
		f << row << " " << col << " " << prob << " " << dr << " ";
	}

	//Info about the box corresponding to max.prob
	if (!greedy)
	{
		diag.calc_max_prob(&maxrow, &maxprob);
		maxcol = diag.get_width(maxrow);
		maxdr = maxprob * (n + 1);
		f << maxrow << " " << maxcol << " " << maxprob << " " << maxdr;
	}

	f << endl;

	if (row != -1)
		diag.add_with_probsum(row);
}

void Sequence::write_strict_fullinfo(StrictDiagram &diag, int l, bool greedy)
{
/*
 * Format of output file:
 * Strict greedy process:
 * 		n maxdr_line maxdr_col maxdr_prob maxdr maxprob_line maxprob_col maxprob maxprob_dr
 *
 * Strict plancherel process:
 * 		n added_line added_col added_prob added_dr maxprob_line maxprob_col maxprob maxprob_dr maxdr_line maxdr_col maxdr_prob maxdr
 *
 * Strict improved sequence:
 * 		n maxprob_line maxprob_col maxprob maxprob_dr maxdr_line maxdr_col maxdr_prob maxdr
 */
	uint64 col, maxprob_line, maxprob_col, maxdr_line, maxdr_col;
	double prob, dr, maxprob, maxprob_dr, maxdr_prob, maxdr;

	f << std::fixed << std::setprecision(PRECISION) << diag.get_size() << " ";

	//Info about the added line
	if (l != -1)
	{
		col = diag.get_width(l);
		prob = diag.calc_plansh_prob(l);
		dr = diag.calc_dim_ratio_fast(l);
		f << l << " " << col << " " << prob << " " << dr << " ";
	}

	//Info about the box corresponding to max.plancherel probability (strict!)
	diag.calc_max_prob(&maxprob_line, &maxprob);
	maxprob_col = diag.get_width(maxprob_line);
	maxprob_dr = diag.calc_dim_ratio_fast(maxprob_line);
	f << maxprob_line << " " << maxprob_col << " " << maxprob << " " << maxprob_dr << " ";

	//Info about the box corresponding to max.dim ratio
	if (!greedy)
	{
		 diag.calc_max_dimratio(&maxdr_line, &maxdr);
		 maxdr_col = diag.get_width(maxdr_line);
		 maxdr_prob = diag.calc_plansh_prob(maxdr_line);
		 f << maxdr_line << " " << maxdr_col << " " << maxdr_prob << " " << maxdr;
	}

	f << endl;

	if (l != -1)
		diag.add(l);
}

void Sequence::write_strict_maxprob(StrictDiagram diag)
{
	double prob;

	diag.calc_max_prob(&line, &prob);
	f << diag.get_size() << " " << line << " " << std::fixed << std::setprecision(PRECISION) << prob << endl;
}

void Sequence::write_strict_lineprob(StrictDiagram diag, uint64 line_)
{
	mi* p;


	////
	uint64 l;
	double prob;
	diag.calc_max_prob(&l, &prob);

	////

	p = diag.get_points();

	f << diag.get_size() << " " << l << " " << std::fixed << std::setprecision(PRECISION) << prob << " ";
	if (p->find(line_) != p->end())
		f << std::fixed << std::setprecision(PRECISION) << diag.calc_plansh_prob(line_);
	else
		f << 0;
	f << endl;
}

uint64 Sequence::get_line()
{
	return line;
}
