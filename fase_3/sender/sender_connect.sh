#!/bin/bash

connmanctl enable wifi
connmanctl tether wifi on rpiWifi wifi1234

./main