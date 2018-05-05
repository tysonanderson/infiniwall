//=============================================================================
// mkshm.cpp
// 
// creates the SHM (SHared Memory) entries for all climbing wall related info.
//=============================================================================
#include "Shm_vars.h"
#include <string>

int main()
{
  SHM::allocate_new_shm();
}
