"""
EMQX MCP Server

This module provides the EMQX MCP Server for any MCP Clients to connect to and interact with EMQX MQTT broker through its HTTP API. 
It sets up a FastMCP server that registers EMQX-specific tools for clients to use.
"""

from mcp.server.fastmcp import FastMCP
from .emqx_mcp_tools import EMQXMCPTools
from .utils.config_loader import load_logging_config

class EMQXMCPServer:

    """
    EMQX MCP Server
    
    This class initializes the EMQX MCP Server and registers the necessary tools.
    """

    def __init__(self):
        """
        Initialize the EMQX MCP Server.

        This method performs the following tasks:
        1. Initializes the FastMCP server.
        2. Loads the configuration from a YAML file.
        3. Sets up logging based on the configuration.
        4. Initializes the EMQX-specific tools for client usage.
        """

        self.name = "emqx_mcp_server"
        self.mcp = FastMCP("emqx_mcp_server")
        self.logger = load_logging_config(logger_name=self.name)

        # Register tools for client usage
        self._register_tools()

    def _register_tools(self):
        # Register tools
        tools = EMQXMCPTools(self.logger)
        tools.register_tools(self.mcp)
        self.logger.info("MCP tools registered")

    def run(self):
        self.logger.info("Starting EMQX MCP Server")
        self.mcp.run()