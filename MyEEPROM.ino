int address = 0; // Переменная для хранения текущего адреса ячейки EEPROM

void Clear_EEPROM() // Очистить все ячейки памяти в EEPROM
{
  for (int i = 0; i < 512; i++) // проходим по всем 512 ячейкам EEPROM
  {
    EEPROM.write(i, '\0'); // записываем в ячейку \0
  }
  EEPROM.commit(); // Принимаем изменения в памяти
}

String Read_all_EEPROM() // считать всё из EEPROM
{
  String _Data = ""; // Буферная переменная типа String
  char _Symbol; // Буферная переменная типа char для последовательной перезаписи в неё каждого символа считанного из EEPROM
  for (int i = 0; i < 512; i++) // проходим по всем 512 ячейкам EEPROM
  {
    _Symbol = EEPROM.read(i); // Считываем значение ячейки EEPROM и записываем его в _Symbol
    if (_Symbol != '\0') // Если символ не \0 добавляем его в конец _Data
    {
      _Data += _Symbol;
    }
    else // Если символ \0 добавляем в конец _Data пробел
    {
      _Data += " ";
    }
  }
  return _Data; // Возвращаем строку собранную из всех ячеек памяти
}

void Write_String_To_EEPROM(String Data) // Функция записи строки в EEPROM
{
  int Data_size = Data.length(); // Узнаем размер текста в битах
  for (int i = 0; i < Data_size; i++) // Проходим по каждому элементу строки
  {
    EEPROM.write(address, Data[i]); // Записываем символ в ячейку
    address++; // Проходим к следующей ячейке EEPROM
  }
  EEPROM.write(address, '\0'); // добавляем null в конец Data
  EEPROM.commit(); // Принимаем внесенные изменения
  address++; // Проходим к следующей ячейке EEPROM
}

String Read_String_From_EEPROM(int _address) // Функция чтения переменной типа String из EEPROM
{
  String Data = ""; // Переменная для посимвольной записи в неё из EEPROM
  char symbol; // Переменная для записи в нее символа из EEPROM
  while (symbol != '\0') // Проходим до NULL
  {
    symbol = EEPROM.read(_address); // Записываем в symbol символ из EEPROM
    _address++; // Переходим к следующей ячейке
    Data += symbol; // Добавляем считанный символ к строке
  }
  return Data; // Возвращаем строку
}

void Config(bool _Read) // Функция чтения и записи настроек
{
  String _Data = ""; // Буферная переменная типа String
  String _Symbol = ""; // Буферная переменная типа char
  int _Address = 0; // Буферная переменная для хранения текущего адреса ячейки EEPROM
  int _Cell_array_1 = 0;
  int _Cell_array_2 = 0;
  if (_Read == true) // Режим чтения из памяти
  {
    // Segment_Count
    _Data = Read_String_From_EEPROM(_Address); // Считываем из EEPROM до \0
    _Address += _Data.length(); // Прибавляем к текущему адресу размер считанной строки
    Segment_Count = _Data.toInt(); // Переводим строку в переменную типа int и записываем её
    // Заполняем массив цветов нулевыми значениями
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        RGB_[i][j] = 0;
      }
    }
    _Data = Read_String_From_EEPROM(_Address); // Считываем из EEPROM от конца предыдущего чтения до \0
    _Address += _Data.length(); // Прибавляем к текущему адресу размер считанной строки
    // Записываем значения из строки в двумерный массив
    for (int i = 0; i < _Data.length(); i++)
    {
      _Symbol = _Data[i];
      if(_Symbol == ",")
      {
        _Cell_array_1++;
      }
      else if(_Symbol == ";")
      {
        _Cell_array_1 = 0;
        _Cell_array_2++;
      }
      else
      {
        RGB_[_Cell_array_2][_Cell_array_1] = RGB_[_Cell_array_2][_Cell_array_1] * 10 + _Symbol.toInt();
      }
    }
  }
  else // Режим записи в память
  {
    address = 0; // обнуляем текущий адрес ячейки EEPROM
    Write_String_To_EEPROM(String(Segment_Count)); // Записываем в память значение кол-ва сегментов
    // Переводим значения из двумерного массива в строку
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        _Data += RGB_[i][j];
        if (j < 2)
        {
          _Data += ",";
        }
      }
      _Data += ";";
    }
    Write_String_To_EEPROM(_Data); // Записываем в память значения массива цветов в виде строки
  }
}