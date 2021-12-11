#! /bin/sh

echo "#include <new>\n
$1 {}" | g++ -x c++ -S - -o- | grep "^_.*:" | sed -e 's/:.*$//'

