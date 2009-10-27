#include "select.ih"

void Select::removeHard(File const &fd)
{
  erase(fd);
  d_deleteSet.erase(fd);
  d_dispatches.erase(fd);
}
