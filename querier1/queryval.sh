#!/bin/bash

valgrind -v --leak-check=full --track-origins=yes querier pages-depth3 indexnm82 -q inputquery1.txt outputquery.txt
