#!/usr/bin/env bash
reset

echo " "
echo "**Compiling"

make intensive_tester
if [ $? -ne 0 ]; then
  echo "  **Compilation error"
  exit 1
fi

DIFFER="diff -d"
ERRORDIFF=0

./intensive_tester int -o real.output.int.txt

$DIFFER real.output.int.txt expected.output.int.txt > /tmp/diff1
if [ $? -ne 0 ]; then
  echo "  **Discrepancy with type int"
  cat /tmp/diff1
  ERRORDIFF=1
fi

./intensive_tester string -o real.output.string.txt

$DIFFER real.output.string.txt expected.output.string.txt > /tmp/diff2
if [ $? -ne 0 ]; then
  echo "  **Discrepancy with type string"
  cat /tmp/diff2
  ERRORDIFF=1
fi

./intensive_tester array -o real.output.array.txt

$DIFFER real.output.array.txt expected.output.array.txt > /tmp/diff2
if [ $? -ne 0 ]; then
  echo "  **Discrepancy with type array"
  cat /tmp/diff2
  ERRORDIFF=1
fi

./intensive_tester list -o real.output.list.txt

$DIFFER real.output.list.txt expected.output.list.txt > /tmp/diff2
if [ $? -ne 0 ]; then
  echo "  **Discrepancy with type list"
  cat /tmp/diff2
  ERRORDIFF=1
fi

./intensive_tester card -o real.output.card.txt

$DIFFER real.output.card.txt expected.output.card.txt > /tmp/diff2
if [ $? -ne 0 ]; then
  echo "  **Discrepancy with type card"
  cat /tmp/diff2
  ERRORDIFF=1
fi

./intensive_tester game -o real.output.game.txt

$DIFFER real.output.game.txt expected.output.game.txt > /tmp/diff2
if [ $? -ne 0 ]; then
  echo "  **Discrepancy with game test"
  cat /tmp/diff2
  ERRORDIFF=1
fi


echo " "
if [ $ERRORDIFF -eq 0 ]; then
  echo "**Every intensive test passed successfully"
else
  exit 1
fi


echo " "
echo "**Running Valgrind"

command -v valgrind > /dev/null
if [ $? -ne 0 ]; then
  echo "ERROR: Valgrind not found."
  exit 1
fi

valgrind --show-reachable=yes --leak-check=full --error-exitcode=1 ./intensive_tester int string array list card game -o /dev/null
if [ $? -ne 0 ]; then
  echo "  **Memory error"
  exit 1
fi

echo " "
echo "**Memory test of intensive test OK"



