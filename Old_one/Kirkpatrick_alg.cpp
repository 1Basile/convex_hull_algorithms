#include <iostream>
#include <climits>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "Kirkpatrick_alg.h"

#include <math.h>
typedef std::vector<VPoint<int>*>  Vertices_int;

Kirkpatrick::Kirkpatrick() {}

Vertices_int* Kirkpatrick::get_convex_hull(Vertices_int* points_)
{
    points = points_;
  for (Vertices_int::iterator point=points->begin(); point != points->end(); ++point) {
    std::cout << "(" << (*point)->x << ", " << (*point)->y << ")"<< " | ";
  }
  std::cout << std::endl;

    sort_by_y_cordinate();

  for (Vertices_int::iterator point=points->begin(); point != points->end(); ++point) {
    std::cout << "(" << (*point)->x << ", " << (*point)->y << ")"<< " | ";
  }
  std::cout << std::endl;

    most_right_points = get_right_most_points();
    most_left_points = get_left_most_points();
    most_right_points = sort_by_left_turn(most_right_points);
    most_left_points = sort_by_right_turn(most_left_points);

    Vertices_int* result_convex_hull = new Vertices_int();
    result_convex_hull->reserve(most_left_points->size() + most_right_points->size());

    result_convex_hull->insert(result_convex_hull->end(), most_right_points->begin(), most_right_points->end()); // gether all convex hull in one list
    result_convex_hull->insert(result_convex_hull->end(), most_left_points->begin()+1, most_left_points->end()); // gether all convex hull in one list

    return result_convex_hull;
}

void Kirkpatrick::sort_by_y_cordinate()
{
  int max_y = INT_MIN, min_y = INT_MAX;
  int shift = 0;
  Vertices_int* sorted_list = new Vertices_int(points->begin(), points->end());

  for (Vertices_int::iterator point=points->begin(); point != points->end(); ++point) {
      if (max_y < ((*point)->y))  max_y = ((*point)->y);
      if (min_y > ((*point)->y))  min_y = ((*point)->y);
  }
  // std::cout << max_y << ", " << min_y << std::endl;

  if (min_y < 0) // Shift to make negative minimum to 0
  {
      shift = abs(min_y);
      min_y = 0;
      max_y += shift;
  }
  // std::cout << "Shift: " << shift << std::endl;

  // for (Vertices_int::iterator point=points->begin(); point != points->end(); ++point) {
  //   std::cout << (*point)->y << " | ";
  // }
  // std::cout << std::endl;


  // for (Vertices_int::iterator point=points->begin(); point != points->end(); ++point) {
  //   std::cout << point_is_left_from_line(*point, points->at(0), points->at(1)) << " | ";
  // }
  // std::cout << std::endl;

  int counting_vect_1[max_y-min_y+1] = {0};
  int counting_vect_2[max_y-min_y+1] = {0};

  for (Vertices_int::iterator point = points->begin(); point != points->end(); ++point) {
      ++counting_vect_1[(*point)->y + shift - min_y];
  }

  for (int i=1; i < sizeof(counting_vect_1)/sizeof(counting_vect_1[0]); i++) {
    counting_vect_1[i] += counting_vect_1[i-1];
  }

  for (int i=1; i < sizeof(counting_vect_1)/sizeof(counting_vect_1[0]); i++) {
    counting_vect_2[i] += counting_vect_1[i-1];
  }

  for (Vertices_int::iterator point = points->begin(); point != points->end(); ++point) {
      int p = counting_vect_2[(*point)->y + shift - min_y];
      (sorted_list->at(p)) = *point;
      ++counting_vect_2[(*point)->y + shift - min_y];
  }

  points = sorted_list;
}

Vertices_int* Kirkpatrick::get_right_most_points()
{
  Vertices_int* right_most_point = new Vertices_int();
  Vertices_int* one_y_coord = new Vertices_int();
  VPoint<int>* last_point = points->at(0);

  for (Vertices_int::iterator point = points->begin()+1; point != points->end(); ++point) {

    if ( (*point)->y == last_point->y ) {
       if ((*point)->x > last_point->x) last_point = (*point);
        continue;
    } else {
        right_most_point->push_back(last_point);
        last_point = (*point);
    }

  }
  right_most_point->push_back(last_point);

  std::cout << "Most right points:"<< std::endl;
  for (Vertices_int::iterator point=right_most_point->begin(); point != right_most_point->end(); ++point) {
    std::cout << "(" << (*point)->x << ", " << (*point)->y << ")"<< " | ";
  }
  std::cout << std::endl;

  return right_most_point;
}

