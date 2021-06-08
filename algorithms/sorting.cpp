#include <iostream> // cout, endl, boolalpha
#include <array> // array
#include <numeric> // iota
#include <random>  // mt19937, seed_seq, uniform_int_distribution
#include <string> // string

// Already included by default but here for visibility
// #include <utility> // swap

template <std::size_t S>
void quick_sort(std::array<int, S> &array) {
 // TODO
}

template <std::size_t S>
void merge_sort(std::array<int, S> &array) {
 // TODO
}

/**
 * Selection sort.
 * - find the index of the smallest value in the unsorted part
 * - push it into the sorted part (by swaping it with the first value of the unsorted part)
 * - grow the sorted part, then repeat from the beginning of the unsorted part
 */
template <std::size_t S>
void selection_sort(std::array<int, S> &arr) {

  int min_index;

  // Note: i is the first index of the unsorted part
  for (int i = 0; i < arr.size() - 1; ++i) {

    // Find min
    min_index = i;
    // Note: we initialize min as arr[i], so just start at i + 1
    for (int j = i + 1; j < arr.size(); ++j) {
      if (arr[j] < arr[min_index])
        min_index = j;
    }

    // Swap the min with the first element of the unsorted subarray
    // Note: std::swap uses move semantics so it's probably faster than copy
    std::swap(arr[min_index], arr[i]);
  }
}

template <std::size_t S>
void insertion_sort(std::array<int, S> &array) {

}

template <std::size_t S>
bool is_ascending(std::array<int, S>& array) {
  for (int i = 1; i < array.size(); ++i) {
    if (array[i] < array[i - 1])
      return false;
  }
  return true;
}

/**
 * Fisher-Yates shuffle.
 * - look at the range [0, i] of the array, where i = array.size - 1 initially, (i.e. we are looking at the full array at the start)
 * - pick a number j in the range [0, i]
 * - swap array[j] and array[i], (i.e. swap a random element with the last element within the range we are looking at)
 * - note that after the swap, array[i] is in its final place
 * - then decrement i
 * - repeat until we only have 1 element left
 */
template <std::size_t S>
void shuffle(std::array<int, S>& array, std::string seed) {

  // TODO: rng could be biased

  // Mersenne Twister engine
  // Note: default seed is 5489u
  std::mt19937 mt(std::seed_seq(seed.begin(), seed.end()));

  // Note: range is [0, array.size())
  std::uniform_int_distribution<int> dist(0, array.size());

  for (int i = array.size() - 1; i > 0; --i) {

    // Note: 0 <= j <= i, so we need mod (i + 1)
    int j = dist(mt) % (i + 1);

    std::swap(array[i], array[j]);
  }

}

template <std::size_t S>
void print_array(std::array<int, S> &array) {
  for (auto e : array) {
    std::cout << e << std::endl;
  }
}

int main() {

  std::array<int, 100> arr;

  // Fill the array with consecutive numbers starting from 0
  // Note: iota is the ninth letter of the greek alphabet, so the name doesn't really give a hint to what it does
  std::iota(arr.begin(), arr.end(), 0);

  shuffle(arr, "greedisgood");

  // Note: std::array overloads the = operator to copy each element instead of just the reference
  auto arr1 = arr;

  selection_sort(arr1);

  std::cout << std::boolalpha << is_ascending(arr1) << std::endl;

  // print_array(arr1);

  return 0;
}