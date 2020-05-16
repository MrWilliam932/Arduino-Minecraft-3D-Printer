# Arduino based Minecraft 3D Printer 

## This is an Arduino Based Minecraft 3D Printer

So some of you asked me for the sketch and a tutorial
on how to get the thing to work, well, this is it. I'm gonna
explain every single bit you need to know to get this thing to
work, also, I will tell you how to expand/add more functionality
to the program so you can modify it.

## First Thing to know

**I've only tested this in Minecraft Java Edition**

The program is WIP (work in progress) which means the program is
not in it's final state, it works, but some stuff has to be done
manually.

For example, This program does not come with a menu where you can
change settings, change the model to print, not even a way to pause
the print if it's not printing correctly.


**Also this is not an Arduino Tutorial nor an Electronics tutorial**
Not trying to be rude but, this is strictly a "this sketch only" tutorial and how to make it work. I'm not going to walk you through basic electronics or arduino programming.

## Parts list

You need: 
	*An Arduino Leonardo, Micro OR Pro Micro
	*An SD Card Breakout board
	*An SD Card

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

Next I'm going to explain How the G1 Command works which is the most important bit of code.

G1 Command is the command that tells the printer to move the extruder in a linear fashion in one or two axis at a time.
The function of the command needs to know a number of things:
*The Current Extruder Position
*The Target Position of the extruder
*The Axis that are going to move
*If it has to extrude material or not
*If the command has to be ignored or not

To move all this data in a comfortable way around the program I've made data structures.

One is the GCommand type which looks like this:
```C++
struct GCommand{
  String Code;
  int XPos;
  int YPos;
  int ZPos;
  bool NewX;
  bool NewY;
  bool NewZ;
  bool Extrude;
  bool IgnoreCmd;
};
```

The second one is the Point type:
```C++
struct Point{
  int X;
  int Y;
  int Z;
};
```
So lets get to the drawing thing of the program, the actual calculations the arduino does to print blocks.

First of all let's talk about single axis movements that extrude material:

Single axis movements are really easy to make work, you just need to know what axis do you wanna move and where, then check if your target is greater or less than your extruder's position
so you can draw the line forwads or backwards from your extruder, this is accomplished by initializing the for with your extruder position, and setting the Target position as your stopping condition.
You increment or decrement the for variable depending on the target position < extruder position we mentioned earlier. 

That's it for single axis extrusions, is that easy.

Now for the tricky(-ish) part, two axis movements.

So in order to accomplish this we need a little bit of math (**a really TINY little bit of math I promise**)

We have to points and we have to make a line, so the obvious thing to use is the **two-point formula** wich looks something like this: 
"y = m * x" 
the only thing that we don't have is the "m" wich is the slope or how inclined is the rect, this value is really important because we get rid of a very important problem.
But first let's see how we can know the slope.

m = (Y2 - Y1)/(X2 - X1)

in the program I've separated the numerator and denominator into variables for convenience. In the program looks like this:

```C++
deltaY = Target.YPos - Extruder.Y;
deltaX = Target.XPos - Extruder.X;
m = (deltaY)/(deltaX);
```

My first attempt to make this whole program work was by using directly this function for everything, even single axis extrusions. Which is a mistake and I will tell you why:

first of all the slope is calculated dividing the amount of movement in Y divided by the amount of movement in X. If you try to move in a single axis that means that the slope formula will look something like this:

*m = 0 / something
*m = something / 0

which is going to result into 0 or infinite, if you multiply that by your X, you'll have 0 or infinite and will try to put the block in Mordor and we don't want that. So that's why this formula is not used for everything.

Ok so, If we use this formula for 2 axis extrussions we should be ok right?

### WRONG!

Well, yes, but actually no. Let me explain:

If we want to draw a line using this formula It's easy to asume that we just need to give the formula the X value multiply it by the slope and we shoud be good-to-gol, and that's kinda correct, you see? If your line moves
more in the X direction than in the Y direction the formula works just fine, we give one value of X and It returns a value of Y. BUT if It moves more in the Y direction that means that if you give a value of X it returns
a value of Y, in the next iteration of the for when you give the next value of X it will return a value of Y and it will be correct, BUT the value of Y compared to the last value of Y will be more than one which is going to
leave a gap between one block and the other. (I will post some images of what I mean **If I remember**).
So we have this problem where the formula works but sometimes, only when the travel in the X value is greater than the travel in the Y value. 

So to solve this we have to use another formula, well It's actually the same, but rearranged.

we saw that "y = m * x" is useful when the travel in X is greater than the travel in Y
so rearraging the formula like this "x = y / m"  we can give Y values and get the X values. Exactly like before but relying on the other axis.

So we need to know when to use this formula or the other.

How do we solve this? How do we know when the line is going to travel more in X than in Y?
This is where **the slope** comes to save the day.

The slope tells us how inclined is a line, the more steep is the line, the bigger is the slope.

