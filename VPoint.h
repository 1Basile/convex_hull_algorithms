#ifndef VPoint_h
#define VPoint_h

/*
  A structure that stores 2D point
*/
template <typename T>
struct VPoint
{
public:

  T x, y;

  VPoint(T nx, T ny)
  {
      x = nx; 
      y = ny;
  }

  VPoint& operator= (VPoint& other) {
    x = other.x;
    y = other.y;
    return *this;
  }
};

#endif
