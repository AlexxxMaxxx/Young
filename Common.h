//#pragma once
#ifndef _COMMON_H
#define _COMMON_H
#include <math.h>
#include <cstdlib>
#include <vector>
#include <deque>
#include <map>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <tuple>
#include <numeric>
#include <gmp.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>
#include <unordered_map>

#define uint64 unsigned long long
#define uchar unsigned char
#define di deque<uint64>
#define mi map<uint64,double>

#define pu pair<uint64, uint64>
#define pc pair<uchar, uchar>
#define pd pair<double, double>

#define vi vector<uint64>
#define vd vector<double>
#define vvi vector<vi>
#define vvd vector<vd>
#define vpu vector<pu>
#define vvpu vector<vpu>

#define mdd map<double, double>
#define mpu map<pu, double> //24 bytes each
#define mpp map<pu, pu>
#define mppd map<pu, pair<double, uint64> >

#define dpu deque<pu>
#define dpc deque<pc>

#define dqu deque<QU>
#define dqc deque<QC>

#define lpu list<pu>
#define lpc list<pc>
#define ltu list<tu>
#define ltc list<tc>		//хватит ли?
#define lqu list<QU>
#define lqc list<QC>

using namespace std;
typedef unsigned int oint128; 
typedef std::vector<std::vector<std::vector<oint128>>> vvvi; 
typedef std::tuple<int,int,int> ti;
typedef std::map<ti, double> mtid;

struct tu {
	uint64 x, y, z;
	tu() : x(0), y(0), z(0) { }
	tu(uint64 xx, uint64 yy, uint64 zz) : x(xx), y(yy), z(zz) { }	
};

struct tc {
	uchar x, y, z; // 255 - limit size table
	tc() : x(0), y(0), z(0) { }
	tc(uchar xx, uchar yy, uchar zz) : x(xx), y(yy), z(zz) { }	
	bool operator <(const tc& obj) const{
    		return std::tie(x, y, z) < std::tie(obj.x, obj.y, obj.z);
	}
	bool operator ==(const tc& obj) const{
    		return std::tie(x, y, z) == std::tie(obj.x, obj.y, obj.z);
	}
};

class QU {                                       //класс четверки - (x;y;z;l)
private:
    uint64 x, y, z, l;

public:
    QU() : x(0), y(0), z(0), l(0) {}                //init
    QU(uint64 xx, uint64 yy, uint64 zz, uint64 ll) : x(xx), y(yy), z(zz), l(ll) {}
    uint64 getX() { return x; }
    uint64 getY() { return y; }
    uint64 getZ() { return z; }
    uint64 getL() { return l; }
    
   friend bool operator< (const QU & obj1, const QU & obj2) {
      	if(obj1.x < obj2.x)
   		return true;
   	else if (obj1.x == obj2.x) {
   		if(obj1.y < obj2.y) return true;
   		else if(obj1.y == obj2.y) {
   			if(obj1.z < obj2.z) return true;
   			else if(obj1.z == obj2.z ) {
   				if(obj1.l < obj2.l) return true;
   			}
   		}
   	}
   	//if(obj1 != obj2) //немного костыль
   	//	return true;
   	return false;
    }
    
    friend bool operator> (const QU & obj1, const QU & obj2) {
        if(obj1.x > obj2.x)
   		return true;
   	else if(obj1.x == obj2.x) {
   		if(obj1.y > obj2.y) return true;
   		else if(obj1.y == obj2.y) {
   			if(obj1.z > obj2.z) return true;
   			else if(obj1.z == obj2.z) {
   				if(obj1.l > obj2.l) return true;
   			} 
   		}
   	}
   	return false;
    }
    
    friend bool operator== (const QU & obj1, const QU & obj2) {
    	return obj1.x == obj2.x && obj1.y == obj2.y && obj1.z == obj2.z && obj1.l == obj2.l; 
    }
    
