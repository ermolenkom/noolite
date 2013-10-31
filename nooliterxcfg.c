/*        Console Utility for nooLite
        (c) Mikhail Ermolenko
        для подсказки - режим компиляции
        gcc nooliterxcfg.c -o nooliterxcfg -lusb-1.0
        выставить режим запуска в правами root
        # chown root nooliterxcfg
        # chmod a+s nooliterxcfg
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

unsigned char COMMAND_ACTION[8] = {0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //{80,0x00,0xaa,0x00,0x0,0x1,0x14,0x05}

int main(int argc, char * argv[])
{
        libusb_device_handle * handle;
        
        int i, ret;
        unsigned int level;
        unsigned char command[1], buf[8], channel;
        char param;
        
        //Arg Control

        if (argc == 1) {
                printf("Using: %s -help\n", argv[0]);
                return -1;
        }

        if (argc >= 2) {
                if(strcmp (argv[1],"-api")){ //first arg "-api" needed
                        if (strcmp (argv[1],"-help")==0)
                        {
                                printf("Using %s -api -<command> <channel> [<level>]\n", argv[0]);
                                printf("        <command> may be:\n");
                                printf("                -norm_ch - normal job \n");
                                printf("                -clear_ch - clear \n");
                                printf("                -clearall_ch - Clear ALL\n");
                                printf("                -reset_ch - Reset channel\n");
                                printf("                -bind_ch - Bind channel\n");
                                printf("                -unbind_ch - Unbind channel\n");
                                printf("        <channel> must be [1..8]\n");
                                return -1;
                        }
                        printf("Неверно указан режим\nИспользование: %s -api -<command> <channel> [<level>]\n", argv[0]);
                        return -1;
                }
        } else {
                printf("Не указан параметр API\r\nИспользование: %s -api -<command> <channel> [<level>]\n", argv[0]);
                return -1;
        }
        
        if (argc >= 3) {
                if (strcmp (argv[2],"-norm_ch")==0) //0-нормальная работа
                {
                        COMMAND_ACTION[0] = 1;
                }
                else if (strcmp(argv[2],"-bind_ch")==0) //1-включить привязку на адрес ячейки, 30 секунд
                {
                        COMMAND_ACTION[0] = 2;
                }
                else if (strcmp(argv[2],"-unbind_ch")==0) //2- выключить привязку принудительно
                {
                        COMMAND_ACTION[0] = 4;
                }
                else if (strcmp(argv[2],"-clear_ch")==0) //3- очистить ячейку (адрес ячейки)
                {
                        COMMAND_ACTION[0] = 8;
                }
                else if (strcmp(argv[2],"-clearall_ch")==0) //4- очистить всю память 
                {
                        COMMAND_ACTION[0] = 16;
                }
                else if (strcmp(argv[2],"-reset_ch")==0) //5- сбросить значение калибровки USB 
                {
                        COMMAND_ACTION[0] = 32;
                }
                else
                {
                        printf("Command unknown\n");
                        return -1;
                }
        } else {
                printf("Не указана команда\nИспользование: %s -api -<command> <channel>\n", argv[0]);
                return -1;
        }

        if (argc >= 4) {
                channel        = atoi(argv[3]);
                channel--;
                if ((channel>63)||(channel<0)) {
                        printf("Неверно указан канал (1-8)\nИспользование: %s -api -<command> <channel> \n", argv[0]);
                        return -1;
                }
                COMMAND_ACTION[1] = channel;
        } else {
                printf("Не указан канал\nИспользование: %s -api -<command> <channel>\n", argv[0]);
                return -1;
        }

//Prepare Command string
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

        ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT, 0x9, 0x300, 0, COMMAND_ACTION, 8, 100);

        libusb_attach_kernel_driver(handle, DEV_INTF);
        libusb_close(handle);
        libusb_exit(NULL);
        
        return 0;
}
