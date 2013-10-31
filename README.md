noolite
=======
This project is written in C for woriking with NooLite USB stick (PC118, PC1116, PC1132, RX1164) for linux.

About NooLite: http://www.noo.com.by/

Author: Mikhail Ermolenko

E-mail: ermolenkom@yandex.ru

Site: https://github.com/ermolenkom/noolite

Этот проект написан на языке Си для работы c адаптером для компьютера системы NooLite для OS Linux.

За основу взят этот проект https://github.com/pythonchik/nooLite

Отличия: 

  увеличино количество команд для PC с 8 до 32
  
  добавлена возможность работать с RGB контроллером
  
  написаны отдельные драйвера для RX1164 на прием и передачу
  
  
Инструкция.

Для ОТПРАВКИ команд выключателям через USB адаптер (PC118, PC1116, PC1132)

Устанавливаем библиотеку libusb (нужна для работы с HID-устройством)

  sudo apt-get install libusb-1.0-0 libusb-1.0-0-dev
  
Скачиваем  

  wget https://github.com/ermolenkom/noolite/archive/noolitepc.tar.gz
  
Разархивируем  

  tar xvfz noolitepc.tar.gz

Компиляция, копирование, назначение прав:

  sudo gcc nooletepc.c -o noolitepc -lusb-1.0
  
  sudo cp noolitepc /usr/local/bin/noolitepc
  
  sudo chown root /usr/local/bin/noolitepc
  
  sudo chmod a+s /usr/local/bin/noolitepc
  

Использование:

  /usr/local/bin/noolitepc -api -set_ch 1 33
  
эта команда устанавливает на 1-м канале уровень в 33%

ВНИМАНИЕ! Отлисие API от версии под Windows. 

  Необходим ПРОБЕЛ перед номером канала.
  
  Нет МИНУСА перед уровнем яркости

Другие примеры:

Привязка устройства к 1-му каналу:

  /usr/local/bin/noolitepc -api -bind_ср 1
  
Включение устройств 1-ого канала:

  /usr/local/bin/noolitepc -api -on_ch 1
  
Выключение на 2 канал:

  /usr/local/bin/noolitepc -api -off_ch 2
  
Управление RGB контроллером (лиловый цвет):

  /usr/local/bin/noolitepc -api -set_color_ch 110 0 255

Помощь, по команде:

/usr/local/bin/noolitepc -help

Using /usr/local/bin/noolitepc -api -<command> <channel> [<level>|<RGB>]

    <command> may be:\n");
    
          -on_ch - Turn channel ON / Включить канал
          
          -off_ch - Turn channel OFF / Выключить канал
          
          -sw_ch - Switch channel ON/OFF / Переключить (вкл/выкл) канал
          
          -set_ch - Set level for channel / Установить уровень канала
          
          -bind_ch - Bind channel / Привязка канала
          
          -unbind_ch - Unbind channel / Отвязка канала
          
          -load_preset_ch - Load preset channel / Вызов сценария
          
          -save_preset_ch - Save preset channel / Запись сценария
          
          -stop_reg_ch - Stop change channel / Остановка перебора цвета
          
          -roll_color_ch - Rolling color / Перебор цвета
          
          -sw_color_ch - Swith color / Смена цвета
          
          -sw_mode_ch - Swith mode / Смена режипа перебора цвета
          
          -speed_mode_sw_ch - Swith speed change color / Смена скорости перебора цвета
          
          -set_color_ch - Set color R[0..255] G[0..255] B[0..255] / Установка цвета
          
     <channel> must be [1..32] / канал
     
     <level> must be [0..100] - using for -set_ch / уровень
     
     <RGB> must be [0..255] [0..255] [0..255] - using for - set_color_ch  / три числа через пробел
     

Для чтения нажатия клавиш USB адаптер RX1164

станавливаем библиотеку libusb (нужна для работы с HID-устройством)

  sudo apt-get install libusb-1.0-0 libusb-1.0-0-dev
  
Скачиваем  

  wget https://github.com/ermolenkom/noolite/archive/nooliterx.tar.gz
  
Разархивируем  

  tar xvfz nooliterx.tar.gz

Компиляция, копирование, назначение прав:

  sudo gcc nooleterx.c -o nooliterx -lusb-1.0
  
  sudo cp nooliterx /usr/local/bin/nooliterx
  
  sudo chown root /usr/local/bin/nooliterx
  
  sudo chmod a+s /usr/local/bin/nooliterx
  
Использование:

запустить 

  /usr/local/bin/nooliterx

Применение в системе MajorDoMo:

необходимо создать скрипт(сценарий) switchNooLitePress

текст скрипта:

$buf0=$params['buf0']; // состояние адаптера
$buf1=$params['buf1']; // адрес ячейки [0..63]
$buf2=$params['buf2']; // принятая команда
$buf3=$params['buf3']; // формат данных 0-нет, 1-1байт, 2-4байта
$buf4=$params['buf4']; // Байт данных 0 
$buf5=$params['buf5']; // Байт данных 1 
$buf6=$params['buf6']; // Байт данных 2 
$buf7=$params['buf7']; // Байт данных 3 

$objects=getObjectsByClass("switchNooLite");
foreach($objects as $obj){
  if (gg($obj['TITLE'].".channel")==$buf1) {
    callMethod($obj['TITLE'].".refresh",array("buf2"=>$buf2));
    return;
  }
}

Скрипт вызывается при нажатии пультов NooLite. В параметрах получет данные.

Далее он перебирает пульты (они введены как объекты класса switchNooLite) и запускает метод refresh нужной кнопки, 
с передачей параметров.

Применение в другой системе "Умный Дом":

При получении нажатия драйвер запрашивает веб-страницу на локальном сервере. Необходимо заменить строчку

wget http://localhost/objects/?script=switchNooLitePress\\&buf0=%i\\&buf1=%i\\&buf2=%i\\&buf3=%i\\&buf4=%i\\&buf5=%i\\&buf6=%i\\&buf7=%i -O /dev/null

на свой вариант.







