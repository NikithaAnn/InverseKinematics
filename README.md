# CCDIK Inverse Kinematic

## Description
CCD (Cyclic Coordinate Descent) is a very fast and reliable inverse kinematics methods that has been extensively used in the computer games industry. The main idea behind the solver is to align one joint with the end effector and the target at a time, so that the last bone of the chain iteratively gets closer to the target.
A simple Link structure loaded from "Link.bvh". If a different file must be used, please changed the constructor in the line 34 of NGLScene.cpp in src folder of this folder.The Load file in the widget does the loading.


### Executing program 
Click the REACH TARGET key and it will display the output when the target is set.


### Other
The user is able to toggle between the wireframe and solid view. 


## Issues
Display function is broken but algorithm can be verified with the printed values in terminal

## Help
If there is a copyerror display, untick the Shadow Build checkbox in the Projects-> Build

