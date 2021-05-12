# Replacement Motor Control

<pre>
Well It finally happened.
On 2021/04/05, the electronics on the control board stopped functioning.  

I'll now be looking to replace the DC motor controllers.  
I'll start with the speed control motor, If I can get that working, I'll proceed to the incline motor.  

What do I know about the speed motor?
it's a Leeson brand Direct Current Permement Magnet Motor.  
RPM 1750
Volts 90
Amps 5
Duty Continuous
Torque 18.0  Inch/Lbs. 
Horse Power: 1/2 
</pre>


### Current Favorites: 

  - [automationdirect motor controller]
with
  - [0-10V voltage converter]


5 mins of searching the web brings up this controller: <br/>
[Omega OMDC-130LC100] DC Motor Speed Control with Reversing Capability

To change directions, I would need a [relay], like this one.

Possibly use the circut from this [stack exchange post]

Another tool that might be useful is a [voltage simulator]

# Shop testing rig.  
There is a youtube channel which discusses testing salvaged dc motors.  
  - [Jeremy Fielding = How to Wire a Large DC Motor and Controll The Speed]

In this video, he demonstrates using a Transformer to control the voltage, and a full bridge rectifier to convert the power.  
His full bridge rectifier is 1000 volts, 50 amps.  

Capacitor in parralell, but is out of scope. 

Note: I have this test with the transformer up and working. 

```sh 
   DC Motor Speed Control
   KBIC-125

   $40
   input 115 V
   output 0-130 VDC
   8 ADC, 16 w heat sink.
```


Reversing switch
maintained reversing switch

[automationdirect motor controller]

<!-- Links -->
[Omega OMDC-130LC100]: https://www.omega.com/en-us/control-monitoring/motion-and-position/speed-controllers/omdc-130-series/p/OMDC-130LC100
[relay]: https://wiki.seeedstudio.com/Grove-SPDT_Relay_30A/#:~:text=The%20SPDT%20Relay(30A)%20is,normally%20closed%20terminal%20have%20continuity.

[voltage simulator]: https://plctools.com/analog-simulator-and-generator-with-lcd-0-10vdc-and-4-20ma/?gclid=CjwKCAjwjbCDBhAwEiwAiudBywzPKOBNzkBl22vg_1nG8vt0JkSl3Sjt2C_kO1iiuhbrtrwZ3A0K1hoClDMQAvD_BwE

[stack exchange post]:https://electronics.stackexchange.com/questions/425653/amplifier-for-10v-led-dimmer-control-using-raspberry-pi

[Jeremy Fielding = How to Wire a Large DC Motor and Controll The Speed]: https://www.youtube.com/watch?v=ASywVBU0ZwE

[automationdirect motor controller]: https://www.automationdirect.com/adc/shopping/catalog/drives_-a-_soft_starters/dc_drives/high_voltage_(%3E_50v)/open_frame_-z-_reversing/gsd7-120-10cr30

[0-10V voltage converter]: http://www.icstation.com/voltage-converter-module-adjustable-converter-power-module-digital-analog-signal-p-12498.html?aid=372
