from dash import Dash, dcc, html, Input, Output, State,ctx, callback
import paho.mqtt.client as mqtt
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("START")
    client.subscribe("STOP")

mensaje = ["mensaje1"]

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
  mensaje.append(msg.payload.decode())
  print(msg.topic+" "+str(msg.payload))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)

client.loop_start()

app = Dash(__name__)

app.layout = html.Div(
  [
    dcc.Tabs([
      dcc.Tab(id="Tab1",label="Genera", children=[
      ]),
      dcc.Tab(label="LluBot1"),
      dcc.Tab(label="LluBot2"),
      dcc.Tab(label="LluBot3"),
      dcc.Tab(label="LluBot4"),
      dcc.Tab(label="LluBot5"),
    ]),
    dcc.Interval(id="Interval1", interval=1000,
                 n_intervals=0)    
  ]
)

@callback(
  [Output('Tab1', 'children')], 
  Input('Interval1', 'n_intervals'))
def update_data(n_intervals):
  
  return [str(mensaje)]


if __name__ == '__main__':
  # practica3.PitayaSetUp()
  app.run_server(debug=True,use_reloader=False)