    friend bool operator!= (const QU & obj1, const QU & obj2) {
    	return obj1.x != obj2.x && obj1.y != obj2.y && obj1.z != obj2.z && obj1.l != obj2.l; 
    }
};

class QC {                                       //класс четверки - (x;y;z;l)
private:
    uchar x, y, z, l;

public:
    QC() : x(0), y(0), z(0), l(0) {}                //init
    QC(uchar xx, uchar yy, uchar zz, uchar ll) : x(xx), y(yy), z(zz), l(ll) {}
    uchar getX() { return x; }
    uchar getY() { return y; }
    uchar getZ() { return z; }
    uchar getL() { return l; }
    
   friend bool operator< (const QC & obj1, const QC & obj2) {
      	if(obj1.x < obj2.x)
   		return true;
   	else if (obj1.x == obj2.x) {
   		if(obj1.y < obj2.y) return true;
   		else if(obj1.y == obj2.y) {
   			if(obj1.z < obj2.z) return true;
   			else if(obj1.z == obj2.z ) {
   				if(obj1.l < obj2.l) return true;
   			}
   		}
   	}
   	return false;
    }
    
    friend bool operator> (const QC & obj1, const QC & obj2) {
        if(obj1.x > obj2.x)
   		return true;
   	else if(obj1.x == obj2.x) {
   		if(obj1.y > obj2.y) return true;
   		else if(obj1.y == obj2.y) {
   			if(obj1.z > obj2.z) return true;
   			else if(obj1.z == obj2.z) {
   				if(obj1.l > obj2.l) return true;
   			} 
   		}
   	}
   	return false;
    }
    
    friend bool operator== (const QC & obj1, const QC & obj2) {
    	return obj1.x == obj2.x && obj1.y == obj2.y && obj1.z == obj2.z && obj1.l == obj2.l; 
    }
    
    friend bool operator!= (const QC & obj1, const QC & obj2) {
    	return obj1.x != obj2.x && obj1.y != obj2.y && obj1.z != obj2.z && obj1.l != obj2.l; 
    }
};


/*
struct hash_pair { 
    template <class T1, class T2> 
    size_t operator()(const std::pair<T1, T2>& p) const
    { 
        auto hash1 = std::hash<T1>{}(p.first); 
        auto hash2 = std::hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
}; 
#define mpu unordered_map<pu, double, hash_pair>
*/

#define dtu deque<tu>
#define dtc deque<tc>
#define mtu map<tu,uint64>

inline bool operator==(const tu& lhs, const tu& rhs)
{
    return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z));
}

//typedef std::map<tu,uint64> mtu;


/*
 * GLOBAL CONFIG
 */
//struct CFG
//{
//	uint64 test;
//};

inline bool operator <(const tu& a, const tu& b)
{
    return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z);
};

using namespace std;




template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I)<<1) {
    static const char* digits = "0123456789abcdef";
    std::string rc(hex_len,'0');
    for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}

inline void mpp2mppd(mpp* prev, mppd* next)
{
	mpp::iterator it;
	for (it = prev->begin(); it != prev->end(); ++it)
		(*next)[it->first] = pu(it->second.first, it->second.second);
}

inline string invert_rgb(const string rgb)
{
	stringstream ss;//, ss2;
	int color;
	string new_rgb;

	if (rgb.size() != 6)
	{
		cout << "Common.h: invert_rgb(): incorrect rgb: " << rgb << endl;
		exit(1);
	}

	new_rgb = rgb;
	ss << hex << rgb.substr(0,2);
	ss >> color;
	color = 255-color;
	//ss.str(""); ss.clear();
	//ss << hex << color;
	//new_rgb.replace(0,2,ss.str());
	new_rgb.replace(0,2,n2hexstr(color, 2));

	ss.str(""); ss.clear();
	ss << hex << rgb.substr(2,2);
	ss >> color;
	color = 255-color;
	//ss.str(""); ss.clear();
	//ss << hex << color;
	//new_rgb.replace(2,2,ss.str());
	new_rgb.replace(2,2,n2hexstr(color, 2));

	ss.str(""); ss.clear();
	ss << hex << rgb.substr(4,2);
	ss >> color;
	color = 255-color;
	//ss.str(""); ss.clear();
	//ss << hex << color;
	//new_rgb.replace(4,2,ss.str());
	new_rgb.replace(4,2,n2hexstr(color, 2));

	return new_rgb;
}

