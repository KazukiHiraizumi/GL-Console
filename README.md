# GL-Console
GL-console pipes console inputs to OpenGL graphic function. It was made to evaluate the V-Sync precision of OpenGL, and it was proved perfect with GLFW. 

## Needed
- libglfw libglfw-dev

## Build
- make  //Half Screen for debugging
or
- make kiosk   //Full Screen

- sudo make install   //copies to /usr/local/bin

## Run
- glcon

## Command
- Format
 - command1 args... ; command1 aggs...   //";" overlays the drawn screen by following commmand
- Draw background command
 - B "region_ratio_to_paint" "gray_level"
- Draw circle command
 - C "center coordinate X"  "center coordinate Y" "gray_level"
- Draw sinusoidal stripes command
 - W "wave_length" "phase" "gray_level_center" "gray_level_180deg"ate 
- Load 3D model data (only .obj available now)
 - L *.obj
- Render preloaded 3D model with rotation
 - R rotX rotY rotZ
