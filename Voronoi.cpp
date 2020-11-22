#include "Voronoi.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>

using namespace vor;

Voronoi::Voronoi()
{
  edges = 0;
}

Edges* Voronoi::get_edges(Vertices* vertices, int w, int h)
{
  places = vertices;
  width = w;
  height = h;
  root = nullptr;

  if (!edges) edges = new Edges(); // Clearing points and edges containers
  else
  {
    for (Vertices::iterator  i = points.begin(); i != points.end(); ++i) delete (*i);
    for (Edges::iterator     i = edges->begin(); i != edges->end(); ++i) delete (*i);
    points.clear();
    edges->clear();
  }

  for(Vertices::iterator i = places->begin(); i!=places->end(); ++i) // Add all vertices to queue
  {
    queue.push(new VEvent(*i, true));
  }

  VEvent* event;
  while(!queue.empty()) // Main algorithm loop
  {
    event = queue.top();
    queue.pop();

    line_y_cord = event->point->y;
    if (deleted.find(event) != deleted.end())
    {
      delete(event);
      deleted.erase(event);
      continue;
    }
    if (event->is_place_event) insert_parabola(event->point);
    else remove_parabola(event);
    delete (event);
  }

  finish_edge(root);

  for(Edges::iterator i = edges->begin(); i != edges->end(); ++i) // Connect two parts of edge into one
  {
    if ((*i)->neighbour)
    {
      (*i)->start = (*i)->neighbour->end;
      delete (*i)->neighbour;
    }
  }

  return edges;
}


void  Voronoi::insert_parabola(VPoint<double> * p)
{
  if (!root)  {root = new VParabola(p); return;}

  if(root->isLeaf && root->site->y - p->y < 1)  // Both on the same line
  {
    VPoint<double>* fp = root->site;
    root->isLeaf = false;
    root->SetLeft(new VParabola(fp));
    root->SetRight(new VParabola(p));
    VPoint<double>* s = new VPoint<double>((p->x + fp->x)/2, height); // Make a point between points
    points.push_back(s);
    if(p->x > fp->x) root->edge = new VEdge(s, fp, p); // Left, right
    else root->edge = new VEdge(s, p, fp);
    edges->push_back(root->edge);
    return;
  }

  VParabola* par = get_parabola_by_x(p->x);

  if (par->cEvent)
  {
    deleted.insert(par->cEvent);
    par->cEvent = 0;
  }

  VPoint<double>* start = new VPoint<double>(p->x, get_y_of_point(par->site, p->x));
  points.push_back(start);

  VEdge* el = new VEdge(start, par->site, p);
  VEdge* er = new VEdge(start, p, par->site);

  el->neighbour = er;
  edges->push_back(el);

  par->edge = er; // Rebilding tree from leaf to edge
  par->isLeaf = false;

  VParabola* p0 = new VParabola(par->site);
  VParabola* p1 = new VParabola(p);
  VParabola* p2 = new VParabola(par->site);

  par->SetRight(p2);
  par->SetLeft(new VParabola());
  par->Left()->edge = el;

  par->Left()->SetLeft(p0);
  par->Left()->SetRight(p1);

  check_circle(p0);
  check_circle(p2);
}

void  Voronoi::remove_parabola(VEvent * e)
{
  VParabola* p1 = e->arch;

  VParabola* xl = VParabola::GetLeftParent(p1); // Delet all one level children and parents circle events
  VParabola* xr = VParabola::GetRightParent(p1);

  VParabola* p0 = VParabola::GetLeftChild(xl);
  VParabola* p2 = VParabola::GetRightChild(xr);

  if  (p0->cEvent)  { deleted.insert(p0->cEvent); p0->cEvent = 0; }
  if  (p2->cEvent)  { deleted.insert(p2->cEvent); p2->cEvent = 0; }

  VPoint<double>* p = new VPoint<double>(e->point->x, get_y_of_point(p1->site, e->point->x)); // Add circle center as vertice to points
  points.push_back(p);

  xl->edge->end = p; // Creating half-edges for given point
  xr->edge->end = p;

  VParabola* higher;
  VParabola* par = p1;
  while (par != root)
  {
    par = par->parent;
    if (par == xl) higher = xl;
    if (par == xr) higher = xr;
  }

  higher->edge = new VEdge(p, p0->site, p2->site);  // Set created leaves as one ended in most parent point
  edges->push_back(higher->edge);

  VParabola* gparent = p1->parent->parent;
  if (p1->parent->Left() == p1) // Make right dependencies 
  {
    if (gparent->Left()  == p1->parent) gparent->SetLeft(p1->parent->Right());
    if (gparent->Right() == p1->parent) gparent->SetRight(p1->parent->Right());
  }

  else
  {
    if (gparent->Left()  == p1->parent) gparent->SetLeft(p1->parent->Left());
    if (gparent->Right() == p1->parent) gparent->SetRight(p1->parent->Left());
  }

  delete p1->parent;
  delete p1;

  check_circle(p0);
  check_circle(p2);
}

