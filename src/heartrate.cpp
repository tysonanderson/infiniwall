// This code was imported to the project as an example driver for the usb 
// heartrate monitor, and modified to work with the rest of this project.
// 
// the setup I use is a sparkfun.com HRMI, by danjuliodesigns.com
// which has unfortunately been retired form the catalog and is no longer for 
// sale.  
// 
// in Dan's code drop, this file was originally called: unix_simple.c

/*
 * Demonstration program for communicating with the HRMI over a serial interface
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Shm_vars.h"

using std::string;
using namespace std;


// Define a constant specifying the largest response string we could get 
// from the HRMI
#define MAX_RSP_CHARS  140


// Global to hold original serial port attributes
static struct termios gOriginalTTYAttrs;


//
// OpenSerialPort: Routine to open and configure the serial prot
//
static int OpenSerialPort(const char *deviceFilePath)
{
    // variables
    int fd = -1;              // file descriptor for serial port
    struct termios options;   // serial port configuration options

    // Open the serial port
    if ((fd = open(deviceFilePath, O_RDWR | O_NOCTTY )) == -1) {
        printf("Error opening serial port %s - %s(%d)\n",
               deviceFilePath, strerror(errno), errno);
        return(-1);
    }

    // Prevent other processes from opening the serial port
    if (ioctl(fd, TIOCEXCL) == -1) {
        printf("Error setting TIOCEXCL on %s - %s(%d)\n",
               deviceFilePath, strerror(errno), errno);
        return(-1);
    }

    // Get the serial port current options and save them to restore on exit
    if (tcgetattr(fd, &gOriginalTTYAttrs) == -1) {
        printf("Error getting tty attributes %s - %s(%d)\n",
               deviceFilePath, strerror(errno), errno);
        return(-1);
    }

    // Configure the serial port
    options = gOriginalTTYAttrs;
    //   Set raw input (non-canonical) mode, with reads blocking until either a
    //   single character has been received or a one second timeout expires
    cfmakeraw(&options);
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 10;
    //    Set the baud rate and word length
    cfsetspeed(&options, B9600);
    options.c_cflag |= (CS8);
    //    Cause the new options to take effect immediately
    if (tcsetattr(fd, TCSANOW, &options) == -1) {
        printf("Error setting tty attributes!\n");
        return(-1);
    }

    return(fd);
}



//
// CloseSerialPort: Close our connection and restore the original settings
//
void CloseSerialPort(int fd)
{
    // Block until all written output has been sent from the device
    if (tcdrain(fd) == -1) {
        printf("Error waiting for drain - %s(%d)\n", strerror(errno), errno);
    }

    // Reset the serial port back to the state in which we found it
    if (tcsetattr(fd, TCSANOW, &gOriginalTTYAttrs) == -1) {
        printf("Error restoring tty attributes - %s(%d)\n", strerror(errno), errno);
    }

    // Close the port
    close(fd);
}



//
// SendGetHeartRate: Function to send a command to get the number of heart rate values
//                   specified in NumEntries
//
//                   Returns 1 for success, 0 for failure
//
int SendGetHeartRate(int fd, int NumEntries)
{
    char SendCommand[8];      // Array sized to hold the largest command string
    int  CmdLength;           // Number of characters in the command string

    // Validate NumEntries
    if (NumEntries < 0)
        NumEntries = 0;
    else if (NumEntries > 32)
        NumEntries = 32;

    // Build the command string
    //   Note: "\015" is the carriage return character
    CmdLength = sprintf(SendCommand, "G%0d\015", NumEntries);

    // Send the command string
    return(write(fd, SendCommand, CmdLength) == CmdLength);
}



//
// GetResponseString: Function to read a response string back from the HRMI
//
int GetResponseString(int fd, char* ResponseString)
{
    char b[2];
    int i = 0;

    do {
        int n = read(fd, b, 1);     // read a char at a time
        if (n == -1)
            return(-1);             // read failed
        if (n == 0) {
            usleep(10 * 1000);      // wait 10 msec before trying again
            continue;
        }

        ResponseString[i] = b[0];   // store the character
        i++;

    // repeat until we see the <CR> character or exceed the buffer
    } while ((b[0] != 0x0D) && (i < MAX_RSP_CHARS));

    ResponseString[i-1] = 0;                   // null terminate the string (replace the <CR>)
    return(0);
}



int main()
{
    SHM::connect_existing_shm();
    int sfd;                      // serial port file descriptor
    int i = 1;                    // loop counter
    char RspBytes[MAX_RSP_CHARS]; // Response string 
    int numBytes;
    //ofstream fout;
    //fout.open ("heart_rate.log");
    std::size_t pos = 0;
    std::stringstream ss;

    int heartrate_curr = 0;

    const int heartrate_period = 30;
    SHM::hr_avg_dur->set(heartrate_period);
    bool heartrage_avg_init = false;
    int heartrate_avg;
    int avg_pos;
    std::vector<int> heartrate_avg_data; 
    heartrate_avg_data.resize(heartrate_period);

    // Open the serial port device associated with the HRMI
    //if ((sfd = OpenSerialPort("/dev/cu.usbserial-A9003PDh")) == -1) {
    if ((sfd = OpenSerialPort("/dev/ttyUSB0")) == -1) {
        return(-1);
    }

    // Send a series of Get Heart Rate commands, each time requesting more history buffer
    // entries
    while (true) {
        if (SendGetHeartRate(sfd, i) == 0) {
            printf("Error: SendGetHeartRate failed!\n");
            break;
        }

        if (GetResponseString(sfd, RspBytes) == -1) {
            printf("Error: GetResponseString failed!\n");
            break;
        } else {
            //printf("Request %2d => %s\n", i, RspBytes);
            //cout << RspBytes << endl;
            string buffer = string(RspBytes); 
            pos = buffer.find(" ");
            ++pos;
            buffer = buffer.substr (pos);     // get from pos to the end
            pos = buffer.find(" ");
            ++pos;
            buffer = buffer.substr (pos);     // get from pos to the end

            ss << buffer;
            ss >> heartrate_curr;

            if (avg_pos >= heartrate_period)
            {
              cout << "flipping with period of " << heartrate_period << " seconds." <<  endl;
              avg_pos = 0; 
              if (!heartrage_avg_init) 
              {
                heartrage_avg_init = true;
              }
            }
            heartrate_avg_data[avg_pos] = heartrate_curr;
            ++avg_pos;

            SHM::hr->set(heartrate_curr);

            if (heartrage_avg_init) 
            {
              int sum = 0;
              for (int i = 0; i < heartrate_avg_data.size(); ++i)
              {
                sum += heartrate_avg_data[i]; 
              }
  
              heartrate_avg = sum / heartrate_avg_data.size();
              
              SHM::hr_avg->set(heartrate_avg);
            }
            else 
            {
              heartrate_avg = heartrate_curr; 
            }

            cout << "Heartrate:   current(" << heartrate_curr
                 << "), average (" << heartrate_avg << ")." <<endl;
            
        }
        sleep(1);
    }

    CloseSerialPort(sfd);
}


