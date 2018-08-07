CC = g++
CFLAGS = -g -Wall
RM = rm -f
TARGET = testModel

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp DataModelDesc.cpp ColumnDesc.cpp TableRef.cpp TableDesc.cpp TblColRef.cpp ModelDimensionDesc.cpp -o $(TARGET) 

clean:
	$(RM) $(TARGET)