import datetime

def custom_command():
    s = input('Enter custom command: ')
    return s

def current_time():
    now = datetime.datetime.now()

    hour = now.hour % 12
    if hour == 0: hour = 12

    minute = now.minute
    second = now.second

    time_str = chr(hour) + chr(minute) + chr(second)

    return time_str

def motor_up():
    return 'B51'

def motor_down():
    return 'B61'

def motor_left():
    return 'B71'

def motor_right():
    return 'B81'
