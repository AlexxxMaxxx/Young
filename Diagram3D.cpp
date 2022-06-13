#include "Diagram3D.h"

Diagram3D::Diagram3D(bool standard)
{
	STANDARD = standard;
	init(standard);
}

int Diagram3D::operator==(const Diagram3D& d) const
{
	return (rows == d.rows);
}

void Diagram3D::init(bool standard)
{
	vi vec;

	STANDARD = standard;
	vec.push_back(1);

	rows.clear();
	cols.clear();
	bars.clear();
	points.clear();
	corners.clear();
	outer_cubes.clear();
	skew_xy.clear();
	skew_zy.clear();
	skew_zx.clear();

	rows.push_back(vec);
	cols.push_back(vec);
	bars.push_back(vec);

	//sk_rows.clear();
	//sk_rows.push_back(vec);
	//!!!!
	vec[0] = 0;
	//!!!!
	skew_xy.push_back(vec);
	skew_zy.push_back(vec);
	skew_zx.push_back(vec);

	if (STANDARD)
	{
		points[pu(0, 0)];
		points[pu(0, 1)];
	}
	points[pu(1, 0)];
	outer_cubes[{0, 0, 0}];
	n = 1;
	num_of_levels = 1;
}

void Diagram3D::init(Diagram3D* copy_from)
{
	rows = copy_from->rows;
	cols = copy_from->cols;
	bars = copy_from->bars;
	//sk_rows = copy_from->sk_rows;
	skew_xy = copy_from->skew_xy;
	skew_zy = copy_from->skew_zy;
	skew_zx = copy_from->skew_zx;
	points = copy_from->points;
	corners = copy_from->corners;
	n = copy_from->n;
	num_of_levels = copy_from->num_of_levels;
	outer_cubes = copy_from->outer_cubes;
	STANDARD = copy_from->STANDARD;
}

void Diagram3D::init_by_cols(vvi c)
{
	/*
	 * rows: (z,y) ==> x
	 * cols: (z,x) ==> y
	 * bars: (x,y) ==> z
	 */

	cols = c;
	calc_size_from_cols();
	calc_rows_from_cols();
	calc_bars_from_cols();

	outer_cubes.clear();
	calc_outers_points_corners(outer_cubes);
	//calc_outer_cubes_slow(outer_cubes);
	//calc_pointscorners();
}

void Diagram3D::init_by_rows(vvi r)
{
	/*
	 * rows: (z,y) ==> x
	 * cols: (z,x) ==> y
	 * bars: (x,y) ==> z
	 */

	rows = r;
	calc_cols_from_rows();
	calc_size_from_cols();
	calc_bars_from_cols();

	outer_cubes.clear();
	calc_outers_points_corners(outer_cubes);
	//calc_outer_cubes_slow(outer_cubes);
	//calc_pointscorners();
}

void Diagram3D::init_by_bars(vvi b)
{
	bars = b;

	calc_cols_from_bars();
	calc_size_from_cols();
	calc_rows_from_cols();

	outer_cubes.clear();
	calc_outers_points_corners(outer_cubes);
	//calc_outer_cubes_slow(outer_cubes);
	//calc_pointscorners();
}

void Diagram3D::calc_size_from_cols()
{
	uint64 x, z;

	n = 0;
	for (z = 0; z < cols.size(); ++z)
		for (x = 0; x < cols.at(z).size(); ++x)
			n += cols.at(z).at(x);
	num_of_levels = cols.size();
}

void Diagram3D::calc_rows_from_cols()
{
	uint64 x, y, z;

	rows.clear();
	rows.resize(cols.size());
	for (z = 0; z < cols.size(); ++z)
	{
		rows.at(z).resize(cols.at(z).at(0), 1);
		for (x = 1; x < cols.at(z).size(); ++x)
			for (y = 0; y < cols.at(z).at(x); ++y)
				rows.at(z).at(y)++;
	}
}

void Diagram3D::calc_cols_from_rows()
{
	uint64 x, y, z;
	cols.clear();
	cols.resize(rows.size());
	for (z = 0; z < rows.size(); ++z)
	{
		cols.at(z).resize(rows.at(z).at(0), 1);
		for (y = 1; y < rows.at(z).size(); ++y)
			for (x = 0; x < rows.at(z).at(y); ++x)
				cols.at(z).at(x)++;
	}
}

void Diagram3D::calc_bars_from_cols()
{
	uint64 x, y, z;

	bars.clear();
	bars.resize(cols.at(0).size());

	for (x = 0; x < cols.at(0).size(); ++x)
		bars.at(x).resize(cols.at(0).at(x), 1);

	for (z = 1; z < cols.size(); ++z)
		for (x = 0; x < cols.at(z).size(); ++x)
			for (y = 0; y < cols.at(z).at(x); ++y)
				bars.at(x).at(y)++;
}

void Diagram3D::calc_cols_from_bars()
{
	uint64 x, y, colz;

	cols.clear();
	cols.resize(bars.at(0).at(0));

	for (x = 0; x < bars.size(); ++x)
	{
		colz = 0;
		for (y = bars.at(x).size() - 1; y != 0; --y)
		{
			// x, y, z(colz)
			for (; colz + 1 <= bars.at(x).at(y); ++colz)
			{
				//cols.at(colz).at(x) = y + 1;
				//cout <<"A\n";
				cols.at(colz).push_back(y + 1);
				//cout << "B\n";
			}
		}
		for (; colz + 1 <= bars.at(x).at(y); ++colz)
		{
			//cols.at(colz).at(x) = y + 1;
			cols.at(colz).push_back(y + 1);
		}
	}

}

void Diagram3D::calc_pointscorners()
{
	///////////Uskorenie: 1. at ==> [];
	//////////2. men'she iteratorov (ciklov)

	uint64 prev_x, x, y, z;

	points.clear();
	corners.clear();

	if (STANDARD)
	{
		//bottom level:
		points[pu(0, 0)];

		if ((n > 1) && (getZ(rows.at(0).at(getY()-1) - 1, getY()-1) == 1))
			corners[pu(0, getY() - 1)];

		prev_x = rows.at(0).at(0);
		for (y = 1; y < rows.at(0).size(); ++y)
		{
			x = rows.at(0).at(y);
			if (x != prev_x)
			{
				points[pu(0, y)];

				if (getZ(prev_x - 1, y - 1) == 1)
					corners[pu(0, y - 1)];
			}
			prev_x = x;
		}
		points[pu(0, y)];

		//other levels
		for (z = 1; z < rows.size(); ++z)
		{
			//point in the 0 row
			if (rows.at(z - 1).at(0) > rows.at(z).at(0))
				points[pu(z, 0)];

			//corner in the uppermost row
			if (getZ(rows.at(z).at(getY(z)-1) - 1, getY(z)-1) == z + 1)
				corners[pu(z, getY(z) - 1)];

			prev_x = rows.at(z).at(0);
			for (y = 1; y < rows.at(z).size(); ++y)
			{
				x = rows.at(z).at(y);
				if (x != prev_x)
				{
					if (getZ(x, y) == z)
						points[pu(z, y)];
					if (getZ(prev_x - 1, y - 1) == z + 1)
						corners[pu(z, y - 1)];
				}
				prev_x = x;
			}

			//point in the uppermost row
			if (cols.at(z-1).at(0) > cols.at(z).at(0))
				points[pu(z, y)];
		}
		//point on the top of the diagram:
		points[pu(z, 0)];

		//CORNERS
		for (z = 0; z < rows.size(); ++z)
			for (y = 0; y < rows.at(z).size(); ++y)
			{
				x = rows.at(z).at(y) - 1;
				if ((cols.at(z).at(x) == y + 1) && (bars.at(x).at(y) == z + 1))
					corners[pu(z,y)];
			}
	}



//strict(old);
/*
		//1. points:
		//1.1 bottom level
			//bottom point
			if (cols.at(0).back() > 1)
				points[pu(0, 0)];
			//other points
			for (x = 1; x < cols.at(0).size(); ++x)
				if (cols.at(0).at(x)+1 != cols.at(0).at(x-1))
					points[pu(0, cols.at(0).at(x))];
			//upper point
			points[pu(0, getY())];

		//1.2 other levels
		for (z = 1; z < num_of_levels; ++z)
		{
			//bottom point
			if ((cols.at(z).back() > 1) && (rows.at(z-1).at(0) > rows.at(z).at(0)))
				points[pu(z, 0)];
			//other points
			for (x = 1; x < cols.at(z).size(); ++x)
				if ((cols.at(z).at(x)+1 != cols.at(z).at(x-1)) && (cols.at(z-1).at(x) > cols.at(z).at(x)))
					points[pu(z, cols.at(z).at(x))];
			//upper point
			points[pu(z, getY(z))];
		}

		//2. corners:
		if (n == 1)
			return;
		//2.1 upper level
		for (x = 0; x < cols.back().size()-1; ++x)
			if (cols.back().at(x) != cols.back().at(x+1)+1)
				corners[pu(cols.size()-1, cols.back().at(x)-1)];
		corners[pu(cols.size()-1, cols.back().back() - 1)];

		//2.2 other levels
		if (num_of_levels == 1)
			return;
		for (z = num_of_levels - 2; z > 0; --z)
		{
			for (x = 0; x < cols.at(z).size()-1; ++x)
			{
				y = cols.at(z).at(x) - 1;
				if ((y != cols.at(z).at(x+1)) && (bars.at(x).at(y) == z + 1))
					corners[pu(z, cols.at(z).at(x)-1)];
			}
			y = cols.at(z).back() - 1;
			if (bars.at(x).at(y) == z + 1)
				corners[pu(z, y)];
			if (z == 0)
				break;
		}
	}
*/

}

void Diagram3D::print_cols(ostream &output, bool print_endl)
{
	uint64 z, x;

	for (z = 0; z < cols.size(); ++z)
	{
		for (x = 0; x < cols.at(z).size(); ++x)
		{
			output << cols.at(z).at(x);
			if (x != cols.at(z).size() - 1)
				output << " ";
		}
		//output << cols.at(z).at(x);
		if (z != cols.size() - 1)
			output << ", ";
	}
	if (print_endl)
		output << endl;
}

void Diagram3D::print_cols_manylines(ostream &output, bool print_endl)
{
	uint64 z, x;

	for (z = 0; z < cols.size(); ++z)
	{
		for (x = 0; x < cols.at(z).size(); ++x)
		{
			output << cols.at(z).at(x);
			if (x != cols.at(z).size() - 1)
				output << " ";
		}
		if (z != cols.size() - 1)
			output << endl;
	}
	if (print_endl)
		output << endl;
}

void Diagram3D::print_rows(ostream &output, bool print_endl)
{
	uint64 z, y;

	for (z = 0; z < rows.size(); ++z)
	{
		for (y = 0; y < rows.at(z).size(); ++y)
		{
			output << rows.at(z).at(y);
			if (y != rows.at(z).size() - 1)
				output << " ";
		}
		if (z != rows.size() - 1)
			output << ", ";
	}
	if (print_endl)
		output << endl;
}

void Diagram3D::print_bars(ostream &output)
{
	uint64 x, y;

	for (x = 0; x < getX(); ++x)
	{
		for (y = 0; y < bars.at(x).size(); ++y)
		{
			output << bars.at(x).at(y);
			if (y != bars.at(x).size() - 1)
				output << " ";
		}
		if (x != getX() - 1)
			output << ", ";
	}
	output << endl;
}

void Diagram3D::print_points(ostream &output)
{
	const bool REVERSE = false;

	uint64 x, y, z;
	mpu::iterator it;

	if (REVERSE)
		output << "z y x prob\n";
	else
		output << "x y z prob\n";
	for (it = points.begin(); it != points.end(); ++it)
	{
		get_point_coords(it, x, y, z);
/*
		if (REVERSE)		
			output << z << " " << y << " " << x << " " << it->second << endl;
		else
			output << x << " " << y << " " << z << " " << it->second << endl;
*/
		output << x << " " << y << " " << z << " " << fixed << setprecision(20) << calc_planch_weight(z, y) << " " << it->second << endl;
	}
}

void Diagram3D::print_corners(ostream &output)
{
	uint64 x, y, z;
	mpu::iterator it;

	//output << "z y x prob\n";

	for (it = corners.begin(); it != corners.end(); ++it)
	{
		z = it->first.first; y = it->first.second;
		x = getX(z, y);
		output << z << " " << y << " " << x << endl;
	}
}

void Diagram3D::print_all()
{
	cout << "Rows: " << endl;
	for (vvi::iterator it = rows.begin(); it != rows.end(); ++it)
	{
		copy(it->begin(), it->end(), ostream_iterator<uint64>(cout, " "));
		cout << endl;
	}
	cout << endl << "Cols: " << endl;
	for (vvi::iterator it = cols.begin(); it != cols.end(); ++it)
	{
		copy(it->begin(), it->end(), ostream_iterator<uint64>(cout, " "));
		cout << endl;
	}
	cout << endl << "Bars:" << endl;
	for (vvi::iterator it = bars.begin(); it != bars.end(); ++it)
	{
		copy(it->begin(), it->end(), ostream_iterator<uint64>(cout, " "));
		cout << endl;
	}

	cout << endl << "Points:" << endl;
	for (mpu::iterator it = points.begin(); it != points.end(); ++it)
	{
		cout << it->first.first << " " << it->first.second << endl;
	}
	cout << endl << "Corners:" << endl;
	for (mpu::iterator it = corners.begin(); it != corners.end(); ++it)
	{
		cout << it->first.first << " " << it->first.second << endl;
	}
	cout << endl;
}

void Diagram3D::add_point(uint64 z, uint64 y, vvi* target_cols, uint64* zadd, uint64* yadd)
{//
	uint64 x;

	if (!STANDARD)
	{
		x = getpointX(z, y);
/*
		if (x != 0)
			if (cols.at(z).at(x - 1) == y + 1)
				return;
		if (y != 0)
			if (bars.at(x).at(y - 1) == z + 1)
				return;
*/
//z=0, y=2
		if (x != 0)
			if (getY(z, x - 1) == y)
			//if (cols.at(z).at(x - 1) == y + 1)
				return;
		if (y != 0)
			if (getZ(x, y - 1) == z + 1)
				return;
	}
	if (target_cols == NULL)
		points[pu(z, y)];
	else
	{
//cout << z << " " << y << " " << *zadd << " " << *yadd << endl;
		x = getpointX(z, y);
//cout << x << endl;
		if ((z == *zadd) && (y == *yadd))
			++x;
//cout << x << endl;

//cout << "AAA " << target_cols->at(z).at(x) << " " << y << endl;
		if (target_cols->at(z).size() != x)
			if (target_cols->at(z).at(x) != y)
				points[pu(z, y)];
	}

}

void Diagram3D::add_point2(uint64 z, uint64 y)
{
	uint64 x;

	if (!STANDARD)
	{
		x = getpointX(z, y);

		if (x != 0)
			if (getY(z, x - 1) == y)
				return;
		if (y != 0)
			if (getZ(x, y - 1) == z)
				return;
	}
	points[pu(z, y)];

}

void Diagram3D::add_corner(uint64 z, uint64 y)
{
	uint64 x;

	if (!STANDARD)
	{
		x = getX(z, y);
		if (x + 1 != getX(z))
			if (cols.at(z).at(x + 1) == y)
				return;

		if (y + 1 != cols.at(0).at(x))
			if (bars.at(x).at(y + 1) == z)
				return;
	}
	corners[pu(z, y)];
}

int Diagram3D::add(const pu place)
{
	return add(place.first, place.second);
}

