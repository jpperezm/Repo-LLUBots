void roombaGame();


// roomba page
const char roombaPage1[] = R"=====(
HTTP/1.1 200 OK
Content-Type: text/html

<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <title> LLUBot: Roomba </title>
    <style>
      body {background-color: #7c05a3; text-align: center; color: white; font-family: Verdana;}
        .main button {  height: 50px; width: 50%; font-size: 25px; }
        .buttonRun {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}
        .num {padding: 30px; text-decoration: none; display: inline-block; width: 60px; font-size: 25px}
        .buttonGo {background-color: #7c05a3; border-radius: 12px; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 40px}
        .menu { font-size: 25px; }
        .app1 button { font-size: 25px; }
      h1 {font-size: 50px;}
      a {color: white; text-decoration: none}
      body {font-size: 30px;}
    </style>
  </head>
  <body>
    <div class='main'>
      <a href="/BACK">
        <button class= 'menu'> Main Menu </button>
      </a>
      <br></br>
      <h1> LLUBot Roomba </h1>
      <div class= 'buttonRun'>
        Run <input class='num' type='text' id='input1'/> veces
        <br></br>
        <a id='a1' href='' color: white>
          <button class= 'buttonGo'> Go! </button>
        </a>
    </div>
    <br></br>
    <div>
)=====";


const char roombaPage2[] = R"=====(
      <br></br>
      <p> Pulsar el siguiente bot&oacuten para ver la explicaci&oacuten del Roomba.</p>
      <button id='btn1'> Roomba: Explicaci&oacuten </button><br><br>
      <img src='https://luisanalarab.files.wordpress.com/2020/07/cuadrc3adcula-mal-hecha.png?w=1024' width='700' height='700'>
    </div>
    
    <div class='app1'>
      <h1> Roomba </h1>
        <h2> Thoughts:  </h2>
          <p> Mobile robotics. </p>
          <img src='https://www.pequeocio.com/wp-content/uploads/2019/02/perimetro-circunferencia.jpg'>
          <p> . </p>
          <p> ? </p>
        <h2> Code: </h2> 
          <p> . </p>
          <img src='https://media.monolithicpower.com/wysiwyg/1_11.png'>
        <button class='back'> Salir de la explicaci&oacuten. </button>
    </div>
    <script>
      let main = document.querySelector('.main');
      let app1 = document.querySelector('.app1');
      let btnBack = document.querySelectorAll('.back');
      btn1.addEventListener('click', () => {
              main.style.display = 'none';
              app1.style.display = 'block';
      });
      btnBack.forEach( btn => {
              btn.addEventListener('click', () => {
                      main.style.display = 'block';
                      app1.style.display = 'none';
              });
      });
      let input1 = document.getElementById('input1');
      let string = '';
      input1.addEventListener('input', () => string = "/ROOMBA-" + input1.value );
      let link1 = document.getElementById('a1');
      link1.addEventListener('click', () => link1.href = string);
    </script>
  </body>
</html>
)=====";