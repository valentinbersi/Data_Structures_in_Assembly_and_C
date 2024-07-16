# Data_Structures_in_Assembly
 Implementation of some data structures in assembly

This was a task from the subject Computer Organization.
It consisted of implementing functions for integers and a linked list in C,
and an array, functions for strings and a spanish card/deck in asm.

The task came with testing scripts:
* runGame.sh
* runIntensiveTests.sh
* runMain.sh
* runUnitTests.sh

The runGame.sh script runs a simulation of a single player game with a deck
to test if the stacking is working fine.

The intensiveTests.sh runs intensive tests on the data structures, testing
intensively all its features.

The runMain.sh runs a custom test that was part of the task.
This test stacks some cards and tests the linked list and the vector.

The runUnitTests.sh runs unit tests for all the data structures, these are
really basic tests and test minimum functionality.

All tests run valgrind when finished.