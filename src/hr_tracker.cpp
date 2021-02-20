//=============================================================================
// hr_tracker.cpp 
//
// A simple program which sets up a workout which adjusts the speed of the 
// climbing wall based on a users average heartrate for a given duration.
// Also interacts with both the logger and the climbing wall correctly.
//=============================================================================
#include "Shm_vars.h"
#include <iostream>

// global data
// ===========================================
// set the workout duration here: 
//
   const int TOTAL_WORKOUT_DURATION = 20; //mins
   const int INCLINE = 8;
   const int START_SPEED= 12;
//
// ===========================================

int main (void)
{
  double target_heartrate = 130;
  double tolerance = 3.5; 
  int64_t start_time = 0;

  SHM::connect_existing_shm();
  start_time = SHM::timestamp->get();
  SHM::session_start_time->set(start_time);  
  SHM::hr_target->set(target_heartrate);
  SHM::log_active->set(1);
  SHM::hr_driven->set(1);
  
  
  std::cout << "Get Ready to Climb in 40 seconds." << std::endl;
  SHM::req_speed->set(0);  
  sleep(40);

  SHM::req_speed->set(START_SPEED);  
  SHM::req_incline->set(INCLINE);  

  while ( SHM::timestamp->get() <= (start_time + (20*60)))
  {
    sleep (24);
    if (abs(SHM::hr_avg->get() - target_heartrate) > tolerance)
    { // heartrate is out of tolerance, adjust speed of climbing wall.
      if(SHM::hr_avg->get() > target_heartrate)
      {
        SHM::req_speed->set(SHM::curr_speed->get() - 1);  
      }
      else
      {
        SHM::req_speed->set(SHM::curr_speed->get() + 1);  
      }
    } 
  }
  std::cout << "WORKOUT COMPLETE!!! " << std::endl;
  SHM::log_active->set(0);
  
 return 0;
}

