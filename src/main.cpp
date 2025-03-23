#include <WaterLevel.h>

JSN_GPIO jsn(21,19); // Trig, Echo
JSN_GPIO2 jsn2(18,5); // Trig, Echo

void setup() {
  Serial.begin(115200);
  jsn.begin();
  jsn2.begin();

}

void loop() {

  float jsnData = jsn.getDistance();
  float jsnData2 = jsn2.getDistance();

  if (jsnData >= 0) {
    Serial.printf("GPIO Sensor Distance 1: %.2f cm\n", jsnData);
  } else {
    Serial.println("GPIO Sensor: No object detected");
  }

  if (jsnData2 >= 0) {
    Serial.printf("GPIO Sensor Distance 2: %.2f cm\n", jsnData2);
  } else {
    Serial.println("GPIO Sensor: No object detected");
  }


  delay(500);
}
