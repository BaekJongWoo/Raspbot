# Makefile

# 컴파일러와 컴파일 플래그
CXX = g++
CXXFLAGS = -Iclient -Iserver -Iminimax -Icar_control -Iutil -Iqr -Wall -Wextra -std=c++11 `pkg-config --cflags opencv4`
LDFLAGS = -lm -lpthread -lwiringPi `pkg-config --libs opencv4`

# 소스 파일과 오브젝트 파일
C_SRC = main.c minimax/minimax.c client/client.c car_control/car_control.c util/util.c
CPP_SRC = qr/qr.cpp
C_OBJ = $(C_SRC:.c=.o)
CPP_OBJ = $(CPP_SRC:.cpp=.o)
OBJ = $(C_OBJ) $(CPP_OBJ)

# 실행 파일 이름
TARGET = myprogram

# 기본 목표: 프로그램 컴파일
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# 개별 소스 파일의 오브젝트 파일 생성 규칙
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 청소 규칙
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
