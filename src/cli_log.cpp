//=============================================================================
// cli_halt.cpp 
// 
// This program allows you to start and stop logging via command line.
//=============================================================================
#include "Shm_vars.h"
#include <string>
#include <iostream>
#include <string>
#include <sstream>

int main (void)
{
  SHM::connect_existing_shm();
  int user_input_speed = SHM::req_halt->get();
  std::string user_input_string = "";
  for (;;)
  {
    std::cout << "Valid range is from 0 or 1." << std::endl
              << "log_active: "  ;

    std::cin >> user_input_string;

    std::stringstream ss;
    ss << user_input_string;
    ss >> user_input_speed;

    SHM::log_active->set(user_input_speed);
    std::cout << "setting requested speed: " <<  user_input_speed << std::endl;
  }
 return 0;
}
