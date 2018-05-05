// ============================================================================
//   kiosk_sim.cpp
// ============================================================================
#include "Command.h"
#include "kiosk_sim.h"

#include <iostream>   // provides cout 
#include <iomanip>    // provides std::hex
#include <sstream>    // provides std::stringstream
#include <stdio.h>    // provides printf

// Open call, and related definitions:
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// ioctls for controlling serial lines other than RxD and TxD.  
#include <sys/ioctl.h>

// our custom interprocess communication.
#include "Shm_vars.h"


kiosk_sim::kiosk_sim()
{
  path = "";
  serial_fd = 0;

  request_status_command.set_to_request_status();
  halt_command.set_to_halt();
  should_perform_test = false;  // don't check in if true. 

  comm_state = need_halt;

  requested_incline = 0;
  requested_speed = 0;

  pending_incline = 0;
  pending_speed = 0;
  
  actual_incline = 0;
  actual_speed = 0;

  pending_incline_command.clear();
  pending_speed_command.clear();
  reply.clear();
  
}


void kiosk_sim::set_serial_port(std::string serial_port_absolute_path)
{
  path = serial_port_absolute_path;
}


void kiosk_sim::connect(void)
{
  if(path == "") 
  {
    return;  // path has not been set, so don't connect.
  }

  // Open the serial port and set its I/O mode to RAW.
  serial_fd = open(path.c_str(), O_RDWR|O_NONBLOCK);

  // save off the original tty state.
  struct termios    tty_state;

  if (tcgetattr(serial_fd, &tty_state) < 0) 
  {
    return;  
  }
  original_tty_state = tty_state;

  // the following code sets tty into raw mode 
  tty_state.c_lflag &= ~(ICANON | IEXTEN | ISIG | ECHO);
  tty_state.c_iflag &= ~(ICRNL | INPCK | ISTRIP | IXON | BRKINT);
  tty_state.c_oflag &= ~OPOST;
  tty_state.c_cflag |= CS8;
  tty_state.c_cc[VMIN]  = 1;
  tty_state.c_cc[VTIME] = 0;
  cfsetispeed(&tty_state, B9600);
  cfsetospeed(&tty_state, B9600);
  if (tcsetattr(serial_fd, TCSAFLUSH, &tty_state) < 0) 
  {
    std::cout << "Error setting raw mode on serial port." << std::endl;
  }

  std::cout << "Establishing connection to the climbing wall..." << std::endl; 

  //set up flags and constants for use in ioctls.
  const int CMD_ON   = TIOCMBIS;
  const int CMD_OFF  = TIOCMBIC;
  const int DTR_FLAG = TIOCM_DTR;
  const int RTS_FLAG = TIOCM_RTS;

  // set DTR (Data Terminal Ready)  I'm ready to communicate. 
  ioctl(serial_fd, CMD_ON, DTR_FLAG);

  // set RTS (Request To Send) Typically used with CTS for flow control, but there
  // doesn't appear to be any flow control.
  ioctl(serial_fd, CMD_ON, RTS_FLAG);
}


void kiosk_sim::run() 
{
  while(true)
  {
    std:: cout << "in run with comm_state: "; 
    switch (comm_state)
    {
      case need_halt:
        std:: cout << "need_halt. " << std::endl;; 
        handle_need_halt();
        break;
        
      case invalid: 
        std:: cout << "invalid. " << std::endl;; 
        handle_invalid();
        break;

      case pending: 
        std:: cout << "pending. " << std::endl;; 
        handle_pending();
        break;

      case success: 
        std:: cout << "success. " << std::endl;; 
        handle_success();
        if (should_perform_test)
        { 
          perform_test();
        }
        break;
    }
  }
}


void kiosk_sim::handle_need_halt(void)
{
  transmit(halt_command);      
  usleep(200000);  // 0.2 seconds.

  comm_state = invalid;
  state_repeat_count = 0;

  handle_invalid();
}


void kiosk_sim::handle_invalid(void)
{
  if (state_repeat_count > 3)
  {
    comm_state = need_halt;
    state_repeat_count = 0;
  } 
  
  requested_incline = SHM::req_incline->get();
  requested_speed   = SHM::req_speed->get();
  requested_halt    = SHM::req_halt->get();

  if (requested_speed == 0)
  {
    pending_speed_command.clear();
    pending_speed_command.set_to_speed(requested_speed);
    transmit(pending_speed_command);  
    pending_speed = 0;
    actual_speed = 0;
    SHM::curr_speed->set(0); 
    usleep(1000000);  // 1 second.
  }
  else
  {
    pending_incline_command.clear();
    pending_incline_command.set_to_incline(requested_incline);
    pending_speed_command.clear();
    pending_speed_command.set_to_speed(requested_speed);

    transmit(pending_incline_command); usleep(200000);  // 0.2 seconds.
    transmit(pending_speed_command);   usleep(200000);  // 0.2 seconds.
    transmit(pending_incline_command); usleep(200000);  // 0.2 seconds.
    transmit(pending_speed_command);   usleep(200000);  // 0.2 seconds.

    pending_incline = requested_incline;
    pending_speed = requested_speed;
    
    if (comm_state == invalid)
    {
      ++state_repeat_count; 
    }
    else 
    {
      comm_state = invalid;
      state_repeat_count = 0;
    }
    handle_pending();
  }
}


