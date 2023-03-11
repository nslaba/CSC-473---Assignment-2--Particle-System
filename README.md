### Spring-Mass Particle System ~ Assignment 2 ~ CSC 473

## Task

Build simple spring-mass particle system on the CPU and a corresponding integrator

## Criteria
- Create a particle system and simulate it in c++
- The simulator should support the following integration methods:  Forward Euler, Symplectic Euler, and Verlet.
- There is a flat ground, oriented along the y-axis, that you should model particle collisions with using penalty forces. 
- The particles can be connected with springs in arbitrary ways and are subject to gravity.
- The system should support an arbitrary number of particles. 
- Some particle's positions should lock based on user input commands

## Summary
This particle system obides exactly to the assignment parameters.

## Functionality
My project contains a particle system which is a c++ class that handles everything related to the particles besides movement. The particles are c++ structs.
The simulator in my project deals with the movement of the particle's based on their spring and other forces. Springs are C++ structs containing the nescessary parameters.

All of my integrators (verlet, symplectic and euler) are therefore done in the simulator. The simulator also accounts for the user input simulation time vs the system's simulation time. If the user inputs a simulation time that is less than the system's simulation time, then my simulator has to find the remaining difference between the two such that the system's positions update the correct amount of time before display.


## Grade received - only submitted now
