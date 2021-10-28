#pragma once
#define CONFIG_FILE "/config.txt"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <map>
#include <list>

long verContHost = 1;
void setValue(String key, String value, bool save);
std::map<String, String> ConfigContent;
typedef void (*configChangeCallback)(String, String);
std::list<configChangeCallback> onConfigChanges;
void setOnConfigChange(void (*func)(String key, String value))
{
  onConfigChanges.push_front(func);
}
SemaphoreHandle_t store_sem;
// Mỗi dòng là một phần tử (một cặp key value) (key):(value)\n
void loadFileIntoConfig(String content)
{
  Serial.println(content);
  while (content.indexOf("\n") >= 0)
  {
    String curLine = content.substring(0, content.indexOf("\n"));
    String key = curLine.substring(0, content.indexOf(":"));
    String value = curLine.substring(content.indexOf(":") + 1);
    ConfigContent[key] = value;
    content.remove(0, curLine.length() + 1);
  }
}
// Kiểm tra key có tồn tại không
bool checkKey(String key)
{
  if (ConfigContent.find(key) != ConfigContent.end())
  {
    return true;
  }
  return false;
}

// Lấy giá trị của Key
String getValue(String key, String def = "", bool setDefaultTokey = true)
{
  if (xSemaphoreTake(store_sem, portMAX_DELAY) == pdTRUE)
  {
    if (checkKey(key)){
      String ret = ConfigContent[key];
      xSemaphoreGive(store_sem);
      return ret;
    }
    else
    {
      if (setDefaultTokey)
      {
        xSemaphoreGive(store_sem);
        setValue(key, def, true);
      }
      return def;
    }
  }
  return "";
}

// Lấy toàn bộ file content
String getValuesByString()
{
  String ret = "";
  for (std::pair<String, String> e : ConfigContent)
  {
    String k = e.first;
    String v = e.second;
    ret += k + ":" + v + "\n";
  }
  return ret;
}
String getValuesByJson()
{
  DynamicJsonDocument doc(8192);
  JsonObject obj = doc.to<JsonObject>();
  String ret;
  for (std::pair<String, String> e : ConfigContent)
  {
    String k = e.first;
    String v = e.second;
    obj[k] = v;
  }
  serializeJson(obj, ret);
  return ret;
}
// Gán giá trị cho key
void setValue(String key, String value, bool save = true)
{
  if (xSemaphoreTake(store_sem, portMAX_DELAY) == pdTRUE)
  {
    ConfigContent[key] = value;
    verContHost++;
    // nếu không yêu cầu lưu vào flash
    if (!save){
      xSemaphoreGive(store_sem);
      return;
    }

    File cfg_file = SPIFFS.open(CONFIG_FILE, "w");
    if (!cfg_file)
    {
      cfg_file.close();
      return;
    }

    for (std::pair<String, String> e : ConfigContent)
    {
      String k = e.first;
      String v = e.second;
      cfg_file.print(k + ":" + v + "\n");
    }

    cfg_file.close();
    for (auto onConfigChange = onConfigChanges.begin();
         onConfigChange != onConfigChanges.end();
         ++onConfigChange)
    {
      if ((*onConfigChange) != NULL)
      {
        (*onConfigChange)(key, value);
      }
    }

    xSemaphoreGive(store_sem);
  }
}

// Khởi tạo
void setupConfig()
{

  store_sem = xSemaphoreCreateBinary();
  if (!SPIFFS.begin())
  {
    SPIFFS.format();
    if (!SPIFFS.begin())
    {
      return;
    }
    else
    {
      xSemaphoreGive(store_sem);
      Serial.println(" mounted SPIFFS");
    }
  }
  else
  {
    xSemaphoreGive(store_sem);
    Serial.println("SPIFFS mounted ");
  }

  File cfg_file = SPIFFS.open(CONFIG_FILE, "r");
  if (cfg_file)
  {
    String tmp = cfg_file.readString();
    loadFileIntoConfig(tmp);
  }
  cfg_file.close();
}

void loopConfig()
{
}
