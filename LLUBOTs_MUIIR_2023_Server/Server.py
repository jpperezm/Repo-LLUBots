#librerías necesarias
from dash import Dash, dcc, html, Input, Output, State, MATCH,ALL,no_update ,ctx, callback
import paho.mqtt.client as mqtt
import dash_bootstrap_components as dbc
from datetime import datetime
import json
import time
import sys

roundabout = 0

class LLUBot:

  def getHouse(self,number):
    if (number == "Blanca"):
      return 1
    if (number == "Violeta"):
      return 2
    if (number == "Azul"):
      return 3
    if (number == "Verde"):
      return 4
    if (number == "Roja"):
      return 5
  
  def unpackJson(self,info):
    global roundabout
    self.SigLineaList.append([info["leftLineFollower"],info["rightLineFollower"]])
    self.LastSigLinea = [info["leftLineFollower"],info["rightLineFollower"]]
    self.UltraSonList.append(info["ultrasonic"])
    self.LastUS = info["ultrasonic"]
    self.NFCInfoList.append(info["NFC"])
    self.LastNFC = info["NFC"]
    self.ModoList.append(info["currentState"])
    self.LastModo = info["currentState"]
    self.Ruta = info["initialStreetName"]
    self.Casa = info["homeName"]
    self.RutaDestino = info["goalStreetName"]
    roundabout = info["numberOfLLUBotsOnRoundabout"]
  
  def __init__(self, id):
    self._ID = id
    #NFC / X
    self.NFCInfoList = []
    self.LastNFC = ""
    #INFO / X
    self.SigLineaList = []
    self.LastSigLinea = ""
    self.UltraSonList = []
    self.LastUS = ""
    #Modo / X
    self.ModoList = []
    self.LastModo = ""

    self.Ruta = ""
    self.RutaDestino = ""
    self.Casa = ""
  
  def clear(self):
    self.NFCInfoList = []
    self.SigLineaList = []
    self.UltraSonList = []
    self.ModoList = []
    self.BateriaList = []
    self.LastSigLinea = ""
    self.LastUS = ""
    self.LastModo = ""
    self.Ruta = ""
    self.RutaDestino = ""
    self.Casa = ""

LLUBOTS = {}
LLUBOTS_ID = []
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
    client.subscribe("/LLUBot/info")



mensajes = []
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
  new_msg = "[" + msg.topic + ":" + str(datetime.utcnow().strftime("%d/%m/%Y, %H:%M:%S")) + "]:"
  new_msg +=  msg.payload.decode()
  mensajes.append(new_msg)
  splitTopic = msg.topic.split("/")
  if (len(splitTopic) > 2 and splitTopic[1] == "LLUBot"):
    if (splitTopic[2] == "info"):
      info = json.loads(msg.payload.decode())
      if (not (info["LLUBotID"] in LLUBOTS_ID)):
        LLUBOTS_ID.append(info["LLUBotID"])
        LLUBOTS[info["LLUBotID"]] = (LLUBot(info["LLUBotID"]))
        subscribe_ULLBOT(client,info["LLUBotID"])
      LLUBOTS[info["LLUBotID"]].unpackJson(info)
  if (len(splitTopic) > 3):
    if (splitTopic[2] == "NFC"):
      LLUBOTS[splitTopic[3]].NFCInfoList.append(msg.payload.decode())
      LLUBOTS[splitTopic[3]].LastNFC = msg.payload.decode()
      
    if (splitTopic[2] == "MODO"):
      LLUBOTS[splitTopic[3]].ModoList.append(msg.payload.decode())
      LLUBOTS[splitTopic[3]].LastModo = msg.payload.decode()



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
                        label='LluBot Blanco',
                        value='pestaña-LluBot-Blanco',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot Violeta',
                        value='pestaña-LluBot-Violeta',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot Azul',
                        value='pestaña-LluBot-Azul',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot Verde',
                        value='pestaña-LluBot-Verde',
                        style=tab_style,
                        selected_style=tab_selected_style
                    ),
                    dcc.Tab(
                        label='LluBot Rojo',
                        value='pestaña-LluBot-Rojo',
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
                 n_intervals=0),
], fluid=True)

# Crea un dcc.Store para almacenar las variables
app.layout.children.append(dcc.Store(id='store', storage_type='memory'))

# Define un callback para actualizar el store con las variables globales
# @app.callback(
#     Output('store', 'data')
#     Input('LluBots', 'value')
# )
# def actualizar_store(tab):
#     # Aquí actualiza las variables globales según sea necesario
#     # Por ejemplo, si tienes variables globales su_casa1, su_ruta1, casa_llegada1, actualízalas aquí

