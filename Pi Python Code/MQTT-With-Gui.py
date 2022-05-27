import tkinter as tk
import paho.mqtt.client as mqtt

# Create the window

window = tk.Tk()
frame = tk.Frame(master=window, width=1000, height=1000)

# Change the colour of the squares


def colourlabelGreen():
    tk.Label(master=frame, bg="green", width=20,
             height=5).grid(column=1, row=0)


def movementLabelGreen():
    tk.Label(master=frame, bg="green", width=20,
             height=5).grid(column=0, row=0)


def colourlabelGrey():
    tk.Label(master=frame, bg="grey", width=20,
             height=5).grid(column=1, row=0)


def movementlabelGrey():
    tk.Label(master=frame, bg="grey", width=20,
             height=5).grid(column=0, row=0)


def on_connect(client, userdata, flags, rc):

    client.subscribe("photonLog")

# Receive the message from the argon


def on_message(client, userdata, message):
    if message.payload.decode("utf-8") == "Movement&Light":
        print("Movement and Light")
        colourlabelGreen()
        movementLabelGreen()

    if message.payload.decode("utf-8") == "Movement":
        print("Movement")
        colourlabelGrey()
        movementLabelGreen()

    if message.payload.decode("utf-8") == "Movement&LightOff":
        print("Movement and Light Off")
        colourlabelGrey()
        movementlabelGrey()

# Creating the client part of the application


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("test.mosquitto.org", 1883, 60)

# Starting the client loop

client.loop_start()
frame.pack()

# Creating the text of the application

tk.Label(master=frame, text="Movement").grid(column=0, row=1)
tk.Label(master=frame, text="Lights On/Off").grid(column=1, row=1)

# Starting the window loop

window.mainloop()
