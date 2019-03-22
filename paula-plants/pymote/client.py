
from coapthon.client.helperclient import HelperClient

"""
Client request current temp from a CoAP server application
"""


def main():
    host = "127.0.0.1"
    port = 5683
    path = "temp"

    client = HelperClient(server=(host, port))

    response = client.get(path)
    print response.pretty_print()

    response = client.post(path, payload="25")

    response = client.get(path)
    print response.pretty_print()
    client.stop()


if __name__ == '__main__':
    main()
