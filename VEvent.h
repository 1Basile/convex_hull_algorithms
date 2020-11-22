#ifndef VEvent_h
#define VEvent_h

#include <iostream>
#include "VPoint.h"
#include "VParabola.h"

/*
  The class for storing place / circle event in event queue.

  point              : the point at which current event occurs (top circle point for circle event, focus point for place event)
  is_place_events    : whether it is a place event or not
  y                  : y coordinate of "point", events are sorted by this "y"
  arch               : if "pe", it is an arch above which the event occurs
*/
class VEvent
{
public:
  VPoint<double>*     point;
  bool        is_place_event;
  double      y;
  VParabola*  arch;

  /*
    point_             : point, at which the event occurs
    is_place_event_    : whether it is a place event or not
  */
  VEvent(VPoint<double>* point_, bool is_place_event_)
  {
    point           = point_;
    is_place_event  = is_place_event_;
    y               = point->y;
    arch            = nullptr;
  }

  /*
    function for comparing two events by "y"
  */
  struct CompareEvent: public std::binary_function<VEvent*, VEvent*, bool>
  {
    bool operator()(const VEvent* l, const VEvent* r) const { return (l->y < r->y); }
  };
};

#endif
