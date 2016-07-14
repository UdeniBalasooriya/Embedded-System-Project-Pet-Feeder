# Arduino-Wifiserver pet feeder

Pet feeder is an embedded project which automate food supplying for pets.

Also the owner can,
1)Control the device via internet
2)Release food to the pet at any time
3)Choose the amount of food to be given
4)Get feedback if the pet has eaten
5)Check if food is available in the reserve

system consists
1) Arduino wifi server
2) Load cell
3) Weight indicator
4) stepper motor

Arduino wifi server is the middle point between users and the pet feeder. 
Via internet, it can be accessed. 

1) IP address
2) port :8080

Arduino wifi server has been witten in arduino 1.0.3 .
It won't work with higher version.

Load cell is used to measure weight of food and check whether food is eat or not.
Weight indicator is used to check whether food is remain or not in the reserve/large container.
Stepper motor is used to choose the amount of food to be given.


