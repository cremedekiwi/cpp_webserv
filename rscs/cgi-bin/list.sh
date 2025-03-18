#!/bin/bash
ls | jq -R '{filenumber:input_line_number, name:.}' | jq -s .
