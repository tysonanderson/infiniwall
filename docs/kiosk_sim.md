## kiosk sim design document

This document will outline the design for a c++ class:
Kiosk_sim

This purpose of this class is to mimic the communication patterns of the sim as closely as possible.  
I hope that as my communications program gets closer and closer to the behavior of the kiosk, some 
of my longstanding bugs will take care of themselves. 

### COM State

Comm state will be managed with an enum vaiable:

enum COMM_STATE
  1) success  (37)
  2) pending  (77)
  3) invalid  (17)
  4) need_halt  (57)
  
int repeat_state_count // simple counter of how many ticks the state has remained as it is.
  
there will be a main loop.  The main loop will wake every one second and check the state. 

main loop: 

#### if the state is need_halt:  void handle_need_halt()

  1)   -  send halt,    usleep (200000)
  2) change state to invalid.

#### if the state is invalid:  void handle_invalid()

  1) if state is invalid and repeat state count is greater than 3...
  2)   -  handle_need_halt()
  3) read requested incline, speed from SHM.
  4) send incline,   usleep (200000)
  5) send speed,   usleep (200000)
  6) send incline,   usleep (200000)
  7) send speed,   usleep (400000)
  8) increase state count.
  9) handle_pending()
  
#### if the state is pending:  void handle_pending()
  6) send chirp (95)(95),  explicitly do not sleep.
  7) read status.  
  8) Verify bytes (00)(01)(02) - (77 or 37)(speed)(incline)
  9) if (speed and incline are correct) and global status is 37, change state to success
  10) else if (speed and incline are correct) and global status is 77, change state to pending.
  11) else change status to invalid.
  12) sleep (800 ms) 
  13 break.
  
#### if the state is success:  void handle_success()
  1) read requested incline, speed from SHM.
  2) if requested speed and incline match previous...
  3)   handle_pending()
  4) else 
  5)   void handle_invalid()

### additional considerations:

as far as integers to store speed and incline, we actually need three pair
  1) requested speed, incline - requested by the user, not yet sent to the climbing wall. 
  2) pending - speed, incline sent to the climbing wall.  not yet successful, in progress
  3) actual - speed, incline sent to the climbing wall, successfull.  the current state of the device.
  
  Command pending_speed_command;        //use _data[1] to verify.
  Command char pending_incline_command;
  

  
