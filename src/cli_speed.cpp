//=============================================================================
// cli_speed.cpp 
// 
// This program allows you to set the speed via command line.
//=============================================================================
#include "Shm_vars.h"
#include <string>
#include <iostream>
#include <string>
#include <sstream>

int main (void)
{
  SHM::connect_existing_shm();
  int user_input_speed = SHM::req_speed->get();
  std::string user_input_string = "";
  for (;;)
  {
    std::cout << "Valid range is from 3 to 50 feet per minute." << std::endl
              << "req_speed: ";

    std::cin >> user_input_string;

    std::stringstream ss;
    ss << user_input_string;
    ss >> user_input_speed;

    SHM::req_speed->set(user_input_speed);

    std::cout << std::endl
              << "setting requested speed: " 
              << user_input_speed 
              << std::endl;
  }
 return 0;
}
