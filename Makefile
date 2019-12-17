default: config build test

CONFIG ?= Release
BUILD_DIR = build_$(CONFIG)

CM_GENERATOR ?= Ninja

.PHONY: config
config:
	cmake -B$(BUILD_DIR) -S. -G"$(CM_GENERATOR)"    \
    -DCMAKE_BUILD_TYPE:STRING=$(CONFIG)             \
    -DTCC_BUILD_PLAYGROUND=ON                       \
    -DBENCHMARK_ENABLE_GTEST_TESTS=OFF              \
    -DBENCHMARK_ENABLE_TESTING=OFF      			

.PHONY: build
build:
	cmake --build $(BUILD_DIR) --config $(CONFIG)

.PHONY: test
test:
	cd $(BUILD_DIR) && ctest -C $(CONFIG)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: stats
stats:
	cloc --exclude-dir=3rd_party,build_Debug,build_Release,.vscode .
	