#     # Devuelve un diccionario con las variables que deseas almacenar
#     return {

#     }

mensajes=[]

def createDataColumn(data,id_data,widthW,title):
  return dbc.Col(width=widthW,children=[
    dbc.Row(html.H4(children=[title],style={'color':'white'}),style={'background-color':'#b54f4c','padding': '10px','text-align': 'center'}),
    dbc.Row(html.Div(id=id_data,
                      children=data,style={"height": "90%",'padding': '10px', 'overflow-y':'scroll',
                                                          'font-weight': 'bold','font-family': 'monospace','font-size': '13px',
                                                          'background-color':'#ffb5b3', 'color':'black'},
                    ),
            class_name='flex-grow-1 overflow-auto')
  ], style={"height": "100%"},class_name="d-flex flex-column")

def createLLUbotTab(id, data):
  style_rec={ 'text-align': 'center', 'border-radius':'10px','padding': '10px 0px 1px 0px', 'font-size': '20px',
             'font-width':'bold', 'background-color':'#ffb5b3', 'color':'black'}
  camino = LLUBOTS[id].RutaDestino if LLUBOTS[id].RutaDestino != "" else "NOT SENT"
  casaEncontrada = LLUBOTS[id].LastNFC if LLUBOTS[id].LastNFC != "" else "NOT SENT"
  rutaActual = LLUBOTS[id].Casa if LLUBOTS[id].Casa != "" else "NOT SENT"
  modo = LLUBOTS[id].LastModo if LLUBOTS[id].LastModo != "" else "NOT SENT"
  
  history = {'US_' + id:[],'SigueLinea_' + id:[],}
  if data != None:
    if ('US_' + id) in data:
      history['US_' + id] =  [x for xs in data['US_' + id]for x in xs]
    if ('SigueLinea_' + id) in data:
      history['SigueLinea_' + id] =[x for xs in data['SigueLinea_' + id]for x in xs] 
  
  return [dbc.Card([dbc.CardBody( [
    dbc.Row([
      dbc.Col([
        dbc.Row([
          dbc.Row(dbc.Col(html.H4('Tiene que llegar a la casa:', style={'color':'white'}),width=6)),
          dbc.Row(dbc.Col(dcc.Markdown(id = {'type':'markdown-su-casa', "index":id} , children=f'{LLUBOTS[id]._ID}', style=style_rec),width=6))
        ]),
        dbc.Row([
          dbc.Row(dbc.Col(html.H4('Tiene que ir por la ruta:', style={ 'color':'white'}),width=6)),
          dbc.Row(dbc.Col(dcc.Markdown(id = {'type': 'markdown-su-ruta', "index":id}, children=f'{camino}', style=style_rec),width=6))
        ]),
        dbc.Row([
          dbc.Row(dbc.Col(html.H4('Ha llegado a la casa:', style={  'color':'white'}),width=6)),
          dbc.Row(dbc.Col(dcc.Markdown(id = {'type':'markdown-casa',"index":id} , children=f'{casaEncontrada}', style=style_rec),width=6))
        ]),
        dbc.Row([
          dbc.Row(dbc.Col(html.H4('Se encuentra en la ruta:', style={'color':'white'}),width=6)),
          dbc.Row(dbc.Col(dcc.Markdown(id = {'type':'markdown-ruta',"index":id}, children=f'{rutaActual}', style=style_rec),width=6))
        ]),
        dbc.Row([
          dbc.Row(dbc.Col(html.H4('Modo', style={'color':'white'}),width=6)),
          dbc.Row(dbc.Col(dcc.Markdown(id = {'type':'markdown-modo',"index":id}, children=f'{modo}', style=style_rec),width=6))
        ]),
      ],width=6,class_name="d-flex flex-column align-self-center"),
      dbc.Col([
        dbc.Row(html.H4('Datos', style={'color':'white','text-align': 'center'})),
        dbc.Row([
          createDataColumn(history['US_' + id],{'type': "UltraSonido", 'index' : id},4,"UltraSonido"),
          createDataColumn(history['SigueLinea_' + id],{'type': "SigueLinea", 'index' : id},4,"SigueLinea"),
          ],style={"height":"100%"},class_name="d-flex")
      ],width=6,style={"max-height":"100%"})
    ],style={"height":"100%"})
  ],className='overflow-hidden')
  ],style={"background-color": "inherit", "border-color": "rgba(255, 0, 0, 0)",'height':'100%'}),
        dcc.Interval(id={'type' : 'Interval', 'index': id}, interval=1000,n_intervals=0),]
  

