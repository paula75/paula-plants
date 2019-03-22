
import sys
import getopt
from coapthon import defines
from coapthon.server.coap import CoAP
from coapthon.resources.resource import Resource


class TempResource(Resource):
    def __init__(self, name="TempResource", coap_server=None):
        super(TempResource, self).__init__(name, coap_server, visible=True, observable=True, allow_children=True)
        self.payload = "Temp Resource"
        self.resource_type = "rt1"
        self.content_type = "text/plain"

    def render_GET(self, request):
        return self

    def render_POST(self, request):
        res = self.init_resource(request, TempResource())
        return res


class CoAPServer(CoAP):
    def __init__(self, host, port, multicast=False):
        CoAP.__init__(self,(host, port), multicast)
        self.add_resource('temp/', TempResource())

        print "CoAP Server start on " + host + ":" + str(port)
        print self.root.dump()


def usage():  # pragma: no cover
    print "server.py -i <ip address> -p <port>"


def main(argv):  # pragma: no cover
    ip = "0.0.0.0"
    port = 5683
    multicast = False
    try:
        opts, args = getopt.getopt(argv, "hi:p:m", ["ip=", "port=", "multicast"])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            usage()
            sys.exit()
        elif opt in ("-i", "--ip"):
            ip = arg
        elif opt in ("-p", "--port"):
            port = int(arg)
        elif opt in ("-m", "--multicast"):
            multicast = True

    server = CoAPServer(ip, port, multicast)
    try:
        server.listen(10)
    except KeyboardInterrupt:
        print "Server Shutdown"
        server.close()
        print "Exiting..."


if __name__ == "__main__":  # pragma: no cover
    main(sys.argv[1:])