int Diagram3D::add_old(const uint64 z, const uint64 y)
/*
Adds a new element to the diagram.
level - a layer where we add the cube
line - line in the layer where we add
*/
{

	uint64 x, x_, y_, z_;//, i1, i2;
	vi vec;

	x = getpointX(z, y);
	vec.push_back(1);

	/*
	//Check the correctness of level and line:
	if (level > rows.size())
	{
		cout << "Diagram3D::add(): Incorrect level!\n";
		return 1;
	}
	if (level != num_of_levels)
	{
		if (line > rows.at(level).size())
		{
			cout << "Diagram3D::add(): Incorrect line!\n";
			return 1;
		}
	}
	else
	{
		if (line != 0)
		{
			cout << "Diagram3D::add(): Incorrect line!\n";
			return 1;
		}
	}
	*/

	if (points.find(pu(z, y)) == points.end())
	{
		cout << "Diagram3D::add(): No point at z=" << z << ", y=" << y << endl;
		cout << "size=" << n << endl;
		print_cols();
		exit(1);
	}

	//the new added cube can be removed
	corners[pu(z, y)];
	//add_corner(level, line);


	if (!STANDARD)
	{
		if (z != 0)
		{
			if (x == 0)
				points[pu(getZ(x, y + 1), y + 1)];
			else
			{
				add_point2(getZ(x, y + 1), y + 1);

				//if (getZ(x - 1, y) != z + 1)
				//	points[pu(getZ(x, y + 1), y + 1)];
			}
		}
		else
		{
			points.erase(pu(0, y));
			points.erase(pu(0, y + 1));
		}

		if (y != 0)
		{
			//esli max.X
			//if (x + 1 == getX())
			//{
				//y - 1 ????? no control of y.....
				if (z == getZ(x + 1, y - 1))
				{
					if (y == 1)
						points[pu(z, y - 1)];
					else
					{
						if (getZ(x + 1, y - 2) > z + 1)
							points[pu(z, y - 1)];
					}
				}
			//}
			//else
			//	if (getZ(x + 1, y) != z + 1)
			//	bars.at(x + 1).size()
		}

	}

	if (z == num_of_levels) //adding a new level
	{
		points[pu(z + 1, 0)]; // we can put on the top
		//add_point(level + 1, 0);

		if (rows.at(z - 1).at(0) != 1) //if there is a cube at (z-1, x+1)
			//points[pu(level, 0)];
			add_point(z, 0);
		else
		{
			points.erase(pu(z, 0));
			corners.erase(pu(z - 1, 0));
		}
		//else
		//	points.erase(pu(level, 0));

		if (cols.at(z - 1).at(0) != 1) // if there is a cube at (z-1, y+1)
			//points[pu(level, 1)];
			add_point(z, 1);

		rows.push_back(vec);
		cols.push_back(vec);
		bars.at(0).at(0)++;

		num_of_levels++;
		n++;

		outer_cubes[{x, y, z}];

		if ((rows.at(z - 1).at(y) > 1) && (cols.at(z - 1).at(x) > 1))
			outer_cubes.erase({x, y, z - 1});
	}
	else
	{
		//Update points:
		if (STANDARD)
		{
			if (z == 0)
			{
				points[pu(0, y + 1)];
			}
			else
				if (rows.at(z - 1).size() > y + 1) //If the lower level has next horizontal line.
				{
					if (rows.at(z).size() <= y + 1) //If the current level has no cubes deeper than line.
						points[pu(z, y + 1)];
						//add_point(level, line + 1);
					else
						if (rows.at(z).at(y + 1) < rows.at(z - 1).at(y + 1)) //If the cubes exist, but the width of the line is smaller than the width of the line on the lower level
							points[pu(z, y + 1)];
							//add_point(level, line + 1);
				}
		}

		//erase points/corners
		if (y == rows.at(z).size()) //If we are adding a new line on existing level z
		{
			if (rows.at(z).at(getY(z) - 1) == 1)
			{
				points.erase(pu(z, y));
				corners.erase(pu(z, y - 1));
			}
		}
		else
		{
			if ((y > 0) && (rows.at(z).at(y - 1) == x + 1))
			{
				points.erase(pu(z, y));
				corners.erase(pu(z, y - 1));
			}
		}

		//If it is not the lowest level, than we check if we reached the permissible boundary.
		//If we reached, than remove point on level and corner on level-1.
		if (z != 0)
		{
			if (rows.at(z - 1).at(y) == x + 1)
			{
				points.erase(pu(z, y));
				corners.erase(pu(z - 1, y));
			}

		}

		//Adding the point to the upper level:
		if (rows.size() - 1 > z) //if this level(s) is higher than level
		{
			//Case 1: New cube is to the left,up, close to the cubes of upper levels.
			if (y == rows.at(z).size())
			{
				if (cols.at(z + 1).at(0) == y)
					//points[pu(level+1,line)];
					add_point(z + 1, y);
			}
			else
			//Case 2: New cube at right,down, near the cubes of upper levels.
			if (y == 0)
			{
				if (rows.at(z + 1).at(0) == x)
					//points[pu(level + 1, line)];
					add_point(z + 1, y);
			}
			else
			//Case 3: Cube is being added to the "hole" on the upper level
			{
				if ((rows.at(z + 1).size() > y) && (cols.at(z + 1).size()  > x))
				{
					if ((rows.at(z + 1).at(y) == x) && (cols.at(z + 1).at(x) == y))
						//points[pu(level + 1, line)];
						add_point(z + 1, y);
				}
			}
		}


		//Update rows:
		if (y == rows.at(z).size())
			rows.at(z).push_back(1);
		else
			rows.at(z).at(y)++;

		//Update cols:
		if (y == 0)
			cols.at(z).push_back(1);
		else
			cols.at(z).at(rows.at(z).at(y) - 1)++;

		//Update bars:
		if (z == 0)
		{
			if (x == bars.size())
				bars.push_back(vec);
			else
				bars.at(x).push_back(1);
		}
		else
			bars.at(x).at(y)++;
		n++;

		//Modify outer cubes
		outer_cubes[{x, y, z}];

		if (x != 0)
		{
			if ((cols.at(z).at(x - 1) > cols.at(z).at(x)) && (bars.at(x - 1).at(y) > bars.at(x).at(y)))
				outer_cubes.erase({x - 1, y, z});
		}
		if (y != 0)
		{
			if ((rows.at(z).at(y - 1) > rows.at(z).at(y)) && (bars.at(x).at(y - 1) > bars.at(x).at(y)))
				outer_cubes.erase({x, y - 1, z});
		}
		if (z != 0)
		{

			if ((rows.at(z - 1).at(y) > rows.at(z).at(y)) && (cols.at(z - 1).at(x) > cols.at(z).at(x)))
				outer_cubes.erase({x, y, z - 1});
		}
	}


	if (!STANDARD)
	{
		//let's remove some corners in a strict diagram

		//XY-plane (z-fixed)
		if ((y != 0) && (rows.at(z).at(y - 1) != rows.at(z).at(y)))
			corners.erase(pu(z, y - 1));
		if ((x != 0) && (cols.at(z).at(x - 1) == y + 2))
			corners.erase(pu(z, y + 1));

		//YZ-plane (x-fixed)

		if ((z != 0) && (cols.at(z - 1).at(x) != cols.at(z).at(x)))
			corners.erase(pu(z - 1, y + 1));

		if ((y != 0) && (bars.at(x).at(y - 1) == z + 2))
			corners.erase(pu(z + 1, y - 1));

/*
		//let's update skewed rows:
		if (z + 1 == num_of_levels)
			sk_rows.push_back(vec);
		else
		{
			//cout << "B\n";
			if (y + x == sk_rows.at(z).size())
			{
				//cout << "B1\n";
				sk_rows.at(z).push_back(1);
			}
			else
			{
				//cout << "B2\n";
				sk_rows.at(z).at(y + x)++;
			}
			//cout << "BBB\n";
		}
*/
		//let's update skewed ranges!
		x_ = x;
		y_ = y + x;
		z_ = z + y_;

		//XY
		if (skew_xy.size() > x_)
		{
			if (skew_xy[x_].size() > y_)
				skew_xy[x_][y_]++;
			else
			{
				skew_xy[x_].resize(y_+1 + 0,0);
				skew_xy[x_][y_] = z_;
			}
		}
		else
		{
			skew_xy.resize(x_+1 + 0);
			skew_xy[x_].resize(y_+1 + 0,0);
			skew_xy[x_][y_] = z_;
		}
		//ZY
		if (skew_zy.size() > z_)
		{
			if (skew_zy[z_].size() > y_)
				skew_zy[z_][y_]++;
			else
			{
				skew_zy[z_].resize(y_+1 + 0,0);
				skew_zy[z_][y_] = x_;
			}
		}
		else
		{
			skew_zy.resize(z_+1 + 0);
			skew_zy[z_].resize(y_+1 + 0,0);
			skew_zy[z_][y_] = x_;
		}
		//ZX
		if (skew_zx.size() > z_)
		{
			if (skew_zx[z_].size() > x_)
				skew_zx[z_][x_]++;
			else
			{
				skew_zx[z_].resize(x_+1 + 0,0);
				skew_zx[z_][x_] = y_;
			}
		}
		else
		{
			skew_zx.resize(z_+1 + 0);
			skew_zx[z_].resize(x_+1 + 0,0);
			skew_zx[z_][x_] = y_;
		}
	}

	return 0;
}

int Diagram3D::add(const uint64 z, const uint64 y)
{
	uint64 x, lenX, lenY, lenZ;

	x = getpointX(z, y);

	if (points.find(pu(z, y)) == points.end())
	{
		cout << "Diagram3D::add(): No point at z=" << z << ", y=" << y << endl;
		cout << "size=" << n << endl;
		print_cols();
		exit(1);
	}

	//I. MODIFY POINTS
	//check a neighbour #1 (x + 1)
	//oXZ-support && oXY-support
	lenY = getpointY(z, x + 1); 
	lenZ = getZ(x + 1, y);      

	if ((lenY == y) && (lenZ == z))
		points[pu(z, y)];
	else
		points.erase(pu(z, y));

	//check a neighbour #2 (y + 1)
	//oYZ-support && oXY-support
	lenX = getpointX(z, y + 1); //0
	lenZ = getZ(x, y + 1);      //0
	if ((lenX == x) && (lenZ == z))
		points[pu(z, y + 1)];

	//check a neighbour #3 (z + 1)
	//oXZ-support && oYZ-support
	lenY = getpointY(z + 1, x);
	lenX = getpointX(z + 1, y);
	if ((lenY == y) && (lenX == x))
		points[pu(z + 1, y)];

	//II. MODIFY CORNERS

	//the new added cube can be removed
	corners[pu(z, y)];
	if (getpointX(z, y - 1) == x + 1)
		corners.erase(pu(z, y - 1));
	if (getpointX(z - 1, y) == x + 1)
		corners.erase(pu(z - 1, y));

	//III. UPDATE rows/cols/bars
	if (z == num_of_levels)
	{
		num_of_levels++;
		rows.push_back({1});
		cols.push_back({1});
	}
	else
	{
		//Update rows:
		if (y == rows.at(z).size())
			rows.at(z).push_back(1);
		else
			rows.at(z).at(y)++;

		//Update cols:
		if (y == 0)
			cols.at(z).push_back(1);
		else
			cols.at(z).at(rows.at(z).at(y) - 1)++;
	}

	//Update bars:
	if (z == 0)
	{
		if (x == bars.size())
			bars.push_back({1});
		else
			bars.at(x).push_back(1);
	}
	else
		bars.at(x).at(y)++;
	n++;

	//Modify outer cubes
	outer_cubes[{x, y, z}];

	if (x != 0)
	{
		if ((cols.at(z).at(x - 1) > cols.at(z).at(x)) && (bars.at(x - 1).at(y) > bars.at(x).at(y)))
			outer_cubes.erase({x - 1, y, z});
	}
	if (y != 0)
	{
		if ((rows.at(z).at(y - 1) > rows.at(z).at(y)) && (bars.at(x).at(y - 1) > bars.at(x).at(y)))
			outer_cubes.erase({x, y - 1, z});
	}
	if (z != 0)
	{

		if ((rows.at(z - 1).at(y) > rows.at(z).at(y)) && (cols.at(z - 1).at(x) > cols.at(z).at(x)))
			outer_cubes.erase({x, y, z - 1});
	}

	return 0;
}


int Diagram3D::add_to_target(pu coord, vvi target)
/*
Adds a new element to the diagram at (z,y).
Do not get out of target & do not make points be out of target.
!!! Strict part was not implemented correctly !!!
*/
{
	uint64 x, y, z, x_, y_, z_;//, i1, i2;
	vi vec;

//cout << "lol...\n";

	z = coord.first; y = coord.second;

	x = getpointX(z, y);
	vec.push_back(1);

	if (points.find(pu(z, y)) == points.end())
	{
		cout << "Diagram3D::add_to_target(): No point at z=" << z << ", y=" << y << endl;
		cout << "size=" << n << endl;
		print_cols();
		exit(1);
	}

	//the new added cube can be removed
	corners[pu(z, y)];

	if (!STANDARD)
	{
		if (z != 0)
		{
			if (x == 0)
				points[pu(getZ(x, y + 1), y + 1)];
			else
			{
				add_point2(getZ(x, y + 1), y + 1);
			}
		}
		else
		{
			points.erase(pu(0, y));
			points.erase(pu(0, y + 1));
		}

		if (y != 0)
		{
			if (z == getZ(x + 1, y - 1))
			{
				if (y == 1)
					points[pu(z, y - 1)];
				else
				{
					if (getZ(x + 1, y - 2) > z + 1)
						points[pu(z, y - 1)];
				}
			}
		}

	}
//cout << "lol2\n";
	if (z == num_of_levels) //adding a new level
	{
		if (z + 1 != target.size())
		{
//			cout << "X adding new level. z+1 = " << z+1 << ", target.size() = " << target.size() << endl;
			points[pu(z + 1, 0)]; // we can put on the top
		}
		if (rows.at(z - 1).at(0) != 1) //if there is a cube at (z-1, x+1)
		{
//			cout << "addpoint1\n";
			add_point(z, 0, &target, &z, &y);
		}
		else
		{
			points.erase(pu(z, 0));
			corners.erase(pu(z - 1, 0));
		}

		if (cols.at(z - 1).at(0) != 1) // if there is a cube at (z-1, y+1)
		{
//			cout << "addpoint2\n";

//cout << "p1\n";
//print_points();

			add_point(z, 1, &target, &z, &y);
//cout << "p2\n";
//print_points();
//			print_points();
		}

		rows.push_back(vec);
		cols.push_back(vec);
		bars.at(0).at(0)++;

		num_of_levels++;
		n++;

		outer_cubes[{x, y, z}];

		if ((rows.at(z - 1).at(y) > 1) && (cols.at(z - 1).at(x) > 1))
			outer_cubes.erase({x, y, z - 1});
	}
	else
	{
		//Update points:
		if (STANDARD)
		{
//cout << "lol3\n";
			if (z == 0)
			{
//cout << "a1\n";
				if (y + 1 != target.at(0).at(x))	
					points[pu(0, y + 1)];
//cout << "b1\n";
			}
			else
				if (rows.at(z - 1).size() > y + 1) //If the lower level has next horizontal line.
				{
//cout << "a2\n";
//cout << z << endl;//1
//cout << rows.at(z).size() << endl;//1
//cout << target.at(z).at(x) << endl;
					if (rows.at(z).size() <= y + 1) //If the current level has no cubes deeper than line.
					{

						if (y + 1 != target.at(z).at(x))
						{
							//cout << "X1\n";
							points[pu(z, y + 1)];
						}
					}
					else
						if (rows.at(z).at(y + 1) < rows.at(z - 1).at(y + 1)) //If the cubes exist, but the width of the line is smaller than the width of the line on the lower level
							if (y + 1 != target.at(z).at(x))
							{
								//cout << "X2\n";
								points[pu(z, y + 1)];
							}

//cout << "mm\n";

				}
		}
//cout << "a3\n";
		//erase points/corners
		if (y == rows.at(z).size()) //If we are adding a new line on existing level z
		{
			if (rows.at(z).at(getY(z) - 1) == 1)
			{
				points.erase(pu(z, y));
				corners.erase(pu(z, y - 1));
			}
		}
		else
		{
			if ((y > 0) && (rows.at(z).at(y - 1) == x + 1))
			{
				points.erase(pu(z, y));
				corners.erase(pu(z, y - 1));
			}
		}
//cout << "a4\n";
		//If it is not the lowest level, than we check if we reached the permissible boundary.
		//If we reached, than remove point on level and corner on level-1.
		if (z != 0)
		{
			if (rows.at(z - 1).at(y) == x + 1)
			{
				points.erase(pu(z, y));
				corners.erase(pu(z - 1, y));
			}

		}
//cout << "a5\n";
		//Adding the point to the upper level:
		if (rows.size() - 1 > z) //if this level(s) is higher than level
		{
			//Case 1: New cube is to the left,up, close to the cubes of upper levels.
			if (y == rows.at(z).size())
			{
				if (cols.at(z + 1).at(0) == y)
				{
//					cout << "addpoint3\n";
					add_point(z + 1, y, &target, &z, &y);
				}
			}
			else
			//Case 2: New cube at right,down, near the cubes of upper levels.
			if (y == 0)
			{
				if (rows.at(z + 1).at(0) == x)
				{
//					cout << "addpoint4\n";
					add_point(z + 1, y, &target, &z, &y);
				}
			}
			else
			//Case 3: Cube is being added to the "hole" on the upper level
			{
				if ((rows.at(z + 1).size() > y) && (cols.at(z + 1).size()  > x))
				{
					if ((rows.at(z + 1).at(y) == x) && (cols.at(z + 1).at(x) == y))
					{
//						cout << "addpoint5\n";
						add_point(z + 1, y, &target, &z, &y);
					}
				}
			}
		}

//cout << "C\n";

/*
cout << "cols1\n";
print_cols();
cout << "points1:\n";
print_points();
cout << "add " << z << " " << y << endl;
*/
		//Update rows:
		if (y == rows.at(z).size())
			rows.at(z).push_back(1);
		else
			rows.at(z).at(y)++;

		//Update cols:
		if (y == 0)
			cols.at(z).push_back(1);
		else
			cols.at(z).at(rows.at(z).at(y) - 1)++;

		//Update bars:
		if (z == 0)
		{
			if (x == bars.size())
				bars.push_back(vec);
			else
				bars.at(x).push_back(1);
		}
		else
			bars.at(x).at(y)++;
		n++;



//cout << "cols2\n";
//print_cols();

/*
		cout << "A3\n";
		cout << z << " " << y << endl;
//z=0, y=1
//cols = 2.  cols.at(0).size = 1
		cout << cols.at(z).size() << " " << target.at(z).size() << endl;
*/
//		cout << "B3\n";

//cout << "points2:\n";
//print_points();
//cout << endl;
	}


	if (target.at(z).size() > x + 1)
	{
		if (target.at(z).at(x + 1) == getpointY(z, x + 1))
			points.erase(pu(z, y));
	}
	else
		points.erase(pu(z, y));

	//if ((getpointX(z, y + 1) == x) && (target.at(z).at(x) == cols.at(z).at(x)))
	//	points.erase(pu(z, y + 1));


	if (!STANDARD)
	{
		//let's remove some corners in a strict diagram

		//XY-plane (z-fixed)
		if ((y != 0) && (rows.at(z).at(y - 1) != rows.at(z).at(y)))
			corners.erase(pu(z, y - 1));
		if ((x != 0) && (cols.at(z).at(x - 1) == y + 2))
			corners.erase(pu(z, y + 1));

		//YZ-plane (x-fixed)

		if ((z != 0) && (cols.at(z - 1).at(x) != cols.at(z).at(x)))
			corners.erase(pu(z - 1, y + 1));

		if ((y != 0) && (bars.at(x).at(y - 1) == z + 2))
			corners.erase(pu(z + 1, y - 1));

		//let's update skewed ranges!
		x_ = x;
		y_ = y + x;
		z_ = z + y_;

		//XY
		if (skew_xy.size() > x_)
		{
			if (skew_xy[x_].size() > y_)
				skew_xy[x_][y_]++;
			else
			{
				skew_xy[x_].resize(y_+1 + 0,0);
				skew_xy[x_][y_] = z_;
			}
		}
		else
		{
			skew_xy.resize(x_+1 + 0);
			skew_xy[x_].resize(y_+1 + 0,0);
			skew_xy[x_][y_] = z_;
		}
		//ZY
		if (skew_zy.size() > z_)
		{
			if (skew_zy[z_].size() > y_)
				skew_zy[z_][y_]++;
			else
			{
				skew_zy[z_].resize(y_+1 + 0,0);
				skew_zy[z_][y_] = x_;
			}
		}
		else
		{
			skew_zy.resize(z_+1 + 0);
			skew_zy[z_].resize(y_+1 + 0,0);
			skew_zy[z_][y_] = x_;
		}
		//ZX
		if (skew_zx.size() > z_)
		{
			if (skew_zx[z_].size() > x_)
				skew_zx[z_][x_]++;
			else
			{
				skew_zx[z_].resize(x_+1 + 0,0);
				skew_zx[z_][x_] = y_;
			}
		}
		else
		{
			skew_zx.resize(z_+1 + 0);
			skew_zx[z_].resize(x_+1 + 0,0);
			skew_zx[z_][x_] = y_;
		}
	}

	return 0;
}

