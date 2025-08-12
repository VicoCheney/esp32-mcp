# EMQX MCP Server Setup Guide

This project provides an MCP (Model Context Protocol) server that can be integrated with MCP-compatible clients such as **Cursor**, **Cline**, or **Cherry Studio**.

## Configuration

To use this MCP server, you need to update your **MCP settings JSON file** for your chosen client.

### Step 1: Locate the MCP settings file
- **Cline**: Usually found in your workspace settings or `/Users/john/Library/Application Support/Code/User/globalStorage/saoudrizwan.claude-dev/settings/cline_mcp_settings.json`. 
- **Cursor** / **Cherry Studio**: Check the client documentation for the exact location.

### Step 2: Add MCP Server configuration

Insert the following JSON snippet into the `"mcpServers"` section of your MCP settings file:

```json
{
  "mcpServers": {
    "ESP32_MCP": {
      "timeout": 60,
      "command": "uv",
      "args": [
        "--directory",
        "/ABSOLUTE/PATH/TO/YOUR/PROJECT/src/mcp-server",
        "run",
        "mcp-server"
      ],
      "type": "stdio",
      "disabled": false
    }
  }
}
````

### Step 3: Replace the project path

Replace the placeholder path:

```
/ABSOLUTE/PATH/TO/YOUR/PROJECT
```

with the **absolute path** to where you cloned or downloaded this project.
For example, if you placed the project under `/Users/john/Projects`, it should look like:

```json
"--directory",
"/Users/john/Projects/esp32-mcp/src/mcp-server"
```

## Example

If your project is located at `/Users/john/Projects/esp32-mcp`, the configuration will be:

```json
{
  "mcpServers": {
    "ESP32_MCP": {
      "timeout": 60,
      "command": "uv",
      "args": [
        "--directory",
        "/Users/john/Projects/esp32-mcp/src/mcp-server",
        "run",
        "mcp-server"
      ],
      "type": "stdio",
      "disabled": false
    }
  }
}
```

---

## Notes

* The `uv` command must be installed and available in your system’s PATH.
* Ensure the path you provide is **absolute**, not relative.
* This configuration assumes the MCP server is launched via `uv run mcp-server` from the `src/mcp-server` directory.