inline string hsv2rgb(const double h, const double s = 1, const double v = 1)
{
    double      hh, p, q, t, ff;
    long        i;
    double r, g, b;
	char rgb[8];
    /*
    if(s <= 0.0) {       // < is bogus, just shuts up warnings
        r = v;
        g = v;
        b = v;
        return res;
    }
    */
    hh = h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch(i) {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    case 5:
    default:
        r = v;
        g = p;
        b = q;
        break;
    }

    snprintf(rgb, 8, "#%.2x%.2x%.2x", (int)(r*255), (int)(g*255), (int)(b*255));
	return string(rgb);
}

inline uint64 lines_num(const char* fname)
{
	uint64 res = 0;
	string str;
	ifstream f(fname);

	while (getline(f, str))
		++res;
	return res;
}



inline double LogE(mpq_t m_op)
{
	// log(a/b) = log(a) - log(b)
	// And if a is represented in base B as:
	// a = a_N B^N + a_{N-1} B^{N-1} + ... + a_0
	// => log(a) \approx log(a_N B^N)
	// = log(a_N) + N log(B)
	// where B is the base; ie: ULONG_MAX

	static double logB = log(ULLONG_MAX);

	// Undefined logs (should probably return NAN in second case?)
	if (mpz_get_ui(mpq_numref(m_op)) == 0 || mpz_sgn(mpq_numref(m_op)) < 0)
		return -INFINITY;

	// Log of numerator
	double lognum = log(mpq_numref(m_op)->_mp_d[abs(mpq_numref(m_op)->_mp_size) - 1]);
	lognum += (abs(mpq_numref(m_op)->_mp_size)-1) * logB;

	// Subtract log of denominator, if it exists
	if (abs(mpq_denref(m_op)->_mp_size) > 0)
	{
		lognum -= log(mpq_denref(m_op)->_mp_d[abs(mpq_denref(m_op)->_mp_size)-1]);
		lognum -= (abs(mpq_denref(m_op)->_mp_size)-1) * logB;
	}
	return lognum;
}

inline double LogE(mpz_t mpz)
{
	double res;
	mpq_t m_op;

	mpq_init(m_op);
	mpq_set_num(m_op, mpz);
	res = LogE(m_op);
	mpq_clear(m_op);
	return res;
}

inline bool point_in_3D_surface(tu p, tu p1, tu p2, tu p3, double EPS)
{
	int x, y, z, x1, y1, z1, x2, y2, z2, x3, y3, z3;
	double A, B, C, D, dist;

	x = p.x; y = p.y; z = p.z;
	x1 = p1.x; y1 = p1.y; z1 = p1.z;
	x2 = p2.x; y2 = p2.y; z2 = p2.z;
	x3 = p3.x; y3 = p3.y; z3 = p3.z;

	A = (y2-y1)*(z3-z1)-(z2-z1)*(y3-y1);
	B = (x2-x1)*(z3-z1)-(x3-x1)*(z2-z1);
	C = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
	D = -x1 * A + y1 * B - z1 * C;

	dist = sqrt(A*A + B*B + C*C);
//cout << A << " " << B << " " << C << " " << D << endl;
//cout << dist << endl;
//cout << A/dist << " " << B/dist << " " << C/dist << " " << D/dist << endl;

	A /= dist; B /= dist; C /= dist; D /= dist;

	return ( abs(A*x - B*y + C*z + D) <= EPS);
}

inline void swap_uints(uint64* x, uint64* y)
{
	uint64 v;

	v = *x; *x = *y; *y = v;
}

