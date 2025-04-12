#pragma once
#include <Arduino.h>
#include <fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

int distanceInput = 0; // Global variable for distance
int volumeUsed = 0;     // Global variable for volume

void setup()
{
    Serial.begin(115200);

    // Define fuzzy input for distance
    FuzzyInput *distance = new FuzzyInput(1);
    FuzzySet *rendah = new FuzzySet(30, 45, 50, 50);
    FuzzySet *sedang = new FuzzySet(10, 25, 25, 40);
    FuzzySet *tinggi = new FuzzySet(-5, 10, 10, 20);
    distance->addFuzzySet(rendah);
    distance->addFuzzySet(sedang);
    distance->addFuzzySet(tinggi);
    fuzzy->addFuzzyInput(distance);

    // Define fuzzy input for volume
    FuzzyInput *volume = new FuzzyInput(2);
    FuzzySet *sedikit = new FuzzySet(-5, -5, 30, 50);
    FuzzySet *normal = new FuzzySet(35, 56, 56, 80);
    FuzzySet *boros = new FuzzySet(66, 75, 100, 100);
    volume->addFuzzySet(sedikit);
    volume->addFuzzySet(normal);
    volume->addFuzzySet(boros);
    fuzzy->addFuzzyInput(volume);

    // Define fuzzy output for servo
    FuzzyOutput *servo = new FuzzyOutput(1);
    FuzzySet *lambat = new FuzzySet(0, 0, 20, 45);
    FuzzySet *sedangServo = new FuzzySet(20, 50, 50, 75);
    FuzzySet *cepat = new FuzzySet(55, 80, 105, 105);
    servo->addFuzzySet(lambat);
    servo->addFuzzySet(sedangServo);
    servo->addFuzzySet(cepat);
    fuzzy->addFuzzyOutput(servo);

    // Define fuzzy rules
    FuzzyRuleAntecedent *tinggi_sedikit_1 = new FuzzyRuleAntecedent();
    tinggi_sedikit_1->joinWithAND(tinggi, sedikit);
    FuzzyRuleConsequent *Servo_Cepat_1 = new FuzzyRuleConsequent();
    Servo_Cepat_1->addOutput(cepat);
    FuzzyRule *fuzzyRule1 = new FuzzyRule(1, tinggi_sedikit_1, Servo_Cepat_1);
    fuzzy->addFuzzyRule(fuzzyRule1);

    //Rule 2
    FuzzyRuleAntecedent *tinggi_normal_2 = new FuzzyRuleAntecedent();
    tinggi_normal_2->joinWithAND(tinggi,normal);
    FuzzyRuleConsequent *Servo_Cepat_2 = new FuzzyRuleConsequent();
    Servo_Cepat_2->addOutput(cepat);
    FuzzyRule *fuzzyRule2 = new FuzzyRule(2, tinggi_normal_2, Servo_Cepat_2);
    fuzzy->addFuzzyRule(fuzzyRule2);

    //Rule 3
    FuzzyRuleAntecedent *tinggi_boros_3 = new FuzzyRuleAntecedent();
    tinggi_boros_3->joinWithAND(tinggi,boros);
    FuzzyRuleConsequent *Servo_Sedang_3 = new FuzzyRuleConsequent();
    Servo_Sedang_3->addOutput(sedangServo);
    FuzzyRule *fuzzyRule3 = new FuzzyRule(3, tinggi_boros_3, Servo_Sedang_3);
    fuzzy->addFuzzyRule(fuzzyRule3);

    //Rule 4
    FuzzyRuleAntecedent *sedang_sedikit_4 = new FuzzyRuleAntecedent();
    sedang_sedikit_4->joinWithAND(sedang, sedikit);
    FuzzyRuleConsequent *Servo_Sedang_4 = new FuzzyRuleConsequent();
    Servo_Sedang_4->addOutput(sedangServo);
    FuzzyRule *fuzzyRule4 = new FuzzyRule(4, sedang_sedikit_4, Servo_Sedang_4);
    fuzzy->addFuzzyRule(fuzzyRule4);

    //Rule 5
    FuzzyRuleAntecedent *sedang_normal_5 = new FuzzyRuleAntecedent();
    sedang_normal_5->joinWithAND(sedang,normal);
    FuzzyRuleConsequent *Servo_Sedang_5 = new FuzzyRuleConsequent();
    Servo_Sedang_5->addOutput(sedangServo);
    FuzzyRule *fuzzyRule5 = new FuzzyRule(5, sedang_normal_5, Servo_Sedang_5);
    fuzzy->addFuzzyRule(fuzzyRule5);

    //Rule 6
    FuzzyRuleAntecedent *sedang_boros_6 = new FuzzyRuleAntecedent();
    sedang_boros_6->joinWithAND(sedang,boros);
    FuzzyRuleConsequent *Servo_Lambat_6 = new FuzzyRuleConsequent();
    Servo_Lambat_6->addOutput(lambat);
    FuzzyRule *fuzzyRule6 = new FuzzyRule(6, sedang_boros_6, Servo_Lambat_6);
    fuzzy->addFuzzyRule(fuzzyRule6);

    //Rule 7
    FuzzyRuleAntecedent *rendah_sedikit_7 = new FuzzyRuleAntecedent();
    rendah_sedikit_7->joinWithAND(rendah,sedikit);
    FuzzyRuleConsequent *Servo_Lambat_7 = new FuzzyRuleConsequent();
    Servo_Lambat_7->addOutput(lambat); // Add output to the rule
    FuzzyRule *fuzzyRule7 = new FuzzyRule(7, rendah_sedikit_7, Servo_Lambat_7);
    fuzzy->addFuzzyRule(fuzzyRule7);

    //Rule 8
    FuzzyRuleAntecedent *rendah_sedang_8 = new FuzzyRuleAntecedent();
    rendah_sedang_8->joinWithAND(rendah,sedang);
    FuzzyRuleConsequent *Servo_Lambat_8 = new FuzzyRuleConsequent();
    Servo_Lambat_8->addOutput(lambat); // Add output to the rule
    FuzzyRule *fuzzyRule8 = new FuzzyRule (8, rendah_sedang_8, Servo_Lambat_8);
    fuzzy->addFuzzyRule(fuzzyRule8);

    //Rule 9
    FuzzyRuleAntecedent *rendah_boros_9 = new FuzzyRuleAntecedent();
    rendah_boros_9->joinWithAND(rendah,boros);
    FuzzyRuleConsequent *Servo_Lambat_9 = new FuzzyRuleConsequent();
    Servo_Lambat_9->addOutput(lambat); // Add output to the rule
    FuzzyRule *fuzzyRule9 = new FuzzyRule (9, rendah_boros_9, Servo_Lambat_9);
    fuzzy->addFuzzyRule(fuzzyRule9);

}

void loop()
{
    // Debug input values
    Serial.printf("Input Distance: %d\n", distanceInput);
    Serial.printf("Input Volume: %d\n", volumeUsed);

    // Set inputs for fuzzy logic
    fuzzy->setInput(1, distanceInput);
    fuzzy->setInput(2, volumeUsed);

    // Perform fuzzification
    fuzzy->fuzzify();

    // Get output from fuzzy logic
    float out = fuzzy->defuzzify(1);

    // Print output
    Serial.printf("Derajat Servo: %.2f\n", out);

    delay(500);
}





