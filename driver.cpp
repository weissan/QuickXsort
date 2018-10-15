/******************************************************************************
* driver.cpp
*
* Driver to benchmark sorting algorithms. Comparison and move measurements do NOT work for all algorithms.
* Modified version of sort-driver.c++ from http://www.diku.dk/~jyrki/Myris/Kat2014S.html
******************************************************************************
* Modified (more test cases etc.) by
* Copyright (C) 2016 Stefan Edelkamp <edelkamp@tzi.de>
* Copyright (C) 2016 Armin Weiß <armin.weiss@fmi.uni-stuttgart.de>
******************************************************************************
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************
*
* Copyright (C) 2014 Amr Elmasry
* Copyright (C) 2014 Jyrki Katajainen
* Copyright (C) 2014 Max Stenmark
*
*****************************************************************************/


#if ! defined(MAXSIZE)
#define MAXSIZE (1024 * 1024 * 1024)
#endif

//#define BLOCKSIZE 4
//Maximal number of bytes to sort
#ifndef PI
#ifndef BYTESTOSORT
#ifdef SELECT
#define BYTESTOSORT (256 * 1024 * 1024)
#else 
#define BYTESTOSORT (128 * 1024 * 1024)
#endif
#endif
#else
#define BYTESTOSORT (32 * 1024 * 1024)
#endif

#include <algorithm>  	
#include <functional> 	
#include <iostream>   	
#include <iterator>   	
#include <string> 
#include <cmath> 
#include <random>
#include <string>
#include <chrono>
#include <typeinfo>
#include <time.h>



//to print types
#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define ALG_NAME STR(NAME)


#ifndef TYPE
	#ifdef MEASURE_COMPARISONS
		#define TYPE blubint
	#else
		#ifdef INDEXSORT
			#define TYPE IndexRecord
		#else
			#define TYPE int32_t
		#endif
	#endif
#endif


extern int ilogb(double) throw();

//for counting numbers of calls to std::partial_sort
#define PARTIAL_SORT_COUNT
long long volatile partial_sort_count = 0;

#ifndef IS_THRESH
#define IS_THRESH (1<<4)
#endif // !1IS_THRESH


#ifdef MEASURE_SCANS
long long volatile scanned_elements = 0;
#endif // MEASURE_SCANS

//measure comparisons does NOT work for all algorithms
#ifdef MEASURE_COMPARISONS

long long volatile comparisons = 0;

template<typename T>
class counting_comparator {
public:

    using first_argument_type = T;
    using second_argument_type = T;
    using result_type = bool;

    bool operator()(T const& a, T const& b) const {
        ++comparisons;
        //return a < b;
        return T::compare(a, b);
    }
};






template<typename T>
class vector_comparator {
public:
    using first_argument_type = T;
    using second_argument_type = T;
    using result_type = bool;
    bool operator()(std::vector<T> const& a, std::vector<T> const& b) const {
        if (a.size() < b.size())
            return true;
        if (b.size() < a.size())
            return false;
        if (a.size() == 0) // also b.size() == 0
            return false;
        return a[0] < b[0];
    }
};
#endif

bool already_partitioned = false;
#include "algorithm.h++"



class Rational {
private:
    int numerator;
    unsigned int denominator;
public:
    Rational(int v) : numerator(v), denominator(1) {};
    Rational() : numerator(0), denominator(1) {};
    double value(void) { return (double)numerator / (double)denominator; }
    bool operator<(const Rational &b) const { return numerator*(long long int) b.denominator < b.numerator*(long long int)denominator; }
    bool operator>(const Rational &b) const { return numerator*(long long int) b.denominator > b.numerator*(long long int)denominator; }
    bool operator<=(const Rational &b) const { return numerator*(long long int) b.denominator <= b.numerator*(long long int)denominator; }
    bool operator!=(const Rational &b) const { return numerator*(long long int) b.denominator != b.numerator*(long long int)denominator; }
    bool operator==(const Rational &b) const { return numerator*(long long int) b.denominator == b.numerator*(long long int)denominator; }
};


class Record {
private:
    int key;
    int blub[20];
public:
    Record(int v) : key(v) { blub[v % 10] = v; };
    Record() : key(0) {};
    int value(void) { return key; }
    bool operator<(const Record &b) const { return key < b.key; }
    bool operator>(const Record &b) const { return key > b.key; }
    bool operator<=(const Record &b) const { return key <= b.key; }
    bool operator!=(const Record &b) const { return key != b.key; }
    bool operator==(const Record &b) const { return key == b.key; }
};





