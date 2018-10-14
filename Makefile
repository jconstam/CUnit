ROOT_DIR=$(shell pwd)

OUTPUT=$(ROOT_DIR)/cmake_build

PARSER_BIN=cup

run: build
	$(OUTPUT)/$(PARSER_BIN) -v -r "$(ROOT_DIR)/tests" -o "$(OUTPUT)/main.c"

build:
	mkdir -p $(OUTPUT)
	cd $(OUTPUT) && cmake .. -DPARSER_PROJECT=$(PARSER_BIN) && $(MAKE)
	
clean:
	rm -rf $(OUTPUT)