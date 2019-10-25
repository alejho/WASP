<b>WASP</b> is a Wemos D1 mini time-programmable smart plug with a web-based control interface:</br>

<img src="https://hackster.imgix.net/uploads/attachments/676077/cover_a1crsKnpTw.jpg?auto=compress%2Cformat&w=900&h=675&fit=min" alt="WASP" style="width:200px;height:300px;">

<h2>Getting started:</h2>
<b>WASP</b> is written in form of Arduino sketch and can be edited/uploaded with the standard Arduino IDE</br></br>

You must <a href="https://www.arduino.cc/en/Guide/Libraries">download and install these libraries</a> in order to get things work properly:

<ul>
<li>FSBrowserNG</li>
<li>Time</li>
<li>ArduinoJson</li>
<li>Ntp Client</li>
</ul>


Edit the file <b>webserver.cpp</b> for network settings:<br><br>
<img src="https://hackster.imgix.net/uploads/attachments/676105/config_ootIRmmPaK.bmp?auto=compress%2Cformat&w=740&h=555&fit=max" alt="WASP" style="width:200px;height:300px;">
<br><br>
<b>Open the sketch and <a href="https://github.com/esp8266/arduino-esp8266fs-plugin">upload the sketch data </a>and the sketch itself!</b><br>

Then connect with a browser (from PC or mobile device) to <b>http://wasp.local</b> or type directly the ip address you set in the file <b>webserver.cpp</b>.<br><br>
<img src="https://hackster.imgix.net/uploads/attachments/676085/mobile-first_kqG8sL0qhC.png?auto=compress%2Cformat&w=1280&h=960&fit=max" alt="WASP" style="width:200px;height:300px;">

See the <a href="https://www.hackster.io/alejho/wasp-wemos-advanced-smart-plug-e4df23">project page on hackster.io</a> for wirings and more details!
