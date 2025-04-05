idf_component_register(
    SRCS
        "MqttHandler.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        mqtt nvs_flash
)