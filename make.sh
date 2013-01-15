#! /bin/sh

####################
#Settings:


CFLAGS=" -O3 -Wall -Wextra -pedantic -std=c++11"
BINDIR="bin"
SRC_DIR="src/test/"

####################

for FILE in $(find $SRC_DIR -type f -iname "*.cpp"); do
	BASENAME=$(basename -s ".cpp" $FILE)
	TARGET="$BINDIR/$BASENAME"
	if [[ ! -f $TARGET || $FILE -nt $TARGET ]]; then
		echo "compiling $BASENAME -> $TARGET"
		$CXX $CFLAGS $FILE -o $TARGET 2>&1 > /dev/null | sed "s/^/\t/"
		echo "done"
	else
		echo "skipping $FILE"
	fi
done
#$CXX $CFLAGS 
