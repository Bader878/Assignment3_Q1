# ---- config ----
TARGET   = Faris_Bader_101304229_A3_Q1
SRC      = Faris_Bader_101304229_A3_Q1.cpp

CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/include
LDFLAGS  = -L/opt/homebrew/lib -Wl,-rpath,/opt/homebrew/lib
LDLIBS   = -lpq

# ---- rules ----
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TARGET)
