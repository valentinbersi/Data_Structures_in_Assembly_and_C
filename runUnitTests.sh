#!/usr/bin/env bash
reset

echo " "
echo "**Compiling"

make unit_tester
if [ $? -ne 0 ]; then
  echo "  **Compilation error"
  exit 1
fi

./unit_tester
if [ $? -ne 0 ]; then
  echo "  **One or more tests failed"
  exit 1
fi

echo "**Every unit test passed successfully"
echo " "

echo " "
echo "**Running Valgrind"

command -v valgrind > /dev/null
if [ $? -ne 0 ]; then
  echo "ERROR: Valgrind not found."
  exit 1
fi

valgrind --show-reachable=yes --leak-check=full --error-exitcode=1 ./unit_tester > /dev/null
if [ $? -ne 0 ]; then
  echo "  **Memory error"
  exit 1
fi


echo "**Memory test of unit tests OK"
echo " "

