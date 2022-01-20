#!/bin/bash

for filename in test_*; do
    echo " "
    echo "------------------ ${filename} ---------------------"
    ./${filename}
done
