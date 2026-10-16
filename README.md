# STM32F103C8T6 Digitally Controlled Synchronous Buck Converter

12 V input, 3.3-11.5 V output synchronous buck converter with digital voltage control on an STM32F103C8T6, built with STM32CubeIDE, HAL and FreeRTOS.

## Credits

The hardware design is based on EDADong. I refactored the firmware architecture, added the FreeRTOS tasks and the PID loop, and brought up and debugged the board.

## Hardware

- Synchronous buck power stage with high-side and low-side MOSFETs
- IR2104 half-bridge gate driver with bootstrap supply, driven by a single PWM input (PA8, TIM1)
- INA213 current-sense amplifier for output current measurement

## Firmware Architecture

| Module | Responsibility |
| --- | --- |
| ADC | Multi-channel sampling of output voltage and current using DMA |
| Measurement | Converts raw ADC counts to voltage/current, with offset and gain calibration |
| PWM | TIM1 PWM output to the IR2104 |
| Protection | Over-voltage and over-current fault detection |
| State machine | OFF / READY / RUN / FAULT states |
| RTOS | FreeRTOS tasks for input handling and control; PID voltage loop |

## Test Status

- [x] Board bring-up and open-loop operation verified with oscilloscope
- [x] ADC offset and gain calibration
- [ ] Closed-loop validation with bench supply and electronic load

## Debug Notes

Hardware and firmware issues found during bring-up, with root causes and fixes, are documented in [docs/](docs/).

## Build

1. Open STM32CubeIDE 2.2.0
2. File > Import > Existing Projects into Workspace > select the `Firmware` folder
3. Build and flash with ST-LINK
