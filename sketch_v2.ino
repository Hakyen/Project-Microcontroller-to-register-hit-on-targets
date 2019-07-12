#include <EEPROM.h> // Библиотека работы с энергонезависимой памятью
#include <ESP8266WiFi.h> // Библиотека для работы с WiFi сетями
#include <ESP8266WebServer.h> // Библиотека для работы с HTTP сервером
#include <FS.h>

ESP8266WebServer server(80); // Создание объекта сервера

const char* ssid_AP = "TARGET_Live"; // Название точки доступа
const char* password_AP = "00000000"; // Пароль точки доступа

bool Tuning = false;

int Reset_Button_PIN = 16; // Пин кнопки перехода в режим настройки

int Segment_Count = 0; // Количество сегментов на мишени
const String Names_Avalible_PIN[10] = {"", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "RX"}; // Массив названий всех доступных пинов на плате
int Segments_PIN[9] = {5, 4, 0, 2, 14, 12, 13, 15, 3}; // Пины платы к которым подключенны датчики

int RGB_Diode_PIN[3] = {1, 10, 9}; // Пины RGB светодиода
int RGB_[9][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

void setup()
{
  EEPROM.begin(512); // Инициализируем энергонезависимую пать (512 байт)
  FS_initialization(); // Включаем работу с SPIFFS памятью
  pinMode(Reset_Button_PIN, INPUT); // Включаем пин кнопки перехода в режим настройки в режим INPUT
  if (digitalRead(Reset_Button_PIN) == HIGH)
  {
    Tuning = true; // bool переменная для определения режима работы в цикле
    WiFi.mode(WIFI_AP); // Включаем WiFi модуль в режиме точки доступа
    WiFi.softAP(ssid_AP, password_AP); // Включаем точку доступа
    Config(true); // Загружаем данные из EEPROM
    HTTP(); // Инициализируем HTTP страницы и запускаем сервер
  }
  else
  {
    Config(true); // Загружаем данные из EEPROM
    for(int i = 0; i < Segment_Count; i++) // Цикл включения пинов датчиков
    {
      pinMode(Segments_PIN[i], INPUT);
    }
    for(int i = 0; i < 3; i++) // Цикл включения пинов RGB светодиода
    {
      pinMode(RGB_Diode_PIN[i], OUTPUT);
    }
    delay(5000); // Задержка 5 секунд перед началом работы
  }

}

void loop()
{
  if (Tuning == false)
  {
    HITREG(); // Проверяем срабатывания датчиков
  }
  else
  {
    server.handleClient(); // Обрабатываем запросы
  }
}
