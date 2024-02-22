# Specify the output directory
OUTPUT_DIR := out

# Define the targets
all: $(OUTPUT_DIR)/server $(OUTPUT_DIR)/client

$(OUTPUT_DIR)/server: server.cpp helpers/utils.cpp | $(OUTPUT_DIR)
	g++ -std=c++11 $^ -o $@

$(OUTPUT_DIR)/client: client.cpp helpers/utils.cpp | $(OUTPUT_DIR)
	g++ -std=c++11 $^ -o $@

# Rule to create the output directory
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Phony target to clean the project
clean:
	rm -rf $(OUTPUT_DIR)