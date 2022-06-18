Catffeinated
============

Authors: Kaili Shan, Garrett Grossner, Andrew Mariano
University: Cal Poly SLO
Term: Spring 2022
Course: CPE 476 - Real-Time 3D Computer Graphics Software
Instructor: Zoë J. Wood


Project Description
-------------------

Catffeinated is a 3D single player collectathon game. You,
the player, are a cat in a cat cafe looking for some 
heart-shaped kibble. Other cats may try and block your 
path - or even fight you for it. Grab as much kibble as 
you can so you can power up! 

Guide
-----
Controls
* WASD - Move
* Mouse cursor - Look around
* Z - Zoe mode toggle (no health loss)
* P - Polygon mode

The objective of the game is to survive for as long as
possible in the cafe. Running into other cats will reduce 
the player's health, and the game will end if their health 
reaches zero. The player has a small window of time after 
being damaged where they can not be damaged again.

Besides running and avoiding the other cats, the player 
should also be collecting the heart-shaped kibble around 
the map. Collecting 10 kibble lets a player get one of 
their health points back. If they collect kibble fast 
enough, they can even gain a huge burst in speed as well 
as a permanent speed boost.

Technologies
------------
* Animation
* Particle system
* Bounding sphere collision detection
* Multiple light sources
* Toon shader
* Sophisticated HUD/GUI
* Complex pathing
* TV static FBO shader