class Logint {
private:
    int key;
public:
    Logint(int v) : key(v) {};
    Logint() : key(0) {};
    int value(void) { return key; }
    bool operator<(const Logint &b) const { return log((float)key + 1.0) < log((float)b.key + 1.0); }
    bool operator>(const Logint &b) const { return log((float)key + 1.0) > log((float)b.key + 1.0); }
    bool operator<=(const Logint &b) const { return log((float)key + 1.0) <= log((float)b.key + 1.0); }
    bool operator!=(const Logint &b) const { return log((float)key + 1.0) != log((float)b.key + 1.0); }
    bool operator==(const Logint &b) const { return log((float)key + 1.0) == log((float)b.key + 1.0); }
};


class blubint {
private:
    int key;
public:
    blubint(int v) : key(v) {};
    blubint() {};
    int value(void) { return key; }
    static bool compare(const blubint & a, const blubint & b)
    {
        return (a.key < b.key);
    }

    	bool operator<(const blubint &b) const { return (key < b.key); }
		bool operator!=(const blubint &b) const { return (key != b.key); }
    /*    bool operator>(const Logint &b) const { return log((float)key +1.0) > log((float)b.key+1.0); }
        bool operator<=(const Logint &b) const { return log((float)key +1.0) <= log((float)b.key+1.0); }
        bool operator!=(const Logint &b) const { return log((float)key +1.0) != log((float)b.key+1.0);  }
        bool operator==(const Logint &b) const { return log((float)key +1.0) == log((float)b.key+1.0);  }*/
};


/*
//To Do
class String {
private:
    int numerator;
    unsigned int denominator;
public:
    String(int v) : numerator(v), denominator(1) {};
    String() : numerator(0), denominator(1) {};
    double value(void) { return (double) numerator / (double) denominator; }
    bool operator<(const String &b) const { return numerator*(long long int) b.denominator < b.numerator*(long long int)denominator; }
    bool operator>(const String &b) const { return numerator*(long long int) b.denominator > b.numerator*(long long int)denominator; }
    bool operator<=(const String &b) const { return numerator*(long long int) b.denominator <= b.numerator*(long long int)denominator; }
    bool operator!=(const String &b) const { return numerator*(long long int) b.denominator != b.numerator*(long long int)denominator;  }
};
*/


class PointerRecord {
private:
    int* blub;
public:
    PointerRecord(int v) { blub = new int[10]; blub[0] = v; };
    PointerRecord() { blub = new int[10]; blub[0] = 0; };
    //	PointerRecord(PointerRecord&& other) :blub(0) {std::swap(other.blub, blub);};
    PointerRecord(const PointerRecord &b) { blub = new int[10]; blub[0] = b.blub[0]; }
    ~PointerRecord() { if (blub != 0) delete[] blub; }
    //C++11
/*	PointerRecord& operator=(PointerRecord&& other)
    {
        std::swap(other.blub, blub);
        return *this;
    }*/
    PointerRecord& operator=(PointerRecord&& other)
    {
        std::swap(other.blub, blub);
        return *this;
    }
    int value(void) { return blub[0]; }
    bool operator<(const PointerRecord &b) const { return blub[0] < b.blub[0]; }
    bool operator>(const PointerRecord &b) const { return blub[0] > b.blub[0]; }
    bool operator<=(const PointerRecord &b) const { return blub[0] <= b.blub[0]; }
    bool operator!=(const PointerRecord &b) const { return blub[0] != b.blub[0]; }
};


#ifdef INDEXSORT


static std::vector<std::vector<Record> > data;
class IndexRecord {
private:
    int vector_index;
	int index;
public:
	
	
    IndexRecord(int i, int vec_ind) { index = i; vector_index = vec_ind; };
    IndexRecord() { index = data[0].size(); vector_index = 0; };
    //	IndexRecord(IndexRecord&& other) :blub(0) {std::swap(other.blub, blub);};
    IndexRecord(const IndexRecord &b) { index = b.index; vector_index = b.vector_index; }
    
    //C++11
	IndexRecord& operator=(IndexRecord&& other)
    {
        index = other.index;
		vector_index = other.vector_index;
        return *this;
    }
	
