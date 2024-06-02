# Makefile

# 컴파일러와 컴파일 플래그
CC = gcc
CFLAGS = -Iclient -Iserver -Iminimax -Wall -Wextra -std=c11

# 소스 파일과 오브젝트 파일
SRC = main.c minimax/minimax.c client/client.c
OBJ = $(SRC:.c=.o)

# 실행 파일 이름
TARGET = myprogram

# 기본 목표: 프로그램 컴파일
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lm -lpthread

# 개별 소스 파일의 오브젝트 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 청소 규칙
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean