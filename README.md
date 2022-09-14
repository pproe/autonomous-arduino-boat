# autonomous-arduino-boat

Autonomous Boat Project for UWA Course Digital Embedded Systems ENSC3020 2019.

This is a simple control mechanism to maintain the direction of the boat by actuating two rear motors, and when reaching an obstacle the boat should turn around and travel in the direct opposite direction.

Uses Arduino Nano, magnetometer compass sensor and ultrasonic distance sensor to guide movement.

### Reflection on outcome and Future Improvements:
Using a Magnetometer compass caused many issues in directional accuracy of the boat's movement. The use of an accelerometer would have been more appropriate to use in this situation as these measurements are more precisely measured on the scale we were using.