int Diagram3D::remove(const pu coord)
{
	return remove(coord.first, coord.second);
}

int Diagram3D::remove(const uint64 z, const uint64 y)
{

	uint64 x, x_, y_, z_, i1, i2;

	if (n == 1)
	{
		cout << "ERROR: Diagram::remove, could not remove, diagram size is equal to 1!";
		return 1;
	}

	if (corners.find(pu(z, y)) == corners.end())
	{
		cout << "Diagram3D::remove(): No corner at z=" << z << ", y=" << y << endl;
		cout << "size=" << n << endl;
		exit(1);
	}

	x = getX(z, y);

        //ADD POINT INSTEAD OF CORNER
	points[pu(z, y)];
        //DELETE CORNER
        corners.erase(pu(z, y));
        //ADDING NEW (NEIGHBOUR) CORNERS
	if (y > 0)
		if ((getX(z, y-1) == x) && (getZ(x, y-1) == z + 1))
			corners[pu(z, y - 1)];
        if (x > 0)
                if ((getY(z, x-1) == y) && (getZ(x-1, y) == z + 1))
                        corners[pu(z, y)];
        if (z > 0)
                if ((getX(z - 1, y) == x) && (getY(z - 1, x) == y))
                        corners[pu(z - 1, y)];
        //REMOVING (NEIGHBOUR) POINTS
        if (getpointX(z, y + 1) == x)
		points.erase(pu(z, y + 1));
        if (getpointX(z + 1, y) == x)
                points.erase(pu(z + 1, y));


	//Modify the structure
	if ((x == 0) && (y == 0))
	{
		rows.pop_back();
		cols.pop_back();
		--num_of_levels;
	}
	else
	{
		if (x == 0)
			rows.at(z).pop_back();
		else
			rows.at(z).at(y)--;

		if (y == 0)
			cols.at(z).pop_back();
		else
			cols.at(z).at(x)--;
	}
//cout << "B\n";
	if ((z == 0) && (y == 0))
		bars.pop_back();
	else
	{
		if (z == 0)
			bars.at(x).pop_back();
		else
			bars.at(x).at(y)--;
	}

	--n;

//cout << "C\n";

	//Modify outer cubes
	outer_cubes.erase({x, y, z});
	if (x != 0)
		outer_cubes[{x - 1, y, z}];
	if (y != 0)
		outer_cubes[{x, y - 1, z}];
	if (z != 0)
		outer_cubes[{x, y, z - 1}];

	if (!STANDARD)
	{
cout << "STRICT REMOVE IS NOT SUPPORTED YET\n";
/*
		//let's update skewed ranges!
		x_ = x;
		y_ = y + x;
		z_ = z + y_;

		//XY
		i1 = skew_xy.at(x_).at(y_).first;
		i2 = skew_xy.at(x_).at(y_).second;
		if (i1 == i2)
			skew_xy.at(x_).erase(y_);
		else
			skew_xy.at(x_).at(y_).second = i2 - 1;

		//ZY
		i1 = skew_zy.at(pu(z_, y_)).first;
		i2 = skew_zy.at(pu(z_, y_)).second;
		if (i1 == i2)
			skew_zy.erase(pu(z_, y_));
		else
			skew_zy.at(pu(z_, y_)).second = i2 - 1;

		//ZX
		i1 = skew_zx.at(pu(z_, x_)).first;
		i2 = skew_zx.at(pu(z_, x_)).second;
		if (i1 == i2)
			skew_zx.erase(pu(z_, x_));
		else
			skew_zx.at(pu(z_, x_)).second = i2 - 1;
*/
	}

/*
	print_cols();
	for (auto it = skew_xy.begin(); it != skew_xy.end(); ++it)
	{
		for (auto it2 = it->begin(); it2 != it->end(); ++it2)
		{
			cout << (*it2) << " ";
		}
		cout << endl;
	}

	cout << endl;
	for (auto it = skew_zy.begin(); it != skew_zy.end(); ++it)
	{
		for (auto it2 = it->begin(); it2 != it->end(); ++it2)
		{
			cout << (*it2) << " ";
		}
		cout << endl;
	}
	cout << endl;
	for (auto it = skew_zx.begin(); it != skew_zx.end(); ++it)
	{
		for (auto it2 = it->begin(); it2 != it->end(); ++it2)
		{
			cout << (*it2) << " ";
		}
		cout << endl;
	}
	cout << endl;
*/

	return 0;
}

void Diagram3D::rescale_probs()
{
	double sum;
	mpu::iterator it;

	sum = 0;
	for (it = points.begin(); it != points.end(); ++it)
		sum += it->second;
	for (it = points.begin(); it != points.end(); ++it)
		it->second /= sum;
}

void Diagram3D::remove_invalid_point(pu coord, vvi& target_rows)
/*
 *
 *
 * 		BAD function (if we need TRUE probability!)
 *
 */
{
	uint64 z, y;

	if (points.find(coord) == points.end())
		return;

	z = coord.first;
	y = coord.second;

	if (z + 1 > target_rows.size())
	{
		points.erase(coord);
	}
	else
		if (y + 1 > target_rows.at(z).size())
		{
			points.erase(coord);
		}
		else
			if (getpointX(z, y) + 1 > target_rows.at(z).at(y))
			{
				points.erase(coord);
			}
}

void Diagram3D::remove_invalid_points(vvi& target_rows)
{
	mpu p_copy;
	mpu::iterator it;

	p_copy = points;
	for (it = p_copy.begin(); it != p_copy.end(); ++it)
		remove_invalid_point(it->first, target_rows);
}

mpu* Diagram3D::get_points()
{
	return(&points);
}

mpu* Diagram3D::get_corners()
{
	return(&corners);
}

vvi* Diagram3D::get_rows()
{
	return(&rows);
}
vvi* Diagram3D::get_cols()
{
	return(&cols);
}
vvi* Diagram3D::get_bars()
{
	return(&bars);
}

/*
int Diagram3D::add_new_level()
{
	points[pu(num_of_levels, 0)];
	if (rows.at(num_of_levels - 1).at(0) != 1)
		points[pu(num_of_levels, 0)];
	if (cols.at(num_of_levels - 1).at(0) != 1)
		points[pu(num_of_levels, 1)];

	vi vec;
	vec.push_back(1);
	rows.push_back(vec);
	cols.push_back(vec);

	num_of_levels++;
	n++;
	return 0;
}
*/
void Diagram3D::print_corners(const char* fname)
{
	ofstream f;
	double x, y, z;
	mpu::iterator it;

	f.open(fname);

	for (it = corners.begin(); it != corners.end(); ++it)
	{
		z = it->first.first;
		y = it->first.second;
		x = getX(z, y);

		f << std::fixed << std::setprecision(5) << x << " " << y << " " << z << " 4 1\n";
	}

	f.close();
}

void Diagram3D::print_outer_cubes()
{
	double x, y, z;
	map<tu, int> cubes;
	map<tu, int>::iterator it;

	get_outer_cubes(cubes);

	for (it = cubes.begin(); it != cubes.end(); ++it)
	{
		//z = it->first.first;
		//y = it->first.second;
		//x = it->second;
		x = it->first.x;
		y = it->first.y;
		z = it->first.z;

		cout << std::fixed << std::setprecision(5) << x << " " << y << " " << z << " 0 1\n";
	}
}

void Diagram3D::print_outer_cubes(const char* fname)
{
	ofstream f;
	double x, y, z;
	map<tu, int> cubes;
	map<tu, int>::iterator it;

	get_outer_cubes(cubes);
	f.open(fname);
	for (it = cubes.begin(); it != cubes.end(); ++it)
	{
		//z = it->first.first;
		//y = it->first.second;
		//x = it->second;
		x = it->first.x;
		y = it->first.y;
		z = it->first.z;

		f << std::fixed << std::setprecision(5) << x << " " << y << " " << z << " 0 1\n";
	}

	f.close();
}

void Diagram3D::print_outer_cubes(fstream& f, double DX, double DY, double DZ, const bool SHOW_STRICT_DIAGS)
{
	double x, y, z;
	map<tu, int> cubes;
	map<tu, int>::iterator it;

	if (!f.is_open())
		return;

	get_outer_cubes(cubes);

	for (it = cubes.begin(); it != cubes.end(); ++it)
	{
		//z = it->first.first + DZ;
		//y = it->first.second + DY;
		//x = it->second + DX;
		x = it->first.x + DX;
		y = it->first.y + DY;
		z = it->first.z + DZ;

		//cout << z << " " << y << " " << x <<endl;

		f << std::fixed << std::setprecision(5) << x << " " << y << " " << z << " ";
		if (SHOW_STRICT_DIAGS)
		{
			if (has_strict_structure())
				f << "1 1\n";
			else
				f << "0 1\n";
		}
		else
			f << n - 1 << " 1\n";
	}
}

void Diagram3D::print_outer_cubes_and_new(const char* fname, pu new_cube, int new_color)
{
	ofstream f;
	uint64 x, y, z;
	map<tu, int> cubes;
	map<tu, int>::iterator it;

	get_outer_cubes(cubes);
	f.open(fname);

	for (it = cubes.begin(); it != cubes.end(); ++it)
	{
		//z = it->first.first;
		//y = it->first.second;
		//x = it->second;
		x = it->first.x;
		y = it->first.y;
		z = it->first.z;

		f << x << " " << y << " " << z;
		if ((z == new_cube.first) && (y == new_cube.second) && (x == rows.at(z).at(y) - 1))
			f << " " << new_color << " 1";
		else
			f << " 0 1";
		f << endl;
	}

	f.close();
}

void Diagram3D::get_central_points(map<tu, int> &cubes, tu &p1, tu &p2, tu &p3)
{
	int x, y, z, x1, y1, z1, x2, y2, z2, x3, y3, z3;

	x1 = 0; y1 = getY()/2; z1 = 0;
	x2 = getX()/3.1; y2 = 0; z2 = 0;
	x3 = getX()/5.3; y3 = 0; z3 = 0;
	for (auto it = cubes.begin(); it != cubes.end(); ++it)
	{
		x = it->first.x; y = it->first.y; z = it->first.z;
		if ((x == x1) && (y == y1))
			z1 = z;
		if ((x == x2) && (y == y2))
			z2 = z;
		if ((x == x3) && (z == z3))
			y3 = y;
	}
	if ((z1 == 0) || (z2 == 0) || (y3 == 0))
	{
		cout << "ERROR: get_central_points(): no special points found...\n";
		exit(1);
	}
	p1 = tu(x1, y1, z1);
	p2 = tu(x2, y2, z2);
	p3 = tu(x3, y3, z3);

	cout << "CENTRAL COORDS:\n";
	cout << x1 << " " << y1 << " " << z1 << endl;
	cout << x2 << " " << y2 << " " << z2 << endl;
	cout << x3 << " " << y3 << " " << z3 << endl;
}

void Diagram3D::print_outer_cubes_verbose(const char* fname)
{
	cfg CFG;
	CFG.fname_coords = (char*)fname;
	print_outer_cubes_verbose(CFG);
}

void Diagram3D::print_outer_cubes_verbose(cfg& CFG)
{
	const double EPSILON = 0.000001;

	ofstream f;
	uint64 x, y, z;
	mpu::iterator it;
	pu max_line;
	double max_prob, min_prob;
	uint64 min_dist, max_dist, dist;
	map<tu, int> cubes;
	map<tu, int>::iterator it2;
	tu diam_coord(0, 0, 0), p1(0, 0, 0), p2(0, 0, 0), p3(0, 0, 0);
	bool show_cubes;

	//int rand_color = 0;

	f.open(CFG.fname_coords);
	
	min_dist = 0; max_dist = 0; min_prob = 0; max_prob = 0; dist = 0;

	if (CFG.show_crossection)
		show_cubes = false;
	else show_cubes = CFG.show_cubes;

	if (CFG.skew)
		calc_outer_cubes_skew(cubes);
	else
		get_outer_cubes(cubes);

	//find the smallest and the biggest x+y+z:
	if (CFG.show_dist)
	{
		max_dist = 0;
		min_dist = n;
		for (it2 = cubes.begin(); it2 != cubes.end(); ++it2)
		{
			dist = it2->first.x + it2->first.y + it2->first.z;
			if (dist < min_dist)
				min_dist = dist;
			if (dist > max_dist)
				max_dist = dist;
		}
	}

	if (CFG.show_crossection)
	{
		for (dist = 0; consists_of(dist, dist, 0); ++dist);
		dist *= 2;
		dist-=2;
		//cout << "dist: " << dist << endl;

		//cout << "diam: " << get_diam(&diam_coord) << endl;
		//cout << "diam: " << diam_coord.x << " " << diam_coord.y << " " << diam_coord.z << endl;

		//get_central_points(cubes, p1, p2, p3);

	}

	for (it2 = cubes.begin(); it2 != cubes.end(); ++it2)
	{
		//z = it2->first.first;
		//y = it2->first.second;
		//x = it2->second;
		x = it2->first.x;
		y = it2->first.y;
		z = it2->first.z;
/*
		if (SKEW)
		{
			//x <= y <= z
			y += x;
			z += y;
		}
*/
		if (CFG.show_dist)
		{
			//if (is_corner(x, y, z))
			//{
				dist = x + y + z;
				//cout << (dist - min_dist)*(MAX_COLOR-MIN_COLOR)/(max_dist - min_dist)+MIN_COLOR << endl;
				f << x << " " << y << " " << z << " " << (dist - min_dist)*(CFG.max_color-CFG.min_color)/(max_dist - min_dist)+CFG.min_color << " 1\n";
			//}
			//else
			//	f << "2 1\n";
		}
		else
		{
			if (show_cubes)
			{
				f << x << " " << y << " " << z << " ";

				if (CFG.show_strict_parts)
				{
					/*
					if ((x == 0) && (z == 0) && (y == 3))
					{
						cout << "A\n";
						cout << (((z >= y) && (y >= x))) << endl;
						exit(1);
					}
					*/
					if ((x > y) && (y > z))
						f << 0;
					else
					if ((x > z) && (z > y))
						f << 1;
					else
					if ((y > x) && (x > z))
						f << 2;
					else
					if ((y > z) && (z > x))
						f << 3;
					else
					if ((z > x) && (x > y))
						f << 4;
					else
					if ((z > y) && (y > x))
						f << 5;
					else
						f << 6;
					f << " 1\n";
				}
				else
				{
					if (CFG.show_layers)
					{
						//f << (z+1)*1.0/getZ() << " 1\n";
						f << z % 12 << " 1\n";
					}
					else
					{
						if (!CFG.skew)
						{
							if (CFG.show_corners && is_corner(x, y, z))
								f << "4";
							else
								f << "2";
							//f << rand_0_to_n_1(6);
							
							f << " 1\n";
						}
						else
						{
							if (CFG.show_corners && is_corner(x, y - x, z - y))
								f << "4 1\n";
							else
								f << "2 1\n";
						}
					}
				}
			}
			//if ((SHOW_CROSSECTION) && ((x + y + z == dist)||is_contour(x, y, z)))
			if (CFG.show_crossection)
			//if ((SHOW_CROSSECTION) && (abs((int)x-(int)y) <= 1) && (abs((int)y-(int)z) <= 1) && (abs((int)z-(int)x) <= 1))
			{
				if ((x + y + z == dist)||is_contour(x, y, z))
					f << x << " " << y << " " << z << " 3 1\n";
				//if ((3*x + 2*y + z == 20)||is_contour(x, y, z))

				//cout << 6*x + 3*y + 2*z << endl;
				//if (6*x + 3*y + 2*z == 270)

				//cout << M_PI * x + (M_PI/2.0) * y + z << endl;
				//if (279*x + 135*y + 87*z == 13776)

				/*
				if ((x == diam_coord.x) && (y == diam_coord.y) && (z == diam_coord.z))
					f << x << " " << y << " " << z << " 4 1\n";
				else
				{
					//if (((is_contour(x,y,z)) && (x == 0) && (y == getY()/2)) || ((is_contour(x,y,z)) && (y == 0) && (x == getX()/2)) || ((is_contour(x,y,z)) && (z == 0) && (x == getX()/2)))
					//if (((x == 0) && (y == getY()/2) && (z == z1)) || ((x == getX()/2) && (y == 0) && (z == z2)) || ((x == getX()/2) && (y == y3) && (z == 0)))
					if ((it2->first == p1) || (it2->first == p2) || (it2->first == p3))
						f << x << " " << y << " " << z << " 5 1\n";
					else
						if (point_in_3D_surface(it2->first, p1, p2, p3, 0.2))
							f << x << " " << y << " " << z << " 4 1\n";
						else
							//if ((is_contour(x,y,z)) || (abs(M_PI * x + (M_PI/2.0) * y + z - 140) <= 0.5))
							if (is_contour(x, y, z))
								f << x << " " << y << " " << z << " 3 1\n";

				}
				*/
			}
		}
	}

	//print points
	if (CFG.show_points)
	{
		if (CFG.show_planch_probs)
		{
			calc_planch_probs(&max_line, &max_prob);
			min_prob = max_prob;
			for (it = points.begin(); it != points.end(); ++it)
				if (it->second < min_prob)
					min_prob = it->second;
		}
		for (it = points.begin(); it != points.end(); ++it)
		{
			z = it->first.first;
			y = it->first.second;
			if (z != rows.size())
			{
				if (y != rows.at(z).size())
					x = rows.at(z).at(y);
				else
					x = 0;
			}
			else
				x = 0;

			if (CFG.skew)
			{
				y += x;
				z += y;
			}

			f << x << " " << y << " " << z << " ";

			if ((fabs(min_prob - max_prob) < EPSILON) || !CFG.show_planch_probs)
				f << CFG.max_color << " ";
			else
				f << (it->second - min_prob)*(CFG.max_color-CFG.min_color)/(max_prob-min_prob)+CFG.min_color+EPSILON << " ";

			if (CFG.show_planch_probs && CFG.show_max_prob && (it->first == max_line))
				f << CFG.bigger_size << "\n";
			else
				f << CFG.small_size << "\n";

		}
	}
	f.close();

}

