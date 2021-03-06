//
// Probability0501.cpp
// 
// Some calculations of probabilities by creating containers of events and sample spaces
// (Slow but fun!)
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

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& v) { std::cout << "(" << v.first << "," << v.second << ")"; return os; }



//
// Three functional operations
//

auto map = [](auto fun, auto container) {
	auto retval = container;
	for (auto& elem : retval) {
		elem = fun(elem);
	}
	return retval;
};

auto filter = [](auto pred, auto& container) {
	auto retval = container;
	retval.erase(std::remove_if(retval.begin(), retval.end(), [=](auto x) {return !pred(x);}), retval.end());
	return retval;
};

auto reduce = [](auto fun, auto container) {
	assert(!cnt.empty());
	auto startval = *container.begin();
	return std::accumulate(++container.begin(), container.end(), startval, fun);
};



//
// Setup for dice calculations
//

// Datatypes for dice

typedef int Dice;
typedef std::vector<Dice> Roll;
typedef std::vector<Roll> DiceEvent;



DiceEvent createDiceSample(int n)
{
	assert(n > 0);
	DiceEvent ret;
	DiceEvent temp;
	Roll newRoll(n);

	//Optimization	
	ret.reserve(1679616);
	temp.reserve(1679616);

	bool finished = false;
	std::vector<int> i(n,0);

	while (!finished) {
		int j;
		for (j = 0; j < n; j++) {
			newRoll[j] = i[j]+1;
		}
		ret.push_back(newRoll);
		j = 0;
		while (j < n && i[j] > 4) j++;
		if (j == n) finished = true;
		else {
			i[j] ++;
			for (int k = 0; k < j; k++) i[k] = 0;
		}
		std::cout << ret.size() << std::endl;
	
	}
	return ret;
}



//
// Alternative version of createDiceSample
//
DiceEvent createDiceSampleAlt(int n)
{
	assert(n > 0);
	Roll rollValues = { 1,2,3,4,5,6 };
	DiceEvent ret;
	DiceEvent temp;
	Roll newRoll;

	//Optimization
	newRoll.reserve(8);
	ret.reserve(1679616);
	temp.reserve(1679616);

	for (auto dval : rollValues) ret.push_back(Roll(1, dval));
	for (int i = 0; i < n - 1; i++) {
		temp.clear();
		for (auto dval : rollValues) {
			for (auto oldRoll : ret) {
				Roll newRoll = oldRoll;
				newRoll.push_back(dval);
				temp.push_back(newRoll);
			}
		}
		ret = temp;
	}
	return ret;

}


//
// Setup for card calculations
//

typedef int Val;
typedef int Suit;
typedef std::pair<Val, Suit> Card;
typedef std::vector<Card> Hand;
typedef std::vector<Hand> CardEvent;


CardEvent createCardSample()
{
	CardEvent ret;
	ret.reserve(2598960);

	Hand newHand(5);
	
	int i[5];

	for (i[0] = 0; i[0] < 52; i[0]++) {
		for (i[1] = i[0] + 1; i[1] < 52; i[1]++) {
			for (i[2] = i[1] + 1; i[2] < 52; i[2]++) {
				for (i[3] = i[2] + 1; i[3] < 52; i[3]++) {
					for (i[4] = i[3]+1; i[4] < 52; i[4]++) {						
						for (int j = 0; j < 5; j++) {
							newHand[j]=Card(i[j] / 4, i[j] % 4);
						}
						ret.push_back(newHand);
					}
				}
			}
		}
	}

	return ret;
}


int countVal(Hand hand, Val val) {
	int c = 0;
	for (Card card : hand) {
		if (card.first == val) c++;
	}
	return c;
}

int countSuit(Hand hand, Suit suit) {
	int c = 0;
	for (Card card : hand) {
		if (card.second == suit) c++;
	}
	return c;
}


//
// Setup for queue calculations
//

typedef int Person;
typedef Roll Queue;
typedef DiceEvent QueueEvent;


