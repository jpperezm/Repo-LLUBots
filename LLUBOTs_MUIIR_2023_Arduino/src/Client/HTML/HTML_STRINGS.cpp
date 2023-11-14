const char MAIN_MENU_HTML[] = R"======(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LLUBot</title>
</head>
<body>
    <div class='main'>
        <h1>LLUBot</h1>

        <div class='button'>
            <a href="/A1">
                <button class='buttonA1'>Roomba</button>
            </a>

            <a href="/A2">
                <button class='buttonA2'>Actividad: Chess</button>
            </a>
        </div>

        <!-- Additional content goes here -->

        <p>Welcome to the Nether</p>
    </div>
</body>
</html>
)======";

const char CSS[] = R"======(
    <style>   
      body {background-color: #7c05a3; text-align: center; color: white; font-family: Verdana}
        .main button {  height: 150px; width: 80%; font-size: 25px; }
        .num {padding: 30px; text-decoration: none; display: inline-block; width: 60px; font-size: 25px}
        .button {background-color: #7c05a3; border-radius: 12px; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 40px}
      h1 {font-size: 50px;}
      a {color: white; text-decoration: none}
      body {font-size: 30px; font-family: Verdana}
    </style>
)======"; 