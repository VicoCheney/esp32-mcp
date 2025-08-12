"""
EMQX MCP Tools

This module provides tools for MCP clients to connect with EMQX brokers.
It registers these tools with the MCP server, making them available for clients to use through the MCP protocol.
"""

import logging
import httpx
import base64
import json
from typing import Any, Optional, Dict
from pydantic import BaseModel, Field, field_validator
from .utils.config_loader import load_emqx_config

class MQTTMessagePublishRequest(BaseModel):
    topic: str = Field(..., description="The MQTT topic to publish to")
    payload: str = Field(..., description="The JSON message content to publish")
    qos: Optional[int] = Field(0, ge=0, le=2, description="Quality of Service level (0, 1, or 2)")
    retain: Optional[bool] = Field(False, description="Whether to retain the message")

    @field_validator("payload")
    def validate_payload_json(cls, v):
        try:
            json.loads(v)
        except Exception:
            raise ValueError("payload must be a valid JSON string")
        return v

class EMQXMCPTools:

    def __init__(self, logger: logging.Logger):
        self.logger = logger

        # Load configuration
        conf = load_emqx_config()
        self.api_endpoint = conf["api_endpoint"]
        self.app_id = conf["app_id"]
        self.app_secret = conf["app_secret"]
        self.timeout = conf["timeout"]
        self._client = httpx.AsyncClient(timeout=self.timeout)

    def _get_auth_header(self):
        """Create authorization header for EMQX Cloud API"""
        auth_string = f"{self.app_id}:{self.app_secret}"
        encoded_auth = base64.b64encode(auth_string.encode()).decode()
        return {
            "Authorization": f"Basic {encoded_auth}",
            "Content-Type": "application/json",
        }
    
    async def close(self):
        await self._client.aclose()

    def _handle_response(self, response):
        """Process API response, extract data and handle errors"""
        try:
            if response.status_code >= 200 and response.status_code < 300:
                return response.json()
            else:
                error_msg = f"EMQX API Error: {response.status_code} - {response.text}"
                self.logger.error(error_msg)
                return {"error": error_msg}
        except Exception as e:
            error_msg = f"Error processing response: {str(e)}"
            self.logger.error(error_msg)
            return {"error": error_msg}

    def register_tools(self, mcp: Any):

        @mcp.tool()
        async def publish_mqtt_message(request: MQTTMessagePublishRequest) -> dict:
            """
            Function:
                Publish MQTT messages to control IoT devices via EMQX Cloud or a self-managed MQTT broker,
                specifically for ESP32-MCP projects.

            Overview:
                This method sends control commands to predefined MQTT topics, enabling remote control
                of ESP32-connected devices such as LEDs and buzzers.

            Supported Topics:
                esp32-mcp/control/led
                    Controls LED state (e.g., turn on or off).
                esp32-mcp/control/buzzer
                    Controls buzzer state (e.g., turn on or off).

            Payload Format (JSON):
                {
                    "command": "<action>"
                }
                command (string, required):
                    Action to perform on the device.
                    Allowed values: "on", "off" (case-insensitive; custom actions may be supported).

            Example Payload:
                {
                    "command": "on"
                }

            Args:
                request (MQTTMessagePublishRequest): Request object containing:
                    topic (str): MQTT topic to publish to. Must match one of the supported topics.
                    payload (str): JSON-formatted message content, following the Payload Format.
                    qos (int, optional): MQTT Quality of Service level (0, 1, or 2). Default is 0.
                    retain (bool, optional): Whether to retain the message on the broker. Default is False.

            Returns:
                dict: Result of the publish operation.

            Notes:
                - Ensure the MQTT broker connection is established before publishing.
                - Payload must be valid JSON; otherwise, the publish will fail.
                - If retain is True, the last message will be stored and sent to new subscribers.
            """

            """
            ============================================
            ESP32 Buzzer Song Score Guide
            ============================================

            This function explains how to convert a traditional song or sheet music
            into a JSON score compatible with the ESP32 buzzer controller.

            --------------------
            1. Score JSON Structure
            --------------------
            Each song uses the following JSON format:

            {
                "command": "PLAY_SONG",
                "tempo": 120,           # Global tempo in BPM
                "score": [
                    {
                        "freq": 262,       # Note frequency in Hz (0 = rest)
                        "beats": 1,        # Note length in beats
                        "amp": 200,        # Volume (PWM duty cycle 0-255)
                        "tie": false,      # Legato: if true, connect to next note
                        "freq_start": 262, # Optional: start frequency for glissando
                        "freq_end": 330    # Optional: end frequency for glissando
                    }
                ]
            }

            --------------------
            2. Steps to Convert a Song
            --------------------

            Step 1: Determine Note Frequencies
                - Refer to the song's sheet music (C major, piano, guitar, etc.)
                - Map notes to frequencies:
                    C4 = 262 Hz
                    D4 = 294 Hz
                    E4 = 330 Hz
                    F4 = 349 Hz
                    G4 = 392 Hz
                    A4 = 440 Hz
                    B4 = 494 Hz
                - Use standard 12-TET table for other octaves.

            Step 2: Determine Note Length
                - Identify the song's time signature (e.g., 4/4)
                - Use the global 'tempo' to calculate one beat duration:
                    beat_ms = 60000 / tempo
                - Set 'beats' for each note (1 beat = quarter note)

            Step 3: Rest Notes
                - Set "freq": 0 for rests
                - Specify "beats" for rest duration

            Step 4: Tie / Legato
                - If two notes are connected without gap, set "tie": true on the first note

            Step 5: Glissando / Slide
                - For notes sliding in pitch:
                    freq_start = starting frequency
                    freq_end   = ending frequency
                - Optionally leave "freq" the same as freq_start

            Step 6: Volume / Dynamics
                - Use "amp" to simulate loudness (0-255)
                - Adjust according to song dynamics

            Step 7: Assemble the Score
                - Arrange all notes in order inside the "score" array
                - Set global "tempo" for the song

            --------------------
            3. Example: "Twinkle Twinkle Little Star"
            --------------------
            | Note | Beats |
            |------|-------|
            | C4   | 1     |
            | C4   | 1     |
            | G4   | 1     |
            | G4   | 1     |
            | A4   | 1     |
            | A4   | 1     |
            | G4   | 2     |

            JSON representation:

            {
            "command": "PLAY_SONG",
            "tempo": 100,
            "score": [
                { "freq": 262, "beats": 1, "amp": 200 },
                { "freq": 262, "beats": 1, "amp": 200 },
                { "freq": 392, "beats": 1, "amp": 200 },
                { "freq": 392, "beats": 1, "amp": 200 },
                { "freq": 440, "beats": 1, "amp": 220 },
                { "freq": 440, "beats": 1, "amp": 220 },
                { "freq": 392, "beats": 2, "amp": 200 }
            ]
            }

            --------------------
            4. Tips
            --------------------
            - Keep "beats" consistent to easily adjust tempo
            - Always include rests for natural rhythm
            - Use "tie" for legato passages
            - Adjust "amp" to simulate dynamics
            - Test and iterate: playback may require slight adjustments

            """


            self.logger.info("Handling publish request")

            if not request.topic:
                err = "Missing required parameter: topic"
                self.logger.error(err)
                return {"error": err}

            if request.payload is None:
                err = "Missing required parameter: payload"
                self.logger.error(err)
                return {"error": err}

            result = await self._publish_message(
                topic=request.topic,
                payload=request.payload,
                qos=request.qos if request.qos is not None else 0,
                retain=request.retain if request.retain is not None else False,
            )

            if "error" not in result:
                self.logger.info(f"Message published successfully to topic: {request.topic}")
            else:
                self.logger.error(f"Failed to publish message to topic: {request.topic}, error: {result['error']}")

            return result
    
    async def _publish_message(
        self, topic: str, payload: str, qos: int = 0, retain: bool = False
    ) -> Dict[str, Any]:
        """
        Publish a message to an MQTT topic.

        Uses the EMQX HTTP API to publish a message to a specific MQTT topic.

        Args:
            topic (str): The MQTT topic to publish to
            payload (str): The message payload to publish
            qos (int, optional): Quality of Service level (0, 1, or 2). Defaults to 0.
            retain (bool, optional): Whether to retain the message. Defaults to False.

        Returns:
            dict: Response from the EMQX API or error information
        """

        url = f"{self.api_endpoint}/publish"
        data = {"topic": topic, "payload": payload, "qos": qos, "retain": retain}
        self.logger.debug(f"Publishing message to topic {topic} with payload {payload}")
        try:
            response = await self._client.post(url, headers=self._get_auth_header(), json=data)
            response.raise_for_status()
            return self._handle_response(response)
        except httpx.RequestError as e:
            err = f"Network error while publishing message: {e}"
            self.logger.error(err)
            return {"error": err}
        except httpx.HTTPStatusError as e:
            err = f"HTTP error while publishing message: {e.response.status_code} - {e.response.text}"
            self.logger.error(err)
            return {"error": err}
        except Exception as e:
            err = f"Unexpected error while publishing message: {str(e)}"
            self.logger.error(err)
            return {"error": err}