*if the slope is < 1, the line travels more in X than in Y
*if the slope is > 1, the line travels more in Y than in X1
*if the slope is = 1, the line traves the same in X than in Y0

So using this criteria, we just need to use the value of m to select which formula to use. (just using the absolute value of it so it works on negative slopes)

**I WILL EXPAND THIS SECTION A LITTLE BIT MORE, BUT FOR NOW THIS IS TAKING TOO LONG, NEXT WEEK YOU'LL HAVE MORE INFO ON THIS SUBJECT**

Then the minecraft command is asembled using all the axis data for each iteration of the program. 

It makes me sad and mad that in 3D printers X and Y are the horizontal axes, and Z the vertical one, and in Minecraft FOR SOME REASON, X and Z are the horizontal ones and Y is the vertical, just keep an eye on that when reading the code.

## Guide to Use the Program

### Important things to take into account
	*Remember that the program is not finished, there are probably some bugs left, and not all planned features are done yet.
	*This 3D printer uses Keyboard emulation, that means it "writes commands" for you, and this means you can't move while is printing, you can look around with the mouse but don't move or will misplace blocks since it uses relative coordinates to place blocks
	*Once the arduino detects the card the print will start automatically, so be sure to be in already in minecraft, not paused and __With the keyboard layout set to ENGLISH US__
	*Keep in mind some popups from other programs can, well..., pop up in front of the game, if that's the case the arduino will keep sending commands regardless, so when the arduino "presses" the Enter Key will act over the popup instead of minecraft, so if you have programs that you know that create popups, close them.

### Let's prepare the Slic3r

First you have to download Slic3r from it's webpage, I'm assuming you know how to install stuff, ok once installed, go to File/Load Config, then select the config.ini I left you in the github repo, Keep in mind this will override the current config you have, but only while you have the Slic3r open, to actually save that config into a profile you have to manually save each part individually:
for that you just go to Settings/Print Settings, hit save and give it a name, then the same for Filament Settings and Printer Settings. Then you should be ready to Slice. Insert a 3D model and put them.

Keep in mind that in the Slic3r positive Y is north (in minecraft) and positive X is east (in minecraft) and the printer 0,0,0 of the Slic3r is the position of your feet in minecraft. 

Ok so, put the model facing the correct orientation, then hit, below the plater you can see different views of the print, The Preview It's the most interesting. Once all of this is done, hit the Export Gcode button.

**Since I'm using the default Arduino SD Card Library the names have to be an 8 character name (or less) + 4 character file extension including the '.', 12 characters total (e.g. 3DBenchy.gco)**
remember to copy the name of the file including the extension, to put it in the Arduino program later, you can just write it again... 

Then go ahead and save it to your SD Card.

### Minecraft Preparation

Open your minecraft client, create a Flat World, open F3, and select a point to stay while printing, and keep your Y coordinate.

Now it's time to calculate your Offset from the ground to insert it into the Arduino Sketch, It's calculated like this:

Vertical offset = 0 - ([Your Y] - ([The Start Print Height] - 1))

if you are at 25 your offset should be -22 

### How to use the Arduino Sketch

First of all, open your arduino IDE, then the 3D Printer Sketch.

Go to Line 61 where you can find this bit of code:

```C++
Point Offset = {3,3,-22};
```
this line sets the offset from your feet. Is a way to "move" the plater position.
Insert your vertical offset where the -22 is :P As I mentioned earlier, 3D Printer Coordinates and Minecraft coordinates have the Y and the Z swapped, so your Y offset (in minecraft) should be in the Z offset (in the 3D Printer Program)

Next line you can edit is this:

```C++
int delayT = 100;
```
This line sets the delay between commands, in other words how fast the printer prints. I'd personally leave it like this.

And the next and Final Line to Edit is the Name of the file wich is in line 98:
```C++
dataFile  = SD.open("Sharkie.gco");
```

### How to wire the thing

Ok since this is a prototype, most of the features, and schematic may vary from your set up to mine, but I can tell you how to wire it. 

Wire the SD card module to your arduino (depending on the arduino you'll have the SPI interface in different places)

The tricky thing is that you "need" a SD card module with a Card Detect pin (CD), If your SD card module has a card detect pin conect it to the digital pin 19 of your corresponding arduino, in Micro and Pro Micro seems to be A1, In Leonardo seems to be A5.
If your SD Card does not have a CD pin output you can do 2 things:

	*Solder a wire to the CD pin in the SD Card Slot, There is always one, but for some reason they don't wire it in some breakout boards. When the card is inserted there is a contact that shorts to ground. Find it with a multimeter or something (I did this one)
	*Just Connect manually the pin 19 to ground when the card is inserted.

I have more things in the program, but they are not used yet. Like an Encoder, or buttons to do different stuff. 

Soo I think that's all. I'll be adding more info If I remember more stuff to be told. 

Now the only thing it's left is to load the Sketch to the Arduino (don't insert the SD Card yet) and go into minecraft.




  