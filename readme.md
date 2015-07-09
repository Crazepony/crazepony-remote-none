## 概述说明
这是Crazepony微型四轴飞行器遥控器代码，未使用实时操作系统。

使用Keil 5进行编译开发，使用UTF-8编码。更详细的开发操作，见Crazepony百科的[开发指南](http://www.crazepony.com/wiki-crazepony.html#rd)。

配合Crazepony微型四轴飞控代码[crazepony-firmware-none](https://github.com/Crazepony/crazepony-firmware-none)使用。

* 默认为左手油门（美国手）

## Overview
This is the RC transmitter source code of Crazepony MINI quadcopter.The suffix none means no RTOS used.

The project is developed in [Keil MDK Version 5 IDE](http://www2.keil.com/mdk5/).You could just import the project to Keil 5 and compile it.The Makefile for cross toolchain in Linux is comming soon.

The default mode is the throttle on the left joystick.The throttle joystick self centres in  the up/down axis and have a excellent operation experience in altitude hold mode. 
