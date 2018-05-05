// ============================================================================
//  kiosk_sim.h
//
//  This class is the primary workhorse of the entire project.  Its manages: 
//    - opening a proper serial connection to the climbing wall. 
//    - sending commands to the climbing wall, tracking climbing wall state.
//    - coordinate new requests for changes in speed and incline with pending
//      requests and previous climbing wall state. 
//
//  The class name "kiosk_sim" reflect the motivater for the design. 
//  Primarally to:
//    1) observe the behavior of the original manufacturer provided kios, and 
//    2) to emulate that behavior as closely as possible.  
//  
//  This remains an ongoing challenge.
// ============================================================================
#pragma once
#include <termios.h>  // provides termios type.
#include <unistd.h>   // needed for termios.
#include <string>     // provides std::string 
#include <fstream>
#include <vector>

#include "Command.h"  // climbing wall commands

class kiosk_sim
{
  public:
  // constructors
  kiosk_sim (void);
  
  void set_serial_port(std::string serial_port_absolute_path);
  void connect(void);
  void run(); // main processing loop.

  private: 
  
  void handle_need_halt(void);
  void handle_invalid(void);
  void handle_pending(void);
  void handle_success(void);

  void transmit(Command c);      
  void receive();      
  
  enum Comm_state {success, pending, invalid, need_halt};
  Comm_state comm_state;

  int state_repeat_count; 

  // requested by the user, not yet sent to the climbing wall.
  int requested_halt;
  int requested_incline;
  int requested_speed;

  // sent to climbing wall, not yet complete. 
  int pending_halt;
  int pending_incline;
  int pending_speed;

  // sent to climbing wall and completed successfully.  
  // if COMM_STATE is success, this is the current state of the climbing wall.
  int actual_halt;
  int actual_incline;
  int actual_speed;

  Command pending_incline_command;
  Command pending_speed_command;

  std::vector<unsigned char> reply;

  Command request_status_command;
  Command halt_command;

  std::string path;      //serial port absolute path.
  int serial_fd;         //once open, serial port file descriptor. 
  struct termios original_tty_state;
  
  //===========================================================================
  // infrastructure for testing new ideas.  ephimeral changes, logically
  // distinct from normal opperations.
  //===========================================================================
  bool should_perform_test;
  void perform_test(void);
};
