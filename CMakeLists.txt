cmake_minimum_required(VERSION 3.25)
project(Labs_2course C)

set(CMAKE_C_STANDARD 11)

include_directories(mathpractic/lab2)
include_directories(mathpractic/lab3)
include_directories(mathpractic/lab4)
include_directories(mathpractic/lab4/task1)
include_directories(mathpractic/lab4/task2)

add_executable(Labs_2course
        mathpractic/lab1/task1/lab1_1.c
        mathpractic/lab1/task10/lab1_10.c
        mathpractic/lab1/task2/lab1_2.c
        mathpractic/lab1/task3/lab1_3.c
        mathpractic/lab1/task4/lab1_4.c
        mathpractic/lab1/task5/lab1_5.c
        mathpractic/lab1/task6/lab1_6.c
        mathpractic/lab1/task7/lab1_7.c
        mathpractic/lab1/task8/lab1_8.c
        mathpractic/lab1/task9/lab1_9.c
        mathpractic/lab2/task1/lab2_1.c
        mathpractic/lab2/task10/lab2_10.c
        mathpractic/lab2/task2/lab2_2.c
        mathpractic/lab2/task3/lab2_3.c
        mathpractic/lab2/task4/lab2_4.c
        mathpractic/lab2/task5/lab2_5.c
        mathpractic/lab2/task7/lab2_7.c
        mathpractic/lab2/task8/lab2_8.c
        mathpractic/lab2/task9/lab2_9.c
        mathpractic/lab2/header.h
        mathpractic/lab3/task1/lab3_1.c
        mathpractic/lab3/task10/lab3_10.c
        mathpractic/lab3/task2/lab3_2.c
        mathpractic/lab3/task3/lab3_3.c
        mathpractic/lab3/task4/lab3_4.c
        mathpractic/lab3/task4/lab3_4_1.c
        mathpractic/lab3/task5/lab3_5.c
        mathpractic/lab3/task7/lab3_7.c
        mathpractic/lab3/task7/list.c
        mathpractic/lab3/header.h
        mathpractic/lab4/task1/header.h
        mathpractic/lab4/task1/lab4_1.c
        mathpractic/lab4/task10/4_10.c
        mathpractic/lab4/task10/expression_tree.c
        mathpractic/lab4/task10/stack.c
        mathpractic/lab4/task2/header.h
        mathpractic/lab4/task2/lab4_2.c
        mathpractic/lab4/task5/lab4_5.c
        mathpractic/lab4/task5/stack.c
        mathpractic/lab4/task6/lab4_6.c
        mathpractic/lab4/task6/tree.c
        mathpractic/lab4/task7/lab4_7.c
        mathpractic/lab4/task9/BinaryHeap.c
        mathpractic/lab4/task9/BinomialHeap.c
        mathpractic/lab4/task9/lab4_9.c
        mathpractic/lab4/header.h
        OSI/lab1/task1/osi_lab1_1.c
        OSI/lab1/task2/osi_lab1_2.c
        OSI/lab1/task3/osi_lab1_3.c
        OSI/lab1/task4/osi_lab1_4.c
        OSI/lab2/task1/lab2_1.c
        OSI/lab2/task2/lab2_2.c
        OSI/lab2/task3/lab2_3.c
        OSI/lab2/task3/lab2_3_1.c
        OSI/lab2/task4/lab2_4.c
        OSI/lab2/task5/lab2_5.c
        OSI/lab3/task1/client.c
        OSI/lab3/task1/server.c
        OSI/lab3/task5/lab3_5.c mathpractic/lab4/task10/bor.c)
