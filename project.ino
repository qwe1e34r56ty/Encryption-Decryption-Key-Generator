#include<Wire.h>
#include<MsTimer2.h>
#include"pitches.h"
#include"LiquidCrystal_I2C.h"
#include"DHT.h"

// 본인이 짠 라이브러리
#include"Primary.h"
#include"RSA.h"

enum {
    CREATE_BUTTON = 2,
    RESET_BUTTON = 3,
    HUMID_AND_TEMP = 6,
    READY_LED = 7,
    CREATED_LED = 8,
    SPEAKER = 9,
    PD = A0
};

enum {
    START = 0,
    READY = 1,
    CREATED = 2,
    STATE_COUNT = 3
};

DHT dht(HUMID_AND_TEMP, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

volatile static long  h;
volatile static long  t;
volatile static long  pd;

volatile static long  n;
volatile static long  d;
volatile static long  e;
volatile static bool  updateNeed = false;
volatile static long  state = START;

static int lastState = START;

void initialize(int createButton, int resetButton, int humidAndTemp, int pd, int readyLed, int createdLed, int speaker,
    HardwareSerial& serial, LiquidCrystal_I2C& lcd_, DHT& dht);
void update();
void lcdUpdate();
void serialUpdate();
void stateUpdate();
void ledUpdate();
void createButtonPushed();
void resetButtonPushed();

void setup() {
    // put your setup code here, to run once:
    initialize(CREATE_BUTTON, RESET_BUTTON, HUMID_AND_TEMP, PD, READY_LED, CREATED_LED, SPEAKER, Serial, lcd, dht);
}

void loop() {
    // put your main code here, to run repeatedly:
    h = dht.readHumidity();
    t = dht.readTemperature();
    pd = analogRead(PD);
    if (updateNeed == true) {
        update();
        updateNeed = false;
    }
}

//==================================================================================================================================================================

void initialize(int createButton, int resetButton, int humidAndTemp, int pd, int readyLed, int createdLed, int speaker,
    HardwareSerial& serial, LiquidCrystal_I2C& lcd, DHT& dht) {
    pinMode(createButton, INPUT_PULLUP);
    pinMode(resetButton, INPUT_PULLUP);
    pinMode(readyLed, OUTPUT);
    pinMode(createdLed, OUTPUT);
    pinMode(speaker, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(createButton), createButtonPushed, FALLING);
    attachInterrupt(digitalPinToInterrupt(resetButton), resetButtonPushed, FALLING);

    MsTimer2::set(500, ledUpdate);
    MsTimer2::start();

    updateNeed = true;
}

void update() {
    stateUpdate();
    lcdUpdate();
    serialUpdate();
}

void lcdUpdate() {
    static void (*lcdUpdate_[STATE_COUNT][STATE_COUNT])(void);
    static boolean initialized = false;
    if (initialized == false) {
        for (int i = 0; i < STATE_COUNT; i++) {
            for (int j = 0; j < STATE_COUNT; j++) {
                lcdUpdate_[i][j] = []() {

                };
            }
        }
        lcdUpdate_[START][READY] = []() {
            lcd.begin();
            lcd.noBacklight();
            lcd.backlight();
            lcd.print("Ready");
        };
        lcdUpdate_[READY][CREATED] = []() {
            lcd.clear();
            lcd.home();
            lcd.print("n=");      lcd.print(n);
            lcd.setCursor(0, 1);
            lcd.print("d=");    lcd.print(d);
            lcd.setCursor(8, 1);
            lcd.print("e=");    lcd.print(e);
        };
        lcdUpdate_[CREATED][READY] = []() {
            lcd.clear();
            lcd.home();
            lcd.print("Ready");
        };
        initialized = true;
    }

    lcdUpdate_[lastState][state]();
}

void serialUpdate() {
    static void (*serialUpdate_[STATE_COUNT][STATE_COUNT])(void);
    static boolean initialized = false;
    if (initialized == false) {
        for (int i = 0; i < STATE_COUNT; i++) {
            for (int j = 0; j < STATE_COUNT; j++) {
                serialUpdate_[i][j] = []() {

                };
            }
        }
        serialUpdate_[START][READY] = []() {
            Serial.begin(9600);
            Serial.println("Ready");
        };
        serialUpdate_[READY][CREATED] = []() {
            Serial.print("n="); Serial.print(n);
            Serial.print(", e="); Serial.println(e); Serial.println();
        };
        serialUpdate_[CREATED][READY] = []() {
            Serial.println("Ready");
        };
        initialized = true;
    }

    serialUpdate_[lastState][state]();
}

void stateUpdate() {
    static void (*stateUpdate_[STATE_COUNT][STATE_COUNT])(void);
    static boolean initialized = false;
    if (initialized == false) {
        for (int i = 0; i < STATE_COUNT; i++) {
            for (int j = 0; j < STATE_COUNT; j++) {
                stateUpdate_[i][j] = []() {

                };
            }
        }
        stateUpdate_[START][START] = []() {
            lastState = START;
            state = READY;
        };
        stateUpdate_[START][READY] = []() {
            lastState = READY;
            state = CREATED;
        };
        stateUpdate_[READY][CREATED] = []() {
            lastState = CREATED;
            state = READY;
        };
        stateUpdate_[CREATED][READY] = []() {
            lastState = READY;
            state = CREATED;
        };
        initialized = true;
    }

    stateUpdate_[lastState][state]();
}

void ledUpdate() {
    static boolean output = HIGH;
    static void (*ledUpdate_[STATE_COUNT])(void);
    static boolean initialized = false;
    if (initialized == false) {
        for (int i = 0; i < STATE_COUNT; i++) {
            ledUpdate_[i] = []() {

            };
        }
        ledUpdate_[READY] = []() {
            digitalWrite(CREATED_LED, LOW);
            digitalWrite(READY_LED, output);
            output = !output;
        };
        ledUpdate_[CREATED] = []() {
            digitalWrite(READY_LED, LOW);
            digitalWrite(CREATED_LED, output);
            output = !output;
        };
        initialized == true;
    }

    ledUpdate_[state]();
}

void createButtonPushed() {
    static void (*createButtonPushed_[STATE_COUNT])(void);
    static boolean initialized = false;
    if (initialized == false) {
        for (int i = 0; i < STATE_COUNT; i++) {
            createButtonPushed_[i] = []() {

            };
        }
        createButtonPushed_[READY] = []() {
            Serial.println("go");
            srand(h * (int)pow(2, 20) + t * (int)pow(2, 10) + pd);
            long  p, q, tmpn, tmpd, tmpe;
            p = Primary::upperPrimaryNumber(rand() % (int)pow(2, 8));
            q = Primary::upperPrimaryNumber(rand() % (int)pow(2, 8));
            RSA::rsa(p, q, tmpn, tmpd, tmpe);
            n = tmpn;
            d = tmpd;
            e = tmpe;
            updateNeed = true;
        };
        createButtonPushed_[CREATED] = []() {
            Serial.println("reset required");
            tone(9, NOTE_G3, 1000 / 8);
        };
        initialized = true;
    }

    createButtonPushed_[state]();
}

void resetButtonPushed() {
    static void (*resetButtonPushed_[STATE_COUNT])(void);
    static boolean initialized = false;
    if (initialized == false) {
        for (int i = 0; i < STATE_COUNT; i++) {
            resetButtonPushed_[i] = []() {

            };
        }
        resetButtonPushed_[READY] = []() {

        };
        resetButtonPushed_[CREATED] = []() {
            Serial.println("Reset");
            updateNeed = true;
        };
        initialized = true;
    }

    resetButtonPushed_[state]();
}
