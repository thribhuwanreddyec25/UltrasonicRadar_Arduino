import processing.serial.*;  // Serial library for communicating with Arduino

Serial myPort;               // Serial port object
String distance = "";        // Distance value received from Arduino
String data = "";            // Raw incoming serial data
String noObject;             // Status text ("In Range" / "Out of Range")
String angle = "";           // Angle value received from Arduino

float pixsDistance;          // Distance converted into pixels
int iAngle, iDistance;       // Numerical angle and distance values

void setup() {
  size(1280, 720);           // Window size
  smooth();                  // Enable smoother graphics

  myPort = new Serial(this, "COM3", 9600); // Open serial port at 9600 baud
  myPort.bufferUntil('.');                // Read incoming data until a '.' appears
}

void draw() {

  // Create the motion blur effect behind the radar sweep
  noStroke();
  fill(0, 4);
  rect(0, 0, width, height - height * 0.065);

  fill(98,245,31);           // Radar green color

  drawRadar();               // Draw radar grid lines and arcs
  drawLine();                // Draw the sweeping radar line
  drawObject();              // Draw detected object indicator
  drawText();                // Draw labels, angle, distance, etc.
}


// Triggered whenever Processing receives a complete message ending with '.'
void serialEvent(Serial myPort) {

  data = myPort.readStringUntil('.');         // Example incoming: "45,23."
  data = data.substring(0, data.length() - 1); // Remove the ending '.'

  int index = data.indexOf(',');  // Locate the comma separator

  angle = data.substring(0, index);        // Extract angle
  distance = data.substring(index + 1);    // Extract distance

  iAngle = int(angle);                     // Convert to integer
  iDistance = int(distance);
}


// ------------------------------------------------------
// Draw the radar's semicircles and angle lines
// ------------------------------------------------------
void drawRadar() {
  pushMatrix();
  translate(width/2, height - height*0.074);

  noFill();
  stroke(98,245,31);
  strokeWeight(2);

  // Radar distance rings
  arc(0,0,width*0.94,width*0.94, PI, TWO_PI);
  arc(0,0,width*0.73,width*0.73, PI, TWO_PI);
  arc(0,0,width*0.52,width*0.52, PI, TWO_PI);
  arc(0,0,width*0.31,width*0.31, PI, TWO_PI);

  // Angle markers every 30 degrees
  for (int ang = 0; ang <= 150; ang += 30) {
    line(0, 0,
         -width/2 * cos(radians(ang)),
         -width/2 * sin(radians(ang)));
  }

  popMatrix();
}


// ------------------------------------------------------
// Draw the detected object as a red segment
// ------------------------------------------------------
void drawObject() {
  pushMatrix();
  translate(width/2, height - height*0.074);

  stroke(255,10,10);       // Red color for object
  strokeWeight(9);

  pixsDistance = iDistance * ((height - height*0.1666) * 0.025);

  // Draw only if the object is within 40 cm
  if (iDistance < 40) {
    line(pixsDistance * cos(radians(iAngle)),
         -pixsDistance * sin(radians(iAngle)),
         (width*0.49) * cos(radians(iAngle)),
         -(width*0.49) * sin(radians(iAngle)));
  }

  popMatrix();
}


// ------------------------------------------------------
// Draw the sweeping radar line
// ------------------------------------------------------
void drawLine() {
  pushMatrix();
  translate(width/2, height - height*0.074);

  stroke(30,250,60);       // Bright green sweep line
  strokeWeight(9);

  line(0, 0,
       (height * 0.88) * cos(radians(iAngle)),
       -(height * 0.88) * sin(radians(iAngle)));

  popMatrix();
}


// ------------------------------------------------------
// Draw bottom UI: angle, distance, labels, status
// ------------------------------------------------------
void drawText() {
  pushMatrix();

  // Range status
  if (iDistance > 40) {
    noObject = "Out of Range";
  } else {
    noObject = "In Range";
  }

  // Background at bottom
  fill(0);
  noStroke();
  rect(0, height - height*0.0648, width, height);

  fill(98,245,31);
  textSize(25);

  // Distance ring labels
  text("10cm", width - width*0.3854, height - height*0.0833);
  text("20cm", width - width*0.281,  height - height*0.0833);
  text("30cm", width - width*0.177,  height - height*0.0833);
  text("40cm", width - width*0.0729, height - height*0.0833);

  // Main bottom text
  textSize(40);
  text("Harsh Sharma", width - width*0.875, height - height*0.0277);
  text("Angle: " + iAngle + "°", width - width*0.48, height - height*0.0277);

  // Show distance only if within range
  if (iDistance < 40) {
    text("Distance: " + iDistance + " cm", width - width*0.225, height - height*0.0277);
  } else {
    text("Out of Range", width - width*0.225, height - height*0.0277);
  }

  popMatrix();
}
