default: config build test

CONFIG ?= Release
BUILD_DIR_BASE = build
BUILD_DIR = $(BUILD_DIR_BASE)_$(CONFIG)

CM_GENERATOR ?= Ninja

.PHONY: win
win: config-vs build test

.PHONY: config
config:
	cmake -B$(BUILD_DIR) -S. -G"$(CM_GENERATOR)"    \
    -DCMAKE_BUILD_TYPE:STRING=$(CONFIG)             \
	-DTCC_BUILD_BENCHMARK=ON						\
    -DBENCHMARK_ENABLE_GTEST_TESTS=OFF              \
    -DBENCHMARK_ENABLE_TESTING=OFF      			

.PHONY: config-vs
config-vs:
	cmake -B$(BUILD_DIR) -S. -G "Visual Studio 16 2019" 		\
	-DTCC_BUILD_BENCHMARK=ON									\
	-DBENCHMARK_ENABLE_GTEST_TESTS=OFF              			\
    -DBENCHMARK_ENABLE_TESTING=OFF      						

.PHONY: build
build:
	cmake --build $(BUILD_DIR) --config $(CONFIG)

.PHONY: test
test:
	cd $(BUILD_DIR) && ctest -C $(CONFIG)

.PHONY: sanitize
sanitize:
	cmake -S. -G$(CM_GENERATOR) -B$(BUILD_DIR_BASE)_sanitize \
		-DCMAKE_BUILD_TYPE=$(CONFIG) \
		-DTCC_BUILD_ASAN=ON \
		-DTCC_BUILD_UBSAN=ON \
		.
	cmake --build $(BUILD_DIR_BASE)_sanitize --config $(CONFIG)
	cd $(BUILD_DIR_BASE)_sanitize && ctest -c

.PHONY: coverage
coverage:
	cmake -S. -G$(CM_GENERATOR) $(CMAKE_FLAGS) -B $(BUILD_DIR_BASE)_coverage -DTCC_BUILD_COVERAGE=ON
	cd $(BUILD_DIR_BASE)_coverage && cmake --build .
	cd $(BUILD_DIR_BASE)_coverage && lcov -c -i -d . --base-directory . -o base_cov.info
	cd $(BUILD_DIR_BASE)_coverage && ctest
	cd $(BUILD_DIR_BASE)_coverage && lcov -c -d . --base-directory . -o test_cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov -a base_cov.info -a test_cov.info -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*boost/*" -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*3rd_party/*" -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*c++*" -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*v1*" -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*Xcode.app*" -o cov.info

.PHONY: report
report:
	cd $(BUILD_DIR_BASE)_coverage && genhtml cov.info --output-directory lcov

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: format
format:
	find tcc -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find tcvm -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find tsl -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find playground -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	
.PHONY: stats
stats:
	cloc --exclude-dir=3rd_party,build,build_coverage,build_sanitize,build_Debug,build_Release,.vscode .
	
