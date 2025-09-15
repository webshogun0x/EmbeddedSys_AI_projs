# ESP32-S3 Camera with Telegram Bot

ESP32-S3 camera project with Telegram bot integration and tactile switch trigger.

## Features

- **Camera Capture**: OV5640 camera with auto-focus
- **Telegram Bot**: Send photos via Telegram commands
- **Physical Button**: Tactile switch to trigger photo capture
- **Web Interface**: Local web server for camera preview
- **Flash Control**: Toggle flash LED via Telegram

## Setup

1. **Clone Repository**
   ```bash
   git clone <your-repo-url>
   cd later
   ```

2. **Configure Credentials**
   ```bash
   cp include/secrets_template.h include/secrets.h
   ```
   Edit `include/secrets.h` with your credentials:
   - WiFi SSID and password
   - Telegram bot token (from @BotFather)
   - Telegram chat ID

3. **Hardware Connections**
   - Tactile switch: GPIO 14 to GND
   - Flash LED: GPIO 4
   - Camera: OV5640 connected per camera_pins.h

4. **Build and Upload**
   ```bash
   pio run --target upload
   ```

## Telegram Commands

- `/start` - Show welcome message
- `/photo` - Take and send photo
- `/flash` - Toggle flash LED

## Hardware Trigger

Press the tactile switch connected to GPIO 14 to capture and send a photo.

## Files

- `src/main.cpp` - Main application code
- `src/camera_server.cpp` - Web server implementation
- `include/secrets.h` - Credentials (not in git)
- `include/secrets_template.h` - Template for credentials
- `include/camera_pins.h` - Camera pin definitions