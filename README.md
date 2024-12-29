STEPS TO RUN THE GAME

Building the Game

The game assets (i.e., image, sound, and level data files) are in a folder named Assets.
The way the main routine is written is such that the program will look for this folder in a
standard place (described below for Mac OS X). Users may find
that their environment is set up in a way that prevents the program from finding the
folder. If that happens to you, change the string literal "Assets" in "Marble Madness Game"/MarbleMadness/main.cpp to the full
path name of wherever you choose to put the folder (e.g., "Z:/MarbleMadness/Assets"
or "/Users/fred/MarbleMadness/Assets").

To build the game, follow these steps:

For macOS

Unzip the MarbleMadness-skeleton-mac.zip archive into a folder on your hard drive.
Double-click on provided MarbleMadnessFinalGame.xcodeproj to start Xcode.
If you build and run your program from within Xcode, the Assets directory should be in
the directory yourProjectDir/DerivedData/yourProjectName/BuildProducts/Debug (e.g.,
/Users/fred/MarbleMadness/DerivedData/MarbleMadness/Build/Products/Debug). On
the other hand, if you launch the program by double-clicking on the executable file, the
Assets directory should be in your home directory (e.g., /Users/fred)


(P.S You can update the parameter msPerTick in main.cpp if the game moves too fast!
AND The Level Class and Level Data File section in the Marble Madness Spec specifies how to create your own levels for the game!)

Example Gameplay Shots!



<img width="711" alt="Screenshot 2567-12-29 at 16 00 17" src="https://github.com/user-attachments/assets/0ecc4d5c-298f-480d-a704-c9e8d24853e5" />




<img width="585" alt="Screenshot 2567-12-29 at 16 01 16" src="https://github.com/user-attachments/assets/ac1d2b75-feb0-44a7-aed7-bda1e8511a4d" />


Enjoy :)
