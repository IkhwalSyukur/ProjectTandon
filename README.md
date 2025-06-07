## Water Torrent ##

## Technology
- **Node-Red**
- **NodeJS**
- **Telegram**
- **Mosquitto**

## Node-Red
### Dashboard
![Dashboard](https://github.com/IkhwalSyukur/ProjectTandon/blob/dev/nodered-telegram/doc/dashboard.jpg)

### Notif Telegram
![Telegram](https://github.com/IkhwalSyukur/ProjectTandon/blob/dev/nodered-telegram/doc/telegram.jpg)


### Set Up
1. Install Node-Red
2. Install NodeJS
3. Install Mosquitto
4. Install Sqlite3 (path for database: C:\"User"\.node-red\data)
5. Install DB Browser
4. Create Telegram Bot (BotFather, getmyid_bot)

### Library Node-Red
1. node-red-node-sqlite
2. node-red-contrib-telegrambot
3. node-red-dashboard

### Payload MQTT

```JSON
{
"sensor_id": 1,
"ketinggian_air": 23.62,
"volume_air": 38.7,
"status_pompa": "OFF",
"timestamp": 1749313960
}

