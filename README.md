# Zephyr Learnings

Hands-on Zephyr RTOS work on an STM32F401RE Nucleo board.

## 01_App_Button

Application that reads a button through a custom out-of-tree driver.
It has no GPIO code of its own — it gets the device with `DEVICE_DT_GET`
and calls through the driver's API struct.

## modules/button

An out-of-tree Zephyr module providing a `custom,button` driver.

- `dts/bindings/input/custom,button.yaml` — devicetree binding
- `drivers/button/button.h` — API struct and config struct
- `drivers/button/button.c` — driver: init, state read, and the
  `DEVICE_DT_INST_DEFINE` instantiation macro
- `zephyr/module.yaml` — makes the folder a Zephyr module

One devicetree node per button; `DT_INST_FOREACH_STATUS_OKAY` creates
an instance for each, so adding a second button needs no driver changes.

## Build

    west build -b nucleo_f401re . -p always
    west flash

Board: ST Nucleo-F401RE. Button is the onboard B1 on PC13 (active low).