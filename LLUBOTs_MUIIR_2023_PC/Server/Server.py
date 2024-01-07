#librerías necesarias
from dash import Dash, dcc, html, Input, Output, State, ctx, callback
import paho.mqtt.client as mqtt
import dash_bootstrap_components as dbc


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("START")
    client.subscribe("STOP")

mensaje = ["mensaje1"]
mi_mensaje = 0
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
  mensaje.append(msg.payload.decode())
  print(msg.topic+" "+str(msg.payload))
  global mi_mensaje
  mi_mensaje = msg.payload


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

#IP del AWS de Jaime con el que vamos a trabajar
client.connect("51.20.185.180", 1883, 60)

client.loop_start()

#debemos conseguir los datos mediante mqtt
su_casa1 = 'verde'
su_ruta1 = '3'
casa_llegada1 = 'morada'

su_casa2 = 'blanco'
su_ruta2 = '8'
casa_llegada2 = 'blanco'

su_casa3 = 'morada'
su_ruta3 = '1'
casa_llegada3 = 'amarillo'

su_casa4 = 'rojo'
su_ruta4 = '7'
casa_llegada4 = 'rojo'

su_casa5 = 'amarillo'
su_ruta5 = '0'
casa_llegada5 = 'verde'

var_llegada1 = 'No ha llegado'

#utilizaremos el tema journal de bootstrap
app = Dash(__name__, external_stylesheets=[dbc.themes.JOURNAL])
# Estilos para las pestañas
tabs_styles = {
    'height': '84px',
    'align-items': 'center'
}

tab_style = {
    'padding': '15px',
    'border-radius': '15px',
    'background-color': '#ff928f',
    'width': '200px',
    'margin': '10px',
}

tab_selected_style = {
    'backgroundColor': '#b54f4c',
    'fontWeight': 'bold',
    'color': 'white',
    'padding': '15px',
    'margin': '10px',
    'border-radius': '10px',
    'width': '200px'
}

