#librerías necesarias
from dash import Dash, dcc, html, Input, Output, State, ctx, callback
import paho.mqtt.client as mqtt
import dash_bootstrap_components as dbc
from datetime import datetime
import json
import time
import sys

class LLUBot:
  _ID = 0
  #NFC / X
  NFCInfo = []
  #INFO / X
  SigLinea = []
  UltraSon = []
  Bateria = []
  #Modo / X
  Modo = []
  
  def unpackJson(self,json_str):
    info = json.loads(json_str)
    self.SigLinea.append(info["SigLinea"])
    self.UltraSon.append(info["UltraSon"])
    self.Bateria.append(info["Bateria"])
  
  def __init__(self, id):
    self._ID = id
  
  def clear(self):
    self.NFCInfo = []
    self.SigLinea = []
    self.UltraSon = []
    self.Modo = []
    self.Bateria = []

LLUBOTS = [LLUBot(1),LLUBot(2),LLUBot(3),LLUBot(4),LLUBot(5)]
# Suscribe el cliente a los puntos de info del llubot endpoints
def subscribe_ULLBOT(client,endPoint):
  client.subscribe("/LLUBots/NFC/" + endPoint)
  client.subscribe("/LLUBots/INFO/" + endPoint)
  client.subscribe("/LLUBots/MODO/" + endPoint)

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("START")
    client.subscribe("STOP")
    subscribe_ULLBOT(client,"1")
    subscribe_ULLBOT(client,"2")
    subscribe_ULLBOT(client,"3")
    subscribe_ULLBOT(client,"4")
    subscribe_ULLBOT(client,"5")

mensajes = []
mi_mensaje = 0
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
  new_msg = "[" + msg.topic + ":" + str(datetime.utcnow().strftime("%d/%m/%Y, %H:%M:%S")) + "]:"
  new_msg +=  msg.payload.decode()
  mensajes.append(new_msg)
  global mi_mensaje
  mi_mensaje = msg.payload.decode()


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

ipMQTT = "51.20.185.180"
port = 1883
if (len(sys.argv) >= 2):
  ipMQTT =  sys.argv[1]
if (len(sys.argv) >= 3):
  port = sys.argv[2]

#IP del AWS de Jaime con el que vamos a trabajar
client.connect(ipMQTT,port, 60)

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
app = Dash(__name__, external_stylesheets=[dbc.themes.JOURNAL],suppress_callback_exceptions=True)
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
                value='pestaña-general',
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
            style={'width': '20px', 'max-height' : '500px','height' : '500px','margin': '20px 50px 30px 55px', 'background-color': '#eb6864', 'border-radius': '10px'},
        ),
    ]),
    dcc.Interval(id="Interval1", interval=1000,
                 n_intervals=0)    
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

mensajes=[]


@callback(Output('contenido-pestaña', 'children'),
          Input('LluBots', 'value'))
