/*
  Скетч для создания, удаления и исполнения умных таймеров в реальном времени.
  Таймеры хранятся в json array. После выполнения таймер удаляется из массива.
  Проверка времени срабатывания таймера происходит вместе с обновлением
  реального времени раз в минуту.
  
  Github:   Portia-Lin
  https://github.com/Portia-Lin/arduino-iot-smart-timers
*/

#include <ArduinoJson.h>
boolean timer_flag = true; // Разрешение работы таймеров
int timers_min_index = 0; // Индекс ближайшего таймера из timers array
int timers_counter; // Количество таймеров, изменяется при удалении таймера
unsigned long timers_min_timestamp; // Timestamp ближайшего таймера
boolean timers_min_action; // Записываемое значение на порт ближайшего таймера
unsigned long timestamp_now = 1603764000; // Установка времени 10/27/2020 @ 2:00am (UTC)
unsigned long timing = millis(); // Текущее время millis()
int timing_update = 60 * 1000; // Время обновления реального времени в секундах (по умолчанию 1 минута, если нет, то изменить добавляемое к timestamp_now значение)

// Исходные данные для создания json array
const size_t capacity = JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(1) + 5*JSON_OBJECT_SIZE(4) + 40;
DynamicJsonDocument doc(capacity);
const char* json = "{\"timers\":[{\"id\":0,\"timestamp\":1603764060,\"regular\":false,\"action\":1},{\"id\":1,\"timestamp\":1603764120,\"regular\":true,\"action\":0},{\"id\":2,\"timestamp\":1603764180,\"regular\":false,\"action\":1},{\"id\":3,\"timestamp\":1603764240,\"regular\":false,\"action\":0},{\"id\":4,\"timestamp\":1603764300,\"regular\":false,\"action\":1}]}";
JsonArray timers;

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("Timestamp сейчас: ");
  Serial.println(timestamp_now);
  
  deserializeJson(doc, json); // Десериализация json
  timers = doc["timers"]; // Создание json массива из элементов внутри "timers"
  Serial.println("\nИсходный список таймеров json array: "); // Вывод исходного списка таймеров json array
  serializeJson(doc, Serial);

  if (timer_flag == true) new_timer(timers); // Загрузить ближайший таймер
  else Serial.println("\nТаймеры не разрешены");
}

void loop() {
  // Реализация хода времени, обновление раз в 60 секунд (1 минута)
  if (millis() - timing > timing_update){
    timing = millis();
    timestamp_now = timestamp_now + 60;
    Serial.print("Новый timestamp сейчас: ");
    Serial.println(timestamp_now);
    // Реализация выполнения таймера
    if (timer_flag == true && timestamp_now >= timers_min_timestamp) {
      write_action(); // Выполнить действие записанное в загруженный таймер
    }
  }
}

void new_timer(JsonArray timers) {
  timers_counter = timers.size(); // Количество таймеров
  if (timers_counter >= 1) {
    unsigned long timer_list[timers_counter]; // Объявление массива для хранения timestamp всех таймеров
    unsigned long min_timestamp = timer_list[timers_min_index]; // минимальный timestamp таймера равен timestamp первого таймера
    Serial.println("\nНовый список таймеров json array: ");
    serializeJson(doc, Serial); 
    for (int i = 0; i < timers_counter; i++) { // Создания массива из timestamp всех таймеров
      unsigned long timer_value = timers[i]["timestamp"];
      timer_list[i] = timer_value;
    }
    for (int i = 1; i < timers_counter; i++){ // поиск наименьшего значения timestamp из таймеров
      if (timer_list[i] < min_timestamp) {
        timers_min_index = i;
        min_timestamp = timer_list[i];
      }
    }
    timers_min_timestamp = timer_list[timers_min_index]; // Запись timestamp ближайшего таймера в переменную
    timers_min_action = timers[timers_min_index]["action"]; // Запись action ближайшего таймера в переменную
    Serial.println("\nЗагружен новый таймер."); 
    Serial.print("Минимальный timestamp: ");
    Serial.println(timers_min_timestamp); 
    Serial.print("Выполнить действие: ");
    Serial.println(timers_min_action);
  }
  else {
    timer_flag = false; // Если количество таймеров 0, запретить выполнения таймеров
    Serial.println("ТАЙМЕРОВ БОЛЬШЕ НЕТ!");
  }   
}

// Функция выполения действия, когда сработал таймер
void write_action() {
  digitalWrite(LED_BUILTIN, !timers_min_action); // Записать инвертированное значение из таймера
  Serial.print("\nДействие выполнено: ");
  Serial.println(timers_min_action);
  timers.remove(timers_min_index); // Удаление текущего таймера
  new_timer(timers); // Загрузить следующий таймер
}
