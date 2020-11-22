#include "VParabola.h"
#include "VPoint.h"

VParabola::VParabola()
{
  isLeaf  = false;
  // site    = 0;
  cEvent  = 0;
  edge    = 0;
  parent  = 0;
}

// VParabola::VParabola(VPoint<double>* s)
// {
//   site    = s;
//   isLeaf  = true;
//   cEvent  = 0;
//   edge    = 0;
//   parent  = 0;
// }

VParabola* VParabola::GetLeft(VParabola* p)
{
  return GetLeftChild(GetLeftParent(p));
}


VParabola* VParabola::GetRight(VParabola* p)
{
  return GetRightChild(GetRightParent(p));
}

VParabola* VParabola::GetLeftParent(VParabola* p)
{
  VParabola* par    = p->parent;
  VParabola* pLast  = p;

  while(par->Left() == pLast) 
  { 
    if (!par->parent) return nullptr;
    pLast = par; 
    par   = par->parent;
  }
  return par;
}

VParabola* VParabola::GetRightParent(VParabola* p)
{
  VParabola* par    = p->parent;
  VParabola* pLast  = p;
  while (par->Right() == pLast) 
  { 
    if (!par->parent) return nullptr;
    pLast = par;
    par   = par->parent;
  }
  return par;
}

VParabola* VParabola::GetLeftChild(VParabola* p)
{
  if (!p) return nullptr;
  VParabola* par = p->Left();
  while (!par->isLeaf) par = par->Right();
  return par;
}

VParabola* VParabola::GetRightChild  (VParabola* p)
{
  if (!p) return nullptr;
  VParabola* par           = p->Right();
  while (!par->isLeaf) par = par->Left();
  return par;
}
