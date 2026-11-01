#include "Ultrasonic.h"
#include "Delay.h"
#include "myfile.h"
float g_Ultrasonic1Distance = 400.0f;   // cm，初始认为前方很远
float g_Ultrasonic2Distance= 400.0f;
float g_UltrasonicThreshold = 10.0f;   // cm，默认刹车阈值
#define FILTER_ALPHA 0.5f  // 范围：0.0~1.0，越小越平滑，0.3是个常用值
void Ultrasonic_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Trig 引脚：推挽输出 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = ULTRASONIC1_TRIG_PIN;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ULTRASONIC1_TRIG_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(ULTRASONIC1_TRIG_PORT, ULTRASONIC1_TRIG_PIN);

    /* Echo 引脚：浮空输入 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = ULTRASONIC1_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ULTRASONIC1_ECHO_PORT, &GPIO_InitStructure);
	
	
    // 2. 初始化Trig引脚（PA9，推挽输出）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = ULTRASONIC2_TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ULTRASONIC2_TRIG_PORT, &GPIO_InitStructure);
    
    // 3. 初始化Echo引脚（PA10，浮空输入）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = ULTRASONIC2_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ULTRASONIC2_ECHO_PORT, &GPIO_InitStructure);
	
}


float Ultrasonic1_MeasureDistance(void)
{
    uint32_t timeout = 0;
    uint32_t high_time = 0;
    float distance = -1.0f;

    /* 发送 10us Trig 脉冲 */
    GPIO_ResetBits(ULTRASONIC1_TRIG_PORT, ULTRASONIC1_TRIG_PIN);
    Delay_us(2);
    GPIO_SetBits(ULTRASONIC1_TRIG_PORT, ULTRASONIC1_TRIG_PIN);
    Delay_us(10);
    GPIO_ResetBits(ULTRASONIC1_TRIG_PORT, ULTRASONIC1_TRIG_PIN);

    /* 等待 Echo 置高，带超时保护 */
    while (GPIO_ReadInputDataBit(ULTRASONIC1_ECHO_PORT, ULTRASONIC1_ECHO_PIN) == Bit_RESET)
    {
        Delay_us(1);
        if (++timeout > 500)   /* 等待超过约 30ms 视为失败 */
        {
            return -1.0f;
        }
    }

    /* 统计 Echo 高电平持续时间，步长约 1us */
    while (GPIO_ReadInputDataBit(ULTRASONIC1_ECHO_PORT, ULTRASONIC1_ECHO_PIN) == Bit_SET)
    {
        Delay_us(1);
        high_time++;
        if (high_time > 1500)   /* 超过约 60ms，视为无效回波 */
        {
            break;
        }
    }

    /* 声速约 340m/s，往返一次：
       距离(cm) ≈ 时间(us) * 0.017 */
    if (high_time > 0)
    {
        distance = high_time * 0.017f;
    }

    return distance;
}
/* 模块2测距（新增，与模块1完全相同，只是引脚不同） */
float Ultrasonic2_MeasureDistance(void)
{
    uint32_t timeout = 0;
    uint32_t high_time = 0;
    float distance = -1.0f;

    GPIO_ResetBits(ULTRASONIC2_TRIG_PORT, ULTRASONIC2_TRIG_PIN);
    Delay_us(2);
    GPIO_SetBits(ULTRASONIC2_TRIG_PORT, ULTRASONIC2_TRIG_PIN);
    Delay_us(10);
    GPIO_ResetBits(ULTRASONIC2_TRIG_PORT, ULTRASONIC2_TRIG_PIN);

    while (GPIO_ReadInputDataBit(ULTRASONIC2_ECHO_PORT, ULTRASONIC2_ECHO_PIN) == Bit_RESET)
    {
        Delay_us(1);
        if (++timeout > 500) return -1.0f;
    }

    while (GPIO_ReadInputDataBit(ULTRASONIC2_ECHO_PORT, ULTRASONIC2_ECHO_PIN) == Bit_SET)
    {
        Delay_us(1);
        high_time++;
        if (high_time > 1500) break;
    }

    if (high_time > 0) distance = high_time * 0.017f;
    return distance;
}
void Ultrasonic1_UpdateDistance(void)
{
    static float last_value1 = 400.0f;
    float d = Ultrasonic1_MeasureDistance();
    if (d >= 0.0f)
    {
        g_Ultrasonic1Distance = LowPassFilter(d, &last_value1, 0.7f);
		d=Min_Max(d,0,400);
    }
}
/* 更新模块2距离（新增） */
void Ultrasonic2_UpdateDistance(void)
{
      static float last_value2 = 400.0f;
    float d = Ultrasonic2_MeasureDistance();
    if (d >= 0.0f)
    {
        g_Ultrasonic2Distance = LowPassFilter(d, &last_value2, 0.7f);
		d=Min_Max(d,0,400);
    }
}

// 低通滤波函数
float LowPassFilter(float new_value, float* last_value, float alpha)
{
    *last_value = alpha * new_value + (1.0f - alpha) * (*last_value);
    return *last_value;
}

