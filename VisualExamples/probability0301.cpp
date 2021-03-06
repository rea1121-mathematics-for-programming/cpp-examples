//
// - Functions in CPP 
// - Lambdas
// - std::vector and std::list, 
// - map,filter and reduce (for now)
//



#include <vector>
#include <list>
#include <functional>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <numeric>



//
// To allow std::cout of std::vector and std::list
//
template <typename T>
std::ostream& myout(std::ostream& os, const T& container)
{
	os << "[";
	for (auto x : container) os << x << " ";
	os << "]";
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) { return myout(os, v); }

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& v) { return myout(os, v); }




//
// Three functional operations
//

auto map = [](auto fun, auto cnt) {
	auto retval = cnt;
	for (auto& elem : retval) {
		elem = fun(elem);
	}
	return retval;
};

auto filter = [](auto pred, auto& cnt) {
	auto retval = cnt;
	retval.erase(std::remove_if(retval.begin(), retval.end(), [=](auto x) {return !pred(x);}), retval.end());
	return retval;
};


auto reduce = [](auto fun, auto cnt) {
	assert(!cnt.empty());
	auto startval = *cnt.begin();
	return std::accumulate(++cnt.begin(), cnt.end(), startval, fun);
};


//
// Squares
//
int square1(int x) {
	return x*x;
}

class Square {
public:
	int operator()(int x) {
		return x*x;
	};
};




//
// Alternative counter
//
class Counter {
public:
	int operator()(int x) { 
		static int c = 0; 
		c++; 
		return c; 
	};
};




int main(int argc, char* argv[]) 
{
	//
	// Computing squares
	//

	//
	// With a function
	//
	std::cout << square1(10) << std::endl;

	//
	// With a function object
	//
	Square square2;
	std::cout << square2(10) << std::endl;

	//
	// Functions as variables
	//
	std::function<int(int)> square3 = square1;
	std::cout << square3(10) << std::endl;
	square3 = square2;
	std::cout << square3(10) << std::endl;

	// 
	// Inline functions
	//
	std::function<int(int)> square4 = [](int x)->int {return x*x; };
	std::cout << square4(10) << std::endl;

	auto square5 = [](auto x) {return x*x;};
	std::cout << square5(10) << std::endl;
	std::cout << square5(10.1f) << std::endl;



	// 
	// Defining vectors and lists
	//
	std::vector<int> v(30, 0);
	std::vector<int> w(30, 0);
	std::list<int> l(30, 0);

	std::cout << v << std::endl << w << std::endl << l << std::endl;


	//
	// Looping over entries initialising elements
	//
	int i = 1;
	for (auto& o : v) {
		o = i;
		i++;
	}

	i = 1;
	for (auto& o : l) {
		o = i;
		i++;
	}

	std::cout << v << std::endl << l << std::endl;


	//
	// Initialising with a map
	//
	w = map([](int x) {static int i = 0; i++; return x+i;}, w);
	std::cout << w << std::endl;

	// avoiding lambdas
	Counter counter;
	w = map(counter, w);
	std::cout << w << std::endl;

	//
	// Removing elements 10 and above
	//
	w = filter([](auto x) {return x < 10;}, w);
	std::cout << w << std::endl;	


	//
	// Summing with reduce
	//
	std::cout << reduce( [](int x, int y) {return x + y;},  w) << std::endl;
	 

	//
	// Solving the problem with alternating squares and odd squares
	//

	//
	// We create the vector of squares
	//
	std::vector<int> squares(21, 0);
	squares = map([](int x) {
			static int i = -1; 
			i++; 
			return x + i;
		}, squares);
	
	squares = map([](int x) {return x*x;}, squares);

	std::cout << squares << std::endl;
	
	//
	// Create the vector of odd squares
	//
	auto oddsquares = filter([](int x) {return x % 2 == 1; }, squares);
	std::cout << oddsquares << std::endl;

	//
	// Create squares and odd squares with alternating sign
	//
	auto altsquares = map([](int x) {
			static int i = -1; 
			i++; 
			if (i % 2 == 0) return x; 
			else return -x;
		},	squares);

	std::cout << altsquares << std::endl;

	auto altoddsquares = map([](int x) {
			static int i = -1; 
			i++; 
			if (i % 2 == 0) return x; 
			else return -x;
		}, oddsquares);

	std::cout << altoddsquares << std::endl;

	//
	// Compute sums
	//
	int sumsquare = reduce([](int x, int y) {return x + y;}, altsquares);
	std::cout << sumsquare << std::endl;

	int oddsum = reduce([](int x, int y) {return x + y;}, altoddsquares);
	std::cout << oddsum << std::endl;

	int c;
	std::cin >> c;
}
