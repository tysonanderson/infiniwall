//=============================================================================
// grindhouse.cpp 
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
#include <sstream>

const int SETS = 2; 

std::string print_duration(const int& duration)
{
  int mins = duration / 60; 
  int secs = duration - (mins * 60); 
  string printable_duration ,tmp_mins, tmp_secs = "";
  std::stringstream ss_mins;
  std::stringstream ss_secs;

  ss_mins << mins;
  ss_secs << secs;

  ss_mins >> tmp_mins;
  ss_secs >> tmp_secs;
  printable_duration += tmp_mins;
  printable_duration += ":";
  printable_duration += tmp_secs;

  return printable_duration;
}

void run_set(float duration /*in mins*/, int speed /*in feet per minute*/, int set_count)
{// first gives the user a 10 second warning before the beginning of a set.  
 // then turns on the treadwall at the given speed for the given duration. 
 // then halts the treadwall for a "rest".  This rest is roughly 60% the 
 // duration of the work set, but not less than 90 seconds.  
  if (duration < 0)
    return;

  SHM::set_count->set(set_count);  
  int work_duration = duration * 60;
  const float rest_percentage = 0.6; 
  int rest_duration = ((duration * 60) * rest_percentage );
  if (rest_duration < 90)
    rest_duration = 90;

  std::cout << std::endl << std::endl;
  std::cout      << "Begin Set: " << set_count 
    //<< std::endl << ",   work duration: " << work_duration 
    //<< std::endl << ",   rest_duration: " << rest_duration; 
    << std::endl << ",   work duration: " << print_duration(work_duration) 
    << std::endl << ",   rest_duration: " << print_duration(rest_duration);
  std::cout << std::endl << std::endl;

  // perform user wake-up, to alert them to the start of the set
  SHM::req_speed->set(speed);  
  sleep(2);
  SHM::req_speed->set(0);  
  std::cout << "Get Ready to Climb." << std::endl;
  sleep(8); 

  // begin the work set
  SHM::set_start_time->set(SHM::timestamp->get());  
  SHM::set_duration->set(work_duration);  
  SHM::req_speed->set(22);  
  sleep(work_duration);                     

  // begin the rest set
  SHM::set_start_time->set(SHM::timestamp->get());  
  SHM::set_duration->set(rest_duration);  
  SHM::req_speed->set(0);  
  sleep(10);  
  system( "killall -9 kiosk_sim");
  sleep(rest_duration - 30); 

  system( "./kiosk_sim& > /dev/null 2>&1 ");
  sleep(10); 

}

int main (void)
{
  SHM::connect_existing_shm();
  SHM::session_start_time->set(SHM::timestamp->get());  

  SHM::log_active->set(0);
  SHM::req_speed->set(0);  

  system( "./kiosk_sim& > /dev/null 2>&1 ");
  sleep (5);
  for (int set_counter = 1; set_counter <= SETS; ++set_counter)
  {
    run_set(7, 22, 1);
    run_set(7, 22, 2);
    run_set(7, 22, 3);
  }
  SHM::req_speed->set(0);  
  std::cout << "WORKOUT COMPLETE!!! " << std::endl;
  SHM::req_speed->set(0);  
  
 return 0;
}

