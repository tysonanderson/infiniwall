//=============================================================================
// scripted_session.cpp 
// 
// This program sets up a workout consisting of sets and reps.   
// It also interacts with both the logger and the climbing wall correctly.
//
// depends on SHM entries having been created, the timestamp being kept up to
// date, the logger running, and the kios_sim running.
//
// If your heartrate monitor is on, that data will be logged by the logger as 
// well.
//=============================================================================
#include "Shm_vars.h"
#include <iostream>

const int SETS = 8; 

int main (void)
{
  SHM::connect_existing_shm();
  SHM::session_start_time->set(SHM::timestamp->get());  
  SHM::log_active->set(1);

  std::cout << "Get Ready to Climb." << std::endl;
  SHM::req_speed->set(0);  
  sleep(15);
  SHM::req_speed->set(22);  
  SHM::req_incline->set(-5);
  std::cout << "5 min warmup." << std::endl;
  sleep(5 * 60);  // 5 min warmup

  for (int set_counter = 1; set_counter <= SETS; ++set_counter)
  {
    SHM::req_incline->set(-3);
    std::cout << "begin set " << set_counter << " of " << SETS << std::endl;
    sleep(60);
    SHM::req_incline->set(8);
    std::cout << "  rest " << set_counter << " of " << SETS << std::endl;
    sleep(90);
  }
  std::cout << "5 min cooldown." << std::endl;
  sleep(5 * 60);  // 5 min cooldown
  std::cout << "WORKOUT COMPLETE!!! " << std::endl;
  SHM::req_speed->set(0);  
  SHM::log_active->set(0);
  
 return 0;
}