void Diagram3D::print_tab(cfg& CFG)
{
	const double EPSILON = 0.000001;

	ofstream f;
	//map<tu, int> cubes;
	uint64 x, y, z;
	mpu::iterator it;
	map<tu, int>::iterator it2;
	pu max_line;
	double max_prob, min_prob;
	bool Xlabel, Ylabel, Zlabel;
	string code;

	f.open(CFG.fname_coords);
	//get_outer_cubes(cubes);

	for (it2 = outer_cubes.begin(); it2 != outer_cubes.end(); ++it2)
	{
		x = it2->first.x;
		y = it2->first.y;
		z = it2->first.z;

		f << x << " " << y << " " << z << " ";
		f << CFG.tab->at(z).at(pu(x,y)).second;
		f << " 1\n";
		Xlabel = true; Ylabel = true; Zlabel = true;
		if (has_xyz(x + 1, y, z))
			Xlabel = false;
		if (has_xyz(x, y + 1, z))
			Ylabel = false;
		if (has_xyz(x, y, z + 1))
			Zlabel = false;
		if (Xlabel) code.push_back('2'); else code.push_back('1');
		if (Ylabel) code.push_back('2'); else code.push_back('1');
		if (Zlabel) code.push_back('2'); else code.push_back('1');

		CFG.tab->at(z).at(pu(x,y)).second = atoi(code.c_str());
		code.clear();
	}

	//print points
	if (CFG.show_points)
	{
		if (CFG.show_planch_probs)
		{
			calc_planch_probs(&max_line, &max_prob);
			min_prob = max_prob;
			for (it = points.begin(); it != points.end(); ++it)
				if (it->second < min_prob)
					min_prob = it->second;
		}
		for (it = points.begin(); it != points.end(); ++it)
		{
			z = it->first.first;
			y = it->first.second;
			if (z != rows.size())
			{
				if (y != rows.at(z).size())
					x = rows.at(z).at(y);
				else
					x = 0;
			}
			else
				x = 0;

			f << x << " " << y << " " << z << " ";

			if ((fabs(min_prob - max_prob) < EPSILON) || !CFG.show_planch_probs)
				f << CFG.max_color << " ";
			else
				f << (it->second - min_prob)*(CFG.max_color-CFG.min_color)/(max_prob-min_prob)+CFG.min_color+EPSILON << " ";

			if (CFG.show_planch_probs && CFG.show_max_prob && (it->first == max_line))
				f << CFG.bigger_size << "\n";
			else
				f << CFG.small_size << "\n";

		}
	}
	f.close();
}

void Diagram3D::get_point_coords(mpu::iterator iter, uint64& x, uint64& y, uint64& z)
/*
 *  Returns the 3D coordinates of a point (the potential cube which has not been added yet)
 */
{
	z = iter->first.first;
	y = iter->first.second;
	x = getpointX(z, y);
}


int Diagram3D::check_correctness()
/*
if (diag->check_correctness())
{
system("pause");
exit(EXIT_FAILURE);
}
*/
{
	uint64 height, width, next_width, depth, next_height;

	for (height = 0; height < rows.size(); ++height)
	{
		for (depth = 0; depth < rows.at(height).size(); ++depth)
		{
			width = rows.at(height).at(depth);
			if (depth != rows.at(height).size() - 1)
				next_width = rows.at(height).at(depth + 1);
			else next_width = width;
			
			next_height = width;
			if (height != rows.size() - 1)
				if (rows.at(height + 1).size() > depth)
					next_height = rows.at(height + 1).at(depth);
			
			if ((next_width > width) || (next_height > width))
			{
				cout << "Diagram3D::check_correctness(): height " << height << ", depth " << depth << endl;
				return 1;
			}
		}
	}
	//cout << "The diagram is correct." << endl;
	return 0;
}

uint64 Diagram3D::get_hook(uint64 x, uint64 y, uint64 z)
/*
����� ����� �������� (x,y)
*/
{ 
/*	
	cout << "x: " << x << ", y: " << y << ", z: " << z << endl;
	print_cols();
	cout << rows.at(z).at(y) << endl;
	cout << cols.at(z).at(x) << endl;
	cout << bars.at(x).at(y) << endl;
*/	
	return rows[z][y] + cols[z][x] + bars[x][y] - x - y - z - 2;
}

uint64 Diagram3D::get_skewed_hook(uint64 x, uint64 y, uint64 z)
{
	//return ( (cols.at(z).at(x) + x) - y ) + ( sk_rows.at(z).at(y) - x ) + ( (bars.at(x).at(y) + y) - z ) - 1;

/*
	if ((skew_xy.find(pu(x, y)) == skew_xy.end()) || (skew_zy.find(pu(z, y)) == skew_zy.end()) || (skew_zx.find(pu(z, x)) == skew_zx.end()))
	{
		cout << "Diagram3D::get_skewed_hook(): ERROR.\n";
		exit(1);
	}
	if ((skew_xy[pu(x, y)].first > z) || (skew_zy[pu(z, y)].first > x) || (skew_zx[pu(z, x)].first > y))
	{
		cout << "Diagram3D::get_skewed_hook(): ERROR.\n";
		exit(1);
	}
*/
//cout << x <<" " << y << " " << z << endl;
//cout << skew_xy.size() << " " << skew_zy.size() << " " <<skew_zx.size() << endl;
//cout << skew_xy.at(x).size() << " " << skew_zy.at(z).size() << " " <<skew_zx.at(z).size() << endl;

//cout << "rays (" << x <<"," << y << "," << z << ")\n";
//cout << skew_xy[x][y] << " " << skew_zy[z][y] << " " << skew_zx[z][x] << endl;
//exit(1);
	return (skew_xy[x][y] - z) + (skew_zy[z][y] - x) + (skew_zx[z][x] - y) + 1;
	//return (skew_xy.at(x).at(y) - z) + (skew_zy.at(z).at(y) - x) + (skew_zx.at(z).at(x) - y) + 1;
}

void Diagram3D::print_to_img(cfg& CFG)
{
	uint64 MAX;

	ofstream f(CFG.fname_plt);

	if (CFG.tab != NULL)
		print_tab(CFG);
	else
		if (CFG.new_color)
			print_outer_cubes_and_new(CFG.fname_coords, CFG.new_cube, CFG.new_cube_color);
		else
			if (CFG.corn_only)
				print_corners(CFG.fname_coords);
			else
				if (CFG.verbose)
					print_outer_cubes_verbose(CFG);
				else
					print_outer_cubes(CFG.fname_coords);
/*
	if (CFG.max_XYZ == 0)
	{
		MAX = rows.at(0).at(0);
		if (cols.at(0).at(0) > MAX)
			MAX = cols.at(0).at(0);
		if (bars.at(0).at(0) > MAX)
			MAX = bars.at(0).at(0);

		if (CFG.min_scale > MAX)
			MAX = CFG.min_scale;
	}
	else
		MAX = CFG.max_XYZ;
	MAX += 1;
*/
	//png:
	print_coords_to_img(CFG);

	//eps:
	//print_coords_to_img(COORDS, "tmpimg", MAX, SCALE, this);
	//if (system((string("epstool --copy --bbox --quiet tmpimg ") + fname_img).c_str()));
}

void Diagram3D::print_to_img(const char* fname_img, bool new_color, pu new_cube, uint64 MAX_XYZ)
{
	const uint64 MIN = 15; //minimum scale

	const bool VERBOSE = true;
	const bool CORNERS_ONLY = false;

	const double SCALE = 1.3;//*1.015483871;

	uint64 MAX;

	ofstream f("tmp_gnuplot.plt");

	const char* COORDS = "tmp_coords.txt";

	//cout << "N: " << n << endl;
	//print_bars();
	//print_points();

	if (new_color)
		print_outer_cubes_and_new(COORDS, new_cube, 2);
	else
		if (CORNERS_ONLY)
			print_corners(COORDS);
		else
			if (VERBOSE)
				print_outer_cubes_verbose(COORDS);
			else
				print_outer_cubes(COORDS);

	if (MAX_XYZ == 0)
	{
		MAX = rows.at(0).at(0);
		if (cols.at(0).at(0) > MAX)
			MAX = cols.at(0).at(0);
		if (bars.at(0).at(0) > MAX)
			MAX = bars.at(0).at(0);

		if (MIN > MAX)
			MAX = MIN;
	}
	else
		MAX = MAX_XYZ;
	MAX += 1;

	//png:
	print_coords_to_img(COORDS, fname_img, MAX, SCALE, this);

	//eps:
	//print_coords_to_img(COORDS, "tmpimg", MAX, SCALE, this);
	//if (system((string("epstool --copy --bbox --quiet tmpimg ") + fname_img).c_str()));
}

void Diagram3D::print_to_img2D(const char* FNAME_IMG)
/*
 * Prints a diagram as a projection on the bottom plane.
 * Each number in a cell is a height of a corresponding bar.
 */
{
	Diagram diag;
	vi c;
	di c2;
	mpp tab;
	uint64 x, y;

	c = cols.at(0);
	copy(c.begin(), c.end(), inserter(c2, c2.end()));
	diag.init_by_cols(c2);

	for (x = 0; x < getX(); ++x)
		for (y = 0; y < bars.at(x).size(); ++y)
		{
			//cout << "coords: " << x << " " << y << endl;
			//cout << bars.at(x).at(y) << endl;
			tab[pu(x, y)] = pu(bars.at(x).at(y), 2);
		}

	diag.print_cols();
	diag.print_to_png(FNAME_IMG, false, 1, pu(0,0), 0, &tab);
}

void Diagram3D::print_coords_to_img(cfg& CFG)
{
/*
	const char* COL0 = "#ffffff";//"#00a732";//"#ce4c7d";  //default
	const char* COL1 = "#ff0000";
	const char* COL2 = "#ff0000";//"#166301";//"#999999";
	const char* COL3 = "#ffffff";
	const char* COL4 = "#ffffff";
	const char* COL5 = "#00ff00";//"#cccccc";
*/
	//3D graph (colors)
	//const char* COL0 = "#ffffff";
	//const char* COL1 = "#ff0000";
	//const char* COL2 = "#006e0c";
	//const char* COL3 = "#6500ff";
	//const char* COL4 = "#ffff00";

	//3D graph (grayscale)
	//const char* COL0 = "#ffffff";
	//const char* COL1 = "#cbcbcb";
	//const char* COL2 = "#989898";
	//const char* COL3 = "#656565";
	//const char* COL4 = "#323232";


	uint64 corn, x, y, z, i;
	string code;

	ofstream f(CFG.fname_plt);

//CROP CAN ALSO BE PUT AS GNUPLOT COMMAND:
	if (CFG.type == "png")
		f << "set term pngcairo background rgb \"" << CFG.bgcol << "\" size 1600, 1200\n"; //crop
	else
		if (CFG.type == "eps")
			f << "set term postscript eps enhanced\n";
		else
		{
			cout << "Diagram3D::print_coords_to_img: Invalid image type!\n";
			exit(1);
		}
	//f << "size 1366, 768\nset nokey\n\n";
	f << "set nokey\n\n";
	f << "set cbrange [0:" << CFG.colors.size()-1 << "]\n";
	if ((CFG.max_XYZ != 0) && (!CFG.show_cubes))
	//if (CFG.max_XYZ != 0)
	{
		f << "set xrange [0:" << CFG.max_XYZ << "]\n";
		f << "set yrange [0:" << CFG.max_XYZ << "]\n";
		f << "set zrange [0:" << CFG.max_XYZ << "]\n";
	}
	//f << "set palette defined (0 '" << CFG.col0 << "', 1 '" << CFG.col1 << "', 2 '" << CFG.col2 << "', 3 '" << CFG.col3 << "', 4 '" << CFG.col4 << "', 5 '" << CFG.col5 << "', 6 '" << CFG.col6 << "')\nset style line 1 lc rgb '#000000' lt 1 lw " << CFG.lw << "\n";

	f << "set palette defined (";
	for (i = 0; i < CFG.colors.size() - 1; ++i)
		f << i << " '" << CFG.colors.at(i) << "', ";
	f << i << " '" << CFG.colors.at(i) << "')\nset style line 1 lc rgb '#000000' lt 1 lw " << CFG.lw << "\n";


	if (CFG.tab != NULL)
	{
		for (map<uint64,mpp>::iterator it = CFG.tab->begin(); it != CFG.tab->end(); ++it)
		{
			for (mpp::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				z = it->first;
				x = it2->first.first;
				y = it2->first.second;
				if (it2->second.second > 100)
				{
					code = to_string(it2->second.second);
					if (code[0] == '2')
						f << "set label \"" << it2->second.first << "\" at " << x+0.5 << "," << y << "," << z << " center font \"," << 120./CFG.max_XYZ <<"\" front\n";
					else
					if (code[1] == '2')
						f << "set label \"" << it2->second.first << "\" at " << x << "," << y+0.5 << "," << z << " center font \"," << 120./CFG.max_XYZ <<"\" front\n";
					else
					//if (code[2] == '2')
						f << "set label \"" << it2->second.first << "\" at " << x << "," << y << "," << z+0.5 << " center font \"," << 120./CFG.max_XYZ <<"\" front\n";

					it2->second.second = 2;
				}

			}	
		}
	}



	if (CFG.show_cubes)
		f << "set pm3d depthorder hidden3d\nset pm3d implicit\nunset hidden3d\n";
	else
		if (CFG.show_balls)
			f << "set pm3d\n";
	f << "set view equal xyz\nset view " << CFG.view1 << "," << CFG.view2 << "," << CFG.scale << "," << CFG.scale << "\nset origin 0,-0.15\n"; //69,130: cute  //54.73,135: orthogonal //65,128 - 3d graph
	if (CFG.diag2 != NULL)
	{
		if (CFG.show_size)
			f << "set label \"Size: " << CFG.diag2->get_size() << "\" at graph 0.1,1,0.7 font \"Arial,30\" front\n";
		if (CFG.show_cornnum)
		{
			corn = CFG.diag2->get_corners()->size();
			if (corn == 0)
				corn = 1;
			f << "set label \"Corners: " << corn << "\" at graph 0.1,1,0.5 font \"Arial,30\" textcolor rgb \"black\" front\n";
		}
		if (CFG.show_clustnum)
			f << "set label \"Clusters: " << CFG.diag2->calc_clusters_number() << "\" at graph 0.1,1,0.3 font \"Arial,10\" textcolor rgb \"black\" front\n";
	}

	//f << "set palette defined (0 '#ce4c7d', 1 '#ffff00')\nset style line 1 lc rgb '#b90046' lt 1 lw 0.5\nset view equal xyz\nset view 67,130\n";

	f << "unset key; unset tics; unset border; unset colorbox\n";
	f << "set output '" << CFG.fname_img << "'\n";

//cout << "A: ";
//cout << CFG.fname_img << endl;
//exit(1);

	//f << "set lmargin 0\nset rmargin 0\nset tmargin 0\nset bmargin 0\n";

	if (CFG.show_axes)
	{
		f << "set arrow 1 from 0,0,0 to " << 1.1*CFG.max_XYZ << ",0,0\n";
		f << "set arrow 2 from 0,0,0 to 0," << 1.1*CFG.max_XYZ << ",0\n";
		f << "set arrow 3 from 0,0,0 to 0,0," << 1.1*CFG.max_XYZ << "\n";

		f << "set arrow 4 nohead from 0,50,550 to -150,50,550\n";
		f << "set arrow 5 nohead from 50,0,386 to 50,-150,386\n";
		f << "set label 6 \"1\" at -160,50,550 font 'Arial,20'\n";
		f << "set label 7 \"2\" at 75,-170,400 font 'Arial,20'\n";
	}

	if (CFG.show_cubes)
	{
		f << "cube(x,y,z,c,s) = sprintf('<echo \"%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\"',0+x,0+y,0+z,c,0+x,0+y,s+z,c,0+x,s+y,s+z,c,0+x,s+y,0+z,c,0+x,0+y,0+z,c,s+x,0+y,0+z,c,s+x,0+y,s+z,c,s+x,s+y,s+z,c,s+x,s+y,0+z,c,s+x,0+y,0+z,c,0+x,0+y,0+z,c,s+x,0+y,0+z,c,s+x,s+y,0+z,c,0+x,s+y,0+z,c,0+x,0+y,0+z,c,0+x,0+y,s+z,c,s+x,0+y,s+z,c,s+x,s+y,s+z,c,0+x,s+y,s+z,c,0+x,0+y,s+z,c)\n";


//		f << "add_cube(x,y,z,c,s) = sprintf('cube(%f,%f,%f,%f,%f) w l ls 1,',x,y,z,c,s)\nCMD = ''\nstats '" << CFG.fname_coords << "' u 1:($4 == 0?CMD = CMD.add_cube($1,$2,$3,$4,$5):CMD = CMD.add_cube($1+(1-$5)/1.5,$2+(1-$5)/1.5,$3+(1-$5)/1.5,$4,$5)) nooutput\nCMD = 'splot '.CMD.'1/0 w l ls 1'\n";
		f << "add_cube(x,y,z,c,s) = sprintf('cube(%f,%f,%f,%f,%f) w l ls 1,',x,y,z,c,s)\nCMD = ''\nstats '" << CFG.fname_coords << "' u 1:($4 == 0?CMD = CMD.add_cube($1,$2,$3,$4,$5):CMD = CMD.add_cube($1+(1-$5)/1.5,$2+(1-$5)/1.5,$3+(1-$5)/1.5,$4,$5)) nooutput\nCMD = 'splot '.CMD";
if (CFG.add_fake_point)
	f << ".'\"<echo ''" <<  CFG.max_XYZ+0.05 << " 0 0\\n0 " <<  CFG.max_XYZ+0.05 << " 0\\n0 0 " <<  CFG.max_XYZ+0.05 << "''\" w d ls 1'\n";
else
	f << endl;

/*
		if (CFG.max_XYZ != 0)
		{
			f << "set xrange [0:" << CFG.max_XYZ << "]\n";
			f << "set yrange [0:" << CFG.max_XYZ << "]\n";
			f << "set zrange [0:" << CFG.max_XYZ << "]\n";
		}
*/

		if (CFG.show_arrows)
		{
			f << "ARROWS = \"'" << CFG.fname_arrows << "' u 1:2:3:($4-$1):($5-$2):($6-$3) w vec ls 1 not\"\n";
			f << "CMD = CMD.', '.ARROWS\n";
		}
		if (CFG.show_labels)
		{
			f << "LABELS = \"'" << CFG.fname_labels << "' u 1:2:3:4 w labels offset 0 font \'Arial,8\' \"\n";
			f << "CMD = CMD.', '.LABELS\n";
		}

		//ADD FAKE POINTS
		//f << "FAKES = '\"<echo ''" << CFG.max_XYZ+0.05 << " 0 0''\" w d lc rgb \"#ff000000\", \"<echo ''0 " << CFG.max_XYZ+0.05 << " 0''\" w d lc rgb \"#ff000000\", \"<echo ''0 0 " << CFG.max_XYZ+0.05 << "''\" w d lc rgb \"#ff000000\"'\n";
		//f << "CMD = CMD.', '.FAKES\n";
		
		f << "eval(CMD);\n";
	}
	else
	{
		if (CFG.show_balls)
		{
			f << "CMD = \"spl '" << CFG.fname_balls << "' u 1:2:3 w p pt 7 ps 0.7 lc 1\"\n";
			if (CFG.show_arrows)
			{
				f << "ARROWS = \"'" << CFG.fname_arrows << "' u 1:2:3:($4-$1):($5-$2):($6-$3) w vec ";
				if (CFG.nohead)
					f << "nohead ";
				f << "lc -1 not\"\n";
				f << "CMD = CMD.', '.ARROWS\n";
			}
			if (CFG.show_labels)
			{
				f << "LABELS = \"'" << CFG.fname_labels << "' u 1:2:3:4 w labels offset 0 font \'Arial,8\' \"\n";
				f << "CMD = CMD.', '.LABELS\n";
			}
			f << "eval(CMD);\n";
		}
		else
			f << "spl '" << CFG.fname_coords << "' u 1:2:3:4 w d palette\n";
	}
	f << "unset output\n";
	f.close();

	if (system((string("gnuplot-x11 ") + CFG.fname_plt).c_str()));

        if (CFG.crop)
	{
		if (CFG.type == "png")
		{
        		if (system((string("convert ") + CFG.fname_img + string(" -trim ") + CFG.fname_img).c_str()));
		}
		else
		{
			if (CFG.type == "eps")
				if (system((string("./fixbb ") + CFG.fname_img).c_str() ));
		}
	}
}

