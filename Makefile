CFLAGS := -std=c++17 -fvisibility=hidden -lpthread -O2 -fopenmp -march=native
CFLAGS += -Wall -Wextra -Werror

SOURCES := src/*.cpp
HEADERS := src/*.h

TARGETBIN := fft.out

.SUFFIXES:
.PHONY: all clean

all: $(TARGETBIN)

$(TARGETBIN): $(SOURCES) $(HEADERS)
	$(CXX) -o $@ $(CFLAGS) $(SOURCES)

format:
	clang-format -i $(SOURCES) $(HEADERS)

clean:
	rm -rf ./*.out
