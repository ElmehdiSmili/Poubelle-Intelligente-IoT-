import paho.mqtt.client as mqtt
import requests

# MQTT broker information
broker_address = "192.168.137.74"
broker_port = 1883
mqtt_topic = "valeur"

# ThingSpeak information
thingspeak_api_key = "VDFR9VIHHS40776F"
thingspeak_url = "https://api.thingspeak.com/update"

# Callback function for MQTT connection
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker")
        # Subscribe to the MQTT topic
        client.subscribe(mqtt_topic)
    else:
        print("Failed to connect to MQTT broker")

def on_message(client, userdata, msg):
    original_message = float(msg.payload.decode())
    modified_message = (40 - original_message)*(5/2)
    print("Received message: " + str(original_message))
    print("Modified message: " + str(modified_message))

    url = f"{thingspeak_url}?api_key={thingspeak_api_key}&field1={modified_message}"
    # Send the GET request
    response = requests.get(url)

    if response.status_code == 200:
        print("Data sent to ThingSpeak successfully")
    else:
        print(response.content)

# Create MQTT client instance
client = mqtt.Client()

# Set callback functions
client.on_connect = on_connect
client.on_message = on_message

# Connect to MQTT broker
client.connect(broker_address, broker_port, 60)

# Start MQTT client loop
client.loop_start()

# Keep the script running to receive messages
# Keep the script running to receive messages
while True:
    pass
