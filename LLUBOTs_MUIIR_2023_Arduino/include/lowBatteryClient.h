extern int lowBatteryMock; 

void lowBatteryClient();

const char lowBatteryPage[] = R"=====(
HTTP/1.1 200 OK
Content-Type: text/html

<!DOCTYPE html>
<html lang='es'>
  <head>
    <title> LLUBot: Low Battery </title>
    <style>
        body { background-color: #7c05a3;
               font-family: Verdana;
               text-align: center;
               font-size: 50px;
               color: white }
    </style>
  </head>
  <body>
    <p> LOW BATTERY! </p>
    <br/>
    <p> Please turn the LLUBot off and charge it completely.</p>
  </body>
</html>
)=====";