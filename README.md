# STM32 循迹小车开源工程

本仓库整理自“STM32 小车开源芋头雨”资料包，保留原始代码、硬件资料、结构建模和说明文档，供后续 Agent 与开发者直接检索。

## 代码入口

工程使用 Keil MDK 和 STM32 标准外设库，目标器件为 STM32F103C8。

### 基础版本（元素未开启）

- Keil 工程：`STM32小车开源芋头雨/代码部分/基础版本（元素未开启）/Project.uvprojx`
- 主程序：`STM32小车开源芋头雨/代码部分/基础版本（元素未开启）/User/main.c`
- 控制入口：`Control/pid.c` 中的 `Control()`
- 传感器误差：`Control/error.c` 中的 `Error_Calcaulate()` 与 `get_fused_error()`
- 路径元素：`Control/element.c`

注意：该版本源码默认 `Basic_Speed=0`，且 `Element_Process()` 只启用 `Element_Normal()`；圆弧、脱线、十字、调头和停车等函数大多被注释，不能仅根据函数是否存在判断功能已经开启。

### 超声波避障版本

- Keil 工程：`STM32小车开源芋头雨/代码部分/超声波避障版本/Project.uvprojx`
- 主程序：`STM32小车开源芋头雨/代码部分/超声波避障版本/User/main.c`
- 超声波驱动：`Hardware/ultrasonic.c`
- 避障状态：`Control/element.c` 中的 `Element_Busy()`

该版本保留相同的循迹控制架构，并增加双路超声波避障。它的基础速度、传感器位置权重和 PD/PI 参数与基础版不同。

## 资料目录

- `STM32小车开源芋头雨/硬件部分/`：PCB、原理图、结构建模和打印文件
- `STM32小车开源芋头雨/多路循迹移植说明.docx`：循迹移植资料
- `STM32小车开源芋头雨/元素思路.pdf`：路径元素设计资料
- `STM32小车开源芋头雨/常见问题说明.pdf`：常见问题
- `STM32小车开源芋头雨/stm32修改日志.pdf`：版本修改记录

## Agent 阅读顺序

1. 先读本文件，确认需要基础版还是超声波版。
2. 按 `main.c → Control/pid.c → Control/error.c → Control/element.c` 阅读循迹链路。
3. 再查看 `Hardware/sensor.c`、`Motor.c`、`PWM.c`、`Encoder.c`，核对实际引脚和输出标度。
4. 移植前必须确认目标板的五路探头、双通道编码器、TIM1 电机 PWM 和方向引脚是否和原工程一致。

## 说明

仓库保留原资料包中的 Keil 中间文件与预编译产物，便于核对历史状态。它们不是当前新工程的构建基线。