inline void int_sums(uint64 max, const char* fname)
{
	uint64 i, sum;
	fstream f;

	f.open(fname, fstream::out | fstream::trunc);
	sum = 0;
	for(i = 0; i <= max; ++i)
	{
		sum += i;
		f << sum << endl;
	}
	f.close();
}

inline bool is_int(const char* c)
{
	int i, j;
	const char digits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	bool res;

	res = false;
	i = 0;
	while (c[i] != 0)
	{
		res = false;
		for (j = 0; j < 10; ++j)
			if (c[i] == digits[j])
			{
				res = true;
				break;
			}
		if (res == false)
			break;
		++i;
	}
	return res;
}

inline bool is_double(const char* c)
{
	int i, j;
	const char digits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	bool res, period_found;

	res = false;
	period_found = false;

	i = 0;
	while (c[i] != 0)
	{
		res = false;
		for (j = 0; j < 10; ++j)
			if (c[i] == digits[j])
			{
				res = true;
				break;
			}
		if ((c[i] == '.') && !period_found)
		{
			period_found = true;
			res = true;
		}

		if (res == false)
			break;
		++i;
	}
	return res;
}

inline bool strings_equal(const char* c1, const char* c2)
/*
  Checks if c1 == c2
 */
{
	int i;

	i = 0;
	while ((c1[i] == c2[i]) && (c1[i] != 0))
		++i;
	if ((c1[i] == c2[i]) && (c1[i] == 0))
		return true;
	else
		return false;
}


inline void diags_diff(const char* in1, const char* in2, const char* out)
{
	int i, j, max, diff, min_diff, num;
	vector<int> v_, v2;
	vector <vector<int> > v1;
	ifstream f1, f2;
	ofstream f3;

	string str1, str2;

	f1.open(in1, ifstream::in);
	f2.open(in2, ifstream::in);
	f3.open(out, ofstream::out);

	max = 0;
	num = 0;
	while (getline(f1, str1) && getline(f2, str2))
	{
		istringstream is1(str1);
		string substr;

		//Формируется двумерный массив диаграмм v1 из первого файла
		while(std::getline(is1, substr, ','))
		{
			//cout << substr << endl;
			istringstream is11(substr);
			while (is11 >> i)
				v_.push_back(i);
			v1.push_back(v_);
			v_.clear();
		}
		v_.clear();

		//Массив диаграмм v2 из второго файла
		istringstream is2 (str2);
		while (is2 >> i)
			v2.push_back(i);

		//считаем разницу

		min_diff = INT_MAX;
		//цикл по диаграммам из первого файла

		for (i = 0; (unsigned int)i < v1.size(); ++i)
		{
			diff = 0;
			for (j = 0; (unsigned int)j < min(v1.at(i).size(), v2.size()); ++j)
			{
				diff += abs(v1.at(i).at(j) - v2.at(j));
			}
			if (v1.at(i).size() > v2.size())
				for (j = v2.size(); (unsigned int)j < v1.at(i).size(); ++j)
					diff += abs(v1.at(i).at(j));
			else
				for (j = v1.at(i).size(); (unsigned int)j < v2.size(); ++j)
					diff += abs(v2.at(j));
			if (diff < min_diff)
				min_diff = diff;
		}

		diff = min_diff;

		diff /= 2;

		f3 << diff << endl;

		if (diff > max)
			max = diff;
		if (diff > 0)
			++num;

		v1.clear();
		v2.clear();

	}
	f3 << "MAX: " << max;
	f3 << "\nNUM_OF_DIFF: " << num;
	f1.close();
	f2.close();
	f3.close();
}

