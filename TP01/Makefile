CC=gcc
FLAGS=-Wall -g

TARGET_version = testVersion
SRC_version=version.c testVersion.c
OBJ_version=$(SRC_version:%.c=%.o)

TARGET_commit = testCommit
SRC_commit=commit.c testCommit.c
OBJ_commit=$(SRC_commit:%.c=%.o)

all: $(TARGET_commit) $(TARGET_version)

$(TARGET_version): $(OBJ_version)
	$(CC) -o $@ $^ $(FLAGS)

%.o:%.c
	$(CC) -c -o $@ $^ $(FLAGS)

$(TARGET_commit): $(OBJ_commit)
	$(CC) -o $@ $^ $(FLAGS)


clean:
	rm -rf $(OBJ_version) $(TARGET_version)
	rm -rf $(OBJ_commit) $(TARGET_commit)
