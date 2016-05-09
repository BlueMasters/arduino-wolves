#!/bin/bash

find -E lib src -regex ".*(h|cpp|ino)" -print | while read f; do
	uncrustify -c uncrustiy.cfg --no-backup -l cpp "$f"
done
