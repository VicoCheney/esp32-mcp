# ESP32-MCP

<div align="center">

![ESP32-MCP Logo](/images/logo.png)

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT) [![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange)](https://platformio.org/) [![MQTT](https://img.shields.io/badge/MQTT-EMQX-green)](https://www.emqx.io/) [![MCP](https://img.shields.io/badge/MCP-red)](https://modelcontextprotocol.io/)

<div>
<a href="#chinese">中文</a> | <a href="#english">English</a>
</div>

</div>

<a id="chinese"></a>

## 📖 项目概述

ESP32-MCP是一个创新的AIoT（人工智能物联网）项目，旨在通过MCP（Model Control Protocol）服务器和MQTT协议，将大型语言模型（LLMs）的能力与本地ESP32设备无缝连接。该项目使开发者能够通过自然语言指令控制物理设备，实现智能家居、工业自动化和教育等领域的创新应用。

### 主要特点

- 🤖 **AI驱动控制**：通过大语言模型解析自然语言指令，控制物理设备
- 🔄 **双向通信**：设备状态反馈给AI，实现闭环控制
- 🔌 **即插即用**：模块化设计，易于扩展新设备和功能
- 🛠️ **可定制性**：支持自定义主题和控制命令
- 📱 **远程控制**：通过MQTT协议实现设备的远程监控和控制
- 🔄 **OTA更新**：支持固件的远程更新

## 🏗️ 系统架构

```
+----------------+      +----------------+      +----------------+
|                |      |                |      |                |
|  大语言模型    | <--> |  MCP服务器     | <--> |  MQTT代理      |
|  (LLM)         |      |  (Python)      |      |  (EMQX)        |
|                |      |                |      |                |
+----------------+      +----------------+      +-------+--------+
                                                        |
                                                        |
                                                        v
                                               +----------------+
                                               |                |
                                               |  ESP32设备     |
                                               |  (Arduino)     |
                                               |                |
                                               +-------+--------+
                                                       |
                                                       |
                                               +-------v--------+
                                               |                |
                                               |  物理设备     |
                                               |  (LED/蜂鸣器等)|
                                               |                |
                                               +----------------+
```

## 🚀 快速开始

### 前提条件

- [PlatformIO](https://platformio.org/) (用于ESP32开发)
- [Python 3.12+](https://www.python.org/downloads/) (用于MCP服务器)
- [EMQX MQTT代理](https://www.emqx.io/) (可以使用云服务或本地部署)
- ESP32-S3开发板

### ESP32设备设置

1. 克隆仓库
   ```bash
   git clone https://github.com/VicoCheney/esp32-mcp.git
   cd esp32-mcp
   ```

2. 配置设备
   - 在`src/esp32/data/`目录下创建`config.json`文件：
   ```json
   {
     "wifi": {
       "ssid": "你的WiFi名称",
       "password": "你的WiFi密码"
     },
     "mqtt": {
       "broker": "mqtt.example.com",
       "port": 1883,
       "username": "mqtt_user",
       "password": "mqtt_password",
       "client_id": "esp32_client"
     }
   }
   ```

3. 编译并上传
   ```bash
   cd src/esp32
   pio run -t upload
   ```

4. 上传文件系统
   ```bash
   pio run -t uploadfs
   ```

### MCP服务器设置

1. 安装依赖
   ```bash
   cd src/mcp-server
   uv sync
   ```

2. 配置服务器
   - 编辑`config.json`文件：
   ```json
   {
     "emqx": {
       "api_endpoint": "https://your-emqx-instance.com/api/v4",
       "app_id": "your_app_id",
       "app_secret": "your_app_secret",
       "timeout": 10
     },
     "logging": {
       "level": "INFO",
       "format": "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
     }
   }
   ```

3. 启动服务器
   ```bash
   cd src/mcp-server
   python src/main.py
   ```

## 📡 MQTT主题

### 控制主题

- **LED控制**：`esp32-mcp/control/led`
  ```json
  {"command": "on"}  // 或 {"command": "off"}
  ```

- **蜂鸣器控制**：`esp32-mcp/control/buzzer`
  ```json
  {"command": "on"}  // 或 {"command": "off"}
  ```

- **蜂鸣器播放音乐**：`esp32-mcp/control/buzzer`
  ```json
  {
    "command": "PLAY_SONG",
    "tempo": 120,
    "score": [
      {"freq": 262, "beats": 1, "amp": 200, "tie": false},
      {"freq": 294, "beats": 1, "amp": 200, "tie": false},
      // 更多音符...
    ]
  }
  ```

### 状态主题

- **设备状态**：`esp32-mcp/status/device`
  ```json
  {
    "device_id": "esp32_client",
    "uptime": 3600,
    "wifi_rssi": -65,
    "free_heap": 120000
  }
  ```

- **LED状态**：`esp32-mcp/status/led`
  ```json
  {"state": "on"}  // 或 {"state": "off"}
  ```

## 🎵 蜂鸣器音乐指南

### 音符频率参考

| 音符 | 频率 (Hz) |
|------|----------|
| C4   | 262      |
| D4   | 294      |
| E4   | 330      |
| F4   | 349      |
| G4   | 392      |
| A4   | 440      |
| B4   | 494      |
| C5   | 523      |

### 示例：《小星星》

```json
{
  "command": "PLAY_SONG",
  "tempo": 120,
  "score": [
    {"freq": 262, "beats": 1, "amp": 200, "tie": false},
    {"freq": 262, "beats": 1, "amp": 200, "tie": false},
    {"freq": 392, "beats": 1, "amp": 200, "tie": false},
    {"freq": 392, "beats": 1, "amp": 200, "tie": false},
    {"freq": 440, "beats": 1, "amp": 200, "tie": false},
    {"freq": 440, "beats": 1, "amp": 200, "tie": false},
    {"freq": 392, "beats": 2, "amp": 200, "tie": false},
    {"freq": 349, "beats": 1, "amp": 200, "tie": false},
    {"freq": 349, "beats": 1, "amp": 200, "tie": false},
    {"freq": 330, "beats": 1, "amp": 200, "tie": false},
    {"freq": 330, "beats": 1, "amp": 200, "tie": false},
    {"freq": 294, "beats": 1, "amp": 200, "tie": false},
    {"freq": 294, "beats": 1, "amp": 200, "tie": false},
    {"freq": 262, "beats": 2, "amp": 200, "tie": false}
  ]
}
```

## 🧩 扩展功能

### 添加新的主题处理器

1. 创建新的处理器类，继承自`TopicHandler`：

```cpp
// 在include目录下创建新文件，如MyHandler.h
#ifndef MY_HANDLER_H
#define MY_HANDLER_H

#include "TopicHandler.h"

class MyHandler : public TopicHandler {
public:
    void init() override {
        // 初始化代码
    }
    
    void handleMessage(DynamicJsonDocument& doc) override {
        // 处理消息的代码
    }
    
    const char* getTopic() const override {
        return "esp32-mcp/control/mydevice";
    }
};

#endif
```

2. 在`TopicHandlerRegistry`中注册新的处理器

## 📚 API参考

### MCP服务器API

#### 发布MQTT消息

```python
publish_mqtt_message(request: MQTTMessagePublishRequest) -> dict
```

参数：
- `topic` (str): MQTT主题
- `payload` (str): JSON格式的消息内容
- `qos` (int, 可选): 服务质量级别 (0, 1, 或 2)，默认为0
- `retain` (bool, 可选): 是否保留消息，默认为False

返回：
- `dict`: 发布操作的结果

## 🔧 故障排除

### 常见问题

1. **ESP32无法连接WiFi**
   - 检查`config.json`中的WiFi凭据
   - 确保WiFi信号强度足够

2. **MQTT连接失败**
   - 验证MQTT代理地址和凭据
   - 检查网络防火墙设置

3. **设备不响应命令**
   - 检查MQTT主题是否正确
   - 验证JSON格式是否有效

## 🤝 贡献

欢迎贡献！请随时提交问题或拉取请求。

## 📄 许可证

本项目采用MIT许可证 - 详情请参阅[LICENSE](LICENSE)文件。

## 📞 联系方式

如有问题或建议，请通过以下方式联系我们：

- 项目维护者：[VicoCheney](mailto:vicocheney@outlook.com)
- GitHub问题：[提交问题](https://github.com/VicoCheney/esp32-mcp/issues)

---

<a id="english"></a>

## 📖 Project Overview

ESP32-MCP is an innovative AIoT (Artificial Intelligence of Things) project designed to seamlessly connect Large Language Models (LLMs) with local ESP32 devices through MCP (Model Control Protocol) server and MQTT protocol. This project enables developers to control physical devices using natural language instructions, facilitating innovative applications in smart homes, industrial automation, and education.

### Key Features

- 🤖 **AI-Driven Control**: Control physical devices through natural language instructions interpreted by large language models
- 🔄 **Bidirectional Communication**: Device status feedback to AI, enabling closed-loop control
- 🔌 **Plug and Play**: Modular design for easy extension of new devices and features
- 🛠️ **Customizability**: Support for custom topics and control commands
- 📱 **Remote Control**: Remote monitoring and control of devices via MQTT protocol
- 🔄 **OTA Updates**: Support for remote firmware updates

## 🏗️ System Architecture

```
+----------------+      +----------------+      +----------------+
|                |      |                |      |                |
|  Large Language| <--> |  MCP Server   | <--> |  MQTT Broker   |
|  Model (LLM)   |      |  (Python)     |      |  (EMQX)        |
|                |      |                |      |                |
+----------------+      +----------------+      +-------+--------+
                                                        |
                                                        |
                                                        v
                                               +----------------+
                                               |                |
                                               |  ESP32 Device  |
                                               |  (Arduino)     |
                                               |                |
                                               +-------+--------+
                                                       |
                                                       |
                                               +-------v--------+
                                               |                |
                                               |  Physical      |
                                               |  Devices       |
                                               |  (LED/Buzzer)  |
                                               +----------------+
```

## 🚀 Quick Start

### Prerequisites

- [PlatformIO](https://platformio.org/) (for ESP32 development)
- [Python 3.12+](https://www.python.org/downloads/) (for MCP server)
- [EMQX MQTT Broker](https://www.emqx.io/) (cloud service or local deployment)
- ESP32-S3 Development Board

### ESP32 Device Setup

1. Clone the repository
   ```bash
   git clone https://github.com/VicoCheney/esp32-mcp.git
   cd esp32-mcp
   ```

2. Configure the device
   - Create a `config.json` file in the `src/esp32/data/` directory:
   ```json
   {
     "wifi": {
       "ssid": "your_wifi_name",
       "password": "your_wifi_password"
     },
     "mqtt": {
       "broker": "mqtt.example.com",
       "port": 1883,
       "username": "mqtt_user",
       "password": "mqtt_password",
       "client_id": "esp32_client"
     }
   }
   ```

3. Compile and upload
   ```bash
   cd src/esp32
   pio run -t upload
   ```

4. Upload the filesystem
   ```bash
   pio run -t uploadfs
   ```

### MCP Server Setup

1. Install dependencies
   ```bash
   cd src/mcp-server
   uv sync
   ```

2. Configure the server
   - Edit the `config.json` file:
   ```json
   {
     "emqx": {
       "api_endpoint": "https://your-emqx-instance.com/api/v4",
       "app_id": "your_app_id",
       "app_secret": "your_app_secret",
       "timeout": 10
     },
     "logging": {
       "level": "INFO",
       "format": "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
     }
   }
   ```

3. Start the server
   ```bash
   cd src/mcp-server
   python src/main.py
   ```

## 📡 MQTT Topics

### Control Topics

- **LED Control**: `esp32-mcp/control/led`
  ```json
  {"command": "on"}  // or {"command": "off"}
  ```

- **Buzzer Control**: `esp32-mcp/control/buzzer`
  ```json
  {"command": "on"}  // or {"command": "off"}
  ```

- **Buzzer Play Music**: `esp32-mcp/control/buzzer`
  ```json
  {
    "command": "PLAY_SONG",
    "tempo": 120,
    "score": [
      {"freq": 262, "beats": 1, "amp": 200, "tie": false},
      {"freq": 294, "beats": 1, "amp": 200, "tie": false},
      // more notes...
    ]
  }
  ```

### Status Topics

- **Device Status**: `esp32-mcp/status/device`
  ```json
  {
    "device_id": "esp32_client",
    "uptime": 3600,
    "wifi_rssi": -65,
    "free_heap": 120000
  }
  ```

- **LED Status**: `esp32-mcp/status/led`
  ```json
  {"state": "on"}  // or {"state": "off"}
  ```

## 🎵 Buzzer Music Guide

### Note Frequency Reference

| Note | Frequency (Hz) |
|------|---------------|
| C4   | 262           |
| D4   | 294           |
| E4   | 330           |
| F4   | 349           |
| G4   | 392           |
| A4   | 440           |
| B4   | 494           |
| C5   | 523           |

### Example: "Twinkle Twinkle Little Star"

```json
{
  "command": "PLAY_SONG",
  "tempo": 120,
  "score": [
    {"freq": 262, "beats": 1, "amp": 200, "tie": false},
    {"freq": 262, "beats": 1, "amp": 200, "tie": false},
    {"freq": 392, "beats": 1, "amp": 200, "tie": false},
    {"freq": 392, "beats": 1, "amp": 200, "tie": false},
    {"freq": 440, "beats": 1, "amp": 200, "tie": false},
    {"freq": 440, "beats": 1, "amp": 200, "tie": false},
    {"freq": 392, "beats": 2, "amp": 200, "tie": false},
    {"freq": 349, "beats": 1, "amp": 200, "tie": false},
    {"freq": 349, "beats": 1, "amp": 200, "tie": false},
    {"freq": 330, "beats": 1, "amp": 200, "tie": false},
    {"freq": 330, "beats": 1, "amp": 200, "tie": false},
    {"freq": 294, "beats": 1, "amp": 200, "tie": false},
    {"freq": 294, "beats": 1, "amp": 200, "tie": false},
    {"freq": 262, "beats": 2, "amp": 200, "tie": false}
  ]
}
```

## 🧩 Extending Functionality

### Adding a New Topic Handler

1. Create a new handler class that inherits from `TopicHandler`:

```cpp
// Create a new file in the include directory, e.g., MyHandler.h
#ifndef MY_HANDLER_H
#define MY_HANDLER_H

#include "TopicHandler.h"

class MyHandler : public TopicHandler {
public:
    void init() override {
        // Initialization code
    }
    
    void handleMessage(DynamicJsonDocument& doc) override {
        // Message handling code
    }
    
    const char* getTopic() const override {
        return "esp32-mcp/control/mydevice";
    }
};

#endif
```

2. Register the new handler in `TopicHandlerRegistry`

## 📚 API Reference

### MCP Server API

#### Publish MQTT Message

```python
publish_mqtt_message(request: MQTTMessagePublishRequest) -> dict
```

Parameters:
- `topic` (str): MQTT topic
- `payload` (str): JSON-formatted message content
- `qos` (int, optional): Quality of Service level (0, 1, or 2), default is 0
- `retain` (bool, optional): Whether to retain the message, default is False

Returns:
- `dict`: Result of the publish operation

## 🔧 Troubleshooting

### Common Issues

1. **ESP32 Cannot Connect to WiFi**
   - Check WiFi credentials in `config.json`
   - Ensure WiFi signal strength is adequate

2. **MQTT Connection Failure**
   - Verify MQTT broker address and credentials
   - Check network firewall settings

3. **Device Not Responding to Commands**
   - Check if MQTT topics are correct
   - Verify JSON format is valid

## 🤝 Contributing

Contributions are welcome! Feel free to submit issues or pull requests.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📞 Contact

For questions or suggestions, please contact us through:

- Project Maintainer: [VicoCheney](mailto:vicocheney@outlook.com)
- GitHub Issues: [Submit an Issue](https://github.com/VicoCheney/esp32-mcp/issues)
