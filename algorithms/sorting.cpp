#include <array> // array
#include <algorithm> // copy, sort, rotate, upper_bound
#include <exception> // exception
#include <iostream> // cout, endl, boolalpha
#include <numeric> // iota
#include <random>  // mt19937, seed_seq, uniform_int_distribution
#include <string> // string

// Already included by default but here for visibility
// #include <utility> // swap

template <std::size_t S>
void print_array(std::array<int, S> &arr) {
  for (auto e : arr) {
    std::cout << e << ' ';
  }
  std::cout << std::endl;
}

template <class Iterator>
void print_array(Iterator begin, Iterator end) {
  for (Iterator i = begin; i != end; ++i) {
    std::cout << *i << ' ';
  }
  std::cout << std::endl;
}

/**
 * Hoare_partition_scheme, pivot is the middle element
 * {@link https://en.wikipedia.org/wiki/Quicksort#Hoare_partition_scheme}
 */
template <std::size_t S>
void quick_sort_helper_v0(std::array<int, S> &arr, size_t left, size_t right) {

  if (left < right) {
    size_t pivot_value = arr[(left + right) / 2];

    // Note: the + 1 and - 1 are to make up for the do while loops later on

    size_t i = left - 1;
    size_t j = right + 1;

    bool done = false;
    // swap values that are out of place, starting from the two ends of the subarray
    while (!done) {

      // TODO: explanation for the lack of bounds checking, the two do-while loops can go past the array but it's still correct
      //      (maybe because the pivot will always be ahead/behind, so theres a guarantee that it will stop because of the strict comparison)

      do {
        i += 1;
      } while (arr[i] < pivot_value);

      do {
        j -= 1;
      } while (arr[j] > pivot_value);

      if (i >= j) {
        done = true;
      } else {
        std::swap(arr[i], arr[j]);
      }

    }

    // Note: j lands where the inversion happens, it also happens to be the last index of the left part
    quick_sort_helper_v0(arr, left, j);
    quick_sort_helper_v0(arr, j + 1, right);

  }

}

/**
 * Lomuto Partition Scheme, pivot is the final element
 * {@link https://en.wikipedia.org/wiki/Quicksort#Lomuto_partition_scheme}
 * Note: choosing the last element as the pivot will perform worse on already sorted arrays
 */
template <std::size_t S>
void quick_sort_helper_v1(std::array<int, S> &arr, size_t left, size_t right) {

  if (left < right) {

    // Note: arr[right] is the index, but we don't assign it to value for convenience because we are using std::swap
    //       which requires a pointer (the compiler can coerce the proper types)

    // the partition index is where the pivot will be placed
    size_t partition_idx = left;
 
    // Note: recall that right is an index, not a size, so we need to put <= instead of just <
    for (size_t i = left; i <= right; ++i) {
      // Note: pi marks the spot for the next value that i finds that is smaller than the pivot,
      //       this makes it unstable
      if (arr[i] < arr[right]) {
        std::swap(arr[i], arr[partition_idx]);
        partition_idx += 1;
      }
    }

    // Note: one observation to note is that arr[right] < any value of arr in range [pi, right - 1], thus arr[right] < arr[pi],
    //       so after the swap, anything to the right of arr[pi] has a bigger value
    std::swap(arr[partition_idx], arr[right]);

    // Note: at this point, arr[pi] is in place, so exclude it from subsequent recursive calls
    quick_sort_helper_v1(arr, left, partition_idx - 1);
    quick_sort_helper_v1(arr, partition_idx + 1, right);

  }

}

/**
 * Quick sort. Recursive.
 * - choose a value from the array, aka the pivot
 * - move values less than the pivot to the left, and greater than, to the right
 * - the pivot is now in the right place
 * - call quicksort recursively on the two parts (excluding the pivot)
 */
template <std::size_t S>
void quick_sort(std::array<int, S> &arr) {
  quick_sort_helper_v0(arr, 0, arr.size() - 1);
  // quick_sort_helper_v1(arr, 0, arr.size() - 1);
}

