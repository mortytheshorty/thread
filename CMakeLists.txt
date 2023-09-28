cmake_minimum_required(VERSION 3.0.0)

project("thread")

add_library(${PROJECT_NAME} "src/task.c" "src/taskqueue.c" "src/thread.c" "src/thread_worker.c" "src/threadpool.c")
target_include_directories(${PROJECT_NAME} PUBLIC "src/include")
target_compile_options(${PROJECT_NAME} PUBLIC "-Wall" "-g" "-v")

add_executable("thread_test" "test.c")
target_link_libraries("thread_test" ${PROJECT_NAME})
