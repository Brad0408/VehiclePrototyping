# Unreal Engine Tank

## Overview

Creating a tank vehcile within Unreal Engine 5.2.1

First time looking a Unreal's VehiclePawns and ChoasWheels/Plugins.

Also first time refactoring functionality into C++ after getting it to work within a blueprint.

([Tank.cpp](https://github.com/Brad0408/VehiclePrototyping/blob/main/Source/Vehicle/Private/Tank.cpp) is the bulk of functionality for this project)

FOR C++ (Go to Source -> Vehicle -> 'Private' & 'Public') or (https://tinyurl.com/9e5z3rdh)




## Gameplay

<ins> Here is a video of the tank working below ! - </ins>


[![Tank](https://i.ytimg.com/vi/kPaiKcoHShI/maxresdefault.jpg?sqp=-oaymwEmCIAKENAF8quKqQMa8AEB-AH-CYAC0AWKAgwIABABGEsgVChlMA8=&amp;rs=AOn4CLBq3-GsuVV9gS1V0YG3h2Mka04BHg)](https://youtu.be/kPaiKcoHShI)



Here is also a development playlist of testing newly implemented mechanics - https://www.youtube.com/playlist?list=PLYBjgWvMiW73fJYIZKgGRJQZQmpUzUtOW


The tank is capable of -

- Driving forwards and backwards and can steer
- Shooting a tank shell that can blow up certain environements
- Tank Shell can also damage other tanks and push them around
- Land mines that can damage the tank
- The tank will blow up at 0 HP
- Toggle camera in 3rd or 1st person
- Also controller support that provides haptic feedback when driving and feeling explosions from land mines/tank shells

All visual and audio assets for the tank were sourced online

## Compliling + Cloning

As this is an Unreal Project normal steps can be taken to clone an Unreal Project
1) Clone the project and stay on the main branch
2) Double click 'Vehicle.uproject' once cloned to open the Unreal project (Generated files if prompted and have Unreal 5.2.1 downloaded)
3) When in engine the game can be played like normal with the green play arrow at the top
