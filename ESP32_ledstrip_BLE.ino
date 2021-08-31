#include <WiFi.h>
#include <NeoPixelBus.h>
#include <NeoPixelBrightnessBus.h>
#include <Husarnet.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            13

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      289

NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUMPIXELS, PIN);

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;

int j = 0;

String header;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID2 "beb5483e-36e1-4688-b7f5-ea07361b26a9"
#define CHARACTERISTIC_UUID3 "beb5483e-36e1-4688-b7f5-ea07361b26aa"
#define CHARACTERISTIC_UUID4 "beb5483e-36e1-4688-b7f5-ea07361b26aa"
int v = 10;
class EtatServeur : public BLECharacteristicCallbacks 
{
    void onWrite(BLECharacteristic* pServer) 
    {
      std::string test = pServer->getValue();
      v = *test.c_str();
      Serial.println(v);
    }
};

void initBLEServer() {


  Serial.println("Starting BLE work!");

  BLEDevice::init("HomeLight");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );

  pCharacteristic->setValue("10");
  BLECharacteristic *pCharacteristic2 = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID2,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );

  pCharacteristic2->setValue("1");
  BLECharacteristic *pCharacteristic3 = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID3,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );

  pCharacteristic3->setValue("1");
  BLECharacteristic *pCharacteristic4 = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID4,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );

  pCharacteristic4->setValue("255");
  pCharacteristic->setCallbacks(new EtatServeur());
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;

void gotTouch1(){
 touch1detected = true;
}

void setup() {
  Serial.begin(115200);
  touchAttachInterrupt(T0, gotTouch1, threshold);
  initBLEServer();
  strip.Begin();
  strip.Show();

  bool connectedSuccess = 0;

  xTaskCreate(
    taskLED,          /* Task function. */
    "taskLED",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL);            /* Task handle. */

//  xTaskCreate(
//    taskWifi,          /* Task function. */
//    "taskWifi",        /* String with name of task. */
//    10000,            /* Stack size in bytes. */
//    NULL,             /* Parameter passed as input of the task */
//    1,                /* Priority of the task. */
//    NULL);            /* Task handle. */
}

void rainbow(int j) {
  int k = 0;
  int l = 0;
  int m = 0;

  for (int i = 0; i < NUMPIXELS; i++) {
    if ( ( i >= 0 ) && ( i < NUMPIXELS * 1 / 3 ) ) {
      red = 255 * k / (NUMPIXELS / 3);
      green = 0;
      blue = 255 - (255 * k / (NUMPIXELS / 3) );
      k++;
    }
    if ( ( i >= NUMPIXELS * 1 / 3 ) && ( i < NUMPIXELS * 2 / 3 ) ) {
      red = 255 - (255 * l / (NUMPIXELS / 3) );
      green = 255 * l / (NUMPIXELS / 3);
      blue = 0;
      l++;
    }
    if ( ( i >= NUMPIXELS * 2 / 3 ) && ( i < NUMPIXELS * 3 / 3 ) ) {
      red = 0;
      green = 255 - (255 * m / (NUMPIXELS / 3) );
      blue = 255 * m / (NUMPIXELS / 3);
      m++;
    }

    strip.SetPixelColor((i + j) % NUMPIXELS, RgbColor(red, green, blue));
  }

  strip.SetBrightness(v);
  strip.Show();
  delay(50);
}

void white_shine(int j) {
  int k = 0;
  int l = 0;
  int m = 0;

  for (int i = 0; i < NUMPIXELS; i++) {
    if ( ( i >= 0 ) && ( i < NUMPIXELS * 1 / 3 ) ) {
      red = 255 * k / (NUMPIXELS / 3);
      green = 255 * k / (NUMPIXELS / 3);
      blue = 255 * k / (NUMPIXELS / 3);
      k++;
    }
    if ( ( i >= NUMPIXELS * 1 / 3 ) && ( i < NUMPIXELS * 2 / 3 ) ) {
      red = 255 - (255 * l / (NUMPIXELS / 3) );
      green = 255 - (255 * l / (NUMPIXELS / 3) );
      blue = 255 - (255 * l / (NUMPIXELS / 3) );
      l++;
    }
    if ( ( i >= NUMPIXELS * 2 / 3 ) && ( i < NUMPIXELS * 3 / 3 ) ) {
      red = 0;
      green = 0;
      blue = 0;
      m++;
    }
    Serial.printf("\r\n[R,G,B] = [%d,%d,%d]", red, green, blue);
    strip.SetPixelColor((i + j) % NUMPIXELS, RgbColor(red, green, blue));
  }
  strip.Show();
  delay(40);
}

void red_shine(int j) {
  int k = 0;
  int l = 0;
  int m = 0;
  
  for (int i = 0; i < NUMPIXELS; i++) {
    if ( ( i >= 0 ) && ( i < NUMPIXELS * 1 / 3 ) ) {
      red = 255;
      green = 255 - 255 * k / (NUMPIXELS / 3);
      blue = 255 - 255 * k / (NUMPIXELS / 3);
      k++;
    }
    if ( ( i >= NUMPIXELS * 1 / 3 ) && ( i < NUMPIXELS * 2 / 3 ) ) {
      red = 255;
      green = (255 * l / (NUMPIXELS / 3) );
      blue = (255 * l / (NUMPIXELS / 3) );
      l++;
    }
    if ( ( i >= NUMPIXELS * 2 / 3 ) && ( i < NUMPIXELS * 3 / 3 ) ) {
      red = 255;
      green = 255;
      blue = 255;
      m++;
    }
    strip.SetPixelColor((i + j) % NUMPIXELS, RgbColor(red, green, blue));
  }
  strip.Show();
  delay(40);
}

void led_white()
{
  for (int i = 0; i < NUMPIXELS; i++) {
    red = 255;
    green = 255;
    blue = 255;
    strip.SetPixelColor((i + j) % NUMPIXELS, RgbColor(red, green, blue));
  }
  delay(5);
  strip.Show();
}

void led_off() {
  for (int i = 0; i < NUMPIXELS; i++) {
    red = 0;
    green = 0;
    blue = 0;
    strip.SetPixelColor((i + j) % NUMPIXELS, RgbColor(red, green, blue));
  }
  delay(100);
  strip.Show();
}


char* getModeName(uint8_t modeNo) {
  switch (modeNo) {
    case 0:
      return "rainbow";
    case 1:
      return "white shine";
    case 2:
      return "red shine";
    case 3:
      return "only white";
    case 4:
      return "off";
  }
  return "error";
}

uint8_t modeRGB = 0;
void taskLED( void * parameter )
{
  while (1) {
    switch (modeRGB) {
      case 0:
        rainbow(j++);
        break;
      case 1:
        white_shine(j++);
        break;
      case 2:
        red_shine(j++);
        break;
      case 3:
        led_white();
        break;
      case 4:
        led_off();
        break;
      default:
        rainbow(j++);
        break;
    }
    if (j >= NUMPIXELS) {
      j = 0;
    }
  }

  Serial.println("Ending task LED");
  vTaskDelete( NULL );
}

void loop() {
  while (1) {
    if(touch1detected){
    touch1detected = false;
    Serial.println("Touch 1 detected");
    
  }
    delay(1000);
  }
}