# Define tu layout
app.layout = dbc.Container([
    # Primera parte: el título
    dbc.Row([
        dbc.Col(html.H1("Visualizacion de los LLUBots",
               className='text-center text-primary p-4'),
                # wide=12
               )
    ]),

    # Segunda parte: la barra de navegación y el cuadrado a la derecha
    dbc.Row([
        # Contenedor para las pestañas con margen ajustado
        dbc.Col(
            dcc.Tabs(
                id="LluBots",
                value='pestaña-LluBot1',
                vertical=True,
                children=[
                    dcc.Tab(
                        label='General',
                        value='pestaña-general',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot1',
                        value='pestaña-LluBot1',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot2',
                        value='pestaña-LluBot2',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot3',
                        value='pestaña-LluBot3',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot4',
                        value='pestaña-LluBot4',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot5',
                        value='pestaña-LluBot5',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                ],
                style={'margin-top': '30px', 'margin-left': '50px'}  # Ajusta el margen superior
            ),
            width=2  # Ancho de las pestañas
        ),

        # Cuadrado a la derecha
        dbc.Col(
            id='contenido-pestaña',
            style={'width': '20px', 'height': '500px', 'margin': '20px 50px 30px 55px', 'background-color': '#eb6864', 'border-radius': '10px'},
        ),
    ]),
], fluid=True)

# Crea un dcc.Store para almacenar las variables
app.layout.children.append(dcc.Store(id='store', storage_type='memory'))

# Define un callback para actualizar el store con las variables globales
@app.callback(
    Output('store', 'data'),
    Input('LluBots', 'value')
)
def actualizar_store(tab):
    # Aquí actualiza las variables globales según sea necesario
    # Por ejemplo, si tienes variables globales su_casa1, su_ruta1, casa_llegada1, actualízalas aquí
    if tab == 'pestaña-LluBot1':
        su_casa1 = 'Valor nuevo para su_casa1'
        su_ruta1 = 'Valor nuevo para su_ruta1'
        casa_llegada1 = 'Valor nuevo para casa_llegada1'
    elif tab == 'pestaña-LluBot2':
        su_casa2 = 'Valor nuevo para su_casa2'
        su_ruta2 = 'Valor nuevo para su_ruta2'
        casa_llegada2 = 'Valor nuevo para casa_llegada2'
    elif tab == 'pestaña-LluBot3':
        su_casa3 = 'Valor nuevo para su_casa2'
        su_ruta3 = 'Valor nuevo para su_ruta2'
        casa_llegada3 = 'Valor nuevo para casa_llegada2'
    # Repite este patrón para las otras pestañas

    # Devuelve un diccionario con las variables que deseas almacenar
    return {
        'su_casa1': su_casa1,
        'su_ruta1': su_ruta1,
        'casa_llegada1': casa_llegada1,
        'su_casa2': su_casa2,
        'su_ruta2': su_ruta2,
        'casa_llegada1': casa_llegada2,
        'su_casa3': su_casa3,
        'su_ruta3': su_ruta3,
        'casa_llegada3': casa_llegada3
    }




@callback(Output('contenido-pestaña', 'children'),
          Input('LluBots', 'value'))
def render_content(tab):
  if tab == 'pestaña-LluBot1':
    return html.Div([
      html.Div([
        html.H3('Tiene que llegar a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '25%', 'color':'white'}),
        dcc.Markdown(id = 'markdown-su-casa1', children=f'{su_casa1}', style={'position': 'absolute', 'left': '30%', 'top': '32%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
        html.H3('Se encuentra en la ruta:', style={'position': 'absolute', 'left': '25%', 'top': '45%', 'color':'white'}),
        print(mi_mensaje),
        dcc.Markdown(id = 'markdown-su-ruta1', children=f'{su_ruta1}', style={'position': 'absolute', 'left': '30%', 'top': '52%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
        html.H3('A llegado a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '65%', 'color':'white'}),
        dcc.Markdown(id = 'markdown-casa1', children=f'{casa_llegada1}', style={'position': 'absolute', 'left': '30%', 'top': '72%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
        

        
      ]),         
    ])
  elif tab == 'pestaña-LluBot2':
      return html.Div([
        html.Div([
          html.H3('Tiene que llegar a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '25%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-casa2', children=f'{su_casa2}', style={'position': 'absolute', 'left': '30%', 'top': '32%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3('Se encuentra en la ruta:', style={'position': 'absolute', 'left': '25%', 'top': '45%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-ruta2', children=f'{su_ruta2}', style={'position': 'absolute', 'left': '30%', 'top': '52%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3('A llegado a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '65%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-casa2', children=f'{casa_llegada2}', style={'position': 'absolute', 'left': '30%', 'top': '72%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
        ]), 
      ])
  elif tab == 'pestaña-LluBot3':
      return html.Div([
        html.Div([
          html.H3('Tiene que llegar a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '25%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-casa3', children=f'{su_casa3}', style={'position': 'absolute', 'left': '30%', 'top': '32%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3('Se encuentra en la ruta:', style={'position': 'absolute', 'left': '25%', 'top': '45%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-ruta3', children=f'{su_ruta3}', style={'position': 'absolute', 'left': '30%', 'top': '52%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3('A llegado a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '65%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-casa3', children=f'{casa_llegada3}', style={'position': 'absolute', 'left': '30%', 'top': '72%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
        ]), 
      ])
  elif tab == 'pestaña-LluBot4':
      return html.Div([
        html.Div([
          html.H3('Tiene que llegar a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '25%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-casa4', children=f'{su_casa4}', style={'position': 'absolute', 'left': '30%', 'top': '32%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3('Se encuentra en la ruta:', style={'position': 'absolute', 'left': '25%', 'top': '45%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-ruta4', children=f'{su_ruta4}', style={'position': 'absolute', 'left': '30%', 'top': '52%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3('A llegado a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '65%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-casa4', children=f'{casa_llegada4}', style={'position': 'absolute', 'left': '30%', 'top': '72%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
        ]), 
      ])
  elif tab == 'pestaña-LluBot5':
      return html.Div([
        html.Div([
          html.H3('Tiene que llegar a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '25%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-casa5', children=f'{su_casa5}', style={'position': 'absolute', 'left': '30%', 'top': '32%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3('Se encuentra en la ruta:', style={'position': 'absolute', 'left': '25%', 'top': '45%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-ruta5', children=f'{su_ruta5}', style={'position': 'absolute', 'left': '30%', 'top': '52%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3('A llegado a la casa:', style={'position': 'absolute', 'left': '25%', 'top': '65%', 'color':'white'}),
          dcc.Markdown(id = 'markdown-casa5', children=f'{casa_llegada5}', style={'position': 'absolute', 'left': '30%', 'top': '72%','width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}),
          html.H3(id= 'llegada1', children = f'{var_llegada1}', style={'position': 'absolute', 'left': '55%', 'top': '65%', 'color':'white'}),
        ]), 
      ])



if __name__ == '__main__':
  # practica3.PitayaSetUp()
  app.run_server(debug=True,use_reloader=False)