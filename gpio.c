#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "gpio.h"


#define BUFFER_MAX 4
#define DIRECTION_MAX 35
#define VALUE_MAX 31



/* pin PA29, 32*1+29 */
#define PE_FRONT    61
/* pin PD13, 32*4+13 */
#define PE_BEHIND   141
/* pin PA22, 32*1+22 */
#define POWER_5V_EN  54 


static unsigned char control_work[CONTROL_PIN_NUM];
typedef enum{
    CONTROL_ERROR = 0,
    CONTROL_WORK =  1
}CONTROL_STATE_E;

const char *control_name[] = {
    "PE_FRONT","PE_BEHIND","POWER_5V_EN"
};

const char *state_name[] = {
    "off", "on"
};

int state_pe_front, state_pe_behind, state_5v;

const int control_pin_num[CONTROL_PIN_NUM] = {
	PE_FRONT,	
	PE_BEHIND,
    POWER_5V_EN
};

int gpio_read(int pin)
{

    char path[VALUE_MAX];
    char value_str[3];
    int fd;

    memset(path, 0, VALUE_MAX);
    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (-1 == fd) {
		Err_log("pin %d failed open value for reading!\n", pin);
        return(-1);
    }

    if (-1 == read(fd, value_str, 3)) {
		Err_log("pin %d failed to read value!\n", pin);
        close(fd);
        return(-1);
    }

    close(fd);

    return(atoi(value_str));
}


int gpio_write(int pin, int value)
{
    char path[VALUE_MAX];
    int fd, ret;

    memset(path, 0, VALUE_MAX);
    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
		Err_log("pin %d Failed to open value.\n", pin);
        return(-1);
    }

    ret = write(fd, LOW == value ? "0" : "1", 1);

    if (1 != ret) {
		Err_log("pin %d Failed to write value!\n", pin);
        close(fd);
        return(-1);
    }

    close(fd);
    return(0);
}

int kr_gpio_control_switch(OUTPUT_TYPE_E type, OUTPUT_STATE_E state)
{
    int ret;
	
    if(type >= CONTROL_PIN_NUM){
        printf("%s()type '%d' not found\n", __func__, type);
        return -1;
    }

    if(control_work[type] == CONTROL_ERROR){
        printf("%s() control pin '%s' not work.\n", __func__, control_name[type]);
        return -1;
    }else{

        if(state != STATE_OFF && state != STATE_ON){
            printf("%s()type '%s' : state '%d' not found\n", __func__, control_name[type], state);
            return -1;
        }
        ret = gpio_write(control_pin_num[type], state==STATE_ON? HIGH : LOW);  //low volt for enable
        switch(type){
            case POWER_5V_EN_N:
                state_5v = state;
                break;
			case PE_FRONT_N:
                state_pe_front = state;
                break;
			case PE_BEHIND_N:
                state_pe_behind = state;
                break;
            default:
                printf("%s()type '%d' not found\n", __func__, type);
                return -1;
                break;
        }
    }

    printf("set '%s' to '%s' %s\n", control_name[type], state_name[state], ret?"err":"");
    return 0;
}

int gpio_export(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    memset(buffer, 0, BUFFER_MAX);
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) {
        Err_log("export pin %d failed.\n", pin);
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

int gpio_unexport(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    memset(buffer, 0, BUFFER_MAX);
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        Err_log("unexport pin %d failed.\n", pin);
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

int gpio_set_direction(int pin, int dir)
{
    static const char str_in[]  = "in";
    static const char str_out[] = "out";
    char path[DIRECTION_MAX];
    int fd;

    memset(path, 0, DIRECTION_MAX);
    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        Err_log("pin %d failed to open direction.\n", pin);
        return(-1);
    }

    if (-1 == write(fd, INPUT == dir ? str_in : str_out, INPUT == dir ? 2 : 3)) {
        Err_log("pin %d set direction '%s' failed!\n", pin, INPUT==dir ? str_in : str_out);
        close(fd);
        return(-1);
    }

    close(fd);
    return(0);
}


int kr_gpio_control_init(void)
{
    int i;
    int ret = 0;
	/* 初始化两个IO口为输入 */
    for(i=0; i< CONTROL_PIN_NUM; i++){
        if(gpio_export(control_pin_num[i]) || gpio_set_direction(control_pin_num[i], OUTPUT)){
            control_work[i] = CONTROL_ERROR;
            Err_log("Control pin '%s' initial Error\n", control_name[i]);
            ret = 1;
        }else{
            control_work[i] = CONTROL_WORK;
        }
		if(i == POWER_5V_EN_N || i == PE_FRONT_N || i == PE_BEHIND_N)
        	ret |= kr_gpio_control_switch(i, STATE_ON);
		else
			ret |= kr_gpio_control_switch(i, STATE_OFF);
    }
    return ret;
}


int kr_gpio_init(void)
{
    int ret = 0;
  
    ret = kr_gpio_control_init();
    if(ret != 0)
        return ret;

	ret |= gpio_set_direction(control_pin_num[PE_FRONT_N], INPUT);
	ret |= gpio_set_direction(control_pin_num[PE_BEHIND_N], INPUT);

    return ret;
}

