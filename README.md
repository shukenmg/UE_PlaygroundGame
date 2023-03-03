# Jimmy's big adventure!
![image](https://user-images.githubusercontent.com/18000205/222743402-b500fd91-a272-4c5f-9db4-852a1ce217e3.png)

### This is a tech demo with the following features:
- Sprinting
- Gliding
- Grabbing and Puzzles
- Death and Respawn system
- Simple NPC Dialog system
- Sound Effects
- BGM

### All made in C++!

## Sprinting
![image](https://user-images.githubusercontent.com/18000205/222744792-3af1e816-9b1c-4456-bf01-b9349433b6f0.png)

This was simple to create and makes gameplay more dynamic for players.

*Time spent to develop: 20 Minutes*



## Gliding
![image](https://user-images.githubusercontent.com/18000205/222745034-dc7f86c5-3e91-4801-a71d-7511bad7899a.png)

Inspired by BOTW's paraglider, the glider slows the player down and helps them to travel long distances in the air.

If only we had animations for the character... and a better glider model (*no! this glider model is beautiful!* XD)

*Time spent to develop: 30 Minutes*



## Grabbing and Puzzles
![image](https://user-images.githubusercontent.com/18000205/222748520-8eb6c0cb-cfd2-4af6-8e54-7ab23db98065.png)
![image](https://user-images.githubusercontent.com/18000205/222749332-e3d867c2-61c0-434d-b475-b776856d1d3e.png)
![image](https://user-images.githubusercontent.com/18000205/222749211-e5372f31-b510-4e0b-902f-ec6fdc1e69ad.png)

JENGA! But in UE5.

Grab physics cubes with **E** and move the camera around to place them, its that simple!
As implemented in the demo, this can create some fun puzzles to solve for the player!

But, a more precise placement system will be needed to build structures and solve more complicated puzzles, so maybe a system like Magnesis in BOTW could work well.

*Time spent to develop: 1 Hour*



## Death and Respawn
![image](https://user-images.githubusercontent.com/18000205/222745458-402242f8-abfa-4ef1-bade-e9acf2a5431e.png)
![image](https://user-images.githubusercontent.com/18000205/222745522-12130cf5-e473-41a0-98c4-c95c4dc8c048.png)

Death is triggered by the player overlapping an TriggerVolume that has the actor tag "Death", the camera then detaches from the character.

After 1 second the character is teleported back to a saved location and the camera is reattached.

The next step would be to make an interface system to notify other gameplay elements about player Death and Respawn so that they can reset.

This was easy to make, no blockers.

*Time spent to develop: 30 Minutes*



## Simple NPC Dialog
![image](https://user-images.githubusercontent.com/18000205/222746155-8e6243bc-252a-43bb-8d7d-455830c14cd3.png)

A simple dialog box based on a Widget Component.
The text is static and pre-set in the editor:
![image](https://user-images.githubusercontent.com/18000205/222746388-95870027-3457-4dbc-b37d-fecc93db4705.png)

When the text is modified, the widget is updated with new text.

A real dialog system would use DataTables and such but this was just for fun. :)

*Time spent to develop: 30 Minutes*



## Sound Effects
![image](https://user-images.githubusercontent.com/18000205/222747737-8fa23f82-94c7-4322-b7a6-a2317a632706.png)

Made with MetaSounds, these effects are triggered from the player's *AudioComponent*.

Sound effects really makes a game feel alive!

*Time spent to develop: 20 Minutes*



## BGM
![image](https://user-images.githubusercontent.com/18000205/222748045-c73944fa-51e5-48e2-8158-36475630d145.png)

A good OST really helps make a game interesting and lively.

Music credit: https://www.youtube.com/watch?v=UYYk-ywAf2w

*Time spent to develop: 10 Seconds* (Drag and drop) XD
