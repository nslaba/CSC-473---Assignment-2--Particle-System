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

The following are particles loaded in the form of a cube and connected by springs:

![Screenshot (9)](https://user-images.githubusercontent.com/77686772/227581450-bea8de0e-c545-4da5-873a-6db9602a5523.png)

The following are many particles randomly loaded:

![Screenshot (11)](https://user-images.githubusercontent.com/77686772/227581964-27a5e9e3-f266-42af-8cbe-af2bcc2f8f90.png)

After the simulation time starts, the particles' behaviour is determined based on gravity, their damping, stiffness forces and other:

![Screenshot (15)](https://user-images.githubusercontent.com/77686772/227582281-b697ca71-aaca-40f5-aeae-b2996141f620.png)

## Functionality
My project contains a particle system which is a c++ class that handles everything related to the particles besides movement. The particles are c++ structs.
The simulator in my project deals with the movement of the particle's based on their spring and other forces. Springs are C++ structs containing the nescessary parameters.

All of my integrators (verlet, symplectic and euler) are therefore done in the simulator. The simulator also accounts for the user input simulation time vs the system's simulation time. If the user inputs a simulation time that is less than the system's simulation time, then my simulator has to find the remaining difference between the two such that the system's positions update the correct amount of time before display.


## Grade received: 
100% :)
