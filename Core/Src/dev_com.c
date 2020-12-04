/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : dev_comctrl.c
 * Author    : ygl
 * Brief     : 端口管理
 * Date      : 2018.11.18
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
 
#include "dev_com.h"
#include "drv_com.h"


// 自定义的缓冲
Com_paser_BuffTypedef m_com_buf;
// 接收回调函数
void (*m_com_rev_callBack)(unsigned char* data,uint32_t size) = 0;

static void dev_comctrl_buff_init(void);
static void dev_comctrl_interrput_rx_handle(uint8_t data);
static void dev_comctrl_tx_handle(void);
static void dev_comctrl_rx_handle(void);

/**
* @ Function Name : dev_comctrl_init
* @ Author        : ygl
* @ Brief         : 端口控制器初始化
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
void dev_comctrl_init(void)
{	
	dev_comctrl_buff_init();
	driver_com_regist_reccallback(1,dev_comctrl_interrput_rx_handle);
	//drv_com_printf(com1,"this is from com1");
}
/**
* @ Function Name : dev_comctrl_handle
* @ Author        : ygl
* @ Brief         : 端口控制器处理函数
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
void dev_comctrl_handle(void){
	 dev_comctrl_tx_handle();
	 dev_comctrl_rx_handle();
}
/**
* @ Function Name : dev_comctrl_handle
* @ Author        : ygl
* @ Brief         : 端口控制器注册接收回调函数
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
void dev_comctrl_regist_rx_callback(void (*arg_callBack)(unsigned char* data,uint32_t size)){
	m_com_rev_callBack = arg_callBack;
}
/**
* @ Function Name : dev_comctrl_buff_init
* @ Author        : ygl
* @ Brief         : 数据初始化
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
static void dev_comctrl_buff_init(void){
	// 清空缓冲区
	memset(&m_com_buf,0,sizeof(Com_paser_BuffTypedef));
}


/**
* @ Function Name : dev_comctrl_rx_handle(u8 data)
* @ Author        : ygl
* @ Brief         : 串口中断接收函数
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
static void dev_comctrl_interrput_rx_handle(uint8_t data){
	// 接收缓冲区添加数据
	m_com_buf.Rx_part[m_com_buf.Rx_write++] = data;
	// 如果达到末尾,写指针归零
	if(m_com_buf.Rx_write >= RX_BUFF_SIZE)
		m_com_buf.Rx_write = 0;
}


/**
* @ Function Name : dev_comctrl_handle
* @ Author        : ygl
* @ Brief         : 端口控制器接收处理
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
static void dev_comctrl_rx_handle(void){
	  int offset_dir;
    uint32_t temp;
    // 获取偏差量
    offset_dir = m_com_buf.Rx_write - m_com_buf.Rx_read;

    if(offset_dir > 0)
    {
        temp = (offset_dir>=RX_DEAL_MAX_SIZE)?RX_DEAL_MAX_SIZE:offset_dir;

    }
    else if(offset_dir < 0)
    {
        temp = ((RX_BUFF_SIZE-m_com_buf.Rx_read)>=RX_DEAL_MAX_SIZE)?
                RX_DEAL_MAX_SIZE:RX_BUFF_SIZE-m_com_buf.Rx_read;
    }
    else
    {
        return;
    }
    if(m_com_rev_callBack !=0)
    {
        m_com_rev_callBack(&m_com_buf.Rx_part[m_com_buf.Rx_read],temp);
        m_com_buf.Rx_read+=temp;
        m_com_buf.Rx_read = (m_com_buf.Rx_read >= RX_BUFF_SIZE) ? 0 : m_com_buf.Rx_read;
    }
}

/**
* @ Function Name : dev_comctrl_handle
* @ Author        : ygl
* @ Brief         : 端口控制器发送处理函数
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
static void dev_comctrl_tx_handle(void){
	
}