inline void double_diff(const char* in1, const char* in2, const char* out)
{
	fstream fin1, fin2, fout;

	fin1.open(in1, fstream::in);
	fin2.open(in2, fstream::in);
	fout.open(out, fstream::out | fstream::trunc);

	double d1, d2;
	uint64 i1, i2;

	while(fin1 >> i1)
	{
		fin1 >> d1;
		fin2 >> i2;
		fin2 >> d2;
		fout << i1 << " " << std::fixed << std::setprecision(20) << d1 - d2 << endl;
	}

	fin1.close();
	fin2.close();
	fout.close();

	/*
	FILE *f1, *f2, *f3;

	double d1, d2, d3;
	uint64 i1, i2;

	f1 = fopen(in1, "r");
	f2 = fopen(in2, "r");
	f3 = fopen(out, "w");

	while (!feof(f1))
	{
		fscanf(f1, "%llu %.8f \n", &i1, &d1);
		fscanf(f2, "%llu %.8f \n", &i2, &d2);
		d3 = d1 - d2;
		fprintf(f3, "%llu %8.8f\n", i1, d3);
	}

	fclose(f1);
	fclose(f2);
	fclose(f3);
	*/
}

inline void diff_in_one_file(const char* fin_name, const char* fout_name)
{
	fstream fin, fout;

	uint64 i;
	double d, d_prev;

	fin.open(fin_name, fstream::in);
	fout.open(fout_name, fstream::out | fstream::trunc);

	fin >> i;
	fin >> d_prev;

	while (fin >> i)
	{
		fin >> d;
		fout << i << " " << std::fixed << std::setprecision(20) << d-d_prev << endl;
		d_prev = d;
	}

	fin.close();
	fout.close();
}

inline void normdim3Ddiff(const char* fin_name, const char* fout_name)
{
	const double SECOND = 0.66;

	fstream fin, fout;

	uint64 n, i;
	double nd, prev_nd;

	fin.open(fin_name, fstream::in);
	fout.open(fout_name, fstream::out | fstream::trunc);

	n = 1;
	fin >> i; fin >> i;	fin >> prev_nd;	fin >> prev_nd;

	prev_nd = prev_nd/pow(n, 2.0/3.0) - (2.0/3.0) * pow(n, 1.0/3.0) * log(n) + SECOND * pow(n, 1.0/3.0);
	fout << std::fixed << std::setprecision(20) << prev_nd << endl;

	while (fin >> i)
	{
		++n;
		fin >> i; fin >> nd; fin >> nd;

		nd = nd/pow(n, 2.0/3.0) - (2.0/3.0) * pow(n, 1.0/3.0) * log(n) + SECOND * pow(n, 1.0/3.0);
		fout << std::fixed << std::setprecision(20) << (nd - prev_nd) * pow(n, 2.0/3.0) << endl;
		prev_nd = nd;
	}

	fin.close();
	fout.close();
}

inline void mpz_diff(const char* in1, const char* in2, const char* out)
{
	FILE *f1, *f2, *f3;
	mpz_t mpz1, mpz2, mpz3;
	uint64 total, zero, plus, minus;
	int check;

	mpz_init(mpz1);
	mpz_init(mpz2);
	mpz_init(mpz3);

	f1 = fopen(in1, "r");
	f2 = fopen(in2, "r");
	f3 = fopen(out, "w");

	total = 0; zero = 0; plus = 0; minus = 0;

	while (mpz_inp_str(mpz1, f1, 0) && mpz_inp_str(mpz2, f2, 0))
	{
		mpz_sub(mpz3, mpz1, mpz2);
		mpz_out_str(f3, 0, mpz3);
		fprintf(f3, "\n");

		check = mpz_sgn(mpz3);
		if (check == 0)
			++zero;
		if (check > 0)
			++plus;
		if (check < 0)
			++minus;
		++total;
	}

	cout << "Statistics:\n";
	cout << "Total number of values = " << total << endl;
	cout << "+ " << plus << " / " << total << " (" << plus*100./total << "%)" << endl;
	cout << "- " << minus << " / " << total << " (" << minus*100./total << "%)" << endl;
	cout << "0 " << zero << " / " << total << " (" << zero*100./total << "%)" << endl;

	mpz_clear(mpz1);
	mpz_clear(mpz2);
	mpz_clear(mpz3);
	fclose(f1);
	fclose(f2);
	fclose(f3);
}

