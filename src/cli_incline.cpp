//=============================================================================
// cli_incline.cpp 
// 
// This program allows you to set the incline via command line.
//=============================================================================
#include "Shm_vars.h"
#include <string>
#include <iostream>
#include <string>
#include <sstream>

int main (void)
{
  SHM::connect_existing_shm();
  int user_input_incline = SHM::req_incline->get();
  std::string user_input_string = "";
  for (;;)
  {
    std::cout << "Valid range is from -85 (overhang) to 15 (slab) degrees." 
              << std::endl
              << "req_incline: "  ;

    std::cin >> user_input_string;

    std::stringstream ss;
    ss << user_input_string;
    ss >> user_input_incline;

    SHM::req_incline->set(user_input_incline);
    std::cout << std::endl 
              <<"setting requested incline: " <<  user_input_incline
              << std::endl;
  }
 return 0;
}
