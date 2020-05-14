# Arduino based Minecraft 3D Printer 

## This is an Arduino Based Minecraft 3D Printer

So some of you asked me for the sketch and a tutorial
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

## How the gcode works
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

First you read the file line by line looking for a carriage
return ('\r') then all the read characters are inserted into a string.
Then the program checks that that string starts with 'G' (**in the program you can actually change this, or add more letters/command types**)
If it doesn't then the command is deleted and then starts
looking for a new line. If it does, then calls the next function
"CommandData" with the line just read as a parameter.

This function then removes the comment if it has one, this is done by
asking the string if it has a ';', if it does, then sets the length 
of the string from the beggining to the semicolon position - 1.
If it doesn't the length is the position of the next carriage return -1.

Once we have the command separated from the comment, it's time to
separate the command into it's parts.

The Gcode has it's parts separated by espaces, so in order to take the parts
of the command we just need to look for the spaces and save the string read
into a new string, then delete the part we just took from the original string
and keep looking for new parts in the command until the command string is empty.
All this data is saved into an Array of 5 Strings.

With our command separated into parts it's time to convert all the strings into
something we can work with, The first string is the Code, this part will tell the
arduino what has to do with the attributes we send to it.

We do that by checking the Code with a list of codes we have at the beggining
of the program, called "ValidCmds" and at the moment just recognizes 2 commands:
G1 (linear movement) and G28 (home all axes), If the command we just sent doesn't
have one of these codes then the command is ignored, but if it doesn't then starts
looking into the array of strings one by one.

Each string in the array is checked against some parameter prefixes, if the command
starts with a known prefix then is decoded, first it removes the prefix in order to 
have a string with only numbers.

X and Y are prefixes that refer to X and Y movements, **these movements are ignored if the printer wants to move less than a block in any given direction**
in other words, if the amount of movement is < 1 then the printer ignores theres a movement in that axis.

Since the Z axis always moves 1 by 1 the printer just needs to know if the value is > 0.

The E parameter, the amount of extrussion is only needed to know if it wants to extrude or not, so if one of the parameters starts with E the printer knows
that has to extrude, we dont care about the amount since every command tell us if it needs to extrude or not.

If a command says it wants to move but does not extrude, then is a simple movement. 

All the parameters are then returned and stored in a data type.

The next function is the CmdExec which calls the function associated with it's name.

Next I'm going to explain 



