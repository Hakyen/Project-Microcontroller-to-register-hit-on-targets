const String _Style_CSS = "/style.css";
const String _Not_found = "/404.html";

void HTTP() // Создаем страницы HTTP сервера для работы приложения
{
  server.onNotFound(Send_Not_found_page); // Если клиент обращается по несуществующему адресу
  server.on("/", Send_Main_page); // Главная страница
  server.begin(); // Запускаем сервер
}

void Send_Not_found_page() // Если клиент обращается по несуществующему адресу
{
  server.send(404, "text / html", Read_File(_Style_CSS) + Read_File(_Not_found)); // Отправка страницы пользователю
}

void Send_Main_page() // Главная страница
{
  String _Page; // Переменная для записи в неё HTML текста страницы
  _Page = "<!DOCTYPE html><html><head>";
  _Page += Read_File(_Style_CSS);
  _Page += "<meta charset=\"utf-8\"><title>TARGET Live</title></head><body>";
  if (server.arg("SR") == "Сохранить и перезагрузить") // Если клиент нажал кнопку "Сохранить и перезагрузить"
  {
    Config(false); // Записываем в EEPROM текущие настройки
    _Page += "<div class = \"main\">Данные сохранены, мишень будет перезагруженна</div></body></html>";
    server.send(200, "text / html", _Page); // Отправляем страницу пользователю
    delay(1000); // Ждем 1 секунду
    ESP.restart(); // Отправляем команду на перезагрузку платы
  }
  if (server.arg("Save_RGB") == "Принять") // Клиент применил внесенные изменения в настройку RGB сигналов
  {
    for (int i = 0; i < Segment_Count; i++) // Проходим по полученным данным и записываем их в RGB_
    {
      String _nameR = "R";
      String _nameG = "G";
      String _nameB = "B";
      _nameR += i;
      _nameG += i;
      _nameB += i;
      String _Red = server.arg(_nameR);
      String _Green = server.arg(_nameG);
      String _Blue = server.arg(_nameB);
      RGB_[i][0] = constrain(_Red.toInt(), 0, 255);
      RGB_[i][1] = constrain(_Green.toInt(), 0, 255);
      RGB_[i][2] = constrain(_Blue.toInt(), 0, 255);
    }
  }
  if (server.arg("Save_sectors_count") == "Принять") // Клиент применил внесенные изменения в настройку количества секторов мишени
  {
    String _buffer = server.arg("Sectors_count");
    Segment_Count = constrain(_buffer.toInt(), 1, 9); // Переводим полученное значение в диапазон от 1 до 9 и записываем в переменную кол-ва секторов
  }
  _Page += "<div class=\"header\"><h1>Настройка ESP8266</h1></div><div class=\"main\"><h1>Настройка количества секторов</h1>"
           "<form name=\"Sectors\" method = \"POST\"><table align=\"center\"><tr><td>Активных секторов</td><td>"
           "<input type=\"text\" name=\"Sectors_count\" value=\"";
  _Page += Segment_Count;
  _Page += "\" class=\"input_number\"></td></tr></table><table align=\"center\"><tr><td>сектор:</td><td>PIN:</td></tr>";
  for (int i = 1; i <= Segment_Count; i++)
  {
    _Page += "<tr><td>";
    _Page += i;
    _Page += "</td><td>";
    _Page += Names_Avalible_PIN[i];
    _Page += "</td></tr>";
  }
  _Page += "</table><input type=\"submit\" name=\"Save_sectors_count\" value=\"Принять\"></form></div>";
  _Page += "<div class=\"main\"><h1>Настройка RGB сигналов</h1><br><form name=\"RGB\" method = \"POST\"><table align=\"center\">"
           "<tr><td>сектор:</td><td colspan=\"3\">сигнал</td></tr><tr><td></td><td>R</td><td>G</td><td>B</td></tr>";
  for (int i = 0; i < Segment_Count; i++)
  {
    _Page += "<tr><td>";
    _Page += i + 1;
    _Page += "</td><td><input type=\"text\" name=\"R";
    _Page += i;
    _Page += "\" value=\"";
    _Page += RGB_[i][0];
    _Page += "\" class=\"input_number\"></td><td><input type=\"text\" name=\"G";
    _Page += i;
    _Page += "\" value=\"";
    _Page += RGB_[i][1];
    _Page += "\" class=\"input_number\"></td><td><input type=\"text\" name=\"B";
    _Page += i;
    _Page += "\" value=\"";
    _Page += RGB_[i][2];
    _Page += "\" class=\"input_number\"></td></tr>\"";
  }
  _Page += "</table><input type=\"submit\" name=\"Save_RGB\" value=\"Принять\"></form></div>"
           "<div class = \"main\"><form name = \"Save_and_Reset\" method = \"POST\"><input type=\"submit\" name=\"SR\" value=\"Сохранить и перезагрузить\">"
           "</form></div><div class=\"foter\">П33-16 Савельев Г.А.</div></body></html>";
  server.send(200, "text / html", _Page); // Отправка страницы пользователю
}
