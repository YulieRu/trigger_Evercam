from tkinter import *
from time import sleep
import serial
import pyautogui

serialTrigger = None

def send_command():
    num_iter = num_iter_box.get(1.0, 'end-1c')
    pause_ms = delay_pause_box.get(1.0, 'end-1c')

    #"[RECORD=10 5000]"
    command = '[RECORD=' + num_iter + ' ' +\
                         pause_ms + ']'

    print(command)
    #serialTrigger = open_port()
    if not serialTrigger:
        lblOpenPrt["text"] = f'Not connected'
        return 0
    serialTrigger.write(command.encode())
    sleep(int(pause_ms) // 1000)
    #pyautogui.press("Enter")
    lbl["text"] = "Command was sent."

def readSerial():
    response = serialTrigger.readline()
    response = response.decode('utf-8')
    print(response)

def open_port():
    global serialTrigger
    num_port = numCOMport_box.get(1.0, 'end-1c')
    baud = 115200
    try:
        serialTrigger = serial.Serial(port=num_port,
                                      baudrate=baud,
                                      timeout=3,
                                      writeTimeout=1,
                                      parity=serial.PARITY_NONE,
                                      stopbits=serial.STOPBITS_ONE)
        lblOpenPrt["text"] = f'Connected'
        print(f'{num_port} is connected')
    except serial.serialutil.SerialException:
        if not serialTrigger:
            lblOpenPrt["text"] = f'Not connected'


win = Tk()
win.title('Command')
win.geometry("600x200")

# Creating a text box widget

label_num_iter = Label(win, text='Number of iterations')
label_num_iter.grid(column=0, row=1)
num_iter_box = Text(win, height=1, width=40)
num_iter_box.insert(1.0, 5)
num_iter_box.grid(column=1, row=1)

label_pause = Label(win, text='Pause between iterations (ms)')
label_pause.grid(column=0, row=2)
delay_pause_box = Text(win, height=1, width=40)
delay_pause_box.insert(1.0, 1000)
delay_pause_box.grid(column=1, row=2)

label_numCOMport = Label(win, text='COMport number in system')
label_numCOMport.grid(column=0, row=5)
numCOMport_box = Text(win, height=1, width=40)
numCOMport_box.insert(1.0, 'COM5')
numCOMport_box.grid(column=1, row=5)

#open_btn = Button(win, text="Open Text File", command=open_text)

# Create a button to save the text
comBut = Button(win, text="Send command", command=send_command)
comBut.grid(column=0, row=6, pady=10, padx=10)
lbl = Label(win, text="No command received yet.")
lbl.grid(column=0, row=7, pady=10, padx=10)

portButton = Button(win, text='COMport', command=open_port)
portButton.grid(column=1, row=6, pady=10, padx=10)
lblOpenPrt = Label(win, text="No COMport")
lblOpenPrt.grid(column=1, row=7, pady=10, padx=10)

win.mainloop()