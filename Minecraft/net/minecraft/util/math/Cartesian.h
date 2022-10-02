#pragma once
#include <any>
namespace Cartesian {

    template<typename T> 
    void CartesianRecurse(std::vector<std::vector<T>> &accum,
  std::vector<T> stack,
  std::vector<std::vector<T>> sequences,
  T index)
    {
      vector<T> sequence = sequences[index];
      for (T i : sequence) {
        stack.push_back(i);
        if (index == 0)
          accum.push_back(stack);
        else
          CartesianRecurse(accum, stack, sequences, index - 1);
        stack.pop_back();
      }
    }


	template<typename T> 
    T cartesianProduct(T sequences)
	{
          std::vector<std::vector<T>> accum;
          std::vector<T> stack;
          if (sequences.size() > 0) CartesianRecurse(accum, stack, sequences, sequences.size() - 1);
          return accum;
	}

}