//=============================================================================
// logger.cpp 
// 
// pulls data from the shm variables in real time and logs it.
//
// NOTES:
// for the logger to function properly, the user must: 
//   - ensure that timestamp is being updated correctly.
//   - set the start time 
//   - set log_active to true.
//
// the logger itself takes care of updating the elevation.
//=============================================================================
#include "Shm_vars.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

void pull_and_log_climbing_wall_data();
void init_log(void);

std::string time_to_string(time_t timestamp);

std::ofstream log;

int64_t recorded_start_time = 0;
float elevation = 0.0;
float duration = 0.0;


int main (void)
{
  SHM::connect_existing_shm();
  init_log();

  while (true) 
  {
    // check that start time is still the same as the recorded start time.
    if(     (recorded_start_time == SHM::session_start_time->get())
         && (recorded_start_time != 0))
    {
      if ( SHM::log_active->get())
      {
        pull_and_log_climbing_wall_data();
      }
    }
    else 
    {
      if (SHM::session_start_time->get() != 0)
      {
        init_log();
      }
    }

    sleep(1);
  }
  
 return 0;
}


void init_log(void)
{

  if(log.good())
  {
    log.close();
  }

  // wait till user sets start time, then make a record of the start time.
  recorded_start_time = SHM::session_start_time->get();
  while (recorded_start_time == 0)
  { 
    sleep(1);
    recorded_start_time = SHM::session_start_time->get();
  }

  std::string file_name = time_to_string (recorded_start_time);
  file_name = "../logs/" + file_name;
  file_name += "_session.csv";

  // open in append mode so we don't overwrite existing log files.
  // in case logger has been stopped and started without start_time
  // being modified.
  log.open(file_name.c_str(), std::ofstream::app);

  elevation = 0.0;
  SHM::elevation->set(0.0);
  duration = 0.0;
  SHM::session_duration->set(0.0);
  std::cout << "started new log file: " << file_name << std::endl;
}


void pull_and_log_climbing_wall_data()
{
  int speed = 0;

  static int draw_labels_counter = 0;
  if (draw_labels_counter == 0)
  {
  log << "#timestamp," 
      << " start_time," 
      << " duration," 

      << " cur_halt," 
      << " cur_speed," 
      << " cur_incline," 

      << " hr," 
      << " hr_avg," 
      << " hr_avg_dur," 

      << " hr_driven," 
      << " hr_target," 

      << " elevation," 
      << std::endl;
  }
  ++draw_labels_counter;    

  if(draw_labels_counter >= 60)
  { 
    draw_labels_counter = 0;
  } 

  speed = SHM::curr_speed->get();
  
  elevation += speed / 60.0;  // distance traveled in a single second.
  SHM::elevation->set(elevation);

  duration = ((SHM::timestamp->get() - SHM::session_start_time->get()) / 60.0);
  SHM::session_duration->set(duration);

  log << SHM::timestamp->get() << ", " 
      << SHM::session_start_time->get() << ", " 
      << duration << ", " 

      << SHM::curr_halt->get() << ", " 
      << speed << ", " 
      << SHM::curr_incline->get() << ", " 

      << SHM::hr->get() << ", " 
      << SHM::hr_avg->get() << ", " 
      << SHM::hr_avg_dur->get() << ", " 

      << SHM::hr_driven->get() << ", " 
      << SHM::hr_target->get() << ", " 

      << elevation << ", " 
      << std::endl;
}


std::string time_to_string(time_t timestamp)
{
  char buffer [80];
  std::string return_value = "";
  strftime (buffer,80,"%Y_%m_%d_%H-%M-%S",(localtime(&timestamp)));
  return_value = buffer;

  return return_value;
}


// about time: 
// time_t is the type used for number of seconds since epoc.
// ctime converts a time_t to a textual representation.
// see man ctime
