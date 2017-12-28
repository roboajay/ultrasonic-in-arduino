#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle  nh;

long duration, distance; // Duration used to calculate distance

sensor_msgs::Range range_msg;
ros::Publisher pub_range_1( "/ultrasound_1", &range_msg);
ros::Publisher pub_range_2( "/ultrasound_2", &range_msg);

char frameid[] = "/ultrasound";

void setup() 

{
  
  
  nh.initNode();
  
  nh.advertise(pub_range_1);
  nh.advertise(pub_range_2);
  
  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.1;  // fake
  range_msg.min_range = 0.0;
  range_msg.max_range = 200;

 
 pinMode(8, OUTPUT);
 pinMode(7, INPUT);
 
 pinMode(9, OUTPUT);
 pinMode(6, INPUT);
}

float getRange_Ultrasound(int echoPin, int trigPin)
{
 int val = 0;
 for(int i=0; i<4; i++) 
 { 
  
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 val += duration;
 
 }
 return val / 232.8 ;
}


long range_time;

void loop() 
{  
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
  if ( millis() >= range_time )
  {
    ROS_INFO("void setup started");
    range_msg.range = getRange_Ultrasound(7,8);
    range_msg.header.stamp = nh.now();
    pub_range_1.publish(&range_msg);

    range_msg.range = getRange_Ultrasound(6,9);
    range_msg.header.stamp = nh.now();
    pub_range_2.publish(&range_msg);
    
    range_time =  millis() + 50;
  }
  nh.spinOnce();
  delay(50);
}
