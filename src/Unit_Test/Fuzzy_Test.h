#pragma once
#include <Arduino.h>
#include <fuzzy.h>
#include <Unit_Test/WaterLevel.h>
#include <Unit_Test/Servo_Pump.h>

Fuzzy *fuzzyTest = new Fuzzy(); // Create a new Fuzzy object
// JSN_GPIO jsnSensor(33, 32); // JSN-SR04T sensor on pins 12 (trig) and 13 (echo)
JSN_GPIO jsnSensor(18, 19);
// Servo_Pump servoPump(14); // Servo motor on pin 14
Servo_Pump servoPump(26);

int distanceInput = 52; // Global variable for distance
int volumeUsed = 0;     // Global variable for volume

void setup()
{
    Serial.begin(115200);
    jsnSensor.begin(); // Initialize the JSN-SR04T sensor
    servoPump.begin(); // Initialize the servo motor

    // Define fuzzy input for distance
    FuzzyInput *distance = new FuzzyInput(1);
    FuzzySet *rendah = new FuzzySet(40, 45, 70, 70);
    FuzzySet *sedang = new FuzzySet(20, 35, 35, 40);
    FuzzySet *tinggi = new FuzzySet(0, 20, 20, 35);
    distance->addFuzzySet(rendah);
    distance->addFuzzySet(sedang);
    distance->addFuzzySet(tinggi);
    fuzzyTest->addFuzzyInput(distance);

    // Define fuzzy input for volume
    FuzzyInput *volume = new FuzzyInput(2);
    FuzzySet *sedikit = new FuzzySet(0, 0, 30, 50);
    FuzzySet *normal = new FuzzySet(35, 56, 56, 80);
    FuzzySet *boros = new FuzzySet(66, 75, 100, 100);
    volume->addFuzzySet(sedikit);
    volume->addFuzzySet(normal);
    volume->addFuzzySet(boros);
    fuzzyTest->addFuzzyInput(volume);

    // Define fuzzy output for servo
    FuzzyOutput *servo = new FuzzyOutput(1);
    FuzzySet *cepat = new FuzzySet(50, 70, 70, 100);
    FuzzySet *sedangServo = new FuzzySet(80, 100, 100, 120);
    FuzzySet *lambat = new FuzzySet(100, 140, 180, 180);
    servo->addFuzzySet(lambat);
    servo->addFuzzySet(sedangServo);
    servo->addFuzzySet(cepat);
    fuzzyTest->addFuzzyOutput(servo);

    // Define fuzzy rules
    FuzzyRuleAntecedent *tinggi_sedikit_1 = new FuzzyRuleAntecedent();
    tinggi_sedikit_1->joinWithAND(tinggi, sedikit);
    FuzzyRuleConsequent *Servo_Cepat_1 = new FuzzyRuleConsequent();
    Servo_Cepat_1->addOutput(cepat);
    FuzzyRule *fuzzyRule1 = new FuzzyRule(1, tinggi_sedikit_1, Servo_Cepat_1);
    fuzzyTest->addFuzzyRule(fuzzyRule1);

    //Rule 2
    FuzzyRuleAntecedent *tinggi_normal_2 = new FuzzyRuleAntecedent();
    tinggi_normal_2->joinWithAND(tinggi,normal);
    FuzzyRuleConsequent *Servo_Cepat_2 = new FuzzyRuleConsequent();
    Servo_Cepat_2->addOutput(cepat);
    FuzzyRule *fuzzyRule2 = new FuzzyRule(2, tinggi_normal_2, Servo_Cepat_2);
    fuzzyTest->addFuzzyRule(fuzzyRule2);

    //Rule 3
    FuzzyRuleAntecedent *tinggi_boros_3 = new FuzzyRuleAntecedent();
    tinggi_boros_3->joinWithAND(tinggi,boros);
    FuzzyRuleConsequent *Servo_Sedang_3 = new FuzzyRuleConsequent();
    Servo_Sedang_3->addOutput(sedangServo);
    FuzzyRule *fuzzyRule3 = new FuzzyRule(3, tinggi_boros_3, Servo_Sedang_3);
    fuzzyTest->addFuzzyRule(fuzzyRule3);

    //Rule 4
    FuzzyRuleAntecedent *sedang_sedikit_4 = new FuzzyRuleAntecedent();
    sedang_sedikit_4->joinWithAND(sedang, sedikit);
    FuzzyRuleConsequent *Servo_Sedang_4 = new FuzzyRuleConsequent();
    Servo_Sedang_4->addOutput(sedangServo);
    FuzzyRule *fuzzyRule4 = new FuzzyRule(4, sedang_sedikit_4, Servo_Sedang_4);
    fuzzyTest->addFuzzyRule(fuzzyRule4);

    //Rule 5
    FuzzyRuleAntecedent *sedang_normal_5 = new FuzzyRuleAntecedent();
    sedang_normal_5->joinWithAND(sedang,normal);
    FuzzyRuleConsequent *Servo_Sedang_5 = new FuzzyRuleConsequent();
    Servo_Sedang_5->addOutput(sedangServo);
    FuzzyRule *fuzzyRule5 = new FuzzyRule(5, sedang_normal_5, Servo_Sedang_5);
    fuzzyTest->addFuzzyRule(fuzzyRule5);

    //Rule 6
    FuzzyRuleAntecedent *sedang_boros_6 = new FuzzyRuleAntecedent();
    sedang_boros_6->joinWithAND(sedang,boros);
    FuzzyRuleConsequent *Servo_Lambat_6 = new FuzzyRuleConsequent();
    Servo_Lambat_6->addOutput(lambat);
    FuzzyRule *fuzzyRule6 = new FuzzyRule(6, sedang_boros_6, Servo_Lambat_6);
    fuzzyTest->addFuzzyRule(fuzzyRule6);

    //Rule 7
    FuzzyRuleAntecedent *rendah_sedikit_7 = new FuzzyRuleAntecedent();
    rendah_sedikit_7->joinWithAND(rendah,sedikit);
    FuzzyRuleConsequent *Servo_Lambat_7 = new FuzzyRuleConsequent();
    Servo_Lambat_7->addOutput(lambat); // Add output to the rule
    FuzzyRule *fuzzyRule7 = new FuzzyRule(7, rendah_sedikit_7, Servo_Lambat_7);
    fuzzyTest->addFuzzyRule(fuzzyRule7);

    //Rule 8
    FuzzyRuleAntecedent *rendah_normal_8 = new FuzzyRuleAntecedent();
    rendah_normal_8->joinWithAND(rendah,normal);
    FuzzyRuleConsequent *Servo_Lambat_8 = new FuzzyRuleConsequent();
    Servo_Lambat_8->addOutput(lambat); // Add output to the rule
    FuzzyRule *fuzzyRule8 = new FuzzyRule (8, rendah_normal_8, Servo_Lambat_8);
    fuzzyTest->addFuzzyRule(fuzzyRule8);

    //Rule 9
    FuzzyRuleAntecedent *rendah_boros_9 = new FuzzyRuleAntecedent();
    rendah_boros_9->joinWithAND(rendah,boros);
    FuzzyRuleConsequent *Servo_Lambat_9 = new FuzzyRuleConsequent();
    Servo_Lambat_9->addOutput(lambat); // Add output to the rule
    FuzzyRule *fuzzyRule9 = new FuzzyRule (9, rendah_boros_9, Servo_Lambat_9);
    fuzzyTest->addFuzzyRule(fuzzyRule9);

}

void loop()
{
    // Debug input values
    // Serial.printf("Input Distance: %d\n", distanceInput);
    // Serial.printf("Input Volume: %d\n", volumeUsed);

    int jsndata = jsnSensor.getDistance(); // Get distance from JSN-SR04T sensor

    // Set inputs for fuzzy logic
    fuzzyTest->setInput(1, jsndata);
    fuzzyTest->setInput(2, volumeUsed);

    // Perform fuzzification
    fuzzyTest->fuzzify();

    // Get output from fuzzy logic
    float out = fuzzyTest->defuzzify(1);

    // Print output
    Serial.printf("Distance: %d cm\n", jsndata);
    Serial.printf("Volume Used: %d L\n", volumeUsed);
    Serial.printf("Derajat Servo: %.2f\n", out);
    
    servoPump.degree(out); // Set servo angle based on fuzzy output

    delay(500);
}





