cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME sort_test)
project(${PROJECT_NAME})

# Data files
file(GLOB INPUT_FILES ${PROJECT_SOURCE_DIR}/etc/*.txt)
list(JOIN INPUT_FILES " " TEST_DATA)

# Tests
enable_testing()
add_test(
    NAME sort
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test.sh $<TARGET_FILE:sort> ${TEST_DATA}"
    )
add_test(
    NAME sort-k1
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test-k1.sh $<TARGET_FILE:sort> ${TEST_DATA}"
    )
add_test(
    NAME sort-k2
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test-k2.sh $<TARGET_FILE:sort> ${TEST_DATA}"
    )
add_test(
    NAME sort-k3
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test-k3.sh $<TARGET_FILE:sort> ${TEST_DATA}"
    )
add_test(
    NAME sort-k1-t-
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test-k1-t-.sh $<TARGET_FILE:sort> ${TEST_DATA}"
    )
add_test(
    NAME sort-k2-t-
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test-k2-t-.sh $<TARGET_FILE:sort> ${TEST_DATA}"
    )
