#ifndef HTMLCODE_H
#define HTMLCODE_H

#define TOUCH_JSON\
	"\"data\": [\r\n"\
	"    {\r\n"\
	"\"dates\":\"2024-06-02 14:35:20\",\"\r\n"\
	"\"msg\":\"发送 sys(timerTask)<administrator> UDP 00000000003 FEFE6820382222640011110103901F003D16\"\r\n"\
	"    }\r\n"\
	"]\r\n"

#define HEAD_S\
	"HTTP/1.1 200 OK\r\n"\
	"Transfer-Encoding:chunked\r\n"\
	"Connection:keep-alive\r\n"\
	"Content-Type:text/html\r\n"\
	"\r\n"
// 网页 html
#define ERROR0 \
	"HTTP /1.1404 Not Found\r\n"\
	"Content-Type:text/plain\r\n"\
	"Content-Length:<20>\r\n"\
	"404 Not Found:404\r\n"\
	"\r\n"
// 网页 html
#define HTML01\
	"<!DOCTYPE html>\r\n"\
	"<html>\r\n"\
	"<head>\r\n"\
	"    <title>Example Page</title>\r\n"\
	"</head>\r\n"\
	"<body>\r\n"\
	"    <h1>Hello, World!</h1>\r\n"\
	"    <p>This is an example page of NO.1 .</p>\r\n"\
	"</body>\r\n"\
	"</html>"
// 网页 html
#define HTML02\
	"<!DOCTYPE html>\r\n"\
	"<html lang=\"zh\">\r\n"\
	"<head>\r\n"\
	"    <meta charset=\"gb2312\">\r\n"\
	"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"\
	"    <title>展示与获取测试</title>\r\n"\
	"</head>\r\n"\
	"<body>\r\n"\
	"    <div>\r\n"\
	"        <h1>展示与获取测试</h1>\r\n"\
	"        <p id=\"dataDisplay\"></p>\r\n"\
	"        <br><br>\r\n"\
	"        <button id=\"fetchButton\">获取数据</button>\r\n"\
	"    </div>\r\n"\
	"<script>\r\n"\
	"    document.getElementById('fetchButton').addEventListener('click', fetchData);\r\n"\
	"    function fetchData() {\r\n"\
	"        fetch('/api/data/isDataFlag')\r\n"\
	"            .then(response => {\r\n"\
	"                if (!response.ok) {\r\n"\
	"                    throw new Error('Failed to fetch data');\r\n"\
	"                }\r\n"\
	"                return response.text();\r\n"\
	"            })\r\n"\
	"            .then(data => {\r\n"\
	"                document.getElementById('dataDisplay').textContent = 'Data: ' + data;\r\n"\
	"            })\r\n"\
	"            .catch(error => {\r\n"\
	"                console.error('Error fetching data:', error);\r\n"\
	"            });\r\n"\
	"    }\r\n"\
	"</script>\r\n"\
	"</body>\r\n"\
	"</html>"
