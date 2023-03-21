#!/bin/bash 

SUCCESS=0
FAIL=0
COUNTER=0

file1=$"Makefile"
file=$"s21_grep.c"
file2=$"s21_grep.h"
reg=$"int"


    for flag in i v c l n h s o e f
    do  
        ./s21_grep -$flag $reg $file > test_s21_grep.log
        grep -$flag $reg $file > test_sys_grep.log
        RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
        (( COUNTER++ ))
        if [ "$RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
        then
        (( SUCCESS++ ))
            echo "\033[0;39m Test  $flag \033[1;35mSUCCESS"
        else
        (( FAIL++ ))
            echo "\033[0;39m Test  $flag \033[1;91mFAIL"
            ./s21_grep  -$flag $reg $file $file1 $file2
            echo "__________"
            grep  -$flag $reg $file $file1 $file2
        fi
    done

    for flag in i v c l n h s o e f
    do  
       ./s21_grep -$flag $reg $file $file1 $file2> test_s21_grep.log
        grep -$flag $reg $file $file1 $file2> test_sys_grep.log
        RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
        (( COUNTER++ ))
        if [ "$RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
        then
        (( SUCCESS++ ))
            echo "\033[0;39mTest  $flag 3 files \033[1;35mSUCCESS"
        else
        (( FAIL++ ))
            echo "\033[0;39mTest  $flag 3 files \033[1;91mFAIL"
            ./s21_grep  -$flag $reg $file $file1 $file2
            echo "__________"
            grep  -$flag $reg $file $file1 $file2
            echo "./s21_grep  -$flag $reg $file $file1 $file2"
            echo "grep  -$flag $reg $file $file1 $file2"
        fi
    done


for flag1 in i v c l n h s o f
do  
    for flag in i v c l n h s o
    do  
        ./s21_grep -$flag1 -$flag $reg $file > test_s21_grep.log
        grep -$flag1 -$flag $reg $file > test_sys_grep.log
        RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
        (( COUNTER++ ))
        if [ "$RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
        then
        (( SUCCESS++ ))
            echo "\033[0;39mTest $flag1 $flag \033[1;35mSUCCESS"
        else
        (( FAIL++ ))
            echo "\033[0;39mTest $flag1 $flag \033[1;91mFAIL"
            ./s21_grep -$flag1 -$flag $reg $file
            grep -$flag1 -$flag $reg $file
        fi
    done
done

for flag1 in i v c l n h s o
do  
    for flag in i v c l n h s o
    do  
        ./s21_grep -$flag1 -$flag $reg $file $file1> test_s21_grep.log
        grep -$flag1 -$flag $reg $file $file1> test_sys_grep.log
        RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
        (( COUNTER++ ))
        if [ "$RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
        then
        (( SUCCESS++ ))
            echo "\033[0;39mTest $flag1 $flag 2 files \033[1;35mSUCCESS"
        else
        (( FAIL++ ))
            echo "\033[0;39mTest $flag1 $flag 2 files \033[1;91mFAIL"
            ./s21_grep -$flag1 -$flag $reg $file $file1
            echo "__________"
            grep -$flag1 -$flag $reg $file $file1
            echo "./s21_grep -$flag1 -$flag $reg $file $file1"
            echo "grep -$flag1 -$flag $reg $file $file1"
        fi
    done
done



echo "\n"
echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"


rm test_s21_grep.log test_sys_grep.log