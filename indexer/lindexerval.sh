#!/bin/bash

valgrind -v --leak-check=full --track-origins=yes lindexer pages-depth3 lindexnm3 3