inline double calc_stddev(vi vec, double mean)
{
	vd diff(vec.size());
	transform(vec.begin(), vec.end(), diff.begin(),
		bind2nd(minus<double>(), mean));
	double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	return sqrt(sq_sum / vec.size());
}

inline void corners2rows(const char* input_f, const char* output_f)
//Converts corners of max.diags from file to rows
{
	uint64 num, i, j;
	long long int prev_ind;
	int odd;
	//vi row_nums, widths;
	map<uint64, uint64> values;
	map<uint64, uint64>::iterator map_it;
	vector<string>::iterator str_it;
	bool validstr;
	size_t pos;

	vector<string> vec_corn, vec_maxc;

	vector< map<uint64, uint64> > value_diffs;

	value_diffs.resize(130);

	ifstream inp(input_f, ios::in);
	ofstream out(output_f, ios::out);

	validstr = false;

	if (inp.good())
	{
	    string str;

	    out << "{ ";
	    //Считываем строчку
	    while(getline(inp, str))
	    {
	    	if ((str.find("max-diag") == string::npos) && !validstr)
	    		continue;
	    	if (str.find("max-c") != string::npos)
	    	{
		    	pos = str.find("max-c =");
		    	str.erase(pos, 7);
		    	str.erase (std::remove(str.begin(), str.end(), ' '), str.end());
		    	vec_maxc.push_back(str);

	    		validstr = false;
	    		continue;
	    	}

	    	pos = str.find("max-diag =");
	    	if (pos != string::npos)
	    		str.erase(pos, 10);

	    	str.erase (std::remove(str.begin(), str.end(), '('), str.end());
	    	str.erase (std::remove(str.begin(), str.end(), ')'), str.end());

	    	if (validstr == false)
	    		vec_corn.push_back(str);
	    	else
	    		vec_corn.back() = vec_corn.back() + str;
	    	validstr = true;
	    }

	   for (vector<string>::iterator it = vec_maxc.begin(); it != vec_maxc.end(); ++it)
		   cout << *it << ", ";

	    for (str_it = vec_corn.begin(); str_it != vec_corn.end(); ++str_it)
	    {
	        istringstream ss(*str_it);
	        odd = 1;

			//Значения из строчки ==> row_nums и widths
			while(ss >> num)
			{
				if (odd == 1)
					i = num;
				else
					values[i] = num+1;
				++odd %= 2;
			}

			prev_ind = -1;

			out << "{";
			for (map_it = values.begin(); map_it != values.end(); ++map_it)
			{
				i = map_it->first;
				for (j = i - prev_ind; j != 0; --j)
				{
					out << map_it->second;
					if (  !(    (j == 1) && (std::next (map_it,1) == values.end())     )   )
						out << ", ";
				}
				prev_ind = i;
			}
			out << "}";
			if (str_it != vec_corn.end() - 1)
				out << ", ";

			values.clear();
	    }
	    out << " };";
	}
}

inline void sumoflogs(const char* fname_in, const char* fname_out)
{
	fstream fin, fout;
	double d, sum;
	uint64 n;

	fin.open(fname_in, fstream::in);
	fout.open(fname_out, fstream::out | fstream::trunc);

	sum = 0;
	n = 1;
	while (fin >> d)
	{
		sum -= log(d/n);
		fout << log(sum/n) << endl;
		/*
		sum += log(d);
		//fout << (sum / (n*6.55859))/log(n) << endl;
		fout << (sum / n) / log(n) << endl;
		//fout << sum / log(n) << endl;
		*/
		++n;
	}
	fin.close();
	fout.close();
}

inline void vec_to_file(vector<uint64> vec, const char* fname)
//Prints vector to file
{
	FILE* f;
	vector<uint64>::iterator it;

	f = fopen(fname, "w");
	for (it = vec.begin(); it != vec.end(); ++it)
		fprintf(f, "%llu ", *it);
	fclose(f);
}

