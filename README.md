# Physics Engine in C/C++

### Description
This project aims to build a 3D physics engine in plain C/C++ with an integrated graphics rendering

### Getting Started
This project was developped on MacOS, but it should be cross-plateform compatible as it uses cross-plateform compatible librairies. If you are not using an IDE such as CLion, in order to get started, create build folder at the root of the project, `cd` into it and run `cmake ..`. This will create the MakeFile for the project. Once completed, in the build directory, run `make all`, and an executable `physics_engine` should be created. Congratulations! You can now see for yourself if the physics works or not (hopefully it does)! The scene you are seeing is defined and rendered in main.cpp, at the root of the project. Feel free to add, subtract and modify the current scene.

### Features left to implement
-  Batch rendering for "CloudParticuleObject"s
-  Octree for collision detection
-  Object orientation
-  Inertia induced by collisions
-  Variable forces
-  Variable force vector-fields
-  Write real tests


### Author
Emir Tuncbilek
