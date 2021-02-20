## Serial Protocol

This document describes everything known about serial communication between the 
computer and the climbing wall.  There was no formal documentation in this 
regard, so all values have been gathered through personal experience.  When 
reading this document, keep in mind that there is room for skepticism and
alternate interpretations. 


### Knonw Commands (from computer to device)

All known commands share some common properties.  Commands are made up of a 
small number of bytes sent all at once.  I've seen commands that are two and 
three bytes in length.  The leftmost byte always seems to uniquely identify the 
command type.   


#### Special Command: Halt

Speculative Purpose   | Serial Command (hex bytes)
----------------------|----------------------------
Halt                  | (89) (89)

The Halt command seems to be a special "emergency stop" type of command.  It 
sets the speed to zero, but it also has the capacity to interrupt and cancel 
commands which are in progress and pending on the climbing wall.  Especially 
any pending changes to incline. 

Halt is occasionally used to transition between modes, so it's use may be more 
complex than a simple halt. for example, sometimes we send halt and then 
immediatly send new incline and speed info before our next chirp.


#### Special Command: Chirp / Request status.

Speculative Purpose   | Serial Command (hex bytes)
----------------------|----------------------------
Ping/Chirp            | (95) (95) 
Ping/Chirp            | (5B) (5B) 

Roughly speaking this command is sent from the computer to the climbing wall to 
elicit a response.  I've seen two different byte patterns being used this way, 
in one of two modes.  

I've come to think of the communication as being two different modes of 
opperation, identified by the chirp being used.  

(95) (95), This mode is simpler and is used by the kiosk initially after 
           startup.
(5B) (5B), This mode is more complex and is used during timed sessions.  Here 
           the computer constantly sends time info to the climbing wall in 
           the form of a countdown timer.  
  
           I have not spent much time trying to understand this mode of 
           opperation.


#### Set Speed

Speed Feet Per Minute | Serial Command (hex bytes) 
----------------------|----------------------------
00                    | (8e) (00) (8e)
01                    | (8e) (03) (91)
02                    | (8e) (06) (94)
03                    | (8e) (09) (97)
04                    | (8e) (0c) (9a)
05                    | (8e) (0f) (9d)
06                    | (8e) (12) (a0)
07                    | (8e) (15) (a3)
08                    | (8e) (18) (a6)
09                    | (8e) (1b) (a9)
10                    | (8e) (1e) (ac)
11                    | (8e) (21) (af)
12                    | (8e) (24) (b2)
13                    | (8e) (27) (b5)
14                    | (8e) (2a) (b8)
15                    | (8e) (2d) (bb)
16                    | (8e) (30) (be)
17                    | (8e) (33) (c1)
18                    | (8e) (36) (c4)
19                    | (8e) (39) (c7)
20                    | (8e) (3c) (ca)
21                    | (8e) (3f) (cd)
22                    | (8e) (42) (d0)
23                    | (8e) (45) (d3)
24                    | (8e) (48) (d6)
25                    | (8e) (4b) (d9)
26                    | (8e) (4e) (dc)
27                    | (8e) (51) (df)
28                    | (8e) (54) (e2)
29                    | (8e) (57) (e5)
30                    | (8e) (5a) (e8)
31                    | (8e) (5d) (eb)
32                    | (8e) (60) (ee)
33                    | (8e) (63) (f1)
34                    | (8e) (66) (f4)
35                    | (8e) (69) (f7)
36                    | (8e) (6c) (fa)
37                    | (8e) (6f) (fd)
38                    | (8e) (73) (00)
39                    | (8e) (76) (03)
40                    | (8e) (79) (06)
41                    | (8e) (7c) (09)
42                    | (8e) (7f) (0c)
43                    | (8e) (82) (0f)
44                    | (8e) (85) (12)
45                    | (8e) (88) (15)
46                    | (8e) (8b) (18)
47                    | (8e) (8e) (1b)
48                    | (8e) (91) (1e)
49                    | (8e) (94) (21)
50                    | (8e) (97) (24)

The leftmost byte (8e) is a command identifier, and the following two bytes are
data.  The data is a two byte long value.  Expressing this in feet per minute is
arbitrary, but we can see each foot per minute is 0x0303 units apart.  When 
performing addition and subtraction, note that we do not carry or borrow between
bytes.  overflow and underflow between the high and low byte is simply dropped.

Finally, while you can set the speed to zero using the speed command.  doing so 
requires finess to get the climbingwall to behave well.  It's probobly best to 
just HALT instead. 

Note: you can not change the incline of the climbing wall whilst the speed is 
zero.  The treads must be crawling at some non-zero speed to adjust the incline. 

#### Set Incline