void Diagram3D::print_coords_to_img(const char* fname_coords, const char* fname_img, double max, double scale, Diagram3D* diag)
{
	const string TYPE = "png";

	const char* ARROWS = "tmp_arrows.txt";
	const char* BALLS = "tmp_balls.txt";
	const char* LABELS = "tmp_labels.txt";

//	const bool RAND_COLOR = true;

	const bool SHOW_CUBES = true;
	const bool SHOW_ARROWS = false;
	const bool NOHEAD = true;
	const bool SHOW_BALLS = false;
	const bool SHOW_SIZE = false;
	const bool SHOW_CORNNUM = false;
	const bool SHOW_CLUSTNUM = false;
	const bool SHOW_AXES = false;
	const bool SHOW_LABELS = false;

	const double LW = 1; //line width

	//const char* BGROUND = "#e6efe6";
	const char* BGROUND = "#ffffff";

	const char* COL0 = "#00ffff";
	const char* COL1 = "#ff0000";
	const char* COL2 = "#ffffff";//"#006e0c";
	const char* COL3 = "#6500ff";
	const char* COL4 = "#ffff00";
	const char* COL5 = "#ce4c7d";


/*
	const char* COL0 = "#ffffff";//"#00a732";//"#ce4c7d";  //default
	const char* COL1 = "#ff0000";
	const char* COL2 = "#ff0000";//"#166301";//"#999999";
	const char* COL3 = "#ffffff";
	const char* COL4 = "#ffffff";
	const char* COL5 = "#00ff00";//"#cccccc";
*/
	//3D graph (colors)
	//const char* COL0 = "#ffffff";
	//const char* COL1 = "#ff0000";
	//const char* COL2 = "#006e0c";
	//const char* COL3 = "#6500ff";
	//const char* COL4 = "#ffff00";
	//const char* COL5 = "#6e0c00";
	//
	//3D graph (grayscale)
	//const char* COL0 = "#ffffff";
	//const char* COL1 = "#cbcbcb";
	//const char* COL2 = "#989898";
	//const char* COL3 = "#656565";
	//const char* COL4 = "#323232";
	//
	//CHRISTMAS TREE
	//const char* COL0 = "#006e0c";
	//const char* COL0 = "#e04040";
	//const char* COL1 = "#7fff8e";
	//const char* COL2 = "#0000ff";
	//const char* COL3 = "#ffff00";
	//const char* COL4 = "#006e0c";
	//const char* COL5 = "#ff00ff";

	uint64 corn;

	ofstream f("tmp_gnuplot.plt");

	if (TYPE == "png")
		f << "set term pngcairo background rgb \"" << BGROUND << "\" size 1600, 1200 crop\n";
	else
		if (TYPE == "eps")
			f << "set term postscript eps enhanced\n";
		else
		{
			cout << "Diagram3D::print_coords_to_img: Invalid image type!\n";
			exit(1);
		}
	//f << "size 1366, 768\nset nokey\n\n";
	f << "set nokey\n\n";
	f << "set cbrange [0:5]\n";
	if ((max != 0) && (!SHOW_CUBES))
	{
		f << "set xrange [0:" << max << "]\n";
		f << "set yrange [0:" << max << "]\n";
		f << "set zrange [0:" << max << "]\n";
	}
	f << "set palette defined (0 '" << COL0 << "', 1 '" << COL1 << "', 2 '" << COL2 << "', 3 '" << COL3 << "', 4 '" << COL4 << "', 5 '" << COL5 << "')\nset style line 1 lc rgb '#000000' lt 1 lw " << LW << "\n";
	if (SHOW_CUBES)
		f << "set pm3d depthorder hidden3d\nset pm3d implicit\nunset hidden3d\n";
	else
		if (SHOW_BALLS)
			f << "set pm3d\n";
	f << "set view equal xyz\nset view 54.73,135," << scale << "," << scale << "\nset origin 0,-0.15\n"; //69,130: cute  //54.73,135: orthogonal //65,128 - 3d graph
	if (diag != NULL)
	{
		if (SHOW_SIZE)
			f << "set label \"Size: " << diag->get_size() << "\" at graph 0.1,1,0.7 font \"Arial,30\" front\n";
		if (SHOW_CORNNUM)
		{
			corn = diag->get_corners()->size();
			if (corn == 0)
				corn = 1;
			f << "set label \"Corners: " << corn << "\" at graph 0.1,1,0.5 font \"Arial,30\" textcolor rgb \"black\" front\n";
		}
		if (SHOW_CLUSTNUM)
			f << "set label \"Clusters: " << diag->calc_clusters_number() << "\" at graph 0.1,1,0.3 font \"Arial,10\" textcolor rgb \"black\" front\n";
	}

	//f << "set palette defined (0 '#ce4c7d', 1 '#ffff00')\nset style line 1 lc rgb '#b90046' lt 1 lw 0.5\nset view equal xyz\nset view 67,130\n";

	f << "unset key; unset tics; unset border; unset colorbox\n";
	f << "set output '" << fname_img << "'\n";
	//f << "set lmargin 0\nset rmargin 0\nset tmargin 0\nset bmargin 0\n";

	if (SHOW_AXES)
	{
		f << "set arrow 1 from 0,0,0 to " << 1.1*max << ",0,0\n";
		f << "set arrow 2 from 0,0,0 to 0," << 1.1*max << ",0\n";
		f << "set arrow 3 from 0,0,0 to 0,0," << 1.1*max << "\n";

		f << "set arrow 4 nohead from 0,50,550 to -150,50,550\n";
		f << "set arrow 5 nohead from 50,0,386 to 50,-150,386\n";
		f << "set label 6 \"1\" at -160,50,550 font 'Arial,20'\n";
		f << "set label 7 \"2\" at 75,-170,400 font 'Arial,20'\n";
	}

	if (SHOW_CUBES)
	{
		f << "cube(x,y,z,c,s) = sprintf('<echo \"%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\\n%f %f %f %f\"',0+x,0+y,0+z,c,0+x,0+y,s+z,c,0+x,s+y,s+z,c,0+x,s+y,0+z,c,0+x,0+y,0+z,c,s+x,0+y,0+z,c,s+x,0+y,s+z,c,s+x,s+y,s+z,c,s+x,s+y,0+z,c,s+x,0+y,0+z,c,0+x,0+y,0+z,c,s+x,0+y,0+z,c,s+x,s+y,0+z,c,0+x,s+y,0+z,c,0+x,0+y,0+z,c,0+x,0+y,s+z,c,s+x,0+y,s+z,c,s+x,s+y,s+z,c,0+x,s+y,s+z,c,0+x,0+y,s+z,c)\n";

//		if (!RAND_COLOR)		
			f << "add_cube(x,y,z,c,s) = sprintf('cube(%f,%f,%f,%f,%f) w l ls 1,',x,y,z,c,s)\nCMD = ''\nstats '" << fname_coords << "' u 1:($4 == 0?CMD = CMD.add_cube($1,$2,$3,$4,$5):CMD = CMD.add_cube($1+(1-$5)/1.5,$2+(1-$5)/1.5,$3+(1-$5)/1.5,$4,$5)) nooutput\nCMD = 'splot '.CMD.'1/0 w l ls 1'\n";
//		else
//floor(rand(0)*5)
//			f << "add_cube(x,y,z,c,s) = sprintf('cube(%f,%f,%f,%f,%f) w l ls 1,',x,y,z,c,s)\nCMD = ''\nstats '" << fname_coords << "' u 1:($4 == 0?CMD = CMD.add_cube($1,$2,$3,rand(0)*3,$5):CMD = CMD.add_cube($1+(1-$5)/1.5,$2+(1-$5)/1.5,$3+(1-$5)/1.5,rand(0)*3,$5)) nooutput\nCMD = 'splot '.CMD.'1/0 w l ls 1'\n";
		if (max != 0)
		{
			f << "set xrange [0:" << max << "]\n";
			f << "set yrange [0:" << max << "]\n";
			f << "set zrange [0:" << max << "]\n";
		}
		if (SHOW_ARROWS)
		{
			f << "ARROWS = \"'" << ARROWS << "' u 1:2:3:($4-$1):($5-$2):($6-$3) w vec lc -1 lw 0.5 not\"\n";
			f << "CMD = CMD.', '.ARROWS\n";
		}
		if (SHOW_LABELS)
		{
			f << "LABELS = \"'" << LABELS << "' u 1:2:3:4 w labels offset 0 font \'Arial,8\' \"\n";
			f << "CMD = CMD.', '.LABELS\n";
		}
		f << "eval(CMD);\n";
	}
	else
	{
		if (SHOW_BALLS)
		{
			f << "CMD = \"spl '" << BALLS << "' u 1:2:3 w p pt 7 ps 0.7 lc 1\"\n";
			if (SHOW_ARROWS)
			{
				f << "ARROWS = \"'" << ARROWS << "' u 1:2:3:($4-$1):($5-$2):($6-$3) w vec ";
				if (NOHEAD)
					f << "nohead ";
				f << "lc -1 not\"\n";
				f << "CMD = CMD.', '.ARROWS\n";
			}
			if (SHOW_LABELS)
			{
				f << "LABELS = \"'" << LABELS << "' u 1:2:3:4 w labels offset 0 font \'Arial,8\' \"\n";
				f << "CMD = CMD.', '.LABELS\n";
			}
			f << "eval(CMD);\n";
		}
		else
			f << "spl '" << fname_coords << "' u 1:2:3:4 w d palette\n";
	}
	f << "unset output\n";
	f.close();

	if (system("gnuplot-x11 tmp_gnuplot.plt"));

	if (system((string("convert ") + fname_img + string(" -trim ") + fname_img).c_str()));

	//CHRISTMAS TREE:
	//if (system((string("convert -pointsize 200 -fill \"#ff00ff\" -draw 'text 130,300 \"20     18\"' -font /usr/share/fonts/truetype/tlwg/Norasi.ttf ") + fname_img + " " + fname_img).c_str()));

}

void Diagram3D::print_verbose_to_img(const char* fname_img, uint64 maxX, uint64 maxY, uint64 maxZ, double scale)
{
	ofstream f("tmp_gnuplot.plt");

	const char* COORDS = "tmp_coords.txt";

	uint64 max_coord;

	print_outer_cubes_verbose(COORDS);

	if (maxX == 0)
		maxX = rows.at(0).at(0) + 1;
	if (maxY == 0)
		maxY = cols.at(0).at(0) + 1;
	if (maxZ == 0)
		maxZ = bars.at(0).at(0) + 1;

	max_coord = max(maxX, max(maxY, maxZ));
	print_coords_to_img(COORDS, fname_img, max_coord);
}

bool Diagram3D::consists_of(uint64 x, uint64 y, uint64 z)
//same as has_xyz?
{
	if (z + 1 > rows.size())
		return false;
	if (y + 1 > rows.at(z).size())
		return false;
	if (x + 1 > rows.at(z).at(y))
		return false;
	return true;

}

bool Diagram3D::is_corner(uint64 x, uint64 y, uint64 z)
{
	if (!consists_of(x, y, z))
		return false;

	if ((corners.find(pu(z, y)) != corners.end()) && (x == rows.at(z).at(y) - 1))
		return true;
	return false;

/*
	bool x_outer, y_outer, z_outer;

	if (!consists_of(x, y, z))
		return false;

	x_outer = (x == rows.at(z).at(y) - 1);
	y_outer = (y == cols.at(z).at(x) - 1);
	z_outer = (z == bars.at(x).at(y) - 1);

	if (x_outer && y_outer && z_outer)
		return true;
	return false;
*/
}

bool Diagram3D::is_contour(uint64 x, uint64 y, uint64 z)
{
	bool x_outer, y_outer, z_outer;

	x_outer = (x == rows.at(z).at(y) - 1);
	y_outer = (y == cols.at(z).at(x) - 1);
	z_outer = (z == bars.at(x).at(y) - 1);

	if (((x == 0) && (y_outer || z_outer)) || ((y == 0) && (x_outer || z_outer)) || ((z == 0) && (x_outer || y_outer)))
		return true;
	return false;
}

bool Diagram3D::is_equal_to(Diagram3D* comp)
{
	if (rows != comp->rows)
		return 0;
	return 1;
}

bool  Diagram3D::is_equal(Diagram3D comp) {
	if (rows != comp.rows)
		return 0;
	return 1;
}

bool Diagram3D::is_symmetric()
{
	return (rows == cols) && (cols == bars);
}

