#include "../core/details/bubblesort.hpp"
#include <algorithm>

using namespace std;
using namespace Vlinder::RTIMDB::Core::Details;

int main()
{
	int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	random_shuffle(begin(values), end(values));
	bubbleSort(begin(values), end(values), less<int>());
	return is_sorted(begin(values), end(values)) ? 0 : 1;
}