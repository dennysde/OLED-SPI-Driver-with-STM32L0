# OLED I2C Driver for STM32L0

Driver for OLED display 64x48 /0.66' -> I2C on STM32.

## Demostration

<img width="764" height="288" alt="demo" src="https://github.com/user-attachments/assets/ccfb4a33-a2cf-4ce7-8a6e-96acd0109809" />


## Hardware

- STM32L011K4
- Display OLED 64x48 I2C -> WEA006448AWPP3N00000
- NUCLEO board -> NUCLEO-L011K4

## Pin Connection

| OLED Pin | Function  | STM32 Pin | NUCLEO-L011K4 pin |
|----------|-----------|-----------|-------------------|
| VCC      | Power     | 3.3V      | 3.3V              |
| GND      | Ground    | GND       | GND               |
| SCL      | I2C Clock | PA4       | A3                |
| SDA      | I2C Data  | PA10      | D0                | 

<img width="1256" height="504" alt="Pinout_ligação display c placa" src="https://github.com/user-attachments/assets/1913d006-b85a-46d4-980e-6b11b3d3b86b" />

## Features

- I2C interface
- Text rendering
- Icon rendering
- images
- Runing on STM32CubeIDE V2.1 / STM32CubeMX 6.17
