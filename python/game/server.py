from http.server import BaseHTTPRequestHandler, HTTPServer

class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        #send the manual
        with open("manual.html", 'r') as infile:
            self.wfile.write(infile.read().encode())
        return

def run(server_class=HTTPServer, handler_class=Handler):
    try:
        print("Server started")
        server_address = ('', 8080)
        httpd = server_class(server_address, handler_class)
        httpd.serve_forever()
    except KeyboardInterrupt:
        httpd.server_close()
        print("Server stopped")