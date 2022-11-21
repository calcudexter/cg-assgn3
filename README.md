# CS475 : Assignment 3(Modelling FMX)
### By Harsh Shah(200050049), Neeraj Jadhav(200050086)

## Run instructions
Clone the repository and in the root directory,
```
make
```
This would create binaries in `bin` folder. To run the binary, change the current working directory to `bin`
```
cd bin 
``` 
and then run the required binary.
```
./fmx-animate
```

# Key bindings :  
## Animation
S : Saving a key frame(appends all the state variables to file `keyframes.txt`)  
L : Loads all the saved keyframes  
P : Plays the animation after interpolating between keyframes
R : Plays the animation and saves the frames to `video_frames` directory

## Cameras
N : Toggle between global camera, Rider Camera(behind rider), and First Person View camera 

## Lights
O : Toggle between lights(four lights in total: 2 global illumination lights, 1 headlight and 1 spotlight)  
I : Turn on/off the current light


## Global rotation
The keys W, A, S, D, Q, E pressed with SHIFT have been used for global rotation. Pressing these
keys change the respective Euler angles as described below. The entire model, even
if rider, bike and track are present, gets rotated using these keys.
Let θx, θy, θz be the Euler angles then
1. W : Decreases θx
2. S : Increases θx
3. A : Decreases θy
4. D : Increases θy
5. Q : Decreases θz
6. E : Increases θz

## Global Translation
The keys 2, 4, 6, 8, 5 have been used for global translation. They function as given
below. The translation caused by pressing these keys is with respect to the rotated
axes (caused by doing global rotation as described in the previous section). Note
that these keys translate only the currently selected object. 
1. 8 : Move towards +Y
2. 2 : Move towards -Y
3. 6 : Move towards +X
4. 4 : Move towards -X
5. 5 : Move towards +Z
6. Shift + 5 : Move towards -Z

## Local rotations
The keys Up, Down, Right, Left, PgUp PgDown have been used for local
rotations. For the rider, they work as follows :
1. Up, Down : Bend forwards or backwards
2. Right, Left : Turn right or left
3. PgUp, PgDown : Bend right or left
For the bike, they work as follows :
1. PgUp, PgDown : Bend forwards or backwards
2. Right, Left : Turn right or left
3. Up, Down : Bend right or left
Similarly for the track, along its axes

## Bike’s movements
Press control and then
1. Pressing and  F and Shift + F rotates the front tire 
2. Pressing R and Shift + R rotates the rear tire
3. Pressing T and Shift + T steers the bike.

## Selecting Parts
### Rider
To select any specific part of rider press the corresponding key as listed below,
1. CTRL + 0(or just B): Select the torso
2. CTRL + 1 : Select the left upper arm
3. CTRL + 2 : Select the left lower arm
4. CTRL + 3 : Select the right upper arm
5. CTRL + 4 : Select the right lower arm
6. CTRL + 5 : Select the left thigh
7. CTRL + 6 : Select the left leg
8. CTRL + 7 : Select the right thigh
9. CTRL + 8 : Select the right leg
10. CTRL +  9 : Select the neck
11. CTRL +  - : Select the head

### Bike
Pressing V selects the bike.

### Track
Pressing C selects the track.

### Bike and Rider
Pressing X selects the bike.

### All objects
Pressing Z selects all objects.

## Scaling the objects
Pressing M and Shift + M scales up and down the selected object respectively.

## Link to video
[Link](https://youtu.be/JisHq4VOFTY)

# Undertaking
We, Harsh Shah and Neeraj Jadhav, swear on river Styx, that we have not copied the code for assignment 2 present in the repository, nor have we indulged in any kind of plagiarism. We have coded all the scripts by ourselves.
