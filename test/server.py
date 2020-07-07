from flask import Flask, abort, redirect, url_for, jsonify, request, send_file
from werkzeug.utils import secure_filename
from dataclasses import dataclass
import json
import base64
import os


@dataclass
class Firmware:
    ''' Class for keeping firmware '''
    name: str
    version: int
    filename: str
    data: bytes = None

    def return_data(self) -> (bytes, Exception):
        if self.data is None:
            try:
                self.data = open(FIRMWARE_DIR + self.filename, 'rb').read()
            except Exception as e:
                return (None, e)
        return (self.data, None)

    def to_json(self):
        return json.dumps(self, default=lambda o: o.__dict__)

    def to_map(self):
        return {"name": self.name, "version": self.version, "data": self.data}

    def to_map_public(self):
        return {"name": self.name, "version": self.version}


class FirmwareStorage:
    firmwares = None
    latest_firmware = None
    filename_id = 0

    @staticmethod
    def init():
        FirmwareStorage.firmwares = [Firmware(name="Simulator device", version=0x00,
                                              filename=FIRMWARE_DIR + "simulator.bin"),
                                     Firmware(name="Device with DHT11 and BMP180",
                                              version=0x01, filename=FIRMWARE_DIR + "complex.bin")]
        try:
            os.mkdir(FIRMWARE_DIR)
        except Exception as e:
            app.logger.error(e)

    @staticmethod
    def append(firmware):
        for i in FirmwareStorage.firmwares:
            if firmware.version == i.version:
                return False
        firmware.filename = FIRMWARE_DIR + \
            str(FirmwareStorage.filename_id) + '.bin'
        try:
            if os.path.isfile(firmware.filename):
                os.remove(firmware.filename)
            open(firmware.filename, 'wb').write(firmware.data)
            print(firmware.filename)
        except Exception as e:
            app.logger.error(e)
            return False

        FirmwareStorage.filename_id += 1
        FirmwareStorage.firmwares.append(firmware)
        return True

    @staticmethod
    def to_json():
        return jsonify(list(map(lambda x: x.to_map_public(), FirmwareStorage.firmwares)))

    @staticmethod
    def get_latest():
        if FirmwareStorage.latest_firmware is not None:
            return FirmwareStorage.latest_firmware.filename
        else:
            return None

    @staticmethod
    def get(version):
        for i in FirmwareStorage.firmwares:
            if i.version == version:
                return i.filename
        return None

    @staticmethod
    def select(version):
        for i in FirmwareStorage.firmwares:
            if i.version == version:
                FirmwareStorage.latest_firmware = i
                return True
        return False


FIRMWARE_DIR = "./firmware/"
app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = FIRMWARE_DIR
ALLOWED_EXTENSIONS = {'bin'}
FirmwareStorage.init()


def json_decorator(default_message: str = None):
    def decorator(func):
        def inner():
            try:
                return func()
            except Exception as e:
                app.logger.error(e)
                return jsonify({"error": e.__str__()
                                if default_message is None else default_message}), 400

        inner.__name__ = func.__name__
        return inner
    return decorator


@app.route('/api/v1/latest_firmware', methods=['GET'])
@json_decorator('firmware for updating wasn\'t choosed yet')
def get_latest():
    return send_file(FirmwareStorage.get_latest(),
                     attachment_filename='firmware.bin',
                     mimetype="application/octet-stream")


@app.route('/api/v1/firmwares', methods=['GET'])
@json_decorator()
def get_versions():
    return FirmwareStorage.to_json()


@app.route('/api/v1/upload_firmware', methods=['POST'])
@json_decorator('cannot upload such firmware')
def upload():
    content = request.get_json()
    if content is None:
        raise TypeError("can't parse request")
    if FirmwareStorage.append(Firmware(name=content['name'],
                                       filename=None,
                                       version=content['version'],
                                       data=base64.b64decode(content['data']))):
        return jsonify({"error": "ok"}), 200
    else:
        return jsonify({"error": "firmware with such version id exists"}), 400


@app.route('/api/v1/select_firmware', methods=['POST'])
@json_decorator('cannot select this version')
def select():
    content = request.get_json()
    if content is None:
        raise TypeError("can't parse request")
    if FirmwareStorage.select(content['version']):
        return jsonify({"error": "ok"}), 200
    else:
        return jsonify({"error": "firmware with such version id not exist"}), 400


@app.route('/api/v1/firmware', methods=['POST'])
@json_decorator('cannot find such firmware')
def get_firmware():
    content = request.get_json()
    if content is None:
        raise TypeError("can't parse request")
    return send_file(FirmwareStorage.get(content['version']),
                     attachment_filename='firmware.bin',
                     mimetype="application/octet-stream")
