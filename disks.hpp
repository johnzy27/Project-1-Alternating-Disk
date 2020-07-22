///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(light_count > 0);

      for (size_t i = 0; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  
  bool is_alternating() const
  { 
      bool flag = true;
      for (size_t index = 0; index < total_count(); index = index + 2)
      {
          if (_colors[index] == DISK_LIGHT)
          {
              flag = false;
        }
      }

      for (size_t index = 1; index < total_count(); index = index + 2)
      {
          if (_colors[index] == DISK_DARK)
          {
              flag = false;
          }
      }
    return flag;
  }

 
  bool is_sorted() const {
      bool flag = true;
      for (size_t index = 0; index < total_count()/2; index = index + 1)
      {
          if (_colors[index] == DISK_DARK)
          {
              flag = false;
          }
      }
      for (size_t index = total_count()/2; index < total_count() ; index = index + 1)
      {
          if (_colors[index] == DISK_LIGHT)
          {
              flag = false;
          }
      }
      
    return flag;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) 
{
 
    //counter variable
    int count = 0;
    disk_state newState = before;
    if (newState.is_alternating())
    {
        for (int index = 0; index < newState.total_count() - 1; index++) 
        {
            for (int j = index; j < newState.total_count() - 1; j++) 
            {
                if ((newState.get(j) == DISK_DARK) && (newState.get(j + 1) == DISK_LIGHT))
                {
                    newState.swap(j);
                    count++;
                }
            }
        }
    }
    return sorted_disks(newState, count);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) 
{
 
  // counter variable
    int count = 0;
    disk_state newState = before;
  
  //checking if new state is alternating
    if (newState.is_alternating()) 
    { 
      //while loop until not sorted
        while (!newState.is_sorted())  
        {
          //traversing disk state and incrementing count
            for (int index = 0; index < newState.total_count() - 1; index = index + 2) 
            { 
                if (newState.is_index(index + 1)) 
                {
                    if ((newState.get(index) == DISK_DARK) && (newState.get(index + 1) == DISK_LIGHT)) 
                    {
                        newState.swap(index);
                        count++;
                    }
                }
            }
                //traversing disk state and incrementing count
            for (int j = newState.total_count() - 1; j > 0; j = j - 2) 
            { 
                if (newState.is_index(j + 1))
                {
                    if ((newState.get(j) == DISK_DARK) && (newState.get(j + 1) == DISK_LIGHT)) 
                    {
                        newState.swap(j);
                        count++;
                    }
                }
            }

        }
      //ending of while loop
    }
    return sorted_disks(newState, count);
}