DiceEvent createQueueSample(int n)
{
	assert(n > 0);
	DiceEvent ret;
	DiceEvent temp;
	Roll newRoll(n);

	//Optimization	
	ret.reserve(1679616);
	temp.reserve(1679616);

	bool finished = false;
	std::vector<int> i(n, 0);

	while (!finished) {
		int j;
		for (j = 0; j < n; j++) {
			newRoll[j] = i[j];
		}
		ret.push_back(newRoll);
		j = 0;
		while (i[j] > 4 && j < n) j++;
		if (j == n) finished = true;
		else {
			i[j] ++;
			for (int k = 0; k < j; k++) i[k] = 0;
		}

	}
	return ret;
}



bool validQueue(Queue queue)
{
	for (Person p : queue) {
		int c = 0;
		for (Person q : queue) {
			if (p == q) c++;
		}
		if (c != 1) return false;
	}
	return true;
}




int main(int argc, char* argv[])
{
	
	{
		//
		// Probability of at least one six
		//
		DiceEvent sample = createDiceSample(2);
		DiceEvent event = filter([](Roll roll) {
			return roll[0] == 6 || roll[1] == 6;
		}, sample);

		 
		std::cout << event.size() << "/" << sample.size() << std::endl;

		//
		// Probability of no sixes
		//
		event = filter([](Roll roll) {
			return roll[0] != 6 && roll[1] != 6;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;


	
		//
		// Probability of sum is 6 when we roll 2 dice
		//
		event = filter([](Roll roll) {
			return roll[0] + roll[1] == 6;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;

		//
		// Probability of sum 10 when we roll 3 dice
		//
		sample = createDiceSample(3);
		event = filter([](Roll roll) {
			return roll[0] + roll[1] + roll[2] == 10;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;


		//
		// Probability of sum 13 when we roll 4 dice
		//
		sample = createDiceSample(4);
		event = filter([](Roll roll) {
			return roll[0] + roll[1] + roll[2] + roll[3] == 13;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;


		//
		// Probability of having four sixes when you roll eight dice
		//
		sample = createDiceSample(8);

		event = filter([](Roll roll) {
			int c = 0;
			for (auto dval : roll) if (dval == 6) c++;
			return c == 4;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;

	
	}

	{
		CardEvent sample;
		sample = createCardSample();
		
		//
		// Probability of drawing four aces
		//
		CardEvent event = filter([](Hand hand) {
			return countVal(hand, 0) == 4; 
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;

	
		//
		// Probability of drawing two aces and three jacks
		//
		event = filter([](Hand hand) {return countVal(hand, 0) == 2 && countVal(hand, 10) == 3; }, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;

		//
		// Probability of all having the same suit
		//
		event = filter([](Hand hand) {
			return countSuit(hand, 0) == 5 ||
				countSuit(hand, 1) == 5 ||
				countSuit(hand, 2) == 5 ||
				countSuit(hand, 3) == 5 ||
				countSuit(hand, 4) == 5;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;


		//
		// Probability of a pair
		//
		event = filter([](Hand hand) {
			return countVal(hand, hand[0].first) +
				countVal(hand, hand[1].first) +
				countVal(hand, hand[2].first) +
				countVal(hand, hand[3].first) +
				countVal(hand, hand[4].first) == 7;
		}, sample);


		std::cout << event.size() << "/" << sample.size() << std::endl;
	}


	{
		QueueEvent sample;
		sample = createQueueSample(6);
		sample = filter(validQueue, sample);

		//
		// Probability that a is at the front
		//
		QueueEvent event = filter([](Queue queue) {
			return queue[0] == 0;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;

		//
		// Probability that a is not at the back
		//
		event = filter([](Queue queue) {
			return queue[0] != 5;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;

		//
		// Probability that a is ahead of b
		//
		event = filter([](Queue queue) {
			return queue[0] < queue[1];
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;


		//
		// Probability that a and b occupy the first two positions
		//
		event = filter([](Queue queue) {
			return queue[0] < 2 && queue[1] < 2;
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;

		//
		// Probability that a and b are ahead of c and d
		//
		event = filter([](Queue queue) {
			return queue[0] < queue[2] && queue[0] < queue[3] && queue[1] < queue[2] && queue[1] < queue[3];
		}, sample);

		std::cout << event.size() << "/" << sample.size() << std::endl;
	}
	 
	
}



