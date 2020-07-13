# CCDIK Inverse Kinematic

### Nikitha Ann George

### MSc Computer Animation and Visual Effects

### Bournemouth University 2015 - 2016

## Description
CCD (Cyclic Coordinate Descent) is a very fast and reliable inverse kinematics methods that has been extensively used in the computer games industry. The main idea behind the solver is to align one joint with the end effector and the target at a time, so that the last bone of the chain iteratively gets closer to the target.
This project is developed in QTCreator with C++ and OpenGL.
A simple Link structure loaded from "Link.bvh". If a different file must be used, please changed the constructor in the line 34 of NGLScene.cpp in src folder of this folder.The Load file in the widget does the loading.


### Executing program 
Click the REACH TARGET key and it will display the output when the target is set.


### Other
The user is able to toggle between the wireframe and solid view. 

## Help
If there is a copyerror display, untick the Shadow Build checkbox in the Projects-> Build


## Requirements
Project requires Qtcreator and C++ with NGL(University Graphics Library)