inline void two_vec_to_file(vector<uint64> vec1, vector<uint64> vec2, const char* fname)
//Prints vector to file
{
	FILE* f;
	uint i;

	if (vec1.size() != vec2.size())
		return;

	f = fopen(fname, "w");

	fprintf(f, "Initial success | Success after shaking\n");
	for (i = 0; i != vec1.size(); ++i)
		fprintf(f, "%llu %llu\n", vec1.at(i), vec2.at(i));

	fclose(f);
}

inline uint64 bit_num(uint64 x)
/*
	���������� ����� ��� � x
*/
{
	//vector<int> PART_NUM = { 1, 2, 3, 4, 5 };


	uint64 res = (uint64)(ceil(log(x*1.) * 1. / log(2.)));
	return res;
}

inline double rand_0_1()
/*
	��������� ������������ [0..1]
*/
{
	return (double)(rand()) / RAND_MAX;
}

inline uint64 rand_0_to_n_1(uint64 n)
/*
	��������� ����� [0..n-1]
*/
{
	return (uint64)(rand()*n*1. / ((uint64)RAND_MAX + 1));
}

inline void print_duration(time_t start, time_t stop)
{
	int hours = (int)((stop - start) / 3600);
	int minutes = (int)(((stop - start) - hours * 3600) / 60);
	int seconds = (int)((stop - start) - hours * 3600 - minutes * 60);

	printf("#time %dh:%2dm:%2ds\n", hours, minutes, seconds);
}

inline int num_digits(uint64 number)
/*
���������� ���������� ���� � number
*/
{
	int digits = 0;
	do
	{
		number /= 10;
		digits++;
	} while (number);
	return digits;
}
/*
//BAD fact, because n may be too big for uint64 result.

inline uint64 fact(uint64 n)
{
	return (n == 1 || n == 0) ? 1 : fact(n - 1) * n;
}
*/

inline bool comp(uint64 i, uint64 j) { return (i > j); }

inline uint64 udiff(uint64 x1, uint64 x2)
{
	if (x1 == x2)
		return 0;
	if (x1 > x2) 
		return x1 - x2;
	if (x2 > x1)
		return x2 - x1;
	return 0;
}

inline void str_to_rows(string str_rows, vvi& rows)
{
	std::size_t prev = 0, pos;
	string layer;
	vi row;
	uint64 i;

	rows.clear();
	do
	{
		pos = str_rows.find_first_of(",", prev);

		if (pos == std::string::npos)
			layer = str_rows.substr(prev, std::string::npos);
		else
			if (pos > prev)
				layer = str_rows.substr(prev, pos-prev);
		prev = pos + 1;

		istringstream is (layer);
		row.clear();
		while (is >> i)
			row.push_back(i);
		rows.push_back(row);
	}
	while (pos != std::string::npos);
}

inline void calc_fractions(double x, vi& noms, vi& dens)
/*
	Continuous fraction of input x. 
	Returns vectors of nominators and denominators (from the best to the worst fit).
*/
{
	const int MAX = 10;
	const double eps = 1e-9;

	//long p[MAX], q[MAX], a[MAX], len;
	uint64 p[MAX], q[MAX], a[MAX], len;

	int i, n; 

	//long cp, cq;
	uint64 cp, cq;

	//The first two convergents are 0/1 and 1/0
	p[0] = 0; q[0] = 1;
	p[1] = 1; q[1] = 0;
	//The rest of the convergents (and continued fraction)
	for(i = 2; i < MAX; ++i) 
	{
		a[i] = lrint(floor(x));
		p[i] = a[i]*p[i-1] + p[i-2];
		q[i] = a[i]*q[i-1] + q[i-2];
		//printf("%ld:  %ld/%ld\n", a[i], p[i], q[i]);
		
		if (i > 2)
		{
			noms.push_back(p[i]);
			dens.push_back(q[i]);		
		}

		len = i;
		if(fabs(x-a[i])<eps) return;
		x = 1.0/(x - a[i]);
	}

}