	IndexRecord& operator=(const IndexRecord& other)
    {
        index = other.index;
		vector_index = other.vector_index;
        return *this;
    }
	
    int value(void) { return data[vector_index][index].value(); }
    bool operator<(const IndexRecord &b) const { return data[vector_index][index] < data[vector_index][b.index]; }
    bool operator>(const IndexRecord &b) const { return data[vector_index][index] > data[vector_index][b.index]; }
    bool operator<=(const IndexRecord &b) const { return data[vector_index][index] <= data[vector_index][b.index]; }
    bool operator!=(const IndexRecord &b) const { return data[vector_index][index] != data[vector_index][b.index]; }
};

#endif


#ifndef VECTORSIZE
#define VECTORSIZE 10
#endif
class Vector {
private:
    double components[VECTORSIZE];
public:
    Vector(int v) { for (int j = 0; j < VECTORSIZE; j++) components[j] = v; };
    Vector() {};
    double value(void) {
        double abs_this = 0;
        for (int j = 0; j < VECTORSIZE; j++) {
            abs_this += components[j] * components[j];
        }
        return abs_this;
    }
    //compare Euclidean norm
    bool operator<(const Vector &b) const {
        double abs_this = 0, abs_b = 0;
        for (int j = 0; j < VECTORSIZE; j++) {
            abs_this += components[j] * components[j];
            abs_b += b.components[j] * b.components[j];
        }
        return abs_this < abs_b;
    }
    bool operator>(const Vector &b) const {
        double abs_this = 0, abs_b = 0;
        for (int j = 0; j < VECTORSIZE; j++) {
            abs_this += components[j] * components[j];
            abs_b += b.components[j] * b.components[j];
        }
        return abs_this > abs_b;
    }
    bool operator<=(const Vector &b) const {
        double abs_this = 0, abs_b = 0;
        for (int j = 0; j < VECTORSIZE; j++) {
            abs_this += components[j] * components[j];
            abs_b += b.components[j] * b.components[j];
        }
        return abs_this <= abs_b;
    }
    bool operator!=(const Vector &b) const {
        double abs_this = 0, abs_b = 0;
        for (int j = 0; j < VECTORSIZE; j++) {
            abs_this += components[j] * components[j];
            abs_b += b.components[j] * b.components[j];
        }
        return abs_this != abs_b;
    }
    bool operator==(const Vector &b) const {
        double abs_this = 0, abs_b = 0;
        for (int j = 0; j < VECTORSIZE; j++) {
            abs_this += components[j] * components[j];
            abs_b += b.components[j] * b.components[j];
        }
        return abs_this == abs_b;
    }
};



//measure moves does NOT work for all algorithms
#ifdef MEASURE_MOVES
long long volatile moves = 0;

template<typename T>
class move_counter {
private:

    T datum;

    move_counter(move_counter const&) = delete;
    move_counter& operator=(move_counter const&) = delete;

public:

    explicit move_counter()
        : datum(0) {
        moves += 1;
    }

    template<typename number>
    explicit move_counter(number x = 0)
        : datum(x) {
        moves += 1;
    }

    move_counter(move_counter&& other) {
        datum = std::move(other.datum);
        moves += 1;
    }

    move_counter& operator=(move_counter&& other) {
        datum = std::move(other.datum);
        moves += 1;
        return *this;
    }

    operator T() const {
        return datum;
    }

    template<typename U>
    friend bool operator<(move_counter<U> const&, move_counter<U> const&);

    template<typename U>
    friend bool operator==(move_counter<U> const&, move_counter<U> const&);

};

template<typename T>
bool operator<(move_counter<T> const& x, move_counter<T> const& y) {
    return x.datum < y.datum;
}

template<typename T>
bool operator==(move_counter<T> const& x, move_counter<T> const& y) {
    return x.datum == y.datum;
}

#endif


void usage(int argc, char **argv) {
    std::cerr << "Usage: " << argv[0]
        << " <N> <b c d i m n o p q r s t u v w z> <seed> "
        << std::endl;
    exit(1);
}