bool Diagram3D::is_symmetric_to(Diagram3D& diag)
{
	Diagram3D diag2;
	if ((rows == *diag.get_rows()) || (rows == *diag.get_cols()) || (rows == *diag.get_bars()) || (cols == *diag.get_rows()) || (cols == *diag.get_bars()) || (bars == *diag.get_rows()) || (bars == *diag.get_cols()))
	return true;
	
	diag2.init_by_cols(rows);
	return ((*diag2.get_rows() == *diag.get_rows()) || (*diag2.get_rows() == *diag.get_cols()) || (*diag2.get_rows() == *diag.get_bars()) || (*diag2.get_cols() == *diag.get_rows()) || (*diag2.get_cols() == *diag.get_bars()) || (*diag2.get_bars() == *diag.get_rows()) || (*diag2.get_bars() == *diag.get_cols()));
}

bool Diagram3D::is_triangle()
{
//NOT VERIFIED
	uint64 H, x, z;

	H = cols.at(0).at(0);
	for (x = 1; x < getX(); ++x)
		if (cols.at(0).at(x) + x != H)
			return false;
	for (z = 1; z < getZ(); ++z)
	{
		if (cols.at(z).at(0) + 1 != H)
			return false;
		H = cols.at(z).at(0);
		for (x = 0; x < getX(z); ++x)
			if (cols.at(z).at(x) + x != H)
				return false;
	}

	return true;
}

bool Diagram3D::has_strict_structure()
{
	uint64 x, y, z;

	for (z = 0; z < cols.size(); ++z)
		for (x = 1; x < cols.at(z).size(); ++x)
			if (cols.at(z).at(x) == cols.at(z).at(x - 1))
				return false;
	for (x = 0; x < bars.size(); ++x)
		for (y = 1; y < bars.at(x).size(); ++y)
			if (bars.at(x).at(y) == bars.at(x).at(y - 1))
				return false;
	return true;
}

bool Diagram3D::is_strict()
{
	uint64 x, y, z;
	mpu::iterator it;

//cout << "cols:\n";
//print_cols();

	//test of the structure
	if (!has_strict_structure())
		return false;

	//test of corners
	for (it = corners.begin(); it != corners.end(); ++it)
	{
		z = it->first.first;
		y = it->first.second;
		x = getX(z, y);

		//cout << z << " " << y << " " << x << endl;

		if (x + 1 != getX(z))
		{
			if (cols.at(z).at(x) == cols.at(z).at(x + 1) + 1)
				return false;
		}
		if (y + 1 != bars.at(x).size())
		{
			if (bars.at(x).at(y) == bars.at(x).at(y + 1) + 1)
				return false;
		}
	}
/*
//NE DODELANO!!!!!!!!!!!!!!!!!!!
	//test of points
	////????
	for (it = points.begin(); it != points.end(); ++it)
	{
		z = it->first.first;
		y = it->first.second;
		x = getpointX(z, y);

		if (x + 1 != getX(z))
		{
			if (cols.at(z).at(x) == cols.at(z).at(x + 1) + 1)
				return false;
		}
		if (y + 1 != bars.at(x).size())
		{
			if (bars.at(x).at(y) == bars.at(x).at(y + 1) + 1)
				return false;
		}
	}
*/

	return true;
}

bool Diagram3D::contains(Diagram3D& inner)
/*
 * Checks whether this diagram contains inner
 */
{
	vvi inner_rows, inner_cols;
	uint64 x, y, z;

	inner_rows = *inner.get_rows();
	inner_cols = *inner.get_cols();

	//if inner diagram is taller, than it is not inner.
	if (inner_rows.size() > num_of_levels)
		return false;

	for (z = 0; z < inner_rows.size(); ++z)
	{
		//if inner diagram has more rows at level z, it is not inner.
		if (inner_rows.at(z).size() > rows.at(z).size())
			return false;
		//if inner diagram has more cols at level z, it is not inner.

		if (inner_cols.at(z).size() > cols.at(z).size())
			return false;

		//cout << inner_rows.at(z).size() << endl;
		for (y = 0; y < inner_rows.at(z).size(); ++y)
		{
			//cout << y << endl;
			//if the row of level z and number y is longer, inner diagram is not inner.
			if (inner_rows.at(z).at(y) > rows.at(z).at(y))
				return false;
		}
		//cout << inner_cols.at(z).size() << endl;
		for (x = 0; x < inner_cols.at(z).size(); ++x)
		{
			//cout << x << endl;
			//if the col of level z and number x is longer, inner diagram is not inner.
			if (inner_cols.at(z).at(x) > cols.at(z).at(x))
				return false;
		}
	}

	return true;
}

uint64 Diagram3D::getX()
{
	return cols.at(0).size();
}

uint64 Diagram3D::getX(uint64 z)
{
	uint64 size;

	size = cols.size();
	if (z > size)
	{
		cout << "ERROR: Diagram3D::getX(): incorrect z!\n";
		exit(1);
	}
	if (z == size)
		return 0;
	else
		return cols.at(z).size();
}

uint64 Diagram3D::getX(pu coord)
{
	return getX(coord.first, coord.second);
}

uint64 Diagram3D::getpointX(uint64 z, uint64 y)
{
	if (z >= rows.size())
		return 0;
	if (y >= rows.at(z).size())
		return 0;
	return rows.at(z).at(y);
}

uint64 Diagram3D::getpointY(uint64 z, uint64 x)
{
	if (z >= rows.size())
		return 0;
	if (x >= cols.at(z).size())
		return 0;
	return cols.at(z).at(x);
}

uint64 Diagram3D::getpointZ(uint64 x, uint64 y) 
{
	if ((cols.at(0).size() <= x) || (rows.at(0).size() <= y))
		return 0;
	if ((x >= rows.at(0).at(y)) || (y >= cols.at(0).at(x)))
		return 0;
	return bars.at(x).at(y);	//?
} 

uint64 Diagram3D::getX(uint64 z, uint64 y)
{
	if (z >= rows.size())
	{
                return 0;
	}
	if (y >= rows.at(z).size())
	{
                return 0;
	}
	return rows.at(z).at(y) - 1;
}

uint64 Diagram3D::getY(uint64 z, uint64 x)
{
	if (z >= rows.size())
	{
		return 0;
	}
	if (x >= cols.at(z).size())
	{
		return 0;
	}

	return cols.at(z).at(x) - 1;
}

uint64 Diagram3D::getZ(uint64 x, uint64 y)
{
	if ((cols.at(0).size() <= x) || (rows.at(0).size() <= y))
		return 0;
	if ((x >= rows.at(0).at(y)) || (y >= cols.at(0).at(x)))
		return 0;
	//cout << x << " " << y << endl;
	return bars.at(x).at(y);
}

uint64 Diagram3D::getY()
{
	return rows.at(0).size();
}

uint64 Diagram3D::getY(uint64 z)
{
	uint64 size;

	size = rows.size();
	if (z > size)
	{
		cout << "ERROR: Diagram3D::getY(): incorrect z!\n";
		exit(1);
	}
	if (z == size)
		return 0;
	else
		return rows.at(z).size();
}

uint64 Diagram3D::getZ()
{
	return rows.size();
}


double Diagram3D::calc_planch_weight(uint64 z, uint64 y)
{
	uint64 x, i, hook_len;
	double weight;
	//cout << "A\n";

	//cout << z << " " << y << endl;

	if (!STANDARD)
		return strict_calc_planch_weight(z, y);

	x = getpointX(z, y);

	weight = 1;

	for (i = 0; i < x; ++i)
	{
		hook_len = get_hook(i, y, z);
		weight *= hook_len * 1. / (hook_len + 1);
	}
	for (i = 0; i < y; ++i)
	{
		hook_len = get_hook(x, i, z);
		weight *= hook_len * 1. / (hook_len + 1);
	}
	for (i = 0; i < z; ++i)
	{
		hook_len = get_hook(x, y, i);
		weight *= hook_len * 1. / (hook_len + 1);
	}

	return weight;
}


double Diagram3D::strict_calc_planch_weight(uint64 z, uint64 y)
{
	uint64 x, i, hook_len;
	double weight;

	x = getpointX(z, y);

//cout << "notskew " << x << " " << y << " " << z << endl;

	// x,y,z - coordinates of a new box (NOT skewed coordinates!!!).
	//Make them skewed:
	y += x;
	z += y;

//cout << "skewed  " << x << " " << y << " " << z << endl;

	weight = 1;

	for (i = 0; i < x; ++i)
	{
		hook_len = get_skewed_hook(i, y, z);
//cout << "x:" << hook_len << " ";
		weight *= hook_len * 1. / (hook_len + 1);
//cout << "weight1: " << weight << endl;
	}
	for (i = x; i < y; ++i)
	{
		hook_len = get_skewed_hook(x, i, z);
//cout << "y:" << hook_len << " ";
		weight *= hook_len * 1. / (hook_len + 1);
//cout << "weight2: " << weight << endl;
	}

	for (i = y; i < z; ++i)
	{
//cout << x << " " << y << " " << z << endl;
		hook_len = get_skewed_hook(x, y, i);
//cout << "z:" << hook_len << " ";
//exit(1);
		weight *= hook_len * 1. / (hook_len + 1);
//cout << "weight3: " << weight << endl;
	}
//cout << "\nweight: " << weight << endl;
	return weight;
}

double Diagram3D::calc_planch_sum(double alpha)
{
	double res;
	//mpu* p;

	res = 0;






/*
BAD: slow parallelize
	#pragma omp parallel
	#pragma omp single
	{
		for (it = p->begin(); it != p->end(); ++it)
		{
			#pragma omp task firstprivate(it)
			it->second /= planch_sum;
			
			if (it->second > mp)
				#pragma omp critical
				if (it->second > mp)
				{
					mp = it->second;
					ml = it->first;
				}
		}
	  	#pragma omp taskwait
	}
*/

	#pragma omp parallel
	{
		uint64 cnt = 0;
		double weight;
		int ithread = omp_get_thread_num();
		int nthreads = omp_get_num_threads();
		for (mpu::iterator it = points.begin(); it != points.end(); ++it, cnt++)
		{
			if(cnt % nthreads != ithread) continue;

			weight = calc_planch_weight(it->first.first, it->first.second);

			if (alpha != 1)
				weight = pow(weight, alpha);

			it->second = weight;
			res += weight;
		}
	}

/*
	for (mpu::iterator it = points.begin(); it != points.end(); ++it)
	{
		weight = calc_planch_weight(it->first.first, it->first.second);

		if (alpha != 1)
			weight = pow(weight, alpha);

		it->second = weight;
		res += weight;
	}
*/
	

	return res;
}

void Diagram3D::calc_planch_probs(pu* max_line, double* max_prob, double alpha)
{
	mpu* p;
	mpu::iterator it;
	double mp;
	pu ml;

	p = get_points();
	mp = 0;
	ml = pu(0, 0);

	planch_sum = calc_planch_sum(alpha);


	for (it = p->begin(); it != p->end(); ++it)
	{
		it->second /= planch_sum;
		if (it->second > mp)
		{
			mp = it->second;
			ml = it->first;
		}
	}

	if (max_line != NULL)
		*max_line = ml;
	if (max_prob != NULL)
		*max_prob = mp;
}

void Diagram3D::calc_planch_probs(double alpha)
{
	pu max_line;
	double max_prob;
	calc_planch_probs(&max_line, &max_prob, alpha);
}

double Diagram3D::calc_planch_prob(uint64 z, uint64 y, double alpha)
{
	pu line; double prob;
	calc_planch_probs(&line, &prob, alpha);
	return points.at(pu(z, y));
}

double Diagram3D::calc_planch_prob(pu coord, double alpha)
{
	return calc_planch_prob(coord.first, coord.second, alpha);
}




void Diagram3D::rec_improve_probs(const int ITERS, pu& max_coord, double& max_prob, const double alpha)
{
	mpu max_points;//, new_points;
	Diagram3D diag;
	mpu::iterator it;
	pu coord, tmp_pu;
	double product, sum, tmp_d;

	if (ITERS == 0)
		calc_planch_probs(&max_coord, &max_prob, alpha);
	else
		rec_improve_probs(ITERS - 1, max_coord, max_prob, alpha);


	diag.init(this);
	diag.add(max_coord);

	if (ITERS == 0)
		diag.calc_planch_probs(alpha);
	else
		diag.rec_improve_probs(ITERS - 1, tmp_pu, tmp_d, alpha);
	max_points = *diag.get_points();

	sum = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		if (coord == max_coord)
		{
			sum += max_prob;
			continue;
		}
		product = max_prob * max_points.at(coord);

		diag.init(this);
		diag.add(coord);

		if (ITERS == 0)
			product /= diag.calc_planch_prob(max_coord, alpha);
		else
		{
			diag.rec_improve_probs(ITERS - 1, tmp_pu, tmp_d, alpha);
			product /= diag.get_points()->at(max_coord);
		}
		points.at(coord) = product;
		sum += product;
	}

	//normalization of points
	max_prob = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		points.at(coord) /= sum;
		if (points.at(coord) > max_prob)
		{
			max_prob = points.at(coord);
			max_coord = coord;
		}
	}
}







void Diagram3D::improve_probs(pu& max_coord, double& max_prob, const double alpha)
/*
 * uluchshaem veroyatnosti (po rombikam)
 */
{
	mpu max_points;//, new_points;
	Diagram3D diag;
	mpu::iterator it;
	pu coord;
	double product, sum;

	calc_planch_probs(&max_coord, &max_prob, alpha);


	diag.init(this);
	diag.add(max_coord);
	diag.calc_planch_probs(alpha);
	max_points = *diag.get_points();

	sum = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		if (coord == max_coord)
		{
			sum += max_prob;
			continue;
		}
		product = max_prob * max_points.at(coord);

		diag.init(this);
		diag.add(coord);
		product /= diag.calc_planch_prob(max_coord, alpha);

		points.at(coord) = product;
		sum += product;
	}

	//normalization of points
	max_prob = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		points.at(coord) /= sum;
		if (points.at(coord) > max_prob)
		{
			max_prob = points.at(coord);
			max_coord = coord;
		}
	}

}

void Diagram3D::improve_probs2(pu& max_coord, double& max_prob, const double alpha)
/*
 * uluchshaem veroyatnosti (po rombikam)
 */
{
	mpu max_points;//, new_points;
	Diagram3D diag;
	mpu::iterator it;
	pu coord;
	double product, sum;

	improve_probs(max_coord, max_prob, alpha);

	diag.init(this);
	diag.add(max_coord);
	diag.improve_probs(alpha);
	max_points = *diag.get_points();
/*
	if (n == 2)
	{
		cout << "max_points:\n";
		for (auto it = max_points.begin(); it != max_points.end(); ++it)
		{
			cout << it->first.first << " " << it->first.second << " " << it->second << endl;
		}
	}
*/
	sum = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
/*
		if (n == 2)
		{
			cout << "coord: " << coord.first << " " << coord.second << endl;
			if (coord == max_coord)
				cout << "sum+=" << max_prob << ". sum = " << sum+max_prob << endl;
		}
*/
		if (coord == max_coord)
		{
			sum += max_prob;
			continue;
		}
/*
		if (n == 2)
		{
			cout << "max_branch: " << max_prob << " " << max_points.at(coord) << endl;
		}
*/
		product = max_prob * max_points.at(coord);

		diag.init(this);
		diag.add(coord);

		diag.improve_probs(alpha);
		product /= diag.get_points()->at(max_coord);
/*
		if (n == 2)
		{
			cout << "third: " << diag.get_points()->at(max_coord) << endl;
			cout << "result: " << product << endl;
		}
*/
		points.at(coord) = product;
		sum += product;
	}

	//normalization of points
	max_prob = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		points.at(coord) /= sum;
		if (points.at(coord) > max_prob)
		{
			max_prob = points.at(coord);
			max_coord = coord;
		}
	}
/*
	if (n == 2)
	{
		print_cols();
		print_points();
		exit(1);
	}
*/
}

void Diagram3D::improve_probs3(pu& max_coord, double& max_prob, const double alpha)
/*
 * uluchshaem veroyatnosti (po rombikam)
 */
{
	mpu max_points;//, new_points;
	Diagram3D diag;
	mpu::iterator it;
	pu coord;
	double product, sum;

	improve_probs2(max_coord, max_prob, alpha);

	diag.init(this);
	diag.add(max_coord);
	diag.improve_probs2(alpha);
	max_points = *diag.get_points();

	sum = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		if (coord == max_coord)
		{
			sum += max_prob;
			continue;
		}
		product = max_prob * max_points.at(coord);

		diag.init(this);
		diag.add(coord);

		diag.improve_probs2(alpha);
		product /= diag.get_points()->at(max_coord);

		points.at(coord) = product;
		sum += product;
	}

	//normalization of points
	max_prob = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		points.at(coord) /= sum;
		if (points.at(coord) > max_prob)
		{
			max_prob = points.at(coord);
			max_coord = coord;
		}
	}
}

void Diagram3D::improve_probs4(pu& max_coord, double& max_prob, const double alpha)
/*
 * uluchshaem veroyatnosti (po rombikam)
 */
{
	mpu max_points;//, new_points;
	Diagram3D diag;
	mpu::iterator it;
	pu coord;
	double product, sum;

	improve_probs3(max_coord, max_prob, alpha);

	diag.init(this);
	diag.add(max_coord);
	diag.improve_probs3(alpha);
	max_points = *diag.get_points();

	sum = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		if (coord == max_coord)
		{
			sum += max_prob;
			continue;
		}
		product = max_prob * max_points.at(coord);

		diag.init(this);
		diag.add(coord);

		diag.improve_probs3(alpha);
		product /= diag.get_points()->at(max_coord);

		points.at(coord) = product;
		sum += product;
	}

	//normalization of points
	max_prob = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		points.at(coord) /= sum;
		if (points.at(coord) > max_prob)
		{
			max_prob = points.at(coord);
			max_coord = coord;
		}
	}
}

