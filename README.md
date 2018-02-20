PINOUT FOR ESP32
================

This library uses VSPI bus ESP32

MCU                       FT810 HOTMCU
==================        ============
MOSI   23 (GPIO23)   -->      MOSI
MISO   19 (GPIO19)   -->      MISO
CLK    18 (GPIO18)   -->      CLK
CS      5 (GPIO5)    -->      CS

PIN PD HOTMCU

Una resistencia de 47K a 3,3v
Un condensador de 100k (o,1uF) a masa
La unión de estas dos a pin PD