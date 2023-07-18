# PROJECT TITLE: **A Renewable-Energy(Solar) Irradiance-Based Control System**

![Alt text](<Frame 3.jpg>)

## DESCRIPTION OF THE PROJECT
This project aims at creating an IOT device that will enable people that make use of solar panels to generate power to efficiently control power-intensive appliances based on the current sunlight level (irradiance). There will be a web application embedded on the ESP32 microcontroller which will enable clients to set parameters based on the irradiance.

## OVERVIEW OF PROJECT
For this project, the solar panels capture sunlight and converts it to electrical energy.

Voltage sensors measure the voltage output from the solar panels, representing the current irradiance level.

ESP32 microcontroller collects voltage data, performs conversion to irradiance and facilitates RF communication.

RF Transceiver enables wireless communication between the ESP32 and the appliance switches.

Application switches receives irradiance data and allows users to set appliance behavior based on the sunlight level. It also receives client settings and controls the connected appliances accordingly.

The webpage provides real-time irradiance information. This will enable the clients to view the current irradiance level, set appliance behavior based on the threshold irradiance value.

## LEARNING OBJECTIVES
This project will help in the following:

Energy efficiency: Appliances operate only when the sunlight level meets the user-defined criteria, reducing unnecessary energy consumption.

Save cost: It optimizes energy usuage by leveraging natural sunlight, reducing reliance on the electrical grid.

Sustainability: Utilizes renewable solar energy to power the system and promote eco-friendly practices.

User control: Clients can customize appliance behaviour based on their specific preferences and sunlight conditions.

Real-time monitoring: The webpage provides up-to-date irradiance information, enabling users to make informed decisions.

## TECHNOLOGIES USED
Framework: *PlatformIO*

Backend: *C/C++*

Frontend: *Html, css, Javascript*

Interface design: *Figma*

API: *Web socket*

Hardware: *ESP32 microcontroller, solar panels*

## CHALLENGES
Establishing a wireless communication between the hardware outside and the one inside.

The IOT device cannot be accessed through the internet. It can only be accessed through a local host because the web app is not hosted on a server but on the microcontroller. 