void Diagram3D::improve_probs5(pu& max_coord, double& max_prob, const double alpha)
/*
 * uluchshaem veroyatnosti (po rombikam)
 */
{
	mpu max_points;//, new_points;
	Diagram3D diag;
	mpu::iterator it;
	pu coord;
	double product, sum;

	improve_probs4(max_coord, max_prob, alpha);

	diag.init(this);
	diag.add(max_coord);
	diag.improve_probs4(alpha);
	max_points = *diag.get_points();

	sum = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		if (coord == max_coord)
		{
			sum += max_prob;
			continue;
		}
		product = max_prob * max_points.at(coord);

		diag.init(this);
		diag.add(coord);

		diag.improve_probs4(alpha);
		product /= diag.get_points()->at(max_coord);

		points.at(coord) = product;
		sum += product;
	}

	//normalization of points
	max_prob = 0;
	for (it = points.begin(); it != points.end(); ++it)
	{
		coord = it->first;
		points.at(coord) /= sum;
		if (points.at(coord) > max_prob)
		{
			max_prob = points.at(coord);
			max_coord = coord;
		}
	}
}

void Diagram3D::improve_probs(const double alpha)
{
	pu max_coord;
	double max_prob;
	improve_probs(max_coord, max_prob, alpha);
}

void Diagram3D::improve_probs2(const double alpha)
{
	pu max_coord;
	double max_prob;
	improve_probs2(max_coord, max_prob, alpha);
}

void Diagram3D::improve_probs3(const double alpha)
{
	pu max_coord;
	double max_prob;
	improve_probs3(max_coord, max_prob, alpha);
}

void Diagram3D::improve_probs4(const double alpha)
{
	pu max_coord;
	double max_prob;
	improve_probs4(max_coord, max_prob, alpha);
}

void Diagram3D::improve_probs5(const double alpha)
{
	pu max_coord;
	double max_prob;
	improve_probs5(max_coord, max_prob, alpha);
}

void Diagram3D::get_outer_cubes(map<tu, int>& cubes)
{
	cubes = outer_cubes;
}

double Diagram3D::get_planch_sum()
{
	return planch_sum;
}

void Diagram3D::calc_outers_points_corners(map<tu, int>& cubes)
/*
 * Finds which cubes are outer and stores them in "cubes". Update points and corners information from "cubes".
 */
{
	uint64 x, y, z;
	map<tu, int> x_c, x_p, y_c, y_p, z_c, z_p;
	map<tu, int>::iterator it;

	for (z = 0; z < rows.size(); ++z)
	{
		for (y = 0; y < rows.at(z).size(); ++y)
		{
			x = rows.at(z).at(y) - 1;
			cubes[{x, y, z}];
			x_c[{x, y, z}];
			x_p[{x + 1, y, z}];
		}
		x_p[{0, y, z}];
		for (x = 0; x < cols.at(z).size(); ++x)
		{
			y = cols.at(z).at(x) - 1;
			cubes[{x, y, z}];
			y_c[{x, y, z}];
			y_p[{x, y + 1, z}];
		}
		y_p[{x, 0, z}];
	}
	x_p[{0, 0, z}];
	y_p[{0, 0, z}];

	for (x = 0; x < bars.size(); ++x)
	{
		for (y = 0; y < bars.at(x).size(); ++y)
		{
			z = bars.at(x).at(y) - 1;
			cubes[{x, y, z}];
			z_c[{x, y, z}];
			z_p[{x, y, z + 1}];
		}
		z_p[{x, y, 0}];
	}
	z_p[{x, 0, 0}];

	points.clear();
	corners.clear();

	for (it = x_c.begin(); it != x_c.end(); ++it)
		if ((y_c.find(tu(it->first)) != y_c.end()) && (z_c.find(tu(it->first)) != z_c.end()))
		{
			x = it->first.x; y = it->first.y; z = it->first.z;
			add_corner(z, y);
		}
	for (it = x_p.begin(); it != x_p.end(); ++it)
		if ((y_p.find(tu(it->first)) != y_p.end()) && (z_p.find(tu(it->first)) != z_p.end()))
		{
			x = it->first.x; y = it->first.y; z = it->first.z;
			add_point(z, y);
		}
}

void Diagram3D::calc_outer_cubes_slow(map<tu, int>& cubes)
{
	uint64 x, y, z;

	for (z = 0; z < rows.size(); ++z)
	{
		for (y = 0; y < rows.at(z).size(); ++y)
		{
			x = rows.at(z).at(y) - 1;
			cubes[{x, y, z}];
			//cubes.insert(pair<pu, uint64>(pu(z,y), x));
		}
		for (x = 0; x < cols.at(z).size(); ++x)
		{
			y = cols.at(z).at(x) - 1;
			cubes[{x, y, z}];
			//cubes.insert(pair<pu, uint64>(pu(z,y), x));
		}
	}

	for (x = 0; x < bars.size(); ++x)
		for (y = 0; y < bars.at(x).size(); ++y)
		{
			z = bars.at(x).at(y) - 1;
			//cubes.insert(pair<pu, uint64>(pu(z,y), x));
			cubes[{x, y, z}];
		}
}

void Diagram3D::calc_outer_cubes_skew(map<tu, int>& cubes)
{
	uint64 x, y, z, x_, y_, z_;

	//x <= y <= z
	for (z = 0; z < rows.size(); ++z)
		for (y = 0; y < rows.at(z).size(); ++y)
			for (x = 0; x < rows.at(z).at(y); ++x)
			{
				x_ = x;
				y_ = y + x_;
				z_ = z + y_;
				//z_ = z;

				//if ((y_ >= getX(z, y)) && (z_ >= getY()))
					cubes[{x_, y_, z_}];
			}
/*
	for (auto it = cubes.begin(); it != cubes.end(); ++it)
	{
		cout << it->first.x << " " << it->first.y << " " << it->first.z << endl;
	}
	exit(1);
*/
}

uint64 Diagram3D::get_size()
{
	return n;
}

bool Diagram3D::are_neighbours(pu cube1, pu cube2)
/*
 * Two corners are named neighbour if they have the same edge.
 * The corner is not neighbour to itself.
*/
{
	uint64 xdiff, ydiff, zdiff;

	zdiff = udiff(cube1.first, cube2.first);
	ydiff = udiff(cube1.second, cube2.second);
	xdiff = udiff(getX(cube1.first, cube1.second), getX(cube2.first, cube2.second));

	if ((xdiff > 1) || (ydiff > 1) || (zdiff > 1) || ((xdiff != 0) && (ydiff != 0) && (zdiff != 0)) || (cube1 == cube2))
		return false;

	return true;
}


int remain;
map<pu, pu> clusters;
uint64 clnum;

void Diagram3D::fill_cluster(uint64 z, uint64 y, uint64 x)
/*
 * The recursive function for filling the cluster of corners with other corners.
 * z,y,x - coordinates of the possible corner
 */
{
	//cout << z <<" " << y << " " << x << endl;
	//print_cols();
	if (!is_corner(x, y, z))
	{
		//cout << endl;
		return;
	}
	if (clusters.find(pu(z, y)) != clusters.end())
		return;
	//cout << "is corner!\n\n";

	clusters.insert(pair<pu, pu>(pu(z,y), pu(x, clnum)));
	--remain;

	if (z != 0)
	{
		//cout << "z1\n";
		fill_cluster(z-1, y, x+1);
		fill_cluster(z-1, y+1, x);
		//cout << "z2\n";
	}

	if (y != 0)
	{
		//cout << "y1\n";
		fill_cluster(z,   y-1, x+1);
		fill_cluster(z+1, y-1, x);
		//cout << "y2\n";
	}

	if (x != 0)
	{
		//cout << "x1\n";
		fill_cluster(z, y+1, x-1);
		fill_cluster(z+1, y, x-1);
		//cout << "x2\n";
	}
}

uint64 Diagram3D::calc_clusters_number(map<tu, int>& nearby)
/*
 * FAST
 * calculates the number of clusters starting from the certain points
 */
{
	map<tu, int>::iterator it;
	uint64 x, y, z;

	clnum = 0;
	clusters.clear();
	//cout << n << " nearby!\n";
	for (it = nearby.begin(); it != nearby.end(); ++it)
	{
		x = it->first.x; y = it->first.y; z = it->first.z;
		//cout << x << "," << y << "," << z << endl;
		if (clusters.find(pu(z, y)) != clusters.end())
			continue;
		++clnum;
		fill_cluster(z, y, x);
	}

	//cout << clnum << endl;
	//exit(1);

	return clnum;
}

uint64 Diagram3D::calc_clusters_number()
/*
 * SLOW
 * Calculates the number of "corner clusters" (groups of neighbour corners)
 * Two corners are named neighbour if they have the same edge
 */
{
	mpu::iterator it;

	map<pu, pu>::iterator it2, ittmp;

	remain = corners.size();
	clnum = 0;
	clusters.clear();



	while (remain != 0)
	{

		//new cluster has started
		++clnum;

		//find the first element of the new cluster
		it = corners.begin();
		while ((clusters.find(it->first) != clusters.end()) && (it != corners.end()))
			++it;


		//search for other elements
		fill_cluster(it->first.first, it->first.second, getX(it->first.first, it->first.second));
	}



	return clnum;
}

uint64 Diagram3D::calc_clusters_number_slow()
/*
 * Calculates the number of "corner clusters" (groups of neighbour corners)
 * Two corners are named neighbour if they have the same edge
 */
{
	mpu::iterator it;
	uint64 grnum;
	mpu c;
	bool found;

	map<pu, pu> groups;
	map<pu, pu>::iterator it2, ittmp;

	c = corners;
	grnum = 0;

	while (!c.empty())
	{
		//new group has started
		++grnum;

		//find the first element of the new group
		it = corners.begin();
		while ((groups.find(it->first) != groups.end()) && (it != corners.end()))
			++it;
		groups.insert(pair<pu, pu>(it->first, pu(getX(it->first.first, it->first.second), grnum)));
		c.erase(it->first);

		//search for other elements
		do
		{
			found = false;
			for (it = corners.begin(); it != corners.end(); ++it)
			{
				if (groups.find(it->first) != groups.end())
					continue;

				for (it2 = groups.begin(); it2 != groups.end(); ++it2)
				{
					if (it2->second.second != grnum)
						continue;

					if (are_neighbours(it->first, it2->first))
					{
						found = true;
						groups.insert(pair<pu, pu>(it->first, pu(getX(it->first.first, it->first.second), grnum)));
						c.erase(it->first);
						break;
					}
				}
			}
		}
		while (found);
	}

	//output (tmp)
	/*
	for (uint64 i = 1; i <= grnum; ++i)
	{
		cout << "group " << i << endl;
		for (it2 = groups.begin(); it2 != groups.end(); ++it2)
		{
			if (it2->second.second == i)
			{
				cout << it2->first.first << " " << it2->first.second << " " << it2->second.first << endl;
			}
		}
		cout << endl;
	}
*/
	return grnum;
}

int Diagram3D::calc_number_of_corners_nearby(uint64 z, uint64 y)
{
	uint64 x, res;
	x = getpointX(z, y);

	res = 0;
	if (x > 0)
	{
		if (is_corner(x - 1, y + 1, z)) ++res;
		if (is_corner(x - 1, y, z + 1)) ++res;
	}
	if (y > 0)
	{
		if (is_corner(x + 1, y - 1, z)) ++res;
		if (is_corner(x, y - 1, z + 1)) ++res;
	}
	if (z > 0)
	{
		if (is_corner(x + 1, y, z - 1)) ++res;
		if (is_corner(x, y + 1, z - 1)) ++res;
	}
	return res;
}

uint64 Diagram3D::get_number_of_large_probs(const double THLD)
/*
 * counts the number of points with relative probabilities > THLD
 */
{
	const double EPSILON = 0.000001;
	const int MIN = 0;
	const int MAX = 1;

	pu max_line;
	double min_prob, max_prob, rprob;
	mpu::iterator it;
	uint64 res;

	res = 0;

	calc_planch_probs(&max_line, &max_prob);
	min_prob = max_prob;
	for (it = points.begin(); it != points.end(); ++it)
		if (it->second < min_prob)
			min_prob = it->second;

	for (it = points.begin(); it != points.end(); ++it)
	{

		if (fabs(min_prob -max_prob) < EPSILON)
			rprob = MAX;
		else
			rprob = (it->second - min_prob) * (MAX - MIN) / (max_prob - min_prob) + MIN;

		if (rprob > THLD)
			++res;
	}

	return res;
}

void Diagram3D::get_tip(Diagram3D* tip)
/*
 * Sdvigaem 3D stroguyu diagrammu, sverhu srezaem "verhushku". Eta verhushka - standartnaya diagramma!
 * Zapisyvaem ee v tip.
 */
{
	uint64 x, y, z;//, x_, y_, z_, X, Y;
	mpu::iterator it;
	vvi r;
	map< uint64, map<uint64, uint64>> c;

	if (STANDARD)
		return;

	if (n == 1)
	{
		tip->init();
		return;
	}

	vvi cols2 = cols;
	vvi bars2;

	//1. sdvig po y + otbrasyvaem lishnie nizhnie stroki
	for (z = 0; z < getZ(); ++z)
		for (x = 0; x < getX(z); ++x)
		{
			cols2.at(z).at(x) = cols.at(z).at(x) + x - getX(z);
			//cout << z << " " << x << " " << cols2.at(z).at(x) << endl;
		}

	//2. cols2 => bars2
	//bars2.resize(cols.at(0).size());
	//for (x = 0; x < cols.at(0).size(); ++x)
	//	bars2.at(x).resize(cols.at(0).at(x), 1);

	for (z = 0; z < cols2.size(); ++z)
		for (x = 0; x < cols2.at(z).size(); ++x)
			for (y = 0; y < cols2.at(z).at(x); ++y)
			{
				if (bars2.size() <= x)
				{
					bars2.push_back(vector<uint64>(1, z + 1));
				}
				else
					if (bars2.at(x).size() <= y)
					{
						bars2.at(x).push_back(z + 1);
					}
					else
						bars2.at(x).at(y) = z + 1;
			}

	//3. sdvig po z + otbrasyvaem lishnie nizhnie stroki
	for (x = 0; x < bars2.size(); ++x)
		for (y = 0; y < bars2.at(x).size(); ++y)
		{
			bars2.at(x).at(y) += y - bars2.at(x).size();
		}

	tip->init_by_bars(bars2);

	//mpu corners2;
/*
	for (it = corners.begin(); it != corners.end(); ++it)
	{
		z = it->first.first;
		y = it->first.second;
		x = getX(z, y);

		x_ = x;
		y_ = y + x_;
		z_ = z + y_;

		X = getX(z);
		//skolko v zadannom x-e Y-kov (na raznyh z...)
		Y = bars.at(x).at(0) - 1;

		if ((y_ > X) && (z_ > Y))
			c[pu(z_ - Y, y_ - X)] = x_;
	}

	for (auto it = c.begin(); it != c.end(); ++it)
	{
		z = it->first;
		if (z > num_of_levels - 1)
		{
			r.push_back(vector<uint64>(1, 0));
			//r.back().at(0) = 0;
		}
		else
		{

		}


		//cout << it->first << endl;
		//for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		//	cout << "\t" << it2->first << endl;

	}

	rows = r;
	init_by_rows(rows);
*/
}

int Diagram3D::get_diam(tu *coord)
{
	const double X_COEF = M_PI;
	const double Y_COEF = M_PI / 2.0;
	const double Z_COEF = 1;
	const double EPS = 0.1;

	int x, y, z;
	for (auto it = outer_cubes.begin(); it != outer_cubes.end(); ++it)
	{
		x = it->first.x;
		y = it->first.y;
		z = it->first.z;

		//cout << x << " " << y << " " << z << endl;

		//z=2y=3x
		//if ((3*x == 2*y) && (3*x == z))
		if ((abs(X_COEF*x - Y_COEF*y) <= EPS) && (abs(X_COEF*x - Z_COEF*z) <= EPS))
		{
			*coord = it->first;
			return x;
		}
	}

	cout << "ERROR: Diagram3D::get_diam(): No diameter point was found!\n";
	exit(1);
}

bool Diagram3D::get_standard_flag()
{
	return STANDARD;
}

