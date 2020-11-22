#include <algorithm>
#include <math.h>
#include <vector>
#include "Fast_convex_hull_alg.h"
#include "VPoint.h"

Fast_convex_hull::Fast_convex_hull() = default;

Vertices_double* Fast_convex_hull::get_convex_hull(Vertices_double* points_)
{
    Vertices_double* points = new Vertices_double();
    Vertices_double* down_from_line_points = new Vertices_double();
    Vertices_double* upper_from_line_points = new Vertices_double();
    Vertices_double*     convex_hull = new Vertices_double();

    for (auto point=points_->begin(); point != points_->end(); point++) {   // copy points to new vector
        points->push_back(*point);
    }

    VPoint<double>* most_left_point  = *points->begin();
    VPoint<double>* most_right_point  = *points->begin();

    for (Vertices_double::iterator point=points->begin(); point != points->end(); ++point) {
        if ( (*point)->x < most_left_point->x ) { most_left_point = *point; }
        if ( (*point)->x > most_right_point->x ) { most_right_point = *point; }
    }

    for (Vertices_double::iterator point=points->begin(); point != points->end(); ++point) {
        if (point_is_left_from_line(*point, most_left_point, most_right_point) < 0 )
            {down_from_line_points->push_back(*point);}
        if (point_is_left_from_line(*point, most_left_point, most_right_point) > 0 )
            {upper_from_line_points->push_back(*point);}
    }


    convex_hull->push_back(most_left_point);
    recursive_convex_hull(down_from_line_points, most_left_point, most_right_point, convex_hull);
    if (most_right_point != most_left_point) { convex_hull->push_back(most_right_point); }
    recursive_convex_hull(upper_from_line_points, most_right_point, most_left_point, convex_hull);

    return convex_hull;
}

void Fast_convex_hull::recursive_convex_hull(Vertices_double* points_, VPoint<double>* A, VPoint<double>* B, Vertices_double* container) {

    if (points_->size() == 0) {return;}

    Vertices_double* points = new Vertices_double();

    for (auto point=points_->begin(); point != points_->end(); point++) {   // copy points to new vector
        points->push_back(*point);
    }

    VPoint<double>* most_far_point = *(points->begin());
    Vertices_double* left_from_line_points_1 = new Vertices_double();
    Vertices_double* left_from_line_points_2 = new Vertices_double();
    double max_distance = 0;
    double currrent_distance;

    for (Vertices_double::iterator point=points->begin(); point != points->end(); ++point) {
        currrent_distance = distance_from_point(A, B, *point);
        if ( currrent_distance > max_distance ) {
            max_distance = currrent_distance;
            most_far_point = *point;
        }
    }

    for (Vertices_double::iterator point=points->begin(); point != points->end(); ++point) {
        if (point_is_left_from_line(*point, A, most_far_point) < 0 )    // !
        {left_from_line_points_1->push_back(*point);}
    }

    for (Vertices_double::iterator point=points->begin(); point != points->end(); ++point) {
        if (point_is_left_from_line(*point, most_far_point, B) < 0 )    // !
        {left_from_line_points_2->push_back(*point);}
    }

    recursive_convex_hull(left_from_line_points_1, A, most_far_point, container);
    container->push_back(most_far_point);
    recursive_convex_hull(left_from_line_points_2, most_far_point, B, container);
}


template <typename T>
double Fast_convex_hull::point_is_left_from_line(VPoint<T>* A, VPoint<T>* B, VPoint<T>* C){
    return (static_cast<double>(C->x) - B->x)*(A->y - B->y) - (C->y - B->y)*(A->x - B->x); }

double Fast_convex_hull::distance_from_point(VPoint<double> *P_1, VPoint<double> *P_2, VPoint<double> *&A) {
    return std::abs( (P_2->y - P_1->y)*A->x - (P_2->x - P_1->x)*A->y + P_2->x*P_1->y - P_2->y*P_1->x ) / sqrt( (P_2->y - P_1->y)*(P_2->y - P_1->y) + (P_2->x - P_1->x)*(P_2->x - P_1->x));
}

//      int main() {
//          srand(time(NULL)); // set random seed
//          Vertices_double* convex_hull_double = new Vertices_double();
//          Vertices_double* ver_double = new Vertices_double();
//          double w = 10;
//          ver_double = new Vertices_double();
//          Vertices_double* dir_double = new Vertices_double();
//          int NUMBER_OF_POINTS = 113;
//
//           srand(time(NULL)); // set random seed
//
//       for(int i=0; i<NUMBER_OF_POINTS; i++)
//       {
//        ver_double->push_back(new VPoint<double>( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX ));
//        dir_double->push_back(new VPoint<double>( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5));
//       }
//
//        Fast_convex_hull* fast_convex_hull = new Fast_convex_hull();
//        for(int i=0; i<6; i++)
//      {
//          Vertices_double::iterator j = dir_double->begin();
//          for(Vertices_double::iterator i = ver_double->begin(); i != ver_double->end(); ++i)
//          {
//              (*i)->x += (*j)->x * w/50;
//              (*i)->y += (*j)->y * w/50;
//              if( (*i)->x > w ) (*j)->x *= -1;
//              if( (*i)->x < 0 ) (*j)->x *= -1;
//
//              if( (*i)->y > w ) (*j)->y *= -1;
//              if( (*i)->y < 0 ) (*j)->y *= -1;
//              ++j;
//          }
//          convex_hull_double = fast_convex_hull->get_convex_hull(ver_double);
//            for (Vertices_double::iterator point=convex_hull_double->begin(); point != convex_hull_double->end(); ++point) {
//              std::cout << "(" << (*point)->x << ", " << (*point)->y << ")"<< " | ";
//            }
//            std::cout << std::endl;
//      }
//        return 0;
//      }
