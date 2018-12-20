# LearnToSolder2018

## Introduction

In late 2018/early 2019 I taught a learn to solder class at the company I work for (CSI - www.csi360.com) because I thought it would be something enjoyable that I could teach my coworkers. Most of the class attendees had never soldered before. I designed a special circuit board for this class which mimics the functionality of one of the devices the company sells, but in miniature.

This site contains all of the information related to the class. If you want to learn more about how to solder, if you want to know what you need to purchase so you can solder at home, or if you want to make more of these boards for yourself or your organization, this site has everything you'll need.

If you want to reach Brian Schmalz (brian@schmalzaus.com) to ask any questions, please do so. 

All text files and designs on this site are Open Source. You are free to use these files and do whatever you wish with them.

## Design Details

The circuit board is designed in the shape of an OAD (Orbital Atherectomy Device). It has a speed switch, which wakes it up from sleep and allows you to select one of three speeds for the motor. The start/stop switch turns the motor on and off. The three green speed LEDs indicate which speed is currently selected. A small microcontroller has code which reads the switches and controls the LEDs and motor. A small FET is used to PWM the motor so various speeds can be achieved. The 3V lithium battery provides power to the circuit. The microcontroller stays asleep until the speed switch is pressed, and goes back to sleep (turning off the motor and LEDs) after 30 seconds of inactivity.

The code for the PIC microcontroller is included in this git repo, as well as the resulting HEX file. If you want to make this board on your own, you will need to program the microcontrollers with this HEX file. An ISCP connector footprint is provided under the battery holder so that the micro can be programed from the back by pressing in a 1x5 .1" header.

## CSI Learn T oSolder 2018 Kit Contents

* Qty 1 Bare Circuit Board (see above image for back and front views of the bare board) - note that the board used for the class already had the surface mount parts (3 of them) soldered on before the students got the boards.
* Qty 1 Speed Switch
* Qty 1 Start Switch
* Qty 3 Green LEDs
* Qty 1 Battery Holder
* Qty 1 Battery
* Qty 1 vibration motor
* Qty 1 black zip tie
* Qty 4 stick on rubber feet

## Tools

To solder electronic kits together, you will need (at a minimum)

### Soldering Iron

We do not recommend buying a soldering iron unless it is 'temperature controlled'. This means you get to set the tip temperature with a knob or buttons, and the iron will only heat up as much as it needs to in order to maintain that temperature. Using a non-temp controlled iron has, in our experience, led to lots of frustration and failed kits.

This is the iron we recommend, as we've had great experience with it and it is very inexpensive : [https://hobbyking.com/en_us/soldering-station-with-adjustable-heat-range-us-warehouse.html]

You can also by this same type of iron from many other places, including Amazon and E-bay, but the price tends to be a bit higher. Some of the companies listed below under Other Soldering Kits also sell tools like irons. There are also many other types of temperature controlled soldering irons which will work just as well.

### Solder

For beginners, we strongly recommend using leaded solder (rather than lead-free). The safety risk is very minimal (just be sure to wash your hands with soap when you are done) and it is far easier to get good soldered joints with leaded solder when you're just starting to learn soldering. You can buy leaded electronic solder at many on-line stores like SparkFun [https://www.sparkfun.com/products/9161] or Adafruit [https://www.adafruit.com/products/1886]. Make sure to buy 60/40 or 63/37 (those numbers represent the ratio of tin to lead in the solder) electronic solder - there are other solders out there (like silver solder for jewelry for example) that won't work for electronics.

### Brass Sponge

When you buy your iron, buy a bras sponge if the iron doesn't already come with it. These are much better than the watter sponge that comes with the iron for cleaning the tip of the soldering iron.

### Wire Cutter

All of the companies listed below sell small wire cutters, or you can get them locally at hardware stores too. The best kind is something that will allow you to cut off wires flush with the board - sometimes called 'flush cutters'. For example [https://www.sparkfun.com/products/11952] or [https://www.adafruit.com/products/152]

### Other tools

There are lots of other tools you can get to help you while solder. Third-hands can be very useful. Needle nose pliers, solder suckers, magnifying glasses, solder wick, wire strippers, etc.

## Kit Assembly Process (overview)

There are only a couple steps to assembling this kit, as it is very simple. If you have done soldering before, you can just walk through this overview of the steps to complete your kit. If you want to learn more about how to do the actual soldering of a joint, the following websites and resources will give you plenty of background materials and explanations:

https://learn.sparkfun.com/tutorials/how-to-solder---through-hole-soldering

http://www.howtogeek.com/63630/how-to-use-a-soldering-iron-a-beginners-guide/

https://cdn-learn.adafruit.com/downloads/pdf/adafruit-guide-excellent-soldering.pdf

### Steps:

1. Insert the two switches into the board. Make sure the switch pins go fully into the board from the top side to the back side. The switches snap down into the board - you sometimes have to press them in hard to get them to snap. Solder the four pins of each switch on the back side of the board.

2. Insert the battery holder to the front of the board, being careful to align the outline of the battery holder to the white outline on the top of the board (there is a square tab on the battery holder than needs to match up to the square tab part of the outline). While holding the battery holder into the board, flip it over and solder both battery holder pins.

3. Insert the three LEDs. Long wires go into square holes. You sometimes have to wiggle the LEDs a bit to get them to press all the way flat down on the top side of the board due to a thick spot on the LED leads. Bend the leads apart on the back side to keep the LEDs in place as you solder the LED wires on the back side of the board.

4. Add solder to the tops of the two motor pads so they form little mounds. Then reheat each mound in turn, inserting a motor wire while the solder is molten. Remove your iron and let the solder solidify without moving the motor wire. Do the same for the other motor wire.

5. Using the black zip tie, tie the motor to the board. There are two small holes in the board on either side of the motor. Put the zip tie through these holes, with the head of the zip tie on the top of the board. Cinch it down tight on the motor, but before you do so, make sure the black rubber sheath around the motor is pushed back away from the rotating weight. Clip the excess zip tie off.

6. Snap the start switch cap onto the start switch.

7. Insert the battery into the holder, flat shiny side up.

8. Put the four rubber sticky feet on the bottom of the board in the areas indicated with the white circles.

Now you can press the speed switch to wake up the OAD and select a speed (low, medium, high), then use the start switch to start and sto the motor. After 30 seconds of inactivity, the OAD will go back to sleep

## Other Soldering Kits

There are SO MANY kits available today, it's impossible to list all of them, or even the good ones. If you do a Google search for "electronic soldering kit" you will find lots of them out there. Some are easier than others, and some come with great instructions (and some don't).

We buy a lot of high quality soldering kits from the following on-line companies:

Evil Mad Scientist [http://shop.evilmadscientist.com/]
* Their Boldport Buggy is excellent [http://shop.evilmadscientist.com/]
* The Flickery Flame is a great beginner kit [http://shop.evilmadscientist.com/productsmenu/793]

Spark Fun Electronics [https://www.sparkfun.com/]
* They have a whole catagory of 'kits', some of which are soldering kits [https://www.sparkfun.com/categories/157]
* The SparkFun Simon Says - Through-Hole kit is really fun [https://www.sparkfun.com/products/10547]

Adafruit [https://www.adafruit.com/]
* The Drawdio is quite fun [https://www.adafruit.com/categories/28]
* Their Minty Boost is actually quite useful [https://www.adafruit.com/products/14]

SeeedStudio [http://www.seeedstudio.com/]
* Their 4-Soldering Zoo Kit looks super cute [https://www.seeedstudio.com/item_detail.html?p_id=1950]
