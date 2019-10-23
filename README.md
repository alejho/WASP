<p align="center">
<b>WASP</b><br>
  A <i>WEMOS D1 Mini</i> based time-programmable smart plug with a web-based control interface
</p>

<img src="https://hackster.imgix.net/uploads/attachments/676077/cover_a1crsKnpTw.jpg?auto=compress%2Cformat&w=900&h=675&fit=min" alt="WASP" style="width:200px;height:300px;">

## Getting started

___WASP___ is written in form of an _Arduino_ sketch and can be edited/uploaded with the standard _Arduino IDE_

You must [download  and install these libraries](https://www.arduino.cc/en/Guide/Libraries) in order to get things work properly:

- FSBrowserNG
- Time
- ArduinoJson
- Ntp Client

Edit the file _webserver.cpp_  and input your network credentials:

<img src="https://hackster.imgix.net/uploads/attachments/676105/config_ootIRmmPaK.bmp?auto=compress%2Cformat&w=740&h=555&fit=max" alt="WASP" style="width:200px;height:300px;">

__Open the sketch and [upload the sketch data](https://github.com/esp8266/arduino-esp8266fs-plugin) and the sketch itself!__

Then connect with a browser (from PC or mobile device) to <b>http://wasp.local</b> or type directly the ip address you set in the _webserver.cpp_ file.

<img src="https://hackster.imgix.net/uploads/attachments/676085/mobile-first_kqG8sL0qhC.png?auto=compress%2Cformat&w=1280&h=960&fit=max" alt="WASP" style="width:200px;height:300px;">

See the [project page on hackster.io](https://www.hackster.io/alejho/wasp-wemos-advanced-smart-plug-e4df23) for wirings and more details!
