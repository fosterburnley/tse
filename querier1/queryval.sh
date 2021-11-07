#!/bin/bash

valgrind -v --leak-check=full querier pages-depth3 indexnm82 -q inputquery1.txt outputquery.txt
