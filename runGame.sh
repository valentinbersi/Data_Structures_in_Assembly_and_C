#!/usr/bin/env bash
reset

command -v valgrind > /dev/null
if [ $? -ne 0 ]; then
  echo "ERROR: Valgrind not found."
  exit 1
fi

make game
if [ $? -ne 0 ]; then
  echo "ERROR: Compilation error."
  exit 1
fi

valgrind --show-reachable=yes --leak-check=full --error-exitcode=1 ./game 0
if [ $? -ne 0 ]; then
  echo "  **Memory error"
  exit 1
fi

echo " "
echo "**Running differences between Lists and Arrays"

DIFFER="diff -d"
ERRORDIFF=0

$DIFFER gameResultArray.txt gameResultList.txt > /tmp/diff
if [ $? -ne 0 ]; then
  echo "  **Discrepancy between Lists and Arrays"
  ERRORDIFF=1
fi