@callback(Output('contenido-pestaña', 'children'),
          Input('LluBots', 'value'),
          State('store','data'))
def render_content(tab,storedMsg):
  child_history = []
  if storedMsg != None and storedMsg['history'] != None:
    child_history = storedMsg['history']
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
                                dbc.Col([
                                    dbc.Input(id='mqtt-topic-input', placeholder='Introduce el topic...', type='text', className='mb-2'),
                                    dbc.Input(id='mqtt-input', placeholder='Introduce un mensaje...', type='text'),
                                    dbc.Button('OK', id='mqtt-publish-button', color='primary', className='mt-2'),
                                    html.Div(id='mqtt-publish-output')
                                ]),
                                dbc.Col(width=2),
                                createDataColumn(child_history,"ZonaMensajes",4,"Mensajes Recibidos"),
                              ],style={"height":"100%"})
                            ],className='overflow-hidden')
                          ],
                    style={"height":"100%",'background-color':'inherit'},
                    class_name='border-0')
  elif tab[0:15] == 'pestaña-LluBot-':
    if (tab[15:] in LLUBOTS_ID):
      return createLLUbotTab(tab[15:],storedMsg)
    else:
      return "NOT STARTED"


@callback(
    Output('mqtt-publish-output', 'children'),
    Input('mqtt-publish-button', 'n_clicks'),
    State('mqtt-topic-input', 'value'),
    State('mqtt-input', 'value'),
    prevent_initial_call=True
)
def publish_mqtt_message(n_clicks, topic, message):
    if message and topic:
        client.publish(topic, message)
        return f'Mensaje "{message}" publicado en {topic}'
    return 'Por favor introduce un topic y un mensaje para publicar'



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

@callback(
  [Output({'type':'UltraSonido', 'index': MATCH},'children'),
   Output({'type':'SigueLinea', 'index': MATCH},'children'),
   Output({'type':'markdown-su-casa', "index":MATCH},'children'),
   Output({'type':'markdown-su-ruta', "index":MATCH},'children'),
   Output({'type':'markdown-casa', "index":MATCH},'children'),
   Output({'type':'markdown-ruta', "index":MATCH},'children'),
   Output({'type':'markdown-modo', "index":MATCH},'children')],
  Input({'type':'Interval', 'index': MATCH},'n_intervals'),
  State({'type':'UltraSonido', 'index': MATCH},'children'),
  State({'type':'SigueLinea', 'index': MATCH},'children'),
)
def checkWOrth(n_intervals,UltraSonido,SigLin):
  trigger = ctx.triggered_id
  if not trigger:
    return no_update
  
  index_number = trigger["index"]
  
  def writeInfo(list,objChildren):
    for msg in list:
      strMsg = "[" + str(datetime.utcnow().strftime("%d/%m/%Y, %H:%M:%S")) +"]:" + str(msg)
      objChildren.append(html.P(children=strMsg))

    list.clear()
  
  writeInfo(LLUBOTS[index_number ].UltraSonList,UltraSonido)
  writeInfo(LLUBOTS[index_number ].SigLineaList,SigLin)
  return [UltraSonido,SigLin,str(LLUBOTS[index_number]._ID),str(LLUBOTS[index_number].RutaDestino),
          str(LLUBOTS[index_number].LastNFC),str(LLUBOTS[index_number].Casa),str(LLUBOTS[index_number].LastModo)]

@callback(
   Output('store', 'data',allow_duplicate=True),
   Input('ZonaMensajes','children'),
   State('store', 'data'),
   prevent_initial_call=True)
def update_store(children,data):
  if data == None:
    return {'history' : children}

  data['history'] = children
  return data

@callback(
   Output('store', 'data',allow_duplicate=True),
   Input({'type':'UltraSonido', 'index': ALL},'children'),
   Input({'type':'SigueLinea', 'index': ALL},'children'),
   State('store', 'data'),
   prevent_initial_call=True)
def update_store(childrenUS,childrenSL,data):
  trigger = ctx.triggered_id
  if trigger:
      if data == None:
        return {'US_' + trigger['index']  : childrenUS,
                'SigueLinea_' + trigger['index']  : childrenSL,}

      data['US_' + trigger['index']] = childrenUS
      data['SigueLinea_' + trigger['index']] = childrenSL
      return data
  return no_update

  
if __name__ == '__main__':
  # practica3.PitayaSetUp()
  app.run_server(debug=True,use_reloader=False)