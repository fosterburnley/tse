#!/bin/bash

valgrind -v --leak-check=full --track-origins=yes indexer 82