int main(int argc, char** argv) {

#ifdef MEASURE_MOVES

    using element = move_counter<int>;
    using C = std::less<element>;



#else
#ifdef TYPE
    using element = TYPE;
#else
    using element = int;
#endif

#ifdef MEASURE_COMPARISONS
    using C = counting_comparator<element>;
#else
    using C = std::less<element>;
    //using C = counting_comparator<element>; 
#endif
#endif

    unsigned int N = 15;
    unsigned int inversions = 0;
    float ratio_unsorted = 0.1;
    unsigned long long int modulo = 2;
    unsigned long long int offset = 0;
    char method = 'i';
    std::string method_string = std::string("random");
    std::string param_string = std::string("0");
    if (argc == 2) {
        N = atoi(argv[1]);
    }
    else if (argc != 3 && argc != 4) {
        usage(argc, argv);
    }
    else {
        N = atoi(argv[1]);
        method = *argv[2];
    }

    if (argc == 5)
    {
        inversions = atoi(argv[3]);
        modulo = inversions;
        offset = inversions;
        //	param_string = std::to_string(atoi(argv[3]));
    }
    else
    {
        inversions = N;
        modulo = (unsigned int)sqrt(N);
        offset = N / 2;
        if (method == 's' || method == 'm' || method == 'o' || method == 'v') {
            param_string = "sqrt(N)";
            modulo = (unsigned int)sqrt(N);
            inversions = (unsigned int)sqrt(N);
        }
        if (method == 'p') {
            param_string = "N";
            inversions = N;
        }
        if (method == 'q') {
            param_string = "NlogN";
            inversions = (int)((double)N*log(N));
        }
        if (method == 'n') {
            param_string = "N/2";
            modulo = N / 2;
        }
    }

    std::random_device rd;
    unsigned long long int seed = 0;
    if (argc == 5)
    {
        seed = atoi(argv[4]);
    }
    else if (argc == 4)
    {
        seed = atoi(argv[3]);
    }
    else
    {
        seed = rd() + time(NULL);
 //       std::cout <<"entropy"<< rd.entropy() << " seed: "<< seed<< " rd: "<< rd()<< std::endl;
    }
	

    if (N < 1 || N > MAXSIZE) {
        std::cerr << "N out of bounds [1.."
            << MAXSIZE
            << "]"
            << std::endl;
        usage(argc, argv);
    };
    switch (method) {
    case 'a':
        method_string = "mo3 killer                ";
        break;
    case 'b':
        method_string = "boolean                   ";
        break;
    case 'c':
        method_string = "constant                  ";
        break;
    case 'd':
        method_string = "decreasing                ";
        break;
    case 'e':
        method_string = "almost sorted  90%        ";
        ratio_unsorted = 0.1;
        break;
    case 'f':
        method_string = "almost sorted  99%        ";
        ratio_unsorted = 0.01;
        break;
    case 'g':
        method_string = "almost sorted  99.9%      ";
        ratio_unsorted = 0.001;
        break;
    case 'h':
        method_string = "merge                     ";
        break;
    case 'i':
        method_string = "increasing                ";
        break;
    case 'm':
        method_string = "modulo                    ";
        break;
    case 'n':
        method_string = "modulo                    ";
        break;
    case 'o':
    case 'p':
    case 'q':
        method_string = "inversions                ";
        break;
    case 'r':
        method_string = "random                    ";
        break;
    case 's':
        method_string = "sawtooth                  ";
        break;
    case 't':
        method_string = "transposition of identity ";
        break;
    case 'u':
        method_string = "step                      ";
        break;
    case 'v':
        method_string = "stairs                    ";
        break;
    case 'w':
        method_string = "eigth_power_mod_power_of_2";
        break;
    case 'z':
        method_string = "squares_mod_power_of_2    ";
        break;
    default:
        std::cerr << "Method not in ['a','d','i','r','e','f','g','h', 'b','m', 's', 'q', 'z', 'c', 't', 'w']" << std::endl;
        usage(argc, argv);
    }


    if ((unsigned long long int)N * sizeof(element) > ((unsigned long long int)1) << 31 || (typeid(element) == typeid(PointerRecord) && (unsigned long long int)N * 200 > ((unsigned long long int)1) << 31) || (typeid(element) == typeid(Logint) && (unsigned long long int)N * 200 > ((unsigned long long int)1) << 31))
        return 0;

#if defined(MEASURE_MOVES)

    moves = 0;

#elif defined(MEASURE_COMPARISONS)

    comparisons = 0;

#endif

#if defined(REPETITIONS)

    int const repetitions = REPETITIONS;

#else


    //for every measurement sort at least almost BYTESTOSORT bytes -- to ensure that the elapsed time is someting around 1 second (this is not always the case, but often)
    unsigned int repetitions = std::max((unsigned int)(BYTESTOSORT / ((unsigned long long int)N * sizeof(element))), (unsigned int)1);
    if (typeid(element) == typeid(PointerRecord))
        repetitions = std::max((unsigned int)(BYTESTOSORT / ((unsigned long long int)N * 100)), (unsigned int)1);
    if (typeid(element) == typeid(Logint))
        repetitions = std::max((unsigned int)(BYTESTOSORT / (10 * (unsigned long long int)N * sizeof(element))), (unsigned int)1);

#endif

    unsigned long long int temp;
    int elements = N;
    unsigned long long int modulo_power_2 = 1 << ((unsigned int)ilogb(elements));
    unsigned long long int offset_zw = modulo_power_2 / 2;

    typedef std::vector<element> tv;
    std::vector<tv> test_data;
    std::vector<tv> test_data_sorted;
    std::mt19937 random_generator(seed);

    //generate test data
//	  std::cout << "Generating " << repetitions << " times " << elements << " elements of test data..." << std::endl;

		
#ifdef INDEXSORT
	test_data.clear();
    test_data.reserve(repetitions);
	
	data.clear();
	data.reserve(repetitions);
	for (unsigned int j = 0; j < repetitions; j++) {
        test_data.push_back(tv());
        test_data_sorted.push_back(tv());
        test_data[j].reserve(elements);
        test_data_sorted[j].reserve(elements);
		
		data.push_back(std::vector<Record>());
		data[j].reserve(elements);
//		std::cout <<j<<std::endl;
		
		for (int i = 0; i < elements; i++) {
			data[j].push_back(Record(i));
		}
//		std::shuffle(data[j].begin(), data[j].end(), random_generator);
		for (int i = 0; i < elements; i++) {
			test_data[j].push_back(element(i,j));
		}
		std::shuffle(test_data[j].begin(), test_data[j].end(), random_generator);
		
		
	}
	// break;
#else
	test_data.clear();
    test_data.reserve(repetitions);
    for (unsigned int j = 0; j < repetitions; j++) {
        test_data.push_back(tv());
        test_data_sorted.push_back(tv());
        test_data[j].reserve(elements);
        test_data_sorted[j].reserve(elements);
        switch (method) {
        case 'a':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i));
                test_data_sorted[j].push_back(element(i));
            }
            if (elements > 250)
            {
                int runlength = 4 * ilogb(elements) + 20;
                test_data[j][0] = 0;
                test_data[j][1] = elements - 2 * runlength - 5;
                test_data[j][elements - 1] = elements - 1;
                test_data[j][elements / 2] = elements - 2;
                test_data[j][elements - 2] = elements - 3;
                test_data[j][elements / 2 - 1] = elements - 4;
                test_data[j][elements - 3] = 1;

                for (int k = 0; k < runlength; k++)
                {
                    test_data[j][elements - 4 - 2 * k] = elements - 5 - 2 * k;
                    test_data[j][elements - 5 - 2 * k] = k + 2;
                    test_data[j][elements / 2 - 2 - k] = elements - 6 - 2 * k;
                }
                int next = elements - 2 * runlength - 5 - 1;
                for (int k = 2; k < elements / 2 - runlength - 1; k++)
                    test_data[j][k] = next--;
                for (int k = elements / 2 + 1; k < elements - 2 * runlength - 3; k++)
                    test_data[j][k] = next--;


                std::shuffle(test_data[j].begin() + 3, test_data[j].begin() + elements / 2 - runlength - 1, random_generator);
                std::shuffle(test_data[j].begin() + elements / 2 + 2, test_data[j].begin() + elements - 2 * runlength - 3, random_generator);
            }
            break;
		case 'b':
            for (int i = 0; i < elements; i++) {
                int el = random_generator() % 2;
                test_data[j].push_back(element(el));
                test_data_sorted[j].push_back(element(el));
            }
            break;
        case 'c':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(1));
                test_data_sorted[j].push_back(element(1));
            }
            break;
        case 'd':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(elements - 1 - i));
                test_data_sorted[j].push_back(element(elements - 1 - i));
            }
            break;
        case 'e':
        case 'f':
        case 'g':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i));
                test_data_sorted[j].push_back(element(i));
            }
            for (int i = 0; i < elements*ratio_unsorted; i++)
            {
                int el = random_generator()% elements;
                unsigned int pos = random_generator() % elements;
                element elmt(el);
                element elmt2(el);
                std::swap(elmt, test_data[j][pos]);
                std::swap(elmt2, test_data_sorted[j][pos]);
            }
            break;
        case 'h':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i));
                test_data_sorted[j].push_back(element(i));
            }
            test_data[j][0] = 0;
            test_data[j][1] = 1;
            for (int k = 1; k < elements / 2; k++)
            {
                test_data[j][k + 1] = 2 * k;
                test_data[j][elements / 2 + k] = 2 * k + 1;
            }
            break;
        case 'i':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i));
                test_data_sorted[j].push_back(element(i));
            }
            break;
        case 'm':
        case 'n':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i % modulo));
                test_data_sorted[j].push_back(element(i % modulo));
            }
            std::shuffle(test_data[j].begin(), test_data[j].end(), random_generator);
            break;
        case 'o':
        case 'p':
        case 'q':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i));
                test_data_sorted[j].push_back(element(i));
            }
            for (unsigned int i = 0; i < inversions; i++) {
                int pos = random_generator() % (elements - 1);
                std::swap(test_data[j][pos], test_data[j][pos + 1]);
                std::swap(test_data_sorted[j][pos], test_data_sorted[j][pos + 1]);
            }
            break;
        case 'r':		
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i));
                test_data_sorted[j].push_back(element(i));
            }
            std::shuffle(test_data[j].begin(), test_data[j].end(), random_generator);
            break;
        case 's':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i % modulo));
                test_data_sorted[j].push_back(element(i % modulo));
            }
            break;
        case 't':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element((i + offset) % elements));
                test_data_sorted[j].push_back(element((i + offset) % elements));
            }
            break;
        case 'u':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i / (elements / 2)));
                test_data_sorted[j].push_back(element(i / (elements / 2)));
            }
            break;
        case 'v':
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element(i / modulo));
                test_data_sorted[j].push_back(element(i / modulo));
            }
            break;
        case 'w':
            for (int i = 0; i < elements; i++) {
                temp = ((long long int)i*(long long int)i) % modulo_power_2;
                temp = ((long long int)temp*(long long int)temp) % modulo_power_2;
                test_data[j].push_back(element((offset_zw + (long long int)temp*(long long int)temp) % modulo_power_2));
                test_data_sorted[j].push_back(element((offset_zw + (long long int)temp*(long long int)temp) % modulo_power_2));
            }
            break;
        case 'z':
            unsigned long long int offset_zw = modulo_power_2 / 2;
            for (int i = 0; i < elements; i++) {
                test_data[j].push_back(element((offset_zw + (long long int)i*(long long int)i) % modulo_power_2));
                test_data_sorted[j].push_back(element((offset_zw + (long long int)i*(long long int)i) % modulo_power_2));
            }
            break;
        }
    }
