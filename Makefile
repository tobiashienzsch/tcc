default: config build test

export PATH := $(shell pwd)/scripts:$(PATH)
CONFIG ?= Release
BUILD_DIR_BASE = build
BUILD_DIR = $(BUILD_DIR_BASE)_$(CONFIG)

CM_GENERATOR ?= Ninja

.PHONY: win
win: config-vs build test

.PHONY: config
config:
	cmake -B$(BUILD_DIR) -S. -G"$(CM_GENERATOR)" -DCMAKE_BUILD_TYPE:STRING=$(CONFIG)

.PHONY: config-vs
config-vs:
	cmake -B$(BUILD_DIR) -S. -G "Visual Studio 16 2019" -- -m

.PHONY: build
build:
	cmake --build $(BUILD_DIR) --config $(CONFIG) -- -j8

.PHONY: test
test:
	cd $(BUILD_DIR) && ctest -C $(CONFIG)

.PHONY: sanitize
sanitize:
	cmake -S. -G$(CM_GENERATOR) -B$(BUILD_DIR_BASE)_sanitize \
		-DTCC_BUILD_ASAN=ON \
		-DTCC_BUILD_UBSAN=ON \
		.
	cmake --build $(BUILD_DIR_BASE)_sanitize
	cd $(BUILD_DIR_BASE)_sanitize && ctest -c

ifneq (,$(findstring clang,$(CXX)))
    LCOV = lcov --gcov-tool llvm-gcov.sh
else
    LCOV = lcov
endif
COVERAGE_DIR=$(BUILD_DIR_BASE)_coverage
.PHONY: coverage
coverage:
	mkdir -p $(COVERAGE_DIR)
	cd $(COVERAGE_DIR) && cmake -G$(CM_GENERATOR) -B$(COVERAGE_DIR) -DTCC_BUILD_COVERAGE=ON ..
	cd $(COVERAGE_DIR) && cmake --build .
	cd $(COVERAGE_DIR) && $(LCOV) -c -i -d . --base-directory . -o base_cov.info
	cd $(COVERAGE_DIR) && ctest
	cd $(COVERAGE_DIR) && $(LCOV) -c -d . --base-directory . -o test_cov.info
	cd $(COVERAGE_DIR) && $(LCOV) -a base_cov.info -a test_cov.info -o cov.info
	cd $(COVERAGE_DIR) && $(LCOV) --remove cov.info "*boost/*" -o cov.info
	cd $(COVERAGE_DIR) && $(LCOV) --remove cov.info "*3rd_party/*" -o cov.info
	cd $(COVERAGE_DIR) && $(LCOV) --remove cov.info "*c++*" -o cov.info
	cd $(COVERAGE_DIR) && $(LCOV) --remove cov.info "*v1*" -o cov.info
	cd $(COVERAGE_DIR) && $(LCOV) --remove cov.info "*Xcode.app*" -o cov.info

.PHONY: tidy
tidy:
	cd $(BUILD_DIR) && run-clang-tidy.py ../tcc -p . -fix -header-filter="tcc/.*"
	cd $(BUILD_DIR) && run-clang-tidy.py ../tcsl -p . -fix -header-filter="tcc/.*"
	cd $(BUILD_DIR) && run-clang-tidy.py ../tcvm -p . -fix -header-filter="tcc/.*"

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
	find tcsl -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i

.PHONY: stats
stats:
	cloc --exclude-dir=3rd_party,build,build_coverage,build_sanitize,build_Debug,build_Release,.vscode,venv .

