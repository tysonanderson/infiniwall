//=============================================================================
// cat_shm.h 
// 
// prints curent values of all variables in SHared Memory (SHM) to the screen
// once per second in a loop.  
//=============================================================================
#include "Shm_vars.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

int main (void)
{

  SHM::connect_existing_shm();

  for(;;)
  {
    cout << "timestamp          : " << SHM::timestamp->get() << endl;
    cout << "start time         : " << SHM::start_time->get() << endl << endl;

    cout << "curr_halt          : " << SHM::curr_halt->get() << endl;
    cout << "curr_speed         : " << SHM::curr_speed->get() << endl;
    cout << "curr_incline       : " << SHM::curr_incline->get() << endl << endl;

    cout << "req_halt           : " << SHM::req_halt->get() << endl;
    cout << "req_speed          : " << SHM::req_speed->get() << endl;
    cout << "req_incline        : " << SHM::req_incline->get() << endl << endl;

    cout << "heartrate          : " << SHM::hr->get() << endl;
    cout << "heartrate avg      : " << SHM::hr_avg->get() << endl;
    cout << "heartrate avg dur  : " << SHM::hr_avg_dur->get() << endl << endl;

    cout << "heartrate driven   : " << SHM::hr_driven->get() << endl;
    cout << "heartrate target   : " << SHM::hr_target->get() << endl << endl;

    cout << "elevation          : " << SHM::elevation->get() << endl;
    cout << "log active         : " << SHM::log_active->get() << endl << endl;
 
    cout << "========================================" << endl;
    
    cout << endl;
    cout << endl;

    sleep (1);
  }
   
  return 0;
}
