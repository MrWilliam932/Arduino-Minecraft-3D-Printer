#Arduino based Minecraft 3D Printer
##This is an Arduino Based Minecraft 3D Printer

So quite a lot of people asked me for the sketch and a tutorial
on how to get the thing to work, well, this is it. I'm gonna
explain every single bit you need to know to get this thing to
work, also, I will tell you how to expand/add more functionality
to the program so you can modify it.

## First Thing to know
The program is WIP (work in progress) which means the program is
not in it's final state, it works, but some stuff has to be done
manually.

For example, This program does not come with a menu where you can
change settings, change the model to print, not even a way to pause
the print if it's not printing correctly.

##How the gcode works
gcode is just a set of instructions which tells the printer/cnc
where it has to go, how fast and at which temperature. 

a gcode command looks like this:

```gcode
	G1 X9.500 Y0.500 E9.00000
```

3D printers seem to just use the G1 Command to do every movement,
this command can contain various parameters if the parameter starts
with X, Y or Z It means where it wants to go (end point) if it 
starts with E mean the quantity of filament it wants to extrude,
for our case we just care about if it's greater than 0, and then
theres another parameter called F that is the speed at wich is going
to move, we don't care about that in this machine.

There are other codes but this is the most important, if I need to
explain another will be explained later, this is what you need to
know for now. 

## How the program works (simplified)

The program reads the gcode file, line by line, then separates
the instructions and its parameters into data that can be
interpreted by the Arduino, Then with the current "extruder"
position and the new point given by the gcode command it
calculates the path to be traveled in order to get to the point.
then each iteration of the line drawing routine a minecraft command
is printed into the game via the setblock command.  

## How the program works (in depth)
As you may already know, conventional 3D printers work by taking 
gcode from an SD Card reading the instructions line by line and 
executing them sometimes it translates to movements, sometimes to
changes in temperature.

This printer works the same way but instead of moving real motors
it converts the gcode commands into Minecraft commands. And that's
done like this:

First you read the file line by line looking for a '\r' (carriage
return) then all the read characters are inserted into a string.
Then the program checks that that string starts with 'G' (in the
program you can actually change this, or add more letters, command
types), If it doesn't then the command is deleted and then starts
looking for a new line. If it does, then calls the next function
"CommandData" with the line just read as parameter.

This function looks 
 