template <std::size_t S>
void merge_v0(std::array<int, S> &arr, size_t left, size_t mid, size_t right) {
  
  size_t size_left = mid - left + 1;

  // Note: typically we put + 1 when calculating size from inclusive indices, 
  //       but because arr[mid] is already included the left part, we omit the + 1
  size_t size_right = right - mid;

  int *temp_arr = new int[size_left + size_right];
  // Make a copy of the subarray we are looking at
  // Note: recall that iterator ranges are [begin, end), that's why we need a + 1 on the ending iterator
  std::copy(arr.begin() + left, arr.begin() + right + 1, temp_arr);

  // i is the amount of items we have taken from left array
  size_t i = 0;
  // j is the amount of items we have taken from right array
  size_t j = 0;
  // k is the start of the sorted part in the original array
  size_t k = left;

  // Compare the start of the two halves, push the min of the two into the sorted part (we can do this because both parts are already sorted)
  while (i < size_left && j < size_right) {

    // Note: checking for equality makes it stable
    //       also the offset to get to the right part is size_left
    if (temp_arr[i] <= temp_arr[size_left + j]) {
      arr[k] = temp_arr[i];
      i += 1;
    }
    else {
      arr[k] = temp_arr[size_left + j];
      j += 1;
    }

    k += 1;
  }

  // Note: at this point, one of the two halves have had all their items put in place, so do the rest for the other half

  // Put the rest of the left part
  while (i < size_left) {
    arr[k] = temp_arr[i];
    k += 1;
    i += 1;
  }

  // Put the rest of the right part
  while (j < size_right) {
    arr[k] = temp_arr[size_left + j];
    k += 1;
    j += 1;
  }

  delete[] temp_arr;
}

template <std::size_t S>
void merge_sort_v0_helper(std::array<int, S> &arr, size_t left, size_t right) {
  if (left < right) {
    size_t mid = (left + right) / 2;

    // recursively call merge sort for left half ...
    merge_sort_v0_helper(arr, left, mid);
    // ... and right half
    merge_sort_v0_helper(arr, mid + 1, right);

    // merge left and right parts
    // Note: at this point [left, mid] is sorted and [mid + 1, right] is sorted
    merge_v0(arr, left, mid, right);
    // TODO: make merge swappable with other sorting algorithms, to do this we need might need to make functions receive begin and end iterators

  }
}

/**
 * Merge sort. Recursive.
 * - divide the array into two parts
 * - revursively call merge_sort on each of the two halves
 * - the base case of the recursive call is when the subarray has a length of 1
 * - after returning to the caller, each of the two halves should be sorted
 * - merge the two halves and sort them
 */
template <std::size_t S>
void merge_sort_v0(std::array<int, S> &arr) {
  merge_sort_v0_helper(arr, 0, arr.size() - 1);
}

/**
 * Selection sort.
 * - find the index of the smallest value in the unsorted part
 * - push it into the sorted part (by swaping it with the first value of the unsorted part, this makes it unstable)
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
    std::swap(arr[min_index], arr[i]);
  }
}

/**
 * Insertion sort v0. Inserts item to sorted part by swapping.
 * - partition the array into sorted and unsorted parts
 * - the sorted part is initially just arr[0]
 * - insert items into the sorted part one by one starting from arr[1]
 */
template <std::size_t S>
void insertion_sort_v0(std::array<int, S> &arr) {
  for (int i = 1; i < arr.size(); ++i) {

    int j = i;

    // go through the sorted part, swap until value is in the right place (i.e. bubble it down to place)
    while (j > 0 && arr[j - 1] > arr[j]) {
      std::swap(arr[j - 1], arr[j]);
      j -= 1;
    }

  }
}

/**
 * Insertion sort v1. Shifts items instead of explicit swapping.
 */
template <std::size_t S>
void insertion_sort_v1(std::array<int, S> &arr) {

  for (int i = 1; i < arr.size(); ++i) {

    // save the item at the current index (first value of the unsorted part)
    int item = arr[i];

    // Note: j starts from the back of the sorted part, it also serves as the insertion index for the element
    int j = i - 1;

    // shift all elements that are greater than item to the right by 1
    while(j >=0 && arr[j] > item) {
      // Note: this creates a `hole` at arr[j], a potential place to insert the item
      arr[j + 1] = arr[j];
      j -= 1;
    }

    // assign the item to the insertion index
    // Note: j + 1 it because of the j -= 1 in last iteration of the while loop
    arr[j + 1] = item;
  }
}

/**
 * Insertion sort v2. Binary search when looking for insertion index. Swaps elements using std::rotate.
 */
