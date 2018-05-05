climbing_wall
=============

This project implements an open-source controller for the Ascent FunRock motorized climbing wall.  The software currently targets the linux platform.  Please review the open issues before running this code.

![heartrate graph](/docs/climbing_wall.jpg)

## basic operation 
The code allows for control of the climbing wall's two motors:
  - one for speed
  - one for incline
  
This allows for the creation of workout sessions that follow pre-scripted sequences.  As well as session which are not pre-scripted, such as using the climber's real time heart-rate to dynamically adjust the speed or slope of the climbing wall. 

## logging and graphs: 
The code also allows for recording of logs and generation of graphs based on the log data. 
![heartrate graph](/docs/heartrate.svg)
![speed and incline graph](/docs/speed_incline.svg)
