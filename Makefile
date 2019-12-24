default: config build test

CONFIG ?= Release
BUILD_DIR = build_$(CONFIG)

CM_GENERATOR ?= Ninja

.PHONY: win
win: config-vs build test

.PHONY: config
config:
	cmake -B$(BUILD_DIR) -S. -G"$(CM_GENERATOR)"    \
    -DCMAKE_BUILD_TYPE:STRING=$(CONFIG)             \
	-DTCC_BUILD_CLANG_TIDY=ON 						\
	-DTCC_BUILD_BENCHMARK=ON						\
    -DBENCHMARK_ENABLE_GTEST_TESTS=OFF              \
    -DBENCHMARK_ENABLE_TESTING=OFF      			

.PHONY: config-vs
config-vs:
	cmake -B$(BUILD_DIR) -S. -G "Visual Studio 16 2019" 		\
	-DBOOST_ROOT="C:/boost/boost_1_71_0" 						\
	-DBOOST_LIBRARYDIR="C:/boost/boost_1_71_0/lib64-msvc-14.2" 	\
	-DBoost_USE_STATIC_LIBS="ON"								\
	-DTCC_BUILD_BENCHMARK=ON									\
	-DBENCHMARK_ENABLE_GTEST_TESTS=OFF              			\
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

.PHONY: format
format:
	find lib -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find tcc -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find tcvm -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	
.PHONY: stats
stats:
	cloc --exclude-dir=3rd_party,build_Debug,build_Release,.vscode .
	
