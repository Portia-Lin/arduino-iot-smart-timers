# arduino-iot-smart-timers
## Скетч для создания, выполнения и удаления смарт таймеров в реальном времени

Чтобы создать новую строку содержащую таймеры воспользуйтесь файлом *timers_array.json*.
Создайте в нем новые таймеры и скопируйте их в окно "Input"  [Ассистента ArduinoJson 6](https://arduinojson.org/v6/assistant/).
Скопируйте из окна ассистена "Parsing program" необходимые данные для создания массива маймеров и замените их в скетче.

В скетче идут часы реального времени от значения 10/27/2020 @ 2:00am (UTC). Обновление данных происходит раз в минуту. 
Так же, в это время происходит проверка времени выполнения лижайшего таймера.

Для получения значений *Unix Time Stamp* воспользуйтесь сайтом [Epoch Unix Time Stamp Converter](https://www.unixtimestamp.com/).

Логирование действий в мониторе порта:
![Log](https://i.ibb.co/gmmmRRC/log.png)
