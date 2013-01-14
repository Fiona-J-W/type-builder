#! /bin/sh

####################
#Settings:


CFLAGS=" -O3 -Wall -Wextra -pedantic -std=c++11"
BINDIR="bin"
SRC_DIR="src/test/"

####################

for FILE in $(find $SRC_DIR -type f -iname "*.cpp"); do
	BASENAME=$(basename -s ".cpp" $FILE)
	TARGET="$BINDIR/BASENAME"
	if [[ ($FILE -ot $TARGET) ]]; then
		echo "compiling $BASENAME "
		$CXX $CFLAGS $FILE -o "$BINDIR/$BASENAME" 2>&1 > /dev/null | sed "s/^/\t/"
		echo "done"
	fi
done
#$CXX $CFLAGS 
