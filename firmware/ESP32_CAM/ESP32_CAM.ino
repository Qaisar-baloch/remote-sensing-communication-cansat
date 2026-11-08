// Includes
#include "FS.h"
#include "SD_MMC.h"
#include "esp_camera.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

TaskHandle_t listen_rx_handle;
TaskHandle_t take_image_handle;
TaskHandle_t transmit_image_handle;
TaskHandle_t log_handle;
TaskHandle_t test_acquire_image_handle;
TaskHandle_t warmup_handle;

byte rec = 0x00;
String rec_string ;
String pic = "/Image_0.jpg";
bool file_index = 0;                                                                              // Index for file; determines append or write
char pic_Index = '0';                                                                             // Index for picture
camera_fb_t *fb;

AsyncWebServer server(80);

void listen_rx(void *pvParameters) {                                                              // RTOSFunction for receive
  for (;;) {
    int current;
    int after;
    if (Serial.available()) {
      rec = Serial.read();
      switch (rec) {
        case 'A':
          current = millis();
          xTaskNotifyGive(take_image_handle);
          after = millis();
          Serial.println(after - current);
          break;
        case 'B':
            xTaskCreatePinnedToCore(
              log_data, "log_" , 4096 ,
              NULL ,
              2 ,
              &log_handle,
              tskNO_AFFINITY );
          xTaskNotifyGive(log_handle);
          break;
        case 'Z':
          Serial.println("Transmit_called");
          xTaskCreatePinnedToCore(test_image, "testing_image",4096,
          NULL,
          2,
          &test_acquire_image_handle,
          1
          );
          break;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void warmup(void *pvParameters){
int a = 0;
  while(a != 100){
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    a++;
  }
  Serial.println("warmup_done");
  vTaskDelete(NULL);
}

void test_image(void *pvParameters){
  fb = esp_camera_fb_get();
  writeImage(SD_MMC ,"/TESTIMG.jpg", fb->buf,fb->len);
  esp_camera_fb_return(fb);
  vTaskDelete(NULL);

}

void take_image(void *pvParameters) {                                                             // RTOSFunction for taking Images
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  for (int a = 0; a < 5; a++) {
    for (int i = 0; i < 2; i++) {
      if (i < 2 - 1) {
        fb = esp_camera_fb_get();
        esp_camera_fb_return(fb);
      } else {
        fb = esp_camera_fb_get();
      }
    }
    if (!fb) {
      Serial.println("Error: Frame buffer could not be acquired");
    }
    switch (a) {
      case 0:
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 1:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 2:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 3:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 4:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 5:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 6:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 7:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 8:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      case 9:
        pic.replace(pic_Index - 1, pic_Index);
        writeImage(SD_MMC, pic.c_str(), fb->buf, fb->len);
        break;
      default:
        pic = "/Image_0.jpg";
        break;
    }
    //return the frame buffer back to be reused
    esp_camera_fb_return(fb);
    pic_Index++;
  }
  pic = "/Image_0.jpg";
  pic_Index = '0';
  vTaskDelete(NULL);
}

//  void transmit_image(void *pvParameters) {                                                        // RTOSFunction for transmitting Image
//   ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//   for (;;){
//     vTaskDelay(pdMS_TO_TICKS(500));
//   }
//  }

void log_data(void *pvParameters) {                                                                                 // Function for data logging
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  for(;;){
  rec_string = Serial.readStringUntil('E');
  rec_string += '\n'; 
    appendFile(SD_MMC, "/log.txt", rec_string.c_str());
  break;
  }
  vTaskDelete(NULL);
}

void writeImage(fs::FS &fs, const char *path, const uint8_t *data, size_t len) {
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    return;
  }
  file.write(data, len);
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {                              // Function for appending to a file on SD_MMC
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {

  } else {
    Serial.println("Append failed");
  }
  Serial.println("a done");
  file.close();
}
///////////////////////////////////////////////////////////////////////////////SETUP////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(250000);

  camera_config_t config;  // Configure Camera
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 10;
  config.fb_count = 4;
  

  esp_err_t err = esp_camera_init(&config);
  while (err != ESP_OK) {
    Serial.println(esp_camera_init(&config));
    ESP.restart();
  }
  while (!SD_MMC.begin()) {
    Serial.println("SD init failed");
  }
  // Task creation should be at the end so that everything is initialized

  xTaskCreatePinnedToCore(                                                                        // Create a task for listening to Serial on core 0
    listen_rx, "ListenRXfunc", 4096,                                                              // Function, Name, Stackdepth
    NULL,                                                                                         // Function parameters
    3,                                                                                            // Priority
    &listen_rx_handle,                                                                            // Handle
    0                                                                                             // Core
  );

  xTaskCreatePinnedToCore(                                                                        // Create a task for taking Image on core 1
    take_image, "Take_Imagefunc", 4096,
    NULL,
    2,
    &take_image_handle,
    1);
    xTaskCreatePinnedToCore(                                                                        // Create a task for taking Image on core 1
    warmup, "Warmupfunc", 1024,
    NULL,
    3,
    &warmup_handle,
    1);

  // xTaskCreatePinnedToCore(                                                                        // Create a task for transmitting images on core 1
  //   transmit_image, "transmitimgfunc", 2048,
  //   NULL,
  //   2,
  //   &transmit_image_handle,
  //   1);
  server.on("/image", HTTP_GET, [](AsyncWebServerRequest *request){
    File file = SD_MMC.open("/TESTIMG.jpg", "r");
    if(!file){
        request->send(404, "text/plain", "File not found");
        return;
    }

    // Create a custom chunked response
    AsyncWebServerResponse *response = request->beginChunkedResponse("image/jpeg", [file](uint8_t *buffer, size_t maxLen, size_t index) mutable -> size_t {
        size_t len = file.read(buffer, maxLen);
        if (len == 0) {
            file.close();
        }
        return len;
    });

    request->send(response);
});
  server.begin();
}
  

void loop() {
  // put your main code here, to run repeatedly:
}
