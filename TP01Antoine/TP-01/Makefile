CC=gcc
FLAGS=-Wall -g

# Chaque cible génère un exécutable du même nom :

# testVersion pour l'exercice 1
# testEx2 pour l'exercice 2
# testCommit pour l'exercice 3
# testHeadCommit pour l'exercice 4


# Version
SRC_VERSION = version.c

# Commit
SRC_COMMIT = commit.c

# Commit avec list_head
SRC_HEAD_COMMIT = headCommit.c

# Test version (Ex1)
TARGET_EX1 = testVersion
SRC_EX1 = $(SRC_VERSION) testVersion.c
OBJ_EX1 = $(SRC_EX1:%.c=%.o)

# Test soft commit (Ex2)
TARGET_EX2 = testEx2
SRC_EX2 = $(SRC_VERSION) testEx2.c
OBJ_EX2 = $(SRC_EX2:%.c=%.o)

# Test listes doublement chainées (Ex3)
TARGET_EX3 = testCommit
SRC_EX3 = $(SRC_VERSION) $(SRC_COMMIT) testCommit.c
OBJ_EX3 = $(SRC_EX3:%.c=%.o)

# Test avec list_head (Ex4)
TARGET_EX4 = testHeadCommit
SRC_EX4 = $(SRC_VERSION) $(SRC_HEAD_COMMIT) testHeadCommit.c
OBJ_EX4 = $(SRC_EX4:%.c=%.o)


# All : compiler tous les tests
all: $(TARGET_EX1) $(TARGET_EX2) $(TARGET_EX3) $(TARGET_EX4)

$(TARGET_EX1): $(OBJ_EX1)
	$(CC) -o $@ $^ $(FLAGS)

$(TARGET_EX2): $(OBJ_EX2)
	$(CC) -o $@ $^ $(FLAGS)

$(TARGET_EX3): $(OBJ_EX3)
	$(CC) -o $@ $^ $(FLAGS)

$(TARGET_EX4): $(OBJ_EX4)
	$(CC) -o $@ $^ $(FLAGS)

# Tous les fichiers .o
%.o:%.c
	$(CC) -c -o $@ $< $(FLAGS)

clean:
	rm -f $(OBJ_EX1) $(TARGET_EX1) \
	$(OBJ_EX2) $(TARGET_EX2) \
	$(OBJ_EX3) $(TARGET_EX3) \
	$(OBJ_EX4) $(TARGET_EX4)

# Les dépendances
commit.o: commit.h
version.o: version.h
headCommit.o: headCommit.h list.h
