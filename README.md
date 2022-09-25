# AirFile

 AirFile firmware (challenge)

## USB CDC Console to Debug

To enable USB CDC and use `serial` instance over USB port, change the lines below in `platform.ini` file

```yaml
build_flags =
    ...
    -D ARDUINO_USB_CDC_ON_BOOT=1
    -D ARDUINO_USB_MODE=1
    ...
```

## TODO

- Security communication (TLS)
- Device authentication (CliendID/OAuth) in application layer
- Send battery status
- Check wake up sanity (reset by watchdog timer can draining battery energy fast)
- If doesn't exist file send a alert with hardware problem and device doesn't power off
- Easy connect wifi
