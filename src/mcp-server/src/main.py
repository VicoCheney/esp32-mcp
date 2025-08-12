from .emqx_mcp_server import EMQXMCPServer

def run():
    server = EMQXMCPServer()
    server.run()

if __name__ == "__main__":
    run()
