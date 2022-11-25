#!/bin/sh
# assumes root is defined as CMAKE_SOURCE_DIR
echo "Running Coverage assuming source root = " ${ROOT}

MOC_FILES=$(find $PWD -type f \( -name "moc_*.h" -o -name "moc_*.cpp" -o -name "*.moc" \) | tr '\n' ' ' )

lcov -c -i -b ${ROOT} -d . --no-external -o Coverage.baseline
./unit_tests
./virtual_machine_gui_test
lcov -c -b ${ROOT} -d . --no-external  -o Coverage.out
lcov -a Coverage.baseline -a Coverage.out -o Coverage.combined
echo "-----------------------------------------------------------------"
lcov -r Coverage.combined ${ROOT}/catch.hpp $MOC_FILES $PWD/test_config.hpp -o Coverage.combined.cleaned
echo "-----------------------------------------------------------------"
genhtml Coverage.combined.cleaned -o Coverage_Report
