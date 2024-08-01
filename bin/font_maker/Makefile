# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I./freetype-2.13.2/include -I./freetype-2.13.2/include/freetype/config -Wall -O2


# Linker flags
LDFLAGS = -lfreetype -lpng

# Target executable
TARGET = FontMaker

# Source files
SOURCES = FontMaker.cpp

# Build target
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET)
	rm -f LetterUV.txt
	rm -f alphabet.png