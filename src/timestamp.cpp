//=============================================================================
// timestamp.cpp
// 
// saves the current timestamp to shared memory once per second.
// timestamp is in seconds since UNIX epoch. 
//=============================================================================
#include "Shm_vars.h"
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>

std::string time_to_string(time_t timestamp);

int main (void)
{
  SHM::connect_existing_shm();
  time_t clock;
  std::string date_string = "";

  for (;;)
  {
    clock = std::time(NULL);

    SHM::timestamp->set(clock);
    std::cout << "setting timestamp: " <<  clock << std::endl;

    std::cout << "date_string: " <<  time_to_string(clock) << std::endl;

    sleep (1);
  }
 return 0;
}

std::string time_to_string(time_t timestamp)
{
  char buffer [80];
  std::string return_value = "";
  strftime (buffer,80,"%Y_%m_%d_%H-%M-%S",(localtime(&timestamp)));
  return_value = buffer;

  return return_value;
}