#endif

    // std::cout << "Start sorting." << std::endl;
#if ! defined(MEASURE_COMPARISONS) && ! defined(MEASURE_MOVES) && ! defined(MEASURE_SCANS)

    auto begin = std::chrono::high_resolution_clock::now();

#endif
#ifdef SELECT
    for (unsigned int j = 0; j < repetitions; ++j) {
        NAME::median(test_data[j].begin(), test_data[j].end(), C());
    }
#else

    for (unsigned int j = 0; j < repetitions; ++j) {
        NAME::sort(test_data[j].begin(), test_data[j].end(), C());
    }
#endif

#if ! defined(MEASURE_COMPARISONS) && ! defined(MEASURE_MOVES) && ! defined(MEASURE_SCANS)

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
#endif

#if ! defined(NDEBUG)

#ifdef SELECT


    std::cout << "Sorting test data..." << std::endl;
    for (unsigned int j = 0; j < repetitions; j++) {
        std::nth_element(test_data_sorted[j].begin(), test_data_sorted[j].begin() + elements / 2, test_data_sorted[j].end(), C());
    }

    for (unsigned int j = 0; j < repetitions; ++j) {
        if (*(test_data_sorted[j].begin() + elements / 2) != *(test_data[j].begin() + elements / 2))
            std::cout << "Median not correct" << std::endl;
    }


