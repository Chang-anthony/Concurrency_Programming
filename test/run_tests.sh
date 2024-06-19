#!/bin/bash
cd ..
cd ..

cd ./build
echo "default: $(pwd)"
echo "Executable files in the current directory:"

# regex="Test"
# find . -type f -executable -exec grep Test {} \; -exec {} \;

desiredExecutable=Test

if [ -x "$desiredExecutable" ]; then
    "./$desiredExecutable"
else
    echo "The desired executable \"$desiredExecutable\" does not exist in the current directory."
fi

if [ $? -eq 0 ]; then
    echo "Tests passed successfully."
else
    echo "Tests failed."
fi