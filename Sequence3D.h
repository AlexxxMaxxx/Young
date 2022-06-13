/*
 * Sequence3D.h
 *
 *  Created on: 27 апр. 2016 г.
 *      Author: vduzhin
 */

#ifndef SEQUENCE3D_H_
#define SEQUENCE3D_H_

#include "Diagram3D.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

class Sequence3D
{
	private:
		std::string io, type, fname;
		fstream f;
		uint64 read_z, read_y;
	public:
		Sequence3D(const char* inputoutput, const char* t, const char* fn);
		~Sequence3D();
		int read_standard(Diagram3D* diag, bool add_cube = true);
		int read_xyz(Diagram3D* diag, bool add_cube = true);
		int read_5col_tab(Diagram3D* diag, bool add_cube = true);
		void write_standard(Diagram3D* diag);
		void write_planchsum(Diagram3D* diag);
		void write_fullinfo(Diagram3D* diag, uint64 z, uint64 y);

		static void print_planchsums(const char* fname_tab, const char* fname_out);

		uint64 get_z();
		uint64 get_y();
};

#endif /* SEQUENCE3D_H_ */