#else
    std::cout << "Sorting test data..." << std::endl;
    for (unsigned int j = 0; j < repetitions; j++) {
        std::sort(test_data_sorted[j].begin(), test_data_sorted[j].end(), C());
    }

    for (unsigned int j = 0; j < repetitions; ++j) {
        for (int i = 0; i < elements; i++) {
            if (test_data[j][i] != test_data_sorted[j][i])
            {
                std::cout << "Not sorted correctly:" << i << std::endl;
                break;
            }
        }
    }
#endif

#endif

    double T = double(repetitions) * double(N);
    std::string type_string;
    if (typeid(element) == typeid(uint32_t) || typeid(element) == typeid(int))
        type_string = std::string("int");
    else if (typeid(element) == typeid(Record))
        type_string = std::string("Record");
    else if (typeid(element) == typeid(PointerRecord))
        type_string = std::string("PointerRecord");
    else if (typeid(element) == typeid(double))
        type_string = std::string("double");
    else if (typeid(element) == typeid(Vector))
        type_string = std::string("Vector");
    else if (typeid(element) == typeid(Rational))
        type_string = std::string("Rational");
    else if (typeid(element) == typeid(std::string))
        type_string = std::string("string");
    else if (typeid(element) == typeid(Logint))
        type_string = std::string("Logint");