// 网页 html
#define HTML03\
	"<!DOCTYPE html>\r\n"\
	"<html lang=\"zh-CN\">\r\n"\
	"  <head>\r\n"\
	"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\r\n"\
	"    <title>配置界面</title>\r\n"\
	"    <style>\r\n"\
	"      *{\r\n"\
	"        margin: 0;\r\n"\
	"        padding: 0;\r\n"\
	"      }\r\n"\
	"      .tabs{\r\n"\
	"        display: flex;\r\n"\
	"        justify-content: space-between;\r\n"\
	"        border: 1px solid #ccc;\r\n"\
	"      }\r\n"\
	"      .tabs>div{\r\n"\
	"        cursor: pointer;\r\n"\
	"        padding: 10px 10px;\r\n"\
	"      }\r\n"\
	"      .container{\r\n"\
	"        padding: 10px 10px 10px 10px;\r\n"\
	"      }\r\n"\
	"      .container>div{\r\n"\
	"        border-bottom: 1px solid #ccc;\r\n"\
	"        padding-bottom: 5px;\r\n"\
	"      }\r\n"\
	"      .header {\r\n"\
	"          display: flex;\r\n"\
	"          align-items: center;\r\n"\
	"          margin-bottom: 10px;\r\n"\
	"      }\r\n"\
	"      .header span {\r\n"\
	"          font-size: 18px;\r\n"\
	"          margin-right: 10px;\r\n"\
	"      }\r\n"\
	"      .config-buttons button {\r\n"\
	"          padding: 5px 10px;\r\n"\
	"          font-size: 14px;\r\n"\
	"          margin-left: 10px;\r\n"\
	"      }\r\n"\
	"      .service-type {\r\n"\
	"          margin-bottom: 20px;\r\n"\
	"      }\r\n"\
	"      .service-type label {\r\n"\
	"          margin-right: 20px;\r\n"\
	"          font-size: 14px;\r\n"\
	"      }\r\n"\
	"      .server-config {\r\n"\
	"          display: flex;\r\n"\
	"          align-items: center;\r\n"\
	"          margin-bottom: 20px;\r\n"\
	"      }\r\n"\
	"      .server-config label {\r\n"\
	"          font-size: 14px;\r\n"\
	"          margin-right: 10px;\r\n"\
	"      }\r\n"\
	"      .baud-rate-config {\r\n"\
	"          display: flex;\r\n"\
	"          align-items: center;\r\n"\
	"          margin-bottom: 20px;\r\n"\
	"      }\r\n"\
	"      .baud-rate-config label {\r\n"\
	"          font-size: 14px;\r\n"\
	"          margin-right: 10px;\r\n"\
	"      }\r\n"\
	"      .baud-rate-config select {\r\n"\
	"          margin-right: 20px;\r\n"\
	"          padding: 5px;\r\n"\
	"          font-size: 14px;\r\n"\
	"      }\r\n"\
	"      .baud-rate-config button {\r\n"\
	"          padding: 5px 10px;\r\n"\
	"          font-size: 14px;\r\n"\
	"      }\r\n"\
	"      .communication-type {\r\n"\
	"          margin-bottom: 20px;\r\n"\
	"          font-size: 14px;\r\n"\
	"      }\r\n"\
	"      .communication-type label {\r\n"\
	"          margin-right: 20px;\r\n"\
	"      }\r\n"\
	"      .downlink-config {\r\n"\
	"          font-size: 14px;\r\n"\
	"      }\r\n"\
	"      .downlink-config label {\r\n"\
	"          margin-right: 20px;\r\n"\
	"      }\r\n"\
	"      .info{\r\n"\
	"        margin-top: 10px;\r\n"\
	"        border-bottom: none !important;\r\n"\
	"      }\r\n"\
	"      textarea{\r\n"\
	"        width: 100%;\r\n"\
	"        background: #fff;\r\n"\
	"      }\r\n"\
	"      .gateway{\r\n"\
	"        display: flex;\r\n"\
	"      }\r\n"\
	"      .gateway>span{\r\n"\
	"        display: block;\r\n"\
	"        width: 80px;\r\n"\
	"      }\r\n"\
	"      .gateway>input{\r\n"\
	"        border: none;\r\n"\
	"        outline: none;\r\n"\
	"        border: 1px solid #ccc;\r\n"\
	"        width: 70%;\r\n"\
	"        height: 30px;\r\n"\
	"        padding-left: 8px;\r\n"\
	"      }\r\n"\
	"      .btn{\r\n"\
	"        width: 20%;\r\n"\
	"        height: 30px;\r\n"\
	"        text-align: center;\r\n"\
	"        line-height: 30px;\r\n"\
	"        border: 1px solid #ccc;\r\n"\
	"        margin-top: 50px;\r\n"\
	"        padding-bottom: 0 !important;\r\n"\
	"        cursor: pointer;\r\n"\
	"        position: relative;\r\n"\
	"        left: 50%;\r\n"\
	"        transform: translateX(-50%);\r\n"\
	"      }\r\n"\
	"      select{\r\n"\
	"        border: 1px solid #ccc;\r\n"\
	"        padding: 5px 0;\r\n"\
	"      }\r\n"\
	"      .tabs>div.active {\r\n"\
	"        background-color: #ccc;\r\n"\
	"      }\r\n"\
	"    </style>\r\n"\
	"  </head>\r\n"\
	"  <body>\r\n"\
	"    <div class=\"tabs\">\r\n"\
	"      <div class=\"active\">基本信息</div>\r\n"\
	"      <div>修改网关标识</div>\r\n"\
	"      <div>修改服务</div>\r\n"\
	"    </div>\r\n"\
	"    <div class=\"container\" style=\"display: block;\">\r\n"\
	"      <div class=\"header\">\r\n"\
	"        <span>网关标识：</span>\r\n"\
	"        <span id=\"GatewayFlag\"></span>\r\n"\
	"      </div>\r\n"\
	"      <div class=\"service-type\">\r\n"\
	"        服务：\r\n"\
	"        <span id=\"WebType\"></span>\r\n"\
	"      </div>\r\n"\
	"      <div class=\"server-config\">\r\n"\
	"        <label for=\"server-address\">服务器地址：</label>\r\n"\
	"        <span id=\"WebServiceAddr\"></span>\r\n"\
	"      </div>\r\n"\
	"      <div class=\"server-config\">\r\n"\
	"        <label for=\"server-port\">服务器端口：</label>\r\n"\
	"        <span id=\"WebServicePort\"></span>\r\n"\
	"      </div>\r\n"\
	"      <div class=\"baud-rate-config\">\r\n"\
	"        <label for=\"mbus-baud-rate\">MBUS波特率：</label>\r\n"\
	"        <span id=\"MUBS_Baud\"></span>\r\n"\
	"        <label for=\"rs485-baud-rate\" style=\"margin-left: 50px;\">RS485波特率：</label>\r\n"\
	"        <span id=\"RS485_Baud\"></span>\r\n"\
	"      </div>\r\n"\
	"      <div class=\"communication-type\">\r\n"\
	"        <span>上行通讯：</span>\r\n"\
	"        <span id=\"Uplink\"></span>\r\n"\
	"      </div>\r\n"\
	"      <div class=\"downlink-config\">\r\n"\
	"        <span>下行通讯：</span>\r\n"\
	"        <span id=\"Downlink\"></span>\r\n"\
	"      </div>\r\n"\
	"      <div class=\"info\"></div>\r\n"\
	"      <textarea disabled cols=\"80\" rows=\"30\"></textarea>\r\n"\
	"    </div>\r\n"\
	"    <div class=\"container\" style=\"display: none;\">\r\n"\
	"      <div class=\"gateway\">\r\n"\
	"        <span>网关标识：</span>\r\n"\
	"        <input type=\"number\" id=\"numberInput\" maxlength=\"11\" oninput=\"enforceMaxLength(event)\" />\r\n"\
	"      </div>\r\n"\
	"      <div class=\"btn\" onclick=\"bnt1()\">确认</div>\r\n"\
	"    </div>\r\n"\
	"    <div class=\"container\" style=\"display: none;\">\r\n"\
	"      <div class=\"gateway\">\r\n"\
	"        <span>服务类型</span>\r\n"\
	"        <select name=\"\" id=\"\">\r\n"\
	"          <option value=\"1\">TCP服务</option>\r\n"\
	"          <option value=\"2\">UDP服务</option>\r\n"\
	"        </select>\r\n"\
	"      </div>\r\n"\
	"      <div class=\"gateway\" style=\"margin-top: 5px;\">\r\n"\
	"        <span>ip地址</span>\r\n"\
	"        <input type=\"text\" id=\"ipInput\" placeholder=\"输入IP地址\" />\r\n"\
	"      </div>\r\n"\
	"      <div class=\"gateway\" style=\"margin-top: 5px;\">\r\n"\
	"        <span>端口</span>\r\n"\
	"        <input type=\"text\" id=\"portInput\" placeholder=\"输入端口号\" />\r\n"\
	"      </div>\r\n"\
	"      <div class=\"btn\" onclick=\"validateInputs()\">确认</div>\r\n"\
	"    </div>\r\n"\
	"  </body>\r\n"\
	"  <script>\r\n"\
	"      function getList() {\r\n"\
	"          var xhr = new XMLHttpRequest();\r\n"\
	"          xhr.open('POST', 'http://192.168.4.1:8888/getWifiData', true);\r\n"\
	"          xhr.setRequestHeader('Content-Type', 'application/json;charset=UTF-8');\r\n"\
	"          xhr.onreadystatechange = function () {\r\n"\
	"              if (xhr.readyState === 4) {\r\n"\
	"                  if (xhr.status === 200) {\r\n"\
	"                    let GatewayFlag = document.getElementById('GatewayFlag')\r\n"\
	"                    let WebType = document.getElementById('WebType')\r\n"\
	"                    let WebServiceAddr = document.getElementById('WebServiceAddr')\r\n"\
	"                    let WebServicePort = document.getElementById('WebServicePort')\r\n"\
	"                    let MUBS_Baud = document.getElementById('MUBS_Baud')\r\n"\
	"                    let RS485_Baud = document.getElementById('RS485_Baud')\r\n"\
	"                    let Uplink = document.getElementById('Uplink')\r\n"\
	"                    let Downlink = document.getElementById('Downlink')\r\n"\
	"                    let res = JSON.parse(xhr.responseText)\r\n"\
	"                    GatewayFlag.innerText = res.GatewayFlag\r\n"\
	"                    WebType.innerText = res.WebType\r\n"\
	"                    WebServiceAddr.innerText = res.WebServiceAddr\r\n"\
	"                    WebServicePort.innerText = res.WebServicePort\r\n"\
	"                    MUBS_Baud.innerText = res.MUBS_Baud\r\n"\
	"                    RS485_Baud.innerText = res.RS485_Baud\r\n"\
	"                    Uplink.innerText = res.Uplink\r\n"\
	"                    Downlink.innerText = res.Downlink\r\n"\
	"                  } else {\r\n"\
	"                      // console.error('Error:', xhr);\r\n"\
	"                      // alert('Request failed: ' + xhr);\r\n"\
	"                  }\r\n"\
	"              }\r\n"\
	"          };\r\n"\
	"          var data = JSON.stringify({\r\n"\
	"              name: 'John Doe',\r\n"\
	"              email: 'john.doe@example.com'\r\n"\
	"          });\r\n"\
	"          xhr.send(data);\r\n"\
	"      }\r\n"\
	"      window.onload = function(){\r\n"\
	"        getList()\r\n"\
	"      }\r\n"\
	"      let tabs = document.querySelectorAll('.tabs>div')\r\n"\
	"      let containers = document.querySelectorAll('.container')\r\n"\
	"      tabs.forEach((tab, index) => {\r\n"\
	"      tab.addEventListener('click', () => {\r\n"\
	"          tabs.forEach(t => t.classList.remove('active'));\r\n"\
	"          containers.forEach(container => container.style.display = 'none');\r\n"\
	"          tab.classList.add('active');\r\n"\
	"          containers[index].style.display = 'block';\r\n"\
	"        });\r\n"\
	"      });\r\n"\
	"      const input1 = document.getElementById('numberInput')\r\n"\
	"      function enforceMaxLength(event) {\r\n"\
	"          const input = event.target;\r\n"\
	"          if (input.value.length > input.maxLength) {\r\n"\
	"              input.value = input.value.slice(0, input.maxLength);\r\n"\
	"          }\r\n"\
	"      }\r\n"\
	"      function saveGateway() {\r\n"\
	"          var xhr = new XMLHttpRequest();\r\n"\
	"          xhr.open('POST', 'http://192.168.4.1:8888/saveGateway', true);\r\n"\
	"          xhr.setRequestHeader('Content-Type', 'application/json;charset=UTF-8');\r\n"\
	"          var data = JSON.stringify({\r\n"\
	"            data:input1.value\r\n"\
	"          });\r\n"\
	"          xhr.send(data);\r\n"\
	"      }\r\n"\
	"      function bnt1(){\r\n"\
	"        if(input1.value.length === 11){\r\n"\
	"          saveGateway()\r\n"\
	"        }\r\n"\
	"      }\r\n"\
	"      function isValidIPAddress(ip) {\r\n"\
	"              const ipPattern = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;\r\n"\
	"              return ipPattern.test(ip);\r\n"\
	"      }\r\n"\
	"      function validateInputs() {\r\n"\
	"              const ipInput = document.getElementById('ipInput');\r\n"\
	"              const portInput = document.getElementById('portInput');\r\n"\
	"              const selectedOption = document.querySelector('select').value;\r\n"\
	"              const ipValue = ipInput.value.trim();\r\n"\
	"              const portValue = portInput.value.trim();\r\n"\
	"              if (ipValue === '' || portValue === '') {\r\n"\
	"                  alert('请输入IP地址和端口号');\r\n"\
	"                  return;\r\n"\
	"              }\r\n"\
	"              if (!isValidIPAddress(ipValue)) {\r\n"\
	"                  alert('无效的IP地址');\r\n"\
	"                  return;\r\n"\
	"              }\r\n"\
	"              if (isNaN(portValue) || portValue < 1 || portValue > 65535) {\r\n"\
	"                  alert('无效的端口号');\r\n"\
	"                  return;\r\n"\
	"              }\r\n"\
	"              const data = {\r\n"\
	"                  serviceType: selectedOption,\r\n"\
	"                  ipAddress: ipValue,\r\n"\
	"                  port: portValue\r\n"\
	"              };\r\n"\
	"              console.log(data);\r\n"\
	"              // You can proceed with your AJAX request or other actions here\r\n"\
	"          }\r\n"\
	"  </script>\r\n"\
	"</html>\r\n"
#endif
