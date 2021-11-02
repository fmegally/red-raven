#include "dispatcher.h"

handler_func_t tlgrm_handler_table[ID_TABLE_SIZE] = 
    {
        [ECHO_MSG] = handler_echo_msg,
        [GPIO_CMD] = handler_gpio_cmd,
        [ADC_CMD]  = handler_adc_cmd
    };

enum gpio_cmd_list
{
        SET_DDR=0x01,
        SET_PORT,
        GET_PORT,
        SET_PIN,
        CLR_PIN
};

enum adc_cmd_list
{
        ENABLE_ADC=0x01,
        SET_CHANNEL,
        GET_CONV_VALUE
};

static
int is_valid_id(int id)
{
        switch(id){
                case NULL_MSG:
                case ECHO_MSG:
                case ACK:
                case CONFIRM:
                case GPIO_CMD:
                case GPIO_REPLY:
                case ADC_CMD:
                case EXCEPTION:
                case NAK:
                        return 1;
                default:
                        return 0;
                }
}

static
int8_t handler_adc_cmd(void* data)
{
        struct adc_cmd
        {
                uint8_t cmd; 
                uint8_t value;
        };

        struct adc_cmd *args = (struct adc_cmd *) data;

        switch(args->cmd){
                case ENABLE_ADC:
                        adc_enable (ADC0);
                        adc_select_refv (ADC0, args->value);
                        break; 
                
                case SET_CHANNEL:
                        adc_select_ch (ADC0, args->value);
                        break;

                case GET_CONV_VALUE:
                        uint16_t tmp = adc_read_ch_blocking (ADC0);

                        struct telegram reply = {ADC_REPLY,{(tmp>>8) & 0xFF,tmp & 0xFF}};

                        reply.chksum = chksum((uint8_t*)&reply,sizeof(struct telegram) - 1);

                        UART_putc(UART0, SD);
                        UART_write(UART0, (uint8_t*)&reply,sizeof(struct telegram));
                        UART_putc(UART0, ED);

                        break;

                default:
                        break;
        }

        return 0;
}
        
static
int8_t handler_gpio_cmd(void* data)
{
        struct gpio_cmd
        {
                uint8_t cmd;
                uint8_t port;
                uint8_t pin;
                uint8_t value;
        };

        struct gpio_cmd *args = (struct gpio_cmd *)data;
        gpio_t* const port = gpio_ports_list[args->port];

        switch(args->cmd){
                case SET_DDR:
                        gpio_setmode(port,args->value);
                        break;

                case SET_PORT:
                        gpio_pwrite(port,args->value);
                        break;

                case GET_PORT:
                        uint8_t port_value = gpio_pread(port);
                        struct telegram reply = {GPIO_REPLY,{port_value}};
                        reply.chksum = chksum((uint8_t*)&reply,sizeof(struct telegram) - 1);
                        UART_putc(UART0, SD);
                        UART_write(UART0, (uint8_t*)&reply,sizeof(struct telegram));
                        UART_putc(UART0, ED);
                        break;

                case SET_PIN:
                        if( args->pin < 8){
                                gpio_setbit(port,args->pin);
                                break;
                        } else {
                                break;
                        }

                case CLR_PIN:
                        if( args->pin < 8){
                                gpio_clrbit(port,args->pin);
                                break;
                        } else {
                                break;
                        }

                default:
                        break;

        }
        return 0;
}


static
int8_t handler_echo_msg(void* data)
{
        return 0;
}


int8_t dispatch(struct telegram *tg, handler_func_t table[])
{
        int8_t r = (*(table[tg->id]))(tg->data);
        return r;
}
