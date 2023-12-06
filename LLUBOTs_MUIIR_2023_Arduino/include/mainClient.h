extern int selectedActivity;
extern int nextActivity;

void mainMenu();
void showTrace(WiFiClient& client);


const char mainPage[] = R"=====(
HTTP/1.1 200 OK
Content-Type: text/html

<!DOCTYPE html>
<html>
  <head>
    <title> LLUBot </title>
    <style>
      body {background-color: #7c05a3; text-align: center; color: white; font-family: Verdana}
        .main button {  height: 150px; width: 80%; font-size: 25px; }
        .num {padding: 30px; text-decoration: none; display: inline-block; width: 60px; font-size: 25px}
        .button {background-color: #7c05a3; border-radius: 12px; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 40px}
      h1 {font-size: 50px;}
      a {color: white; text-decoration: none}
      body {font-size: 30px; font-family: Verdana}
    </style>
  </head>

  <body>
    <div class='main'>
      <h1> LLUBot </h1>
      <div class= 'button'>
      <a href="/A1">
        <button class= 'buttonA1'>
          Roomba 
        </button>
      </a>
        <br></br>
        <a href="/A2">
          <button class= 'buttonA2'> Actividad: Chess  </button>
        </a>
      </div>
      <br></br>
      <br></br>
      <br></br>
      <br></br>
      <br></br>
      <br></br>
      <br></br>
      <br></br>
      <p> Welcome to the Nether </p>
    </div>
  </body>
</html>
)=====";