//=============================================================================
// anaerobic_tracker.cpp
//
// this workout alternates between two sets of speed&incline.  One as a work
// set, and the other as a rest set.  
//=============================================================================
#include "Shm_vars.h"
#include <iostream>

// global data
// ===========================================
// set the workout duration here: 
//
   const int SESSION_COMPLETE_HR = 180;
         int WORK_HR = 140;
   const int WORK_SPEED = 19;
   const int WORK_INCLINE = 0;
   
         int REST_AVG_HR = 110;
   const int REST_SPEED = 12;
   const int REST_INCLINE = 8;
//
// ===========================================

int main (void)
{
  int64_t start_time = 0;

  enum State { WORK = 0, REST};
  State state = REST;

  SHM::connect_existing_shm();
  start_time = SHM::timestamp->get();
  SHM::start_time->set(start_time);  
  SHM::log_active->set(1);
  SHM::hr_driven->set(1);
  
  
  std::cout << "Get Ready to Climb in 30 seconds." << std::endl;
  SHM::req_speed->set(0);  
  sleep(30);

  do
  {
    switch (state)
    {
      case WORK :
        if ( SHM::hr->get() > WORK_HR)
        {
          state = REST;
          REST_AVG_HR += 5;
          SHM::req_speed->set(REST_SPEED);
          SHM::req_incline->set(REST_INCLINE);
          SHM::hr_target->set(REST_AVG_HR);
          std::cout << "BEGIN REST SET : " << REST_AVG_HR << std::endl;
          sleep (15);
          SHM::req_speed->set(0);
        }
        else 
        {
          std::cout << "  " << SHM::hr->get() <<  "  of  " << WORK_HR 
                    << "  --  " << SHM::duration->get() << std::endl;
        }
      break;

      case REST :
        if ( SHM::hr_avg->get() < REST_AVG_HR)
        {
          state = WORK;
          WORK_HR += 5;
          SHM::req_speed->set(WORK_SPEED);
          SHM::req_incline->set(WORK_INCLINE);
          SHM::hr_target->set(WORK_HR);
          std::cout << "BEGIN WORK SET : " << WORK_HR << std::endl;
        }
        else 
        {
          std::cout << "  " << SHM::hr_avg->get() <<  "  of  " << REST_AVG_HR 
                    << "  --  " << SHM::duration->get() << std::endl;
        }
      break;
    } 
    sleep (1);
  } while ( WORK_HR < SESSION_COMPLETE_HR);

  SHM::req_speed->set(REST_SPEED);
  SHM::req_incline->set(REST_INCLINE);
  SHM::hr_target->set(REST_AVG_HR);
  std::cout << "BEGIN FINAL REST SET : " << REST_AVG_HR << std::endl;
  sleep (15);
  SHM::req_speed->set(0);
  sleep (60);
  std::cout << "WORKOUT COMPLETE!!! " << std::endl;
  std::cout << "WORKOUT COMPLETE!!! " << std::endl;
  std::cout << "WORKOUT COMPLETE!!! " << std::endl;
  SHM::log_active->set(0);
  
 return 0;
}

