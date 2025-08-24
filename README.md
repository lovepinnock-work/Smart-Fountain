# Smart-Fountain
Combining hardware &amp; software to make a Smart Dog Fountain

Hello Readers,
This is a personal project for a pet very close to my heart named Drake :) 

I bought him a simple dog fountain that got the job done. When first plugged in a blue light would light up, the fountain then pumped water continuously all day until the water either evaporated or was totally consumed by my dogs ravenous thirst. After which the water pump would run dry and squeal alerting humans near by it needed more water. 
I sought to turn this into a Smart water fountain by doing 3 things:

1. Improve water efficiency. Run only when my dog needs water
2. Prevent water pump from running dry. Run only when there was enough water to run.
3. Indicate when water is low. Alert humans without a squeal.

I achieved this through a combination of hardware and software (and good ole fashion soldering)!

- Hardware
1. (1) Arduino UNO rev3
2. (4) Ultrasonic Ping Sensors HC-SR04
3. (2) 5v 1-channel relay switches w/ opto isolation 
4. (1) Non-contact water sensor
5. Jumper wires
6. Soldering Iron
7. Multimeter
8. Construction paper

- Software
1. Arduino IDE
2. C++

- Steps
1. Brainstorming. Came up with an initial idea of how to detect my dog, what parts I would need to do so, requirements the fountain would need, etc. I had an Arduino sitting around and had previous experience with ping sensors so I opted to use both in this project. I would use the sensors to detect him within a certain distance of the fountain. Initial requirements for the fountain were detect water levels, a visual to show when water was low, power via USB, and 1 minute water run time. 
2.  Measurements. I had to take measurements of the fountain and my dog. I collected his height, the height/width of the fountain, & the arc/distance the ping sensors could detect. These measurements were used to position the ping sensors optimally around the fountain and at the correct height where they would detect him. 
3. Blueprint. I then came up with some sketches of how my parts would be aligned, how they'd connect to the Arduino, which pin, researched whether to use a LOW/HIGH signal for my relays, how to waterproof the design, and the power requirements, ensuring they stayed well within the abilities of the Arduino Uno. 
4. Prototype. After I had my parts and a blueprint, I assembled the pieces together, wrote some software and experimented with the design to see what worked best. During this stage I experimented with sensitivity of my water sensor. I also used the multimeter to determine which wires controlled the motor, the light, and positive/negative as they weren't color coded to standard convention (Black was actually the positive wire, while red was negative and white was a control wire). 
5. Testing. After I had a working design, next was to test things out and optimize. I wrote tests to ensure everything was hooked up correctly and to check that none of the hardware itself was buggy. Make sure no there were no short circuits. Check if power supply could handle max current draw from all components on at once. After I confirmed things on the hardware side were good, next was to optimize. I played around with the water run time and timed how long my dog stood there drinking water or would come back to drink water in a certain span of time. I changed the threshold detection distance and delays between my ping sensor to improve sensitivity/detection rates, and more to see what worked best.

Here are a few images from during the development cycle and the finished product.

My Arduino, Relay and Water sensor during testing

![Arduino UNO, Relays and Water sensor during testing](https://github.com/user-attachments/assets/30633c13-0141-4d2c-a1ee-418e4acf974c)


Labeled Jumper wires so it was easy to know which pins to plug them in and which sensor they connected to (S3 is sensor #3)

![Labeled Jumper Wires](https://github.com/user-attachments/assets/bdf97da1-d6e5-4468-9f3f-66d3b4e6f326)


Fountain during prototyping

![Prototype](https://github.com/user-attachments/assets/216c511d-f096-4a99-9079-b874c8da395b)


Fountain connected to control box, used red cardboard to make it sleek and insulate Arduino & relays from water damage. The bottom of the red box has a plastic lining to prevent water from seeping in and top is easily removable but secure to allow me to check components.

![Control Box and Fountain](https://github.com/user-attachments/assets/b252851a-541a-4690-a736-8235e7351829)


Smart fountain connected to power supply

![Fountain](https://github.com/user-attachments/assets/16eef2e9-c08e-4f59-a200-3af147c3595e)


- Finished Product
  How the final product works is as follows:
  1.  We have 4 ping sensors, the Arduino cycles through them
  2.  When my dog passes a certain threshold, the ping sensors send a signal to the Arduino to activate a relay. 
  3.  The relay controls the water pump. The Arduino tells the relay how long to keep the pump on. 
  4.  While running, we check the water level constantly to ensure there is enough water to pump.
  5.  If there is not, we shut off the pump and turn on a blue light indicating there isn't enough water. 

This workflow achieved my 3 goals to transform this regular fountain into a smart fountain:

1. Improved water efficiency by only running the pump when necessary. Extends pump life and reduces water efficiency.
2. Prevent pump from running dry. We now can directly detect when water is low and prevent the pump from running in the first place.
3. Indicate clearly when water is low by turning on blue light that catches peoples attention with their most used sense, sight.

- What I Would Do Differently Next Time

There are a couple things I would do on further iterations of this project to improve the ease of use and usefulness. 

1. I would use a visual detection system, I believe it is possible using AI and ML to detect when my dog is near. Right now, any object that crosses the threshold is assumed to be my dog and activates the pump. This design will also have the advantage of no deadzones, there are spaces between the ping sensors that my dog isn't detected and I can't fill the gaps practically because of cost factors/power draw/speed of sound/space limitations/etc. A visual system could easily over come them.

2. Another way I could improve is use a radar system, using servo motors and 2 ping sensors rotating constantly. This would require more power, but balance somewhat with the reduced ping sensors and have less deadzones as I could increment the speed to improve the granularity of the radar. 

3. I would create a power cord. Right now I have to unplug and re-insert jumper wires to clean the water fountain. I would definitely make it easier to separate components.
