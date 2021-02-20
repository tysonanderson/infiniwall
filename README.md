climbing_wall
=============

This project allows you to control the motorized speed and incline 
of your Ascent FunRock climbing wall from a Linux computer using Serial
communication.  

Please review the open issues before running this code.

![climbing wall](/docs/climbing_wall.jpg)

## project status
While one persistent issue lingers on (likely in the closed-source 
microprocessor firmware itself), the code base is stable, and I have been
happily using variations of it myself for years. 

## basic operation 
The code allows for control of the climbing wall's two motors:
  - one for speed
  - one for incline  
  
This allows for the creation of workout sessions that follow pre-scripted 
sequences.  As well as session which are not pre-scripted, such as using the 
climber's real time heart-rate data to dynamically adjust the speed or 
incline of the climbing wall. 

## logging and graphs: 
The code also allows for recording of logs and generation of graphs based on 
the log data. 
![heartrate graph](/docs/heartrate.svg)
![speed and incline graph](/docs/speed_incline.svg)
