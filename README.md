## Water Torrent ##

## Technology
- **Node-Red**
- **NodeJS**
- **Telegram**
- **Mosquitto**

## Communication Protocol
**MQTT**
- Broker: test.mosquitto.org
- PORT: 1883
- QoS: 0
- Topic Node 1: water_torrent/sensorData
- Topic Node 2: water_torrent/sensorData_2

## Node-Red
### Dashboard
![Dashboard](https://github.com/IkhwalSyukur/ProjectTandon/blob/dev/nodered-telegram/doc/dashboard.jpg)

### Notif Telegram
![Telegram](https://github.com/IkhwalSyukur/ProjectTandon/blob/dev/nodered-telegram/doc/telegram.jpg)


### Set Up
1. Install Node-Red
2. Install NodeJS
3. Install Sqlite3 (path for database: C:\"User"\.node-red\data)
4. Install DB Browser
5. Create Telegram Bot (BotFather, getmyid_bot)

### Library Node-Red
1. node-red-node-sqlite
2. node-red-contrib-telegrambot
3. node-red-dashboard

## Command Telegram
- "/start" command for start telegram bot
- "/status_node1" and "/status_node2" for get information node
- Alert (Node will be automatic to send information to telegram bot)
 
## Payload MQTT
Change sensor_id with actual node number. i.e: "1" / "2".

```JSON
{
"sensor_id": 1,
"ketinggian_air": 23.62,
"volume_air": 38.7,
"status_pompa": "OFF",
"status_ketinggia_air": "Tinggi",
"status_volume_air": "Sedikit",
"status_laju_air": "Lambat",
"timestamp": 1749313960
}

