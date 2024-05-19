# 컴파일러 설정
CXX = g++

# 컴파일러 플래그 설정
FLAGS = `pkg-config --cflags --libs opencv4`

# 소스 파일
SRC = main.cpp

# 출력 파일
TARGET = qrrecognition

# 기본 빌드 규칙
all: $(TARGET)

# 실행 파일 빌드 규칙
$(TARGET): $(SRC)
	$(CXX) -o $(TARGET) $(SRC) $(FLAGS) 

# 클린 규칙
clean:
	rm -f $(TARGET)

.PHONY: all clean

