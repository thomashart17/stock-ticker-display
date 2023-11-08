# stock-ticker-display

## Description
This project implements a simple stock ticker display using an LCD display connected to an Arduino Uno WiFi. The stock prices are retrived from the Yahoo Finance API periodically and displayed on the LCD screen. There is also a python script to automatically generate code required each time a ticker is added or removed.

## Dependencies
In order to use this project, users will need an Arduino Uno with WiFi capabilities and and LCD display with the required wiring. Currently the project is set up to use a standard LCD display with 16 columns and 2 rows, but it could very easily be reconfigured to use an I2C LCD.

## Installation
To install this project, simply clone the repository and open the Arduino sketch in Arduino IDE. To modify the tickers that are displayed, update the list in `scripts/tickers.json` and run `scripts/update_tickers.py`. This will generate the code required for the project to use these specific tickers. Note that the project will need to be recompiled and uploaded to the Arduino every time the script is run. In addition, users will have to create a `secrets.h` file in the `main` directory with the following contents with constants defined as `SSID` and `PASSWORD` for the WiFi network to connect to.
