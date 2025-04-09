from http.server import BaseHTTPRequestHandler, HTTPServer

class UnrealServerRequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        print("handling request!!!")


class UnrealSessionCoordinator:
    def __init__(self):
        self.HOST="0.0.0.0"
        self.PORT=80
        self.server = HTTPServer((self.HOST, self.PORT), UnrealServerRequestHandler)
        try:
            print("Start running at local host")
            self.server.serve_forever()
        except KeyboardInterrupt:
            print("shutting down server...")
            self.server.server_close()

UnrealSessionCoordinator()