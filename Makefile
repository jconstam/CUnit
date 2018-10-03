ROOT_DIR=$(shell pwd)

OUTPUT=$(ROOT_DIR)/cmake_build

PARSER_BIN=cup

run: build
	$(OUTPUT)/$(PARSER_BIN) -r "$(ROOT_DIR)/unittests"

build:
	mkdir -p $(OUTPUT)
	cd $(OUTPUT) && cmake .. -DPARSER_PROJECT=$(PARSER_BIN) && $(MAKE)
	
clean:
	rm -rf $(OUTPUT)