template <std::size_t S>
void insertion_sort_v2(std::array<int, S> &arr) {

  for (int i = 1; i < arr.size(); ++i) {
    // do insertion when item to be inserted is not in the right place within the sorted part
    if (arr[i] < arr[i - 1]) {

      int val = arr[i];
      int left = 0;
      int right = i;
      size_t mid = (left + right) / 2;
      
      // Note: we can opt for < instead of <= because we don't need to check the last single value
      while (left < right) {

        // Note: arr[mid] should be strictly greater than item so that it is stable
        if (arr[mid] > val) {

          // Note: if mid == 0, it will shortcircuit
          if (mid == 0 || arr[mid - 1] <= val) {
            break;
          }
          else {
            right = mid - 1;
          }
        }
        else if (arr[mid] <= val) {
          left = mid + 1;
        }

        mid = (left + right) / 2;
      }

      // Note: last argument of rotate must point to one past the end
      std::rotate(arr.begin() + mid, arr.begin() + i, arr.begin() + i + 1);

    }
  }
}

/**
 * Insertion sort v3. Three lines. Makes use of iterators.
 * @see example from https://en.cppreference.com/w/cpp/algorithm/rotate.
 */
template <std::size_t S>
void insertion_sort_v3(std::array<int, S> &arr) {
  for (auto i = arr.begin(); i != arr.end(); ++i) {
    // Note: upper_bound returns a pointer to the first item that is greater than arr[i] 
    //       within the sorted part (between [0, i - 1], recall that the last iterator must point to one past the end) 
    //       (i.e. find where to insert the item).
    //       Then rotate the the part (between [insertion_inex, i]) so that arr[i] would be the first item 
    //       (i.e. swap the items around to put the item in place)
    std::rotate(std::upper_bound(arr.begin(), i, *i), i, i + 1);
  }
}

/**
 * Finds the index where item would be inserted in a sorted array.
 */
template <std::size_t S>
int find_insertion_point(std::array<int, S> &arr, int item) {
  int right = arr.size() - 1;
  int left = 0;
  
  // Insert at the end if item is greater than the last item in the array
  if (arr[arr.size() - 1] <= item) {
    return arr.size();
  }

  int mid = (left + right) / 2;

  // Note: we can opt for < instead of <= because we don't need to check the last single value
  while (left < right) {

    std::cout << left << ' ' << mid << ' ' << right << std::endl;

    // Note: arr[mid] should be strictly greater than item so that it is stable 
    if (arr[mid] > item) {

      // Note: if mid == 0, it will shortcircuit 
      if (mid == 0 || arr[mid - 1] <= item) {
        break;
      } else {
        right = mid - 1;
      }

    }
    else if (arr[mid] <= item) {
      left = mid + 1;
    }

    mid = (left + right) / 2;
  }

  return mid;
}

/**
 * Binary search, iterative.
 * Returns the index of the item if found, else returns -1
 * assumes array is in ascending order
 * [from, to]
 */
template <std::size_t S>
int binary_search(std::array<int, S> &arr, int item, size_t from, size_t to)
{

  if (to < from) {
    throw std::exception("to must be greater than from");
  }

  size_t left = from;
  size_t right = to;

  while (left <= right) {
    // Note: integer division will truncate the decimal, same as floor()
    size_t mid = (left + right) / 2;

    // if item is less than the one in mid, then the item must be on the left side of mid
    // Note: +/- 1 is to get rid of mid in search space, (i.e. only get the the left/right side of mid)
    if (item < arr[mid]) {
      right = mid - 1;
    // if item is greater than the one in mid, then the item must be on the right side of mid
    } else if (item > arr[mid]) {
      left = mid + 1;
    } else {
      return mid;
    }
  }

  return -1;
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

int main() {

  std::array<int, 10> arr;

  // Fill the array with consecutive numbers starting from 0
  // Note: iota is the ninth letter of the greek alphabet, so the name doesn't really give a hint to what it does
  std::iota(arr.begin(), arr.end(), 0);

  shuffle(arr, "greedisgood");

  // Note: std::array overloads the = operator to copy each element instead of just the reference
  auto arr1 = arr;

  print_array(arr1);
  // insertion_sort_v0(arr1);
  // insertion_sort_v1(arr1);
  // insertion_sort_v2(arr1);
  // insertion_sort_v3(arr1);
  // selection_sort(arr1);
  // merge_sort_v0(arr1);
  quick_sort(arr1);

  // std::cout << find_insertion_point(arr1, -1) << std::endl;
  // std::cout << binary_search(arr1, 0, 0, 9) << std::endl;

  print_array(arr1);
  std::cout << std::boolalpha << is_ascending(arr1) << std::endl;

  return 0;
}