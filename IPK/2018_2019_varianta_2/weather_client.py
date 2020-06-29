#!/usr/bin/env python3.6

# FIT VUT Brno
# IPK 1. project 2018/2019
# author: xsedla1e

import socket
import json
import sys

# Size of chunk for socket
CHUNK = 1024
# Constant for conversion
KMH = 3.6
# Constant for conversion
KELVIN = 273.15
# Address of wheather server
URL = "api.openweathermap.org"
# Port for listening
PORT = 80


def ms_to_kmh(value):
    """Converts from m/s to km/h"""
    return value * KMH


def kelvin_to_celsius(value):
    """Converts from Kelvin to Celsius"""
    return value - KELVIN


def get_http_code(data):
    """Function gets HTTP code from received data"""
    try:
        code = int(data.split(" ")[1])
    except Exception:
        print_error("Internal error. Please try again")
    return code


def http_code_check(code):
    """Switch for the most common codes
    Which can occur, user possibly will know what to do
    """

    if code == 400:
        print_error("Bad request")
    elif code == 401:
        print_error("Invalid API key")
    elif code == 404:
        print_error("City not found")
    elif code != 200:
        print_error(f"Something went wrong.\nHTTP status-code: {code}")


def get_http_body(city, api_key):
    """Create a socket with stream to listen

    Specific request is sent to the server
    Listening in loop, because we don't have to receive data in one chunk
    Socket is properly closed after receiving all data

    Function returns data in UTF-8 encoding
    """

    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((URL, PORT))
        s.sendall(bytes(f'GET /data/2.5/weather?q={city}&APPID={api_key}'
                         ' HTTP/1.0\r\n\r\n', 'utf-8'))
        data = b''
        while True:
            tmp = s.recv(CHUNK)
            if not tmp:
                break
            data += tmp
        s.shutdown(socket.SHUT_RDWR)
        s.close()
    except Exception:
        print_error("Error during socket workflow")
    return str(data, 'utf-8')


def format_output(data):
    """Formating output to stdin in specific format, should be easy to read"""
    json_data = json.loads(data)
    city = json_data['name']
    country = json_data['sys']['country']
    weather = json_data['weather'][0]['main']
    temp = kelvin_to_celsius(json_data['main']['temp'])
    humidity = json_data['main']['humidity']
    pressure = json_data['main']['pressure']
    wind_speed = ms_to_kmh(json_data['wind']['speed'])

    try:
        wind_deg = json_data['wind']['deg']
    except KeyError:
        wind_deg = "None"

    print(f"City: {city}, {country}\n"
          f"Weather: {weather}\n"
          f"Temperature: {temp:.2f} °C\n"
          f"Humidity: {humidity}%\n"
          f"Pressure: {pressure} hPa\n"
          f"Wind speed: {wind_speed:.2f} km/h\n"
          f"Wind degree: {wind_deg}")


def print_error(msg):
    """Print msg on STDERR and kill the program with return code"""
    print(msg, file=sys.stderr)
    sys.exit(1)


def check_arguments(api_key, city):
    """If user forgot insert API_KEY or city, it will warn an user"""
    if not api_key:
        print_error("API key is blank")
    if not city:
        print_error("City is blank")


def get_arguments():
    """Try to get arguments

    Function returns tuple with API_KEY and city
    """

    try:
        api_key = sys.argv[1]
        city = sys.argv[2].lower()
    except Exception:
        print_error("Try again, bad arguments")
    return api_key, city


def main():
    """Main function where all the magic is going on"""
    api_key, city = get_arguments()
    check_arguments(api_key, city)
    data = get_http_body(city, api_key)
    data = data.split(r"POST")
    code = get_http_code(data[0])
    http_code_check(code)
    json_data = data[1].strip()
    format_output(json_data)

if __name__ == '__main__':
    """Python purpose"""
    main()