Incline degrees       | Serial Command (hex bytes) 
----------------------|----------------------------
15 slab               | (92) (00) (92)
14 slab               | (92) (02) (94)
13 slab               | (92) (04) (96)
12 slab               | (92) (06) (98)
11 slab               | (92) (08) (9a)
10 slab               | (92) (0a) (9c)
09 slab               | (92) (0c) (9e)
08 slab               | (92) (0e) (a0)
07 slab               | (92) (10) (a2)
06 slab               | (92) (12) (a4)
05 slab               | (92) (14) (a6)
04 slab               | (92) (16) (a8)
03 slab               | (92) (18) (aa)
02 slab               | (92) (1a) (ac)
01 slab               | (92) (1c) (ae)
00 vertical           | (92) (1e) (b0)
01 overhang           | (92) (20) (b2)
02 overhang           | (92) (22) (b4)
03 overhang           | (92) (24) (b6)
04 overhang           | (92) (26) (b8)
05 overhang           | (92) (28) (ba)
06 overhang           | (92) (2a) (bc)
07 overhang           | (92) (2c) (be)
08 overhang           | (92) (2e) (c0)
09 overhang           | (92) (30) (c2)
10 overhang           | (92) (32) (c4)
11 overhang           | (92) (34) (c6)
12 overhang           | (92) (36) (c8)
13 overhang           | (92) (38) (ca)
14 overhang           | (92) (3a) (cc)
15 overhang           | (92) (3c) (ce)
16 overhang           | (92) (3e) (d0)
17 overhang           | (92) (40) (d2)
18 overhang           | (92) (42) (d4)
19 overhang           | (92) (44) (d6)
20 overhang           | (92) (46) (d8)
21 overhang           | (92) (48) (da)
22 overhang           | (92) (4a) (dc)
23 overhang           | (92) (4c) (de)
24 overhang           | (92) (4e) (e0)
25 overhang           | (92) (50) (e2)
26 overhang           | (92) (52) (e4)
27 overhang           | (92) (54) (e6)
28 overhang           | (92) (56) (e8)
29 overhang           | (92) (58) (ea)
30 overhang           | (92) (5a) (ec)
31 overhang           | (92) (5c) (ee)
32 overhang           | (92) (5e) (f0)
33 overhang           | (92) (60) (f2)
34 overhang           | (92) (62) (f4)
35 overhang           | (92) (64) (f6)
36 overhang           | (92) (66) (f8)
37 overhang           | (92) (68) (fa)
38 overhang           | (92) (6a) (fc)
39 overhang           | (92) (6c) (fe)
40 overhang           | (92) (6e) (00)
41 overhang           | (92) (70) (02)
42 overhang           | (92) (72) (04)
43 overhang           | (92) (74) (06)
44 overhang           | (92) (76) (08)
45 overhang           | (92) (78) (0a)
46 overhang           | (92) (7a) (0c)
47 overhang           | (92) (7c) (0e)
48 overhang           | (92) (7e) (10)
49 overhang           | (92) (80) (12)
50 overhang           | (92) (82) (14)
51 overhang           | (92) (84) (16)
52 overhang           | (92) (86) (18)
53 overhang           | (92) (88) (1a)
54 overhang           | (92) (8a) (1c)
55 overhang           | (92) (8c) (1e)
56 overhang           | (92) (8e) (20)
57 overhang           | (92) (90) (22)
58 overhang           | (92) (92) (24)
59 overhang           | (92) (94) (26)
60 overhang           | (92) (96) (28)
61 overhang           | (92) (98) (2a)
62 overhang           | (92) (9a) (2c)
63 overhang           | (92) (9c) (2e)
64 overhang           | (92) (9e) (30)
65 overhang           | (92) (a0) (32)
66 overhang           | (92) (a2) (34)
67 overhang           | (92) (a4) (36)
68 overhang           | (92) (a6) (38)
69 overhang           | (92) (a8) (3a)
70 overhang           | (92) (aa) (3c)
71 overhang           | (92) (ac) (3e)
72 overhang           | (92) (ae) (40)
73 overhang           | (92) (b0) (42)
74 overhang           | (92) (b2) (44)
75 overhang           | (92) (b4) (46)
76 overhang           | (92) (b6) (48)
77 overhang           | (92) (b8) (4a)
78 overhang           | (92) (ba) (4c)
79 overhang           | (92) (bc) (4e)
80 overhang           | (92) (be) (50)
81 overhang           | (92) (c0) (52)
82 overhang           | (92) (c2) (54)
83 overhang           | (92) (c4) (56)
84 overhang           | (92) (c6) (58)
85 overhang           | (92) (c8) (5a)
86 overhang           | (92) (ca) (5c)
87 overhang           | (92) (cc) (5e)
88 overhang           | (92) (ce) (60)
89 overhang           | (92) (d0) (62)
90 overhang           | (92) (d2) (64)

The leftmost byte (92) is a command identifier, and the following two bytes 
are data.  The data is a two byte long value.  Expressing this in degrees is 
arbitrary, but we can see each degree is 0x0202 units apart.  When performing 
addition and subtraction, note that we do not carry or borrow between bytes.  
overflow and underflow between the high and low bytes are simply dropped.


#### Response from climbing wall

Response data from the climbing wall obeys some rules, let's examine a sample 
response:

  - 0x017 - status byte, here status 17
  - 0x00A - repeat high order byte of most recent set speed command. 
  - 0x014 - repeat high order byte of most recent set incline command. 
  - 0x035 - checksum.  here x017 + x00A +x014 = x035

known status byte values
  - success   (37)
  - pending   (77)
  - invalid   (17)
  - need_halt (57)