void kiosk_sim::handle_pending(void)
{
  transmit(request_status_command);      
  receive();
  
  if (reply.size() >= 2)
  {
    if (
            (reply[1] == pending_speed_command._data[1])    //is speed correct?
         && (reply[2] == pending_incline_command._data[1])  //is incline correct?
       )
    {
      if (reply[0] == 0x37)
      {
        comm_state = success;
        state_repeat_count = 0;
      }
      else if (reply[0] == 0x77)
      { 
        comm_state = pending;
        state_repeat_count = 0;
      }

    }
    else
    {
      if (reply[0] == 0x57)
      { 
        comm_state = need_halt;
        state_repeat_count = 0;
      }
      else 
      { 
        comm_state = invalid;
        state_repeat_count = 0;
      }
    }
  }
  usleep(800000);  // 0.8 seconds.
}


void kiosk_sim::handle_success(void)
{
  actual_incline = pending_incline;
  actual_speed = pending_speed;
  SHM::curr_incline->set(actual_incline);
  SHM::curr_speed->set(actual_speed);

  requested_incline = SHM::req_incline->get();
  requested_speed = SHM::req_speed->get();
  requested_halt = SHM::req_halt->get();
 
  if (
          (requested_incline == pending_incline)  //is incline correct?
       && (requested_speed   == pending_speed  )  //is speed correct?
     )
  {
    handle_pending();
  }
  else
  {
    comm_state = invalid;
    state_repeat_count = 0;
    handle_invalid();
  }
}


void kiosk_sim::transmit(Command c)
{
  int retry_count = 1;
  int bytes_read = 0;
  int write_count = 0; 
  
  while ( (bytes_read <= 0) && retry_count > 0)
  {

    write_count = write(serial_fd, c._data.data(), c._data.size());

    if (write_count == 0) 
    {
      std::cout << "write failed???" << std::endl;
    }
    std::cout << "wrote: " << c.get_printable() << std::endl;
    --retry_count;
  }
}


void kiosk_sim::receive()
{
  { // read reply from the climbing wall 

    static const int MAX_COMMAND_SIZE = 4;
    struct serial_command
    { 
      unsigned char data [MAX_COMMAND_SIZE];
      int size;
    };
    typedef serial_command command;

    command receive_buffer;
    receive_buffer.size = 0;
    int bytes_read = 0;
  
    bytes_read = read(serial_fd, receive_buffer.data, MAX_COMMAND_SIZE); 
    receive_buffer.size = bytes_read;
  
    if (bytes_read > 0)
    {
      std::cout << "read (" << bytes_read << ") bytes: "; 
      receive_buffer.size = bytes_read;
      reply.clear();
    }
    else 
    { 
      std::cout << "no response."; 
    } 
    // copy buffer into reply;
    for (int index = 0; index < receive_buffer.size; ++index)
    {
      reply.push_back(receive_buffer.data[index]);
    }

    for (int index = 0; index < reply.size(); ++index)
    {
      printf("(%02x) ", reply[index]);
    }
    std::cout << std::endl;

  }
}


void kiosk_sim::perform_test(void)
{
  static Command test_speed;
  static Command test_incline;

  std::cout << "===Performing Test===" << std::endl;
  test_speed.set_to_hex_string("8e008e", "set speed zero");
  test_incline.set_to_hex_string("921eb0", "set incline to zero");

  transmit(test_incline);            usleep(200000);  // 0.2 seconds.
  transmit(test_speed);              usleep(200000);  // 0.2 seconds.
  transmit(test_incline);            usleep(200000);  // 0.2 seconds.
  transmit(test_speed);              usleep(400000);  // 0.4 seconds.

  comm_state = invalid;
  state_repeat_count = 0;

  handle_pending();
}


int main (void)
{
  SHM::connect_existing_shm();

  kiosk_sim kiosk;
  kiosk.set_serial_port("/dev/ttyS1");
  kiosk.connect();
  kiosk.run();
  return 0;
}

