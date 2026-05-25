idf_component_register(SRCS "main.c" "i2c/i2c.c" "osal/i2c.c" "sensors/sensors.c" "libsensors/htu21d/htu21d.c" "libsensors/bmp280/bmp280.c"
                    INCLUDE_DIRS "." "sensors" "libsensors/osal" "libsensors/htu21d" "libsensors/bmp280")
