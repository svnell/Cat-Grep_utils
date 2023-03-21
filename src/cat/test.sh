#!/bin/bash 

file=$"test_case_cat.txt"
#file=$"123.txt"

for flag in b e n s t v
do  
    ./s21_cat -$flag $file > test_s21_cat.log
    cat -$flag $file > test_sys_cat.log
    RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    if [ "$RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
        echo "Test $flag SUCCESS"
    else
        echo "Test $flag FAIL"
    fi
done

rm test_s21_cat.log test_sys_cat.log