#! /bin/bash
#==============================================================================
# rmshm.sh 
#
# This script removes all the shared memory entries associated with the 
# climbing wall.
#==============================================================================

TARGET_LIST="curr_halt curr_incline curr_speed duration elevation heartrate heartrate_avg heartrate_avg_duration heartrate_driven heartrate_target log_active req_halt req_incline req_speed start_time timestamp"

echo removing all climbing-wall entries from /dev/shm.
for BIN in $TARGET_LIST 
do 
  echo "removing $BIN"
  rm -f /dev/shm/$BIN
done
