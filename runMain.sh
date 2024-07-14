#!/usr/bin/env bash
reset

command -v valgrind > /dev/null
if [ $? -ne 0 ]; then
  echo "ERROR: Valgrind not found."
  exit 1
fi

make main
if [ $? -ne 0 ]; then
  echo "ERROR: Compilation error."
  exit 1
fi

valgrind --show-reachable=yes --leak-check=full --error-exitcode=1 ./main
if [ $? -ne 0 ]; then
  echo "  **Memory error"
  exit 1
fi