Vertices_int* Kirkpatrick::get_left_most_points()
{
  Vertices_int* left_most_point = new Vertices_int();
  Vertices_int* one_y_coord = new Vertices_int();
  VPoint<int>* last_point = points->at(0);

  for (Vertices_int::iterator point = points->begin()+1; point != points->end(); ++point) {

    if ( (*point)->y == last_point->y ) {
       if ((*point)->x < last_point->x) last_point = (*point);
        continue;
    } else {
        left_most_point->push_back(last_point);
        last_point = (*point);
    }
  }
  left_most_point->push_back(last_point);
  

  std::cout << "Most right points:"<< std::endl;
  for (Vertices_int::iterator point=left_most_point->begin(); point != left_most_point->end(); ++point) {
    std::cout << "(" << (*point)->x << ", " << (*point)->y << ")"<< " | ";
  }
  std::cout << std::endl;

  return left_most_point;
}

Vertices_int* Kirkpatrick::sort_by_left_turn(Vertices_int* vertices) {
    Vertices_int* sorted_list = new Vertices_int();

    sorted_list->push_back(vertices->at(0));
    sorted_list->push_back(vertices->at(1));

    for (Vertices_int::iterator point = vertices->begin() + 2; point != vertices->end(); ++point) {
        std::cout << "\nItereation" << std::endl;

        if (sorted_list->size() <= 2) { std::cout << "Less then one" << std::endl; sorted_list->push_back(*point); continue; }
        while (sorted_list->size() > 2 && point_is_left_from_line(*point, sorted_list->at(sorted_list->size()-1), sorted_list->at(sorted_list->size()-2)) < 0) {

          std::cout << "Not left rotate: " << point_is_left_from_line(*point, sorted_list->at(sorted_list->size()-1), sorted_list->at(sorted_list->size()-2)) << std::endl;

          sorted_list->pop_back();
        }
        sorted_list->push_back(*point);
    }

    std::cout << "In result list: " << std::endl;
    for (Vertices_int::iterator point=sorted_list->begin(); point != sorted_list->end(); ++point) {
      std::cout << "(" << (*point)->x << ", " << (*point)->y << ")"<< std::endl;
    }
    return sorted_list;
}

Vertices_int* Kirkpatrick::sort_by_right_turn(Vertices_int* vertices) {
    Vertices_int* sorted_list = new Vertices_int();

    sorted_list->push_back(vertices->at(0));
    sorted_list->push_back(vertices->at(1));

    for (Vertices_int::iterator point = vertices->begin() + 2; point != vertices->end(); ++point) {
        std::cout << "\nItereation" << std::endl;

        if (sorted_list->size() <= 2) { std::cout << "Less then one" << std::endl; sorted_list->push_back(*point); continue; }
        while (sorted_list->size() > 2 && point_is_left_from_line(*point, sorted_list->at(sorted_list->size()-1), sorted_list->at(sorted_list->size()-2)) > 0) {

          std::cout << "Not right rotate: " << point_is_left_from_line(*point, sorted_list->at(sorted_list->size()-1), sorted_list->at(sorted_list->size()-2)) << std::endl;

          sorted_list->pop_back();
        }
        sorted_list->push_back(*point);
    }

    std::cout << "In result list: " << std::endl;
    for (Vertices_int::iterator point=sorted_list->begin(); point != sorted_list->end(); ++point) {
    std::cout << "(" << (*point)->x << ", " << (*point)->y << ")"<< std::endl;
    }
    return sorted_list;

}

template <typename T>
double Kirkpatrick::point_is_left_from_line(VPoint<T>* A, VPoint<T>* B, VPoint<T>* C){
    return (static_cast<double>(C->x) - B->x)*(A->y - B->y) - (C->y - B->y)*(A->x - B->x); }
