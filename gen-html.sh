#!/bin/bash
cd "$(dirname "$0")"
pandoc -s --template template.html --metadata title="psexe2rom" readme.md -o readme.html