void Diagram3D::standard_from_strict(Diagram3D& stand)
{
	//NE DODELANO!!!
	//vvi r, c, b;
	uint64 x, y, z;
	map< uint64, map<uint64, uint64>> r;
	vvi res_rows;
	if (STANDARD)
		return;

	//cout << outer_cubes.size() << endl;

	for (auto it = outer_cubes.begin(); it != outer_cubes.end(); ++it)
	{
		x = it->first.x; y = it->first.y; z = it->first.z;

		//cout << x << " " << y << " " << z << endl;

		//++x; ++y; ++z;

		//xyz
		//002
		//200

		if (r[x][y] < z+1)
			r[x][y] = z+1;
		if (r[x][z] < y+1)
			r[x][z] = y+1;
		if (r[y][x] < z+1)
			r[y][x] = z+1;
		if (r[y][z] < x+1)
			r[y][z] = x+1;
		if (r[z][x] < y+1)
			r[z][x] = y+1;
		if (r[z][y] < x+1)
			r[z][y] = x+1;



/*
		if (r[x+y+z][x+y] < x+1) //x y z
			r[x+y+z][x+y] = x+1;
		if (r[x+z][x+y+z] < x+1) //x z y
			r[x+z][x+y+z] = x+1;
		if (r[x+y+z][y] < x+1 + y) //y x z
			r[x+y+z][y] = x+1 + y;
		if (r[y+z][y] < x+1 + y + z) //y z x
			r[y+z][y] = x+1 + y + z;
		if (r[z][x+y+z] < z + x+1) //z x y
			r[z][x+y+z] = z + x+1;
		if (r[z][z+y] < x+1 + y + z) //z y x
			r[z][z+y] = x+1 + y + z;
*/
	}

	res_rows.resize(r.size());

	//cout << "s: " << res_rows.at(0).size() << endl;


	for (auto it = r.begin(); it != r.end(); ++it)
	{
		for (auto it2 = r.at(it->first).begin(); it2 != r.at(it->first).end(); ++it2)
		{
			//cout << "A\n";
			res_rows.at(it->first).push_back(it2->second);
			//cout << "B\n";
		}
	}

	//cout << res_rows.size() << " " << res_rows.at(0).size() << endl;

	stand.init_by_rows(res_rows);
/*
	cout << "res:\n";
	for (auto it = res_rows.begin(); it != res_rows.end(); ++it)
	{
		for (auto it2 = it->begin(); it2 != it->end(); ++it2)
			cout << *it2 << endl;
		cout << endl;
	}
*/
}

bool Diagram3D::has_xyz(uint64 x, uint64 y, uint64 z)
//same as consists_of?
{
	if (rows.size() > z)
		if (rows.at(z).size() > y)
			if (rows.at(z).at(y) > x)
				return true;
	return false;
}

void Diagram3D::get_legal_points(Diagram3D* target, mpu* leg_p)
{
	mpu::iterator it;
	uint64 x, y, z;

	leg_p->clear();
	for (it = points.begin(); it != points.end(); ++it)
	{
		z = it->first.first;
		y = it->first.second;
		x = getpointX(z, y);

		if (target->has_xyz(x, y, z))
			(*leg_p)[pu(z, y)] = it->second;
	}
}

void Diagram3D::calc_all_tabs(set<dtu>* tabs, dtu tab, Diagram3D diag)
{
	mpu leg_p;
	mpu::iterator it;
	pu coord;
        uint64 x, y, z;

	if (n == diag.get_size())
	{
		tabs->insert(tab);
		return;
	}

	diag.get_legal_points(this, &leg_p);

	for (it = leg_p.begin(); it != leg_p.end(); ++it)
	{
		coord = it->first;
                z = coord.first; y = coord.second;
                x = diag.getpointX(z, y);

                tab.push_back(tu(x, y, z));
		diag.add(coord);
		calc_all_tabs(tabs, tab, diag);
		tab.pop_back();
		diag.remove(coord);
	}
}

map<uint64, map<vvi, uint64> > Diagram3D::calc_all_dimensions_UINT64(bool STANDARD, const uint64 MAX_LEVEL)
/*
	NOTE: ALL dims are limited by uint64!

	Prints/Returns the dimensions of all 3D diagrams (sta/stri) of levels <= MAX_LEVEL.	
*/
{
	list<Diagram3D> queue;
	map<uint64, map<vvi, uint64> > dims;
	Diagram3D diag(STANDARD), diag2(STANDARD);
	uint64 level, i, j, dim, sum, sum2;
	mpu* points;
	mpu::iterator it;
	vvi *rows, *rows2, r;

	points = diag.get_points();
	rows = diag.get_rows();
	rows2 = diag2.get_rows();

	queue.push_back(diag);

	dims[1][{{1}}] = 1;

	do
	{
		diag = queue.front();
		queue.pop_front();

		level = diag.get_size();

		if (level == MAX_LEVEL)
			break;

		//cout << "\n\ncurrent INIT diag:\n";
		//diag.print_cols();
		//cout << "queue size: " << queue.size() << endl;

		for (it = points->begin(); it != points->end(); ++it)
		{
			diag2.init(&diag);
			diag2.add(it->first);

			//cout << "current diag:\n";
			//diag2.print_cols();

			if (dims[level + 1].find(*rows2) == dims[level + 1].end())
			{
				queue.push_back(diag2);
			}

			dims[level + 1][*rows2] += dims[level][*rows];

		}
		
	}
	while (1);

	return dims;
}

/*
map<uint64, map<vvi, mpz_t> > Diagram3D::calc_all_dimensions(bool STANDARD, const uint64 MAX_LEVEL)
//Prints/Returns the dimensions of all 3D diagrams (sta/stri) of levels <= MAX_LEVEL.	

{

	list<Diagram3D> queue;
	map<uint64, map<vvi, mpz_t> > dims;
	Diagram3D diag(STANDARD), diag2(STANDARD);
	uint64 level, i, j;
	mpu* points;
	mpu::iterator it;
	vvi *rows, *rows2, r;

	points = diag.get_points();
	rows = diag.get_rows();
	rows2 = diag2.get_rows();

	queue.push_back(diag);

	mpz_init_set_ui(dims[1][{{1}}], 1);

	do
	{
		diag = queue.front();
		queue.pop_front();

		level = diag.get_size();

		if (level == MAX_LEVEL)
			break;

		//cout << "\n\ncurrent INIT diag:\n";
		//diag.print_cols();
		//cout << "queue size: " << queue.size() << endl;

		for (it = points->begin(); it != points->end(); ++it)
		{
			diag2.init(&diag);
			diag2.add(it->first);

			//cout << "current diag:\n";
			//diag2.print_cols();

			if (dims[level + 1].find(*rows2) == dims[level + 1].end())
			{
				queue.push_back(diag2);
				mpz_init_set_ui(dims[level + 1][*rows2], 0);
			}

			mpz_add(dims[level + 1][*rows2], dims[level + 1][*rows2], dims[level][*rows]);
		}
		
	}
	while (1);

	return dims;

}
*/
/*
map<uint64, map<vvi, mpz_class> > Diagram3D::calc_all_dimensions2(bool STANDARD, const uint64 MAX_LEVEL)
//Prints/Returns the dimensions of all 3D diagrams (sta/stri) of levels <= MAX_LEVEL.	
{

	list<Diagram3D> queue;
	map<uint64, map<vvi, mpz_class> > dims;
	Diagram3D diag(STANDARD), diag2(STANDARD);
	uint64 level, i, j;
	mpu* points;
	mpu::iterator it;
	vvi *rows, *rows2, r;

	points = diag.get_points();
	rows = diag.get_rows();
	rows2 = diag2.get_rows();

	queue.push_back(diag);

	dims[1][{{1}}] = 1;

	do
	{
		diag = queue.front();
		queue.pop_front();

		level = diag.get_size();

		if (level == MAX_LEVEL)
			break;

		//cout << "\n\ncurrent INIT diag:\n";
		//diag.print_cols();
		//cout << "queue size: " << queue.size() << endl;

		for (it = points->begin(); it != points->end(); ++it)
		{
			diag2.init(&diag);
			diag2.add(it->first);

			//cout << "current diag:\n";
			//diag2.print_cols();

			if (dims[level + 1].find(*rows2) == dims[level + 1].end())
			{
				queue.push_back(diag2);
				dims[level + 1][*rows2] = 0;
			}

			dims[level + 1][*rows2] += dims[level][*rows];
		}
		
	}
	while (1);

	return dims;

}
*/

void Diagram3D::print_dims(map<vvi, mpz_class> dims, ofstream& f)
{
	vvi r;
	uint64 i, j;
	map<vvi, mpz_class>::iterator it;

	for (it = dims.begin(); it != dims.end(); ++it)
	{
		r = it->first;
		f << "\"";
		for (i = 0; i < r.size(); ++i)
		{
			for (j = 0; j < r.at(i).size(); ++j)
			{
				f << r.at(i).at(j);
				if (j + 1 != r.at(i).size())
					f << " ";
			}
			if (i + 1 != r.size())
				f << ", ";
		}
		f << "\" ";
		f << it->second << "\n";
	};
	f << "\n";
}

void Diagram3D::print_all_and_max_dims(const bool STANDARD, const uint64 MAX_LEVEL)
{
	queue<Diagram3D> queue;
	Diagram3D diag(STANDARD), diag2(STANDARD);
	map<vvi, mpz_class> dims1, dims2; //dimensions (this and next levels)
	mpz_class maxdim;
	mpu* points;
	vvi *rows, *rows2, maxrows;
	uint64 level1, level2, z, y;
	mpu::iterator it;

	ofstream fmax(("max_dims_" + to_string(MAX_LEVEL) + ".txt").c_str());
	ofstream fall(("all_dims_" + to_string(MAX_LEVEL) + ".txt").c_str());

	points = diag.get_points();
	rows = diag.get_rows();
	rows2 = diag2.get_rows();

	queue.push(diag);
	dims1[{{1}}] = 1;

	fmax << "1       1                               1\n";
	fall << "level 1\n\"1\" 1\n";

	level1 = 1;
	maxdim = 0;
	do
	{
		diag = queue.front();
		queue.pop();

		level2 = diag.get_size();

		if (level1 != level2)
		{
			//save all diags
			fall << "level " << level2 << endl;
			print_dims(dims2, fall);

			//save max diag
			auto max = std::max_element(dims2.begin(), dims2.end(),
			    [](const pair<vvi, mpz_class>& p1, const pair<vvi, mpz_class>& p2) {
				return p1.second < p2.second; });

			maxrows = max->first;
			maxdim = max->second;

			fmax << left << setw(2) << level2 << "\t" << setw(30) << maxdim << "\t";

			for (z = 0; z < maxrows.size(); ++z)
			{
				for (y = 0; y < maxrows.at(z).size(); ++y)
				{
				        fmax << maxrows.at(z).at(y);
				        if (y != maxrows.at(z).size() - 1)
				                fmax << " ";
				}
				if (z != maxrows.size() - 1)
				        fmax << ", ";
			}
			fmax << "\n";
			fmax.flush();
			maxdim = 0;


			dims1 = dims2;
			dims2.clear();
			level1 = level2;
		}

		if (level2 == MAX_LEVEL)
			continue;

		for (it = points->begin(); it != points->end(); ++it)
		{
			diag2.init(&diag);
			diag2.add(it->first);

			if (dims2.find(*rows2) == dims2.end())
			{
				queue.push(diag2);
				dims2[*rows2] = 0;
			}

			dims2[*rows2] += dims1[*rows];
		}

	}
	while (!queue.empty());



	fmax.close(); fall.close();
	
}
unsigned long int Diagram3D::getDim() {
	mpz_t dim;
	mpz_init(dim);
	calc_exact_dim(dim);
	return mpz_get_ui(dim);
}

void Diagram3D::calc_exact_dim(mpz_t dim)
{

	const bool nop = true;

	list<Diagram3D> queue;
	map<uint64, map<vvi, mpz_t> > dims;
	Diagram3D diag(STANDARD), diag2(STANDARD);
	uint64 level, prevlevel, i, j;
	mpu* p;
	mpu::iterator it;
	vvi *r, *r2;

	p = diag.get_points();
	r = diag.get_rows();
	r2 = diag2.get_rows();

	diag.remove_invalid_points(rows);

	queue.push_back(diag);

	mpz_init_set_ui(dims[1][{{1}}], 1);

	prevlevel = 0;

	do
	{
		diag = queue.front();
		queue.pop_front();

		level = diag.get_size();

		//when we switch to next level
		if (level != prevlevel)
		{
			//delete all previous diagrams
			if (level > 1)
			{
				for (auto it = dims.at(level-1).begin(); it != dims.at(level-1).end(); ++it)
					mpz_clear(it->second);
				dims.erase(level-1);
			}

			//cout << "level " << level << " / " << n << endl;
			prevlevel = level;
		}

		if (level == n)
			break;

		for (it = p->begin(); it != p->end(); ++it)
		{

			diag2.init(&diag);
			diag2.add_to_target(it->first, *get_cols());
			if (dims[level + 1].find(*r2) == dims[level + 1].end())
			{
				queue.push_back(diag2);
				mpz_init_set_ui(dims[level + 1][*r2], 0);
			}

			mpz_add(dims[level + 1][*r2], dims[level + 1][*r2], dims[level][*r]);
		}
	}
	while (1);

	mpz_set(dim, dims.at(n).at(rows));

}

void Diagram3D::print_exact_dim()
{
	mpz_t dim;
	mpz_init(dim);
	calc_exact_dim(dim);
	gmp_printf("%Zd\n", dim);
	mpz_clear(dim);
}


/*
NE DODELANO - PARALLEL

void Diagram3D::calc_exact_dim(mpz_t dim)
{
	const bool nop = true;

	list<Diagram3D> queue;
	map<uint64, map<vvi, mpz_t> > dims;
	Diagram3D diag(STANDARD), diag2(STANDARD);
	uint64 level, prevlevel;
	mpu* p;
	vvi *r, *r2;
	map<int,int> levels; //how many threads has switched to a new level

	p = diag.get_points();
	r = diag.get_rows();
	r2 = diag2.get_rows();

	diag.remove_invalid_points(rows);

	queue.push_back(diag);

	mpz_init_set_ui(dims[1][{{1}}], 1);

	prevlevel = 0;

	#pragma omp parallel for if (!nop) private (level, prevlevel, diag, diag2, p, r, r2)
	while(1)//for (; 1; )
	{
		#pragma omp critical
		{
			diag = queue.front();
			queue.pop_front();
		}

		level = diag.get_size();

		//when we switch to next level
		if (level != prevlevel)
		{
			levels[level]++;

			//if it is a last thread which switched to next level
			if (levels[level] == omp_get_max_threads())
			{
				#pragma omp barrier
				//delete all previous diagrams
				if (level > 10)
				{
					for (uint64 l = 1; l < level; ++l)
					{
						for (auto it = dims.at(l).begin(); it != dims.at(l).end(); ++it)
							mpz_clear(it->second);
						dims.erase(l);
					}
				}

				cout << "level " << level << " / " << n << endl;
				prevlevel = level;
			}
		}

		//if (level == n)
		//	break;

		for (mpu::iterator it = p->begin(); it != p->end(); ++it)
		{

			diag2.init(&diag);
			diag2.add_to_target(it->first, *get_cols());
			if (dims[level + 1].find(*r2) == dims[level + 1].end())
			{
				queue.push_back(diag2);
				mpz_init_set_ui(dims[level + 1][*r2], 0);
			}

			mpz_add(dims[level + 1][*r2], dims[level + 1][*r2], dims[level][*r]);
		}
		
	}

	mpz_set(dim, dims.at(n).at(rows));
}
*/

void Diagram3D::print_all_dimensions(bool STANDARD, const uint64 MAX_LEVEL)
{
/*
	map<uint64, map<vvi, mpz_t> > dims;
	mpz_t dim, sum, sum2, mul;
	FILE *f_dims, *f_dimsum;
	uint64 i, j, level;
	vvi r;

	f_dims = fopen(("3D_dims_" + to_string(MAX_LEVEL) + ".txt").c_str(), "w");
	f_dimsum = fopen(("3D_dims_sum_" + to_string(MAX_LEVEL) + ".txt").c_str(), "w");

	mpz_init(dim); mpz_init(sum); mpz_init(sum2); mpz_init(mul);
	dims = calc_all_dimensions(STANDARD, MAX_LEVEL);
	for (level = 1; level <= MAX_LEVEL; ++level)
	{
		fprintf(f_dims, "level %llu\n", level);
		for (auto it = dims.at(level).begin(); it != dims.at(level).end(); ++it)
		{
			r = it->first;
			fprintf(f_dims, "\"");
			for (i = 0; i < r.size(); ++i)
			{
				for (j = 0; j < r.at(i).size(); ++j)
				{
					fprintf(f_dims, "%llu", r.at(i).at(j));
					if (j + 1 != r.at(i).size())
						fprintf(f_dims, " ");
				}
				if (i + 1 != r.size())
					fprintf(f_dims, ", ");
			}
			fprintf(f_dims, "\" ");
			gmp_fprintf(f_dims, "%Zd\n", it->second);
		};
		fprintf(f_dims, "\n");
	}



	for (level = 1; level <= MAX_LEVEL; ++level)
	{
		mpz_set_ui(sum, 0); mpz_set_ui(sum2, 0);		
		for (auto it = dims.at(level).begin(); it != dims.at(level).end(); ++it)
		{
			mpz_set(dim, it->second);
			mpz_add(sum, sum, dim);
			mpz_mul(mul, dim, dim);
			mpz_add(sum2, sum2, mul);
		}
		gmp_fprintf(f_dimsum, "%llu %Zd %Zd\n", level, sum, sum2);
	}
	mpz_clear(dim); mpz_clear(sum); mpz_clear(sum2); mpz_clear(mul);

	fclose(f_dims);
	fclose(f_dimsum);
*/
}

int Diagram3D::calc_hooks_min_GCD()
{
	int res, hooklen;
	uint64 x, y, z, i;
	int hooklengths[10000];
	res = (int)(get_hook(0, 0, 0));

	i = 0;
	for (z = 0; z != num_of_levels; ++z)
		for (y = 0; y != rows.at(z).size(); ++y)
			for (x = 0; x != rows.at(z).at(y); ++x) 
			{
				//cout << x << " " << y << " " << z << endl;
				hooklen = (int)(get_hook(x, y, z));
				//res = (res * hooklen) / GCD(res, hooklen);
				hooklengths[i++] = hooklen;
				cout << hooklen << " ";
			}
	cout << "\nlcm: " << findlcm(hooklengths, i) << endl;

	return res;
}

