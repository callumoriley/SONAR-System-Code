import processing.serial.*;
Serial port;
FloatList currFrame;
FloatList lastFrame;
int lastTime = 0;
int currTime;
float rps;

void setup()
{
  currFrame = new FloatList();
  lastFrame = new FloatList();
  
  size(800,800);
  port = new Serial(this, "COM4", 9600);
}
void draw()
{
  background(0);
  if (port.available() > 0)
  {
    String serialData = port.readStringUntil('\n');
    println(serialData);
    if (serialData.indexOf("s") != -1) // send s from the Arduino when the light sensor passes
    {
      lastFrame.clear(); // copies the values from currFrame to lastFrame
      for (int i = 0; i < currFrame.size(); i++)
      {
        lastFrame.append(currFrame.get(i));
      }
      currFrame.clear(); // clears currFrame
      
      currTime = millis(); // calculate and display RPS
      rps = 1000.0/(currTime - lastTime);
      lastTime = currTime;
    }
    else
    {
      currFrame.append(float(serialData.substring(0, serialData.length() - 1))); // adds the number received through serial to the list of the current frame. Also cuts off the last character of the serial data
    }
  }
  stroke(255);
  float angle;
  float currDist;
  
  ellipseMode(CENTER);
  noFill();
  for (int i = 0; i < lastFrame.size(); i++) // draws the arcs counterclockwise, as the motor spins counterclockwise
  {
    angle = -TWO_PI/lastFrame.size()*i+PI;
    currDist = lastFrame.get(i);
    arc(width/2, height/2, currDist*2, currDist*2, angle - TWO_PI/(2*lastFrame.size()), angle + TWO_PI/(2*lastFrame.size()), OPEN);
  }
  
  textSize(12); // display RPS and number of samples
  fill(255);
  text("RPS: " + str(rps) + ", measurements: "+str(lastFrame.size()), 20, 20);
}
void keyPressed() // takes a screenshot
{
  save(str(month())+"-"+str(day())+"-"+str(year())+"-"+str(hour())+"-"+str(minute())+"-"+str(second())+".png");
}
