export

ifneq (, $(findstring STM32F0, $(DEVICE)))
    SERIES_FOLDER = STM32F0xx
else \
ifneq (, $(findstring STM32F1, $(DEVICE)))
    SERIES_FOLDER = STM32F1xx
else \
ifneq (, $(findstring STM32F2, $(DEVICE)))
    SERIES_FOLDER = STM32F2xx
else \
ifneq (, $(findstring STM32F3, $(DEVICE)))
    SERIES_FOLDER = STM32F3xx
else \
ifneq (, $(findstring STM32F4, $(DEVICE)))
    SERIES_FOLDER = STM32F4xx
else \
ifneq (, $(findstring STM32F7, $(DEVICE)))
    SERIES_FOLDER = STM32F7xx
else
    $(error Please set a valid DEVICE name.)
endif
