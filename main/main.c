

#include "trace.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GY69_X_CHANNEL 2
#define GY69_Y_CHANNEL 3
#define GY69_Z_CHANNEL 4

#define UNIT ADC_UNIT_1
#define DEFAULT_VREF 1100
#define NO_OF_SAMPLES 64

static esp_adc_cal_characteristics_t *adc_chars_x;
static esp_adc_cal_characteristics_t *adc_chars_y;
static esp_adc_cal_characteristics_t *adc_chars_z;


static void accelerometer_x_task(void* args)
{
    adc_chars_x = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type_x = esp_adc_cal_characterize(UNIT, ADC_ATTEN_DB_11, 3, DEFAULT_VREF, adc_chars_x);
    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            adc_reading += adc1_get_raw(GY69_X_CHANNEL);
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars_x);
        TRACE_B("<===========================================>");
        TRACE_D("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        // float d = (((adc_reading/4096.0)*6000.0) - 3000.0)/1000;
        float d = ((adc_reading/4096.0)*6.0) - 3.0;
        TRACE_D("X-axis reading is %.3f m/s^2", d*9.8);
        TRACE_B("<++++++++++++++++++++++++++++++++++++++++++++>\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void accelerometer_y_task(void* args)
{
    adc_chars_y = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type_y = esp_adc_cal_characterize(UNIT, ADC_ATTEN_DB_11, 3, DEFAULT_VREF, adc_chars_y);
    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            adc_reading += adc1_get_raw(GY69_Y_CHANNEL);
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars_y);
        TRACE_B("<===========================================>");
        // TRACE_D("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        // float d = (((adc_reading/4096.0)*6000.0) - 3000.0)/1000;
        float d = ((adc_reading/4096.0)*6.0) - 3.0;
        TRACE_D("Y-axis reading is %.3f m/s^2", d*9.8);
        TRACE_B("<++++++++++++++++++++++++++++++++++++++++++++>\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void accelerometer_z_task(void* args)
{
    adc_chars_z = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type_z = esp_adc_cal_characterize(UNIT, ADC_ATTEN_DB_11, 3, DEFAULT_VREF, adc_chars_z);
    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            adc_reading += adc1_get_raw(GY69_Z_CHANNEL);
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars_z);
        TRACE_B("<===========================================>");
        TRACE_D("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        // float d = (((adc_reading/4096.0)*6000.0) - 3000.0)/1000;
        float d = ((adc_reading/4096.0)*6.0) - 3.0;
        TRACE_D("Z-axis reading is %.3f m/s^2", d*9.8);
        TRACE_B("<++++++++++++++++++++++++++++++++++++++++++++>\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main()
{
    adc1_config_width(3);
    adc1_config_channel_atten(GY69_X_CHANNEL, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(GY69_Y_CHANNEL, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(GY69_Z_CHANNEL, ADC_ATTEN_DB_11);

    xTaskCreate(accelerometer_x_task, "accelerometer_x_task", 2*2048, NULL, 0, NULL);
    xTaskCreate(accelerometer_y_task, "accelerometer_y_task", 2*2048, NULL, 0, NULL);
    xTaskCreate(accelerometer_z_task, "accelerometer_z_task", 2*2048, NULL, 0, NULL);
}

