import json
import logging
import logging.config
import os
from pathlib import Path    
from typing import Any, Dict
from .path_helper import get_project_root

def load_logging_config(config_filename="config.json", logger_name=None):
    """
    Load logging configuration from the project root directory and initialize logging.

    - Automatically locate the project root directory (based on pyproject.toml)
    - Read the specified configuration file (default: config.json)
    - Replace relative log file paths in logging handlers with absolute paths
    - Configure the logging module

    Returns:
        logging.Logger instance with the specified logger_name,
        or the root logger if logger_name is None.
    """
    root_path = get_project_root()
    config_path = root_path / config_filename

    if not config_path.exists():
        raise FileNotFoundError(f"Cannot find logging config file: {config_path}")

    config = json.loads(config_path.read_text(encoding="utf-8"))

    # Replace relative paths in handlers with absolute paths
    for handler in config.get("logging", {}).get("handlers", {}).values():
        if "filename" in handler:
            log_path = Path(handler["filename"])
            if not log_path.is_absolute():
                handler["filename"] = str(root_path / log_path)

    logging.config.dictConfig(config["logging"])

    return logging.getLogger(logger_name)


def load_emqx_config(config_filename="config.json") -> Dict[str, Any]:
    """
    Load EMQX API configuration from config.json or environment variables.
    Raises FileNotFoundError or KeyError if required config missing.
    """

    root_path = get_project_root()
    config_path = root_path / config_filename
    if not config_path.exists():
        raise FileNotFoundError(f"Cannot find config file: {config_path}")

    try:
        config = json.loads(config_path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as e:
        raise ValueError(f"Invalid JSON in config file: {e}")

    emqx_deployment_api_key = config.get("emqx", {}).get("deployment_api_key", {})

    # load API param from environment variables or config
    api_endpoint = emqx_deployment_api_key.get("api_endpoint")
    app_id = os.getenv("EMQX_DEPLOYMENT_API_KEY_APP_ID", emqx_deployment_api_key.get("app_id"))
    app_secret = os.getenv("EMQX_API_APP_SECRET", emqx_deployment_api_key.get("app_secret"))

    if not all([app_id, app_secret, api_endpoint]):
        raise KeyError("EMQX Deployment API Key, missing config")

    # allow timeout，default 60s
    timeout = config.get("emqx", {}).get("api", {}).get("timeout", 60)

    return {"api_endpoint": api_endpoint, "app_id": app_id, "app_secret": app_secret, "timeout": timeout}