# 컴파일러 설정
CC = gcc

# 컴파일러 플래그 설정
CFLAGS = -lwiringPi

# 소스 파일
SRC = linetrace.c

# 출력 파일
TARGET = linetracer

# 기본 빌드 규칙
all: $(TARGET)

# 실행 파일 빌드 규칙
$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS)

# 클린 규칙
clean:
	rm -f $(TARGET)

.PHONY: all clean

