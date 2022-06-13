/*
 * Sequence3D.cpp
 *
 *  Created on: 27 апр. 2016 г.
 *      Author: vduzhin
 */

#include "Sequence3D.h"

Sequence3D::Sequence3D(const char* inputoutput, const char* t, const char* fn)
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
	if (f.fail())
	{
		cout << "Sequence3D::Sequence3D(): could not open the file " << fname << endl;
		exit(1);
	}
}

Sequence3D::~Sequence3D()
{
	f.close();
}


int Sequence3D::read_standard(Diagram3D* diag, bool add_cube)
{
	vi one_layer;
	vvi cols;
	string str, str2;
	uint64 i, j;
	int first, next;

	if (io != "r")
		return 0;

	if (!getline(f, str))
		return 0;

	if (type == "tab")
	{
		istringstream is (str);
		is >> i; is >> j;
		read_z = i;
		read_y = j;

		//cout << read_z << " " << read_y << endl;

		if (add_cube)
			diag->add(pu(read_z, read_y));
	}
	else
		if (type == "col")
		{
			first = 0;
			next = str.find_first_of(',', first);
			if (next != -1)
				str2 = str.substr(first, next);
			else
				str2 = str.substr(first);
			istringstream is (str2);
			one_layer.clear();
			while (is >> i)
				one_layer.push_back(i);
			cols.push_back(one_layer);

			while (next != -1)
			{
				first = str.find_first_of(',', first+1);
				next = str.find_first_of(',', first+1);
				if (next != -1)
					str2 = str.substr(first+1, next-first-1);
				else
					str2 = str.substr(first+1);
				istringstream is (str2);
				one_layer.clear();
				while (is >> i)
					one_layer.push_back(i);
				cols.push_back(one_layer);
			}

			diag->init_by_cols(cols);
		}
	return 1;
}


int Sequence3D::read_xyz(Diagram3D* diag, bool add_cube)
{
	vi one_layer;
	vvi cols;
	string str, str2;

	if ((io != "r") || (!getline(f, str)) || (type != "tab"))
		return 0;

	istringstream is (str);
	is >> read_y; is >> read_y; is >> read_z;

	if (add_cube)
		diag->add(pu(read_z, read_y));

	return 1;
}

int Sequence3D::read_5col_tab(Diagram3D* diag, bool add_cube)
{
	vi one_layer;
	vvi cols;
	string str, str2;
	uint64 i, j;
	double d;

	if ((io != "r") || (type != "tab") || (!getline(f, str)))
		return 0;

	istringstream is (str);
	is >> i; is >> i; is >> j;
	read_z = j;
	read_y = i;
	is >> d; is >> d;

	//cout << read_z << " " << read_y << endl;

	if (add_cube)
		diag->add(pu(read_z, read_y));

	return 1;
}

void Sequence3D::write_standard(Diagram3D* diag)
//writes columns
{
	diag->print_cols(f);
}

void Sequence3D::write_planchsum(Diagram3D* diag)
{
	f << diag->get_size() << " " << std::fixed << std::setprecision(20) << diag->calc_planch_sum() << endl;
}

void Sequence3D::write_fullinfo(Diagram3D* diag, uint64 z, uint64 y)
//format:
//n x y z normdim normdim_diff*n^(1/3) prob outer_cubes_num corners_num diam.length
{
	double prob;
	uint64 n, x;
	map<tu, int> cubes;

	static double probsum = 0;
	//static double nsum = 0;

	static double prev_c = 0;
	static uint64 diam = 1;

	double c, cdiff;

	x = diag->getpointX(z, y);
	diag->calc_planch_probs();
	prob = diag->calc_planch_prob(z, y);
	n = diag->get_size();
	if ((x == y) && (y == z))
		++diam;
	diag->get_outer_cubes(cubes);

	probsum += log(1.0/(prob * pow(n, 2.0/3.0)));

	//nsum += log(n);
	//probsum -= (1.0/3.0) * nsum;

	c = probsum / (pow(n, 7.0/6.0) * log(n));
	//c = probsum / (log(n) * log(n));
	/////////////////cdiff = (c - prev_c) * pow(n, 1.0/3.0);
	cdiff = c - prev_c;

	f << n << " " << x << " " << y << " " << z << " " << std::fixed << std::setprecision(20) << c << " " << cdiff << " " << prob << cubes.size() << " " << diag->get_corners()->size() << " " << diam << endl;
	prev_c = c;
	//f << n << " " << std::fixed << std::setprecision(20) << probsum - (1.0/3.0) * nsum << " " << nsum << endl;

}

uint64 Sequence3D::get_z()
{
	return read_z;
}

uint64 Sequence3D::get_y()
{
	return read_y;
}

void Sequence3D::print_planchsums(const char* fname_tab, const char* fname_out)
{
	Diagram3D diag;
	Sequence3D seq_in("r", "tab", fname_tab);
	Sequence3D seq_out("w", "tab", fname_out);

	do
	{
		seq_out.write_planchsum(&diag);
	}
	while (seq_in.read_standard(&diag));
}
