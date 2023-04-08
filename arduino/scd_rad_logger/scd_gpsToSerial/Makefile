# Make sure you've run `arudino-cli core install arduino:avr` to have all the arduino files you need
# Also, to use monitor you will need minicom. You can use a different program

.PHONY: help compile upload

SKETCHNAME=$(basename $(PWD))
FQBN=arduino:avr:mega
PORT=/dev/serial/by-id/*Arduino*

help:  ## Display this help
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \033[36m<target>\033[0m\n\nTargets:\n"} /^[a-zA-Z0-9_-]+:.*?##/ { printf "  \033[36m%-10s\033[0m %s\n", $$1, $$2 }' $(MAKEFILE_LIST)

compile:  ## Check that the sketch compiles
	arduino-cli compile --fqbn $(FQBN) $(NAME)

upload:  ## Upload the sketch to the arduino
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(NAME)

monitor:  ## Serial monitor an attached arduino
	minicom -D $(PORT) -b 115200
