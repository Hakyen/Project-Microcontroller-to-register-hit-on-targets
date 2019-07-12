bool SPIFFS_Active = false;

void FS_initialization()
{
  if (SPIFFS.begin())
  {
    SPIFFS_Active = true;
  }
}

// Чтение файлов из SPIFFS
// Если SPIFFS не запущен вернет строку с текстом "SPIFFS Error"
// Если не удается найти файл с указанным именем вернет строку "Unable To Find 'Имя файла'"
// Если не удается открыть для чтения указанный файл вернет строку "Unable To Open 'Имя файла' for Reading"
String Read_File(String File_Name)
{
  String Buffer_String;
  if (SPIFFS_Active)
  {
    if (SPIFFS.exists(File_Name))
    {
      File File_ = SPIFFS.open(File_Name, "r");
      if (File_)
      {
        Buffer_String = File_.readString();
        File_.close();
      }
      else
      {
        Buffer_String = "Unable To Open '" + File_Name + "' for Reading";
      }
    }
    else
    {
      Buffer_String = "Unable To Find '" + File_Name + "'";
    }
  }
  else
  {
    Buffer_String = "SPIFFS Error";
  }
  return Buffer_String;
}
