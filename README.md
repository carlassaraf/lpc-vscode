# LPC-VSCODE
Embedded software development workspace for LPC4337.

## Supported targets:
- LPC4337 (M4 and M0 cores)

## Supported boards:
- CIAA-NXP and EDU-CIAA-NXP (www.proyecto-ciaa.com.ar)

## Available libraries:
- [CMSIS](http://www.arm.com/products/processors/cortex-m/cortex-microcontroller-software-interface-standard.php) and [LPCOpen](https://www.lpcware.com/lpcopen)
- [CMSIS-DSPLIB](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html)
- [lwip](http://lwip.wikia.com/wiki/LwIP_Wiki)
- [FreeRTOS](http://www.freertos.org/)

## Supported toolchains:
- gcc-arm-none-eabi

## Usage
* Make sure you have an ```arm-none-eabi-*``` toolchain configured in your ```PATH```. If you don't have it, download [GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded).
* Have Visual Studio Code and Cortex-Debug extension installed.
* ```git clone https://github.com/carlassaraf/lpc-vscode.git && cd lpc-vscode```
* ```cp project.mk.template project.mk && code .```
* Define ```PROJECT```, ```TARGET``` and ```BOARD``` (optional) variables in ```project.mk``` according to the project you want to compile.
* Compile with ```make``` or run ```Build``` task from the command pallete.

## Download firmware and debug options

* First option is with OpenOCD. Make sure that you have it installed and run ```make download``` if you only want to flash the firmware or run the ```Debug with OpenOCD``` debug session.
* If you have an MCU-Link Debug Probe, run the ```Start LinkServer``` task from the command pallete and then debug the firmware running the ```MCU-Link M4``` debug session.

## Acknowledgements
This was possible thanks to the Argentine Open Industrial Computer project (Proyecto CIAA). See more about the project at http://www.github.com/ciaa and http://www.proyecto-ciaa.com.ar.

Special thanks to [pridolfi](https://github.com/pridolfi) from whom this repository is forked and [ErichStyger](https://github.com/ErichStyger) for the data on how to implement MCU-Link Debug Probe with VS Code.