inline void fractions_fitting(const vvi noms, const vvi dens, vvpu& res, uint64 diag_num = 0, vi frac_numbers = vi(0))
/*
	Tries to choose the fractions so their sum = 1
*/
{
	uint64 i;
	uint64 nom, den;	
	static mpq_t rat, sum;
	vpu solution;

	if (noms.size() != dens.size())
		return;
	if (diag_num == 0)
	{
		mpq_init(sum);
		mpq_init(rat);
	}
	if (diag_num >= noms.size())
	{
		mpq_set_ui(sum, 0, 1);
		for (i = 0; i < frac_numbers.size(); ++i)
		{
			nom = noms.at(i).at(frac_numbers.at(i));
			den = dens.at(i).at(frac_numbers.at(i));
			mpq_set_ui(rat, nom, den);
			mpq_add(sum, sum, rat);
		}

		if (mpq_cmp_ui(sum, 1, 1) == 0)
		{
			for (i = 0; i < frac_numbers.size(); ++i)
			{
				nom = noms.at(i).at(frac_numbers.at(i));
				den = dens.at(i).at(frac_numbers.at(i));
				solution.push_back(pu(nom, den));
			}	
			res.push_back(solution);
		}
		return;
	}
	frac_numbers.resize(noms.size());

	for (i = 0; i < noms.at(diag_num).size(); ++i)
	{
		if (diag_num == 0)
			mpq_set_ui(sum, noms.at(diag_num).at(i), dens.at(diag_num).at(i));

		frac_numbers.at(diag_num) = i;
		fractions_fitting(noms, dens, res, diag_num + 1, frac_numbers);
	}

	if (diag_num == 0)
	{
		mpq_clear(sum);
		mpq_clear(rat);
	}
}

inline bool is_prime(uint64 x)
{
	uint64 i;

	if (x == 1)
		return false;
	for(i = 2; i <= x / 2; ++i)
		if(x % i == 0)
			return false;
	return true;
}

inline bool is_prime_mpz(mpz_t x)
//tmp is needed for comparison purposes (it is too costly to init it within this function).
{
	static mpz_t i, tmp;

	mpz_init(i);
	mpz_init(tmp);

	if (!mpz_cmp_ui(x, 1))
		return false;

	mpz_fdiv_q_ui(tmp, x, 2);
	for(mpz_set_ui(i, 2); mpz_cmp(i, tmp) <= 0; mpz_add_ui(i, i, 1))
	{

		mpz_mod(tmp, x, i);
		if (!mpz_cmp_ui(tmp, 0))
			return false;

		mpz_fdiv_q_ui(tmp, x, 2);
	}
	return true;
}

//inline int GCD(int a, int b)
////naim.obsch.kratnoe
//{
//	while (a - b != 0)
//		a > b ? a -= b : b -= a;
//	return a;
//}


// A function to print all prime factors of a given number n 
inline di primeFactors(uint64 n) 
{ 
	di res;

	// Print the number of 2s that divide n 
	while (n%2 == 0) 
	{ 
		//printf("%d ", 2); 
		res.push_front(2);
		n = n/2; 
	} 

	// n must be odd at this point. So we can skip 
	// one element (Note i = i +2) 
	for (uint64 i = 3; i <= sqrt(n); i = i+2) 
	{ 
		// While i divides n, print i and divide n 
		while (n%i == 0) 
		{ 
			//printf("%d ", i); 
			res.push_front(i);
			n = n/i; 
		} 
	} 

	// This condition is to handle the case when n 
	// is a prime number greater than 2 
	if (n > 2) 
		//printf ("%d ", n); 
		res.push_front(n);

	return res;
} 


inline int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
inline long long int findlcm(int arr[], int n)
{
    // Initialize result
    long long int ans = arr[0];
 
    // ans contains LCM of arr[0], ..arr[i]
    // after i'th iteration,
    for (int i = 1; i < n; i++)
        ans = (((arr[i] * ans)) /
                (gcd(arr[i], ans)));
 
    return ans;
}


#endif