void  Voronoi::finish_edge(VParabola* n)
{
  if (n->isLeaf) {delete n; return;}

  double mx;
  if(n->edge->direction->x > 0.0) 
    {mx = std::max(width,  n->edge->start->x + 10 );}
  else 
    {mx = std::min(0.0, n->edge->start->x - 10);}

  VPoint<double>* end = new VPoint<double>(mx, mx * n->edge->a + n->edge->b); 
  n->edge->end = end;
  points.push_back(end);

  finish_edge(n->Left() );
  finish_edge(n->Right());
  delete n;
}


double  Voronoi::get_x_of_egde(VParabola* par, double y)
{
  VParabola* left = VParabola::GetLeftChild(par);
  VParabola* right= VParabola::GetRightChild(par);

  VPoint<double>* p = left->site;
  VPoint<double>* r = right->site;

  double dp = 2.0 * (p->y - y);
  double a1 = 1.0 / dp;
  double b1 = -2.0 * p->x / dp;
  double c1 = y + dp / 4 + p->x * p->x / dp;

         dp = 2.0 * (r->y - y);
  double a2 = 1.0 / dp;
  double b2 = -2.0 * r->x/dp;
  double c2 = line_y_cord + dp / 4 + r->x * r->x / dp;

  double a = a1 - a2;
  double b = b1 - b2;
  double c = c1 - c2;

  double disc = b*b - 4 * a * c;
  double x1 = (-b + std::sqrt(disc)) / (2*a);
  double x2 = (-b - std::sqrt(disc)) / (2*a);

  double ry;
  if(p->y < r->y ) ry =  std::max(x1, x2);
  else ry = std::min(x1, x2);

  return ry;
}


VParabola* Voronoi::get_parabola_by_x(double xx)
{
  VParabola* par = root;
  double x = 0.0;

  while(!par->isLeaf) // move through tree up to apropriate leaf
  {
    x = get_x_of_egde(par, line_y_cord);
    if(x>xx) par = par->Left();
    else par = par->Right();        
  }
  return par;
}


double  Voronoi::get_y_of_point(VPoint<double> * p, double x) // Get y coordiante of this point parabola for this x
{
  double dp = 2 * (p->y - line_y_cord);
  double a1 = 1 / dp;
  double b1 = -2 * p->x / dp;
  double c1 = line_y_cord + dp / 4 + p->x * p->x / dp;

  return(a1*x*x + b1*x + c1);
}


void  Voronoi::check_circle(VParabola* b)
{
  VParabola* lp = VParabola::GetLeftParent (b);
  VParabola* rp = VParabola::GetRightParent(b);

  VParabola* a  = VParabola::GetLeftChild (lp);
  VParabola* c  = VParabola::GetRightChild(rp);

  if (!a || !c || a->site == c->site) return;

  VPoint<double>* s = 0;
  s = get_edge_intersection(lp->edge, rp->edge);
  if (s == 0) return;

  double dx = a->site->x - s->x;
  double dy = a->site->y - s->y;

  double d = std::sqrt( (dx * dx) + (dy * dy) );

  if (s->y - d >= line_y_cord)  return;

  VEvent* e = new VEvent(new VPoint<double>(s->x, s->y - d), false);
  points.push_back(e->point);
  b->cEvent = e;
  e->arch = b;
  queue.push(e);
}

VPoint<double>* Voronoi::get_edge_intersection(VEdge* a, VEdge* b)
{
  double x = (b->b - a->b) / (a->a - b->a);
  double y = a->a * x + a->b;

  if ((x - a->start->x) / a->direction->x < 0) return 0;
  if ((y - a->start->y) / a->direction->y < 0) return 0;

  if ((x - b->start->x) / b->direction->x < 0) return 0;
  if ((y - b->start->y) / b->direction->y < 0) return 0;

  VPoint<double>* p = new VPoint<double>(x, y);
  points.push_back(p);
  return p;
}



