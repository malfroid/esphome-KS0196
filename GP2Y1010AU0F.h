#include "esphome.h"

static const char *TAG = "GP2Y1010AU0F.sensor";

// measure VCC while the sensor is running
#define VOLTAGE_VREF 5.11

#define PIN_ADC A0
#define PIN_LED D1

// measure every 12 seconds = 5 times per minute
#define UPDATE_INTERVAL 12000 // update interval in ms

class GP2Y1010AU0F : public PollingComponent, public Sensor
{
public:
    GP2Y1010AU0F() : PollingComponent(UPDATE_INTERVAL) {}

    float get_setup_priority() const override { return esphome::setup_priority::DATA; }

    void setup() override
    {
        ESP_LOGCONFIG(TAG, "Setting up sensor...");

        pinMode(PIN_LED, OUTPUT); // sensor led pin
        pinMode(PIN_ADC, INPUT);  // output form sensor
    }

    void update() override
    {
        float value   = 0;
        float voltage = 0;
        float density = 0;

        // enable led in sensor
        digitalWrite(PIN_LED, LOW);
        delay(280);

        // measure voltage
        value = analogRead(PIN_ADC);
        delay(40);

        // led off
        digitalWrite(PIN_LED, HIGH);

        // calculate voltage
        voltage = value * (VOLTAGE_VREF / 1024.0);

        // for calibration
        ESP_LOGCONFIG(TAG, "Measurements done, VRef: %f, ADC Value: %f, Calculated Voltage: %f", VOLTAGE_VREF, value, voltage);

        // taken from https://www.howmuchsnow.com/arduino/airquality/
        publish_state(170 * voltage - 0.1); // publish
    }
};