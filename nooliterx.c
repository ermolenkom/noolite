/*        Console Utility for nooLite
        (c) Mikhail Ermolenko
        для подсказки - режим компиляции
        gcc nooliterx.c -o nooliterx -lusb-1.0
        выставить режим запуска в правами root
        # chown root nooliterx
        # chmod a+s nooliterx
*/

#define LINUX

#ifdef WINDOWS
#include "libusb.h"
#endif
#ifdef LINUX
#include <libusb-1.0/libusb.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DEV_VID 0x16c0 //0x5824
#define DEV_PID 0x05dc //0x1500
#define DEV_CONFIG 1
#define DEV_INTF 0
#define EP_IN 0x81
#define EP_OUT 0x01

unsigned char COMMAND_ACTION[8] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //{80,0x00,0xaa,0x00,0x0,0x1,0x14,0x05}

int main(int argc, char * argv[])
{
        libusb_device_handle * handle;
        int i, ret;
        unsigned int level;
        unsigned char command[1], buf[8], channel, togl;
        char param;
        char commandtxt[255]; //Prepare Command string

        libusb_init(NULL);
        libusb_set_debug(NULL, 3);
        handle = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID);
        if (handle == NULL) {
         printf("Не удалось найти устройство\n");
         libusb_exit(NULL);
         return 0;
        }
        if (libusb_kernel_driver_active(handle,DEV_INTF))
                libusb_detach_kernel_driver(handle, DEV_INTF);
        if ((ret = libusb_set_configuration(handle, DEV_CONFIG)) < 0)
        {
                printf("Ошибка конфигурации\n");
                libusb_close(handle);
                libusb_exit(NULL);
                if (ret == LIBUSB_ERROR_BUSY)
                 printf("B\n");
                printf("ret:%i\n", ret);
                return 0;
        }
        if (libusb_claim_interface(handle, DEV_INTF) < 0)
        {
                printf("Ошибка интерфейса\n");
                libusb_close(handle);
                libusb_exit(NULL);
                return 0;
        }
        //0x9 - номер запроса
        //0x300 - значение запроса - их надо получить из мониторинга

        //ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_IN, 0x9, 0x300, 0, COMMAND_ACTION, 8, 100);

    /*---Запускаем цикл до нажатия клавиши ENTER... ---*/
    while (1){

      char i;



ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_IN, 0x9, 0x300, 0, buf, 8, 1000);



if (togl!=buf[0]){
  printf("buf[0]:%i ", buf[0]);
  printf("buf[1]:%i ", buf[1]);
  printf("buf[2]:%i\n", buf[2]);
  
  togl = buf[0];

//  sprintf(commandtxt, "wget http://localhost/objects/?script=switchNooLitePress\\&buf0=%i\\&buf1=%i\\&buf2=%i\\&buf3=%i\\&buf4=%i\\&buf5=%i\\&buf6=%i\\&buf7=%i -O /dev/null",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
  sprintf(commandtxt, "timeout 120 wget -q -T 10  http://localhost/objects/?script=switchNooLitePress\\&buf0=%i\\&buf1=%i\\&buf2=%i\\&buf3=%i\\&buf4=%i\\&buf5=%i\\&buf6=%i\\&buf7=%i -O /dev/null &",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
  system(commandtxt);
  printf("comm:%s\n", commandtxt);
}
usleep(150000);

      scanf("%c",i);

      if (i=='\r') {
        printf("Выход\n");
        break;
      }
    }

        libusb_attach_kernel_driver(handle, DEV_INTF);
        libusb_close(handle);
        libusb_exit(NULL);
        return 0;
}