#if defined(MEASURE_COMPARISONS)
    //prints N, #comparisons/N ,  #comparisons/(NlogN ) ,  (#comparisons-NlogN)/N
    double avg_comp = double(comparisons) / double(repetitions);
    double c = ((double)avg_comp / (N * log2((double)N)));
    double d = ((double)(avg_comp - (N * log2((double)N))) / (double)N);
    std::cout.precision(50);
#ifdef UNDERSAMPLING_TEST
	std::cout << ALG_NAME << ",\t" << N << ",\t" << UNDERSAMPLING_SIZE << ",\t" << avg_comp;
#else
    std::cout << ALG_NAME << ",\t" << N << ",\t" << avg_comp;
#endif
    //  std::cout.precision(3);
    std::cout.precision(6);
    std::cout  << ",\t" << double(comparisons) / T;
    std::cout << ",\t" << c;
    std::cout << ",\t" << d << std::endl;

#elif defined(MEASURE_MOVES)
    //prints N, #moves/N ,  #moves/(NlogN )
    std::cout.precision(5);
    double avg_move = double(moves) / double(repetitions);
    double c = ((double)avg_move / (N * log2((double)N)));
    std::cout << ALG_NAME << "," << N << ',' << avg_move << ',' << double(moves) / T;
    std::cout << ',' << c << std::endl;

#elif defined(MEASURE_SCANS)
    //prints N, #moves/N ,  #moves/(NlogN )
    std::cout.precision(6);
    double avg_scan = double(scanned_elements) / double(repetitions);
    double c = ((double)avg_scan / (N * log2((double)N)));
    if(repetitions == 1)
        std::cout << ALG_NAME << "," << N << ',' << scanned_elements;
    else
        std::cout << ALG_NAME << "," << N << ',' << avg_scan;
    std::cout << ',' << double(scanned_elements) / T << ',' << c << std::endl;

#else

    double ns = double(duration);
    std::cout.precision(5);

#ifdef BLOCKSIZETEST
    std::cout << type_string << '\t' << BLOCKSIZE << '\t' << N << '\t' << ns / T << '\t' << partial_sort_count << '\t' << repetitions << std::endl;
#else	
#ifdef TOFILE
#ifdef INS_SIZE_TEST
    std::cout << ALG_NAME << ',' << method_string << ',' << param_string << ',' << type_string << ',' << N << ',' << ns / T << ',' << partial_sort_count << ',' << repetitions << ',' << INS_SIZE << std::endl;
#else
	std::cout.width(10);
    std::cout << ALG_NAME << ',' << method_string << ',' << param_string << ',' << type_string << ',' << N << ',' << ns / T << ',' << partial_sort_count << ',' << repetitions << std::endl;
#endif // INS_SIZE_TEST

   #else
	std::cout.width(10);
    std::cout << ALG_NAME << '\t' << method_string << '\t' << param_string << '\t' << type_string << '\t' << N << '\t' << ns / T << '\t' << partial_sort_count << '\t' << repetitions << std::endl;
#endif
#endif
#endif

    return 0;
}
