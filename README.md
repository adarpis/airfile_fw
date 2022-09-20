# AirFile

 AirFile firmware (challenge)

## USB CDC Console to Debug

To enable USB CDC and use `serial` instance over USB port, change the lines below in `platform.ini` file

```yaml
build_flags =
    ...
    -D ARDUINO_USB_CDC_ON_BOOT=0
    -D ARDUINO_USB_MODE=0
    ...
```

## TODO

- Security communication
- Device authentication
- Send battery status
- Check wake up sanity (reset by watchdog timer can draining battery energy fast)