def render_content(tab):
  style_rec={ 'width': '200px', 'height': '50px','text-align': 'center', 'padding': '10px', 'font-size': '20px', 'background-color':'#ffb5b3', 'color':'black'}
  if tab == 'pestaña-general':
    return dbc.Card(children=[
                            dbc.CardHeader(children=[
                              dbc.Row(children=[
                                html.H3(children=[
                                  "Información general LLUBOTS"
                                ],className="h3 d-flex justify-content-center",
                                  style={'color':'white'})
                              ])
                            ],style={'background-color':'inherit'},
                              class_name='border-0'),
                            dbc.CardBody(children=[
                              dbc.Row(children=[
                                dbc.Col(width=8),
                                dbc.Col(width=4,children=[
                                  dbc.Row(html.H4(children=["MENSAJES RECIBIDOS"],style={'color':'white'}),style={'background-color':'#b54f4c','padding': '10px'}),
                                  dbc.Row(html.Div(id="ZonaMensajes",
                                                   children=[],style={"height": "90%",'padding': '10px', 'overflow-y':'scroll',
                                                                                        'font-weight': 'bold','font-family': 'monospace','font-size': '15px',
                                                                                        'background-color':'#ffb5b3', 'color':'black'},
                                                  ),
                                          class_name='flex-grow-1 overflow-auto')
                                ], style={"height": "100%"},class_name="d-flex flex-column")
                              ],style={"height":"100%"})
                            ],className='overflow-hidden')
                          ],
                    style={"height":"100%",'background-color':'inherit'},
                    class_name='border-0')
  
  elif tab == 'pestaña-LluBot1':
    return html.Div([
        html.H3('Tiene que llegar a la casa:', style={'color':'white'}),
        dcc.Markdown(id = 'markdown-su-casa1', children=f'{su_casa1}', style=style_rec),
        html.H3('Se encuentra en la ruta:', style={ 'color':'white'}),
        print(mi_mensaje),
        dcc.Markdown(id = 'markdown-su-ruta1', children=f'{su_ruta1}', style=style_rec),
        html.H3('A llegado a la casa:', style={  'color':'white'}),
        dcc.Markdown(id = 'markdown-casa1', children=f'{casa_llegada1}', style=style_rec),
    ])
  elif tab == 'pestaña-LluBot2':
      return html.Div([
          html.H3('Tiene que llegar a la casa:', style={ 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-casa2', children=f'{su_casa2}', style=style_rec),
          html.H3('Se encuentra en la ruta:', style={ 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-ruta2', children=f'{su_ruta2}', style=style_rec),
          html.H3('A llegado a la casa:', style={  'color':'white'}),
          dcc.Markdown(id = 'markdown-casa2', children=f'{casa_llegada2}', style=style_rec),
        ])
  elif tab == 'pestaña-LluBot3':
      return html.Div([
          html.H3('Tiene que llegar a la casa:', style={ 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-casa3', children=f'{su_casa3}', style=style_rec),
          html.H3('Se encuentra en la ruta:', style={ 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-ruta3', children=f'{su_ruta3}', style=style_rec),
          html.H3('A llegado a la casa:', style={  'color':'white'}),
          dcc.Markdown(id = 'markdown-casa3', children=f'{casa_llegada3}', style=style_rec),
        ])
  elif tab == 'pestaña-LluBot4':
      return html.Div([
          html.H3('Tiene que llegar a la casa:', style={ 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-casa4', children=f'{su_casa4}', style=style_rec),
          html.H3('Se encuentra en la ruta:', style={ 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-ruta4', children=f'{su_ruta4}', style=style_rec),
          html.H3('A llegado a la casa:', style={  'color':'white'}),
          dcc.Markdown(id = 'markdown-casa4', children=f'{casa_llegada4}', style=style_rec),
        ])
  elif tab == 'pestaña-LluBot5':
      return html.Div([
          html.H3('Tiene que llegar a la casa:', style={ 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-casa5', children=f'{su_casa5}', style=style_rec),
          html.H3('Se encuentra en la ruta:', style={ 'color':'white'}),
          dcc.Markdown(id = 'markdown-su-ruta5', children=f'{su_ruta5}', style=style_rec),
          html.H3('A llegado a la casa:', style={  'color':'white'}),
          dcc.Markdown(id = 'markdown-casa5', children=f'{casa_llegada5}', style=style_rec),
          html.H3(id= 'llegada1', children = f'{var_llegada1}', style={ 'left': '55%', 'top': '65%', 'color':'white'}),
        ])

@callback(
  [Output('ZonaMensajes', 'children')], 
  Input('Interval1', 'n_intervals'),
  State('ZonaMensajes','children'),
  prevent_initial_call=True)
def update_data(n_intervals,children):
  for msg in mensajes:
    children.append(html.P(children=msg))
  if len(mensajes) != 0:
    mensajes.clear()
  return [children]

if __name__ == '__main__':
  # practica3.PitayaSetUp()
  app.run_server(debug=True,use_reloader=False)