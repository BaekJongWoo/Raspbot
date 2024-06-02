# ì»´íŒŒ?¼?Ÿ¬ ?„¤? •
CC = gcc

# ì»´íŒŒ?¼?Ÿ¬ ?”Œ?ž˜ê·? ?„¤? •
CFLAGS = -lwiringPi

# ?†Œ?Š¤ ?ŒŒ?¼
SRC = linetracePID.c

# ì¶œë ¥ ?ŒŒ?¼
TARGET = linetracer

# ê¸°ë³¸ ë¹Œë“œ ê·œì¹™
all: $(TARGET)

# ?‹¤?–‰ ?ŒŒ?¼ ë¹Œë“œ ê·œì¹™
$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS)

# ?´ë¦? ê·œì¹™
clean:
	rm -f $(TARGET)

.PHONY: all clean
